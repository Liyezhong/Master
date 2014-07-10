/****************************************************************************/
/*! \file RsHeatingErr30SRetry.cpp
 *
 *  \brief CRsHeatingErr30Retry class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ June 12th, 2014
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


#include "Scheduler/Include/RsHeatingErr30SRetry.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief    Constructor
 *  \iparam   pStateMachine: Pointer to current state machine
 *  \iparam   pParentState: Pointer to parent state
 */
/****************************************************************************/
CRsHeatingErr30SRetry::CRsHeatingErr30SRetry(QStateMachine* pStateMachine, QState* pParentState)
{
    m_counter = 0;

    if(pParentState)
    {
        mp_Initial = new QState(pParentState);
        mp_StopTempCtrl = new QState(pParentState);
        mp_StartTempCtrl = new QState(pParentState);
        mp_CheckDevStatus = new QState(pParentState);
        pParentState->setInitialState(mp_Initial);

        mp_Initial->addTransition(this, SIGNAL(StopTempCtrl()), mp_StopTempCtrl);
        mp_StopTempCtrl->addTransition(this, SIGNAL(StartTempCtrl()), mp_StartTempCtrl);
        mp_StartTempCtrl->addTransition(this, SIGNAL(CheckDevStatus()), mp_CheckDevStatus);
        mp_CheckDevStatus->addTransition(this,SIGNAL(TasksDone(bool)), mp_Initial);

        //For Retry
        mp_StopTempCtrl->addTransition(this, SIGNAL(Retry()), mp_Initial);
        mp_StartTempCtrl->addTransition(this, SIGNAL(Retry()), mp_Initial);
        mp_CheckDevStatus->addTransition(this,SIGNAL(Retry()), mp_Initial);

		//For error cases
        mp_StopTempCtrl->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial);
        mp_StartTempCtrl->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial);
        mp_CheckDevStatus->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial);
    }
}

/****************************************************************************/
/*!
 *  \brief    Destructor
 */
/****************************************************************************/
CRsHeatingErr30SRetry::~CRsHeatingErr30SRetry()
{
    delete mp_Initial;
    mp_Initial = NULL;

    delete mp_StopTempCtrl;
    mp_StopTempCtrl = NULL;

    delete mp_StartTempCtrl;
    mp_StartTempCtrl = NULL;

    delete mp_CheckDevStatus;
    mp_CheckDevStatus = NULL;
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
SchedulerStateMachine_t CRsHeatingErr30SRetry::GetCurrentState(QSet<QAbstractState*> statesList)
{
    SchedulerStateMachine_t currentState = SM_ERR_RS_HEATINGERR30SRETRY;

    if (statesList.contains(mp_Initial))
    {
        currentState = SM_ERR_RS_HEATINGERR30SRETRY;
    }
    return currentState;
}

void CRsHeatingErr30SRetry::OnHandleWorkFlow(bool flag)
{
    if (false == flag)
    {
        emit TasksDone(false);
        return;
    }
}
}
