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

CRsStandbyWithTissue::CRsStandbyWithTissue(SchedulerMainThreadController* SchedController, quint8 type)
    :mp_SchedulerController(SchedController), m_StandbyType(type)
{
    mp_StateMachine = QSharedPointer<QStateMachine>(new QStateMachine());

    mp_ShutdownFailedHeater = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_RTBottomStopTempCtrl = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_RTSideStopTempCtrl = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_CheckTempModuleCurrent = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_ReleasePressure = QSharedPointer<QState>(new QState(mp_StateMachine.data()));

    mp_StateMachine->setInitialState(mp_ShutdownFailedHeater.data());

    mp_ShutdownFailedHeater->addTransition(this, SIGNAL(StopRTBottomTempCtrl()), mp_RTBottomStopTempCtrl.data());
    mp_ShutdownFailedHeater->addTransition(this, SIGNAL(CheckTempModuleCurrernt()), mp_CheckTempModuleCurrent.data()); //For RS_Standby
    mp_RTBottomStopTempCtrl->addTransition(this, SIGNAL(StopRTSideTempCtrl()), mp_RTSideStopTempCtrl.data());
    mp_RTSideStopTempCtrl->addTransition(this, SIGNAL(CheckTempModuleCurrernt()), mp_CheckTempModuleCurrent.data());
    mp_CheckTempModuleCurrent->addTransition(this, SIGNAL(ReleasePressure()), mp_ReleasePressure.data());
    mp_ReleasePressure->addTransition(this,SIGNAL(TasksDone(bool)), mp_ShutdownFailedHeater.data());

    CONNECTSIGNALSLOT(mp_ShutdownFailedHeater.data(), exited(), this, OnShutdownHeaters());
    CONNECTSIGNALSLOT(mp_CheckTempModuleCurrent.data(), entered(), this, OnEnterCheckingTempModuleCurrent());
    CONNECTSIGNALSLOT(mp_ReleasePressure.data(), entered(), this, OnReleasePressure());

	//For error cases
    mp_RTBottomStopTempCtrl->addTransition(this, SIGNAL(TasksDone(bool)), mp_ShutdownFailedHeater.data());
    mp_RTSideStopTempCtrl->addTransition(this, SIGNAL(TasksDone(bool)), mp_ShutdownFailedHeater.data());
    mp_CheckTempModuleCurrent->addTransition(this, SIGNAL(TasksDone(bool)), mp_ShutdownFailedHeater.data());

    mp_StateMachine->start();
    m_ShutdownHeatersTime = 0;
    m_StartCheckingTime = 0;
    }

CRsStandbyWithTissue::~CRsStandbyWithTissue()
{
    mp_StateMachine->stop();
}

void CRsStandbyWithTissue::OnReleasePressure()
{
    mp_SchedulerController->LogDebug("In RS_Standby_WithTissue or RS_Standby, begin to run CmdALReleasePressure");
    mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, mp_SchedulerController));
}

CRsStandbyWithTissue::StateList_t CRsStandbyWithTissue::GetCurrentState(QSet<QAbstractState*> statesList)
{
    StateList_t currentState = UNDEF;

    if(statesList.contains(mp_ShutdownFailedHeater.data()))
    {
           currentState = SHUTDOWN_FAILED_HEATER;
    }
    else if (statesList.contains(mp_RTBottomStopTempCtrl.data()))
    {
        currentState = RTBOTTOM_STOP_TEMPCTRL;
    }
    else if (statesList.contains(mp_RTSideStopTempCtrl.data()))
    {
        currentState = RTSIDE_STOP_TEMPCTRL;
    }
    else if (statesList.contains(mp_CheckTempModuleCurrent.data()))
    {
        currentState = CHECK_TEMPMODULE_CURRENT;
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
    qint64 now = 0;
    ReportError_t reportError1;
    memset(&reportError1, 0, sizeof(reportError1));
    ReportError_t reportError2;
    memset(&reportError2, 0, sizeof(reportError2));

	switch (currentState)
    {
    case SHUTDOWN_FAILED_HEATER:
        mp_SchedulerController->LogDebug("RS_Standby_WithTissue or RS_Standby, in state SHUTDOWN_FAILD_HEATER");
        if (true == mp_SchedulerController->ShutdownFailedHeaters())
        {
            if (0 == m_StandbyType) // RS_Standby_WithTissue
            {
                m_StartCheckingTime = QDateTime::currentMSecsSinceEpoch();
                if (RETORT != mp_SchedulerController->GetFailerHeaterType())
                {
                    emit StopRTBottomTempCtrl();
                }
                else
                {
                    emit CheckTempModuleCurrernt();
                }
            }
            else //RS_Standby
            {
				m_StartCheckingTime = QDateTime::currentMSecsSinceEpoch();
                emit CheckTempModuleCurrernt();
            }
        }
        else
        {
            emit TasksDone(false);
        }
        break;
    case RTBOTTOM_STOP_TEMPCTRL:
        mp_SchedulerController->LogDebug("RS_Standby_WithTissue, in state RTBOTTOM_STOP_TEMPCTRL");
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
        mp_SchedulerController->LogDebug("RS_Standby_WithTissue, in state RTSIDE_STOP_TEMPCTRL");
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_SchedulerController->GetHeatingStrategy()->StopTemperatureControl("RTSide"))
        {
            emit CheckTempModuleCurrernt();
        }
        else
        {
            emit TasksDone(false);
        }
        break;
    case CHECK_TEMPMODULE_CURRENT:
        mp_SchedulerController->LogDebug("RS_Standby_WithTissue or RS_Standby, in state CHECK_TEMPMODULE_CURRENT");
        now = QDateTime::currentMSecsSinceEpoch();
        if ((now - m_StartCheckingTime) > 3*1000)
        {
            emit ReleasePressure();
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
                        emit TasksDone(false);
                    }
                    if (reportError2.instanceID != 0 && (now - reportError2.errorTime) <= 3*1000)
                    {
                        emit TasksDone(false);
                    }
                }
            }
            if (false == mp_SchedulerController->CheckSlaveTempModulesCurrentRange(3))
            {
                emit TasksDone(false);
            }
        }
        break;
    case RELEASE_PRESSURE:
        mp_SchedulerController->LogDebug("RS_Standby_WithTissue or RS_Standby, in state RELEASE_PRESSURE");
        if ("Scheduler::ALReleasePressure" == cmdName)
        {
            if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                emit TasksDone(true);
            }
            else
            {
                emit TasksDone(false);
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

