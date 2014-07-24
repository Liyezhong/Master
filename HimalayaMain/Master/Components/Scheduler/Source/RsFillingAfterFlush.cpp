/****************************************************************************/
/*! \file RsFillingAfterFlush.h
 *
 *  \brief CRsFillingAfterFlush class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ July 22th, 2014
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

#include "Scheduler/Include/RsFillingAfterFlush.h"
#include "Global/Include/Utils.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/HeatingStrategy.h"

namespace Scheduler{

CRsFillingAfterFlush::CRsFillingAfterFlush(SchedulerMainThreadController* SchedController)
    :mp_SchedulerThreadController(SchedController)
{
    mp_StateMachine = QSharedPointer<QStateMachine>(new QStateMachine());

    mp_Initial = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_BuildPressure = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_Wait30s = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_HeatLevelSensor = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_CheckLevelSensorTemp = QSharedPointer<QState>(new QState(mp_StateMachine.data()));
    mp_Filling = QSharedPointer<QState>(new QState(mp_StateMachine.data()));

    mp_StateMachine->setInitialState(mp_Initial.data());

    mp_Initial->addTransition(this, SIGNAL(SigBuildPressure()), mp_BuildPressure.data());
    CONNECTSIGNALSLOT(mp_BuildPressure.data(), entered(), mp_SchedulerThreadController, HighPressure());
    mp_BuildPressure->addTransition(this, SIGNAL(SigWait30s()), mp_Wait30s.data());
    mp_Wait30s->addTransition(this, SIGNAL(SigHeatLevelSensor()), mp_HeatLevelSensor.data());
    mp_HeatLevelSensor->addTransition(this, SIGNAL(SigCheckLevelSensorTemp()), mp_CheckLevelSensorTemp.data());
    mp_CheckLevelSensorTemp->addTransition(this, SIGNAL(SigFilling()), mp_Filling.data());
    CONNECTSIGNALSLOT(mp_Filling.data(), entered(), mp_SchedulerThreadController, Fill());
    CONNECTSIGNALSLOT(mp_Filling.data(), exited(), mp_SchedulerThreadController, OnStopFill());
    mp_Filling->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());

    //for error case
    mp_BuildPressure->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_Wait30s->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_HeatLevelSensor->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());
    mp_CheckLevelSensorTemp->addTransition(this, SIGNAL(TasksDone(bool)), mp_Initial.data());

    mp_StateMachine->start();
}

CRsFillingAfterFlush::~CRsFillingAfterFlush()
{
    mp_StateMachine->stop();
}

CRsFillingAfterFlush::StateList_t CRsFillingAfterFlush::GetCurrentState(QSet<QAbstractState*> statesList)
{
    StateList_t currentState = RSFILLINGAFTERFLUSH_UNDEF;

     if (statesList.contains(mp_Initial.data()))
     {
         currentState = RSFILLINGAFTERFLUSH_INIT;
     }
     else if (statesList.contains(mp_BuildPressure.data()))
     {
         currentState = RSFILLINGAFTERFLUSH_PRESSURE;
     }
     else if (statesList.contains(mp_Wait30s.data()))
     {
         currentState = RSFILLINGAFTERFLUSH_WAIT;
     }
     else if (statesList.contains(mp_HeatLevelSensor.data()))
     {
         currentState = RSFILLINGAFTERFLUSH_HEATINGLEVELSENSOR;
     }
     else if (statesList.contains(mp_CheckLevelSensorTemp.data()))
     {
         currentState = RSFILLINGAFTERFLUSH_CHECKLEVELSENSORTEMP;
     }
     else if (statesList.contains(mp_Filling.data()))
     {
         currentState = RSFILLINGAFTERFLUSH_FILLING;
     }

     return currentState;
}

void CRsFillingAfterFlush::HandleWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    StateList_t currentState = GetCurrentState(mp_StateMachine->configuration());
    qint64 nowTime = 0;
    quint16 retValue = 0;
    qreal tempLevelSensor = 0.0;

    switch(currentState)
    {
    case RSFILLINGAFTERFLUSH_INIT:
        emit SigBuildPressure();
        break;
    case RSFILLINGAFTERFLUSH_PRESSURE:
        if("Scheduler::ALPressure" == cmdName)
        {
            if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
            {
                emit TasksDone(false);
            }
            else
            {
                m_StartTime = QDateTime::currentMSecsSinceEpoch();
                emit SigWait30s();
            }
        }
        break;
    case RSFILLINGAFTERFLUSH_WAIT:
        nowTime = QDateTime::currentMSecsSinceEpoch();
        if(nowTime - m_StartTime >= 30 * 1000)
        {
            emit SigHeatLevelSensor();
        }
        break;
    case RSFILLINGAFTERFLUSH_HEATINGLEVELSENSOR:
        if(DCL_ERR_FCT_CALL_SUCCESS != mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControl("LevelSensor"))
        {
            emit TasksDone(false);
        }
        else
        {
            emit SigCheckLevelSensorTemp();
        }
        break;
    case RSFILLINGAFTERFLUSH_CHECKLEVELSENSORTEMP:
        tempLevelSensor = mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().TempALLevelSensor;
        retValue = mp_SchedulerThreadController->GetHeatingStrategy()->CheckTemperatureOverTime("LevelSensor",tempLevelSensor);
        if (0 == retValue)
        {
            // Do nothing
        }
        else if (1 == retValue)
        {
            emit TasksDone(false);
        }
        else if (2 == retValue)
        {
            emit SigFilling();
        }
        break;
    case RSFILLINGAFTERFLUSH_FILLING:
        if( "Scheduler::ALFilling" == cmdName)
        {
            if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
            {
                emit TasksDone(false);
            }
            else
            {
                emit TasksDone(true);
            }
        }
        break;
    default:
        mp_SchedulerThreadController->LogDebug("RsFillingAfterFlush: Get the default branch");
        break;
    }
}



}

