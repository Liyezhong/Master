/****************************************************************************/
/*! \file RsTissueProtect.cpp
 *
 *  \brief CRsTissueProtect class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ Auguest 8th, 2014
 *   $Author:  $ Songtao Yu
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

#include "Global/Include/Utils.h"
#include "Scheduler/Include/RsTissueProtect.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "Scheduler/Include/HeatingStrategy.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToRVPosition.h"
#include "Scheduler/Commands/Include/CmdALDraining.h"
#include "Scheduler/Commands/Include/CmdALPressure.h"
#include "Scheduler/Commands/Include/CmdIDForceDraining.h"
#include "Scheduler/Commands/Include/CmdALStopCmdExec.h"
#include "Scheduler/Commands/Include/CmdALReleasePressure.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToInitialPosition.h"
#include "Scheduler/Include/SchedulerStateHandler.h"
//#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdStationSuckDrain.h"

using namespace DeviceControl;

namespace Scheduler{
/*lint -e534 */

CRsTissueProtect::CRsTissueProtect(SchedulerMainThreadController* SchedController, CSchedulerStateHandler* SchedulerStatehandler, CSchedulerStateMachine* StateMachhine)
    :mp_SchedulerController(SchedController)
    ,mp_SchedulerStateHandler(SchedulerStatehandler)
    ,mp_StateMachine(StateMachhine)
    ,m_IsLevelSensorRelated(false)
    ,m_StationID("")
    ,m_lastStationID("")
{
    m_DrainCurReagentSeq = 0;
    m_MoveToTubeSeq = 0;
    m_FillSeq = 0;
    m_LevelSensorSeq = 0;
    m_MoveToSealSeq = 0;
    m_ReleasePressure = 0;
    m_DrainSafeReagent = 0;
    m_ProcessingSafeReagent = 0;
    m_IsSafeReagentSuccessful = true;
    m_CurrentStep = UNDEF;
}

CRsTissueProtect::~CRsTissueProtect()
{
}

void CRsTissueProtect::Start()
{
    m_IsLevelSensorRelated = false;
    m_DrainCurReagentSeq = 0;
    m_MoveToTubeSeq = 0;
    m_FillSeq = 0;
    m_LevelSensorSeq = 0;
    m_MoveToSealSeq = 0;
    m_ReleasePressure = 0;
    m_DrainSafeReagent = 0;
    m_ProcessingSafeReagent = 0;
    m_IsSafeReagentSuccessful = true;
    m_CurrentStep = INIT;
}

void CRsTissueProtect::SendTasksDoneSig(bool flag)
{
    mp_StateMachine->OnTasksDoneRSTissueProtect(flag);
    m_CurrentStep = UNDEF;
}

