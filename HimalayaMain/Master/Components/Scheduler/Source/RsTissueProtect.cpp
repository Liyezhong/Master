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
    mp_LevelSensorHeating->addTransition(this, SIGNAL(Filling()), mp_Filling.data()); //For RS_Standby
    mp_Filling->addTransition(this, SIGNAL(MoveToSealing()), mp_MoveToSealing.data());
    mp_MoveToSealing->addTransition(this,SIGNAL(TasksDone(bool)), mp_MoveToTube.data());

	//For error cases
    mp_LevelSensorHeating->addTransition(this, SIGNAL(TasksDone(bool)), mp_MoveToTube.data());
    mp_Filling->addTransition(this, SIGNAL(TasksDone(bool)), mp_MoveToTube.data());

    m_ProgramStationList = mp_SchedulerController->GetProgramStationList();

    mp_StateMachine->start();
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

    StateList_t currentState = this->GetCurrentState(mp_StateMachine->configuration());
	switch (currentState)
    {
    case MOVE_TO_TUBE:
        break;
    case LEVELSENSOR_HEATING:
        break;
    case FILLING:
		break;
    case MOVE_TO_SEALING:
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
    if (DCL_ERR_DEV_RETORT_LEVELSENSOR_HEATING_OVERTIME == EventId || DCL_ERR_DEV_LEVELSENSOR_TEMPERATURE_OVERRANGE == EventId)
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

}

