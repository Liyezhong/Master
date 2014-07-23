/****************************************************************************/
/*! \file SchedulerMachine.cpp
 *
 *  \brief CSchedulerStateMachine class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 02.01.2014
 *   $Author:  $ Quan Zhang
 *
 *  \b Company:
 *
 *       Leica Biosystems SH CN.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#include "Global/Include/Commands/Command.h"
#include "Global/Include/Utils.h"
#include "Scheduler/Include/SchedulerMachine.h"
#include "Scheduler/Include/HimalayaDeviceEventCodes.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "Scheduler/Include/HeatingStrategy.h"
#include "Scheduler/Commands/Include/CmdRTSetTempCtrlOFF.h"
#include "Scheduler/Commands/Include/CmdALAllStop.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToInitialPosition.h"
#include "Scheduler/Include/RsStandby.h"
#include "Scheduler/Include/RsStandbyWithTissue.h"
#include "Scheduler/Include/RsHeatingErr30SRetry.h"
#include "Scheduler/Include/RsTSensorErr3MinRetry.h"
#include "Scheduler/Include/RcLevelsensorHeatingOvertime.h"
#include "Scheduler/Include/RcRestart.h"
#include "Scheduler/Include/RcReport.h"
#include "Scheduler/Include/ProgramSelfTest.h"
#include "Scheduler/Include/RsFillingAfterFlush.h"
#include <QDebug>
#include <QDateTime>

namespace Scheduler
{
/****************************************************************************/
/*!
 *  \brief    Constructor
 */
