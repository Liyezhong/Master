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
#include "Scheduler/Commands/Include/CmdIDForceDraining.h"
#include "Scheduler/Commands/Include/CmdALStopCmdExec.h"
#include "Scheduler/Commands/Include/CmdALReleasePressure.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToInitialPosition.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdStationSuckDrain.h"

using namespace DeviceControl;

namespace Scheduler{
/*lint -e534 */

CRsTissueProtect::CRsTissueProtect(SchedulerMainThreadController* SchedController)
    :mp_SchedulerController(SchedController),m_StationID("")
{
    mp_StateMachine = QSharedPointer<QStateMachine>(new QStateMachine());

    mp_Init = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_StopCmdExec = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_DrainCurReagent = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_MoveToTube = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_LevelSensorHeating = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_Filling = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_Wait8S = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_MoveToSealing = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_ReleasePressure = QSharedPointer<QState>(new QState(mp_StateMachine.data()));

    mp_StateMachine->setInitialState(mp_Init.data());

    mp_Init->addTransition(this, SIGNAL(DrainCurReagent()), mp_DrainCurReagent.data());
    mp_Init->addTransition(this, SIGNAL(StopCmdExec()), mp_StopCmdExec.data());
    mp_Init->addTransition(this, SIGNAL(MoveToTube()), mp_MoveToTube.data());
    mp_StopCmdExec->addTransition(this, SIGNAL(DrainCurReagent()), mp_DrainCurReagent.data());
    mp_DrainCurReagent->addTransition(this, SIGNAL(MoveToTube()), mp_MoveToTube.data());
    mp_MoveToTube->addTransition(this, SIGNAL(LevelSensorHeating()), mp_LevelSensorHeating.data());
    mp_MoveToTube->addTransition(this,SIGNAL(Filling()), mp_Filling.data());
    mp_LevelSensorHeating->addTransition(this, SIGNAL(Filling()), mp_Filling.data());
    mp_Filling->addTransition(this, SIGNAL(MoveToSealing()), mp_MoveToSealing.data());
    mp_Filling->addTransition(this, SIGNAL(Wait8Seconds()), mp_Wait8S.data());
    mp_Wait8S->addTransition(this, SIGNAL(MoveToSealing()), mp_MoveToSealing.data());
    mp_MoveToSealing->addTransition(this,SIGNAL(ReleasePressure()), mp_ReleasePressure.data());
    mp_ReleasePressure->addTransition(this, SIGNAL(TasksDone(bool)), mp_Init.data());

	//For error cases
    mp_StopCmdExec->addTransition(this, SIGNAL(TasksDone(bool)), mp_Init.data());
    mp_DrainCurReagent->addTransition(this, SIGNAL(TasksDone(bool)), mp_Init.data());
    mp_MoveToTube->addTransition(this, SIGNAL(TasksDone(bool)), mp_Init.data());
    mp_LevelSensorHeating->addTransition(this, SIGNAL(TasksDone(bool)), mp_Init.data());
    mp_Filling->addTransition(this, SIGNAL(TasksDone(bool)), mp_Init.data());
    mp_MoveToSealing->addTransition(this, SIGNAL(TasksDone(bool)), mp_Init.data());

    CONNECTSIGNALSLOT(mp_MoveToTube.data(), entered(), this, OnMoveToTube());
    CONNECTSIGNALSLOT(mp_MoveToSealing.data(), entered(), this, OnMoveToSeal());
    CONNECTSIGNALSLOT(mp_ReleasePressure.data(), entered(), this, OnReleasePressure());

    m_IsLevelSensorRelated = false;
    m_DrainCurReagentSeq = 0;
    m_MoveToTubeSeq = 0;
    m_FillSeq = 0;
    m_LevelSensorSeq = 0;
    m_MoveToSealSeq = 0;
    m_IsFillingSuccessful = true;
}


void CRsTissueProtect::Start()
{
    if (mp_StateMachine->isRunning())
    {
        mp_StateMachine->stop();
        // holde on 200 ms
        QTime delayTime = QTime::currentTime().addMSecs(200);
        while (QTime::currentTime() < delayTime)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }

    mp_StateMachine->start();

    m_IsLevelSensorRelated = false;
    m_DrainCurReagentSeq = 0;
    m_MoveToTubeSeq = 0;
    m_FillSeq = 0;
    m_LevelSensorSeq = 0;
    m_MoveToSealSeq = 0;
    m_IsFillingSuccessful = true;
}

CRsTissueProtect::~CRsTissueProtect()
{
    /*lint -e1551 */
    mp_StateMachine->stop();
}


CRsTissueProtect::StateList_t CRsTissueProtect::GetCurrentState(QSet<QAbstractState*> statesList)
{
    StateList_t currentState = UNDEF;

    if (statesList.contains(mp_Init.data()))
    {
        currentState = INIT;
    }
    else if (statesList.contains(mp_StopCmdExec.data()))
    {
        currentState = STOP_CMDEXEC;
    }
    else if (statesList.contains(mp_DrainCurReagent.data()))
    {
        currentState = DRAIN_CUR_REAGENT;
    }
    else if (statesList.contains(mp_MoveToTube.data()))
    {
           currentState = MOVE_TO_TUBE;
    }
    else if (statesList.contains(mp_LevelSensorHeating.data()))
    {
        currentState = LEVELSENSOR_HEATING;
    }
    else if (statesList.contains(mp_Filling.data()))
    {
        currentState = FILLING;
    }
    else if (statesList.contains(mp_Wait8S.data()))
    {
        currentState = WAIT_8S;
    }
    else if (statesList.contains(mp_MoveToSealing.data()))
    {
        currentState = MOVE_TO_SEALING;
    }
    else if (statesList.contains(mp_ReleasePressure.data()))
    {
        currentState = RELEASE_PRESSURE;
    }

    return currentState;
}


void CRsTissueProtect::HandleWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
    StateList_t currentState = this->GetCurrentState(mp_StateMachine->configuration());
    /*lint -e525 */
	switch (currentState)
    {
    case INIT:
        //Reset all the variable to initial value
        m_IsLevelSensorRelated = false;
        m_DrainCurReagentSeq = 0;
        m_MoveToTubeSeq = 0;
        m_FillSeq = 0;
        m_LevelSensorSeq = 0;
        m_MoveToSealSeq = 0;;
        m_StartWaitTime = 0;
        m_IsFillingSuccessful = true;

        m_StationID = this->GetStationID();
        if ("" == m_StationID)
        {
            TasksDone(false);
            break;
        }
        else
        {
            // Stop level sensor heating at first
            if (DCL_ERR_FCT_CALL_SUCCESS != mp_SchedulerController->GetHeatingStrategy()->StopTemperatureControl("LevelSensor"))
            {
                //TasksDone(false);
                // Based on comments from team members, we ignore the error
            }

            quint32 Scenario = mp_SchedulerController->GetCurrentScenario();
            bool ok = false;
            // For filling
            if (QString::number(Scenario).left(1) == "2" && QString::number(Scenario).right(1) =="2")
            {
                CmdALStopCmdExec* ALStopCmd = new CmdALStopCmdExec(500, mp_SchedulerController);
                ALStopCmd->SetCmdType(0);
                mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(ALStopCmd);
                emit StopCmdExec();
            }
            else if (QString::number(Scenario).left(1) == "2" && QString::number(Scenario).right(1) =="6") // For Draining
            {
                CmdALStopCmdExec* ALStopCmd = new CmdALStopCmdExec(500, mp_SchedulerController);
                ALStopCmd->SetCmdType(1);
                mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(ALStopCmd);
                emit StopCmdExec();
            }
            else if (QString::number(Scenario).right(1).toInt(&ok)>=2 && QString::number(Scenario).right(1).toInt(&ok) <= 6)
            {
                emit DrainCurReagent();
            }
            else
            {
                emit MoveToTube();
            }

        }
        break;
    case STOP_CMDEXEC:
        mp_SchedulerController->LogDebug("RS_Safe_Reagent, in Stop_CommandExcution state");
        if ("Scheduler::ALStopCmdExec" == cmdName)
        {
            emit DrainCurReagent();
        }
        break;
    case DRAIN_CUR_REAGENT:
        mp_SchedulerController->LogDebug("RS_Safe_Reagent, in Drain_cur_Reagent state");
        if (0 == m_DrainCurReagentSeq)
        {
            mp_SchedulerController->LogDebug("Send cmd to DCL to force Drain current reagent in RS_Tissue_Protect");
            CmdIDForceDraining* cmd  = new CmdIDForceDraining(500, mp_SchedulerController);
            QString stationID = mp_SchedulerController->GetCurrentStationID();
            mp_SchedulerController->LogDebug(QString("current station ID is: %1").arg(stationID));
            RVPosition_t tubePos = mp_SchedulerController->GetRVTubePositionByStationID(stationID);
            cmd->SetRVPosition((quint32)(tubePos));
            cmd->SetDrainPressure(40.0);
            mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(cmd);
            m_DrainCurReagentSeq++;
        }
        else
        {
            if ("Scheduler::IDForceDraining" == cmdName)
            {
                m_DrainCurReagentSeq = 0;
                if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    emit MoveToTube();
                }
                else
                {
                    TasksDone(false);
                }
            }
            else
            {
                // Do nothing
            }
        }
        break;
    case MOVE_TO_TUBE:
        mp_SchedulerController->LogDebug("RS_Safe_Reagent, in Move_to_Tube state");
        if (0 == m_MoveToTubeSeq)
        {
            if ("Scheduler::RVReqMoveToRVPosition" == cmdName)
            {
                if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    m_MoveToTubeSeq++;
                }
                else if (DCL_ERR_DEV_RV_MOTOR_LOSTCURRENTPOSITION == retCode)
                {
                    m_MoveToTubeSeq = 2;
                    mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(new CmdRVReqMoveToInitialPosition(500, mp_SchedulerController));
                }
                else
                {
                    m_MoveToTubeSeq = 0;
                    m_FillSeq = 0;
                    m_LevelSensorSeq = 0;
                    m_MoveToSealSeq = 0;
                    TasksDone(false);
                }
            }
        }
        else if (1 == m_MoveToTubeSeq)
        {
            if( mp_SchedulerController->IsRVRightPosition(TUBE_POS) )
            {
                m_MoveToTubeSeq = 0;
                mp_SchedulerController->LogDebug("RS_Safe_Reagent, in Move_To_Tube state, move to tube success");
                if (false == m_IsLevelSensorRelated)
                {
                    emit LevelSensorHeating();
                }
                else
                {
                    emit Filling();
                }
            }
            else
            {
                // Do nothing, just wait
            }
        }
        else if (2 == m_MoveToTubeSeq)
        {
            if( mp_SchedulerController->IsRVRightPosition(INITIALIZE_POS) )
            {
                this->OnMoveToTube();
                m_MoveToTubeSeq = 0; //rollback to move to tube

            }
        }
        break;
    case LEVELSENSOR_HEATING:
        mp_SchedulerController->LogDebug("RS_Safe_Reagent, in LevelSensor Heating state");
        if (0 == m_LevelSensorSeq)
        {
            if (DCL_ERR_FCT_CALL_SUCCESS != mp_SchedulerController->GetHeatingStrategy()->StartTemperatureControl("LevelSensor"))
            {
                m_MoveToTubeSeq = 0;
                m_FillSeq = 0;
                m_LevelSensorSeq = 0;
                m_MoveToSealSeq = 0;
                TasksDone(false);
            }
            else
            {
                m_LevelSensorSeq++;
            }
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
                m_MoveToTubeSeq = 0;
                m_FillSeq = 0;
                m_LevelSensorSeq = 0;
                m_MoveToSealSeq = 0;
                TasksDone(false);
            }
            else if (2 == retValue)
            {
                m_LevelSensorSeq = 0;
                emit Filling();
            }
        }
        break;
    case FILLING:
        mp_SchedulerController->LogDebug("RS_Safe_Reagent, in Filling state");
        if (0 == m_FillSeq)
        {
            if (true == m_IsLevelSensorRelated)
            {
                mp_SchedulerController->FillRsTissueProtect(m_StationID, false);
            }
            else
            {
                mp_SchedulerController->FillRsTissueProtect(m_StationID, true);
            }
            m_FillSeq++;
        }
        else
        {
            if( "Scheduler::ALFilling" == cmdName)
            {
                mp_SchedulerController->StopFillRsTissueProtect(m_StationID);
                m_FillSeq = 0;
                // Both in success and failire, we always move it to sealing position                    
                if (true == m_IsLevelSensorRelated)
                {
                    if (DCL_ERR_DEV_LA_FILLING_OVERFLOW == retCode)
                    {

                    }
                    else
                    {
                        m_IsFillingSuccessful = false;
                    }
                    CmdALDraining* cmd = new CmdALDraining(500, mp_SchedulerController);
                    cmd->SetDelayTime(5000);
                    mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(new CmdALDraining(500, mp_SchedulerController));
                    m_StartWaitTime = QDateTime::currentMSecsSinceEpoch();
                    emit Wait8Seconds();
                }
                else
                {
                    if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
                    {
                        mp_SchedulerController->LogDebug(QString("Program Step Filling OK"));
                    }
                    else
                    {
                        mp_SchedulerController->LogDebug(QString("Program Step Filling failed"));
                        m_IsFillingSuccessful = false;
                    }
                    emit MoveToSealing();
                }
            }
        }
        break;
    case WAIT_8S:
        mp_SchedulerController->LogDebug("RS_Safe_Reagent, in wait 8 seconds state");
        if ((QDateTime::currentMSecsSinceEpoch() - m_StartWaitTime) >= 8*1000)
        {
            m_StartWaitTime = 0;
            emit MoveToSealing();
        }
        else
        {
            // Do nothing, just wait
        }
        break;
    case MOVE_TO_SEALING:
        mp_SchedulerController->LogDebug("RS_Safe_Reagent, in Move_To_Seal state");
        if (0 == m_MoveToSealSeq)
        {
            if(("Scheduler::RVReqMoveToRVPosition" == cmdName))
            {
                if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    m_MoveToSealSeq++;
                }
                else // In this case, we just release pressure
                {
                    mp_SchedulerController->LogDebug("RS_Safe_Reagent, in Move_To_Seal state, move to seal failed");
                    emit ReleasePressure();
                }
            }
        }
        else
        {
            if( mp_SchedulerController->IsRVRightPosition(SEAL_POS) )
            {
                mp_SchedulerController->LogDebug("RS_Safe_Reagent, in Move_To_Seal state, move to seal passed");
                emit ReleasePressure();
            }
        }
        break;
    case RELEASE_PRESSURE:
        mp_SchedulerController->LogDebug("RS_Safe_Reagent, in state RELEASE_PRESSURE");
        if ("Scheduler::ALReleasePressure" == cmdName)
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
            if (m_IsFillingSuccessful)
            {
                emit TasksDone(true);
            }
            else
            {
                m_IsFillingSuccessful = true; //Reset the value
                emit TasksDone(false);
            }
        }
        else
        {
            // Do nothing, just wait for the command response
        }
        break;
    default:
        break;
	}
}

