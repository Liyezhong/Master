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

CRsTSensorErr3MinRetry::CRsTSensorErr3MinRetry(SchedulerMainThreadController* SchedController)
    :mp_SchedulerThreadController(SchedController)
{
    mp_SchedulerMachine = QSharedPointer<QStateMachine>(new QStateMachine());
    mp_Initial = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    mp_ReleasePressure  = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    mp_StopTempCtrl = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    mp_Wait3Min = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    mp_StartTempCtrl = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    mp_CheckTemp = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));

    mp_SchedulerMachine->setInitialState(mp_Initial.data());
    mp_Initial->addTransition(this, SIGNAL(ReleasePressure()), mp_ReleasePressure.data());
    mp_ReleasePressure->addTransition(this, SIGNAL(StopTempCtrlTSensor()), mp_StopTempCtrl.data());
    mp_StopTempCtrl->addTransition(this, SIGNAL(Wait3MinTSensor()), mp_Wait3Min.data());
    mp_Wait3Min->addTransition(this, SIGNAL(StartTempCtrlTSensor()), mp_StartTempCtrl.data());
    mp_StartTempCtrl->addTransition(this, SIGNAL(CheckTSensorStatus()), mp_CheckTemp.data());
    mp_CheckTemp->addTransition(this,SIGNAL(TasksDone(bool)), mp_Initial.data());

    CONNECTSIGNALSLOT(mp_ReleasePressure.data(), entered(), mp_SchedulerThreadController, ReleasePressure());
    //For error cases
    mp_ReleasePressure->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_StopTempCtrl->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_Wait3Min->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_StartTempCtrl->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_CheckTemp->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());

    mp_WaitBeginTime = 0;
    mp_SchedulerMachine->start();
}

CRsTSensorErr3MinRetry::~CRsTSensorErr3MinRetry()
{
    mp_SchedulerMachine->stop();
}

CRsTSensorErr3MinRetry::StateList_t CRsTSensorErr3MinRetry::GetCurrentState(QSet<QAbstractState*> statesList)
{
    StateList_t currentState = RS_TSENSORERR3MINRETRY_UNDEF;

    if ( statesList.contains(mp_Initial.data()) )
    {
        currentState = RS_TSENSORERR3MINRETRY_INIT;
    }
    else if ( statesList.contains(mp_ReleasePressure.data()))
    {
        currentState = RS_TSENSORERR3MINRETRY_RELEASEPRESSURE;
    }
    else if ( statesList.contains(mp_StopTempCtrl.data()) )
    {
        currentState = RS_TSENSORERR3MINRETRY_STOPTEMPCTRL;
    }
    else if( statesList.contains(mp_Wait3Min.data()) )
    {
        currentState = RS_TSENSORERR3MINRETRY_WAIT3MIN;
    }
    else if ( statesList.contains(mp_StartTempCtrl.data()) )
    {
        currentState = RS_TSENSORERR3MINRETRY_STARTTEMPCTRL;
    }
    else if( statesList.contains(mp_CheckTemp.data()) )
    {
        currentState = RS_TSENSORERR3MINRETRY_CHECKSTATUS;
    }
    return currentState;
}

void CRsTSensorErr3MinRetry::HandleWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    StateList_t currentState = this->GetCurrentState(mp_SchedulerMachine->configuration());
    qint64 nowTime = 0;
    switch (currentState)
    {
    case RS_TSENSORERR3MINRETRY_INIT:
        emit ReleasePressure();
        break;
    case RS_TSENSORERR3MINRETRY_RELEASEPRESSURE:
        mp_SchedulerThreadController->LogDebug("RS_TSensorErr_3MinSRetry, in state RELEASE_PRESSURE");
        if ("Scheduler::ALReleasePressure" == cmdName)
        {
            if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
            {
                mp_WaitBeginTime = 0;
                emit TasksDone(false);
            }
            else
            {
                emit StopTempCtrlTSensor();
            }
        }
    case RS_TSENSORERR3MINRETRY_STOPTEMPCTRL:
        mp_SchedulerThreadController->LogDebug("RS_TSensorErr_3MinSRetry, in state STOP_TEMPCTRL");
        if( true == mp_SchedulerThreadController->ShutdownFailedHeaters() )
        {
            mp_WaitBeginTime = QDateTime::currentMSecsSinceEpoch();
            emit Wait3MinTSensor();
        }
        else
        {
            mp_WaitBeginTime = 0;
            emit TasksDone(false);
        }
        break;
    case RS_TSENSORERR3MINRETRY_WAIT3MIN:
        mp_SchedulerThreadController->LogDebug("RS_TSensorErr_3MinSRetry, in state WAIT3MIN");
        nowTime = QDateTime::currentMSecsSinceEpoch();
        if(nowTime - mp_WaitBeginTime > 180 * 1000)
        {
            emit StartTempCtrlTSensor();
        }
        break;
    case RS_TSENSORERR3MINRETRY_STARTTEMPCTRL:
        mp_SchedulerThreadController->LogDebug("RS_TSensorErr_3MinSRetry, in state START_TEMPCTRL");
        if( true == mp_SchedulerThreadController->RestartFailedHeaters() )
        {
            mp_WaitBeginTime = QDateTime::currentMSecsSinceEpoch();
            emit CheckTSensorStatus();
        }
        else
        {
            mp_WaitBeginTime = 0;
            mp_SchedulerThreadController->ShutdownFailedHeaters();
            emit TasksDone(false);
        }
        break;
    case RS_TSENSORERR3MINRETRY_CHECKSTATUS:
        mp_SchedulerThreadController->LogDebug("RS_TSensorErr_3MinSRetry, in state CHECK_STATUS");
        nowTime = QDateTime::currentMSecsSinceEpoch();
        if(nowTime - mp_WaitBeginTime > 3 * 1000)
        {
            mp_WaitBeginTime = 0;
            mp_SchedulerThreadController->ShutdownFailedHeaters();
            emit TasksDone(true);
        }
        else
        {
            if (false == CheckTSensorCurrentStatus())
            {
                mp_WaitBeginTime = 0;
                mp_SchedulerThreadController->ShutdownFailedHeaters();
                emit TasksDone(false);
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
    return ret;
}

}