void CRsTissueProtect::HandleWorkFlow(const QString& cmdName, ReturnCode_t retCode, ControlCommandType_t ctrlCmd)
{
    /*lint -e525 */
    switch (m_CurrentStep)
    {
    case INIT:
        //Reset all the variable to initial value
        m_IsLevelSensorRelated = false;
        m_DrainCurReagentSeq = 0;
        m_MoveToTubeSeq = 0;
        m_FillSeq = 0;
        m_LevelSensorSeq = 0;
        m_MoveToSealSeq = 0;
        m_StartWaitTime = 0;
        m_IsSafeReagentSuccessful = true;
        m_lastStationID = mp_SchedulerStateHandler->GetCurrentStationID();

        m_StationID = this->GetStationID();
        if ("" == m_StationID)
        {
            SendTasksDoneSig(false);
            break;
        }
        else
        {
            // Stop level sensor heating at first
            if (DCL_ERR_FCT_CALL_SUCCESS != mp_SchedulerController->GetHeatingStrategy()->StopTemperatureControl("LevelSensor"))
            {
                //SendTasksDoneSig(false);
                // Based on comments from team members, we ignore the error
            }
            if("RG6" == m_ReagentGroup)
            {
                mp_SchedulerController->GetHeatingStrategy()->StartTemperatureControl("RTBottom");
                mp_SchedulerController->GetHeatingStrategy()->StartTemperatureControl("RTSide");
            }

            quint32 Scenario = mp_SchedulerStateHandler->GetCurrentScenario();
            bool ok = false;

            if (QString::number(Scenario).left(1) == "2" && QString::number(Scenario).right(1) =="2")      // For filling
            {
                CmdALStopCmdExec* ALStopCmd = new CmdALStopCmdExec(500, m_Sender);
                ALStopCmd->SetCmdType(0); //for SYNC_CMD_AL_PROCEDURE_SUCKING_LEVELSENSOR
                mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(ALStopCmd);
                m_CurrentStep = STOP_CMDEXEC;
            }
            else if (QString::number(Scenario).left(1) == "2" && QString::number(Scenario).right(1) =="6") // For Draining
            {
                CmdALStopCmdExec* ALStopCmd = new CmdALStopCmdExec(500, m_Sender);
                ALStopCmd->SetCmdType(1); //for SYNC_CMD_AL_PROCEDURE_DRAINING
                mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(ALStopCmd);
                m_CurrentStep = STOP_CMDEXEC;
            }
            else if (QString::number(Scenario).right(1).toInt(&ok)>=2 && QString::number(Scenario).right(1).toInt(&ok) <= 6)
            {
                m_CurrentStep = DRAIN_CUR_REAGENT;
            }
            else
            {
                m_CurrentStep = MOVE_TO_TUBE;
            }

        }
        break;
    case STOP_CMDEXEC:
        mp_SchedulerController->LogDebug("RS_Safe_Reagent, in Stop_CommandExcution state");
        if ("Scheduler::ALStopCmdExec" == cmdName)
        {
            m_CurrentStep = DRAIN_CUR_REAGENT;
        }
        break;
    case DRAIN_CUR_REAGENT:
        if (0 == m_DrainCurReagentSeq)
        {
            mp_SchedulerController->LogDebug("RS_Safe_Reagent, in Drain_cur_Reagent state");
            CmdIDForceDraining* cmd  = new CmdIDForceDraining(500, m_Sender);
            mp_SchedulerController->LogDebug(QString("current station ID is: %1").arg(m_lastStationID));
            RVPosition_t tubePos = mp_SchedulerStateHandler->GetRVTubePositionByStationID(m_lastStationID);
            cmd->SetRVPosition((quint32)(tubePos));
            cmd->SetDrainPressure(40.0);
            mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(cmd);
            mp_SchedulerStateHandler->OnBeginDrain();
            m_DrainCurReagentSeq++;
        }
        else
        {
            if ("Scheduler::IDForceDraining" == cmdName)
            {
                mp_SchedulerStateHandler->OnStopDrain();
                m_DrainCurReagentSeq = 0;
                if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    m_CurrentStep = MOVE_TO_TUBE;
                }
                else
                {
                    SendTasksDoneSig(false);
                }
            }
            else
            {
                // Do nothing
            }
        }
        break;
    case MOVE_TO_TUBE:
        if (0 == m_MoveToTubeSeq)
        {
            mp_SchedulerController->LogDebug("RS_Safe_Reagent, in Move_to_Tube state");
            OnMoveToTube();
            m_MoveToTubeSeq++;
        }
        else if (1 == m_MoveToTubeSeq)
        {
            if ("Scheduler::RVReqMoveToRVPosition" == cmdName)
            {
                if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    m_MoveToTubeSeq++;
                }
                else if (DCL_ERR_DEV_RV_MOTOR_LOSTCURRENTPOSITION == retCode)
                {
                    m_MoveToTubeSeq = 3;
                    mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(new CmdRVReqMoveToInitialPosition(500, m_Sender));
                }
                else
                {
                    m_MoveToTubeSeq = 0;
                    m_FillSeq = 0;
                    m_LevelSensorSeq = 0;
                    m_MoveToSealSeq = 0;
                    SendTasksDoneSig(false);
                }
            }
        }
        else if (2 == m_MoveToTubeSeq)
        {
            if( mp_SchedulerStateHandler->IsRVRightPosition(TUBE_POS) )
            {
                m_MoveToTubeSeq = 0;
                mp_SchedulerController->LogDebug("RS_Safe_Reagent, in Move_To_Tube state, move to tube success");

                //For Paraffin, in case of level sensor's error, we still reuse level sensor
                if (false == m_IsLevelSensorRelated || "RG6" == m_ReagentGroup)
                {
                    m_CurrentStep = LEVELSENSOR_HEATING;
                }
                else
                {
                    m_CurrentStep = FILLING;
                }
            }
            else
            {
                // Do nothing, just wait
            }
        }
        else if (3 == m_MoveToTubeSeq)
        {
            if( mp_SchedulerStateHandler->IsRVRightPosition(INITIALIZE_POS) )
            {
                m_MoveToTubeSeq = 0; //rollback to move to tube

            }
        }
        break;
    case LEVELSENSOR_HEATING:
        if (0 == m_LevelSensorSeq)
        {
            mp_SchedulerController->LogDebug("RS_Safe_Reagent, in LevelSensor Heating state");
            if (DCL_ERR_FCT_CALL_SUCCESS != mp_SchedulerController->GetHeatingStrategy()->StartTemperatureControl("LevelSensor"))
            {
                m_IsSafeReagentSuccessful = false;
            }
            m_LevelSensorSeq++;
        }
        else
        {
            qreal tempLevelSensor = mp_SchedulerController->GetSchedCommandProcessor()->HardwareMonitor().TempALLevelSensor;
            quint16 retValue = mp_SchedulerController->GetHeatingStrategy()->CheckTemperatureOverTime("LevelSensor",tempLevelSensor);
            if (0 == retValue)
            {
                // Do nothing
            }
            else if (1 == retValue)
            {
                m_IsSafeReagentSuccessful = false;
                m_LevelSensorSeq = 0;
                m_CurrentStep = FILLING;
            }
            else if (2 == retValue)
            {
                m_LevelSensorSeq = 0;
                m_CurrentStep = FILLING;
            }
        }
        break;
    case FILLING:
        if (0 == m_FillSeq)
        {
            mp_SchedulerController->LogDebug("RS_Safe_Reagent, in Filling state");
            if ("RG6" == m_ReagentGroup)
            {
                mp_SchedulerController->FillRsTissueProtect(m_StationID, false, true);
            }
            else
            {
                mp_SchedulerController->FillRsTissueProtect(m_StationID, true, false);
            }
            m_FillSeq++;
        }
        else
        {
            if( "Scheduler::ALFilling" == cmdName)
            {
                mp_SchedulerController->StopFillRsTissueProtect(m_StationID);
                m_FillSeq = 0;
                // If return code belongs to one of the followings, we will move RV to sealing position.
                // Otherwise, we think the filling failed and throw safe reagent error
                // 1. DCL_ERR_FCT_CALL_SUCCESS
                // 2. DCL_ERR_DEV_LA_FILLING_OVERFLOW
                // 3. DCL_ERR_DEV_LA_FILLING_INSUFFICIENT
                // 4. DCL_ERR_DEV_LA_FILLING_TIMEOUT_4MIN
                if ( retCode != DCL_ERR_FCT_CALL_SUCCESS
                   && retCode != DCL_ERR_DEV_LA_FILLING_OVERFLOW
                   && retCode != DCL_ERR_DEV_LA_FILLING_INSUFFICIENT
                   && retCode != DCL_ERR_DEV_LA_FILLING_TIMEOUT_4MIN
                   && retCode != DCL_ERR_DEV_LA_FILLING_SOAK_EMPTY)
                {
                    mp_SchedulerController->LogDebug("RS_Safe_Reagent, Filling failed");
                    if (retCode == DCL_ERR_DEV_LA_RELEASING_TIMEOUT)
                    {
                        bool bExecuted = TryNextSafeReagent();
                        if (bExecuted)
                            return;
                    }

                    SendTasksDoneSig(false);
                    return;
                }

                if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    mp_SchedulerController->LogDebug(QString("Return code is successful, program Step Filling OK"));
                    m_CurrentStep = MOVE_TO_SEALING;
                    break;
                }

                if (DCL_ERR_DEV_LA_FILLING_INSUFFICIENT == retCode || DCL_ERR_DEV_LA_FILLING_TIMEOUT_4MIN == retCode)
                {
                    mp_SchedulerController->LogDebug(QString("program Step Filling failed, and retrun code is insufficient or 4 min timeout"));
                    bool bExecuted = TryNextSafeReagent();
                    if (bExecuted)
                        break;


                    m_IsSafeReagentSuccessful = false;
                    m_CurrentStep = MOVE_TO_SEALING;
                    break;
                }

                if (DCL_ERR_DEV_LA_FILLING_OVERFLOW == retCode)
                {
                    mp_SchedulerController->LogDebug(QString(" Return code of program Step Filling is filling overflow"));
                    mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(new CmdALPressure(500, m_Sender));
                    m_StartWaitTime = QDateTime::currentMSecsSinceEpoch();
                    m_CurrentStep = WAIT_8S;

                    if (false == m_IsLevelSensorRelated)
                    {
                        m_IsSafeReagentSuccessful = false;
                    }
                    else
                    {
                        m_IsSafeReagentSuccessful = true;
                    }

                    break;
                }

                if (DCL_ERR_DEV_LA_FILLING_SOAK_EMPTY == retCode) // Only for paraffin
                {
                    mp_SchedulerController->LogDebug(QString("Paraffin soak empty, program Step Filling OK"));
                    bool bExecuted = TryNextSafeReagent();
                    if (bExecuted)
                        break;

                    m_CurrentStep = MOVE_TO_SEALING;
                    break;
                }
            }
        }
        break;
    case WAIT_8S:
        mp_SchedulerController->LogDebug("RS_Safe_Reagent, in wait 8 seconds state");
        if ((QDateTime::currentMSecsSinceEpoch() - m_StartWaitTime) >= 8*1000)
        {
            m_StartWaitTime = 0;
            OnReleasePressure();
            m_CurrentStep = MOVE_TO_SEALING;
        }
        else
        {
            // Do nothing, just wait
        }
        break;
    case MOVE_TO_SEALING:
        if (0 == m_MoveToSealSeq)
        {
            mp_SchedulerController->LogDebug("RS_Safe_Reagent, in Move_To_Seal state");
            OnMoveToSeal();
            m_MoveToSealSeq++;
        }
        else if (1 == m_MoveToSealSeq)
        {
            if(("Scheduler::RVReqMoveToRVPosition" == cmdName))
            {
                if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    m_MoveToSealSeq++;
                }
                else
                {
                    mp_SchedulerController->LogDebug("RS_Safe_Reagent, in Move_To_Seal state, move to seal failed");
                    SendTasksDoneSig(false);
                }
            }
        }
        else
        {
            if( mp_SchedulerStateHandler->IsRVRightPosition(SEAL_POS) )
            {
                mp_SchedulerController->LogDebug("RS_Safe_Reagent, in Move_To_Seal state, move to seal passed");
                m_CurrentStep = RELEASE_PRESSURE;
            }
        }
        break;
    case RELEASE_PRESSURE:
        if (0 == m_ReleasePressure)
        {
            mp_SchedulerController->LogDebug("RS_Safe_Reagent, in state RELEASE_PRESSURE");
            OnReleasePressure();
            m_ReleasePressure++;
        }
        else if (1 == m_ReleasePressure && "Scheduler::ALReleasePressure" == cmdName)
        {
            m_MoveToTubeSeq = 0;
            m_FillSeq = 0;
            m_LevelSensorSeq = 0;
            m_MoveToSealSeq = 0;
            if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                mp_SchedulerController->LogDebug("RS_Safe_Reagent, in RELEASE_PRESSURE, Pressuer release passed");
            }
            else
            {
                mp_SchedulerController->LogDebug("RS_Safe_Reagent, in RELEASE_PRESSURE, Pressuer release failed");
            }
            m_ReleasePressure = 0;
            m_CurrentStep = PROCESSING_SAFE_REAGENT;
        }
        else
        {
            // Do nothing, just wait for the command response
        }
        break;
    case PROCESSING_SAFE_REAGENT:
        if (0 == m_ProcessingSafeReagent)
        {
            if("RG6" == m_ReagentGroup)
            {

            }
            else
            {
                mp_SchedulerController->GetHeatingStrategy()->StopTemperatureControl("RTSide");
                mp_SchedulerController->GetHeatingStrategy()->StopTemperatureControl("RTBottom");
            }
            m_ProcessingSafeReagent++;
        }
        else if(1 == m_ProcessingSafeReagent)
        {
            if (m_IsSafeReagentSuccessful)
            {
                mp_SchedulerController->SendTissueProtectMsg(true);
            }
            else
            {
                mp_SchedulerController->SendTissueProtectMsg(false);
            }
            m_ProcessingSafeReagent++;
        }
        if(2 == m_ProcessingSafeReagent && CTRL_CMD_DRAIN_SR == ctrlCmd)
        {
            mp_SchedulerController->LogDebug("RS_Safe_Reagent, processing safe reagent pass.");
            m_CurrentStep = DRAIN_SAFE_REAGENT;
            m_ProcessingSafeReagent = 0;
        }
        break;
    case DRAIN_SAFE_REAGENT:
        if (0 == m_DrainSafeReagent)
        {
            mp_SchedulerController->LogDebug("RS_Safe_Reagent, drain safe reagent.");
            OnMoveToTube();
            m_DrainSafeReagent++;
        }
        else if(1 == m_DrainSafeReagent)
        {
            if ("Scheduler::RVReqMoveToRVPosition" == cmdName)
            {
                if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                }
                else
                {
                    SendTasksDoneSig(false);
                }
            }
            else
            {
                if(mp_SchedulerStateHandler->IsRVRightPosition(TUBE_POS))
                {
                    m_DrainSafeReagent++;
                }
            }
        }
        else if(2 == m_DrainSafeReagent)
        {
            mp_SchedulerStateHandler->Drain();
            m_DrainSafeReagent++;
        }
        else if(3 == m_DrainSafeReagent)
        {
            if( "Scheduler::ALDraining"== cmdName)
            {
                mp_SchedulerStateHandler->OnStopDrain();
                m_DrainSafeReagent = 0;
               if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
               {
                   mp_SchedulerController->LogDebug("RS_Safe_Reagent, drain safe reagent passed.");
                   m_CurrentStep = SAFE_REAGENT_FINISHED;
               }
               else
               {
                   SendTasksDoneSig(false);
               }
            }
        }
        break;
    case SAFE_REAGENT_FINISHED:
        mp_SchedulerController->SendSafeReagentFinishedCmd();
        mp_SchedulerController->LogDebug("RS_Safe_Reagent is finished sucessfully.");
        SendTasksDoneSig(true);
        break;
    default:
        break;
	}
}

