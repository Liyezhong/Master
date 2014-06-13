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
#include "Scheduler/Commands/Include/CmdRTSetTempCtrlOFF.h"
#include "Scheduler/Commands/Include/CmdALAllStop.h"
#include "Scheduler/Include/RsStandby.h"
#include "Scheduler/Include/RsStandbyWithTissue.h"
#include "Scheduler/Include/RcLevelsensorHeatingOvertime.h"
#include "Scheduler/Include/RcRestart.h"
#include "Scheduler/Include/RcReport.h"
#include "Scheduler/Include/RsHeatingErr30SRetry.h"
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
    ,mp_SchedulerCommandProcessor(NULL)
{
    qRegisterMetaType<DeviceControl::RTTempCtrlType_t>("DeviceControl::RTTempCtrlType_t");
    m_PreviousState = SM_UNDEF;
    m_CurrentState = SM_UNDEF;

    mp_SchedulerMachine = new QStateMachine();
    mp_InitState = new QState(mp_SchedulerMachine);
    mp_IdleState = new QState(mp_SchedulerMachine);
    mp_BusyState = new QState(mp_SchedulerMachine);
    mp_ErrorState = new QState(mp_SchedulerMachine);
    mp_ErrorWaitState = new QState(mp_ErrorState);

    mp_ErrorRsStandbyWithTissueState = new QState(mp_ErrorState);
    mp_ErrorRcLevelSensorHeatingOvertimeState = new QState(mp_ErrorState);
    mp_ErrorRcRestart = new QState(mp_ErrorState);
    mp_ErrorRsHeatingErr30SRetry = new QState(mp_ErrorState);

    mp_SchedulerMachine->setInitialState(mp_InitState);
    mp_ErrorState->setInitialState(mp_ErrorWaitState);

    mp_ProgramStepStates = new CProgramStepStateMachine(mp_BusyState, mp_ErrorState);
    mp_RsRvGetOriginalPositionAgain = new CRsRvGetOriginalPositionAgain(mp_SchedulerMachine, mp_ErrorState );
    mp_RcReport = new CRcReport(mp_SchedulerMachine, mp_ErrorState );
    mp_RsStandby = new CRsStandby(mp_SchedulerMachine, mp_ErrorState);

    mp_RsStandbyWithTissue = new CRsStandbyWithTissue(mp_SchedulerMachine, mp_ErrorRsStandbyWithTissueState);
    mp_RcLevelSensorHeatingOvertime = new CRcLevelSensorHeatingOvertime(mp_SchedulerMachine, mp_ErrorRcLevelSensorHeatingOvertimeState);
    mp_RcRestart = new CRcRestart(mp_SchedulerMachine, mp_ErrorRcRestart);
    mp_RsHeatingErr30SRetry = new CRsHeatingErr30SRetry(mp_SchedulerMachine, mp_ErrorRsHeatingErr30SRetry);

    mp_InitState->addTransition(this, SIGNAL(SchedulerInitComplete()), mp_IdleState);
    mp_IdleState->addTransition(this, SIGNAL(RunSignal()), mp_BusyState);
    mp_BusyState->addTransition(this, SIGNAL(RunComplete()), mp_IdleState);
    mp_BusyState->addTransition(this, SIGNAL(ErrorSignal()), mp_ErrorState);
    mp_InitState->addTransition(this, SIGNAL(ErrorSignal()), mp_ErrorState);
    mp_IdleState->addTransition(this, SIGNAL(ErrorSignal()), mp_ErrorState);

    connect(mp_InitState, SIGNAL(exited()), this, SLOT(OnStateChanged()));
    connect(mp_IdleState, SIGNAL(exited()), this, SLOT(OnStateChanged()));
    connect(mp_BusyState, SIGNAL(exited()), this, SLOT(OnStateChanged()));
    connect(mp_ErrorState, SIGNAL(exited()), this, SLOT(OnStateChanged()));

    //RS_Standby_WithTissue related logic
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRsStandByWithTissue()), mp_ErrorRsStandbyWithTissueState);
    mp_ErrorRsStandbyWithTissueState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState);

    //RC_Levelsensor_Heating_Overtime related logic
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRcLevelsensorHeatingOvertime()), mp_ErrorRcLevelSensorHeatingOvertimeState);
    mp_ErrorRcLevelSensorHeatingOvertimeState->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState);

    //RC_Restart related logic
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRcRestart()), mp_ErrorRcRestart);
    mp_ErrorRcRestart->addTransition(this, SIGNAL(sigStateChange()), mp_BusyState);

    //RS_HeatingErr30SRetry related logic
    mp_ErrorState->addTransition(this, SIGNAL(SigEnterRSHeatingErr30SRetry()), mp_ErrorRsHeatingErr30SRetry);
    mp_ErrorRsHeatingErr30SRetry->addTransition(this, SIGNAL(sigStateChange()), mp_ErrorWaitState);

    connect(this, SIGNAL(RunCleaning()), mp_ProgramStepStates, SIGNAL(TempsReady()));
    connect(this, SIGNAL(RunSelfTest()), mp_ProgramStepStates, SIGNAL(SelfTestSig()));

    connect(this, SIGNAL(sigStInitOK()), mp_ProgramStepStates, SIGNAL(StInitOK()));
    connect(this, SIGNAL(sigStTempOK()), mp_ProgramStepStates, SIGNAL(StTempOK()));
    connect(this, SIGNAL(sigStCurrentOK()), mp_ProgramStepStates, SIGNAL(StCurrentOK()));
    connect(this, SIGNAL(sigStVoltageOK()), mp_ProgramStepStates, SIGNAL(StVoltageOK()));
    connect(this, SIGNAL(sigStRVPositionOK()), mp_ProgramStepStates, SIGNAL(StRVPositionOK()));
    connect(this, SIGNAL(sigStPressureOK()), mp_ProgramStepStates, SIGNAL(StPressureOK()));
    connect(this, SIGNAL(sigStSealingOK()), mp_ProgramStepStates, SIGNAL(StSealingOK()));
    connect(this, SIGNAL(sigStGetStationcheckResult()), mp_ProgramStepStates, SIGNAL(StGetStationcheckResult()));
    connect(this, SIGNAL(sigStStationLeft()), mp_ProgramStepStates, SIGNAL(StStationLeft()));
    connect(this, SIGNAL(sigStStationOK()), mp_ProgramStepStates, SIGNAL(StStationOK()));
    connect(this, SIGNAL(sigStDone()), mp_ProgramStepStates, SIGNAL(StDone()));
    connect(this, SIGNAL(sigTempsReady()), mp_ProgramStepStates, SIGNAL(TempsReady()));
    connect(this, SIGNAL(sigLevelSensorTempS1Ready()), mp_ProgramStepStates, SIGNAL(LevelSensorTempS1Ready()));
    connect(this, SIGNAL(sigLevelSensorTempS2Ready()), mp_ProgramStepStates, SIGNAL(LevelSensorTempS2Ready()));
    connect(this, SIGNAL(sigHitTubeBefore()), mp_ProgramStepStates, SIGNAL(HitTubeBefore()));
    connect(this, SIGNAL(sigFillFinished()), mp_ProgramStepStates, SIGNAL(FillFinished()));
    connect(this, SIGNAL(sigHitSeal()), mp_ProgramStepStates, SIGNAL(HitSeal()));
    connect(this, SIGNAL(sigSoakFinished()), mp_ProgramStepStates, SIGNAL(SoakFinished()));
    connect(this, SIGNAL(sigHitTubeAfter()), mp_ProgramStepStates, SIGNAL(HitTubeAfter()));
    connect(this, SIGNAL(sigDrainFinished()), mp_ProgramStepStates, SIGNAL(DrainFinished()));
    connect(this, SIGNAL(sigStepFinished()), mp_ProgramStepStates, SIGNAL(StepFinished()));
    connect(this, SIGNAL(sigProgramFinished()), mp_ProgramStepStates, SIGNAL(ProgramFinished()));
    connect(this, SIGNAL(sigError()), mp_ProgramStepStates, SIGNAL(Error()));
    connect(this, SIGNAL(sigPause()), mp_ProgramStepStates, SIGNAL(Pause()));
    connect(this, SIGNAL(sigResumeToSelftest()), mp_ProgramStepStates, SIGNAL(ResumeToSelftest()));
    connect(this, SIGNAL(sigResumeToInit()), mp_ProgramStepStates, SIGNAL(ResumeToInit()));
    connect(this, SIGNAL(sigResumeFromErrorToBegin()), mp_ProgramStepStates, SIGNAL(ResumeFromErrorToBegin()));
    connect(this, SIGNAL(sigResumeToHeatLevelSensorS1()), mp_ProgramStepStates, SIGNAL(ResumeToHeatLevelSensorS1()));
    connect(this, SIGNAL(sigResumeToHeatLevelSensorS2()), mp_ProgramStepStates, SIGNAL(ResumeToHeatLevelSensorS2()));
    connect(this, SIGNAL(sigResumeToReadyToFill()), mp_ProgramStepStates, SIGNAL(ResumeToReadyToFill()));
    connect(this, SIGNAL(sigResumeToSoak()), mp_ProgramStepStates, SIGNAL(ResumeToSoak()));
    connect(this, SIGNAL(sigResumeToStepFinished()), mp_ProgramStepStates, SIGNAL(ResumeToStepFinished()));
    connect(this, SIGNAL(sigResumeToReadyToTubeAfter()), mp_ProgramStepStates, SIGNAL(ResumeToReadyToTubeAfter()));
    connect(this, SIGNAL(sigAbort()), mp_ProgramStepStates, SIGNAL(Abort()));

    connect(this, SIGNAL(sigRsRvMoveToInitPosition()), mp_RsRvGetOriginalPositionAgain, SIGNAL(RvMoveToInitPosition()));
    connect(this, SIGNAL(sigRsRvMoveToInitPositionFinished()), mp_RsRvGetOriginalPositionAgain, SIGNAL(RvMoveToInitPositionFinished()));

    connect(this, SIGNAL(sigRcReport()), mp_RcReport, SIGNAL(RcReport()));
    connect(this, SIGNAL(sigRcReport()), mp_RcReport, SIGNAL(RcReport()));

    connect(this, SIGNAL(sigReleasePressure()), mp_RsStandby, SIGNAL(ReleasePressure()));
    connect(this, SIGNAL(sigShutdownFailedHeater()), mp_RsStandby, SIGNAL(ShutdownFailedHeater()));
    connect(this, SIGNAL(sigShutdownFailedHeaterFinished()), mp_RsStandby, SIGNAL(ShutdownFailedHeaterFinished()));

    connect(mp_ProgramStepStates, SIGNAL(OnInit()), this, SIGNAL(sigOnInit()));
    connect(mp_ProgramStepStates, SIGNAL(OnHeatLevelSensorTempS1()), this, SIGNAL(sigOnHeatLevelSensorTempS1()));
    connect(mp_ProgramStepStates, SIGNAL(OnHeatLevelSensorTempS2()), this, SIGNAL(sigOnHeatLevelSensorTempS2()));
    connect(mp_ProgramStepStates, SIGNAL(OnMoveToTubeBefore()), this, SIGNAL(sigOnMoveToTubeBefore()));
    connect(mp_ProgramStepStates, SIGNAL(OnMoveToTubeAfter()), this, SIGNAL(sigOnMoveToTubeAfter()));
    connect(mp_ProgramStepStates, SIGNAL(OnMoveToSeal()), this, SIGNAL(sigOnMoveToSeal()));
    connect(mp_ProgramStepStates, SIGNAL(OnFill()), this, SIGNAL(sigOnFill()));
    connect(mp_ProgramStepStates, SIGNAL(OnStopFill()), this, SIGNAL(sigOnStopFill()));
    connect(mp_ProgramStepStates, SIGNAL(OnSoak()), this, SIGNAL(sigOnSoak()));
    connect(mp_ProgramStepStates, SIGNAL(OnDrain()), this, SIGNAL(sigOnDrain()));
    connect(mp_ProgramStepStates, SIGNAL(OnStopDrain()), this, SIGNAL(sigOnStopDrain()));
    connect(mp_ProgramStepStates, SIGNAL(OnAborting()), this, SIGNAL(sigOnAborting()));
    connect(mp_ProgramStepStates, SIGNAL(OnAborted()), this, SIGNAL(sigOnAborted()));
    connect(mp_ProgramStepStates, SIGNAL(OnPause()), this, SIGNAL(sigOnPause()));
    connect(mp_ProgramStepStates, SIGNAL(OnPauseDrain()), this, SIGNAL(sigOnPauseDrain()));
    connect(mp_ProgramStepStates, SIGNAL(OnSealingCheck()), this, SIGNAL(sigOnSealingCheck()));
    connect(mp_ProgramStepStates, SIGNAL(OnRVPositionCheck()), this, SIGNAL(sigOnRVPositionCheck()));
    connect(mp_ProgramStepStates, SIGNAL(OnStateExited()), this, SLOT(OnStateChanged()));
    connect(mp_RsRvGetOriginalPositionAgain, SIGNAL(OnRvMoveToInitPosition()), this, SIGNAL(sigOnRsRvMoveToInitPosition()));
    connect(mp_RcReport, SIGNAL(OnRcReport()), this, SIGNAL(sigOnRcReport()));

    connect(mp_RsStandby, SIGNAL(OnReleasePressure()), this, SIGNAL(sigOnRsReleasePressure()));
    connect(mp_RsStandby, SIGNAL(OnShutdownFailedHeater()), this, SIGNAL(sigOnRsShutdownFailedHeater()));

    CONNECTSIGNALSLOT(mp_SchedulerThreadController, NotifyResume(), this, OnNotifyResume());

    CONNECTSIGNALSLOT(mp_RsStandbyWithTissue, RTStopTempCtrl(DeviceControl::RTTempCtrlType_t), this, OnRsRTStopTempCtrl(DeviceControl::RTTempCtrlType_t));
    CONNECTSIGNALSLOT(mp_RsStandbyWithTissue, ShutdownFailedHeater(), this, OnRsShutdownFailedHeater());
    CONNECTSIGNALSLOT(mp_RsStandbyWithTissue, ReleasePressure(), this, OnRsReleasePressure());
    CONNECTSIGNALSLOT(mp_RsStandbyWithTissue, TasksDone(bool), this, OnTasksDone(bool));

    CONNECTSIGNALSLOT(mp_RcLevelSensorHeatingOvertime, RestartLevelSensorTempControl(), this, OnRestartLevelSensorTempControl());
    CONNECTSIGNALSLOT(mp_RcLevelSensorHeatingOvertime, TasksDone(bool), this, OnTasksDone(bool));

    CONNECTSIGNALSLOT(mp_RcRestart, Recover(), this, OnNotifyResume());
    CONNECTSIGNALSLOT(mp_RcRestart, TasksDone(bool), this, OnTasksDone(bool));

    CONNECTSIGNALSLOT(mp_RsHeatingErr30SRetry, StopTempCtrl(), this, OnStopDeviceTempCtrl());
    CONNECTSIGNALSLOT(mp_RsHeatingErr30SRetry, StartTempCtrl(), this, OnStartDeviceTempCtrl());
    CONNECTSIGNALSLOT(mp_RsHeatingErr30SRetry, CheckDevStatus(), this, OnCheckDeviceStatus());
    CONNECTSIGNALSLOT(mp_RsHeatingErr30SRetry, TasksDone(bool), this, OnTasksDone(bool));
}