/****************************************************************************/
CSchedulerStateMachine::CSchedulerStateMachine(SchedulerMainThreadController* SchedulerThreadController)
    :mp_SchedulerThreadController(SchedulerThreadController)
{
    qRegisterMetaType<DeviceControl::RTTempCtrlType_t>("DeviceControl::RTTempCtrlType_t");
    m_PreviousState = SM_UNDEF;
    m_CurrentState = SM_UNDEF;

    mp_SchedulerMachine = QSharedPointer<QStateMachine>(new QStateMachine());

    // Layer one states
    mp_InitState = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    mp_IdleState = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    mp_BusyState = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    mp_ErrorState = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));

    // Layer two states (for Busy state)
    mp_PssmInitState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmPreTestState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmFillingHeatingRVState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmFillingLevelSensorHeatingState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmFillingState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmRVMoveToSealState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmProcessingState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmRVMoveToTubeState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmDrainingState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmRVPosChangeState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmStepFinish = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmProgramFinish = QSharedPointer<QFinalState>(new QFinalState(mp_BusyState.data()));
    mp_PssmError = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmPause = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmPauseDrain = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmAborting = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmAborted = QSharedPointer<QState>(new QState(mp_BusyState.data()));

    // Layer two states (for Error state)
    mp_ErrorWaitState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRsStandbyState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRsStandbyWithTissueState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRcLevelSensorHeatingOvertimeState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRcRestartState= QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRsHeatingErr30SRetryState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRsTSensorErr3MinRetryState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRsRVGetOriginalPositionAgainState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_RcPressure = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_RcVacuum = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_RcFilling = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_RcDraining = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_RsDrainAtOnce = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_RcBottleCheckI = QSharedPointer<QState>(new QState(mp_RcBottleCheckI.data()));
    mp_ErrorRsFillingAfterFlushState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));

    // Set Initial states
    mp_SchedulerMachine->setInitialState(mp_InitState.data());
    mp_BusyState->setInitialState(mp_PssmInitState.data());
    mp_ErrorState->setInitialState(mp_ErrorWaitState.data());

    // Add transition
    mp_InitState->addTransition(this, SIGNAL(SchedulerInitComplete()), mp_IdleState.data());
    mp_IdleState->addTransition(this, SIGNAL(RunSignal()), mp_BusyState.data());
    mp_BusyState->addTransition(this, SIGNAL(RunComplete()), mp_IdleState.data());
    mp_IdleState->addTransition(this, SIGNAL(ErrorSignal()), mp_ErrorState.data());
    mp_InitState->addTransition(this, SIGNAL(ErrorSignal()), mp_ErrorState.data());
    mp_BusyState->addTransition(this, SIGNAL(ErrorSignal()), mp_ErrorState.data());
    mp_ErrorState->addTransition(this, SIGNAL(SigEnterRcRestart()), mp_BusyState.data());

    // Sate machines for Run handling
    mp_ProgramSelfTest = QSharedPointer<CProgramSelfTest>(new CProgramSelfTest(mp_SchedulerThreadController));

    // Run Handling related logic
    mp_PssmInitState->addTransition(this, SIGNAL(RunPreTest()), mp_PssmPreTestState.data());
    mp_PssmInitState->addTransition(this, SIGNAL(ResumeFillingRVRodHeating()), mp_PssmFillingHeatingRVState.data());
    mp_PssmInitState->addTransition(this, SIGNAL(ResumeFillingLevelSensorHeating()), mp_PssmFillingLevelSensorHeatingState.data());
    mp_PssmInitState->addTransition(this, SIGNAL(ResumeFiling()), mp_PssmFillingState.data());
    mp_PssmInitState->addTransition(this, SIGNAL(ResumeRVMoveToSeal()), mp_PssmRVMoveToSealState.data());
    mp_PssmInitState->addTransition(this, SIGNAL(ResumeProcessing()), mp_PssmProcessingState.data());
    mp_PssmInitState->addTransition(this, SIGNAL(ResumeRVMoveTube()), mp_PssmRVMoveToTubeState.data());
    mp_PssmInitState->addTransition(this, SIGNAL(ResumeDraining()), mp_PssmDrainingState.data());
    mp_PssmInitState->addTransition(this, SIGNAL(ResumeRVPosChange()), mp_PssmRVPosChangeState.data());

    mp_PssmPreTestState->addTransition(mp_ProgramSelfTest.data(), SIGNAL(TasksDone()), mp_PssmFillingHeatingRVState.data());
    mp_PssmFillingHeatingRVState->addTransition(this, SIGNAL(sigRVRodHeatingReady()), mp_PssmFillingLevelSensorHeatingState.data());
    mp_PssmFillingLevelSensorHeatingState->addTransition(this, SIGNAL(sigLevelSensorHeatingReady()), mp_PssmFillingState.data());
    CONNECTSIGNALSLOT(mp_PssmFillingState.data(), entered(), mp_SchedulerThreadController, Fill());
    CONNECTSIGNALSLOT(mp_PssmFillingState.data(), exited(), mp_SchedulerThreadController, OnStopFill());
    mp_PssmFillingState->addTransition(this, SIGNAL(sigFillFinished()), mp_PssmRVMoveToSealState.data());
    CONNECTSIGNALSLOT(mp_PssmRVMoveToSealState.data(), entered(), this, OnRVMoveToSeal());
    mp_PssmRVMoveToSealState->addTransition(this, SIGNAL(sigRVMoveToSealReady()), mp_PssmProcessingState.data());
    CONNECTSIGNALSLOT(mp_PssmProcessingState.data(), entered(), mp_SchedulerThreadController, OnEnterPssmProcessing());
    mp_PssmProcessingState->addTransition(this,SIGNAL(sigProcessingFinished()), mp_PssmRVMoveToTubeState.data());
    CONNECTSIGNALSLOT(mp_PssmRVMoveToTubeState.data(), entered(), this, OnRVMoveToTube());
    mp_PssmRVMoveToTubeState->addTransition(this,SIGNAL(sigRVMoveToTubeReady()), mp_PssmDrainingState.data());
    CONNECTSIGNALSLOT(mp_PssmDrainingState.data(), entered(), mp_SchedulerThreadController, Drain());
    CONNECTSIGNALSLOT(mp_PssmDrainingState.data(), exited(), mp_SchedulerThreadController, OnStopDrain());
    mp_PssmDrainingState->addTransition(this, SIGNAL(sigDrainFinished()), mp_PssmRVPosChangeState.data());
    CONNECTSIGNALSLOT(mp_PssmRVPosChangeState.data(), entered(), this, OnRVMoveToNextTube());
    mp_PssmRVPosChangeState->addTransition(this, SIGNAL(sigStepFinished()), mp_PssmStepFinish.data());
    mp_PssmStepFinish->addTransition(this, SIGNAL(sigStepProgramFinished()),mp_PssmFillingHeatingRVState.data());
    mp_PssmStepFinish->addTransition(this, SIGNAL(sigProgramFinished()), mp_PssmProgramFinish.data());

    // State machines for Error handling
    mp_RsRvGetOriginalPositionAgain = QSharedPointer<CRsRvGetOriginalPositionAgain>(new CRsRvGetOriginalPositionAgain(mp_SchedulerMachine.data(), mp_ErrorState.data()));
    mp_RsStandby = QSharedPointer<CRsStandbyWithTissue>(new CRsStandbyWithTissue(SchedulerThreadController, 1));
    mp_RsHeatingErr30SRetry = QSharedPointer<CRsHeatingErr30SRetry>(new CRsHeatingErr30SRetry(SchedulerThreadController));
    mp_RsTSensorErr3MinRetry = QSharedPointer<CRsTSensorErr3MinRetry>(new CRsTSensorErr3MinRetry(SchedulerThreadController));
    mp_RsStandbyWithTissue = QSharedPointer<CRsStandbyWithTissue>(new CRsStandbyWithTissue(SchedulerThreadController));
    mp_RcRestart = QSharedPointer<CRcRestart>(new CRcRestart(mp_SchedulerMachine.data(), mp_ErrorRcRestartState.data()));
    mp_RsFillingAfterFlush = QSharedPointer<CRsFillingAfterFlush>(new CRsFillingAfterFlush(SchedulerThreadController));

    //RS_Standby related logic
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRsStandBy()), mp_ErrorRsStandbyState.data());
    CONNECTSIGNALSLOT(mp_RsStandby.data(), TasksDone(bool), this, OnTasksDone(bool));
    mp_ErrorRsStandbyState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RS_Standby_WithTissue related logic
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRsStandByWithTissue()), mp_ErrorRsStandbyWithTissueState.data());
    CONNECTSIGNALSLOT(mp_RsStandbyWithTissue.data(), TasksDone(bool), this, OnTasksDone(bool));
    mp_ErrorRsStandbyWithTissueState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RS_HeatingErr30SRetry related logic
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRsHeatingErr30SRetry()), mp_ErrorRsHeatingErr30SRetryState.data());
    CONNECTSIGNALSLOT(mp_RsHeatingErr30SRetry.data(), TasksDone(bool), this, OnTasksDone(bool));
    mp_ErrorRsHeatingErr30SRetryState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //Rs_TSensorErr3MinRetry
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRSTSensorErr3MinRetry()), mp_ErrorRsTSensorErr3MinRetryState.data());
    CONNECTSIGNALSLOT(mp_RsTSensorErr3MinRetry.data(), TasksDone(bool), this, OnTasksDone(bool));
    mp_ErrorRsTSensorErr3MinRetryState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RC_Levelsensor_Heating_Overtime related logic
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRcLevelsensorHeatingOvertime()), mp_ErrorRcLevelSensorHeatingOvertimeState.data());
    mp_ErrorRcLevelSensorHeatingOvertimeState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RS_RV_GetOriginalPositionAgain related logic
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRsRVGetOriginalPositionAgain()), mp_ErrorRsRVGetOriginalPositionAgainState.data());
    mp_ErrorRsRVGetOriginalPositionAgainState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RC_Pressure
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigRcPressure()), mp_RcPressure.data());
    CONNECTSIGNALSLOT(mp_RcPressure.data(), entered(), mp_SchedulerThreadController, Pressure());
    mp_RcPressure->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RC_Vacuum
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigRcVacuum()), mp_RcVacuum.data());
    CONNECTSIGNALSLOT(mp_RcVacuum.data(), entered(), mp_SchedulerThreadController, Vaccum());
    mp_RcVacuum->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RC_Filling
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigRcFilling()), mp_RcFilling.data());
    CONNECTSIGNALSLOT(this, SigRcFillingBegin(), mp_SchedulerThreadController, Fill());
    CONNECTSIGNALSLOT(mp_RcFilling.data(), exited(), mp_SchedulerThreadController, OnStopFill());
    mp_RcFilling->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RC_Draining
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigRcDraining()), mp_RcDraining.data());
    CONNECTSIGNALSLOT(mp_RcDraining.data(), entered(), mp_SchedulerThreadController, RCDrain());
    CONNECTSIGNALSLOT(mp_RcDraining.data(), exited(), mp_SchedulerThreadController, OnStopDrain());
    mp_RcDraining->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RS_DrainAtOnce
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigRsDrainAtOnce()), mp_RsDrainAtOnce.data());
    CONNECTSIGNALSLOT(mp_RsDrainAtOnce.data(), entered(), mp_SchedulerThreadController, Drain());
    CONNECTSIGNALSLOT(mp_RsDrainAtOnce.data(), exited(), mp_SchedulerThreadController, OnStopDrain());
    mp_RsDrainAtOnce->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RC_BottleCheck_I
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigRcBottleCheckI()), mp_RcBottleCheckI.data());
    CONNECTSIGNALSLOT(mp_RcBottleCheckI.data(), entered(), mp_SchedulerThreadController, RcBottleCheckI());
    mp_RcBottleCheckI->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RS_FillingAfterFlush
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigRsFillingAfterFlush()), mp_ErrorRsFillingAfterFlushState.data());
    CONNECTSIGNALSLOT(mp_RsFillingAfterFlush.data(), TasksDone(bool), this, OnTasksDone(bool));
    mp_ErrorRsFillingAfterFlushState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    m_RestartLevelSensor = RESTART_LEVELSENSOR;
    m_RVGetOriginalPosition = MOVE_TO_INITIAL_POS;
    m_RVOrgPosCmdTime = 0;
    m_RcFilling = HEATING_LEVELSENSOR;
}


