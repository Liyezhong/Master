/****************************************************************************/
/*! \file	RcLevelsensorHeatingOvertime.cpp
 *
 *  \brief	Implementation file of class RC_LevelSensor_Heating_Overtime 
 *
 *   $Version: $ 0.1
 *   $Date:    $ May 23rd, 2014
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


#include "Scheduler/Include/RcLevelsensorHeatingOvertime.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief    Constructor
 *  \iparam   pStateMachine: Pointer to current state machine
 *  \iparam   pParentState: Pointer to parent state
 */
/****************************************************************************/
CRcLevelSensorHeatingOvertime::CRcLevelSensorHeatingOvertime(QStateMachine* pStateMachine, QState* pParentState) : CErrorHandlingSMBase(pStateMachine, pParentState)
{
    if(pParentState)
    {
        mp_Initial = new QState(pParentState);
        mp_RestartLevelSensorTempControl = new QState(pParentState);
        pParentState->setInitialState(mp_Initial);

        mp_Initial->addTransition(this, SIGNAL(RestartLevelSensorTempControl()), mp_RestartLevelSensorTempControl);
        mp_RestartLevelSensorTempControl->addTransition(this,SIGNAL(TasksDone(bool)), mp_Initial);
    }
}

/****************************************************************************/
/*!
 *  \brief    Destructor
 */
/****************************************************************************/
CRcLevelSensorHeatingOvertime::~CRcLevelSensorHeatingOvertime()
{
    delete mp_Initial;
    mp_Initial = NULL;

    delete mp_RestartLevelSensorTempControl;
    mp_RestartLevelSensorTempControl = NULL;
}

/****************************************************************************/
/*!
 *  \brief   Return the current state of the state machine
 *  \iparam statesList = A list contains the current state, usually get from
 *                       the State Machine current states belogs to
 *
 *  \return The current state of the state machine
 */
/****************************************************************************/
SchedulerStateMachine_t CRcLevelSensorHeatingOvertime::GetCurrentState(QSet<QAbstractState*> statesList)
{
    SchedulerStateMachine_t currentState = SM_ERR_RC_LEVELSENSOR_HEATING_OVERTIME;

    if (statesList.contains(mp_Initial))
    {
        currentState = SM_ERR_RC_LEVELSENSOR_HEATING_OVERTIME;
    }
    else if (statesList.contains(mp_RestartLevelSensorTempControl))
    {
        currentState = SM_ERR_RC_LEVELSENSOR_HEATING_OVERTIME_RESTART_LEVELSENSOR_TEMPCTRL;
    }

    return currentState;
}

void CRcLevelSensorHeatingOvertime::OnHandleWorkFlow(bool flag)
{
    if (false == flag)
    {
        emit TasksDone(false);
        return;
    }
	SchedulerStateMachine_t currentState = this->GetCurrentState(mp_StateMachine->configuration());
	switch (currentState)
	{
    case SM_ERR_RC_LEVELSENSOR_HEATING_OVERTIME:
        emit RestartLevelSensorTempControl();
        break;
	case SM_ERR_RC_LEVELSENSOR_HEATING_OVERTIME_RESTART_LEVELSENSOR_TEMPCTRL:
        emit TasksDone(true);
        break;
    default:
        break;
	}
}
}
