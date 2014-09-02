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
#include "Scheduler/Commands/Include/CmdALStopCmdExec.h"
#include "Scheduler/Commands/Include/CmdALReleasePressure.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdStationSuckDrain.h"

using namespace DeviceControl;

namespace Scheduler{

CRsTissueProtect::CRsTissueProtect(SchedulerMainThreadController* SchedController)
    :mp_SchedulerController(SchedController),m_StationID("")
{
    mp_StateMachine = QSharedPointer<QStateMachine>(new QStateMachine());

    mp_Init = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_StopFilling = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_StopDraining = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_MoveToTube = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_LevelSensorHeating = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_Filling = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_Wait8S = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_MoveToSealing = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_ReleasePressure = QSharedPointer<QState>(new QState(mp_StateMachine.data()));

    mp_StateMachine->setInitialState(mp_Init.data());

    mp_Init->addTransition(this, SIGNAL(MoveToTube()), mp_MoveToTube.data());
    mp_Init->addTransition(this, SIGNAL(StopFilling()), mp_StopFilling.data());
    mp_Init->addTransition(this, SIGNAL(StopDraining()), mp_StopDraining.data());
    mp_MoveToTube->addTransition(this, SIGNAL(LevelSensorHeating()), mp_LevelSensorHeating.data());
    mp_MoveToTube->addTransition(this,SIGNAL(Filling()), mp_Filling.data());
    mp_LevelSensorHeating->addTransition(this, SIGNAL(Filling()), mp_Filling.data());
    mp_Filling->addTransition(this, SIGNAL(MoveToSealing()), mp_MoveToSealing.data());
    mp_Filling->addTransition(this, SIGNAL(Wait8Seconds()), mp_Wait8S.data());
    mp_Wait8S->addTransition(this, SIGNAL(MoveToSealing()), mp_MoveToSealing.data());
    mp_MoveToSealing->addTransition(this,SIGNAL(ReleasePressure()), mp_ReleasePressure.data());
    mp_ReleasePressure->addTransition(this, SIGNAL(TasksDone(bool)), mp_Init.data());

	//For error cases
    mp_MoveToTube->addTransition(this, SIGNAL(TasksDone(bool)), mp_Init.data());
    mp_LevelSensorHeating->addTransition(this, SIGNAL(TasksDone(bool)), mp_Init.data());
    mp_Filling->addTransition(this, SIGNAL(TasksDone(bool)), mp_Init.data());
    mp_MoveToSealing->addTransition(this, SIGNAL(TasksDone(bool)), mp_Init.data());

    CONNECTSIGNALSLOT(mp_MoveToTube.data(), entered(), this, OnMoveToTube());
    CONNECTSIGNALSLOT(mp_MoveToSealing.data(), entered(), this, OnMoveToSeal());
    CONNECTSIGNALSLOT(mp_ReleasePressure.data(), entered(), this, OnReleasePressure());

    mp_StateMachine->start();
    m_IsLevelSensorRelated = false;
    m_MoveToTubeSeq = 0;
    m_FillSeq = 0;
    m_LevelSensorSeq = 0;
    m_MoveToSealSeq = 0;
    m_StopFillingSeq = 0;
    m_StopDrainingSeq = 0;
    m_StartWaitTime = 0;
    }

CRsTissueProtect::~CRsTissueProtect()
{
    mp_StateMachine->stop();
}


CRsTissueProtect::StateList_t CRsTissueProtect::GetCurrentState(QSet<QAbstractState*> statesList)
{
    StateList_t currentState = UNDEF;

    if (statesList.contains(mp_Init.data()))
    {
        currentState = INIT;
    }
    else if (statesList.contains(mp_StopFilling.data()))
    {
        currentState = STOP_FILLING;
    }
    else if (statesList.contains(mp_StopDraining.data()))
    {
        currentState = STOP_DRAINING;
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
	switch (currentState)
    {
    case INIT:
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
                TasksDone(false);
            }

            quint32 Scenario = mp_SchedulerController->GetCurrentScenario();
            // For filling
            if (QString::number(Scenario).left(1) == "2" && QString::number(Scenario).right(1) =="2")
            {
                emit StopFilling();
            }
            else if (QString::number(Scenario).left(1) == "2" && QString::number(Scenario).right(1) =="6") // For Draining
            {
                emit StopDraining();
            }
            else
            {
                emit MoveToTube();
            }

        }
         break;
    case STOP_FILLING:
        mp_SchedulerController->LogDebug("RS_Safe_Reagent, in Stop_Filling state");
        if (0 == m_StopFillingSeq)
        {
            CmdALStopCmdExec* ALStopCmd = new CmdALStopCmdExec(500, mp_SchedulerController);
            ALStopCmd->SetCmdType(0);
            mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(ALStopCmd);
        }
        else
        {
            if ("Scheduler::ALStopCmdExec" == cmdName)
            {
                m_StopFillingSeq = 0;
                emit MoveToTube();
            }
        }
        break;
    case STOP_DRAINING:
        mp_SchedulerController->LogDebug("RS_Safe_Reagent, in Stop_Draining state");
        if (0 == m_StopDrainingSeq)
        {
            CmdALStopCmdExec* ALStopCmd = new CmdALStopCmdExec(500, mp_SchedulerController);
            ALStopCmd->SetCmdType(1);
            mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(ALStopCmd);
        }
        else
        {
            if ("Scheduler::ALStopCmdExec" == cmdName)
            {
                m_StopDrainingSeq = 0;
                emit MoveToTube();
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
        else
        {
            RVPosition_t targetPos = mp_SchedulerController->GetRVTubePositionByStationID(m_StationID);
            if (targetPos == mp_SchedulerController->GetSchedCommandProcessor()->HardwareMonitor().PositionRV)
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
            mp_SchedulerController->FillRsTissueProtect(m_StationID);
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

                    }
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
            RVPosition_t sealPos = mp_SchedulerController->GetRVSealPositionByStationID(m_StationID);
            if (sealPos == mp_SchedulerController->GetSchedCommandProcessor()->HardwareMonitor().PositionRV)
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
            if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                m_MoveToTubeSeq = 0;
                m_FillSeq = 0;
                m_LevelSensorSeq = 0;
                m_MoveToSealSeq = 0;
                mp_SchedulerController->LogDebug("RS_Safe_Reagent, in RELEASE_PRESSURE, Pressuer release passed");
                emit TasksDone(true);
            }
            else
            {
                m_MoveToTubeSeq = 0;
                m_FillSeq = 0;
                m_LevelSensorSeq = 0;
                m_MoveToSealSeq = 0;
                mp_SchedulerController->LogDebug("RS_Safe_Reagent, in RELEASE_PRESSURE, Pressuer release failed");
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
    ReturnCode_t EventId = mp_SchedulerController->GetCurErrEventID();
    bool IsRTRVOVenError = false;
    //Firstly, check if the event id is related with Level Sensor or not
    if (DCL_ERR_DEV_RETORT_LEVELSENSOR_HEATING_OVERTIME == EventId
            || DCL_ERR_DEV_LEVELSENSOR_TEMPERATURE_OVERRANGE == EventId
            || DCL_ERR_DEV_MC_DC_5V_ASB15_OUTOFRANGE == EventId)
    {
        m_IsLevelSensorRelated = true;
    }
    else
    {
        m_IsLevelSensorRelated = false;
    }

    //Secondly, check if the event id is relatd with Retort, heating tube, RV and Oven failed error
    if (DCL_ERR_DEV_RETORT_BOTTOM_HEATING_ELEMENT_FAILED == EventId
            || DCL_ERR_DEV_RETORT_SIDETOP_HEATING_ELEMENT_FAILED == EventId
            || DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_OVERRANGE == EventId
            || DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_NOSIGNAL == EventId
            || DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_OVERRANGE == EventId
            || DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_NOSIGNAL == EventId
            || DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_OVERRANGE == EventId
            || DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_NOSIGNAL == EventId
            || DCL_ERR_DEV_RETORT_TSENSOR1_TO_2_SELFCALIBRATION_FAILED == EventId
            || DCL_ERR_DEV_WAXBATH_TOP_HEATINGPAD_CURRENT_OUTOFRANGE == EventId
            || DCL_ERR_DEV_WAXBATH_BOTTOM_HEATINGPAD_CURRENT_OUTOFRANGE == EventId
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
            || DCL_ERR_DEV_MC_DC_5V_ASB15_OUTOFRANGE == EventId
            || DCL_ERR_DEV_MC_VOLTAGE_24V_ASB15_OUTOFRANGE == EventId)
    {
        IsRTRVOVenError = true;
    }

    ReagentType_t ret = UNKNOWN;
    if (false == m_IsLevelSensorRelated && Scenario >= 200 && Scenario <= 221)
    {
        ret = Fixation;
    }
    if (false == m_IsLevelSensorRelated && Scenario >= 222 && Scenario <= 247)
    {
        ret = Concentration_Dehydration;
    }
    if (false == m_IsLevelSensorRelated && Scenario >= 251 && Scenario <= 271)
    {
        ret = Clearing;
    }
    if (false == m_IsLevelSensorRelated && Scenario >= 272 && Scenario <= 277)
    {
        if (false == IsRTRVOVenError)
        {
            ret = Paraffin;
        }
        else
        {
            ret = Clearing;
        }
    }

    if (true == m_IsLevelSensorRelated && Scenario >= 200 && Scenario <= 221)
    {
        ret = Fixation_Overflow;
    }
    if (true == m_IsLevelSensorRelated && Scenario >= 222 && Scenario <= 247)
    {
        ret = Concentration_Dehydration_Overflow;
    }
    if (true == m_IsLevelSensorRelated && Scenario >= 251 && Scenario <= 271)
    {
        ret = Clearing_Overflow;
    }
    if (true == m_IsLevelSensorRelated && Scenario >= 272 && Scenario <= 277)
    {
        if (false == IsRTRVOVenError)
        {
            ret = Clearing_Overflow;
        }
        else
        {
            ret = Paraffin_Overflow;
        }
    }

    return ret;
}

QString CRsTissueProtect::GetStationID()
{
    // Get reagent type
    ReagentType_t reagentType = this->GetReagentType();

    qint8 pos = -1;
    QQueue<ProgramStationInfo_t> ProgramStationList = mp_SchedulerController->GetProgramStationList();
    for (int i=0; i<ProgramStationList.size();++i)
    {
        if (Fixation == reagentType || Fixation_Overflow == reagentType)
        {
            if ("RG1" == ProgramStationList[i].ReagentGroupID)
            {
                pos = i;
                break;
            }
        }
        else if (Concentration_Dehydration == reagentType || Concentration_Dehydration_Overflow == reagentType)
        {
            if ("RG3" == ProgramStationList[i].ReagentGroupID)
            {
                pos = i;
                break;
            }
        }
        else if (Clearing == reagentType || Clearing_Overflow == reagentType)
        {
            if ("RG5" == ProgramStationList[i].ReagentGroupID)
            {
                pos = i;// For Clearing, we use the last one.
            }
        }
        else if (Paraffin == reagentType || Paraffin_Overflow == reagentType)
        {
            if ("RG6" == ProgramStationList[i].ReagentGroupID)
            {
                pos = i;
                break;
            }
        }
    }

    if (pos == -1)
    {
        mp_SchedulerController->LogDebug("In RS_Tissue_Protect, we can't find the safe reagent");
        return ""; // We can't find the safe reagent
    }
    return ProgramStationList[pos].StationID;
}

void CRsTissueProtect::OnMoveToTube()
{
    if ("" == m_StationID)
    {
        return;
    }
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

