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

#include "Global/Include/Utils.h"
#include "Scheduler/Include/RsStandbyWithTissue.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "Scheduler/Include/HeatingStrategy.h"
#include "Scheduler/Commands/Include/CmdALReleasePressure.h"

using namespace DeviceControl;

namespace Scheduler{

CRsStandbyWithTissue::CRsStandbyWithTissue(SchedulerMainThreadController* SchedController): mp_SchedulerController(SchedController)
{
    mp_StateMachine = QSharedPointer<QStateMachine>(new QStateMachine());

    mp_ShutdownFailedHeater = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_RTBottomStopTempCtrl = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_RTSideStopTempCtrl = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_CheckTempModuleCurrent = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_ReleasePressure = QSharedPointer<QState>(new QState(mp_StateMachine.data()));

    mp_StateMachine->setInitialState(mp_ShutdownFailedHeater.data());
    mp_ShutdownFailedHeater->addTransition(this, SIGNAL(StopRTBottomTempCtrl()), mp_RTBottomStopTempCtrl.data());
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
    mp_SchedulerController->LogDebug("In RS_Standby_WithTissue, begin to run CmdALReleasePressure");
    mp_SchedulerController->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, mp_SchedulerController));
}

CRsStandbyWithTissue::StateList_t CRsStandbyWithTissue::GetCurrentState(QSet<QAbstractState*> statesList)
{
    StateList_t currentState = UNDEF;

    if(statesList.contains(mp_ShutdownFailedHeater.data()))
    {
           currentState = SHUTDOWN_FAILD_HEATER;
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

CRsStandbyWithTissue::HeaterType_t CRsStandbyWithTissue::GetFailerHeaterType()
{
    ReturnCode_t eventID = mp_SchedulerController->GetCurErrEventID();
    if (DCL_ERR_DEV_RETORT_LEVELSENSOR_HEATING_OVERTIME == eventID
            || DCL_ERR_DEV_LEVELSENSOR_TEMPERATURE_OVERRANGE == eventID)
    {
        return LEVELSENSOR;
    }
    else if (DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_ABNORMAL == eventID
             || DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR1_OUTOFRANGE == eventID
             || DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_NOTREACHTARGETTEMP == eventID)
    {
        return LATUBE1;
    }
    else if (DCL_ERR_DEV_LA_TUBEHEATING_TUBE2_ABNORMAL == eventID
             || DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR2_OUTOFRANGE == eventID
             || DCL_ERR_DEV_LA_TUBEHEATING_TUBE2_NOTREACHTARGETTEMP == eventID)
    {
        return LATUBE1;
    }
    else if ("50001" == QString::number(eventID).left(5))
    {
        return RETORT;
    }
    else if ("50002" == QString::number(eventID).left(5))
    {
        return OVEN;
    }
    else if ("50003" == QString::number(eventID).left(5))
    {
        return RV;
    }

    return UNKNOWN;
}

bool CRsStandbyWithTissue::ShutdownHeaters()
{
    HeaterType_t heaterType = this->GetFailerHeaterType();
    switch (heaterType)
    {
    case LEVELSENSOR:
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_SchedulerController->GetHeatingStrategy()->StopTemperatureControl("LevelSensor"))
        {
            return true;
        }
        break;
    case LATUBE1:
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_SchedulerController->GetHeatingStrategy()->StopTemperatureControl("LA_Tube1"))
        {
            return true;
        }
        break;
    case LATUBE2:
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_SchedulerController->GetHeatingStrategy()->StopTemperatureControl("LA_Tube2"))
        {
            return true;
        }
        break;
    case RETORT:
        return true; //In this case, we just skip to next state
    case OVEN:
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_SchedulerController->GetHeatingStrategy()->StopTemperatureControl("OvenTop") &&
                DCL_ERR_FCT_CALL_SUCCESS == mp_SchedulerController->GetHeatingStrategy()->StopTemperatureControl("OvenBottom"))
        {
            return true;
        }
        break;
    case RV:
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_SchedulerController->GetHeatingStrategy()->StopTemperatureControl("OvenTop"))
        {
            return true;
        }
        break;
    default:
        break;
    }

    return false;
}

