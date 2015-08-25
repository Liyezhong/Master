/****************************************************************************/
/*! \file ProgramSelfTest.cpp
 *
 *  \brief CProgramSelfTest class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ Aug 18th, 2014
 *   $Author:  $ Shufa Li
 *
 *  \b Company:
 *
 *       Leica Biosystems SH CN.
 *
 *  (C) Copyright 2014 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Scheduler/Include/ProgramSelfTest.h"
#include "Global/Include/Utils.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/HeatingStrategy.h"
#include "Scheduler/Commands/Include/CmdALTurnOnFan.h"
#include "Scheduler/Commands/Include/CmdALTurnOffFan.h"
#include "Scheduler/Commands/Include/CmdALControlValve.h"
#include "Scheduler/Commands/Include/CmdRVSetTemperatureSwitchState.h"
#include "Scheduler/Commands/Include/CmdRTSetTemperatureSwitchState.h"
#include "Scheduler/Commands/Include/CmdALPressure.h"
#include "Scheduler/Commands/Include/CmdALReleasePressure.h"
#include "Scheduler/Include/SchedulerEventCodes.h"

namespace Scheduler{
/*lint -e534 */

CProgramSelfTest::CProgramSelfTest(SchedulerMainThreadController* SchedController)
    :mp_SchedulerThreadController(SchedController)
    ,m_CurrentState(SELFTEST_AC_VOLTAGE)
    ,m_StateACVoltageStep(SET_VOLTAGE_ASB3_SWITCH)
    ,m_StateDCHeatingStep(STARTHEATING_LATUBE1)
    ,m_StatePressureStep(START_PUMP)
    ,m_StateACHeatingStep(STARTHEATING_RETORTTOP)
    ,m_DelayBeginTime(0)
    ,m_StartReq(0)
    ,m_StateACVoltageStepCount(0)
    ,m_IsSelfTestDone(false)
    ,m_ASB3SwitchType(0)
    ,m_ASB5SwitchType(0)
    ,m_IsLoged(0)
{
    CONNECTSIGNALSLOT(this, SigSelfTestDone(bool), mp_SchedulerThreadController, OnSelfTestDone(bool));
}

CProgramSelfTest::~CProgramSelfTest()
{
}

void CProgramSelfTest::SendSignalSelfTestDone(bool flag)
{
    if(flag)
    {
        m_CurrentState = SELFTEST_UNDF;
        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_SUCCESSFULLY);
        emit SigSelfTestDone(true);
    }
    else
    {
        emit SigSelfTestDone(false);
    }
    m_IsSelfTestDone = true;
}

void CProgramSelfTest::HandleWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    switch(m_CurrentState)
    {
        case SELFTEST_AC_VOLTAGE:
            HandleStateACVoltage(cmdName, retCode);
            break;
        case SELFTEST_DC_HEATING:
            HandleStateDCHeating();
            break;
        case SELFTEST_PRESSURE_MODE:
            HandlePressureMode(cmdName, retCode);
            break;
        case SELFTEST_AC_HEATING:
            HandleStateACHeating();
            break;
        default:
            break;
    }
}

