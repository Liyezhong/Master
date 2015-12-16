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
#include "Scheduler/Include/SchedulerEventCodes.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "Scheduler/Include/HeatingStrategy.h"
#include "Scheduler/Commands/Include/CmdRTSetTempCtrlOFF.h"
#include "Scheduler/Commands/Include/CmdALAllStop.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToInitialPosition.h"
#include "Scheduler/Commands/Include/CmdALReleasePressure.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToRVPosition.h"
#include "Scheduler/Commands/Include/CmdALStopCmdExec.h"
#include "Scheduler/Commands/Include/CmdIDForceDraining.h"
#include "Scheduler/Commands/Include/CmdALVaccum.h"
#include "Scheduler/Commands/Include/CmdIDBottleCheck.h"
#include "Scheduler/Include/RsStandbyWithTissue.h"
#include "Scheduler/Include/RsHeatingErr30SRetry.h"
#include "Scheduler/Include/RsPressureOverRange3SRetry.h"
#include "Scheduler/Include/RsTSensorErr3MinRetry.h"
#include "Scheduler/Include/ProgramPreTest.h"
#include "Scheduler/Include/ProgramSelfTest.h"
#include "Scheduler/Include/RsFillingAfterFlush.h"
#include "Scheduler/Include/RsTissueProtect.h"
#include "Scheduler/Include/RcReHeating.h"
#include "EventHandler/Include/StateHandler.h"
#include <QDebug>
#include <QDateTime>
#include <QCoreApplication>