void CSchedulerStateMachine::OnStartDeviceTempCtrl()
{

}

void CSchedulerStateMachine::OnCheckDeviceStatus()
{

}

void CSchedulerStateMachine::OnTasksDone(bool flag)
{
    Global::EventObject::Instance().RaiseEvent(mp_SchedulerThreadController->GetEventKey(), 0, 0, flag);
    emit sigStateChange();
}

void CSchedulerStateMachine::OnRVMoveToSeal()
{
    mp_SchedulerThreadController->MoveRV(1);
}

void CSchedulerStateMachine::OnRVMoveToTube()
{
    mp_SchedulerThreadController->MoveRV(0);
}

void CSchedulerStateMachine::OnRVMoveToNextTube()
{
    mp_SchedulerThreadController->MoveRV(2);
}

/****************************************************************************/
/*!
 *  \brief    Destructor
 */
/****************************************************************************/
CSchedulerStateMachine::~CSchedulerStateMachine()
{

}

//mp_SelfTestStateMachine
void CSchedulerStateMachine::UpdateCurrentState(SchedulerStateMachine_t currentState)
{
    if(m_CurrentState != currentState )
    {
        QString msg = QString("%1 Previous state: %2, Current State: %3").arg(QDateTime::currentDateTime().toString()).arg(m_CurrentState,0,16).arg(currentState, 0, 16);
        Global::EventObject::Instance().RaiseEvent(Global::EVENT_GLOBAL_STRING_ID_DEBUG_MESSAGE,Global::tTranslatableStringList()<<msg);
        qDebug()<<"DBG"<< QDateTime::currentDateTime()<<"Previous state is: "<<hex<<m_CurrentState<<" Current State is:" <<hex<<currentState;
        m_PreviousState = m_CurrentState;
        m_CurrentState = currentState;
    }
}