QString CRsTissueProtect::GetCurReagentType(const QString& lastReagentGroup)
{
    quint32 Scenario = mp_SchedulerStateHandler->GetCurrentScenario();

    ReturnCode_t EventId = mp_SchedulerController->GetCurErrEventID();

    //Firstly, check if the event id is related with Level Sensor or not
    if (DCL_ERR_DEV_RETORT_LEVELSENSOR_HEATING_OVERTIME == EventId
            || DCL_ERR_DEV_LEVELSENSOR_TEMPERATURE_OVERRANGE == EventId)
    {
        m_IsLevelSensorRelated = true;
    }
    else
    {
        m_IsLevelSensorRelated = false;
    }

    QString ret = "";
    if(200 == Scenario)
    {
        return "FIRST_STEP";
    }

    if ((211 == Scenario) || (221 == Scenario) ||
                (231 == Scenario) || (241 == Scenario) || (251 == Scenario))
    {
        if (lastReagentGroup.isEmpty())
            return "FIRST_STEP";
        else
            return lastReagentGroup;
    }

    if (Scenario >= 212 && Scenario <= 217)
    {
        return Global::FixationReagent;
    }

    if (Scenario >= 222 && Scenario <= 227)
    {
        return Global::WaterReagent;
    }

    if (Scenario >= 232 && Scenario <= 237)
    {
        return Global::DehydratingDiluted;
    }

    if (Scenario >= 242 && Scenario <= 247)
    {
        return Global::DehydratingAbsolute;
    }

    if (Scenario >= 252 && Scenario <= 257)
    {
        return Global::ClearingReagent;
    }

    if (Scenario >= 272 && Scenario <= 277)
    {
        return Global::ParaffinReagent;
    }

    //260:Protocol-GetReadyFor-Paraffin;
    //271:Protocol-Paraffin-Filling-LevelSensor-Heating
    //before fill paraffin
    if ((260 == Scenario || 271 == Scenario))
    {
        if (Global::ParaffinReagent == lastReagentGroup){
             ret = Global::ParaffinReagent;
        }
        else
        {
            if (mp_SchedulerStateHandler->CurProgramHasClearingReagent())
            {
                ret = Global::ClearingReagent;
            }
            else
            {
                ret = Global::ParaffinReagent;
            }
        }
    }

    return ret;
}

