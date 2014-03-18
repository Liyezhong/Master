/****************************************************************************/
/*! \file RsStandby.cpp
 *
 *  \brief CRsStandby class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 18.03.2014
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


#include "../Include/RsStandby.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief    Constructor
 *  \iparam   pStateMachine: Pointer to current state machine
 *  \iparam   pParentState: Pointer to parent state
 */
/****************************************************************************/
CRsStandby::CRsStandby(QStateMachine* pStateMachine, QState* pParentState) : CErrorHandlingSMBase(pStateMachine, pParentState)
{
    if(pParentState)
    {
        mp_ReleasePressure = new QState(pParentState);
        mp_ShutdownFailedHeater = new QState(pParentState);
        QState *pInitState = (QState*)pParentState->initialState();
        pInitState->addTransition(this, SIGNAL(ReleasePressure()), mp_ReleasePressure);
        mp_ReleasePressure->addTransition(this, SIGNAL(ShutdownFailedHeater()), mp_ShutdownFailedHeater);
        mp_ShutdownFailedHeater->addTransition(this, SIGNAL(ShutdownFailedHeaterFinished()), pInitState);
        connect(mp_ReleasePressure, SIGNAL(entered()), this, SIGNAL(OnReleasePressure()));
        connect(mp_ShutdownFailedHeater, SIGNAL(entered()), this, SIGNAL(OnShutdownFailedHeater()));
    }
}

/****************************************************************************/
/*!
 *  \brief    Destructor
 */
/****************************************************************************/
CRsStandby::~CRsStandby()
{
    delete mp_ReleasePressure;
    mp_ReleasePressure = NULL;
    delete mp_ShutdownFailedHeater;
    mp_ShutdownFailedHeater = NULL;
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
SchedulerStateMachine_t CRsStandby::GetCurrentState(QSet<QAbstractState*> statesList)
{
    SchedulerStateMachine_t currentState = SM_UNDEF;
    if(statesList.contains(mp_ReleasePressure))
    {
        currentState = SM_ERR_RS_RELEASE_PRESSURE;
    }
    else if(statesList.contains(mp_ShutdownFailedHeater))
    {
        currentState = SM_ERR_RS_SHUTDOWN_FAILED_HEATER;
    }

    return currentState;
}
}