void CProgramSelfTest::HandleStateACVoltage(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    qint64 nowTime = 0;
    ReturnCode_t ret = DCL_ERR_FCT_CALL_SUCCESS;

    /*lint -e616 */
    switch(m_StateACVoltageStep)
    {
        case SET_VOLTAGE_ASB3_SWITCH:
            if(0 == m_StartReq)
            {
                CmdRVSetTemperatureSwitchState* cmd = new CmdRVSetTemperatureSwitchState(500, mp_SchedulerThreadController);
                cmd->SetHeaterVoltage(-1);
                cmd->SetAutoType(AUTO_SWITCH_ENABLE);
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
                m_StartReq++;
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_ASB3VOLTAGE_SWITCHAUTO);
            }
            else
            {
                if("Scheduler::RVSetTemperatureSwitchState" == cmdName)
                {
                    if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                    {
                        m_StateACVoltageStep = SET_VOLTAGE_ASB5_SWITCH;
                        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_ASB3VOLTAGE_SWITCHAUTO_SUCCESS);
                    }
                    else
                    {
                        if(mp_SchedulerThreadController->RaiseError(0, retCode, 2, true))
                            SendSignalSelfTestDone(false);
                    }
                    m_StartReq = 0;
                }
            }
            break;
        case SET_VOLTAGE_ASB5_SWITCH:
            if(0 == m_StartReq)
            {
                CmdRTSetTemperatureSwitchState* cmd = new CmdRTSetTemperatureSwitchState(500, mp_SchedulerThreadController);
                cmd->SetType(RT_BOTTOM);
                cmd->SetHeaterVoltage(-1);
                cmd->SetAutoType(AUTO_SWITCH_ENABLE);
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
                m_StartReq++;
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_ASB5VOLTAGE_SWITCHAUTO);
            }
            else
            {
                if("Scheduler::RTSetTemperatureSwitchState" == cmdName)
                {
                    if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                    {
                        mp_SchedulerThreadController->RaiseEvent(EVENT_SHCEDULER_ASB5VOLTAGE_SWITCHAUTO_SUCESS);
                        m_StateACVoltageStep = START_HEATING_ACMODE;
                    }
                    else
                    {
                        if(mp_SchedulerThreadController->RaiseError(0, retCode, 2, true))
                            SendSignalSelfTestDone(false);
                    }
                    m_StartReq = 0;
                }
            }
            break;
        case START_HEATING_ACMODE:
            ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForSelfTest("RV", true);
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_ROTARYVALVE_TEMP);
            if(DCL_ERR_FCT_CALL_SUCCESS == ret)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_ROTARYVALVE_TEMP_SUCCESS);
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForSelfTest("RTBottom", true);
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_RETORTBOTTOM_TEMP);
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    m_StateACVoltageStep = STOP_HEATING_ACMODE;
                    m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_RETORTBOTTOM_TEMP_SUCCESS);
                }
                else
                {
                    if(mp_SchedulerThreadController->RaiseError(0, retCode, 2, true))
                        SendSignalSelfTestDone(false);
                }
            }
            else
            {
                if(mp_SchedulerThreadController->RaiseError(0, retCode, 2, true))
                    SendSignalSelfTestDone(false);
            }
            break;
        case STOP_HEATING_ACMODE:
            if(0 == m_IsLoged)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_DELAY_5S);
                m_IsLoged++;
            }
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 5 * 1000)
            {
                m_IsLoged = 0;
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("RV");
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_ROTARYVALVE_TEMP);
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_ROTARYVALVE_TEMP_SUCCESS);
                    ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("RTBottom");
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_RETORTBOTTOM_TEMP);
                    if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                    {
                        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_RETORTBOTTOM_TEMP_SUCCESS);
                        m_StateACVoltageStep = CHECK_VOLTAGE_RANGE;
                    }
                    else
                    {
                        if(mp_SchedulerThreadController->RaiseError(0, retCode, 2, true))
                            SendSignalSelfTestDone(false);
                    }
                }
                else
                {
                    if(mp_SchedulerThreadController->RaiseError(0, retCode, 2, true))
                        SendSignalSelfTestDone(false);
                }
            }
            break;
        case CHECK_VOLTAGE_RANGE:
            m_ASB5SwitchType = mp_SchedulerThreadController->GetSchedCommandProcessor()->GetHeaterSwitchType("Retort");
            m_ASB3SwitchType = mp_SchedulerThreadController->GetSchedCommandProcessor()->GetHeaterSwitchType("RV");
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_ASB5_ASB3_SWITCHTYPE, QStringList()<<QString::number(m_ASB5SwitchType)<<QString::number(m_ASB3SwitchType));

            if(0 == m_StateACVoltageStepCount)
            {
                //first time
                if( (1 == m_ASB5SwitchType || 2 == m_ASB5SwitchType) && (m_ASB5SwitchType == m_ASB3SwitchType) )
                {
                    m_StateACVoltageStep = VOLTAGE_TEST_DONE;
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_ASB5_SAME_ASB3);
                }
                else
                {
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_RETRY_AC_VOLTAGE_TEST);
                    m_StateACVoltageStepCount++;
                    m_StateACVoltageStep = SET_VOLTAGE_ASB3_SWITCH;
                }
            }
            else
            {
                //the secod time
                if( 1 == m_ASB5SwitchType || 2 == m_ASB5SwitchType)
                {
                    if(m_ASB5SwitchType == m_ASB3SwitchType)
                    {
                        // is the same pass
                        m_StateACVoltageStep = VOLTAGE_TEST_DONE;
                        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_ASB5_SAME_ASB3);
                    }
                    else
                    {
                        m_StateACVoltageStep = CHECK_VOLTAGE_RANGE_AGAIN;
                    }
                    m_StateACVoltageStepCount = 0;
                }
                else
                {
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_UNKNOW_ASB5_SWITCHTYPE);
                    m_StateACVoltageStep = VOLTAGE_TEST_DONE;
                    m_StateACVoltageStepCount = 0;
                }
            }
            break;
        case CHECK_VOLTAGE_RANGE_AGAIN:
            if(0 == m_StartReq)
            {
                CmdRVSetTemperatureSwitchState* cmd = new CmdRVSetTemperatureSwitchState(500, mp_SchedulerThreadController);
                cmd->SetAutoType(AUTO_SWITCH_DISABLE);
                cmd->SetHeaterVoltage(m_ASB5SwitchType);
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
                m_StartReq++;
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_SET_ASB3VOLTAGE_WITH_ASB5);
            }
            else
            {
                if("Scheduler::RVSetTemperatureSwitchState" == cmdName)
                {
                    if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                    {
                        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_SET_ASB3VOLTAGE_WITH_ASB5_SUCCESS);
                        m_StateACVoltageStep = VOLTAGE_TEST_DONE;
                    }
                    else
                    {
                        if(mp_SchedulerThreadController->RaiseError(0, retCode, 2, true))
                            SendSignalSelfTestDone(false);
                    }
                    m_StartReq = 0;
                }
            }
            break;
        case VOLTAGE_TEST_DONE:
            ResetVarList();
            m_CurrentState = SELFTEST_DC_HEATING;
            break;
        default:
            break;
    }

}

