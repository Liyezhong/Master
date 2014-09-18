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

namespace Scheduler{

CProgramSelfTest::CProgramSelfTest(SchedulerMainThreadController* SchedController)
    :mp_SchedulerThreadController(SchedController)
    ,m_StateACVoltageStep(SET_VOLTAGE_ASB3_AWITCH)
    ,m_StateDCHeatingStep(STARTHEATING_LATUBE1)
    ,m_StatePressureStep(START_PUMP)
    ,m_StateACHeatingStep(STARTHEATING_RETORTTOP)
    ,m_DelayBeginTime(0)
    ,m_StartReq(0)
    ,m_StateACVoltageStepCount(0)
    ,m_ASB3SwitchType(0)
    ,m_ASB5SwitchType(0)
{
    mp_StateMachine = QSharedPointer<QStateMachine>(new QStateMachine());

    mp_Initial = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_StateACVoltage = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_StateDCHeating = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_StatePressure = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_StateACHeating = QSharedPointer<QState>(new QState(mp_StateMachine.data()));

    mp_StateMachine->setInitialState(mp_Initial.data());
    mp_Initial->addTransition(this, SIGNAL(SigACVoltageSwitch()), mp_StateACVoltage.data());
    mp_StateACVoltage->addTransition(this, SIGNAL(SigDCHeating()), mp_StateDCHeating.data());
    mp_StateDCHeating->addTransition(this, SIGNAL(SigPressureSensorPumpValve()), mp_StatePressure.data());
    mp_StatePressure->addTransition(this, SIGNAL(SigACHeating()), mp_StateACHeating.data());
    mp_StateACHeating->addTransition(this, SIGNAL(SigSelfTestDone(bool)), mp_Initial.data());

    CONNECTSIGNALSLOT(this, SigSelfTestDone(bool), mp_SchedulerThreadController, OnSelfTestDone(bool));

    mp_StateMachine->start();
}

CProgramSelfTest::~CProgramSelfTest()
{
    mp_StateMachine->stop();
}

void CProgramSelfTest::SendSignalSelfTestDone(bool flag)
{
    if(flag)
    {
        emit SigSelfTestDone(true);
    }
    else
    {
        emit SigSelfTestDone(false);
    }
    m_IsSelfTestDone = true;
}


CProgramSelfTest::StateList_t CProgramSelfTest::GetCurrentState(QSet<QAbstractState*> statesList)
{
    StateList_t currentState = SELFTEST_UNDF;
    if(statesList.contains(mp_Initial.data()))
    {
        currentState = SELFTEST_INIT;
    }
    else if(statesList.contains(mp_StateACVoltage.data()))
    {
        currentState = SELFTEST_AC_VOLTAGE;
    }
    else if(statesList.contains(mp_StateDCHeating.data()))
    {
        currentState = SELFTEST_DC_HEATING;
    }
    else if(statesList.contains(mp_StatePressure.data()))
    {
        currentState = SELFTEST_PRESSURE;
    }
    else if(statesList.contains(mp_StateACHeating.data()))
    {
        currentState = SELFTEST_AC_HEATING;
    }
    return currentState;
}

void CProgramSelfTest::HandleWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    StateList_t currentState = GetCurrentState(mp_StateMachine->configuration());

    if(SELFTEST_INIT == currentState)
    {
        emit SigACVoltageSwitch();
    }
    else if(SELFTEST_AC_VOLTAGE == currentState)
    {
        HandleStateACVoltage(cmdName, retCode);
    }
    else if(SELFTEST_DC_HEATING == currentState)
    {
        HandleStateDCHeating();
    }
    else if(SELFTEST_PRESSURE == currentState)
    {
        HandlePressure(cmdName, retCode);
    }
    else if(SELFTEST_AC_HEATING == currentState)
    {
        HandleStateACHeating();
    }
    else
    {
        mp_SchedulerThreadController->LogDebug("In ProgramSelfTest unknown state");
    }

}

