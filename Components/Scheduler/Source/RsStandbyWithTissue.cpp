/****************************************************************************/
/*! \file RsStandbyWithTissue.cpp
 *
 *  \brief CRsStandbyWithTissue class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ April 22nd, 2014
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

#include "Global/Include/Utils.h"
#include "Scheduler/Include/RsStandbyWithTissue.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "Scheduler/Include/HeatingStrategy.h"
#include "Scheduler/Commands/Include/CmdALReleasePressure.h"

using namespace DeviceControl;

namespace Scheduler{

CRsStandbyWithTissue::CRsStandbyWithTissue(SchedulerMainThreadController* SchedController, CSchedulerStateMachine* StateMachine, quint8 type)
    :mp_SchedulerController(SchedController), mp_StateMachine(StateMachine), m_StandbyType(type)
{
    m_CurrentState = SHUTDOWN_FAILED_HEATER;
    m_StartCheckingTime = 0;
}

void CRsStandbyWithTissue::Start()
{
    m_CurrentState = SHUTDOWN_FAILED_HEATER;
    m_StartCheckingTime = 0;
}

CRsStandbyWithTissue::~CRsStandbyWithTissue()
{
}

void CRsStandbyWithTissue::OnReleasePressure()
{
    mp_SchedulerController->LogDebug("In RS_Standby_WithTissue or RS_Standby, begin to run CmdALReleasePressure");
    mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, m_Sender));
}

void CRsStandbyWithTissue::HandleWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
    qint64 now = 0;
    ReportError_t reportError1;
    memset(&reportError1, 0, sizeof(reportError1));
    ReportError_t reportError2;
    memset(&reportError2, 0, sizeof(reportError2));

    /*lint -e525 */
    switch (m_CurrentState)
    {
    case SHUTDOWN_FAILED_HEATER:
        mp_SchedulerController->LogDebug("RS_Standby_WithTissue or RS_Standby, in state SHUTDOWN_FAILD_HEATER");

        //For open retort LID, we need stop level sensor
        if (DCL_ERR_DEV_LIDLOCK_CLOSE_STATUS_ERROR == mp_SchedulerController->GetCurErrEventID())
        {
             mp_SchedulerController->GetHeatingStrategy()->StopTemperatureControl("LevelSensor");
        }
        if (true == mp_SchedulerController->ShutdownFailedHeaters())
        {
            if (0 == m_StandbyType) // RS_Standby_WithTissue
            {
                m_StartCheckingTime = QDateTime::currentMSecsSinceEpoch();
                if (RETORT != mp_SchedulerController->GetFailerHeaterType())
                {
                    m_CurrentState = RTBOTTOM_STOP_TEMPCTRL;
                }
                else
                {
                    m_StartCheckingTime = QDateTime::currentMSecsSinceEpoch();
                    m_CurrentState = CHECK_TEMPMODULE_CURRENT;
                }
            }
            else //RS_Standby
            {
				m_StartCheckingTime = QDateTime::currentMSecsSinceEpoch();
                m_CurrentState = CHECK_TEMPMODULE_CURRENT;
            }
        }
        else
        {
            mp_StateMachine->OnTasksDoneRsStandyWithTissue(false);
        }
        break;
    case RTBOTTOM_STOP_TEMPCTRL:
        mp_SchedulerController->LogDebug("RS_Standby_WithTissue, in state RTBOTTOM_STOP_TEMPCTRL");
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_SchedulerController->GetHeatingStrategy()->StopTemperatureControl("RTBottom"))
        {
            m_CurrentState = RTSIDE_STOP_TEMPCTRL;
        }
        else
        {
            mp_StateMachine->OnTasksDoneRsStandyWithTissue(false);
        }
        break;
    case RTSIDE_STOP_TEMPCTRL:
        mp_SchedulerController->LogDebug("RS_Standby_WithTissue, in state RTSIDE_STOP_TEMPCTRL");
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_SchedulerController->GetHeatingStrategy()->StopTemperatureControl("RTSide"))
        {
            m_CurrentState = CHECK_TEMPMODULE_CURRENT;
            m_StartCheckingTime = QDateTime::currentMSecsSinceEpoch();
        }
        else
        {
            mp_StateMachine->OnTasksDoneRsStandyWithTissue(false);
        }
        break;
    case CHECK_TEMPMODULE_CURRENT:
        mp_SchedulerController->LogDebug("RS_Standby_WithTissue or RS_Standby, in state CHECK_TEMPMODULE_CURRENT");
        now = QDateTime::currentMSecsSinceEpoch();
        if ((now - m_StartCheckingTime) > 3*1000)
        {
            OnReleasePressure();
            m_CurrentState = RELEASE_PRESSURE;
        }
        else
        {
            if (0 == m_StandbyType) // RS_Standby_WithTissue
            {
                if (RETORT != mp_SchedulerController->GetFailerHeaterType())
                {
                    reportError1 = mp_SchedulerController->GetSchedCommandProcessor()->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE,"Retort", RT_BOTTOM);
                    reportError2 = mp_SchedulerController->GetSchedCommandProcessor()->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE,"Retort", RT_SIDE);
                    if (reportError1.instanceID != 0 && (now - reportError1.errorTime) <= 3*1000)
                    {
                        mp_StateMachine->OnTasksDoneRsStandyWithTissue(false);
                    }
                    if (reportError2.instanceID != 0 && (now - reportError2.errorTime) <= 3*1000)
                    {
                        mp_StateMachine->OnTasksDoneRsStandyWithTissue(false);
                    }
                }
            }
            if (false == mp_SchedulerController->CheckSlaveTempModulesCurrentRange(3))
            {
                mp_StateMachine->OnTasksDoneRsStandyWithTissue(false);
            }
        }
        break;
    case RELEASE_PRESSURE:
        mp_SchedulerController->LogDebug("RS_Standby_WithTissue or RS_Standby, in state RELEASE_PRESSURE");
        if ("Scheduler::ALReleasePressure" == cmdName)
        {
            if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                mp_StateMachine->OnTasksDoneRsStandyWithTissue(true);
            }
            else
            {
                mp_StateMachine->OnTasksDoneRsStandyWithTissue(false);
            }
        }
        else
        {
            // Do nothing, just wait for the command response
        }
        break;
    default:
        break;
	}
}

}