namespace Scheduler
{
/*lint -e534 */
/*lint -e616 */

CSchedulerStateMachine::CSchedulerStateMachine(SchedulerMainThreadController* SchedulerThreadController)
    :mp_SchedulerThreadController(SchedulerThreadController)
{
    qRegisterMetaType<DeviceControl::RTTempCtrlType_t>("DeviceControl::RTTempCtrlType_t");
    m_PreviousState = SM_UNDEF;
    m_CurrentState = SM_UNDEF;

    mp_SchedulerMachine = QSharedPointer<QStateMachine>(new QStateMachine());

    // Layer one states
    mp_InitState = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    CONNECTSIGNALSLOT(mp_InitState.data(), entered(), this, OnEnterInitState());
    CONNECTSIGNALSLOT(mp_InitState.data(), exited(), mp_SchedulerThreadController, OnExitedInitState());
    mp_PowerFailureState = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    mp_IdleState = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    CONNECTSIGNALSLOT(mp_IdleState.data(), entered(), this, OnEnterIdleState());
    CONNECTSIGNALSLOT(mp_IdleState.data(), exited(), mp_SchedulerThreadController, OnExitIdleState());
    mp_BusyState = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    CONNECTSIGNALSLOT(mp_BusyState.data(), entered(), this, OnEnterBusyState());
    mp_ErrorState = QSharedPointer<QState>(new QState(mp_SchedulerMachine.data()));
    CONNECTSIGNALSLOT(mp_ErrorState.data(), entered(), this, OnEnterErrorState());
    CONNECTSIGNALSLOT(mp_PowerFailureState.data(), entered(), this, OnPowerFailureState());

    // Layer two states (for Init state)
    mp_SelfTestState = QSharedPointer<QState>(new QState(mp_InitState.data()));
    CONNECTSIGNALSLOT(mp_SelfTestState.data(), entered(), this, OnEnterSelfTest());

    // Layer two states (for Busy state)
    mp_PssmInitState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    CONNECTSIGNALSLOT(mp_PssmInitState.data(), entered(), mp_SchedulerThreadController, OnStartTimer());
    mp_PssmPreTestState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    CONNECTSIGNALSLOT(mp_PssmPreTestState.data(), entered(), this, OnEnterPretest());
    mp_PssmFillingHeatingRVState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmFillingLevelSensorHeatingState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    CONNECTSIGNALSLOT(mp_PssmFillingLevelSensorHeatingState.data(), entered(), this, OnEnterHeatingLevelSensor());
    mp_PssmFillingState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmRVMoveToSealState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmProcessingState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmRVMoveToTubeState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmDrainingState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmRVPosChangeState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmStepFinish = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmCleaningDryStep = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmProgramFinish = QSharedPointer<QFinalState>(new QFinalState(mp_BusyState.data()));
    mp_PssmPause = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    mp_PssmBottleCheckState = QSharedPointer<QState>(new QState(mp_BusyState.data()));
    CONNECTSIGNALSLOT(mp_PssmBottleCheckState.data(), entered(), this, OnEnterBottleCheckState());

    // Layer two states (for Error state)
    mp_ErrorWaitState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    CONNECTSIGNALSLOT(mp_ErrorWaitState.data(), entered(), mp_SchedulerThreadController, OnStartTimer());
    mp_ErrorRsStandbyState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRsStandbyWithTissueState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRcLevelSensorHeatingOvertimeState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRcRestartState= QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRsHeatingErr30SRetryState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRsPressureOverRange3SRetryState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRsTSensorErr3MinRetryState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRsRVGetOriginalPositionAgainState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_RcPressure = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_RcVacuum = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_RcFilling = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_RcDraining = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_RcForceDraining = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_RsDrainAtOnce = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_RcBottleCheckI = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRsFillingAfterFlushState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRsCheckBlockageState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRsPauseState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRsRVWaitintTempUpState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRsTissueProtectState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRcCheckRTLockState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRcReHeatingState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRsRvMoveToSealPositionState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));
    mp_ErrorRsAbortState = QSharedPointer<QState>(new QState(mp_ErrorState.data()));

    // Set Initial states
    mp_SchedulerMachine->setInitialState(mp_InitState.data());
    mp_InitState->setInitialState(mp_SelfTestState.data());
    mp_BusyState->setInitialState(mp_PssmInitState.data());
    mp_ErrorState->setInitialState(mp_ErrorWaitState.data());

    // Add transition
    mp_InitState->addTransition(this, SIGNAL(SchedulerInitComplete()), mp_IdleState.data());
    mp_InitState->addTransition(this, SIGNAL(SigPowerFailure()), mp_PowerFailureState.data());

    mp_IdleState->addTransition(this, SIGNAL(RunSignal()), mp_BusyState.data());
    mp_BusyState->addTransition(this, SIGNAL(RunComplete()), mp_IdleState.data());
    mp_IdleState->addTransition(this, SIGNAL(ErrorSignal()), mp_ErrorState.data());
    mp_PowerFailureState->addTransition(this, SIGNAL(ErrorSignal()), mp_ErrorState.data());
    mp_InitState->addTransition(this, SIGNAL(ErrorSignal()), mp_ErrorState.data());
    mp_BusyState->addTransition(this, SIGNAL(ErrorSignal()), mp_ErrorState.data());
    mp_ErrorState->addTransition(this, SIGNAL(SigBackToBusy()), mp_BusyState.data());
    mp_ErrorState->addTransition(this, SIGNAL(SigSelfRcRestart()), mp_InitState.data());
    mp_ErrorState->addTransition(this, SIGNAL(SigIdleRcRestart()), mp_IdleState.data());
    mp_ErrorState->addTransition(this, SIGNAL(RunComplete()), mp_IdleState.data());
    CONNECTSIGNALSLOT(this, SigBackToBusy(), mp_SchedulerThreadController, OnBackToBusy());
    CONNECTSIGNALSLOT(mp_ErrorState.data(), entered(), mp_SchedulerThreadController, OnSystemError());

    // Sate machines for Run handling
    mp_ProgramPreTest = QSharedPointer<CProgramPreTest>(new CProgramPreTest(mp_SchedulerThreadController));
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
    mp_PssmInitState->addTransition(this, SIGNAL(ResumeDryStep()), mp_PssmCleaningDryStep.data());
    mp_PssmInitState->addTransition(this, SIGNAL(SigRunBottleCheck()), mp_PssmBottleCheckState.data());

    mp_PssmPreTestState->addTransition(mp_ProgramPreTest.data(), SIGNAL(TasksDone()), mp_PssmFillingHeatingRVState.data());

    CONNECTSIGNALSLOT(mp_PssmFillingHeatingRVState.data(), entered(), mp_SchedulerThreadController, OnFillingHeatingRV());

    mp_PssmFillingHeatingRVState->addTransition(this, SIGNAL(sigRVRodHeatingReady()), mp_PssmFillingLevelSensorHeatingState.data());
    mp_PssmFillingHeatingRVState->addTransition(this, SIGNAL(sigPause()), mp_PssmPause.data());
    mp_PssmPause->addTransition(this, SIGNAL(sigRVRodHeatingReady()), mp_PssmFillingLevelSensorHeatingState.data());

    mp_PssmFillingLevelSensorHeatingState->addTransition(this, SIGNAL(sigLevelSensorHeatingReady()), mp_PssmFillingState.data());
    mp_PssmFillingLevelSensorHeatingState->addTransition(this, SIGNAL(sigPause()), mp_PssmPause.data());
    CONNECTSIGNALSLOT(mp_PssmFillingState.data(), entered(), mp_SchedulerThreadController, Fill());

    mp_PssmFillingState->addTransition(this, SIGNAL(sigPause()), mp_PssmPause.data());
    mp_PssmPause->addTransition(this, SIGNAL(sigLevelSensorHeatingReady()), mp_PssmFillingState.data());
    mp_PssmPause->addTransition(this, SIGNAL(sigRVMoveToSealReady()), mp_PssmProcessingState.data());
    mp_PssmPause->addTransition(this, SIGNAL(sigResumeToProcessing()), mp_PssmProcessingState.data());

    CONNECTSIGNALSLOT(mp_PssmFillingState.data(), exited(), mp_SchedulerThreadController, OnStopFill());
    mp_PssmFillingState->addTransition(this, SIGNAL(sigFillFinished()), mp_PssmRVMoveToSealState.data());
    CONNECTSIGNALSLOT(mp_PssmRVMoveToSealState.data(), entered(), this, OnRVMoveToSeal());
    mp_PssmRVMoveToSealState->addTransition(this, SIGNAL(sigRVMoveToSealReady()), mp_PssmProcessingState.data());
    mp_PssmRVMoveToSealState->addTransition(this, SIGNAL(sigPause()), mp_PssmPause.data());

    CONNECTSIGNALSLOT(mp_PssmProcessingState.data(), entered(), mp_SchedulerThreadController, OnEnterPssmProcessing());

    mp_PssmProcessingState->addTransition(this,SIGNAL(sigProcessingFinished()), mp_PssmRVMoveToTubeState.data());
    CONNECTSIGNALSLOT(mp_PssmRVMoveToTubeState.data(), entered(), this, InitRVMoveToTubeState());
    mp_PssmRVMoveToTubeState->addTransition(this,SIGNAL(sigRVMoveToTubeReady()), mp_PssmDrainingState.data());
    mp_PssmRVMoveToTubeState->addTransition(this,SIGNAL(sigPause()), mp_PssmPause.data());
    CONNECTSIGNALSLOT(mp_PssmDrainingState.data(), entered(), mp_SchedulerThreadController, Drain());
    CONNECTSIGNALSLOT(mp_PssmDrainingState.data(), exited(), mp_SchedulerThreadController, OnStopDrain());
    mp_PssmDrainingState->addTransition(this, SIGNAL(sigDrainFinished()), mp_PssmRVPosChangeState.data());

    mp_PssmDrainingState->addTransition(this, SIGNAL(sigPause()), mp_PssmPause.data());
    mp_PssmPause->addTransition(this, SIGNAL(sigDrainFinished()), mp_PssmRVPosChangeState.data());

    mp_PssmDrainingState->addTransition(this, SIGNAL(sigProgramFinished()), mp_PssmProgramFinish.data());

    CONNECTSIGNALSLOT(mp_PssmRVPosChangeState.data(), entered(), this, OnRVMoveToNextTube());
    mp_PssmRVPosChangeState->addTransition(this, SIGNAL(sigStepFinished()), mp_PssmStepFinish.data());
    mp_PssmRVPosChangeState->addTransition(this, SIGNAL(sigPause()), mp_PssmPause.data());
    mp_PssmPause->addTransition(this, SIGNAL(sigStepFinished()), mp_PssmStepFinish.data());
    CONNECTSIGNALSLOT(this, sigStepFinished(), mp_SchedulerThreadController, DisablePauseButton());

    mp_PssmStepFinish->addTransition(this, SIGNAL(sigStepProgramFinished()),mp_PssmFillingHeatingRVState.data());
    mp_PssmStepFinish->addTransition(this, SIGNAL(sigProgramFinished()), mp_PssmProgramFinish.data());
    mp_PssmStepFinish->addTransition(this, SIGNAL(sigEnterDryStep()), mp_PssmCleaningDryStep.data());
    CONNECTSIGNALSLOT(mp_PssmStepFinish.data(), entered(), mp_SchedulerThreadController, OnEnterPssMStepFin());
    CONNECTSIGNALSLOT(mp_PssmProgramFinish.data(), entered(), mp_SchedulerThreadController, OnEnterPssMProgFin());

    mp_PssmCleaningDryStep->addTransition(this, SIGNAL(sigProgramFinished()), mp_PssmProgramFinish.data());
    CONNECTSIGNALSLOT(mp_PssmCleaningDryStep.data(), entered(), mp_SchedulerThreadController, OnEnterDryStepState());

    mp_PssmProcessingState->addTransition(this, SIGNAL(sigPause()), mp_PssmPause.data());
    CONNECTSIGNALSLOT(mp_PssmPause.data(), entered(), mp_SchedulerThreadController, Pause());

    CONNECTSIGNALSLOT(mp_SchedulerThreadController, NotifyResume(), this, OnNotifyResume());

    //For Pssm Aborting
    CONNECTSIGNALSLOT(this, sigOnForceDrain(), mp_SchedulerThreadController, OnBeginDrain());
    CONNECTSIGNALSLOT(this, sigOnStopForceDrain(), mp_SchedulerThreadController, OnStopDrain());
    CONNECTSIGNALSLOT(this, sigOnStopForceDrain(), mp_SchedulerThreadController, DisablePauseButton());

    // State machines for Error handling
    mp_RsStandby = QSharedPointer<CRsStandbyWithTissue>(new CRsStandbyWithTissue(SchedulerThreadController, this, 1));
    mp_RsHeatingErr30SRetry = QSharedPointer<CRsHeatingErr30SRetry>(new CRsHeatingErr30SRetry(SchedulerThreadController, this));
    mp_RsPressureOverRange3SRetry = QSharedPointer<CRsPressureOverRange3SRetry>(new CRsPressureOverRange3SRetry(SchedulerThreadController, this));
    mp_RsTSensorErr3MinRetry = QSharedPointer<CRsTSensorErr3MinRetry>(new CRsTSensorErr3MinRetry(SchedulerThreadController, this));
    mp_RsStandbyWithTissue = QSharedPointer<CRsStandbyWithTissue>(new CRsStandbyWithTissue(SchedulerThreadController, this));
    mp_RsFillingAfterFlush = QSharedPointer<CRsFillingAfterFlush>(new CRsFillingAfterFlush(SchedulerThreadController, this));
    mp_RsTissueProtect = QSharedPointer<CRsTissueProtect>(new CRsTissueProtect(SchedulerThreadController, this));
    mp_RcReHeating = QSharedPointer<CRcReHeating>(new CRcReHeating(SchedulerThreadController, this));

    //RS_Standby related logic
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRsStandBy()), mp_ErrorRsStandbyState.data());
    CONNECTSIGNALSLOT(mp_ErrorRsStandbyState.data(), entered(), mp_SchedulerThreadController, OnStartTimer());
    mp_ErrorRsStandbyState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RS_Standby_WithTissue related logic
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRsStandByWithTissue()), mp_ErrorRsStandbyWithTissueState.data());
    CONNECTSIGNALSLOT(mp_ErrorRsStandbyWithTissueState.data(), entered(), mp_SchedulerThreadController, OnStartTimer());
    mp_ErrorRsStandbyWithTissueState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RS_HeatingErr30SRetry related logic
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRsHeatingErr30SRetry()), mp_ErrorRsHeatingErr30SRetryState.data());
    CONNECTSIGNALSLOT(mp_ErrorRsHeatingErr30SRetryState.data(), entered(), mp_SchedulerThreadController, OnStartTimer());
    mp_ErrorRsHeatingErr30SRetryState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RS_PressureOverRange_3SRetry related logic
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRsPressureOverRange3SRetry()), mp_ErrorRsPressureOverRange3SRetryState.data());
    CONNECTSIGNALSLOT(mp_ErrorRsPressureOverRange3SRetryState.data(), entered(), mp_SchedulerThreadController, OnStartTimer());
    mp_ErrorRsPressureOverRange3SRetryState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //Rs_TSensorErr3MinRetry
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRSTSensorErr3MinRetry()), mp_ErrorRsTSensorErr3MinRetryState.data());
    CONNECTSIGNALSLOT(mp_ErrorRsTSensorErr3MinRetryState.data(), entered(), mp_SchedulerThreadController, OnStartTimer());
    mp_ErrorRsTSensorErr3MinRetryState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RC_Levelsensor_Heating_Overtime related logic
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRcLevelsensorHeatingOvertime()), mp_ErrorRcLevelSensorHeatingOvertimeState.data());
    CONNECTSIGNALSLOT(mp_ErrorRcLevelSensorHeatingOvertimeState.data(), entered(), mp_SchedulerThreadController, OnStartTimer());
    mp_ErrorRcLevelSensorHeatingOvertimeState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RS_RV_GetOriginalPositionAgain related logic
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRsRVGetOriginalPositionAgain()), mp_ErrorRsRVGetOriginalPositionAgainState.data());
    CONNECTSIGNALSLOT(mp_ErrorRsRVGetOriginalPositionAgainState.data(), entered(), mp_SchedulerThreadController, OnStartTimer());
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
    CONNECTSIGNALSLOT(mp_RcFilling.data(), entered(), mp_SchedulerThreadController, OnStartTimer());
    CONNECTSIGNALSLOT(this, SigRcFillingBegin(), mp_SchedulerThreadController, Fill());
    CONNECTSIGNALSLOT(mp_RcFilling.data(), exited(), mp_SchedulerThreadController, OnStopFill());
    mp_RcFilling->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RC_Draining
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigRcDraining()), mp_RcDraining.data());
    CONNECTSIGNALSLOT(mp_RcDraining.data(), entered(), mp_SchedulerThreadController, RCDrain());
    CONNECTSIGNALSLOT(mp_RcDraining.data(), exited(), mp_SchedulerThreadController, OnStopDrain());
    mp_RcDraining->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RC_ForceDraining
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigRcForceDraining()), mp_RcForceDraining.data());
    CONNECTSIGNALSLOT(mp_RcForceDraining.data(), entered(), mp_SchedulerThreadController, RCForceDrain());
    CONNECTSIGNALSLOT(mp_RcForceDraining.data(), exited(), mp_SchedulerThreadController, OnStopDrain());
    mp_RcForceDraining->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RS_DrainAtOnce
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigRsDrainAtOnce()), mp_RsDrainAtOnce.data());
    CONNECTSIGNALSLOT(mp_RsDrainAtOnce.data(), entered(), mp_SchedulerThreadController, RcDrainAtOnce());
    CONNECTSIGNALSLOT(mp_RsDrainAtOnce.data(), exited(), mp_SchedulerThreadController, OnStopDrain());
    mp_RsDrainAtOnce->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RC_BottleCheck_I
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigRcBottleCheckI()), mp_RcBottleCheckI.data());
    CONNECTSIGNALSLOT(mp_RcBottleCheckI.data(), entered(), mp_SchedulerThreadController, RcBottleCheckI());
    mp_RcBottleCheckI->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RS_FillingAfterFlush
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigRsFillingAfterFlush()), mp_ErrorRsFillingAfterFlushState.data());
    CONNECTSIGNALSLOT(mp_ErrorRsFillingAfterFlushState.data(), entered(), mp_SchedulerThreadController, OnStartTimer());
    mp_ErrorRsFillingAfterFlushState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RS_Check_Blockage
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigRsCheckBlockage()), mp_ErrorRsCheckBlockageState.data());
    CONNECTSIGNALSLOT(mp_ErrorRsCheckBlockageState.data(), entered(), mp_SchedulerThreadController, HighPressure());
    mp_ErrorRsCheckBlockageState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RS_Pause
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigRsPause()), mp_ErrorRsPauseState.data());
    CONNECTSIGNALSLOT(mp_ErrorRsPauseState.data(), entered(), mp_SchedulerThreadController, SendCoverLidOpenMsg());
    mp_ErrorRsPauseState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RS_RV_WaitingTempUp
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigRsRVWaitingTempUp()), mp_ErrorRsRVWaitintTempUpState.data());
    CONNECTSIGNALSLOT(mp_ErrorRsRVWaitintTempUpState.data(), entered(), mp_SchedulerThreadController, OnStartTimer());
    mp_ErrorRsRVWaitintTempUpState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RS_Tissue_Protect
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRsTissueProtect()), mp_ErrorRsTissueProtectState.data());
    CONNECTSIGNALSLOT(mp_ErrorRsTissueProtectState.data(), entered(), mp_SchedulerThreadController, OnStartTimer());
    mp_ErrorRsTissueProtectState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RC_Check_RTLock
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRcCheckRTLock()), mp_ErrorRcCheckRTLockState.data());
    CONNECTSIGNALSLOT(mp_ErrorRcCheckRTLockState.data(), entered(), mp_SchedulerThreadController, OnStartTimer());
    mp_ErrorRcCheckRTLockState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //Rc_ReHeating
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigRcReHeating()), mp_ErrorRcReHeatingState.data());
    CONNECTSIGNALSLOT(mp_ErrorRcReHeatingState.data(), entered(), mp_SchedulerThreadController, OnStartTimer());
    mp_ErrorRcReHeatingState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //Rs_Rv_MoveToPosition3.5
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigRsRvMoveToSealPositon()), mp_ErrorRsRvMoveToSealPositionState.data());
    CONNECTSIGNALSLOT(mp_ErrorRsRvMoveToSealPositionState.data(), entered(), mp_SchedulerThreadController, OnStartTimer());
    mp_ErrorRsRvMoveToSealPositionState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState.data());

    //RC_Restart
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRcRestart()), mp_ErrorRcRestartState.data());
    CONNECTSIGNALSLOT(mp_ErrorRcRestartState.data(), entered(), this, OnEnterErrorRcRestartState());

    //RS_Abort
    mp_ErrorWaitState->addTransition(this, SIGNAL(sigAbort()), mp_ErrorRsAbortState.data());
    CONNECTSIGNALSLOT(mp_ErrorRsAbortState.data(), entered(), this, OnEnterRsAbortState());

    m_RestartLevelSensor = RESTART_LEVELSENSOR;
    m_LevelSensorWaitTime = 0;
    m_RVGetOriginalPosition = MOVE_TO_INITIAL_POS;
    m_RVOrgPosCmdTime = 0;
    m_RcFilling = HEATING_LEVELSENSOR;
    m_RsCheckBlockage = BUILD_HIGHPRESSURE;
    m_RsCheckBlockageStartTime = 0;
    m_RsPauseStartTime = 0;
    m_RsPauseCount = 0;
    m_RsRVWaitingTempUp = STOP_HEATING;
    m_RsRVWaitingTempUpTime = 0;
    m_RcPressureSeq = 0;
    m_RcPressureDelayTime = 0;
    m_RcRestart_AtDrain = STOP_DRAINING;
    m_RsMoveToPSeal = BUILD_VACUUM;
    m_PssmAbortingSeq = 0;
    m_PssmAbortingInMoveToTube = false;
    m_PssmMVTubeSeq = 0;
    m_EnableLowerPressure = Global::Workaroundchecking("LOWER_PRESSURE");
    m_ErrorRcRestartSeq = 0;
    m_TimeReEnterFilling = 0;
    m_BottleCheckSeq = 0;
    m_PressureCalibrationSeq = 0;
    m_ReleasePressureTime = 0;
    m_PressureCalibrationCounter = 0;
    m_PressureDriftOffset = 0.0;
    m_NonRVErrorOccured = false;
}