bool CRsStandbyWithTissue::CheckTempModulesCurrent()
{

    ReportError_t reportError1;
    memset(&reportError1, 0, sizeof(reportError1));
    ReportError_t reportError2;
    memset(&reportError2, 0, sizeof(reportError2));

    HeaterType_t heaterType = this->GetFailerHeaterType();
    switch (heaterType)
    {
    case LEVELSENSOR:
        reportError1 = mp_SchedulerController->GetSchedCommandProcessor()->GetSlaveModuleReportError("LA", AL_LEVELSENSOR);
        break;
    case LATUBE1:
         reportError1 = mp_SchedulerController->GetSchedCommandProcessor()->GetSlaveModuleReportError("LA", AL_TUBE1);
         break;
    case LATUBE2:
        reportError1 = mp_SchedulerController->GetSchedCommandProcessor()->GetSlaveModuleReportError("LA", AL_TUBE1);
        break;
    case RETORT:
        break;
    case OVEN:
        reportError1 = mp_SchedulerController->GetSchedCommandProcessor()->GetSlaveModuleReportError("Oven", OVEN_TOP);
        reportError2 = mp_SchedulerController->GetSchedCommandProcessor()->GetSlaveModuleReportError("Oven", OVEN_BOTTOM);
        break;
    case RV:
        reportError1 =  mp_SchedulerController->GetSchedCommandProcessor()->GetSlaveModuleReportError("RV");
        break;
    default:
        break;
    }
    if (reportError1.instanceID != 0 && reportError1.errorTime.toMSecsSinceEpoch() >= m_ShutdownHeatersTime)
    {
        return false;
    }

    if (reportError2.instanceID != 0 && reportError2.errorTime.toMSecsSinceEpoch() >= m_ShutdownHeatersTime)
    {
        return false;
    }

    // For Retort Bottom and Side
    memset(&reportError1, 0, sizeof(reportError1));
    memset(&reportError2, 0, sizeof(reportError2));
    reportError1 = mp_SchedulerController->GetSchedCommandProcessor()->GetSlaveModuleReportError("Retort", RT_BOTTOM);
    reportError2 = mp_SchedulerController->GetSchedCommandProcessor()->GetSlaveModuleReportError("Retort", RT_SIDE);
    if (reportError1.instanceID != 0 && reportError1.errorTime.toMSecsSinceEpoch() >= m_ShutdownHeatersTime)
    {
        return false;
    }

    if (reportError2.instanceID != 0 && reportError2.errorTime.toMSecsSinceEpoch() >= m_ShutdownHeatersTime)
    {
        return false;
    }
    return true;
}

void CRsStandbyWithTissue::HandleWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{

    StateList_t currentState = this->GetCurrentState(mp_StateMachine->configuration());
    qint64 now = 0;
	switch (currentState)
	{
    case SHUTDOWN_FAILD_HEATER:
        mp_SchedulerController->LogDebug("RS_Standby_WithTissue, in state SHUTDOWN_FAILD_HEATER");
        if (true == this->ShutdownHeaters())
        {
            emit StopRTBottomTempCtrl();
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
        mp_SchedulerController->LogDebug("RS_Standby_WithTissue, in state CHECK_TEMPMODULE_CURRENT");
        now = QDateTime::currentMSecsSinceEpoch();
        if ((now - m_StartCheckingTime) > 3*1000)
        {
            emit ReleasePressure();
        }
        else
        {
            if (false == CheckTempModulesCurrent())
            {
                emit TasksDone(false);
            }
            else
            {
                // Do nothing, just wait for next tick
            }
        }
        break;
    case RELEASE_PRESSURE:
        mp_SchedulerController->LogDebug("RS_Standby_WithTissue, in state RELEASE_PRESSURE");
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

