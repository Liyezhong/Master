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
#include "Scheduler/Commands/Include/CmdRVReqMoveToInitialPosition.h"
#include "Scheduler/Commands/Include/CmdIDForceDraining.h"
#include "Scheduler/Commands/Include/CmdALVaccum.h"
#include "Scheduler/Commands/Include/CmdALReleasePressure.h"
#include "Scheduler/Commands/Include/CmdALPressure.h"

namespace Scheduler{

CRcReHeating::CRcReHeating(SchedulerMainThreadController* SchedController)
    :mp_SchedulerThreadController(SchedController)
    ,m_CurrentStep(INIT_STATE)
    ,m_LastScenario(0)
    ,m_StartReq(0)
    ,m_StartHeatingTime(0)
    ,m_StartPressureTime(0)
    ,m_IsNeedRunCleaning(false)
    ,m_DrainIsOk(false)
    ,m_RsReagentCheckStep(FORCE_DRAIN)
{
    CONNECTSIGNALSLOT(this, SignalDrain(), mp_SchedulerThreadController, OnBeginDrain());
    CONNECTSIGNALSLOT(this, SignalStopDrain(), mp_SchedulerThreadController, OnStopDrain());
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
        case START_TEMPERATURE:
            if(true == StartHeatingSensor())
            {
                m_StartHeatingTime = QDateTime::currentMSecsSinceEpoch();
                m_CurrentStep = CHECK_TEMPERATURE;
            }
            else
            {
                emit TasksDone(false);
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
    if(200 == m_LastScenario || 211 == m_LastScenario || 260 == m_LastScenario)
    {
        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_POWER_FAILURE_SPECIAL_STEP);
        if(!m_IsNeedRunCleaning)
        {
            mp_SchedulerThreadController->SendPowerFailureMsg();
        }
    }
    else if(212 <= m_LastScenario && m_LastScenario <= 257)
    {
        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_POWER_FAILURE_REAGENT_STEP);
        if(!m_IsNeedRunCleaning)
        {
            mp_SchedulerThreadController->SendPowerFailureMsg();
        }
    }
    else if(271 <= m_LastScenario && m_LastScenario <= 277)
    {
        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_POWER_FAILURE_PARAFFIN_STEP);
        if(!m_IsNeedRunCleaning)
        {
            mp_SchedulerThreadController->SendPowerFailureMsg();
        }
    }
    else if(203 == m_LastScenario && 281 <= m_LastScenario && m_LastScenario <= 297)
    {
        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_POWER_FAILURE_BACK_CLEANING);
    }
    m_CurrentStep = START_TEMPERATURE;
}

bool CRcReHeating::StartHeatingSensor()
{
    ReturnCode_t ret = DCL_ERR_FCT_CALL_SUCCESS;
    if( 200 == m_LastScenario || 260 == m_LastScenario || (m_LastScenario >= 211 && m_LastScenario <= 257) )
    {
        mp_SchedulerThreadController->GetHeatingStrategy()->SetHeatingStrategyScenario(4);
    }
    else if(203 == m_LastScenario || (281 <= m_LastScenario && m_LastScenario <= 297) )
    {
        mp_SchedulerThreadController->GetHeatingStrategy()->SetHeatingStrategyScenario(4);
        ret = mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControlForPowerFailure("RV");
        mp_SchedulerThreadController->LogDebug(QString("In RcReHeating start heating RV,retCode:%1").arg(ret));
        if(DCL_ERR_FCT_CALL_SUCCESS != ret)
            return false;
        else
            return true;
    }
    else
    {
        mp_SchedulerThreadController->GetHeatingStrategy()->SetHeatingStrategyScenario(260);
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
        return true;

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
            emit TasksDone(false);
            return;
        }
        if(tmperature > RV_SENSOR2_TEMP)
        {
            m_CurrentStep = GET_RV_POSOTION;
        }
        else
        {
            if(QDateTime::currentMSecsSinceEpoch() - m_StartHeatingTime > WAIT_RV_SENSOR2_TEMP * 1000)
            {
                emit TasksDone(false);
            }
        }
    }
    else if(271 <= m_LastScenario && m_LastScenario <= 277)
    {
        // for parrafin program
        qint64 CurrentTime = QDateTime::currentMSecsSinceEpoch();
        if( CurrentTime - m_StartHeatingTime > mp_SchedulerThreadController->GetOvenHeatingRemainingTime() )
        {
            if(mp_SchedulerThreadController->GetHeatingStrategy()->Check260SensorsTemp())
            {
                m_CurrentStep = GET_RV_POSOTION;
            }
            else
            {
                if( CurrentTime - m_StartHeatingTime > WAIT_PARAFFIN_TEMP_TIME * 1000)
                {
                    emit TasksDone(false);
                }
            }
        }
    }
    else
    {
        mp_SchedulerThreadController->LogDebug(QString("Invalid scenario in power failure"));
        emit TasksDone(false);
    }
}