void CSchedulerStateMachine::Start()
{
    mp_SchedulerMachine->start();
}

void CSchedulerStateMachine::Stop()
{
    mp_SchedulerMachine->stop();
}

void CSchedulerStateMachine::SendSchedulerInitComplete()
{
    emit SchedulerInitComplete();
}

void CSchedulerStateMachine::SendRunSignal()
{
    emit RunSignal();
}

void CSchedulerStateMachine::SendRunComplete()
{
    emit RunComplete();
}

void CSchedulerStateMachine::SendResumeFillingRVRodHeating()
{
    emit ResumeFillingRVRodHeating();
}

void CSchedulerStateMachine::SendResumeFillingLevelSensorHeating()
{
    emit ResumeFillingLevelSensorHeating();
}

void CSchedulerStateMachine::SendResumeFiling()
{
    emit ResumeFiling();
}

void CSchedulerStateMachine::SendResumeRVMoveToSeal()
{
    emit ResumeRVMoveToSeal();
}

void CSchedulerStateMachine::SendResumeProcessing()
{
    emit ResumeProcessing();
}

void CSchedulerStateMachine::SendResumeRVMoveTube()
{
    emit ResumeRVMoveTube();
}

void CSchedulerStateMachine::SendResumeDraining()
{
    emit ResumeDraining();
}

void CSchedulerStateMachine::SendResumeRVPosChange()
{
    emit ResumeRVPosChange();
}

void CSchedulerStateMachine::SendErrorSignal()
{
    emit ErrorSignal();
}

