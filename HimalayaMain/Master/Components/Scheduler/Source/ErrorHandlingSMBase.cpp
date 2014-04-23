/****************************************************************************/
/*! \file ErrorHandlingSMBase.cpp
 *
 *  \brief CErrorHandlingSMBase class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 02.01.2014
 *   $Author:  $ Quan Zhang
 *
 *  \b Company:
 *
 *       Leica Biosystems SH CN.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "../Include/ErrorHandlingSMBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief    Constructor
 *  \iparam   pStateMachine: Pointer to current state machine
 *  \iparam   pParentState: Pointer to parent state
 */
/****************************************************************************/
CErrorHandlingSMBase::CErrorHandlingSMBase(QStateMachine* pStateMachine, QState* pParentState)
{
    if(pStateMachine)
    {
        mp_StateMachine = pStateMachine;
    }
    if(pParentState)
    {
        mp_ParentState = pParentState;
    }

}

/****************************************************************************/
/*!
 *  \brief    Destructor
 */
/****************************************************************************/
CErrorHandlingSMBase::~CErrorHandlingSMBase()
{
    mp_StateMachine = NULL;
    mp_ParentState = NULL;
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
SchedulerStateMachine_t CErrorHandlingSMBase::GetCurrentState(QSet<QAbstractState*> statesList)
{
    SchedulerStateMachine_t currentState = SM_UNDEF;
    if(statesList.contains(mp_ParentState))
    {
        currentState = SM_ERROR;
    }

    return currentState;
}


}