QString CRsTissueProtect::GetStationID()
{
    // Here the fact is that the current reagent has been filled, then the current step failed

    // Get reagent type
    QString reagentGroupOfLastStep = mp_SchedulerStateHandler->ReagentGroupOfLastStep();
    m_ReagentGroup = this->GetCurReagentType(reagentGroupOfLastStep);
    bool firstTimeUse = mp_SchedulerStateHandler->IsFirstTimeUseCurReagent(m_ReagentGroup);

    m_safeReagentStations.clear();
    bool ret = false;
    if ("FIRST_STEP" == m_ReagentGroup){
        ret = mp_SchedulerStateHandler->GetSafeReagentForSpecial(0, m_ReagentGroup, m_safeReagentStations);
        if (m_ReagentGroup == Global::WaterReagent){
            m_safeReagentStations.clear();
        }
    }
    else if(!m_ReagentGroup.isEmpty()){
        ret = mp_SchedulerStateHandler->GetSafeReagentStationList(m_ReagentGroup, true, firstTimeUse, m_safeReagentStations);
    }

    if (false == ret || m_safeReagentStations.empty())
    {
        mp_SchedulerController->LogDebug("In RS_Tissue_Protect, we can't find the safe reagent");
        return ""; // We can't find the safe reagent
    }
    m_CurSafeReagentIndex = 0;
    return m_safeReagentStations.at(0);
}