void CRcReHeating::GetRvPosition(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    qreal CurrentPressure = 0.0;
    if(200 == m_LastScenario || 211 == m_LastScenario || 260 == m_LastScenario)
    {
        if (0 == m_StartReq)
        {
            mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdRVReqMoveToInitialPosition(500, mp_SchedulerThreadController));
            m_StartReq++;
        }
        else if(mp_SchedulerThreadController->IsRVRightPosition(INITIALIZE_POS))
        {
            if(m_IsNeedRunCleaning)
            {
                mp_SchedulerThreadController->SetCurrentStepState(PSSM_POWERFAILURE_FINISH);
            }
            emit TasksDone(true);
            m_StartReq = 0;
        }
        else if("Scheduler::RVReqMoveToInitialPosition" == cmdName)
        {
            if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
            {
                emit TasksDone(false);
                m_StartReq = 0;
            }
        }
    }
    else
    {
        switch(m_RsReagentCheckStep)
        {
        case FORCE_DRAIN:
            if(0 == m_StartReq)
            {
                mp_SchedulerThreadController->LogDebug("reagent check build pressure.");
                CmdALPressure* CmdPressure = new CmdALPressure(500, mp_SchedulerThreadController);
                CmdPressure->SetTargetPressure(40.0);
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(CmdPressure);
                m_StartPressureTime = QDateTime::currentMSecsSinceEpoch();
                m_StartReq++;
            }
            else if(1 == m_StartReq)
            {
                if("Scheduler::ALPressure" == cmdName)
                {
                    m_StartReq++;
                }
            }
            else if(2 == m_StartReq)
            {
                if(QDateTime::currentMSecsSinceEpoch() - m_StartPressureTime > 60 * 1000)
                {
                    mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, mp_SchedulerThreadController));
                    m_StartReq++;
                }
            }
            else if(3 == m_StartReq)
            {
                if("Scheduler::ALReleasePressure" == cmdName)
                {
                    m_RsReagentCheckStep = BUILD_VACUUM;
                    m_StartReq = 0;
                }
            }
            break;
        case BUILD_VACUUM:
            if(0 == m_StartReq)
            {
                mp_SchedulerThreadController->LogDebug("Send cmd to DCL to build vacuum in PowerFailure");
                CmdALVaccum* cmd = new CmdALVaccum(500, mp_SchedulerThreadController);
                cmd->SetTargetPressure(-0.5);
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
                m_StartReq++;
            }
            else if("Scheduler::ALVaccum" == cmdName)
            {
                if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    m_StartReq++;
                    m_StartPressureTime = QDateTime::currentMSecsSinceEpoch();
                }
                else
                {
                    m_RsReagentCheckStep = FORCE_DRAIN;
                    m_StartReq = 0;
                    emit TasksDone(false);
                }
            }
            else if(2 == m_StartReq)
            {
                CurrentPressure = mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().PressureAL;
                if( -5 < CurrentPressure && CurrentPressure <= -0.5)
                {
                    mp_SchedulerThreadController->LogDebug(QString("Build vaccum success, the pressure:%1").arg(CurrentPressure));
                    m_RsReagentCheckStep = MOVE_INITIALIZE_POSITION;
                    m_StartReq = 0;
                }
                else
                {
                    if(QDateTime::currentMSecsSinceEpoch() - m_StartPressureTime > 5 * 1000)
                    {
                        m_StartReq = 0;
                        m_RsReagentCheckStep = FORCE_DRAIN;
                        emit TasksDone(false);
                    }
                }
            }
            break;
        case MOVE_INITIALIZE_POSITION:
            if(0 == m_StartReq)
            {
                CmdRVReqMoveToInitialPosition *cmd = new CmdRVReqMoveToInitialPosition(500, mp_SchedulerThreadController);
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
                m_StartReq++;
            }
            else if(mp_SchedulerThreadController->IsRVRightPosition(INITIALIZE_POS))
            {
                m_RsReagentCheckStep = MOVE_TUBEPOSITION;
                m_StartReq = 0;
            }
            else if("Scheduler::RVReqMoveToInitialPosition" == cmdName)
            {
                if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
                {
                    m_RsReagentCheckStep = FORCE_DRAIN;
                    emit TasksDone(false);
                    m_StartReq = 0;
                }
            }
            break;
        case MOVE_TUBEPOSITION:
            if(0 == m_StartReq)
            {
                if( !mp_SchedulerThreadController->MoveRV(TUBE_POS) )
                {
                    m_RsReagentCheckStep = FORCE_DRAIN;
                    emit TasksDone(false);
                    m_StartReq = 0;
                }
                else
                {
                    m_StartReq++;
                }
            }
            else if(mp_SchedulerThreadController->IsRVRightPosition(TUBE_POS))
            {
                m_StartReq = 0;
                m_RsReagentCheckStep = REALSE_PRESSURE;
            }
            else
            {
                if("Scheduler::RVReqMoveToRVPosition" == cmdName)
                {
                    if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
                    {
                        m_RsReagentCheckStep = FORCE_DRAIN;
                        emit TasksDone(false);
                    }
                }
            }
            break;
        case REALSE_PRESSURE:
            if(0 == m_StartReq)
            {
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, mp_SchedulerThreadController));
                m_StartReq++;
            }
            else if("Scheduler::ALReleasePressure" == cmdName)
            {
                if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    m_CurrentStep = BEGIN_DRAIN;
                }
                else
                {
                    emit TasksDone(false);
                }
                m_StartReq = 0;
                m_RsReagentCheckStep = FORCE_DRAIN;
            }
            break;
        default:
            break;
        }
    }
}

