/****************************************************************************/
/*! \file RcReHeating.cpp
 *
 *  \brief CRcReHeating class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ Sep 18th, 2014
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

#include "Scheduler/Include/RcReHeating.h"
#include "Global/Include/Utils.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/HeatingStrategy.h"
#include "Scheduler/Include/SchedulerEventCodes.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToCurrentTubePosition.h"
#include "Scheduler/Commands/Include/CmdALReleasePressure.h"
#include "Scheduler/Commands/Include/CmdALPressure.h"

namespace Scheduler{

CRcReHeating::CRcReHeating(SchedulerMainThreadController* SchedController, CSchedulerStateMachine* StateMachine)
    :mp_SchedulerThreadController(SchedController)
    ,mp_StateMachine(StateMachine)
    ,m_CurrentStep(INIT_STATE)
    ,m_LastScenario(0)
    ,m_StartReq(0)
    ,m_StartHeatingTime(0)
    ,m_StartPressureTime(0)
    ,m_Is5MinTimeOut(false)
    ,m_CountTheEffectiveTemp(0)
    ,m_PressureCalibrationSeq(0)
    ,m_PressureDriftOffset(0.0)
    ,m_PressureCalibrationCounter(0)
    ,m_ReleasePressureTime(0)
    ,m_IsLoged(0)
{
}

CRcReHeating::~CRcReHeating()
{
}

void CRcReHeating::HandleWorkFlow(const QString &cmdName, ReturnCode_t retCode)
{
    switch(m_CurrentStep)
    {
        case INIT_STATE:
            HandleInint();
            break;
        case PRESSURE_CALIBRATION:
            HandlePressureCalibration(cmdName);
            break;
        case START_TEMPERATURE:
            if(true == StartHeatingSensor())
            {
                m_StartHeatingTime = QDateTime::currentMSecsSinceEpoch();
                m_CurrentStep = CHECK_TEMPERATURE;
            }
            else
            {
                mp_StateMachine->OnTasksDone(false);
            }
            break;
        case CHECK_TEMPERATURE:
            CheckTheTemperature();
            break;
        case GET_RV_POSOTION:
            GetRvPosition(cmdName, retCode);
            break;
        case BEGIN_DRAIN:
            ProcessDraining(cmdName, retCode);
            break;
        default:
            break;
    }
}

void CRcReHeating::HandleInint()
{
    if(200 == m_LastScenario|| 260 == m_LastScenario || (QString::number(m_LastScenario).left(1) == "2" && QString::number(m_LastScenario).right(1) =="1"))
    {
        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_POWER_FAILURE_SPECIAL_STEP);
        if(m_Is5MinTimeOut)
        {
            mp_SchedulerThreadController->SendPowerFailureMsg();
        }
    }
    else if(212 <= m_LastScenario && m_LastScenario <= 257)
    {
        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_POWER_FAILURE_REAGENT_STEP);
        if(m_Is5MinTimeOut)
        {
            mp_SchedulerThreadController->SendPowerFailureMsg();
        }
    }
    else if(271 <= m_LastScenario && m_LastScenario <= 277)
    {
        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_POWER_FAILURE_PARAFFIN_STEP);
        if(m_Is5MinTimeOut)
        {
            mp_SchedulerThreadController->SendPowerFailureMsg();
        }
    }
    else if(203 == m_LastScenario || (281 <= m_LastScenario && m_LastScenario <= 297) )
    {
        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_POWER_FAILURE_BACK_CLEANING);
    }
    m_CurrentStep = PRESSURE_CALIBRATION;
}

void CRcReHeating::HandlePressureCalibration(const QString &cmdName)
{
    if (0 == m_PressureCalibrationSeq)
    {
        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_RELEASE_PRESSURE_CALIBRATION);
        mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, mp_SchedulerThreadController));
        m_PressureCalibrationSeq++;
    }
    else if (1 == m_PressureCalibrationSeq)
    {
        if ("Scheduler::ALReleasePressure" == cmdName)
        {
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_RELEASE_PRESSURE_CALIBRATION_SUCCESS);
            m_PressureCalibrationSeq++;
            m_ReleasePressureTime = QDateTime::currentMSecsSinceEpoch();
        }
    }
    else if (2 == m_PressureCalibrationSeq)
    {
        if(0 == m_IsLoged)
        {
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_WAIT_20S);
            m_IsLoged++;
        }
        if ((QDateTime::currentMSecsSinceEpoch() - m_ReleasePressureTime) >= 20*1000)
        {
            m_IsLoged = 0;
            m_PressureCalibrationSeq++;
        }
    }
    else if (3 == m_PressureCalibrationSeq)
    {
        // Firstly, check if calibration has been 3 times
        if (3 == m_PressureCalibrationCounter)
        {
            m_PressureCalibrationSeq = 0;
            m_PressureCalibrationCounter = 0;
            mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_LA_PRESSURESENSOR_PRECHECK_FAILED);//Just log with warning
            m_CurrentStep = START_TEMPERATURE; //move to next step
        }
        else if (m_PressureCalibrationCounter > 0)
        {
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_RETRY_PRESSURE_CALIBRATION);
        }

        m_PressureCalibrationCounter++;

        qreal currentPressure = mp_SchedulerThreadController->GetSchedCommandProcessor()->ALGetRecentPressure();
        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_GET_CURRENT_PRESSURE, QStringList()<<QString::number(currentPressure));

        if (qAbs(currentPressure) < 0.2) // Calibration is Not needed
        {
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_PRESSURE_CALIBRATION_SUCCESS);
            m_PressureCalibrationSeq = 0;
            m_PressureCalibrationCounter = 0;
            m_CurrentStep = START_TEMPERATURE;
        }
        else if (qAbs(currentPressure) <= 3.0) //offset the calibration
        {
            m_PressureDriftOffset = m_PressureDriftOffset + currentPressure;
            mp_SchedulerThreadController->GetSchedCommandProcessor()->ALSetPressureDrift(m_PressureDriftOffset);
            mp_SchedulerThreadController->SetLastPressureOffset(m_PressureDriftOffset);
            m_PressureCalibrationSeq = 0;
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_OFFSET_CALIBRATION);
        }
    }
}

bool CRcReHeating::StartHeatingSensor()
{
    ReturnCode_t ret = DCL_ERR_FCT_CALL_SUCCESS;
    if( 200 == m_LastScenario || 260 == m_LastScenario || (m_LastScenario >= 211 && m_LastScenario <= 257)
            || 203 == m_LastScenario || (281 <= m_LastScenario && m_LastScenario <= 297) )
    {
        mp_SchedulerThreadController->GetHeatingStrategy()->SetHeatingStrategyScenario(4);
    }
    else if(m_LastScenario >= 271 && m_LastScenario <= 277)
    {
        mp_SchedulerThreadController->GetHeatingStrategy()->SetHeatingStrategyScenario(260);
    }
    else
    {
        mp_SchedulerThreadController->LogDebug(QString("Invalid scenario in power failure"));
        return false;
    }

    ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("RV");
    mp_SchedulerThreadController->LogDebug(QString("In RcReHeating start heating RV,retCode:%1").arg(ret));
    if(DCL_ERR_FCT_CALL_SUCCESS != ret)
        return false;

    ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("LA_Tube1");
    mp_SchedulerThreadController->LogDebug(QString("In RcReHeating start heating LA_Tube1,retCode:%1").arg(ret));
    if(DCL_ERR_FCT_CALL_SUCCESS != ret)
        return false;

    ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("LA_Tube2");
    mp_SchedulerThreadController->LogDebug(QString("In RcReHeating start heating LA_Tube2,retCode:%1").arg(ret));
    if(DCL_ERR_FCT_CALL_SUCCESS != ret)
        return false;

    ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("OvenTop");
    mp_SchedulerThreadController->LogDebug(QString("In RcReHeating start heating oven top,retCode:%1").arg(ret));
    if(DCL_ERR_FCT_CALL_SUCCESS != ret)
        return false;

    ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("OvenBottom");
    mp_SchedulerThreadController->LogDebug(QString("In RcReHeating start heating oven bottom,retCode:%1").arg(ret));
    if(DCL_ERR_FCT_CALL_SUCCESS != ret)
        return false;

    if(m_LastScenario >= 271 && m_LastScenario <= 277)
    {
        mp_SchedulerThreadController->GetHeatingStrategy()->Init260ParamList();
        ret =  mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("RTSide");
        mp_SchedulerThreadController->LogDebug(QString("In RcReHeating start heating retort top,retCode:%1").arg(ret));
        if(DCL_ERR_FCT_CALL_SUCCESS != ret)
            return false;

        ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("RTBottom");
        mp_SchedulerThreadController->LogDebug(QString("In RcReHeating start heating retort bottom,retCode:%1").arg(ret));
        if(DCL_ERR_FCT_CALL_SUCCESS != ret)
            return false;
    }
    return true;
}

void CRcReHeating::CheckTheTemperature()
{
    qreal tmperature = 0.0;

    if(200 == m_LastScenario || 260 == m_LastScenario || (m_LastScenario >= 211 && m_LastScenario <= 257)
           || 203 == m_LastScenario || (281 <= m_LastScenario && m_LastScenario <= 297) )
    {
        //for normal program and cleaning program
        tmperature = mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().TempRV2;
        if(!mp_SchedulerThreadController->GetHeatingStrategy()->isEffectiveTemp(tmperature))
        {
            m_CountTheEffectiveTemp++;
            if(m_CountTheEffectiveTemp > 3)
            {
                m_CountTheEffectiveTemp = 0;
                mp_SchedulerThreadController->LogDebug("Temperature of RV2 is NOT effective in RCReHeating");
                mp_StateMachine->OnTasksDone(false);
            }
            return;
        }
        m_CountTheEffectiveTemp = 0;
        if(tmperature > RV_SENSOR2_TEMP)
        {
            m_CurrentStep = GET_RV_POSOTION;
        }
        else
        {
            if(QDateTime::currentMSecsSinceEpoch() - m_StartHeatingTime > WAIT_RV_SENSOR2_TEMP * 1000)
            {
                mp_SchedulerThreadController->LogDebug("Temperature of RV2 is NOT effective in RCReHeating");
                mp_StateMachine->OnTasksDone(false);
            }
        }
    }
    else if(271 <= m_LastScenario && m_LastScenario <= 277)
    {
        // for parrafin program
        qint64 CurrentTime = QDateTime::currentMSecsSinceEpoch();
        if((CurrentTime - m_StartHeatingTime) > mp_SchedulerThreadController->GetOvenHeatingRemainingTime() )
        {
            if(mp_SchedulerThreadController->GetHeatingStrategy()->Check260SensorsTemp(true))
            {
                m_CurrentStep = GET_RV_POSOTION;
            }
            else
            {
                if( CurrentTime - m_StartHeatingTime > WAIT_PARAFFIN_TEMP_TIME * 1000)
                {
                    mp_SchedulerThreadController->LogDebug("RV2 can't reach target temperature in RCReHeating");
                    mp_StateMachine->OnTasksDone(false);
                }
            }
        }
    }
}

void CRcReHeating::GetRvPosition(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    //For the scenarios 2*3 and 2*5, we always consider power failure failed.
    if ((QString::number(m_LastScenario).left(1) == "2" && QString::number(m_LastScenario).right(1) =="3")
            || (QString::number(m_LastScenario).left(1) == "2" && QString::number(m_LastScenario).right(1) =="5"))
    {
        mp_SchedulerThreadController->LogDebug(QString("Position of Rotary Valve is uncertain in the scenario %1").arg(m_LastScenario));
        mp_StateMachine->OnTasksDone(false);
        return;
    }

    //For the scenarios 200, 203, 260, 2*1 and 2*7, we always consider power failure succeeded.
    if(200 == m_LastScenario || 203 == m_LastScenario || 260 == m_LastScenario
            || (QString::number(m_LastScenario).left(1) == "2" && QString::number(m_LastScenario).right(1) =="1")
            || (QString::number(m_LastScenario).left(1) == "2" && QString::number(m_LastScenario).right(1) =="7"))
    {
        if (mp_SchedulerThreadController->GetCurProgramID().at(0) == 'C')
        {
            mp_SchedulerThreadController->SetCurrentStepState(PSSM_INIT);
        }
        else
        {
            mp_SchedulerThreadController->SetCurrentStepState(PSSM_POWERFAILURE_FINISH);
        }
        mp_SchedulerThreadController->LogDebug(QString("RcReheating always succeeds in the scenario %1").arg(m_LastScenario));
        mp_StateMachine->OnTasksDone(true);
        return;
    }

    // For the scenarios 2*4, we need move back to tube position
    if (QString::number(m_LastScenario).left(1) == "2" && QString::number(m_LastScenario).right(1) =="4")
    {
        if (0 == m_StartReq)
        {
            RVPosition_t sealPos = mp_SchedulerThreadController->GetRVSealPositionByStationID(m_LastStationID);
            CmdRVReqMoveToCurrentTubePosition* cmd = new CmdRVReqMoveToCurrentTubePosition(500, mp_SchedulerThreadController);
            cmd->SetRVPosition(sealPos);
            mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
            m_StartReq++;
            mp_SchedulerThreadController->LogDebug(QString("Move RV back to tube postion in the scenario %1").arg(m_LastScenario));
        }
        else if (1 == m_StartReq)
        {
            if("Scheduler::RVReqMoveToCurrentTubePosition" == cmdName)
            {
                if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
                {
                    mp_StateMachine->OnTasksDone(false);
                    m_StartReq = 0;
                    return;
                }
            }

            RVPosition_t tubePos = mp_SchedulerThreadController->GetRVTubePositionByStationID(m_LastStationID);
            if (tubePos == mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().PositionRV)
            {
                mp_SchedulerThreadController->LogDebug(QString("RV has backed to tube position in Power Failure: %1").arg(tubePos));
                m_CurrentStep = BEGIN_DRAIN;
                m_StartReq = 0;
                return;
            }

        }
    }
    else
    {
        m_CurrentStep = BEGIN_DRAIN;
        return;
    }
}

void CRcReHeating::ProcessDraining(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    Q_UNUSED(retCode)

    if (0 == m_StartReq)
    {
        CmdALPressure* cmd  = new CmdALPressure(500, mp_SchedulerThreadController);
        cmd->SetTargetPressure(40.0);
        mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
        m_StartReq ++;
        m_StartPressureTime = QDateTime::currentMSecsSinceEpoch();
        mp_SchedulerThreadController->OnBeginDrain();
        mp_SchedulerThreadController->LogDebug("Begin to forced draining in RcReHeating");
    }
    else if (1 == m_StartReq)
    {
        if ("Scheduler::ALPressure" == cmdName)
        {
            m_StartReq++;
        }
    }
    else if (2 == m_StartReq)
    {
        if (QDateTime::currentMSecsSinceEpoch() - m_StartPressureTime > 60*1000)
        {
            mp_SchedulerThreadController->LogDebug("Complete forced draining and release pressure in RcReHeating");
            mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, mp_SchedulerThreadController));
            m_StartReq++;
            mp_SchedulerThreadController->OnStopDrain();
        }
    }
    else if (3 == m_StartReq)
    {
        if ("Scheduler::ALReleasePressure" == cmdName)
        {
            if (mp_SchedulerThreadController->GetCurProgramID().at(0) == 'C')
            {
                mp_SchedulerThreadController->SetCurrentStepState(PSSM_INIT);
            }
            else
            {
                mp_SchedulerThreadController->SetCurrentStepState(PSSM_POWERFAILURE_FINISH);
            }
            mp_StateMachine->OnTasksDone(true);
            m_StartReq = 0;
        }
    }
}

}