SchedulerStateMachine_t CSchedulerStateMachine::GetCurrentState()
{
    SchedulerStateMachine_t currentState = SM_UNDEF;
    if(mp_SchedulerMachine->configuration().contains(mp_InitState.data()))
    {
        currentState = SM_INIT;
    }
    else if(mp_SchedulerMachine->configuration().contains(mp_IdleState.data()))
    {
        currentState = SM_IDLE;
    }
    else if(mp_SchedulerMachine->configuration().contains(mp_ErrorState.data()))
    {
        if(mp_SchedulerMachine->configuration().contains(mp_ErrorWaitState.data()))
        {
            return SM_ERR_WAIT;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRsRVGetOriginalPositionAgainState.data()))
        {
            return SM_ERR_RS_RV_GETORIGINALPOSITIONAGAIN;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRsStandbyState.data()))
        {
            return SM_ERR_RS_STANDBY;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRsHeatingErr30SRetryState.data()))
        {
            return SM_ERR_RS_HEATINGERR30SRETRY;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_ErrorRsTSensorErr3MinRetryState.data()))
        {
            return SM_ERR_RS_TSENSORERR_3MINRETRY;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRsStandbyWithTissueState.data()))
        {
            return SM_ERR_RS_STANDBY_WITH_TISSUE;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRcLevelSensorHeatingOvertimeState.data()))
        {
            return SM_ERR_RC_LEVELSENSOR_HEATING_OVERTIME;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRcRestartState.data()))
        {
            return SM_ERR_RC_RESTART;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_RcPressure.data()))
        {
            return SM_ERR_RC_PRESSURE;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_RcVacuum.data()))
        {
            return SM_ERR_RC_VACUUM;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_RcFilling.data()))
        {
            return SM_ERR_RC_FILLING;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_RcDraining.data()))
        {
            return SM_ERR_RC_DRAINING;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_RsDrainAtOnce.data()))
        {
            return SM_ERR_RS_DRAINATONCE;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_RcBottleCheckI.data()))
        {
            return SM_ERR_RS_BOTTLECHECK_I;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRsFillingAfterFlushState.data()))
        {
            return SM_ERR_RS_FILLINGAFTERFFLUSH;
        }
    }
    else if(mp_SchedulerMachine->configuration().contains(mp_BusyState.data()))
    {
        if(mp_SchedulerMachine->configuration().contains(mp_PssmInitState.data()))
        {
            return PSSM_INIT;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmPreTestState.data()))
        {
            return PSSM_PRETEST;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmFillingHeatingRVState.data()))
        {
            return PSSM_FILLING_RVROD_HEATING;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmFillingLevelSensorHeatingState.data()))
        {
            return PSSM_FILLING_LEVELSENSOR_HEATING;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmFillingState.data()))
        {
            return PSSM_FILLING;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmRVMoveToSealState.data()))
        {
            return PSSM_RV_MOVE_TO_SEAL;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmProcessingState.data()))
        {
            return PSSM_PROCESSING;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_PssmRVMoveToTubeState.data()))
        {
            return PSSM_RV_MOVE_TO_TUBE;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmDrainingState.data()))
        {
            return PSSM_DRAINING;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmRVPosChangeState.data()))
        {
            return PSSM_RV_POS_CHANGE;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmStepFinish.data()))
        {
            return PSSM_STEP_PROGRAM_FINISH;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmProgramFinish.data()))
        {
            return PSSM_PROGRAM_FINISH;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmPause.data()))
        {
            return PSSM_PAUSE;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmPauseDrain.data()))
        {
            return PSSM_PAUSE_DRAIN;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmAborting.data()))
        {
            return PSSM_ABORTING;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmAborted.data()))
        {
            return PSSM_ABORTED;
        }
    }

    return currentState;
}

SchedulerStateMachine_t CSchedulerStateMachine::GetPreviousState()
{
    return m_PreviousState;
}

void CSchedulerStateMachine::NotifyStInitOK()
{
    emit sigStInitOK();
}

void CSchedulerStateMachine::NotifyStTempOK()
{
    emit sigStTempOK();
}

void CSchedulerStateMachine::NotifyStCurrentOK()
{
    emit sigStCurrentOK();
}

void CSchedulerStateMachine::NotifyStVoltageOK()
{
    emit sigStVoltageOK();
}

void CSchedulerStateMachine::NotifyStRVPositionOK()
{
    emit sigStRVPositionOK();
}

void CSchedulerStateMachine::NotifyStPressureOK()
{
    emit sigStPressureOK();
}

