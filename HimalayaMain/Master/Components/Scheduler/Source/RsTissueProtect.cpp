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

using namespace DeviceControl;

namespace Scheduler{

CRsTissueProtect::CRsTissueProtect(SchedulerMainThreadController* SchedController)
    :mp_SchedulerController(SchedController)
{
    mp_StateMachine = QSharedPointer<QStateMachine>(new QStateMachine());

    mp_MoveToTube = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_LevelSensorHeating = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_Filling = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_MoveToSealing = QSharedPointer<QState>(new QState(mp_StateMachine.data()));

    mp_StateMachine->setInitialState(mp_MoveToTube.data());

    mp_MoveToTube->addTransition(this, SIGNAL(LevelSensorHeating()), mp_LevelSensorHeating.data());
    mp_LevelSensorHeating->addTransition(this, SIGNAL(Filling()), mp_Filling.data());
    mp_Filling->addTransition(this, SIGNAL(MoveToSealing()), mp_MoveToSealing.data());
    mp_MoveToSealing->addTransition(this,SIGNAL(TasksDone(bool)), mp_MoveToTube.data());

	//For error cases
    mp_LevelSensorHeating->addTransition(this, SIGNAL(TasksDone(bool)), mp_MoveToTube.data());
    mp_Filling->addTransition(this, SIGNAL(TasksDone(bool)), mp_MoveToTube.data());

    CONNECTSIGNALSLOT(mp_MoveToTube.data(), entered(), this, OnMoveToTube());
    CONNECTSIGNALSLOT(mp_Filling.data(), entered(), mp_SchedulerController, Fill());
    CONNECTSIGNALSLOT(mp_MoveToSealing.data(), entered(), this, OnMoveToSeal());

    mp_StateMachine->start();
    m_StationID = this->GetStationID();
    m_MoveToTubeSeq = 0;
    m_LevelSensorSeq = 0;
    m_MoveToSealSeq = 0;
    }

CRsTissueProtect::~CRsTissueProtect()
{
    mp_StateMachine->stop();
}


CRsTissueProtect::StateList_t CRsTissueProtect::GetCurrentState(QSet<QAbstractState*> statesList)
{
    StateList_t currentState = UNDEF;

    if(statesList.contains(mp_MoveToTube.data()))
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
    else if (statesList.contains(mp_MoveToSealing.data()))
    {
        currentState = MOVE_TO_SEALING;
    }

    return currentState;
}


void CRsTissueProtect::HandleWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
    qreal tempLevelSensor = 0.0;
    quint16 retValue = 0;
    RVPosition_t targetPos = mp_SchedulerController->GetRVTubePositionByStationID(m_StationID);
    RVPosition_t sealPos = mp_SchedulerController->GetRVSealPositionByStationID(m_StationID);
    StateList_t currentState = this->GetCurrentState(mp_StateMachine->configuration());
	switch (currentState)
    {
    case MOVE_TO_TUBE:
        if ("" == m_StationID)
        {
            TasksDone(false);
        }
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
                    TasksDone(false);
                }
            }
        }
        else
        {
            if (targetPos == mp_SchedulerController->GetSchedCommandProcessor()->HardwareMonitor().PositionRV)
            {
                m_MoveToTubeSeq = 0;
                emit LevelSensorHeating();
            }
            else
            {
                // Do nothing, just wait
            }
        }
        break;
    case LEVELSENSOR_HEATING:
        if (0 == m_LevelSensorSeq)
        {
            if (DCL_ERR_FCT_CALL_SUCCESS != mp_SchedulerController->GetHeatingStrategy()->StartTemperatureControl("LevelSensor"))
            {
                TasksDone(false);
            }
            else
            {
                m_LevelSensorSeq++;
            }
        }
        else
        {
            tempLevelSensor = mp_SchedulerController->GetSchedCommandProcessor()->HardwareMonitor().TempALLevelSensor;
            retValue = mp_SchedulerController->GetHeatingStrategy()->CheckTemperatureOverTime("LevelSensor",tempLevelSensor);
            if (0 == retValue)
            {
                // Do nothing
            }
            else if (1 == retValue)
            {
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
        if( "Scheduler::ALFilling" == cmdName)
        {
            if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                mp_SchedulerController->LogDebug(QString("Program Step Filling OK"));
                emit MoveToSealing();
            }
            else
            {
                mp_SchedulerController->LogDebug(QString("Program Step Filling failed"));
                TasksDone(false);
            }
        }
		break;
    case MOVE_TO_SEALING:
        if (0 == m_MoveToSealSeq)
        {
            if(("Scheduler::RVReqMoveToRVPosition" == cmdName))
            {
                if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    m_MoveToSealSeq++;
                }
                else
                {
                    TasksDone(false);
                }
            }
        }
        else
        {
            if (sealPos == mp_SchedulerController->GetSchedCommandProcessor()->HardwareMonitor().PositionRV)
            {
                m_MoveToSealSeq = 0;
                TasksDone(true);
            }
        }
		break;
    default:
        break;
	}
}