void CSchedulerStateMachine::OnTasksDone(bool flag)
{
    mp_SchedulerThreadController->StopTimer();
    mp_SchedulerThreadController->RaiseError(mp_SchedulerThreadController->GetEventKey(), DCL_ERR_FCT_CALL_SUCCESS, 0, flag);
    emit sigStateChange();
}

void CSchedulerStateMachine::OnTasksDoneRsStandyWithTissue(bool flag)
{
    mp_SchedulerThreadController->StopTimer();
    mp_SchedulerThreadController->RaiseError(mp_SchedulerThreadController->GetEventKey(), DCL_ERR_FCT_CALL_SUCCESS, 0, flag);
    emit sigStateChange();
    if(DCL_ERR_DEV_LIDLOCK_CLOSE_STATUS_ERROR == mp_SchedulerThreadController->GetCurErrEventID())
    {
        mp_SchedulerThreadController->SendCoverLidOpenMsg();
    }
}

void CSchedulerStateMachine::OnTasksDoneRSTissueProtect(bool flag)
{
    mp_SchedulerThreadController->StopTimer();
    // stop Retort sensors heating
    mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("RTSide");
    mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("RTBottom");

    if (false == flag)
    {
        mp_SchedulerThreadController->RaiseError(0, DCL_ERR_DEV_INTER_TISSUE_PROTECT_REPORT, 0, false);
        emit sigStateChange();
    }
    else
    {
        emit SigIdleRcRestart();
    }
}

void CSchedulerStateMachine::OnRVMoveToSeal()
{
    mp_SchedulerThreadController->MoveRV(SEAL_POS);
    mp_SchedulerThreadController->StartTimer();
}

void CSchedulerStateMachine::InitRVMoveToTubeState()
{
    m_PssmMVTubeSeq = 0;
    mp_SchedulerThreadController->Pressure();
    m_PssmMVTubePressureTime = QDateTime::currentMSecsSinceEpoch();
    mp_SchedulerThreadController->StartTimer();
}

void CSchedulerStateMachine::OnEnterErrorRcRestartState()
{
    m_ErrorRcRestartSeq = 0;
    m_TimeReEnterFilling = 0;
    mp_SchedulerThreadController->StartTimer();
}

void CSchedulerStateMachine::OnEnterBottleCheckState()
{
    m_BottleCheckSeq = 0;
    m_PressureCalibrationSeq = 0;
    m_ReleasePressureTime = 0;
    m_PressureCalibrationCounter = 0;
    m_PressureDriftOffset = 0.0;
    m_NonRVErrorOccured = false;
    //Set pressure dirft to zero before pressure calibration
    mp_SchedulerThreadController->GetSchedCommandProcessor()->ALSetPressureDrift(0.0);
    m_BottleCheckStationIter = mp_SchedulerThreadController->GetDashboardStationList().begin();
    mp_SchedulerThreadController->SendBottleCheckReply("", DataManager::BOTTLECHECK_STARTED);
    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_BOTTLE_CHECK_START);
    mp_SchedulerThreadController->StartTimer();
}

