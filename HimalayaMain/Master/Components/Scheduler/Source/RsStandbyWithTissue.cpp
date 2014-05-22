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
        mp_Initial = new QState(pParentState);
        mp_RTBottomStopTempCtrl = new QState(pParentState);
        mp_RTSideStopTempCtrl = new QState(pParentState);
        mp_ShutdownFailedHeater = new QState(pParentState);
        mp_ReleasePressure = new QState(pParentState);
        pParentState->setInitialState(mp_Initial);

        mp_Initial->addTransition(this, SIGNAL(RTBottomStopTempCtrl()), mp_RTBottomStopTempCtrl);
        mp_RTBottomStopTempCtrl->addTransition(this, SIGNAL(RTSideStopTempCtrl()), mp_RTSideStopTempCtrl);
        mp_RTSideStopTempCtrl->addTransition(this, SIGNAL(ShutdownFailedHeater()), mp_ShutdownFailedHeater);
        mp_ShutdownFailedHeater->addTransition(this, SIGNAL(ReleasePressure()), mp_ReleasePressure);
        mp_ReleasePressure->addTransition(this,SIGNAL(TasksDone(bool)), mp_Initial);

		//For error cases
        mp_RTBottomStopTempCtrl->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial);
        mp_RTSideStopTempCtrl->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial);
        mp_ShutdownFailedHeater->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial);
    }
}

/****************************************************************************/
/*!
 *  \brief    Destructor
 */
/****************************************************************************/
CRsStandbyWithTissue::~CRsStandbyWithTissue()
{
    delete mp_RTBottomStopTempCtrl;
    mp_RTBottomStopTempCtrl = NULL;

    delete mp_RTSideStopTempCtrl;
    mp_RTSideStopTempCtrl = NULL;

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
SchedulerStateMachine_t CRsStandbyWithTissue::GetCurrentState(QSet<QAbstractState*> statesList)
{
    SchedulerStateMachine_t currentState = SM_ERR_RS_STANDBY_WITH_TISSUE;

    if (statesList.contains(mp_RTBottomStopTempCtrl))
    {
        currentState = SM_ERR_RS_STANDBY_WITH_TISSUE_RT_BOTTOM_STOP_TEMPCTRL;
    }
    else if (statesList.contains(mp_RTSideStopTempCtrl))
    {
        currentState =SM_ERR_RS_STANDBY_WITH_TISSUE_RT_SIDE_STOP_TEMPCTRL;
    }
    else if(statesList.contains(mp_ShutdownFailedHeater))
    {
        currentState =SM_ERR_RS_STANDBY_WITH_TISSUE_SHUTDOWN_FAILED_HEATER;
    }
    else if(statesList.contains(mp_ReleasePressure))
    {
        currentState = SM_ERR_RS_STANDBY_WITH_TISSUE_RELEASE_PRESSURE;
    }

    return currentState;
}

void CRsStandbyWithTissue::OnHandleWorkFlow(bool flag)
{
    if (false == flag)
    {
        emit TasksDone(false);
        return;
    }
	SchedulerStateMachine_t currentState = this->GetCurrentState(mp_StateMachine->configuration());
	switch (currentState)
	{
    case SM_ERR_RS_STANDBY_WITH_TISSUE:
        emit RTStopTempCtrl(DeviceControl::RT_BOTTOM);
        break;
	case SM_ERR_RS_STANDBY_WITH_TISSUE_RT_BOTTOM_STOP_TEMPCTRL:
        emit RTStopTempCtrl(DeviceControl::RT_SIDE);
        break;
    case SM_ERR_RS_STANDBY_WITH_TISSUE_RT_SIDE_STOP_TEMPCTRL:
        emit ShutdownFailedHeater();
        break;
    case SM_ERR_RS_STANDBY_WITH_TISSUE_SHUTDOWN_FAILED_HEATER:
        emit ReleasePressure();
        break;
    case SM_ERR_RS_STANDBY_WITH_TISSUE_RELEASE_PRESSURE:
        emit TasksDone(true);
        break;
    default:
        break;
	}
}
}
