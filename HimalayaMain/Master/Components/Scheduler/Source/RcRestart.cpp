/****************************************************************************/
/*! \file	RcRestart.cpp
 *
 *  \brief	Implementation file of class RC_Restart
 *
 *   $Version: $ 0.1
 *   $Date:    $ June 3rd, 2014
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


#include "Scheduler/Include/RcRestart.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief    Constructor
 *  \iparam   pStateMachine: Pointer to current state machine
 *  \iparam   pParentState: Pointer to parent state
 */
/****************************************************************************/
CRcRestart::CRcRestart(QStateMachine* pStateMachine, QState* pParentState) : CErrorHandlingSMBase(pStateMachine, pParentState)
{
    if(pParentState)
    {
        mp_Initial = new QState(pParentState);
        mp_Recover = new QState(pParentState);
        pParentState->setInitialState(mp_Initial);

        mp_Initial->addTransition(this, SIGNAL(Recover()), mp_Recover);
        mp_Recover->addTransition(this,SIGNAL(TasksDone(bool)), mp_Initial);
    }
}

/****************************************************************************/
/*!
 *  \brief    Destructor
 */
/****************************************************************************/
CRcRestart::~CRcRestart()
{
    delete mp_Initial;
    mp_Initial = NULL;

    delete mp_Recover;
    mp_Recover = NULL;
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
SchedulerStateMachine_t CRcRestart::GetCurrentState(QSet<QAbstractState*> statesList)
{
    SchedulerStateMachine_t currentState = SM_ERR_RC_RESTART;

    if (statesList.contains(mp_Initial))
    {
        currentState = SM_ERR_RC_RESTART;
    }

    return currentState;
}

void CRcRestart::OnHandleWorkFlow(bool flag)
{
    if (false == flag)
    {
        emit TasksDone(false);
    }
    SchedulerStateMachine_t currentState = this->GetCurrentState(mp_StateMachine->configuration());
    switch (currentState)
    {
    case SM_ERR_RC_RESTART:
        emit Recover();
        break;
    default:
        break;
    }
}

}