void CSchedulerStateMachine::OnRVMoveToNextTube()
{
    mp_SchedulerThreadController->MoveRV(NEXT_TUBE_POS);
    mp_SchedulerThreadController->StartTimer();
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
        mp_SchedulerThreadController->LogDebug(msg);
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

void CSchedulerStateMachine::EnterPowerFailure()
{
    mp_SchedulerThreadController->StopTimer();
    emit SigPowerFailure();
}

void CSchedulerStateMachine::SendSchedulerInitComplete()
{
    mp_SchedulerThreadController->StopTimer();
    emit SchedulerInitComplete();
}

void CSchedulerStateMachine::SendRunSignal()
{
    mp_SchedulerThreadController->StopTimer();
    emit RunSignal();
}

void CSchedulerStateMachine::SendRunComplete()
{
    mp_SchedulerThreadController->StopTimer();
    emit RunComplete();
}

void CSchedulerStateMachine::SendResumeFillingRVRodHeating()
{
    mp_SchedulerThreadController->StopTimer();
    emit ResumeFillingRVRodHeating();
}

void CSchedulerStateMachine::SendResumeFillingLevelSensorHeating()
{
    mp_SchedulerThreadController->StopTimer();
    emit ResumeFillingLevelSensorHeating();
}

void CSchedulerStateMachine::SendResumeFiling()
{
    emit ResumeFiling();
}
void CSchedulerStateMachine::StartPreCheck()
{
    mp_ProgramPreTest->Start();
}

void CSchedulerStateMachine::SendResumeRVMoveToSeal()
{
    mp_SchedulerThreadController->StopTimer();
    emit ResumeRVMoveToSeal();
}

void CSchedulerStateMachine::SendResumeProcessing()
{
    mp_SchedulerThreadController->StopTimer();
    emit ResumeProcessing();
}

void CSchedulerStateMachine::SendResumeRVMoveTube()
{
    mp_SchedulerThreadController->StopTimer();
    emit ResumeRVMoveTube();
}

void CSchedulerStateMachine::SendResumeDraining()
{
    emit ResumeDraining();
}

void CSchedulerStateMachine::SendResumeRVPosChange()
{
    mp_SchedulerThreadController->StopTimer();
    emit ResumeRVPosChange();
}

void CSchedulerStateMachine::SendResumeDryStep()
{
    mp_SchedulerThreadController->StopTimer();
    emit ResumeDryStep();
}

void CSchedulerStateMachine::SendErrorSignal()
{
    mp_SchedulerThreadController->StopTimer();
    emit ErrorSignal();
}

SchedulerStateMachine_t CSchedulerStateMachine::GetCurrentState()
{
    SchedulerStateMachine_t currentState = SM_UNDEF;
    if(mp_SchedulerMachine->configuration().contains(mp_InitState.data()))
    {
        if(mp_SchedulerMachine->configuration().contains(mp_SelfTestState.data()))
        {
            currentState = SM_INIT_SELFTEST;
        }
    }
    else if(mp_SchedulerMachine->configuration().contains(mp_PowerFailureState.data()))
    {
        currentState = SM_POWER_FAILURE;
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
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRsPressureOverRange3SRetryState.data()))
        {
            return SM_ERR_RS_PRESSUREOVERRANGE3SRETRY;
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
        else if (mp_SchedulerMachine->configuration().contains(mp_RcForceDraining.data()))
        {
            return SM_ERR_RC_FORCEDRAINING;
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
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRsCheckBlockageState.data()))
        {
            return SM_ERR_RS_CHECK_BLOCKAGE;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRsPauseState.data()))
        {
            return SM_ERR_RS_PS_PAUSE;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRsRVWaitintTempUpState.data()))
        {
            return SM_ERR_RS_RV_WAITINGTEMPUP;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRsTissueProtectState.data()))
        {
            return SM_ERR_RS_TISSUE_PROTECT;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRcCheckRTLockState.data()))
        {
            return SM_ERR_RC_CHECK_RTLOCK;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRcReHeatingState.data()))
        {
            return SM_ERR_RC_REHEATING;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRsRvMoveToSealPositionState.data()))
        {
            return SM_ERR_RS_RV_MOVETOPOSITIONSEAL;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRsAbortState.data()))
        {
            return SM_ERR_RS_ABORT;
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
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmCleaningDryStep.data()))
        {
            return PSSM_CLEANING_DRY_STEP;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmProgramFinish.data()))
        {
            return PSSM_PROGRAM_FINISH;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmPause.data()))
        {
            return PSSM_PAUSE;
        }
        else if(mp_SchedulerMachine->configuration().contains(mp_PssmBottleCheckState.data()))
        {
            return PSSM_BOTTLE_CHECK;
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
     mp_SchedulerThreadController->StopTimer();
    emit sigLevelSensorHeatingReady();
}

void CSchedulerStateMachine::NotifyRVRodHeatingReady()
{
    mp_SchedulerThreadController->StopTimer();
    emit sigRVRodHeatingReady();
}

void CSchedulerStateMachine::NotifyRVMoveToSealReady()
{
    mp_SchedulerThreadController->StopTimer();
    emit sigRVMoveToSealReady();
}
void CSchedulerStateMachine::NotifyRVMoveToTubeReady()
{
    mp_SchedulerThreadController->StopTimer();
    emit sigRVMoveToTubeReady();
}

void CSchedulerStateMachine::NotifyFillFinished()
{
    mp_SchedulerThreadController->StopTimer();
    emit sigFillFinished();
}

void CSchedulerStateMachine::NotifyProcessingFinished()
{
    mp_SchedulerThreadController->StopTimer();
    emit sigProcessingFinished();
}

void CSchedulerStateMachine::NotifyDrainFinished()
{
    mp_SchedulerThreadController->StopTimer();
    emit sigDrainFinished();
}

void CSchedulerStateMachine::NotifyStepFinished()
{
    mp_SchedulerThreadController->StopTimer();
    emit sigStepFinished();
}

void CSchedulerStateMachine::NotifyProgramFinished()
{
    mp_SchedulerThreadController->StopTimer();
    emit sigProgramFinished();
}

void CSchedulerStateMachine::NotifyEnterCleaningDryStep()
{
    mp_SchedulerThreadController->StopTimer();
    emit sigEnterDryStep();
}

void CSchedulerStateMachine::NotifyStepProgramFinished()
{
    mp_SchedulerThreadController->StopTimer();
    emit sigStepProgramFinished();
}

void CSchedulerStateMachine::NotifyError()
{
    emit sigError();
}


void CSchedulerStateMachine::NotifyPause(SchedulerStateMachine_t PreviousState)
{
    Q_UNUSED(PreviousState);
    mp_SchedulerThreadController->StopTimer();
    emit sigPause();
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
    mp_SchedulerThreadController->StopTimer();
    mp_RsStandby->Start();
    emit SigEnterRsStandBy();
}

void CSchedulerStateMachine::EnterRsHeatingErr30SRetry()
{
    mp_SchedulerThreadController->StopTimer();
    mp_RsHeatingErr30SRetry->Start();
    emit SigEnterRsHeatingErr30SRetry();
}

void CSchedulerStateMachine::EnterRsPressureOverRange3SRetry()
{
    mp_SchedulerThreadController->StopTimer();
    mp_RsPressureOverRange3SRetry->Start();
    emit SigEnterRsPressureOverRange3SRetry();
}

void CSchedulerStateMachine::EnterRsTSensorErr3MinRetry()
{
    mp_SchedulerThreadController->StopTimer();
    mp_RsTSensorErr3MinRetry->Start();
    emit SigEnterRSTSensorErr3MinRetry();
}

void CSchedulerStateMachine::EnterRsStandByWithTissue()
{
    mp_SchedulerThreadController->StopTimer();
    mp_RsStandbyWithTissue->Start();
    emit SigEnterRsStandByWithTissue();
}

void CSchedulerStateMachine::EnterRcPressure()
{
    mp_SchedulerThreadController->StopTimer();
    emit SigRcPressure();
}

void CSchedulerStateMachine::EnterRcVacuum()
{
    mp_SchedulerThreadController->StopTimer();
    emit SigRcVacuum();
}

void CSchedulerStateMachine::EnterRcFilling()
{
    mp_SchedulerThreadController->StopTimer();
    emit SigRcFilling();
}

void CSchedulerStateMachine::EnterRcDraining()
{
    mp_SchedulerThreadController->StopTimer();
    emit SigRcDraining();
}

void CSchedulerStateMachine::EnterRcForceDraining()
{
    mp_SchedulerThreadController->StopTimer();
    emit SigRcForceDraining();
}

void CSchedulerStateMachine::EnterRsDrainAtOnce()
{
    mp_SchedulerThreadController->StopTimer();
    emit SigRsDrainAtOnce();
}

void CSchedulerStateMachine::EnterRcBottleCheckI()
{
    mp_SchedulerThreadController->StopTimer();
    emit SigRcBottleCheckI();
}

void CSchedulerStateMachine::EnterRsFillingAfterFlush()
{
    mp_SchedulerThreadController->StopTimer();
    mp_RsFillingAfterFlush->Start();
    emit SigRsFillingAfterFlush();
}

void CSchedulerStateMachine::EnterRsCheckBlockage()
{
    mp_SchedulerThreadController->StopTimer();
    emit SigRsCheckBlockage();
}

void CSchedulerStateMachine::EnterRsPause()
{
    mp_SchedulerThreadController->StopTimer();
    emit SigRsPause();
}