void CProgramSelfTest::HandleStateDCHeating()
{
    qint64 nowTime = 0;
    ReturnCode_t ret = DCL_ERR_FCT_CALL_SUCCESS;

    switch(m_StateDCHeatingStep)
    {
        case STARTHEATING_LATUBE1:
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_TUBE1_TEMP);
            ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForSelfTest("LA_Tube1");
            if(DCL_ERR_FCT_CALL_SUCCESS == ret)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_TUBE1_TEMP_SUCCESS);
                m_StateDCHeatingStep = STOPHEATING_LATUBE1;
                m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
            }
            else
            {
                if(mp_SchedulerThreadController->RaiseError(0, ret, 2, true))
                    SendSignalSelfTestDone(false);
            }
            break;
        case STOPHEATING_LATUBE1:
            if(0 == m_IsLoged)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_DELAY_3S);
                m_IsLoged++;
            }
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 3 * 1000)
            {
                m_IsLoged = 0;
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_TUBE1_TEMP);
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("LA_Tube1");
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_TUBE1_TEMP_SUCCESS);
                    m_StateDCHeatingStep = STARTHEATING_LATUBE2;
                }
                else
                {
                    if(mp_SchedulerThreadController->RaiseError(0, ret, 2, true))
                        SendSignalSelfTestDone(false);
                }
            }
            break;
        case STARTHEATING_LATUBE2:
            ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForSelfTest("LA_Tube2");
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_TUBE2_TEMP);
            if(DCL_ERR_FCT_CALL_SUCCESS == ret)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_TUBE2_TEMP_SUCCESS);
                m_StateDCHeatingStep = STOPHEATING_LATBUBE2;
                m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
            }
            else
            {
                if(mp_SchedulerThreadController->RaiseError(0, ret, 2, true))
                    SendSignalSelfTestDone(false);
            }
            break;
        case STOPHEATING_LATBUBE2:
            if(0 == m_IsLoged)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_DELAY_3S);
                m_IsLoged++;
            }
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 3 * 1000)
            {
                m_IsLoged = 0;
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("LA_Tube2");
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_TUBE2_TEMP);
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_TUBE2_TEMP_SUCCESS);
                    m_StateDCHeatingStep = STARTHEATING_LEVELSENSOR;
                }
                else
                {
                    if(mp_SchedulerThreadController->RaiseError(0, ret, 2, true))
                        SendSignalSelfTestDone(false);
                }
            }
            break;
        case STARTHEATING_LEVELSENSOR:
            ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForSelfTest("LevelSensor");
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_LEVELSENSOR_TEMP);
            if(DCL_ERR_FCT_CALL_SUCCESS == ret)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_LEVELSENSOR_TEMP_SUCCESS);
                m_StateDCHeatingStep = STOPHEATING_LEVELSENSOR;
                m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
            }
            else
            {
                if(mp_SchedulerThreadController->RaiseError(0, ret, 2, true))
                    SendSignalSelfTestDone(false);
            }
            break;
        case STOPHEATING_LEVELSENSOR:
            if(0 == m_IsLoged)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_DELAY_3S);
                m_IsLoged++;
            }
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 3 * 1000)
            {
                m_IsLoged = 0;
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("LevelSensor");
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_LEVELSENSOR_TEMP);
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_LEVELSENSOR_TEMP_SUCCESS);
                    ResetVarList();
                    m_CurrentState = SELFTEST_PRESSURE_MODE;
                }
                else
                {
                    if(mp_SchedulerThreadController->RaiseError(0, ret, 2, true))
                        SendSignalSelfTestDone(false);
                }
            }
            break;
        default:
            break;
        }

}