CRsTissueProtect::ReagentType_t CRsTissueProtect::GetReagentType() const
{
    quint32 Scenario = mp_SchedulerController->GetCurrentScenario();
    ReturnCode_t EventId = mp_SchedulerController->GetCurErrEventID();
    bool IsLevelSensorRelated = false;
    bool IsRTRVOVenError = false;

    //Firstly, check if the event id is related with Level Sensor or not
    if (DCL_ERR_DEV_RETORT_LEVELSENSOR_HEATING_OVERTIME == EventId
            || DCL_ERR_DEV_LEVELSENSOR_TEMPERATURE_OVERRANGE == EventId
            || DCL_ERR_DEV_MC_DC_5V_ASB15_OUTOFRANGE == EventId)
    {
        IsLevelSensorRelated = true;
    }

    //Secondly, check if the event id is relatd with Retort, heating tube, RV and Oven failed error
    if ("50001" == QString::number(EventId).left(5)
            || "50002" == QString::number(EventId).left(5)
            || "50003" == QString::number(EventId).left(5))
    {
        IsRTRVOVenError = true;
    }

    ReagentType_t ret = UNKNOWN;
    if (false == IsLevelSensorRelated && Scenario >= 200 && Scenario <= 221)
    {
        ret = Fixation;
    }
    if (false == IsLevelSensorRelated && Scenario >= 222 && Scenario <= 247)
    {
        ret = Concentration_Dehydration;
    }
    if (false == IsLevelSensorRelated && Scenario >= 251 && Scenario <= 271)
    {
        ret = Clearing;
    }
    if (false == IsLevelSensorRelated && Scenario >= 272 && Scenario <= 277)
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

    if (true == IsLevelSensorRelated && Scenario >= 200 && Scenario <= 221)
    {
        ret = Fixation_Overflow;
    }
    if (true == IsLevelSensorRelated && Scenario >= 222 && Scenario <= 247)
    {
        ret = Concentration_Dehydration_Overflow;
    }
    if (true == IsLevelSensorRelated && Scenario >= 251 && Scenario <= 271)
    {
        ret = Clearing_Overflow;
    }
    if (true == IsLevelSensorRelated && Scenario >= 272 && Scenario <= 277)
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
            if ("RG1" == ProgramStationList[pos].ReagentGroupID)
            {
                pos = i;
                break;
            }
        }
        else if (Concentration_Dehydration == reagentType || Concentration_Dehydration_Overflow == reagentType)
        {
            if ("RG3" == ProgramStationList[pos].ReagentGroupID)
            {
                pos = i;
                break;
            }
        }
        else if (Clearing == reagentType || Clearing_Overflow == reagentType)
        {
            if ("RG5" == ProgramStationList[pos].ReagentGroupID)
            {
                pos = i;// For Clearing, we use the last one.
            }
        }
        else if (Paraffin == reagentType || Paraffin_Overflow == reagentType)
        {
            if ("RG6" == ProgramStationList[pos].ReagentGroupID)
            {
                pos = i;
                break;
            }
        }
    }

    if (pos == -1)
    {
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
    CmdRVReqMoveToRVPosition* cmd = new CmdRVReqMoveToRVPosition(500, mp_SchedulerController);
    cmd->SetRVPosition(RVPos);
    mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(cmd);
}

void CRsTissueProtect::OnMoveToSeal()
{
    RVPosition_t RVPos = mp_SchedulerController->GetRVSealPositionByStationID(m_StationID);
    CmdRVReqMoveToRVPosition* cmd = new CmdRVReqMoveToRVPosition(500, mp_SchedulerController);
    cmd->SetRVPosition(RVPos);
    mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(cmd);
}

}