void CSchedulerStateMachine::EnterRsRVWaitingTempUp()
{
    mp_SchedulerThreadController->StopTimer();
    emit SigRsRVWaitingTempUp();
}

void CSchedulerStateMachine::EnterRsTissueProtect()
{
    mp_SchedulerThreadController->StopTimer();
    mp_RsTissueProtect->Start();
    emit SigEnterRsTissueProtect();
}

void CSchedulerStateMachine::EnterRcCheckRTLock()
{
    mp_SchedulerThreadController->StopTimer();
    emit SigEnterRcCheckRTLock();
}

void CSchedulerStateMachine::EnterRcReHeating(quint32 Scenario, const QString& StationID, bool NeedResume, bool Is5MinTimeOut)
{
    mp_SchedulerThreadController->StopTimer();
    if (NeedResume)
    {
        mp_RcReHeating->SetNeedResume(true);
    }
    if (Is5MinTimeOut)
    {
        mp_RcReHeating->Set5MinTimeOut(true);
    }
    mp_RcReHeating->SetScenario(Scenario);
    mp_RcReHeating->SetStationID(StationID);
    emit SigRcReHeating();
}

void CSchedulerStateMachine::EnterRsReagentCheck()
{
    mp_SchedulerThreadController->StopTimer();
    emit SigRsReagentCheck();
}

void CSchedulerStateMachine::EnterRsRVMoveToSealPosition()
{
    mp_SchedulerThreadController->StopTimer();
    emit SigRsRvMoveToSealPositon();
}

void CSchedulerStateMachine::EnterRsAbort()
{
    mp_SchedulerThreadController->StopTimer();
    emit sigAbort();
}

void CSchedulerStateMachine::HandlePssmPreTestWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
    mp_ProgramPreTest->HandleWorkFlow(cmdName, retCode);
}

void CSchedulerStateMachine::HandleSelfTestWorkFlow(const QString &cmdName, ReturnCode_t retCode)
{
    mp_ProgramSelfTest->HandleWorkFlow(cmdName, retCode);
}

void CSchedulerStateMachine::HandleRsStandByWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
    mp_RsStandby->HandleWorkFlow(cmdName, retCode);
}

void CSchedulerStateMachine::HandleRsHeatingErr30SRetry(const QString& cmdName, ReturnCode_t retCode)
{
    mp_RsHeatingErr30SRetry->HandleWorkFlow(cmdName, retCode);
}

void CSchedulerStateMachine::HandleRsPressureOverRange3SRetry(const QString& cmdName, ReturnCode_t retCode)
{
    mp_RsPressureOverRange3SRetry->HandleWorkFlow(cmdName,retCode);
}

void CSchedulerStateMachine::HandleRsTSensorErr3MinRetry(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    mp_RsTSensorErr3MinRetry->HandleWorkFlow(cmdName, retCode);
}

void CSchedulerStateMachine::HandleRsStandByWithTissueWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
    mp_RsStandbyWithTissue->HandleWorkFlow(cmdName, retCode);
}

void CSchedulerStateMachine::HandleRsTissueProtectWorkFlow(const QString& cmdName, ReturnCode_t retCode, ControlCommandType_t ctrlCmd)
{
    mp_RsTissueProtect->HandleWorkFlow(cmdName, retCode, ctrlCmd);
}

void CSchedulerStateMachine::EnterRcLevelsensorHeatingOvertime()
{
    mp_SchedulerThreadController->StopTimer();
    emit SigEnterRcLevelsensorHeatingOvertime();
}

void CSchedulerStateMachine::HandleRcCheckRTLockWorkFlow()
{
    if (mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().RetortLockStatus == 1)
    {
        this->OnTasksDone(false);
    }
    else
    {
        this->OnTasksDone(true);
    }
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
            m_RestartLevelSensor = STOP_LEVELSENSOR; // Move to next step
        }
        break;
    case STOP_LEVELSENSOR:
        if (DCL_ERR_FCT_CALL_SUCCESS != mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("LevelSensor"))
        {
            this->OnTasksDone(false);
        }
        else
        {
            m_RestartLevelSensor = WAIT2MIN;
        }
        break;
    case WAIT2MIN:
        if ((QDateTime::currentMSecsSinceEpoch() - m_LevelSensorWaitTime) >= 15*1000)
        {
            m_RestartLevelSensor = RESTART_LEVELSENSOR;
            this->OnTasksDone(true);
        }
        else
        {
            // Do nothing, just wait for time out
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
    if (0 == m_RcPressureSeq)
    {
        if( "Scheduler::ALPressure" == cmdName)
        {
            if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
            {
                OnTasksDone(false);
            }
            else
            {
                m_RcPressureDelayTime = QDateTime::currentMSecsSinceEpoch();
                m_RcPressureSeq++;
            }
        }
    }
    else if (1 == m_RcPressureSeq)
    {
        if ((QDateTime::currentMSecsSinceEpoch()- m_RcPressureDelayTime) <= 30 * 1000)
        {
            qreal currentPressure = mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().PressureAL;
            if (qAbs(currentPressure-30.0) < 5.0)
            {
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, mp_SchedulerThreadController));
                m_RcPressureSeq++;
            }
        }
        else
        {
            m_RcPressureSeq = 0;
            OnTasksDone(false);
        }
    }
    else
    {
        if ("Scheduler::ALReleasePressure" == cmdName)
        {
            if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
               m_RcPressureSeq = 0;
               OnTasksDone(true);
            }
            else
            {
                m_RcPressureSeq = 0;
                OnTasksDone(false);
            }
        }
        else
        {
            // Do nothing, just wait
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
            m_RcFilling = HEATING_LEVELSENSOR;
            OnTasksDone(false);
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
                m_RcFilling = HEATING_LEVELSENSOR;
                OnTasksDone(false);
            }
            else
            {
                mp_SchedulerThreadController->MoveRV(SEAL_POS);
                m_RcFilling = CHECK_SEALING_POS;
            }
        }
    case CHECK_SEALING_POS:
        if (true == mp_SchedulerThreadController->IsRVRightPosition(SEAL_POS))
        {
            mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, mp_SchedulerThreadController));
            m_RcFilling = RELEASE_PRESSURE;
        }
        break;
    case RELEASE_PRESSURE:
        if ("Scheduler::ALReleasePressure" == cmdName)
        {
            if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                m_RcFilling = HEATING_LEVELSENSOR;
                mp_SchedulerThreadController->SetCurrentStepState(PSSM_PROCESSING);
                OnTasksDone(true);
            }
            else
            {
                m_RcFilling = HEATING_LEVELSENSOR;
                OnTasksDone(false);
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

void CSchedulerStateMachine::HandleForceDrainingWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    if("Scheduler::IDForceDraining" == cmdName)
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
            mp_SchedulerThreadController->LogDebug("RC Bottle Check I failed");
            OnTasksDone(false);
        }
        else
        {
            mp_SchedulerThreadController->LogDebug("RC Bottle Check I sucess");
            OnTasksDone(true);
        }
    }
}

void CSchedulerStateMachine::HandleRsFillingAfterFlushWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
    mp_RsFillingAfterFlush->HandleWorkFlow(cmdName, retCode);
}

void CSchedulerStateMachine::HandleRsCheckBlockageWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
    qint64 nowTime = 0;
    qreal currentPressure = 0.0;

    switch(m_RsCheckBlockage)
    {
    case BUILD_HIGHPRESSURE:
        if("Scheduler::ALPressure" == cmdName)
        {
            if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
            {
                m_RsCheckBlockage = BUILD_HIGHPRESSURE;
                OnTasksDone(false);
            }
            else
            {
                m_RsCheckBlockageStartTime = QDateTime::currentMSecsSinceEpoch();
                m_RsCheckBlockage = WAIT_30S;
            }
        }
        break;
    case WAIT_30S:
        nowTime = QDateTime::currentMSecsSinceEpoch();
        if(nowTime - m_RsCheckBlockageStartTime >= 30 * 1000)
        {
            m_RsCheckBlockage = CHECK_PRESSURE;
        }
        break;
    case CHECK_PRESSURE:
        currentPressure = mp_SchedulerThreadController->GetSchedCommandProcessor()->ALGetRecentPressure();
        if(currentPressure < 2.66)
        {
            m_RsCheckBlockage = BUILD_HIGHPRESSURE;
            OnTasksDone(true);
        }
        else
        {
            m_RsCheckBlockage = BUILD_HIGHPRESSURE;
            OnTasksDone(false);
        }
        break;
    default:
        break;
    }
}

