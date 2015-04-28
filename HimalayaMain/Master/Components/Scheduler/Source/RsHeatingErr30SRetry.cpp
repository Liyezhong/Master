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
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "Scheduler/Include/HeatingStrategy.h"

namespace Scheduler{
/*lint -e534 */

CRsHeatingErr30SRetry::CRsHeatingErr30SRetry(SchedulerMainThreadController* SchedController, CSchedulerStateMachine* StateMachine)
    :mp_SchedulerController(SchedController)
    ,mp_StateMachine(StateMachine)
{
    m_CurrentState = HEATINGERROR_30SRetry_INIT;
    m_ShutdownHeaterTime = 0;
    m_StartTime = 0;
    m_Counter = 0;
}

void CRsHeatingErr30SRetry::Start()
{
    m_CurrentState = HEATINGERROR_30SRetry_INIT;
    m_ShutdownHeaterTime = 0;
    m_StartTime = 0;
    m_Counter = 0;
}

/****************************************************************************/
/*!
 *  \brief    Destructor
 */
/****************************************************************************/
CRsHeatingErr30SRetry::~CRsHeatingErr30SRetry()
{
}

void CRsHeatingErr30SRetry::HandleWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
    qint64 now = 0;
     HeaterType_t heaterType = mp_SchedulerController->GetFailerHeaterType();
    switch (m_CurrentState)
    {
    case HEATINGERROR_30SRetry_INIT:
        mp_SchedulerController->LogDebug("RS_HeatingErr_30SRetry, in state INITIALIZE");
        mp_SchedulerController->ReleasePressure();
        m_CurrentState = RELEASE_PRESSURE;
        break;
    case RELEASE_PRESSURE:
        mp_SchedulerController->LogDebug("RS_HeatingErr_30SRetry, in state RELEASE_PRESSURE");
        if ("Scheduler::ALReleasePressure" == cmdName)
        {
            if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
            {
                m_ShutdownHeaterTime = 0;
                m_StartTime = 0;
                m_Counter = 0;
                mp_StateMachine->OnTasksDone(false);
            }
            else
            {
                m_CurrentState = SHUTDOWN_FAILD_HEATER;
            }
        }
        break;
    case SHUTDOWN_FAILD_HEATER:
        mp_SchedulerController->LogDebug("RS_HeatingErr_30SRetry, in state SHUTDOWN_FAILD_HEATER");
        if (true == mp_SchedulerController->ShutdownFailedHeaters())
        {
            m_ShutdownHeaterTime = QDateTime::currentMSecsSinceEpoch();
            m_CurrentState = WAIT_FOR_10SECONDS;
        }
        else
        {
            mp_SchedulerController->LogDebug("RS_HeatingErr_30SRetry, SHUTDOWN_FAILD_HEATER failed");
            m_ShutdownHeaterTime = 0;
            m_StartTime = 0;
            m_Counter = 0;
            mp_StateMachine->OnTasksDone(false);
        }
        break;
    case WAIT_FOR_10SECONDS:
        mp_SchedulerController->LogDebug("RS_HeatingErr_30SRetry, in state WAIT_FOR_10SECONDS");
        now = QDateTime::currentMSecsSinceEpoch();
        if ((now - m_ShutdownHeaterTime) >= 10*1000)
        {
            m_CurrentState = RESTART_FAILED_HEATER;
        }
        break;
    case RESTART_FAILED_HEATER:
        mp_SchedulerController->LogDebug("RS_HeatingErr_30SRetry, in state RESTART_FAILED_HEATER");
        if (true == mp_SchedulerController->RestartFailedHeaters())
        {
            m_StartTime = QDateTime::currentMSecsSinceEpoch();
            m_CurrentState = CHECK_MODULE_CURRENT;
        }
        else
        {
            m_ShutdownHeaterTime = 0;
            m_StartTime = 0;
            m_Counter = 0;
            (void)mp_SchedulerController->ShutdownFailedHeaters();
            mp_StateMachine->OnTasksDone(false);
        }
        break;
    case CHECK_MODULE_CURRENT:
        mp_SchedulerController->LogDebug("RS_HeatingErr_30SRetry, in state CHECK_MODULE_CURRENT");
        now = QDateTime::currentMSecsSinceEpoch();
        if ((now - m_StartTime) > 3*1000)
        {
            m_ShutdownHeaterTime = 0;
            m_StartTime = 0;
            m_Counter = 0;
            //For FAN, we need NOT trun off it.
            if (heaterType != FAN)
            {
                (void)mp_SchedulerController->ShutdownFailedHeaters();
            }
            mp_StateMachine->OnTasksDone(true);
        }
        else
        {
            bool ret = true;
            if (heaterType == FAN)
            {
                // Do nothing, we need NOT check temperature in this case
            }
            else if (heaterType == LATUBE1ABNORMAL)
            {
                ret = mp_SchedulerController->GetHeatingStrategy()->CheckLATbueTempAbnormal(mp_SchedulerController->GetSchedCommandProcessor()->HardwareMonitor().TempALTube1);
            }
            else if (heaterType == LATUBE2ABNORMAL)
            {
                ret = mp_SchedulerController->GetHeatingStrategy()->CheckLATbueTempAbnormal(mp_SchedulerController->GetSchedCommandProcessor()->HardwareMonitor().TempALTube2);
            }
            else
            {
                ret = mp_SchedulerController->CheckSensorTempOverange();
            }
            if (false ==  mp_SchedulerController->CheckSlaveTempModulesCurrentRange(3)
                    || false == ret)
            {
                m_Counter++;
                if (3 == m_Counter)
                {
                    m_ShutdownHeaterTime = 0;
                    m_StartTime = 0;
                    m_Counter = 0;
                    (void)mp_SchedulerController->ShutdownFailedHeaters();
                    mp_StateMachine->OnTasksDone(false);
                }
                else
                {
                    m_ShutdownHeaterTime = 0;
                    m_StartTime = 0;
                    //Retry
                    m_CurrentState = SHUTDOWN_FAILD_HEATER;
                }
            }
        }
        break;
    default:
        break;
    }
}



}