CRsTissueProtect::ReagentType_t CRsTissueProtect::GetReagentType()
{
    quint32 Scenario = mp_SchedulerController->GetCurrentScenario();
    if(200 == Scenario)
    {
        return FIRST_STEP;
    }
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
#if 0
    //Secondly, check if the event id is relatd with Retort, heating tube, RV and Oven failed error
    if (DCL_ERR_DEV_ASB5_AC_CURRENT_OUTOFRANGE == EventId
            || DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_OVERRANGE == EventId
            || DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_NOSIGNAL == EventId
            || DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_OVERRANGE == EventId
            || DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_NOSIGNAL == EventId
            || DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_OVERRANGE == EventId
            || DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_NOSIGNAL == EventId
            || DCL_ERR_DEV_RETORT_TSENSOR1_TO_2_SELFCALIBRATION_FAILED == EventId
            || DCL_ERR_DEV_WAXBATH_TSENSORUP_OUTOFRANGE == EventId
            || DCL_ERR_DEV_WAXBATH_TSENSORDOWN1_OUTOFRANGE == EventId
            || DCL_ERR_DEV_WAXBATH_TSENSORDOWN2_OUTOFRANGE == EventId
            || DCL_ERR_DEV_WAXBATH_SENSORUP_HEATING_ABNORMAL == EventId
            || DCL_ERR_DEV_WAXBATH_SENSORDOWN1_HEATING_ABNORMAL == EventId
            || DCL_ERR_DEV_WAXBATH_SENSORDOWN2_HEATING_ABNORMAL == EventId
            || DCL_ERR_DEV_WAXBATH_SENSORUP_HEATING_OUTOFTARGETRANGE == EventId
            || DCL_ERR_DEV_WAXBATH_SENSORDOWN1_HEATING_OUTOFTARGETRANGE == EventId
            || DCL_ERR_DEV_WAXBATH_SENSORDOWN2_HEATING_OUTOFTARGETRANGE == EventId
            || DCL_ERR_DEV_RV_HEATING_TEMPSENSOR1_OUTOFRANGE == EventId
            || DCL_ERR_DEV_RV_HEATING_TEMPSENSOR2_OUTOFRANGE == EventId
            || DCL_ERR_DEV_RV_HEATING_CURRENT_OUTOFRANGE == EventId
            || DCL_ERR_DEV_RV_HEATING_TEMPSENSOR2_NOTREACHTARGET == EventId
            || DCL_ERR_DEV_RV_HEATING_TSENSOR2_LESSTHAN_30DEGREEC_OVERTIME == EventId
            || DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_ABNORMAL == EventId
            || DCL_ERR_DEV_LA_TUBEHEATING_TUBE2_ABNORMAL == EventId
            || DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR1_OUTOFRANGE == EventId
            || DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR2_OUTOFRANGE == EventId
            || DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_NOTREACHTARGETTEMP == EventId
            || DCL_ERR_DEV_LA_TUBEHEATING_TUBE2_NOTREACHTARGETTEMP == EventId
            || DCL_ERR_DEV_MC_DC_5V_ASB3_OUTOFRANGE == EventId
            || DCL_ERR_DEV_MC_DC_5V_ASB5_OUTOFRANGE == EventId
            || DCL_ERR_DEV_MC_VOLTAGE_24V_ASB15_OUTOFRANGE == EventId)
    {
        IsRTRVOVenError = true;
    }
#endif

    ReagentType_t ret = UNKNOWN;
    if (false == m_IsLevelSensorRelated && Scenario >= 211 && Scenario <= 217)
    {
        ret = Fixation;
    }
    if (false == m_IsLevelSensorRelated && Scenario >= 221 && Scenario <= 247)
    {
        ret = Concentration_Dehydration;
    }
    if (false == m_IsLevelSensorRelated && Scenario >= 251 && Scenario <= 260)
    {
        ret = Clearing;
    }
    if (false == m_IsLevelSensorRelated && Scenario >= 271 && Scenario <= 277)
    {
        ret = Paraffin;
    }

    if (true == m_IsLevelSensorRelated && Scenario >= 211 && Scenario <= 217)
    {
        ret = Fixation_Overflow;
    }
    if (true == m_IsLevelSensorRelated && Scenario >= 221 && Scenario <= 247)
    {
        ret = Concentration_Dehydration_Overflow;
    }
    if (true == m_IsLevelSensorRelated && Scenario >= 251 && Scenario <= 260)
    {
        ret = Clearing_Overflow;
    }
    if (true == m_IsLevelSensorRelated && Scenario >= 271 && Scenario <= 277)
    {
        ret = Paraffin_Overflow;
    }

    return ret;
}

