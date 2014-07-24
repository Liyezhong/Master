/****************************************************************************/
/*! \file RsPressureOverRange3SRetry.cpp
 *
 *  \brief CRsPressureOverRange3SRetry class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ July 24th, 2014
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

#include <QDateTime>
#include "Global/Include/Utils.h"
#include "Scheduler/Include/RsPressureOverRange3SRetry.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"


namespace Scheduler{

using namespace DeviceControl;
/****************************************************************************/
/*!
 *  \brief    Constructor
 *  \iparam   SchedController: Pointer to ScheduleMainThreadController 
 */
/****************************************************************************/
CRsPressureOverRange3SRetry::CRsPressureOverRange3SRetry(SchedulerMainThreadController* SchedController)
    :mp_SchedulerController(SchedController)
{
    mp_StateMachine = QSharedPointer<QStateMachine>(new QStateMachine());
    mp_CheckPressure = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_WaitFor1S = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_ReleasePressure = QSharedPointer<QState>(new QState(mp_StateMachine.data()));

    mp_StateMachine->setInitialState(mp_CheckPressure.data());

    mp_CheckPressure->addTransition(this, SIGNAL(WaitFor1S()), mp_WaitFor1S.data());
    mp_WaitFor1S->addTransition(this, SIGNAL(Retry()), mp_CheckPressure.data());
    mp_WaitFor1S->addTransition(this, SIGNAL(ReleasePressure()), mp_ReleasePressure.data());
    mp_ReleasePressure->addTransition(this, SIGNAL(TasksDone(bool)), mp_CheckPressure.data());
    CONNECTSIGNALSLOT(mp_ReleasePressure.data(), entered(), mp_SchedulerController,ReleasePressure());

    // For error cases
    mp_WaitFor1S->addTransition(this, SIGNAL(TasksDone(bool)), mp_CheckPressure.data());
    m_Counter = 0;
    m_CheckPressureTime = 0;
    mp_StateMachine->start();
}

/****************************************************************************/
/*!
 *  \brief    Destructor
 */
/****************************************************************************/
CRsPressureOverRange3SRetry::~CRsPressureOverRange3SRetry()
{
    mp_StateMachine->stop();
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
CRsPressureOverRange3SRetry::StateList_t CRsPressureOverRange3SRetry::GetCurrentState(QSet<QAbstractState*> statesList)
{
    StateList_t currentState = UNDEF;
    if(statesList.contains(mp_CheckPressure.data()))
    {
        currentState = CHECK_PRESSURE;
    }
    else if(statesList.contains(mp_WaitFor1S.data()))
    {
        currentState = WAIT_FOR_1S;
    }
    else if(statesList.contains(mp_ReleasePressure.data()))
    {
        currentState = RELEASE_PRESSURE;
    }

    return currentState;
}

void CRsPressureOverRange3SRetry::HandleWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
    StateList_t currentState = this->GetCurrentState(mp_StateMachine->configuration());
    qint64 now = 0;

    switch (currentState)
    {
    case CHECK_PRESSURE:
        mp_SchedulerController->LogDebug("RS_PressureOverRange_3SRetry, in state CHECK_PRESSURE");
        if (qAbs(mp_SchedulerController->GetSchedCommandProcessor()->HardwareMonitor().PressureAL) >= 40.0)
        {
            m_CheckPressureTime = QDateTime::currentMSecsSinceEpoch();
            m_Counter++;
            emit WaitFor1S();
        }
        else
        {
            emit TasksDone(true);
        }
        break;
    case WAIT_FOR_1S:
        mp_SchedulerController->LogDebug("RS_PressureOverRange_3SRetry, in state WAIT_FOR_1S");
        now = QDateTime::currentMSecsSinceEpoch();
        if ((now - m_CheckPressureTime) >= 1*1000)
        {
            if (m_Counter < 3)
            {
                emit Retry();
            }
            else
            {
                emit ReleasePressure();
            }
        }
        break;
    case RELEASE_PRESSURE:
        mp_SchedulerController->LogDebug("RS_PressureOverRange_3SRetry, in state RELEASE_PRESSURE");
        if ("Scheduler::ALReleasePressure" == cmdName)
        {
            // We always return failure
            if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                emit TasksDone(false);
            }
            else
            {
                emit TasksDone(false);
            }
        }
        break;
    default:
        break;
    }
}

}