void CProgramSelfTest::HandleStateACVoltage(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    qint64 nowTime = 0;

    ReturnCode_t ret = DCL_ERR_FCT_CALL_SUCCESS;

    switch(m_StateACVoltageStep)
    {
        case SET_VOLTAGE_ASB3_AWITCH:
            if(0 == m_StartReq)
            {
                CmdRVSetTemperatureSwitchState* cmd = new CmdRVSetTemperatureSwitchState(500, mp_SchedulerThreadController);
                cmd->SetHeaterVoltage(-1);
                cmd->SetAutoType(AUTO_SWITCH_ENABLE);
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
                m_StartReq++;
            }
            else
            {
                if("Scheduler::RVSetTemperatureSwitchState" == cmdName)
                {
                    mp_SchedulerThreadController->LogDebug(QString("Self-Test ASB3 switch state auto, the retCode:%1").arg(retCode));
                    if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                    {
                        m_StateACVoltageStep = SET_VOLTAGE_ASB5_AWITCH;
                    }
                    else
                    {
                        m_StateACVoltageStep = SET_VOLTAGE_ASB3_AWITCH;
                        mp_SchedulerThreadController->SendOutErrMsg(retCode);
                        SendSignalSelfTestDone(false);
                    }
                    m_StartReq = 0;
                }
            }
            break;
        case SET_VOLTAGE_ASB5_AWITCH:
            if(0 == m_StartReq)
            {
                CmdRTSetTemperatureSwitchState* cmd = new CmdRTSetTemperatureSwitchState(500, mp_SchedulerThreadController);
                cmd->SetType(RT_BOTTOM);
                cmd->SetHeaterVoltage(-1);
                cmd->SetAutoType(AUTO_SWITCH_ENABLE);
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
                m_StartReq++;
            }
            else
            {
                if("Scheduler::RTSetTemperatureSwitchState" == cmdName)
                {
                    mp_SchedulerThreadController->LogDebug(QString("Self-Test ASB5 switch auto, the retCode:%1").arg(retCode));
                    if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                    {
                        m_StateACVoltageStep = START_HEATING_ACMODE;
                    }
                    else
                    {
                        m_StateACVoltageStep = SET_VOLTAGE_ASB3_AWITCH;
                        mp_SchedulerThreadController->SendOutErrMsg(retCode);
                        SendSignalSelfTestDone(false);
                    }
                    m_StartReq = 0;
                }
            }
            break;
        case START_HEATING_ACMODE:
            ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForSelfTest("RV");
            mp_SchedulerThreadController->LogDebug(QString("Self-Test start RV temperature, the retCode:%1").arg(ret));
            if(DCL_ERR_FCT_CALL_SUCCESS == ret)
            {
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForSelfTest("RTBottom");
                mp_SchedulerThreadController->LogDebug(QString("Self-Test start RT temperature, the retCode:%1").arg(ret));
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    m_StateACVoltageStep = STOP_HEATING_ACMODE;
                    m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
                }
                else
                {
                    m_StateACVoltageStep = SET_VOLTAGE_ASB3_AWITCH;
                    mp_SchedulerThreadController->SendOutErrMsg(ret);
                    SendSignalSelfTestDone(false);
                }
            }
            else
            {
                m_StateACVoltageStep = SET_VOLTAGE_ASB3_AWITCH;
                mp_SchedulerThreadController->SendOutErrMsg(ret);
                SendSignalSelfTestDone(false);
            }
            break;
        case STOP_HEATING_ACMODE:
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 5 * 1000)
            {
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("RV");
                mp_SchedulerThreadController->LogDebug(QString("Self-Test stop RV temperature,the retCode:%1").arg(ret));
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("RTBottom");
                    mp_SchedulerThreadController->LogDebug(QString("Self-Test stop RT temperature,the retCode:%1").arg(ret));
                    if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                    {
                        m_StateACVoltageStep = CHECK_VOLTAGE_RANGE;
                    }
                    else
                    {
                        m_StateACVoltageStep = SET_VOLTAGE_ASB3_AWITCH;
                        mp_SchedulerThreadController->SendOutErrMsg(ret);
                        SendSignalSelfTestDone(false);
                    }
                }
                else
                {
                    m_StateACVoltageStep = SET_VOLTAGE_ASB3_AWITCH;
                    mp_SchedulerThreadController->SendOutErrMsg(ret);
                    SendSignalSelfTestDone(false);
                }
            }
            break;
        case CHECK_VOLTAGE_RANGE:
            m_ASB5SwitchType = mp_SchedulerThreadController->GetSchedCommandProcessor()->GetHeaterSwitchType("Retort");
            m_ASB3SwitchType = mp_SchedulerThreadController->GetSchedCommandProcessor()->GetHeaterSwitchType("RV");
            mp_SchedulerThreadController->LogDebug(QString("Self-Test the ASB5SwithcType:%1,ASB3SwitchType:%2").arg(m_ASB5SwitchType).arg(m_ASB3SwitchType));

            if(0 == m_StateACVoltageStepCount)
            {
                //first time
                if( (1 == m_ASB5SwitchType || 2 == m_ASB5SwitchType) && (m_ASB5SwitchType == m_ASB3SwitchType) )
                {
                    emit SigDCHeating();
                }
                else
                {
                    mp_SchedulerThreadController->LogDebug("Retry the AC voltage switch test");
                    m_StateACVoltageStepCount++;
                }
                m_StateACVoltageStep = SET_VOLTAGE_ASB3_AWITCH;
            }
            else
            {
                //the secod time
                if( 1 == m_ASB5SwitchType || 2 == m_ASB5SwitchType)
                {
                    if(m_ASB5SwitchType == m_ASB3SwitchType)
                    {
                        // is the same pass
                        emit SigDCHeating();
                        m_StateACVoltageStep = SET_VOLTAGE_ASB3_AWITCH;
                    }
                    else
                    {
                        m_StateACVoltageStep = CHECK_VOLTAGE_RANGE_AGAIN;
                    }
                    m_StateACVoltageStepCount = 0;
                }
                else
                {
                    mp_SchedulerThreadController->LogDebug("Failed unknow the ASB5 voltage");
                    mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_ASB5_VOLTAGE_VALUE_UNKNOWN);
                    SendSignalSelfTestDone(false);
                    m_StateACVoltageStep = SET_VOLTAGE_ASB3_AWITCH;
                    m_StateACVoltageStepCount = 0;
                }
            }
            break;
        case CHECK_VOLTAGE_RANGE_AGAIN:
            if(1 == m_ASB5SwitchType)
            {
                //Serial for 220V
                if(0 == m_StartReq)
                {
                    CmdRVSetTemperatureSwitchState* cmd = new CmdRVSetTemperatureSwitchState(500, mp_SchedulerThreadController);
                    cmd->SetHeaterVoltage(HEATER_220V);
                    cmd->SetAutoType(AUTO_SWITCH_ENABLE);
                    mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
                    m_StartReq++;
                }
                else
                {
                    if("Scheduler::RVSetTemperatureSwitchState" == cmdName)
                    {
                        mp_SchedulerThreadController->LogDebug(QString("Self-Test set the ASB5 to ASB3 220v votage,the recode:%1").arg(retCode));
                        if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                        {
                            emit SigDCHeating();
                        }
                        else
                        {
                            mp_SchedulerThreadController->SendOutErrMsg(retCode);
                            SendSignalSelfTestDone(false);
                        }
                        m_StartReq = 0;
                        m_StateACVoltageStep = SET_VOLTAGE_ASB3_AWITCH;
                    }
                }
            }
            else
            {
                //Parallel for 110V
                if(0 == m_StartReq)
                {
                    CmdRVSetTemperatureSwitchState* cmd = new CmdRVSetTemperatureSwitchState(500, mp_SchedulerThreadController);
                    cmd->SetHeaterVoltage(HEATER_110V);
                    cmd->SetAutoType(AUTO_SWITCH_ENABLE);
                    mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
                    m_StartReq++;
                }
                else
                {
                    if("Scheduler::RVSetTemperatureSwitchState" == cmdName)
                    {
                        mp_SchedulerThreadController->LogDebug(QString("Self-Test set the ASB5 to ASB3 110v votage,the recode:%1").arg(retCode));
                        if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                        {
                            emit SigDCHeating();
                        }
                        else
                        {
                            mp_SchedulerThreadController->SendOutErrMsg(retCode);
                            SendSignalSelfTestDone(false);
                        }
                        m_StartReq = 0;
                        m_StateACVoltageStep = SET_VOLTAGE_ASB3_AWITCH;
                    }
                }
            }
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
            ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForSelfTest("LA_Tube1");
            mp_SchedulerThreadController->LogDebug(QString("Self-Test in DC start heating LA_Tube1,recode:%1").arg(ret));
            if(DCL_ERR_FCT_CALL_SUCCESS == ret)
            {
                m_StateDCHeatingStep = STOPHEATING_LATUBE1;
                m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
            }
            else
            {
                mp_SchedulerThreadController->SendOutErrMsg(ret);
                SendSignalSelfTestDone(false);
            }
            break;
        case STOPHEATING_LATUBE1:
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 3 * 1000)
            {
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("LA_Tube1");
                mp_SchedulerThreadController->LogDebug(QString("Self-Test in DC stop heating LA_Tube1,recode:%1").arg(ret));
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    m_StateDCHeatingStep = STARTHEATING_LATUBE2;
                }
                else
                {
                    m_StateDCHeatingStep = STARTHEATING_LATUBE1;
                    mp_SchedulerThreadController->SendOutErrMsg(ret);
                    SendSignalSelfTestDone(false);
                }
            }
            break;
        case STARTHEATING_LATUBE2:
            ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForSelfTest("LA_Tube2");
            mp_SchedulerThreadController->LogDebug(QString("Self-Test in DC start heating LA_Tube2,recode:%1").arg(ret));
            if(DCL_ERR_FCT_CALL_SUCCESS == ret)
            {
                m_StateDCHeatingStep = STOPHEATING_LATBUBE2;
                m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
            }
            else
            {
                m_StateDCHeatingStep = STARTHEATING_LATUBE1;
                mp_SchedulerThreadController->SendOutErrMsg(ret);
                SendSignalSelfTestDone(false);
            }
            break;
        case STOPHEATING_LATBUBE2:
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 3 * 1000)
            {
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("LA_Tube2");
                mp_SchedulerThreadController->LogDebug(QString("Self-Test in DC stop heating LA_Tube2,recode:%1").arg(ret));
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    m_StateDCHeatingStep = STARTHEATING_LEVELSENSOR;
                }
                else
                {
                    m_StateDCHeatingStep = STARTHEATING_LATUBE1;
                    mp_SchedulerThreadController->SendOutErrMsg(ret);
                    SendSignalSelfTestDone(false);
                }
            }
            break;
        case STARTHEATING_LEVELSENSOR:
            ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForSelfTest("LevelSensor");
            mp_SchedulerThreadController->LogDebug(QString("Self-Test in DC start heating LevelSensor,recode:%1").arg(ret));
            if(DCL_ERR_FCT_CALL_SUCCESS == ret)
            {
                m_StateDCHeatingStep = STOPHEATING_LEVELSENSOR;
                m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
            }
            else
            {
                m_StateDCHeatingStep = STARTHEATING_LATUBE1;
                mp_SchedulerThreadController->SendOutErrMsg(ret);
                SendSignalSelfTestDone(false);
            }
            break;
        case STOPHEATING_LEVELSENSOR:
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 3 * 1000)
            {
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("LevelSensor");
                mp_SchedulerThreadController->LogDebug(QString("Self-Test in DC stop heating LevelSensor,recode:%1").arg(ret));
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    m_StateDCHeatingStep = STARTHEATING_LATUBE1;
                    emit SigPressureSensorPumpValve();
                }
                else
                {
                    m_StateDCHeatingStep = STARTHEATING_LATUBE1;
                    mp_SchedulerThreadController->SendOutErrMsg(ret);
                    SendSignalSelfTestDone(false);
                }
            }
            break;
        default:
            break;
        }

}