void CSchedulerStateMachine::NotifyStSealingOK()
{
    emit sigStSealingOK();
}

void CSchedulerStateMachine::NotifyStGetStationcheckResult()
{
    emit sigStGetStationcheckResult();
}

void CSchedulerStateMachine::NotifyStStationLeft()
{
    emit sigStStationLeft();
}

void CSchedulerStateMachine::NotifyStStationOK()
{
    emit sigStStationOK();
}

void CSchedulerStateMachine::NotifyStDone()
{
    emit sigStDone();
}
void CSchedulerStateMachine::NotifyTempsReady()
{
    emit sigTempsReady();
}

void CSchedulerStateMachine::NotifyLevelSensorHeatingReady()
{
    emit sigLevelSensorHeatingReady();
}

void CSchedulerStateMachine::NotifyRVRodHeatingReady()
{
    emit sigRVRodHeatingReady();
}

void CSchedulerStateMachine::NotifyRVMoveToSealReady()
{
    emit sigRVMoveToSealReady();
}
void CSchedulerStateMachine::NotifyRVMoveToTubeReady()
{
    emit sigRVMoveToTubeReady();
}

void CSchedulerStateMachine::NotifyHitTubeBefore()
{
    emit sigHitTubeBefore();
}

void CSchedulerStateMachine::NotifyFillFinished()
{
    emit sigFillFinished();
}

void CSchedulerStateMachine::NotifyHitSeal()
{
    emit sigHitSeal();
}

void CSchedulerStateMachine::NotifyProcessingFinished()
{
    emit sigProcessingFinished();
}

void CSchedulerStateMachine::NotifyHitTubeAfter()
{
    emit sigHitTubeAfter();
}

void CSchedulerStateMachine::NotifyDrainFinished()
{
    emit sigDrainFinished();
}

void CSchedulerStateMachine::NotifyStepFinished()
{
    emit sigStepFinished();
}

void CSchedulerStateMachine::NotifyProgramFinished()
{
    emit sigProgramFinished();
}

void CSchedulerStateMachine::NotifyStepProgramFinished()
{
    emit sigStepProgramFinished();
}

void CSchedulerStateMachine::NotifyError()
{
    emit sigError();
}


void CSchedulerStateMachine::NotifyPause(SchedulerStateMachine_t PreviousState)
{
    Q_UNUSED(PreviousState);
    emit sigPause();
}

void CSchedulerStateMachine::NotifyAbort()
{
    emit sigAbort();
}

void CSchedulerStateMachine::NotifyRsRvMoveToInitPositionFinished()
{
    emit sigRsRvMoveToInitPositionFinished();
}

void CSchedulerStateMachine::NotifyRcReport()
{
    emit sigRcReport();
}

void CSchedulerStateMachine::NotifyRsReleasePressure()
{
    emit sigReleasePressure();
}

void CSchedulerStateMachine::NotifyRsShutdownFailedHeater()
{
    emit sigShutdownFailedHeater();
}

void CSchedulerStateMachine::NotifyRsShutdownFailedHeaterFinished()
{
    emit sigShutdownFailedHeaterFinished();
}

void CSchedulerStateMachine::NotifyResumeDrain()
{
    if(this->GetPreviousState() == (PSSM_RV_MOVE_TO_TUBE))
    {
        emit sigRVMoveToTubeReady();
    }
    else if(this->GetPreviousState() == (PSSM_DRAINING))
    {
        emit sigDrainFinished();
    }
}

void CSchedulerStateMachine::EnterRsStandBy()
{
    emit SigEnterRsStandBy();
}

void CSchedulerStateMachine::EnterRsHeatingErr30SRetry()
{
    emit SigEnterRsHeatingErr30SRetry();
}

void CSchedulerStateMachine::EnterRsTSensorErr3MinRetry()
{
    emit SigEnterRSTSensorErr3MinRetry();
}

void CSchedulerStateMachine::EnterRsStandByWithTissue()
{
    emit SigEnterRsStandByWithTissue();
}

void CSchedulerStateMachine::EnterRcPressure()
{
    emit SigRcPressure();
}

void CSchedulerStateMachine::EnterRcVacuum()
{
    emit SigRcVacuum();
}

void CSchedulerStateMachine::EnterRcFilling()
{
    emit SigRcFilling();
}

void CSchedulerStateMachine::EnterRcDraining()
{
    emit SigRcDraining();
}

void CSchedulerStateMachine::EnterRsDrainAtOnce()
{
    emit SigRsDrainAtOnce();
}