QString CRsTissueProtect::GetStationID()
{
    // Get reagent type
    ReagentType_t reagentType = this->GetReagentType();

    QList<QString> stationList;
    QString ReagentGroupID="";
    bool ret = false;
    switch (reagentType)
    {
    case FIRST_STEP:
        ret = mp_SchedulerController->GetSafeReagentForSpecial(0, ReagentGroupID, stationList);
        m_ReagentGroup = ReagentGroupID;
        break;
    case Fixation:
    case Fixation_Overflow:
        ret = mp_SchedulerController->GetSafeReagentStationList("RG1", stationList);
        m_ReagentGroup = "RG1";
        break;
    case Concentration_Dehydration:
    case Concentration_Dehydration_Overflow:
        ret = mp_SchedulerController->GetSafeReagentStationList("RG3", stationList);
        m_ReagentGroup = "RG3";
        break;
    case Clearing:
    case Clearing_Overflow:
        ret = mp_SchedulerController->GetSafeReagentStationList("RG5", stationList);
        m_ReagentGroup = "RG5";
        break;
    case Paraffin:
    case Paraffin_Overflow:
        ret = mp_SchedulerController->GetSafeReagentStationList("RG6", stationList);
        m_ReagentGroup = "RG6";
        break;
    default:
        break;
    }

    if (false == ret || stationList.empty())
    {
        mp_SchedulerController->LogDebug("In RS_Tissue_Protect, we can't find the safe reagent");
        return ""; // We can't find the safe reagent
    }
    return stationList.at(0);
}