void CProgramSelfTest::HandlePressure(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    qint64 nowTime = 0;

    switch(m_StatePressureStep)
    {
        case START_PUMP:
            if(0 == m_StartReq)
            {
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdALPressure(500, mp_SchedulerThreadController));
                m_StartReq++;
            }
            else
            {
                if("Scheduler::ALPressure" == cmdName)
                {
                    mp_SchedulerThreadController->LogDebug(QString("Self-Test in pressure,recode:%1").arg(retCode));
                    m_StatePressureStep = STOP_PUMP;
                    m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
                    m_StartReq = 0;
                }
            }
            break;
        case STOP_PUMP:
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 3 * 1000)
            {
                if(0 == m_StartReq)
                {
                    mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, mp_SchedulerThreadController));
                    m_StartReq++;
                }
                else
                {
                    if("Scheduler::ALReleasePressure" == cmdName)
                    {
                        mp_SchedulerThreadController->LogDebug(QString("Self-Test in release pressure,recode:%1").arg(retCode));
                        m_StatePressureStep = START_VALVE1;
                        m_StartReq = 0;
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
            }
            else
            {
                if("Scheduler::ALControlValve" == cmdName)
                {
                    mp_SchedulerThreadController->LogDebug(QString("Self-Test start vavel1,recode:%1").arg(retCode));
                    if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                    {
                        m_StatePressureStep = STOP_VALVE1;
                        m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
                    }
                    else
                    {
                        m_StatePressureStep = START_PUMP;
                        mp_SchedulerThreadController->SendOutErrMsg(retCode);
                        SendSignalSelfTestDone(false);
                    }
                    m_StartReq = 0;
                }
            }
            break;
        case STOP_VALVE1:
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
                }
                else
                {
                    if("Scheduler::ALControlValve" == cmdName)
                    {
                        mp_SchedulerThreadController->LogDebug(QString("Self-Test in stop valve1,recode:%1").arg(retCode));
                        if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                        {
                            m_StatePressureStep = START_VALVE2;
                        }
                        else
                        {
                            m_StatePressureStep = START_PUMP;
                            mp_SchedulerThreadController->SendOutErrMsg(retCode);
                            SendSignalSelfTestDone(false);
                        }
                        m_StartReq = 0;
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
            }
            else
            {
                if("Scheduler::ALControlValve" == cmdName)
                {
                    mp_SchedulerThreadController->LogDebug(QString("Self-Test start valve2,recode:%1").arg(retCode));
                    if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                    {
                        m_StatePressureStep = STOP_VALVE2;
                        m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
                    }
                    else
                    {
                        m_StatePressureStep = START_PUMP;
                        mp_SchedulerThreadController->SendOutErrMsg(retCode);
                        SendSignalSelfTestDone(false);
                    }
                    m_StartReq = 0;
                }
            }
            break;
        case STOP_VALVE2:
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
                }
                else
                {
                    if("Scheduler::ALControlValve" == cmdName)
                    {
                        mp_SchedulerThreadController->LogDebug(QString("Self-Test stop valve2,recode:%1").arg(retCode));
                        if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                        {
                            m_StatePressureStep = START_EXHAUSTFAN;
                        }
                        else
                        {
                            m_StatePressureStep = START_PUMP;
                            mp_SchedulerThreadController->SendOutErrMsg(retCode);
                            SendSignalSelfTestDone(false);
                        }
                        m_StartReq = 0;
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
            }
            else
            {
                if("Scheduler::ALTurnOnFan" == cmdName)
                {
                    mp_SchedulerThreadController->LogDebug(QString("Self-Test start fan,recode:%1").arg(retCode));
                    if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                    {
                        m_StatePressureStep = STOP_EXHAUSTFAN;
                        m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
                    }
                    else
                    {
                        m_StatePressureStep = START_PUMP;
                        mp_SchedulerThreadController->SendOutErrMsg(retCode);
                        SendSignalSelfTestDone(false);
                    }
                    m_StartReq = 0;
                }
            }
            break;
        case STOP_EXHAUSTFAN:
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 3 * 1000)
            {
                if(0 == m_StartReq)
                {
                    CmdALTurnOffFan* cmd = new CmdALTurnOffFan(500, mp_SchedulerThreadController);
                    mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
                    m_StartReq++;
                }
                else
                {
                    if("Scheduler::ALTurnOffFan" == cmdName)
                    {
                        mp_SchedulerThreadController->LogDebug(QString("Self-Test stop fan,recode:%1").arg(retCode));
                        if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                        {
                            m_StatePressureStep = START_PUMP;
                            emit SigACHeating();
                        }
                        else
                        {
                            m_StatePressureStep = START_PUMP;
                            mp_SchedulerThreadController->SendOutErrMsg(retCode);
                            SendSignalSelfTestDone(false);
                        }
                        m_StartReq = 0;
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
            mp_SchedulerThreadController->LogDebug(QString("Self-Test AC start heating RTSide,the retCode:%1").arg(ret));
            if(DCL_ERR_FCT_CALL_SUCCESS == ret )
            {
                m_StateACHeatingStep = STOPHEATING_RETORTOP;
                m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
            }
            else
            {
                m_StateACHeatingStep = STARTHEATING_RETORTTOP;
                mp_SchedulerThreadController->SendOutErrMsg(ret);
                SendSignalSelfTestDone(false);
            }
            break;
        case STOPHEATING_RETORTOP:
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 5 * 1000)
            {
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("RTSide");
                mp_SchedulerThreadController->LogDebug(QString("Self-Test AC stop heating RTSide,the retCode:%1").arg(ret));
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    m_StateACHeatingStep = STARTHEATING_RETORTBOTTOM;
                }
                else
                {
                    m_StateACHeatingStep = STARTHEATING_RETORTTOP;
                    mp_SchedulerThreadController->SendOutErrMsg(ret);
                    SendSignalSelfTestDone(false);
                }
            }
            break;
        case STARTHEATING_RETORTBOTTOM:
            ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForSelfTest("RTBottom");
            mp_SchedulerThreadController->LogDebug(QString("Self-Test AC start heating RTBottom,the retCode:%1").arg(ret));
            if(DCL_ERR_FCT_CALL_SUCCESS == ret)
            {
                m_StateACHeatingStep = STOPHEATING_RETORTBOTTOM;
                m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
            }
            else
            {
                m_StateACHeatingStep = STARTHEATING_RETORTTOP;
                mp_SchedulerThreadController->SendOutErrMsg(ret);
                SendSignalSelfTestDone(false);
            }
            break;
        case STOPHEATING_RETORTBOTTOM:
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 5 * 1000)
            {
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("RTBottom");
                mp_SchedulerThreadController->LogDebug(QString("Self-Test AC stop heating RTBottom,the retCode:%1").arg(ret));
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    m_StateACHeatingStep = STARTHEATING_OVENTOP;
                }
                else
                {
                    m_StateACHeatingStep = STARTHEATING_RETORTTOP;
                    mp_SchedulerThreadController->SendOutErrMsg(ret);
                    SendSignalSelfTestDone(false);
                }
            }
            break;
        case STARTHEATING_OVENTOP:
            ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForSelfTest("OvenTop");
            mp_SchedulerThreadController->LogDebug(QString("Self-Test AC start heating OvenTop,the retCode:%1").arg(ret));
            if(DCL_ERR_FCT_CALL_SUCCESS == ret)
            {
                m_StateACHeatingStep = STOPHEATING_OVENTOP;
                m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
            }
            else
            {
                m_StateACHeatingStep = STARTHEATING_RETORTTOP;
                mp_SchedulerThreadController->SendOutErrMsg(ret);
                SendSignalSelfTestDone(false);
            }
            break;
        case STOPHEATING_OVENTOP:
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 5 * 1000)
            {
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("OvenTop");
                mp_SchedulerThreadController->LogDebug(QString("Self-Test AC stop heating OvenTop,the retCode:%1").arg(ret));
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    m_StateACHeatingStep = STARTHEATING_OVENBOTTOM;
                }
                else
                {
                    m_StateACHeatingStep = STARTHEATING_RETORTTOP;
                    mp_SchedulerThreadController->SendOutErrMsg(ret);
                    SendSignalSelfTestDone(false);
                }
            }
            break;
        case STARTHEATING_OVENBOTTOM:
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 5 * 1000)
            {
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForSelfTest("OvenBottom");
                mp_SchedulerThreadController->LogDebug(QString("Self-Test AC start heating OvenBottom,the retCode:%1").arg(ret));
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    m_StateACHeatingStep = STOPHEATING_OVENBOTTOM;
                    m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
                }
                else
                {
                    m_StateACHeatingStep = STARTHEATING_RETORTTOP;
                    mp_SchedulerThreadController->SendOutErrMsg(ret);
                    SendSignalSelfTestDone(false);
                }
            }
            break;
        case STOPHEATING_OVENBOTTOM:
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 5 * 1000)
            {
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("OvenBottom");
                mp_SchedulerThreadController->LogDebug(QString("Self-Test AC stop heating OvenBottom,the retCode:%1").arg(ret));
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    m_StateACHeatingStep = STARTHEATING_RV;
                }
                else
                {
                    m_StateACHeatingStep = STARTHEATING_RETORTTOP;
                    mp_SchedulerThreadController->SendOutErrMsg(ret);
                    SendSignalSelfTestDone(false);
                }
            }
            break;
        case STARTHEATING_RV:
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 5 * 1000)
            {
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForSelfTest("RV");
                mp_SchedulerThreadController->LogDebug(QString("Self-Test AC start heating RV,the retCode:%1").arg(ret));
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    m_StateACHeatingStep = STOPHEATING_RV;
                    m_DelayBeginTime = QDateTime::currentMSecsSinceEpoch();
                }
                else
                {
                    m_StateACHeatingStep = STARTHEATING_RETORTTOP;
                    mp_SchedulerThreadController->SendOutErrMsg(ret);
                    SendSignalSelfTestDone(false);
                }
            }
            break;
        case STOPHEATING_RV:
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_DelayBeginTime > 5 * 1000)
            {
                ret = mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("RV");
                mp_SchedulerThreadController->LogDebug(QString("Self-Test AC stop heating RV,the retCode:%1").arg(ret));
                if(DCL_ERR_FCT_CALL_SUCCESS == ret)
                {
                    m_StateACHeatingStep = STARTHEATING_RETORTTOP;
                    SendSignalSelfTestDone(true);
                }
                else
                {
                    m_StateACHeatingStep = STARTHEATING_RETORTTOP;
                    mp_SchedulerThreadController->SendOutErrMsg(ret);
                    SendSignalSelfTestDone(false);
                }
            }
            break;
        default:
            break;
    }
}

}
