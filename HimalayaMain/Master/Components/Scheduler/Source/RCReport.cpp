/****************************************************************************/
/*! \file RCReport.cpp
 *
 *  \brief CRCReport class definition.
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
#include "../Include/RCReport.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief    Constructor
 *  \iparam   pStateMachine: Pointer to current state machine
 *  \iparam   pParentState: Pointer to parent state
 */
/****************************************************************************/
CRCReport::CRCReport(QStateMachine* pStateMachine, QState* pParentState) : CErrorHandlingSMBase(pStateMachine, pParentState)
{
    if(pParentState)
    {
        mp_RCReport = new QState(pParentState);
        QState *pInitState = (QState*)pParentState->initialState();
        pInitState->addTransition(this, SIGNAL(RCReport()), mp_RCReport);
        connect(mp_RCReport, SIGNAL(entered()), this, SIGNAL(OnRCReport()));
    }
}

/****************************************************************************/
/*!
 *  \brief    Destructor
 */
/****************************************************************************/
CRCReport::~CRCReport()
{
    delete mp_RCReport;
    mp_RCReport = NULL;
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
SchedulerStateMachine_t CRCReport::GetCurrentState(QSet<QAbstractState*> statesList)
{
    SchedulerStateMachine_t currentState = SM_UNDEF;
    if(statesList.contains(mp_RCReport))
    {
        currentState = SM_ERR_RC_REPORT;
    }

    return currentState;
}
}