void CRcReHeating::ProcessDraining(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    if (0 == m_StartReq)
    {
        CmdIDForceDraining* cmd  = new CmdIDForceDraining(500, mp_SchedulerThreadController);

        QString stationID = mp_SchedulerThreadController->GetCurrentStationID();
        RVPosition_t tubePos = mp_SchedulerThreadController->GetRVTubePositionByStationID(stationID);
        cmd->SetRVPosition((quint32)(tubePos));
        cmd->SetDrainPressure(40.0);
        cmd->SetReagentGrpID(GetReagentID());
        mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
        m_StartReq ++;
        emit SignalDrain();
    }
    else if ("Scheduler::IDForceDraining" == cmdName)
    {
        emit SignalStopDrain();
        if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
        {
            m_DrainIsOk = true;
        }
        else
        {
            m_DrainIsOk = false;
        }
        m_StartReq++;
    }
    else if(2 == m_StartReq)
    {
        if(m_DrainIsOk)
        {
            if(m_IsNeedRunCleaning)
            {
                mp_SchedulerThreadController->SetCurrentStepState(PSSM_POWERFAILURE_FINISH);
            }
            emit TasksDone(true);
        }
        else
        {
            emit TasksDone(false);
        }
        emit TasksDone(true);
        m_StartReq = 0;
    }
}

QString CRcReHeating::GetReagentID()
{
    QString ReagentID;
    if(281 <= m_LastScenario && m_LastScenario <= 287)
    {
        ReagentID = "RG7";
    }
    else if(291 <= m_LastScenario && m_LastScenario <= 297)
    {
        ReagentID = "RG8";
    }
    else
    {
        ReagentID = m_LastReagentID;
    }
    return ReagentID;
}

}