void CRsTissueProtect::OnMoveToTube()
{
    if ("" == m_StationID)
    {
        return;
    }
    mp_SchedulerStateHandler->UpdateCurProgramStepInfo(m_StationID, m_ReagentGroup);
    RVPosition_t RVPos = mp_SchedulerStateHandler->GetRVTubePositionByStationID(m_StationID);
    mp_SchedulerController->LogDebug(QString("In OnMoveToTube, tube position is %1").arg(RVPos));
    CmdRVReqMoveToRVPosition* cmd = new CmdRVReqMoveToRVPosition(500, m_Sender);
    cmd->SetRVPosition(RVPos);
    mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(cmd);
}

void CRsTissueProtect::OnMoveToSeal()
{
    RVPosition_t RVPos = mp_SchedulerStateHandler->GetRVSealPositionByStationID(m_StationID);
    mp_SchedulerController->LogDebug(QString("In OnMoveToSeal, seal position is %1").arg(RVPos));
    CmdRVReqMoveToRVPosition* cmd = new CmdRVReqMoveToRVPosition(500, m_Sender);
    cmd->SetRVPosition(RVPos);
    mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(cmd);
}

void CRsTissueProtect::OnReleasePressure()
{
    mp_SchedulerController->LogDebug("In RS_Tissue_Protect, begin to run CmdALReleasePressure");
    mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, m_Sender));
}

bool CRsTissueProtect::TryNextSafeReagent()
{
    if (m_safeReagentStations.size() > ++m_CurSafeReagentIndex){//other safe reagent is available, try to fill from it
        m_lastStationID = m_StationID;
        m_StationID = m_safeReagentStations.at(m_CurSafeReagentIndex);
        m_CurrentStep = DRAIN_CUR_REAGENT;
        m_IsSafeReagentSuccessful = true;
        mp_SchedulerController->LogDebug(QString("In TryNextSafeReagent, Filling failed, retry station:%1.").arg(m_StationID));
        return true;
    }

    return false;
}

}