void CProgramSelfTest::HandlePressureMode(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    qint64 nowTime = 0;

    switch(m_StatePressureStep)
    {
        case START_PUMP:
            if(0 == m_StartReq)
            {
                CmdALPressure *cmdPressure = new CmdALPressure(500, mp_SchedulerThreadController);
                cmdPressure->SetTargetPressure(0.0);
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmdPressure);
                m_StartReq++;
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_PUMP);
            }
            else
            {
                if("Scheduler::ALPressure" == cmdName)
                {
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_PUMP_SUCCESS);
                    m_StatePressureStep = STOP_PUMP;
                    m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
                    m_StartReq = 0;
                }
            }
            break;
        case STOP_PUMP:
            if(0 == m_IsLoged)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_DELAY_3S);
                m_IsLoged++;
            }
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 3 * 1000)
            {

                if(0 == m_StartReq)
                {
                    mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, mp_SchedulerThreadController));
                    m_StartReq++;
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_PUMP);
                }
                else
                {
                    if("Scheduler::ALReleasePressure" == cmdName)
                    {
                        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_PUMP_SUCCESS);
                        m_StatePressureStep = START_VALVE1;
                        m_StartReq = 0;
                        m_IsLoged = 0;
                    }
                }
            }
            break;
        case START_VALVE1:
            if(0 == m_StartReq)
            {
                CmdALControlValve* cmd = new CmdALControlValve(500, mp_SchedulerThreadController);
                cmd->SetValveIndex(VALVE_1_INDEX);
                cmd->SetValveState(VALVE_STATE_OPEN);
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
                m_StartReq++;
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_VALVE1);
            }
            else
            {
                if("Scheduler::ALControlValve" == cmdName)
                {
                    if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                    {
                        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_VALVE1_SUCCESS);
                        m_StatePressureStep = STOP_VALVE1;
                        m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
                    }
                    else
                    {
                        if(mp_SchedulerThreadController->RaiseError(0, retCode, 2, true))
                            SendSignalSelfTestDone(false);
                    }
                    m_StartReq = 0;
                }
            }
            break;
        case STOP_VALVE1:
            if(0 == m_IsLoged)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_DELAY_3S);
                m_IsLoged++;
            }
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 3 * 1000)
            {
                if(0 == m_StartReq)
                {
                    CmdALControlValve* cmd = new CmdALControlValve(500, mp_SchedulerThreadController);
                    cmd->SetValveIndex(VALVE_1_INDEX);
                    cmd->SetValveState(VALVE_STATE_CLOSE);
                    mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
                    m_StartReq++;
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_VALVE1);
                }
                else
                {
                    if("Scheduler::ALControlValve" == cmdName)
                    {
                        if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                        {
                            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_VALVE1_SUCCESS);
                            m_StatePressureStep = START_VALVE2;
                        }
                        else
                        {
                            if(mp_SchedulerThreadController->RaiseError(0, retCode, 2, true))
                                SendSignalSelfTestDone(false);
                        }
                        m_StartReq = 0;
                        m_IsLoged = 0;
                    }
                }
            }
            break;
        case START_VALVE2:
            if(0 == m_StartReq)
            {
                CmdALControlValve* cmd = new CmdALControlValve(500, mp_SchedulerThreadController);
                cmd->SetValveIndex(VALVE_2_INDEX);
                cmd->SetValveState(VALVE_STATE_OPEN);
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
                m_StartReq++;
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_VALVE2);
            }
            else
            {
                if("Scheduler::ALControlValve" == cmdName)
                {
                    if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                    {
                        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_VALVE2_SUCCESS);
                        m_StatePressureStep = STOP_VALVE2;
                        m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
                    }
                    else
                    {
                        if(mp_SchedulerThreadController->RaiseError(0, retCode, 2, true))
                            SendSignalSelfTestDone(false);
                    }
                    m_StartReq = 0;
                }
            }
            break;
        case STOP_VALVE2:
            if(0 == m_IsLoged)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_DELAY_3S);
                m_IsLoged++;
            }
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 3 * 1000)
            {
                if(0 == m_StartReq)
                {
                    CmdALControlValve* cmd = new CmdALControlValve(500, mp_SchedulerThreadController);
                    cmd->SetValveIndex(VALVE_2_INDEX);
                    cmd->SetValveState(VALVE_STATE_CLOSE);
                    mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
                    m_StartReq++;
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_VALVE2);
                }
                else
                {
                    if("Scheduler::ALControlValve" == cmdName)
                    {
                        if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                        {
                            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_VALVE2_SUCCESS);
                            m_StatePressureStep = START_EXHAUSTFAN;
                        }
                        else
                        {
                            if(mp_SchedulerThreadController->RaiseError(0, retCode, 2, true))
                                SendSignalSelfTestDone(false);
                        }
                        m_StartReq = 0;
                        m_IsLoged = 0;
                    }
                }
            }
            break;
        case START_EXHAUSTFAN:
            if(0 == m_StartReq)
            {
                CmdALTurnOnFan* cmd = new CmdALTurnOnFan(500, mp_SchedulerThreadController);
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
                m_StartReq++;
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_FAN);
            }
            else
            {
                if("Scheduler::ALTurnOnFan" == cmdName)
                {
                    if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                    {
                        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_FAN_SUCCESS);
                        m_StatePressureStep = STOP_EXHAUSTFAN;
                        m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
                    }
                    else
                    {
                        if(mp_SchedulerThreadController->RaiseError(0, retCode, 2, true))
                            SendSignalSelfTestDone(false);
                    }
                    m_StartReq = 0;
                }
            }
            break;
        case STOP_EXHAUSTFAN:
            if(0 == m_IsLoged)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_DELAY_3S);
                m_IsLoged++;
            }
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 3 * 1000)
            {
                if(0 == m_StartReq)
                {
                    CmdALTurnOffFan* cmd = new CmdALTurnOffFan(500, mp_SchedulerThreadController);
                    mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
                    m_StartReq++;
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_FAN);
                }
                else
                {
                    if("Scheduler::ALTurnOffFan" == cmdName)
                    {
                        if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                        {
                            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_FAN_SUCCESS);
                            ResetVarList();
                            m_CurrentState = SELFTEST_AC_HEATING;
                        }
                        else
                        {
                            if(mp_SchedulerThreadController->RaiseError(0, retCode, 2, true))
                                SendSignalSelfTestDone(false);
                        }
                        m_StartReq = 0;
                        m_IsLoged = 0;
                    }
                }
            }
        default:
            break;
    }

}