void CRsTissueProtect::OnMoveToTube()
{
    if ("" == m_StationID)
    {
        return;
    }
    mp_SchedulerController->UpdateCurProgramStepInfo(m_StationID, m_ReagentGroup);
    RVPosition_t RVPos = mp_SchedulerController->GetRVTubePositionByStationID(m_StationID);
    mp_SchedulerController->LogDebug(QString("In OnMoveToTube, tube position is %1").arg(RVPos));
    CmdRVReqMoveToRVPosition* cmd = new CmdRVReqMoveToRVPosition(500, mp_SchedulerController);
    cmd->SetRVPosition(RVPos);
    mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(cmd);
}

void CRsTissueProtect::OnMoveToSeal()
{
    RVPosition_t RVPos = mp_SchedulerController->GetRVSealPositionByStationID(m_StationID);
    mp_SchedulerController->LogDebug(QString("In OnMoveToSeal, seal position is %1").arg(RVPos));
    CmdRVReqMoveToRVPosition* cmd = new CmdRVReqMoveToRVPosition(500, mp_SchedulerController);
    cmd->SetRVPosition(RVPos);
    mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(cmd);
}

void CRsTissueProtect::OnReleasePressure()
{
    mp_SchedulerController->LogDebug("In RS_Tissue_Protect, begin to run CmdALReleasePressure");
    mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, mp_SchedulerController));
}

}