void CSchedulerStateMachine::OnStateChanged()
{
    //m_PreviousState = m_CurrentState;
#if 0
    quint32 stateid = STR_UNEXPECTED_STATE;
    switch(currentState)
    {
    case SM_INIT:
        stateid = STR_INIT;
        break;
    case SM_IDLE:
        stateid = STR_IDLE;
        break;
    case SM_BUSY:
        stateid = STR_RUN;
        break;
    case SM_ERROR:
        stateid = STR_ERROR;
        break;
    default:
        stateid = STR_UNEXPECTED_STATE;
    }
//    LOG_STR_ARG(STR_SCHDEULER_MAIN_CONTROLLER_STATE,Global::tTranslatableStringList()<<Global::TranslatableString(stateid));
#endif
}

void CSchedulerStateMachine::OnRsRTStopTempCtrl(DeviceControl::RTTempCtrlType_t type)
{
    CmdRTSetTempCtrlOFF* cmd = new CmdRTSetTempCtrlOFF(500, mp_SchedulerThreadController);
    cmd->SetType(type);
    mp_SchedulerCommandProcessor->pushCmd(cmd);
}

void CSchedulerStateMachine::OnRsShutdownFailedHeater()
{
    mp_SchedulerThreadController->ShutdownFailedHeater();
}