void CSchedulerStateMachine::HandleRsPauseWorkFlow(ControlCommandType_t ctrlCmd)
{
    qint32 scenario = 0;
    qint64 nowTime = 0;

    if(0 == m_RsPauseCount)
    {
        m_RsPauseStartTime = QDateTime::currentMSecsSinceEpoch();
        m_RsPauseCount++;
    }

    // We will always wait there until user clicks the OK button
    if (CTRL_CMD_OVEN_COVER_OPEN != ctrlCmd)
    {
        return;
    }

    scenario = mp_SchedulerThreadController->GetCurrentScenario();
    if(273 == scenario)//move seal position
    {
        if( !mp_SchedulerThreadController->IsRVRightPosition(SEAL_POS) )
        {
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_RsPauseStartTime >= 5 * 1000)
            {
                m_RsPauseCount = 0;
                OnTasksDone(false);
            }
        }
        else
        {
            if (mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().OvenLidStatus == 0)
            {
                OnTasksDone(true);
            }
            else
            {
                OnTasksDone(false);
            }
            m_RsPauseCount =0;
        }
    }
    else if(275 == scenario)//move tube position
    {
        if( !mp_SchedulerThreadController->IsRVRightPosition(TUBE_POS) )
        {
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_RsPauseStartTime >= 5 * 1000)
            {
                OnTasksDone(false);
                m_RsPauseCount =0;
            }
        }
        else
        {
            if (mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().OvenLidStatus == 0)
            {
                OnTasksDone(true);
            }
            else
            {
                OnTasksDone(false);
            }
            m_RsPauseCount =0;
        }
    }
    else if(277 == scenario)//move next tube position
    {
        if( !mp_SchedulerThreadController->IsRVRightPosition(NEXT_TUBE_POS) )
        {
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_RsPauseStartTime >= 5 * 1000)
            {
                OnTasksDone(false);
                m_RsPauseCount =0;
            }
        }
        else
        {
            if (mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().OvenLidStatus == 0)
            {
                OnTasksDone(true);
            }
            else
            {
                OnTasksDone(false);
            }
            m_RsPauseCount =0;
        }
    }
    else
    {
        if (mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().OvenLidStatus == 0)
        {
            OnTasksDone(true);
        }
        else
        {
            OnTasksDone(false);
        }
        m_RsPauseCount =0;
    }
}

void CSchedulerStateMachine::HandleRsRVWaitingTempUpWorkFlow(const QString& cmdName, ReturnCode_t retCode)
{
    Q_UNUSED(cmdName)
    Q_UNUSED(retCode)
    qint64 nowTime = 0;
    switch(m_RsRVWaitingTempUp)
    {
    case STOP_HEATING:
        if(DCL_ERR_FCT_CALL_SUCCESS == mp_SchedulerThreadController->GetHeatingStrategy()->StopTemperatureControl("RV"))
        {
            m_RsRVWaitingTempUp = START_HEATING;
        }
        else
        {
            OnTasksDone(false);
        }
        break;
    case START_HEATING:
        if(DCL_ERR_FCT_CALL_SUCCESS == mp_SchedulerThreadController->GetHeatingStrategy()->StartTemperatureControl("RV"))
        {
            m_RsRVWaitingTempUp = CHECK_RVTEMP;
            m_RsRVWaitingTempUpTime = QDateTime::currentMSecsSinceEpoch();
        }
        else
        {
            m_RsRVWaitingTempUp = STOP_HEATING;
            OnTasksDone(false);
        }
        break;
    case CHECK_RVTEMP:
         if(mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().TempRV2 < 40)
         {
            nowTime = QDateTime::currentMSecsSinceEpoch();
            if(nowTime - m_RsRVWaitingTempUpTime > 10 * 1000)
            {
                OnTasksDone(false);
                m_RsRVWaitingTempUp = STOP_HEATING;
            }
         }
         else
         {
            OnTasksDone(true);
            m_RsRVWaitingTempUp = STOP_HEATING;
         }
        break;
    default:
        break;
    }
}
void CSchedulerStateMachine::HandleRcRestart(const QString& cmdName)
{
    SchedulerStateMachine_t currentStepState = mp_SchedulerThreadController->GetCurrentStepState();
    if (currentStepState == PSSM_FILLING)
    {
        switch (m_ErrorRcRestartSeq)
        {
        case 0:
            mp_SchedulerThreadController->HighPressure();
            m_ErrorRcRestartSeq++;
            break;
        case 1:
            if("Scheduler::ALPressure" == cmdName)
            {
                m_ErrorRcRestartSeq++;
                m_TimeReEnterFilling = QDateTime::currentMSecsSinceEpoch();
            }
            else
            {
                // Do nothing, just wait
            }
            break;
        case 2:
            if ((QDateTime::currentMSecsSinceEpoch() - m_TimeReEnterFilling) > 20*1000)
            {
                m_ErrorRcRestartSeq = 0;
                m_TimeReEnterFilling = 0;

                mp_SchedulerThreadController->StopTimer();
                emit SigBackToBusy();
            }
            else
            {
                // Do nothing, just wait for timeout
            }
            break;
        default:
            break;
        }
    }
    else if (currentStepState == PSSM_DRAINING)
    {
        switch (m_ErrorRcRestartSeq)
        {
        case 0:
        {
            mp_SchedulerThreadController->OnBeginDrain();
            CmdIDForceDraining* cmd  = new CmdIDForceDraining(500, mp_SchedulerThreadController);
            QString stationID = mp_SchedulerThreadController->GetCurrentStationID();
            cmd->SetRVPosition(mp_SchedulerThreadController->GetRVTubePositionByStationID(stationID));
            cmd->SetDrainPressure(40.0);
            mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
            m_ErrorRcRestartSeq++;
        }
            break;
        case 1:
            if ("Scheduler::IDForceDraining" == cmdName)
            {
                mp_SchedulerThreadController->OnStopDrain();
                m_ErrorRcRestartSeq = 0;
                mp_SchedulerThreadController->StopTimer();
                emit SigBackToBusy();
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
    else
    {
        mp_SchedulerThreadController->StopTimer();
        emit SigBackToBusy();
    }
}

void CSchedulerStateMachine::HandleRcReHeatingWorkFlow(const QString& cmdName,  DeviceControl::ReturnCode_t retCode)
{
    mp_RcReHeating->HandleWorkFlow(cmdName, retCode);
}

void CSchedulerStateMachine::HandleRsMoveToPSeal(const QString& cmdName,  DeviceControl::ReturnCode_t retCode)
{
    static qint32 startReq = 0;
    switch(m_RsMoveToPSeal)
    {
    case BUILD_VACUUM:
        if(0 == startReq)
        {
            CmdALVaccum* cmd = new CmdALVaccum(500, mp_SchedulerThreadController);
            cmd->SetTargetPressure(-1.0);
            mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
            startReq++;
        }
        else if("Scheduler::ALVaccum" == cmdName)
        {
            if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                m_RsMoveToPSeal = MOVE_INITIALIZE_POSITION;
            }
            else
            {
                m_RsMoveToPSeal = BUILD_VACUUM;
                OnTasksDone(false);
            }
            startReq = 0;
        }
        break;
    case MOVE_INITIALIZE_POSITION:
        if(0 == startReq)
        {
            CmdRVReqMoveToInitialPosition *cmd = new CmdRVReqMoveToInitialPosition(500, mp_SchedulerThreadController);
            mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
            startReq++;
        }
        else if("Scheduler::RVReqMoveToInitialPosition" == cmdName)
        {
            if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                m_RsMoveToPSeal = MOVE_SEALPOSITION;
            }
            else
            {
                m_RsMoveToPSeal = BUILD_VACUUM;
                OnTasksDone(false);
            }
            startReq = 0;
        }
        break;
    case MOVE_SEALPOSITION:
        if(0 == startReq)
        {
            CmdRVReqMoveToRVPosition* CmdMvRV = new CmdRVReqMoveToRVPosition(500, mp_SchedulerThreadController);
            CmdMvRV->SetRVPosition(DeviceControl::RV_SEAL_16);
            mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(CmdMvRV);
            startReq++;
        }
        else if(DeviceControl::RV_SEAL_16 == mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().PositionRV)
        {
            startReq = 0;
            m_RsMoveToPSeal = REALSE_PRESSRE;
        }
        else
        {
            if("Scheduler::RVReqMoveToRVPosition" == cmdName)
            {
                if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
                {
                    m_RsMoveToPSeal = BUILD_VACUUM;
                    OnTasksDone(false);
                }
            }
        }
        break;
    case REALSE_PRESSRE:
        if(0 == startReq)
        {
            mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, mp_SchedulerThreadController));
            startReq++;
        }
        else if("Scheduler::ALReleasePressure" == cmdName)
        {
            if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                OnTasksDone(true);
            }
            else
            {
                OnTasksDone(false);
            }
            startReq = 0;
            m_RsMoveToPSeal = BUILD_VACUUM;
        }
        break;
    default:
        break;
    }
}

