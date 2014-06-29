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
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/HeatingStrategy.h"

namespace Scheduler{

CRsStandbyWithTissue::CRsStandbyWithTissue(SchedulerMainThreadController* SchedController): mp_SchedulerController(SchedController)
{
    qRegisterMetaType<DeviceControl::RTTempCtrlType_t>("DeviceControl::RTTempCtrlType_t");
    mp_StateMachine = QSharedPointer<QStateMachine>(new QStateMachine());

    mp_RTBottomStopTempCtrl = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_RTSideStopTempCtrl = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_ShutdownFailedHeater = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_ReleasePressure = QSharedPointer<QState>(new QState(mp_StateMachine.data()));

    mp_StateMachine->setInitialState(mp_RTBottomStopTempCtrl.data());
    mp_RTBottomStopTempCtrl->addTransition(this, SIGNAL(RTStopTempCtrl(DeviceControl::RTTempCtrlType_t)), mp_RTSideStopTempCtrl.data());
    mp_RTSideStopTempCtrl->addTransition(this, SIGNAL(ShutdownFailedHeater()), mp_ShutdownFailedHeater.data());
    mp_ShutdownFailedHeater->addTransition(this, SIGNAL(ReleasePressure()), mp_ReleasePressure.data());
    mp_ReleasePressure->addTransition(this,SIGNAL(TasksDone(bool)), mp_RTBottomStopTempCtrl.data());

	//For error cases

    mp_RTSideStopTempCtrl->addTransition(this, SIGNAL(TasksDone(bool)), mp_RTBottomStopTempCtrl.data());
    mp_ShutdownFailedHeater->addTransition(this, SIGNAL(TasksDone(bool)), mp_RTBottomStopTempCtrl.data());

    mp_StateMachine->start();
}

CRsStandbyWithTissue::~CRsStandbyWithTissue()
{
    mp_StateMachine->stop();
}

CRsStandbyWithTissue::StateList_t CRsStandbyWithTissue::GetCurrentState(QSet<QAbstractState*> statesList)
{
    StateList_t currentState = UNDEF;

    if (statesList.contains(mp_RTBottomStopTempCtrl.data()))
    {
        currentState = RTBOTTOM_STOP_TEMPCTRL;
    }
    else if (statesList.contains(mp_RTSideStopTempCtrl.data()))
    {
        currentState = RTSIDE_STOP_TEMPCTRL;
    }
    else if(statesList.contains(mp_ShutdownFailedHeater.data()))
    {
        currentState = SHUTDOWN_FAILD_HEATER;
    }
    else if(statesList.contains(mp_ReleasePressure.data()))
    {
        currentState = RELEASE_PRESSURE;
    }

    return currentState;
}

void CRsStandbyWithTissue::HandleWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{

    StateList_t currentState = this->GetCurrentState(mp_StateMachine->configuration());
	switch (currentState)
	{
    case RTBOTTOM_STOP_TEMPCTRL:
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_SchedulerController->GetHeatingStrategy()->StopTemperatureControl("RTBottom"))
        {
            emit StopRTSideTempCtrl();
        }
        else
        {
            emit TasksDone(false);
        }
        break;
    case RTSIDE_STOP_TEMPCTRL:
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_SchedulerController->GetHeatingStrategy()->StopTemperatureControl("RTSide"))
        {
            emit ShutdownFailedHeater();
        }
        else
        {
            emit TasksDone(false);
        }
        break;
    case SHUTDOWN_FAILD_HEATER:
        emit ReleasePressure();
        break;
    case RELEASE_PRESSURE:
        emit TasksDone(true);
        break;
    default:
        break;
	}
}
}
