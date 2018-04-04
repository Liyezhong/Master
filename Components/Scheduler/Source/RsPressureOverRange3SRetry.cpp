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
CRsPressureOverRange3SRetry::CRsPressureOverRange3SRetry(SchedulerMainThreadController* SchedController, CSchedulerStateMachine* StateMachine)
    :mp_SchedulerController(SchedController)
    ,mp_StateMachine(StateMachine)
{
    m_CurrentState = CHECK_PRESSURE;
    m_Counter = 0;
    m_CheckPressureTime = 0;
}

void CRsPressureOverRange3SRetry::Start()
{
    m_CurrentState = CHECK_PRESSURE;
    m_Counter = 0;
    m_CheckPressureTime = 0;
}

/****************************************************************************/
/*!
 *  \brief    Destructor
 */
/****************************************************************************/
CRsPressureOverRange3SRetry::~CRsPressureOverRange3SRetry()
{
}

void CRsPressureOverRange3SRetry::HandleWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
    qint64 now = 0;

    switch (m_CurrentState)
    {
    case CHECK_PRESSURE:
        mp_SchedulerController->LogDebug("RS_PressureOverRange_3SRetry, in state CHECK_PRESSURE");
        if (qAbs(mp_SchedulerController->GetSchedCommandProcessor()->HardwareMonitor(m_Sender).PressureAL) >= 40.0)
        {
            m_CheckPressureTime = QDateTime::currentMSecsSinceEpoch();
            m_Counter++;
            m_CurrentState = WAIT_FOR_1S;
        }
        else
        {
           mp_StateMachine->OnTasksDone(true);
        }
        break;
    case WAIT_FOR_1S:
        mp_SchedulerController->LogDebug("RS_PressureOverRange_3SRetry, in state WAIT_FOR_1S");
        now = QDateTime::currentMSecsSinceEpoch();
        if ((now - m_CheckPressureTime) >= 1*1000)
        {
            if (m_Counter < 3)
            {
                //Retry
                m_CurrentState = CHECK_PRESSURE;
            }
            else
            {
                mp_SchedulerController->ReleasePressure();
                m_CurrentState = RELEASE_PRESSURE;
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
                mp_StateMachine->OnTasksDone(false);
            }
            else
            {
                mp_StateMachine->OnTasksDone(false);
            }
        }
        break;
    default:
        break;
    }
}

}
