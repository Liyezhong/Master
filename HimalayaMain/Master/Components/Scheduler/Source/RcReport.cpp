/****************************************************************************/
/*! \file RcReport.cpp
 *
 *  \brief CRcReport class definition.
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
#include "Scheduler/Include/RcReport.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief    Constructor
 *  \iparam   pStateMachine: Pointer to current state machine
 *  \iparam   pParentState: Pointer to parent state
 */
/****************************************************************************/
CRcReport::CRcReport(QStateMachine* pStateMachine, QState* pParentState) : CErrorHandlingSMBase(pStateMachine, pParentState)
{
    if(pParentState)
    {
        mp_RcReport = new QState(pParentState);
        QState *pInitState = (QState*)pParentState->initialState();
        pInitState->addTransition(this, SIGNAL(RcReport()), mp_RcReport);
        connect(mp_RcReport, SIGNAL(entered()), this, SIGNAL(OnRcReport()));
    }
}

/****************************************************************************/
/*!
 *  \brief    Destructor
 */
/****************************************************************************/
CRcReport::~CRcReport()
{
    delete mp_RcReport;
    mp_RcReport = NULL;
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
SchedulerStateMachine_t CRcReport::GetCurrentState(QSet<QAbstractState*> statesList)
{
    SchedulerStateMachine_t currentState = SM_UNDEF;
    if(statesList.contains(mp_RcReport))
    {
        currentState = SM_UNDEF;
    }

    return currentState;
}
}