void CSchedulerStateMachine::OnRsReleasePressure()
{
    mp_SchedulerCommandProcessor->ALBreakAllOperation();
    mp_SchedulerCommandProcessor->pushCmd(new CmdALAllStop(500, mp_SchedulerThreadController));
}
void CSchedulerStateMachine::OnRestartLevelSensorTempControl()
{
    mp_SchedulerThreadController->RestartLevelSensorTempCtrlInError();
}

void CSchedulerStateMachine::OnStopDeviceTempCtrl()
{

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

/****************************************************************************/
/*!
 *  \brief    Destructor
 */
/****************************************************************************/
CSchedulerStateMachine::~CSchedulerStateMachine()
{
    delete mp_SchedulerMachine;
    mp_SchedulerMachine = NULL;

    delete mp_InitState;
    mp_InitState = NULL;

    delete mp_IdleState;
    mp_IdleState = NULL;

    delete mp_BusyState;
    mp_BusyState = NULL;

    delete mp_ErrorState;
    mp_ErrorState = NULL;

    delete mp_ErrorWaitState;
    mp_ErrorWaitState = NULL;

    delete mp_ProgramStepStates;
    mp_ProgramStepStates = NULL;

    delete mp_RsRvGetOriginalPositionAgain;
    mp_RsRvGetOriginalPositionAgain = NULL;

    delete mp_RsStandby;
    mp_RsStandby = NULL;

    delete mp_ErrorRsStandbyWithTissueState;
    mp_ErrorRsStandbyWithTissueState = NULL;

    delete mp_RsStandbyWithTissue;
    mp_RsStandbyWithTissue = NULL;

    delete mp_ErrorRsHeatingErr30SRetry;
    mp_ErrorRsHeatingErr30SRetry = NULL;

    delete mp_RsHeatingErr30SRetry;
    mp_RsHeatingErr30SRetry = NULL;

    delete mp_ErrorRcLevelSensorHeatingOvertimeState;
    mp_ErrorRcLevelSensorHeatingOvertimeState = NULL;

    delete mp_RcLevelSensorHeatingOvertime;
    mp_RcLevelSensorHeatingOvertime = NULL;

    delete mp_RcReport;
    mp_RcReport = NULL;
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

void CSchedulerStateMachine::SendErrorSignal()
{
    emit ErrorSignal();
}

SchedulerStateMachine_t CSchedulerStateMachine::GetCurrentState()
{
    SchedulerStateMachine_t currentState = SM_UNDEF;
    if(mp_SchedulerMachine->configuration().contains(mp_InitState))
    {
        currentState = SM_INIT;
    }else if(mp_SchedulerMachine->configuration().contains(mp_IdleState))
    {
        currentState = SM_IDLE;
    }else if(mp_SchedulerMachine->configuration().contains(mp_ErrorState))
    {
        if(mp_SchedulerMachine->configuration().contains(mp_ErrorWaitState))
        {
            return SM_ERR_WAIT;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRsStandbyWithTissueState))
        {
            return SM_ERR_RS_STANDBY_WITH_TISSUE;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRcLevelSensorHeatingOvertimeState))
        {
            return SM_ERR_RC_LEVELSENSOR_HEATING_OVERTIME;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRcRestart))
        {
            return SM_ERR_RC_RESTART;
        }
        else if((currentState = mp_RsRvGetOriginalPositionAgain->GetCurrentState(mp_SchedulerMachine->configuration())) != SM_UNDEF);
        else if((currentState = mp_RcReport->GetCurrentState(mp_SchedulerMachine->configuration())) != SM_UNDEF);
        else if((currentState = mp_RsStandby->GetCurrentState(mp_SchedulerMachine->configuration())) != SM_UNDEF);

    }else if(mp_SchedulerMachine->configuration().contains(mp_BusyState))
    {
        currentState = mp_ProgramStepStates->GetCurrentState(mp_SchedulerMachine->configuration());
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

void CSchedulerStateMachine::NotifyLevelSensorTempS1Ready()
{
    emit sigLevelSensorTempS1Ready();
}

void CSchedulerStateMachine::NotifyLevelSensorTempS2Ready()
{
    emit sigLevelSensorTempS2Ready();
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

void CSchedulerStateMachine::NotifySoakFinished()
{
    emit sigSoakFinished();
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

void CSchedulerStateMachine::NotifyRsRvMoveToInitPosition()
{
    emit sigRsRvMoveToInitPosition();
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

void CSchedulerStateMachine::OnNotifyResume()
{
    if((this->GetPreviousState() & 0xFFFF) == PSSM_ST)
    {
        emit sigResumeToSelftest();
    }
    else if( this->GetPreviousState() == (PSSM_INIT))
    {
        emit sigResumeToInit();
    }
    else if( this->GetPreviousState()== (PSSM_READY_TO_HEAT_LEVEL_SENSOR_S1))
    {
        emit sigResumeToHeatLevelSensorS1();
    }
    else if( this->GetPreviousState() == (PSSM_READY_TO_HEAT_LEVEL_SENSOR_S2))
    {
        emit sigResumeToHeatLevelSensorS2();
    }
    else if( this->GetPreviousState() == (PSSM_READY_TO_TUBE_BEFORE))
    {
        if(((this->GetCurrentState())&0xFF) == (SM_ERROR))
        {
            emit sigResumeFromErrorToBegin();
        }
        else
        {
            emit sigResumeToReadyToFill();
        }
    }
    else if( this->GetPreviousState() == (PSSM_READY_TO_FILL))
    {
        emit sigResumeToReadyToFill();
    }
    else if( this->GetPreviousState() == (PSSM_READY_TO_SEAL))
    {

        if(((this->GetCurrentState())&0xFF) == (SM_ERROR))
        {
            emit sigResumeFromErrorToBegin();
        }
        else
        {
            emit sigResumeToSoak();
        }
    }
    else if( this->GetPreviousState() == (PSSM_SOAK))
    {
        emit sigResumeToSoak();
    }
    else if( this->GetPreviousState() == (PSSM_STEP_FINISH))
    {
        emit sigResumeToStepFinished();
    }
    else
    {
        //should not enter here
    }
}

void CSchedulerStateMachine::NotifyResumeDrain()
{
    if(this->GetPreviousState() == (PSSM_READY_TO_TUBE_AFTER))
    {
        emit sigResumeToReadyToTubeAfter();
    }
    else if(this->GetPreviousState() == (PSSM_READY_TO_DRAIN))
    {
        emit sigResumeToReadyToTubeAfter();
    }
}

void CSchedulerStateMachine::EnterRsStandByWithTissue()
{
    emit SigEnterRsStandByWithTissue();
}

void CSchedulerStateMachine::EnterRsHeatingErr30SRetry()
{
    emit SigEnterRSHeatingErr30SRetry();
}

void CSchedulerStateMachine::HandleRsStandByWithTissueWorkFlow(bool flag)
{
    mp_RsStandbyWithTissue->OnHandleWorkFlow(flag);
}

void CSchedulerStateMachine::HandleRsHeatingErr30SRetry(bool flag)
{
    mp_RsHeatingErr30SRetry->OnHandleWorkFlow(flag);
}


void CSchedulerStateMachine::EnterRcLevelsensorHeatingOvertime()
{
    emit SigEnterRcLevelsensorHeatingOvertime();
}


void CSchedulerStateMachine::HandleRcLevelSensorHeatingOvertimeWorkFlow(bool flag)
{
    mp_RcLevelSensorHeatingOvertime->OnHandleWorkFlow(flag);
}

void CSchedulerStateMachine::EnterRcRestart()
{
    emit SigEnterRcRestart();
}


void CSchedulerStateMachine::HandleRcRestart(bool flag)
{
    mp_RcRestart->OnHandleWorkFlow(flag);
}

void CSchedulerStateMachine::SendRunSelfTest()
{
    emit RunSelfTest();
}

void CSchedulerStateMachine::SendRunCleaning()
{
    emit RunCleaning();
}


QString CSchedulerStateMachine::GetDeviceName()
{
    ReturnCode_t EventID = mp_SchedulerThreadController->GetCurErrEventID();

    QString DevName = "";
    switch (EventID)
    {
    case DCL_ERR_DEV_RETORT_BOTTOM_SIDELOW_HEATING_ELEMENT_FAILED:
    case DCL_ERR_DEV_RETORT_SIDTOP_SIDEMID_HEATING_ELEMENT_FAILED:
    case DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_OVERANGE:
    case DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_NOSIGNAL:
    case DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_OVERRANGE:
    case DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_NOSIGNAL:
    case DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_OVERRANGE:
    case DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_NOSIGNAL:
    case DCL_ERR_DEV_RETORT_TSENSOR1_TO_2_SELFCALIBRATION_FAILED:
        DevName = "Retort";
        break;
    case DCL_ERR_DEV_RETORT_LEVELSENSOR_TEMPERATURE_OVERRANGE:
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