void CSchedulerStateMachine::EnterRcBottleCheckI()
{
    emit SigRcBottleCheckI();
}

void CSchedulerStateMachine::EnterRsFillingAfterFlush()
{
    emit SigRsFillingAfterFlush();
}

void CSchedulerStateMachine::HandlePssmPreTestWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
    mp_ProgramSelfTest->HandleWorkFlow(cmdName, retCode);
}

void CSchedulerStateMachine::HandleRsStandByWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
    mp_RsStandby->HandleWorkFlow(cmdName, retCode);
}

void CSchedulerStateMachine::HandleRsHeatingErr30SRetry()
{
    mp_RsHeatingErr30SRetry->HandleWorkFlow();
}

void CSchedulerStateMachine::HandleRsTSensorErr3MinRetry(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    mp_RsTSensorErr3MinRetry->HandleWorkFlow(cmdName, retCode);
}

void CSchedulerStateMachine::HandleRsStandByWithTissueWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
    mp_RsStandbyWithTissue->HandleWorkFlow(cmdName, retCode);
}

void CSchedulerStateMachine::EnterRcLevelsensorHeatingOvertime()
{
    emit SigEnterRcLevelsensorHeatingOvertime();
}

void CSchedulerStateMachine::HandleRcLevelSensorHeatingOvertimeWorkFlow()
{
    qreal tempLevelSensor = 0.0;
    quint16 retValue = 0;

    switch (m_RestartLevelSensor)
    {
    case RESTART_LEVELSENSOR:
        if (DCL_ERR_FCT_CALL_SUCCESS != mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControl("LevelSensor"))
        {
            this->OnTasksDone(false);
        }
        else
        {
            m_RestartLevelSensor = CHECK_TEMPERATURE;
        }
        break;
    case CHECK_TEMPERATURE:
        tempLevelSensor = mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().TempALLevelSensor;
        retValue = mp_SchedulerThreadController->GetHeatingStrategy()->CheckTemperatureOverTime("LevelSensor",tempLevelSensor);
        if (0 == retValue)
        {
            // Do nothing
        }
        else if (1 == retValue)
        {
            m_RestartLevelSensor = RESTART_LEVELSENSOR;
            this->OnTasksDone(false);
        }
        else if (2 == retValue)
        {
            m_RestartLevelSensor = RESTART_LEVELSENSOR;
            this->OnTasksDone(true);
        }
        break;
    default:
        break;
    }

 }

void CSchedulerStateMachine::HandleRsRVGetOriginalPositionAgainWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
    switch (m_RVGetOriginalPosition)
    {
    case MOVE_TO_INITIAL_POS:
        mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdRVReqMoveToInitialPosition(500, mp_SchedulerThreadController));
        m_RVGetOriginalPosition = CHECK_INITIAL_POS_RESULT;
        m_RVOrgPosCmdTime = QDateTime::currentMSecsSinceEpoch();
        break;
    case CHECK_INITIAL_POS_RESULT:
        if (1 == mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().PositionRV) //Initial position
        {
            m_RVGetOriginalPosition = MOVE_TO_INITIAL_POS;
            this->OnTasksDone(true);
        }
        else if ("Scheduler::RVReqMoveToInitialPosition" == cmdName)
        {
            if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                // do nothing
            }
            else
            {
                this->OnTasksDone(false);
            }
        }
        else if ((QDateTime::currentMSecsSinceEpoch()) - m_RVOrgPosCmdTime > 120*1000)
        {
            this->OnTasksDone(false);
        }
    }
}

void CSchedulerStateMachine::HandleRcPressureWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    if( "Scheduler::ALPressure" == cmdName)
    {
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            OnTasksDone(false);
        }
        else
        {
            OnTasksDone(true);
        }
    }
}

void CSchedulerStateMachine::HandleRcVacuumWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    if( "Scheduler::ALVaccum" == cmdName)
    {
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            OnTasksDone(false);
        }
        else
        {
            OnTasksDone(true);
        }
    }
}