void CSchedulerStateMachine::HandlePssmBottleCheckWorkFlow(const QString& cmdName,  DeviceControl::ReturnCode_t retCode)
{
    switch (m_BottleCheckSeq)
    {
    case 0:
        if (0 == m_PressureCalibrationSeq)
        {
            mp_SchedulerThreadController->LogDebug("Begin pressure calibration for Bottle check");
            mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, mp_SchedulerThreadController));
            m_PressureCalibrationSeq++;
        }
        else if (1 == m_PressureCalibrationSeq)
        {
            if ("Scheduler::ALReleasePressure" == cmdName)
            {
                m_PressureCalibrationSeq++;
                m_ReleasePressureTime = QDateTime::currentMSecsSinceEpoch();
            }
        }
        else if (2 == m_PressureCalibrationSeq)
        {
            if ((QDateTime::currentMSecsSinceEpoch() - m_ReleasePressureTime) >= 20*1000)
            {
                m_PressureCalibrationSeq++;
            }
        }
        else if (3 == m_PressureCalibrationSeq)
        {
            // Firstly, check if calibration has been 3 times
            if (3 == m_PressureCalibrationCounter)
            {
                mp_SchedulerThreadController->LogDebug("Pressure calibration for Bottle check failed");
                mp_SchedulerThreadController->RaiseError(0, DCL_ERR_DEV_INTER_INTER_BOTTLECHECK_FAILED_WARNING, 007, true);
                mp_SchedulerThreadController->SendBottleCheckReply("", DataManager::BOTTLECHECK_ALLCOMPLETE);
                this->SendRunComplete();
            }

            m_PressureCalibrationCounter++;
            qreal currentPressure = mp_SchedulerThreadController->GetSchedCommandProcessor()->ALGetRecentPressure();

            if (qAbs(currentPressure) < 0.2) // Calibration is Not needed
            {
                mp_SchedulerThreadController->LogDebug("Pressure calibration for Bottle check completes");
                m_BottleCheckSeq++;
            }
            else if (qAbs(currentPressure) <= 3.0) //offset the calibration
            {
                m_PressureDriftOffset = m_PressureDriftOffset + currentPressure;
                mp_SchedulerThreadController->GetSchedCommandProcessor()->ALSetPressureDrift(m_PressureDriftOffset);
                mp_SchedulerThreadController->SetLastPressureOffset(m_PressureDriftOffset);
                m_PressureCalibrationSeq = 0;
            }
        }
        break;
    case 1:
        if (m_BottleCheckStationIter != mp_SchedulerThreadController->GetDashboardStationList().end())
        {
            RVPosition_t tubePos = mp_SchedulerThreadController->GetRVTubePositionByStationID(m_BottleCheckStationIter->first);
            if (tubePos != RV_UNDEF)
            {
                mp_SchedulerThreadController->SendBottleCheckReply(m_BottleCheckStationIter->first, DataManager::BOTTLECHECK_CHECKING);
                QString reagentGroupId = m_BottleCheckStationIter->second;
                mp_SchedulerThreadController->LogDebug(QString("Bottle check for tube %1").arg(m_BottleCheckStationIter->first));
                if (!reagentGroupId.isEmpty())
                {
                    CmdIDBottleCheck* cmd  = new CmdIDBottleCheck(500, mp_SchedulerThreadController);
                    cmd->SetReagentGrpID(reagentGroupId);
                    cmd->SetTubePos(tubePos);
                    mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);
                    m_BottleCheckSeq++;
                }
                else
                {
                    mp_SchedulerThreadController->SendBottleCheckReply(m_BottleCheckStationIter->first, DataManager::BOTTLECHECK_EMPTY);
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_BOTTLE_CHECK_EMPTY, QStringList() << m_BottleCheckStationIter->first);
                    m_BottleCheckStationIter++;
                }
            }
        }
        else
        {
            mp_SchedulerThreadController->SendBottleCheckReply("", DataManager::BOTTLECHECK_ALLCOMPLETE);
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_BOTTLE_CHECK_SUCCESS);
            this->SendRunComplete();
        }
        break;
    case 2:
        if ("Scheduler::IDBottleCheck" == cmdName)
        {
            if (m_NonRVErrorOccured)
            {
                mp_SchedulerThreadController->SendBottleCheckReply(m_BottleCheckStationIter->first, DataManager::BOTTLECHECK_FAILED);
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_BOTTLE_CHECK_FAILED, QStringList() << m_BottleCheckStationIter->first);
                this->SendRunComplete();
                return;
            }
            m_BottleCheckSeq = 1;
            if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                mp_SchedulerThreadController->SendBottleCheckReply(m_BottleCheckStationIter->first, DataManager::BOTTLECHECK_PASSED);
                m_BottleCheckStationIter++;
            }
            else if (DCL_ERR_DEV_LA_BOTTLECHECK_PRESSUREBUILD_FAILED == retCode)
            {
                mp_SchedulerThreadController->SendBottleCheckReply(m_BottleCheckStationIter->first, DataManager::BOTTLECHECK_BUILDPRESSUREFAILED);
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_BOTTLE_CHECK_BUILD_PRESSURE_FAILED, QStringList() << m_BottleCheckStationIter->first);
                m_BottleCheckStationIter++;
            }
            else if(DCL_ERR_DEV_LA_BOTTLECHECK_FAILED_EMPTY == retCode)
            {
                mp_SchedulerThreadController->SendBottleCheckReply(m_BottleCheckStationIter->first, DataManager::BOTTLECHECK_EMPTY);
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_BOTTLE_CHECK_EMPTY, QStringList() << m_BottleCheckStationIter->first);

                m_BottleCheckStationIter++;
            }
            else if (DCL_ERR_DEV_LA_BOTTLECHECK_FAILED_BLOCKAGE == retCode)
            {
                mp_SchedulerThreadController->SendBottleCheckReply(m_BottleCheckStationIter->first, DataManager::BOTTLECHECK_BLOCKAGE);
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_BOTTLE_CHECK_BLOCKAGE, QStringList() << m_BottleCheckStationIter->first);
                m_BottleCheckStationIter++;
            }
            else
            {
                mp_SchedulerThreadController->SendBottleCheckReply(m_BottleCheckStationIter->first, DataManager::BOTTLECHECK_FAILED);
                mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_INTER_INTER_BOTTLECHECK_RV_FAILED);
            }

        }
        break;
    default:
        break;
    }
}

void CSchedulerStateMachine::HandleRsAbortWorkFlow(const QString& cmdName,  DeviceControl::ReturnCode_t retCode)
{
        SchedulerStateMachine_t stateAtAbort = mp_SchedulerThreadController->GetCurrentStepState();
        switch (stateAtAbort)
        {
        case PSSM_PRETEST:
            HandlePssmPreTestWorkFlow(cmdName, retCode);
            if (0 == m_PssmAbortingSeq)
            {
                mp_ProgramPreTest->RecvAbort();
                m_PssmAbortingSeq++;
            }
            else
            {
                if (mp_ProgramPreTest->TasksAborted())
                {
                    m_PssmAbortingSeq = 0;
                    mp_SchedulerThreadController->CompleteRsAbort();
                }

            }
            break;
        case PSSM_FILLING:
        case PSSM_DRAINING:
        case PSSM_PROCESSING:
        case PSSM_PAUSE:
            if (0 == m_PssmAbortingSeq)
            {
                mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, mp_SchedulerThreadController));
                m_PssmAbortingSeq++;
            }
            else if (1 == m_PssmAbortingSeq)
            {
                if ("Scheduler::ALReleasePressure" == cmdName)
                {
                    if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
                    {
                        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_DRAINING);
                        CmdIDForceDraining* cmd  = new CmdIDForceDraining(500, mp_SchedulerThreadController);
                        QString stationID = mp_SchedulerThreadController->GetCurrentStationID();
                        RVPosition_t tubePos = mp_SchedulerThreadController->GetRVTubePositionByStationID(stationID);
                        cmd->SetRVPosition((quint32)(tubePos));
                        if (m_EnableLowerPressure)
                        {
                            cmd->SetDrainPressure(20.0);
                        }
                        else
                        {
                            cmd->SetDrainPressure(30.0);
                        }
                        mp_SchedulerThreadController->GetSchedCommandProcessor()->pushCmd(cmd);

                        //Notify GUI
                        emit sigOnForceDrain();
                        m_PssmAbortingSeq++;
                    }
                    else
                    {
                        OnTasksDone(false);
                    }
                }
            }
            else if (2 == m_PssmAbortingSeq)
            {
                if ("Scheduler::IDForceDraining" == cmdName)
                {
                    //Notify GUI
                    emit sigOnStopForceDrain();

                    if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
                    {
                        m_PssmAbortingSeq = 0;
                        mp_SchedulerThreadController->CompleteRsAbort();
                    }
                    else
                    {
                        OnTasksDone(false);
                    }
                }
            }
            break;
        case PSSM_RV_MOVE_TO_SEAL:
            if (mp_SchedulerThreadController->IsRVRightPosition(SEAL_POS))
            {
                mp_SchedulerThreadController->SetCurrentStepState(PSSM_FILLING);
            }
            else
            {
                // do nothing
            }
            break;
        case PSSM_RV_MOVE_TO_TUBE:
            if (0 == m_PssmAbortingSeq)
            {
                this->HandlePssmMoveTubeWorkflow(cmdName, retCode, true);
                if (m_PssmAbortingInMoveToTube)
                {
                    m_PssmAbortingSeq++;
                }
            }
            else
            {
                m_PssmAbortingSeq = 0;
                mp_SchedulerThreadController->SetCurrentStepState(PSSM_FILLING);
            }
        break;
        case PSSM_RV_POS_CHANGE:
            if (mp_SchedulerThreadController->IsRVRightPosition(NEXT_TUBE_POS))
            {
                mp_SchedulerThreadController->CompleteRsAbort();
            }
            else
            {
                // do nothing
            }
            break;
        case PSSM_FILLING_RVROD_HEATING:
        case PSSM_CLEANING_DRY_STEP:
        case PSSM_STEP_PROGRAM_FINISH:
        case PSSM_PROGRAM_FINISH:
        case PSSM_INIT:
             mp_SchedulerThreadController->CompleteRsAbort();
             m_PssmAbortingSeq = 0;
            break;
        default:
            mp_SchedulerThreadController->CompleteRsAbort();
            m_PssmAbortingSeq = 0;
            break;
        }
}

