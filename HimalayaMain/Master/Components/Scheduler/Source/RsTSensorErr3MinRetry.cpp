/****************************************************************************/
/*! \file RsTSensorErr3MinRetry.cpp
 *
 *  \brief CRsTSensorErr3MinRetry class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ June 19th, 2014
 *   $Author:  $ Shufa Li
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

#include "Global/Include/Utils.h"
#include "Scheduler/Include/RsTSensorErr3MinRetry.h"
#include "Scheduler/Include/HeatingStrategy.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"

namespace Scheduler{
/*lint -e534 */

CRsTSensorErr3MinRetry::CRsTSensorErr3MinRetry(SchedulerMainThreadController* SchedController, CSchedulerStateMachine* StateMachine)
    :mp_SchedulerThreadController(SchedController)
    ,mp_SchedulerMachine(StateMachine)
{
    m_CurrentState = RS_TSENSORERR3MINRETRY_INIT;
    m_WaitBeginTime = 0;
}

CRsTSensorErr3MinRetry::~CRsTSensorErr3MinRetry()
{
}

void CRsTSensorErr3MinRetry::Start()
{
    m_CurrentState = RS_TSENSORERR3MINRETRY_INIT;
    m_WaitBeginTime = 0;
}

void CRsTSensorErr3MinRetry::HandleWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    qint64 nowTime = 0;
    /*lint -e616 */
    switch (m_CurrentState)
    {
    case RS_TSENSORERR3MINRETRY_INIT:
        mp_SchedulerThreadController->ReleasePressure();
        m_CurrentState = RS_TSENSORERR3MINRETRY_RELEASEPRESSURE;
        break;
    case RS_TSENSORERR3MINRETRY_RELEASEPRESSURE:
        mp_SchedulerThreadController->LogDebug("RS_TSensorErr_3MinSRetry, in state RELEASE_PRESSURE");
        if ("Scheduler::ALReleasePressure" == cmdName)
        {
            if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
            {
                m_WaitBeginTime = 0;
                mp_SchedulerMachine->OnTasksDone(false);
            }
            else
            {
                m_CurrentState = RS_TSENSORERR3MINRETRY_STOPTEMPCTRL;
            }
        }
    case RS_TSENSORERR3MINRETRY_STOPTEMPCTRL:
        mp_SchedulerThreadController->LogDebug("RS_TSensorErr_3MinSRetry, in state STOP_TEMPCTRL");
        if( true == mp_SchedulerThreadController->ShutdownFailedHeaters() )
        {
            m_WaitBeginTime = QDateTime::currentMSecsSinceEpoch();
            m_CurrentState = RS_TSENSORERR3MINRETRY_WAIT3MIN;
        }
        else
        {
            m_WaitBeginTime = 0;
            mp_SchedulerMachine->OnTasksDone(false);
        }
        break;
    case RS_TSENSORERR3MINRETRY_WAIT3MIN:
        mp_SchedulerThreadController->LogDebug("RS_TSensorErr_3MinSRetry, in state WAIT3MIN");
        nowTime = QDateTime::currentMSecsSinceEpoch();
        if(nowTime - m_WaitBeginTime > 180 * 1000)
        {
            m_CurrentState = RS_TSENSORERR3MINRETRY_STARTTEMPCTRL;
        }
        break;
    case RS_TSENSORERR3MINRETRY_STARTTEMPCTRL:
        mp_SchedulerThreadController->LogDebug("RS_TSensorErr_3MinSRetry, in state START_TEMPCTRL");
        if( true == mp_SchedulerThreadController->RestartFailedHeaters() )
        {
            m_WaitBeginTime = QDateTime::currentMSecsSinceEpoch();
            m_CurrentState = RS_TSENSORERR3MINRETRY_CHECKSTATUS;
        }
        else
        {
            m_WaitBeginTime = 0;
            mp_SchedulerThreadController->ShutdownFailedHeaters();
            mp_SchedulerMachine->OnTasksDone(false);
        }
        break;
    case RS_TSENSORERR3MINRETRY_CHECKSTATUS:
        mp_SchedulerThreadController->LogDebug("RS_TSensorErr_3MinSRetry, in state CHECK_STATUS");
        nowTime = QDateTime::currentMSecsSinceEpoch();
        if(nowTime - m_WaitBeginTime > 3 * 1000)
        {
            m_WaitBeginTime = 0;
            mp_SchedulerThreadController->ShutdownFailedHeaters();
            mp_SchedulerMachine->OnTasksDone(true);
        }
        else
        {
            if (false == CheckTSensorCurrentStatus())
            {
                m_WaitBeginTime = 0;
                mp_SchedulerThreadController->ShutdownFailedHeaters();
                mp_SchedulerMachine->OnTasksDone(false);
            }
        }
        break;
    default:
        break;
    }
}

bool CRsTSensorErr3MinRetry::CheckTSensorCurrentStatus()
{
    bool ret = true;
    qreal temp1 = mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().TempOvenTop;
    if (false == mp_SchedulerThreadController->GetHeatingStrategy()->CheckSensorTempOverRange("OvenTop", temp1) )
    {
        ret = false;
    }

    qreal temp2 = mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().TempOvenBottom1;
    if ( false == mp_SchedulerThreadController->GetHeatingStrategy()->CheckSensorTempOverRange("OvenBottom", temp2) )
    {
        ret = false;
    }

    if ( false == mp_SchedulerThreadController->CheckSlaveTempModulesCurrentRange(3) )
    {
        ret = false;
    }

    // New requirement - check lower limt of the three Oven sensors (40 degrees)
    qreal temp3 = mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().TempOvenBottom2;
    if (temp1<40.0 || temp2<40.0 || temp3<40.0)
    {
        return false;
    }
    return ret;
}

}