void CSchedulerStateMachine::HandleRcFillingWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    qreal tempLevelSensor = 0.0;
    quint16 retValue = 0;
    switch(m_RcFilling)
    {
    case HEATING_LEVELSENSOR:
        if (DCL_ERR_FCT_CALL_SUCCESS != mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControl("LevelSensor"))
        {
            this->OnTasksDone(false);
        }
        else
        {
            m_RcFilling = CHECK_FILLINGTEMP;
        }
        break;
    case CHECK_FILLINGTEMP:
        tempLevelSensor = mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().TempALLevelSensor;
        retValue = mp_SchedulerThreadController->GetHeatingStrategy()->CheckTemperatureOverTime("LevelSensor",tempLevelSensor);
        if (0 == retValue)
        {
            // Do nothing
        }
        else if (1 == retValue)
        {
            m_RcFilling = CHECK_FILLINGTEMP;
            this->OnTasksDone(false);
        }
        else if (2 == retValue)
        {
            m_RcFilling = RC_FILLING;
            emit SigRcFillingBegin();
        }
        break;
    case RC_FILLING:
        if( "Scheduler::ALFilling" == cmdName)
        {
            if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
            {
                OnTasksDone(false);
                m_RcFilling = HEATING_LEVELSENSOR;
            }
            else
            {
                OnTasksDone(true);
                m_RcFilling = HEATING_LEVELSENSOR;
            }
        }
    default:
        break;
    }

}

void CSchedulerStateMachine::HandleDrainingWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    if( "Scheduler::ALDraining" == cmdName)
    {
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            OnTasksDone(false);
        }
        else
        {
            OnTasksDone(true);
        }
    }
}

void CSchedulerStateMachine::HandleRcBottleCheckIWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
    if ( "Scheduler::IDBottleCheck" == cmdName)
    {
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            OnTasksDone(false);
        }
        else
        {
            OnTasksDone(true);
        }
    }
}

void CSchedulerStateMachine::HandleRsFillingAfterFlushWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
    mp_RsFillingAfterFlush->HandleWorkFlow(cmdName, retCode);
}

void CSchedulerStateMachine::EnterRcRestart()
{
    emit SigEnterRcRestart();
}

void CSchedulerStateMachine::EnterRsRVGetOriginalPositionAgain()
{
    emit SigEnterRsRVGetOriginalPositionAgain();
}
void CSchedulerStateMachine::SendRunPreTest()
{
    emit RunPreTest();
}

QString CSchedulerStateMachine::GetDeviceName()
{
    ReturnCode_t EventID = mp_SchedulerThreadController->GetCurErrEventID();

    QString DevName = "";
    switch (EventID)
    {
    case DCL_ERR_DEV_RETORT_BOTTOM_HEATING_ELEMENT_FAILED:
    case DCL_ERR_DEV_RETORT_SIDETOP_HEATING_ELEMENT_FAILED:
    case DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_OVERRANGE:
    case DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_NOSIGNAL:
    case DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_OVERRANGE:
    case DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_NOSIGNAL:
    case DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_OVERRANGE:
    case DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_NOSIGNAL:
    case DCL_ERR_DEV_RETORT_TSENSOR1_TO_2_SELFCALIBRATION_FAILED:
        DevName = "Retort";
        break;
    case DCL_ERR_DEV_LEVELSENSOR_TEMPERATURE_OVERRANGE:
        DevName = "LevelSensor";
        break;
    case DCL_ERR_DEV_WAXBATH_TOP_HEATINGPAD_CURRENT_OUTOFRANGE:
    case DCL_ERR_DEV_WAXBATH_BOTTOM_HEATINGPAD_CURRENT_OUTOFRANGE:
    case DCL_ERR_DEV_WAXBATH_TSENSORDOWN1_OUTOFRANGE:
    case DCL_ERR_DEV_WAXBATH_TSENSORDOWN2_OUTOFRANGE:
        DevName = "Oven";
        break;
    case DCL_ERR_DEV_RV_HEATING_TEMPSENSOR1_OUTOFRANGE:
    case DCL_ERR_DEV_RV_HEATING_TEMPSENSOR2_OUTOFRANGE:
    case DCL_ERR_DEV_RV_HEATING_CURRENT_OUTOFRANGE:
        DevName = "Rotary Valve";
        break;
    case DCL_ERR_DEV_LA_PRESSURESENSOR_OUTOFRANGE:
    case DCL_ERR_DEV_LA_STATUS_EXHAUSTFAN:
    case DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_ABNORMAL:
    case DCL_ERR_DEV_LA_TUBEHEATING_TUBE2_ABNORMAL:
    case DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR1_OUTOFRANGE:
    case DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR2_OUTOFRANGE:
        DevName = "LA";
        break;
    default:
        break;
    }
    return DevName;
}

}