void CSchedulerStateMachine::HandlePssmMoveTubeWorkflow(const QString& cmdName, DeviceControl::ReturnCode_t retCode, bool isAbortState)
{
    if (0 == m_PssmMVTubeSeq)
    {
        if ("Scheduler::ALPressure" == cmdName)
        {
            if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
            {
                if (isAbortState)
                {
                    m_PssmAbortingInMoveToTube = true;
                    return;
                }
                else
                {
                    mp_SchedulerThreadController->SendOutErrMsg(retCode);
                }
            }
            else
            {
                m_PssmMVTubeSeq++;
            }
        }
        else
        {
            // Do nothing, just wait
        }
    }
    else if (1 == m_PssmMVTubeSeq)
    {
        if ((QDateTime::currentMSecsSinceEpoch() - m_PssmMVTubePressureTime) > 30*1000)
        {
            m_PssmMVTubeSeq = 0;
            if (isAbortState)
            {
                m_PssmAbortingInMoveToTube = true;
                return;
            }
            else
            {
                mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_LA_PRESSURE_TEST);
            }
        }
        qreal threshPressure = 0.0;
        if (m_EnableLowerPressure)
        {
            threshPressure = 15.0;
        }
        else
        {
            threshPressure = 25.0;
        }
        if (mp_SchedulerThreadController->GetSchedCommandProcessor()->HardwareMonitor().PressureAL >= threshPressure)
        {
            mp_SchedulerThreadController->ReleasePressure();
            m_PssmMVTubePressureTime = QDateTime::currentMSecsSinceEpoch();
            m_PssmMVTubeSeq++;
        }
    }
    else if (2 == m_PssmMVTubeSeq)
    {
        if ("Scheduler::ALReleasePressure" == cmdName)
        {
            if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
            {
                m_PssmMVTubeSeq = 0;
                if (isAbortState)
                {
                    m_PssmAbortingInMoveToTube = true;
                    return;
                }
                else
                {

                    mp_SchedulerThreadController->SendOutErrMsg(retCode);
                }
            }
            else
            {
                 m_PssmMVTubeSeq++;
            }
        }
        else
        {
            // Do nothing, just wait
        }
    }
    else if (3 == m_PssmMVTubeSeq)
    {
        if (isAbortState)
        {
            m_PssmAbortingInMoveToTube = true;
            return;
        }

        mp_SchedulerThreadController->MoveRV(TUBE_POS);
        m_PssmMVTubeSeq++;
    }
    else if (4 == m_PssmMVTubeSeq)
    {
        if (isAbortState)
        {
            m_PssmAbortingInMoveToTube = true;
            return;
        }
        if(mp_SchedulerThreadController->IsRVRightPosition(TUBE_POS))
        {
            //startTime
            mp_SchedulerThreadController->CalculateTheGapTimeAndBufferTime(true, false);
            m_PssmMVTubeSeq = 0;
            this->NotifyRVMoveToTubeReady();
        }
        else
        {
            if(("Scheduler::RVReqMoveToRVPosition" == cmdName))
            {
                if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
                {
                    m_PssmMVTubeSeq = 0;
                    mp_SchedulerThreadController->SendOutErrMsg(retCode);
                }
            }

        }
    }
}

void CSchedulerStateMachine::CheckNonRVErr4BottleCheck(DeviceControl::ReturnCode_t retCode)
{
    if (DCL_ERR_DEV_INTER_INTER_BOTTLECHECK_RV_FAILED == retCode)
    {
        return;
    }

    mp_SchedulerThreadController->RaiseError(0, DCL_ERR_DEV_INTER_INTER_BOTTLECHECK_FAILED_WARNING, 7, true);

    if( DCL_ERR_DEV_LIDLOCK_CLOSE_STATUS_ERROR == retCode ||
            DCL_ERR_DEV_WAXBATH_OVENCOVER_STATUS_OPEN == retCode ||
            DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_OVERRANGE == retCode ||
            DCL_ERR_DEV_WAXBATH_TSENSORUP_OUTOFRANGE == retCode ||
            DCL_ERR_DEV_WAXBATH_TSENSORDOWN1_OUTOFRANGE == retCode ||
            DCL_ERR_DEV_LA_PRESSURESENSOR_OUTOFRANGE == retCode ||
            DCL_ERR_DEV_ASB5_AC_CURRENT_OUTOFRANGE == retCode ||
            DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_OVERRANGE == retCode ||
            DCL_ERR_DEV_LA_RELEASING_TIMEOUT == retCode)
    {
        m_NonRVErrorOccured = true;
    }
}

void CSchedulerStateMachine::EnterRcRestart()
{
    mp_SchedulerThreadController->StopTimer();
    if(false == mp_ProgramSelfTest->IsSelfTestDone())
    {
        mp_ProgramSelfTest->ResetVarList();
        emit SigSelfRcRestart();
    }
    else if(SM_IDLE == mp_SchedulerThreadController->GetCurrentStepState())
    {
        emit SigIdleRcRestart();
    }
    else
    {
        emit SigEnterRcRestart();
    }
}

void CSchedulerStateMachine::EnterRsRVGetOriginalPositionAgain()
{
    mp_SchedulerThreadController->StopTimer();
    emit SigEnterRsRVGetOriginalPositionAgain();
}
void CSchedulerStateMachine::SendRunPreTest()
{
    mp_SchedulerThreadController->StopTimer();
    mp_ProgramPreTest->ResetVarList(true);
    emit RunPreTest();
}

void CSchedulerStateMachine::SendRunBottleCheck()
{
    mp_SchedulerThreadController->StopTimer();
    emit SigRunBottleCheck();
}

QString CSchedulerStateMachine::GetDeviceName()
{
    ReturnCode_t EventID = mp_SchedulerThreadController->GetCurErrEventID();

    QString DevName = "";
    switch (EventID)
    {
    case DCL_ERR_DEV_ASB5_AC_CURRENT_OUTOFRANGE:
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

void CSchedulerStateMachine::OnNotifyResume()
{
    mp_SchedulerThreadController->StopTimer();
    SchedulerStateMachine_t previousState = this->GetPreviousState();
    if(previousState == PSSM_FILLING_RVROD_HEATING)
    {
        emit sigRVRodHeatingReady();
    }
    else if(previousState == PSSM_FILLING_LEVELSENSOR_HEATING)
    {
        emit sigLevelSensorHeatingReady();
    }
    else if((previousState == PSSM_FILLING) || (previousState == PSSM_RV_MOVE_TO_SEAL))
    {
        emit sigRVMoveToSealReady();
    }
    else if (previousState == PSSM_PROCESSING)
    {
        emit sigResumeToProcessing();
    }
    else if(previousState == PSSM_DRAINING)
    {
        emit sigDrainFinished();
    }
    else if(previousState == PSSM_RV_POS_CHANGE)
    {
        emit sigStepFinished();
    }

}

void CSchedulerStateMachine::OnEnterRsAbortState()
{
    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_OVEN_ABORTING);
    mp_SchedulerThreadController->SetTransitionPeriod(false);
    m_PssmAbortingSeq = 0;
    m_PssmAbortingInMoveToTube = false;
    mp_SchedulerThreadController->StartTimer();
}

void CSchedulerStateMachine::OnEnterInitState()
{
    EventHandler::StateHandler::Instance().setInitState();
}

void CSchedulerStateMachine::OnEnterSelfTest()
{
    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_SELFTEST);
    mp_SchedulerThreadController->GetHeatingStrategy()->InitSelfTest();
    mp_SchedulerThreadController->StartTimer();
}

void CSchedulerStateMachine::OnEnterIdleState()
{
    EventHandler::StateHandler::Instance().setIdleState();
    EventHandler::StateHandler::Instance().setAvailability(false, EVENT_SCHEDULER_IN_ERROR_STATE);
    EventHandler::StateHandler::Instance().setActivityUpdate(false, 0);
    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_IN_IDLE_STATE);
    mp_SchedulerThreadController->OnEnterIdleState();
    mp_SchedulerThreadController->StartTimer();
}

void CSchedulerStateMachine::OnEnterErrorState()
{
    mp_SchedulerThreadController->InitProgramStatus();
    EventHandler::StateHandler::Instance().setAvailability(true, EVENT_SCHEDULER_IN_ERROR_STATE);
    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_IN_ERROR_STATE);
}

void CSchedulerStateMachine::OnPowerFailureState()
{
    EventHandler::StateHandler::Instance().setAvailability(true, EVENT_SCHEDULER_POWER_FAILURE);
    mp_SchedulerThreadController->StartTimer();
}

void CSchedulerStateMachine::OnEnterBusyState()
{
    EventHandler::StateHandler::Instance().setActivityUpdate(true, 0);
}

void CSchedulerStateMachine::OnEnterPretest()
{
     mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_PRETEST);
     mp_SchedulerThreadController->StartTimer();
}
void CSchedulerStateMachine::OnEnterHeatingLevelSensor()
{
    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_HEATING_LEVEL_SENSOR_FOR_FILLING);
    mp_SchedulerThreadController->StartTimer();
}
}
