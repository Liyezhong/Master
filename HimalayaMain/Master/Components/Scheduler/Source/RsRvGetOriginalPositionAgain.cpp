/****************************************************************************/
/*! \file RsRvGetOriginalPositionAgain.cpp
 *
 *  \brief CRsRvGetOriginalPositionAgain class definition.
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


#include "../Include/RsRvGetOriginalPositionAgain.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief    Constructor
 *  \iparam   pStateMachine: Pointer to current state machine
 *  \iparam   pParentState: Pointer to parent state
 */
/****************************************************************************/
CRsRvGetOriginalPositionAgain::CRsRvGetOriginalPositionAgain(QStateMachine* pStateMachine, QState* pParentState) : CErrorHandlingSMBase(pStateMachine, pParentState)
{
    if(pParentState)
    {
        mp_RvMovingToInitialPosition = new QState(pParentState);
        QState *pInitState = (QState*)pParentState->initialState();
        pInitState->addTransition(this, SIGNAL(RvMoveToInitPosition()), mp_RvMovingToInitialPosition);
        mp_RvMovingToInitialPosition->addTransition(this, SIGNAL(RvMoveToInitPositionFinished()), pInitState);
        connect(mp_RvMovingToInitialPosition, SIGNAL(entered()), this, SIGNAL(OnRvMoveToInitPosition()));
    }
}

/****************************************************************************/
/*!
 *  \brief    Destructor
 */
/****************************************************************************/
CRsRvGetOriginalPositionAgain::~CRsRvGetOriginalPositionAgain()
{
    delete mp_RvMovingToInitialPosition;
    mp_RvMovingToInitialPosition = NULL;
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
SchedulerStateMachine_t CRsRvGetOriginalPositionAgain::GetCurrentState(QSet<QAbstractState*> statesList)
{
    SchedulerStateMachine_t currentState = SM_UNDEF;
    if(statesList.contains(mp_RvMovingToInitialPosition))
    {
        currentState = SM_ERR_RS_RV_MOVING_TO_INIT_POS;
    }

    return currentState;
}
}
