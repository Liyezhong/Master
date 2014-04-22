/****************************************************************************/
/*! \file RsStandbyWithTissue.cpp
 *
 *  \brief CRsStandbyWithTissue class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 22.04.2014
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


#include "Scheduler/Include/RsStandbyWithTissue.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief    Constructor
 *  \iparam   pStateMachine: Pointer to current state machine
 *  \iparam   pParentState: Pointer to parent state
 */
/****************************************************************************/
CRsStandbyWithTissue::CRsStandbyWithTissue(QStateMachine* pStateMachine, QState* pParentState) : CErrorHandlingSMBase(pStateMachine, pParentState)
{
    if(pParentState)
    {
        mp_ReleasePressure = new QState(pParentState);
        mp_ShutdownFailedHeater = new QState(pParentState);
        mp_RTBottomStopTempCtrl = new QState(pParentState);
        mp_RTTopStopTempCtrl = new QState(pParentState);
        QState *pInitState = (QState*)pParentState->initialState();

        pInitState->addTransition(this, SIGNAL(RTBottomStopTempCtrl()), mp_RTBottomStopTempCtrl);
        mp_RTBottomStopTempCtrl->addTransition(this, SIGNAL(RTTopStopTempCtrl()), mp_ReleasePressure);
        mp_ReleasePressure->addTransition(this, SIGNAL(ShutdownFailedHeater()), mp_ShutdownFailedHeater);
        mp_ShutdownFailedHeater->addTransition(this, SIGNAL(ShutdownFailedHeaterFinished()), pInitState);
        connect(mp_ReleasePressure, SIGNAL(entered()), this, SIGNAL(OnReleasePressure()));

        connect(mp_ShutdownFailedHeater, SIGNAL(entered()), this, SIGNAL(OnShutdownFailedHeater()));
        connect(mp_RTBottomStopTempCtrl, SIGNAL(entered()), this, SIGNAL(OnRTBottomStopTempCtrl()));
        connect(mp_RTTopStopTempCtrl, SIGNAL(entered()), this, SIGNAL(OnRTTopStopTempCtrl()));
    }
}

/****************************************************************************/
/*!
 *  \brief    Destructor
 */
/****************************************************************************/
CRsStandbyWithTissue::~CRsStandbyWithTissue()
{
    delete mp_ReleasePressure;
    mp_ReleasePressure = NULL;

    delete mp_ShutdownFailedHeater;
    mp_ShutdownFailedHeater = NULL;

    delete mp_RTBottomStopTempCtrl;
    mp_RTBottomStopTempCtrl = NULL;

    delete mp_RTTopStopTempCtrl;
    mp_RTTopStopTempCtrl = NULL;
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
SchedulerStateMachine_t CRsStandbyWithTissue::GetCurrentState(QSet<QAbstractState*> statesList)
{
    SchedulerStateMachine_t currentState = SM_UNDEF;
    if(statesList.contains(mp_ReleasePressure))
    {
        currentState = SM_ERR_RS_RELEASE_PRESSURE_AT_RS_STADNDBY_WITHTISSUE;
    }
    else if(statesList.contains(mp_ShutdownFailedHeater))
    {
        currentState = SM_ERR_RS_SHUTDOWN_FAILED_HEATER_AT_RS_STADNDBY_WITHTISSUE;
    }
    else if (statesList.contains(mp_RTBottomStopTempCtrl))
    {
        currentState = SM_ERR_RS_RT_BOTTOM_STOP_TEMP_CTRL_AT_RS_STADNDBY_WITHTISSUE;
    }
    else if (statesList.contains(mp_RTTopStopTempCtrl))
    {
        currentState = SM_ERR_RS_RT_TOP_STOP_TEMP_CTRL_AT_RS_STADNDBY_WITHTISSUE;
    }
    return currentState;
}
}