void CProgramSelfTest::HandleStateACHeating()
{
    qint64 nowTime = 0;
    ReturnCode_t ret = DCL_ERR_FCT_CALL_SUCCESS;

    switch(m_StateACHeatingStep)
    {
        case STARTHEATING_RETORTTOP:
            ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForSelfTest("RTSide");
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_RETORTSIDE_TEMP);
            if(DCL_ERR_FCT_CALL_SUCCESS == ret )
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_RETORTSIDE_TEMP_SUCCESS);
                m_StateACHeatingStep = STOPHEATING_RETORTOP;
                m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
            }
            else
            {
                if(mp_SchedulerThreadController->RaiseError(0, ret, 2, true))
                    SendSignalSelfTestDone(false);
            }
            break;
        case STOPHEATING_RETORTOP:
            if(0 == m_IsLoged)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_DELAY_5S);
                m_IsLoged++;
            }
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 5 * 1000)
            {
                m_IsLoged = 0;
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("RTSide");
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_RETORTSIDE_TEMP);
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_RETORTSIDE_TEMP_SUCCESS);
                    m_StateACHeatingStep = STARTHEATING_RETORTBOTTOM;
                }
                else
                {
                    if(mp_SchedulerThreadController->RaiseError(0, ret, 2, true))
                        SendSignalSelfTestDone(false);
                }
            }
            break;
        case STARTHEATING_RETORTBOTTOM:
            ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForSelfTest("RTBottom");
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_RETORTBOTTOM_TEMP);
            if(DCL_ERR_FCT_CALL_SUCCESS == ret)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_RETORTBOTTOM_TEMP_SUCCESS);
                m_StateACHeatingStep = STOPHEATING_RETORTBOTTOM;
                m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
            }
            else
            {
                if(mp_SchedulerThreadController->RaiseError(0, ret, 2, true))
                    SendSignalSelfTestDone(false);
            }
            break;
        case STOPHEATING_RETORTBOTTOM:
            if(0 == m_IsLoged)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_DELAY_5S);
                m_IsLoged++;
            }
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 5 * 1000)
            {
                m_IsLoged = 0;
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("RTBottom");
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_RETORTBOTTOM_TEMP);
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_RETORTBOTTOM_TEMP_SUCCESS);
                    m_StateACHeatingStep = STARTHEATING_OVENTOP;
                }
                else
                {
                    if(mp_SchedulerThreadController->RaiseError(0, ret, 2, true))
                        SendSignalSelfTestDone(false);
                }
            }
            break;
        case STARTHEATING_OVENTOP:
            ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForSelfTest("OvenTop");
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_OVENTOP_TEMP);
            if(DCL_ERR_FCT_CALL_SUCCESS == ret)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_OVENTOP_TEMP_SUCCESS);
                m_StateACHeatingStep = STOPHEATING_OVENTOP;
                m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
            }
            else
            {
                if(mp_SchedulerThreadController->RaiseError(0, ret, 2, true))
                    SendSignalSelfTestDone(false);
            }
            break;
        case STOPHEATING_OVENTOP:
            if(0 == m_IsLoged)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_DELAY_5S);
                m_IsLoged++;
            }
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 5 * 1000)
            {
                m_IsLoged = 0;
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("OvenTop");
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_OVENTOP_TEMP);
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_OVENTOP_TEMP_SUCCESS);
                    m_StateACHeatingStep = STARTHEATING_OVENBOTTOM;
                }
                else
                {
                    if(mp_SchedulerThreadController->RaiseError(0, ret, 2, true))
                        SendSignalSelfTestDone(false);
                }
            }
            break;
        case STARTHEATING_OVENBOTTOM:
            ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForSelfTest("OvenBottom");
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_OVENBOT_TEMP);
            if(DCL_ERR_FCT_CALL_SUCCESS == ret)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_OVENBOT_TEMP_SUCCESS);
                m_StateACHeatingStep = STOPHEATING_OVENBOTTOM;
                m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
            }
            else
            {
                if(mp_SchedulerThreadController->RaiseError(0, ret, 2, true))
                    SendSignalSelfTestDone(false);
            }
            break;
        case STOPHEATING_OVENBOTTOM:
            if(0 == m_IsLoged)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_DELAY_5S);
                m_IsLoged++;
            }
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 5 * 1000)
            {
                m_IsLoged = 0;
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("OvenBottom");
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_OVENBOT_TEMP);
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_OVENBOT_TEMP_SUCCESS);
                    m_StateACHeatingStep = STARTHEATING_RV;
                }
                else
                {
                    if(mp_SchedulerThreadController->RaiseError(0, ret, 2, true))
                        SendSignalSelfTestDone(false);
                }
            }
            break;
        case STARTHEATING_RV:
            ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForSelfTest("RV");
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_ROTARYVALVE_TEMP);
            if(DCL_ERR_FCT_CALL_SUCCESS == ret)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_ROTARYVALVE_TEMP_SUCCESS);
                m_StateACHeatingStep = STOPHEATING_RV;
                m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
            }
            else
            {
                if(mp_SchedulerThreadController->RaiseError(0, ret, 2, true))
                    SendSignalSelfTestDone(false);
            }
            break;
        case STOPHEATING_RV:
            if(0 == m_IsLoged)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_DELAY_5S);
                m_IsLoged++;
            }
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 5 * 1000)
            {
                m_IsLoged = 0;
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("RV");
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_ROTARYVALVE_TEMP);
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_STOP_ROTARYVALVE_TEMP_SUCCESS);
                    // When temperature of RV Bottom is less than 25 degress, we need
                    // make higher target temperature and more heating time
                    mp_SchedulerThreadController->GetHeatingStrategy()->UpdateOvenParamAt110V();
                    SendSignalSelfTestDone(true);
                }
                else
                {
                    if(mp_SchedulerThreadController->RaiseError(0, ret, 2, true))
                        SendSignalSelfTestDone(false);
                }
            }
            break;
        default:
            break;
    }
}

}
