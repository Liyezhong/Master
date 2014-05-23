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
    m_PreviousState = SM_UNDEF;
    m_CurrentState = SM_UNDEF;

    mp_SchedulerMachine = new QStateMachine();
    mp_InitState = new QState(mp_SchedulerMachine);
    mp_IdleState = new QState(mp_SchedulerMachine);
    mp_BusyState = new QState(mp_SchedulerMachine);
    mp_ErrorState = new QState(mp_SchedulerMachine);
    mp_ErrorWaitState = new QState(mp_ErrorState);
    mp_ErrorRSStandbyWithTissueState = new QState(mp_ErrorState);
    mp_ErrorRCLevelSensorHeatingOvertimeState = new QState(mp_ErrorState);
    mp_SchedulerMachine->setInitialState(mp_InitState);
    mp_ErrorState->setInitialState(mp_ErrorWaitState);

    mp_ProgramStepStates = new CProgramStepStateMachine(mp_BusyState, mp_ErrorState);
    mp_RSRvGetOriginalPositionAgain = new CRsRvGetOriginalPositionAgain(mp_SchedulerMachine, mp_ErrorState );
    mp_RCReport = new CRCReport(mp_SchedulerMachine, mp_ErrorState );
    mp_RSStandby = new CRsStandby(mp_SchedulerMachine, mp_ErrorState);
    mp_RSStandbyWithTissue = new CRsStandbyWithTissue(mp_SchedulerMachine, mp_ErrorRSStandbyWithTissueState);
    mp_RcLevelSensorHeatingOvertime = new CRcLevelSensorHeatingOvertime(mp_SchedulerMachine, mp_ErrorRCLevelSensorHeatingOvertimeState);

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
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRsStandByWithTissue()), mp_ErrorRSStandbyWithTissueState);
    mp_ErrorRSStandbyWithTissueState->addTransition(mp_RSStandbyWithTissue, SIGNAL(TasksDone(bool)), mp_ErrorWaitState);

    //RC_Levelsensor_Heating_Overtime related logic
    mp_ErrorWaitState->addTransition(this, SIGNAL(SigEnterRcLevelsensorHeatingOvertime()), mp_ErrorRCLevelSensorHeatingOvertimeState);
    mp_ErrorRCLevelSensorHeatingOvertimeState->addTransition(mp_RcLevelSensorHeatingOvertime, SIGNAL(TasksDone(bool)), mp_ErrorWaitState);


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

    connect(this, SIGNAL(sigRsRvMoveToInitPosition()), mp_RSRvGetOriginalPositionAgain, SIGNAL(RvMoveToInitPosition()));
    connect(this, SIGNAL(sigRsRvMoveToInitPositionFinished()), mp_RSRvGetOriginalPositionAgain, SIGNAL(RvMoveToInitPositionFinished()));

    connect(this, SIGNAL(sigRCReport()), mp_RCReport, SIGNAL(RCReport()));
    connect(this, SIGNAL(sigRCReport()), mp_RCReport, SIGNAL(RCReport()));

    connect(this, SIGNAL(sigReleasePressure()), mp_RSStandby, SIGNAL(ReleasePressure()));
    connect(this, SIGNAL(sigShutdownFailedHeater()), mp_RSStandby, SIGNAL(ShutdownFailedHeater()));
    connect(this, SIGNAL(sigShutdownFailedHeaterFinished()), mp_RSStandby, SIGNAL(ShutdownFailedHeaterFinished()));

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
    connect(mp_RSRvGetOriginalPositionAgain, SIGNAL(OnRvMoveToInitPosition()), this, SIGNAL(sigOnRsRvMoveToInitPosition()));
    connect(mp_RCReport, SIGNAL(OnRCReport()), this, SIGNAL(sigOnRCReport()));

    connect(mp_RSStandby, SIGNAL(OnReleasePressure()), this, SIGNAL(sigOnRsReleasePressure()));
    connect(mp_RSStandby, SIGNAL(OnShutdownFailedHeater()), this, SIGNAL(sigOnRsShutdownFailedHeater()));


    CONNECTSIGNALSLOT(mp_RSStandbyWithTissue, RTStopTempCtrl(DeviceControl::RTTempCtrlType_t), this, OnRsRTStopTempCtrl(DeviceControl::RTTempCtrlType_t));
    CONNECTSIGNALSLOT(mp_RSStandbyWithTissue, ShutdownFailedHeater(), this, OnRsShutdownFailedHeater());
    CONNECTSIGNALSLOT(mp_RSStandbyWithTissue, ReleasePressure(), this, OnRsReleasePressure());
    CONNECTSIGNALSLOT(mp_RSStandbyWithTissue, TasksDone(bool), this, OnTasksDone(bool));

    CONNECTSIGNALSLOT(mp_RcLevelSensorHeatingOvertime, RestartLevelSensorTempControl(), this, OnRestartLevelSensorTempControl());
    CONNECTSIGNALSLOT(mp_RcLevelSensorHeatingOvertime, TasksDone(bool), this, OnTasksDone(bool));
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

void CSchedulerStateMachine::OnRsRTStopTempCtrl(DeviceControl::RTTempCtrlType_t Type)
{
    CmdRTSetTempCtrlOFF* cmd = new CmdRTSetTempCtrlOFF(500, mp_SchedulerThreadController);
    cmd->SetType(Type);
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
   mp_SchedulerThreadController->RestartLevelSensorTempCtrl();
}

void CSchedulerStateMachine::OnTasksDone(bool flag)
{
    Global::EventObject::Instance().RaiseEvent(mp_SchedulerThreadController->GetEventKey(), 0, 0, flag);
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

    delete mp_RSRvGetOriginalPositionAgain;
    mp_RSRvGetOriginalPositionAgain = NULL;

    delete mp_RSStandby;
    mp_RSStandby = NULL;

    delete mp_ErrorRSStandbyWithTissueState;
    mp_ErrorRSStandbyWithTissueState = NULL;

    delete mp_RSStandbyWithTissue;
    mp_RSStandbyWithTissue = NULL;

    delete mp_ErrorRCLevelSensorHeatingOvertimeState;
    mp_ErrorRCLevelSensorHeatingOvertimeState = NULL;

    delete mp_RcLevelSensorHeatingOvertime;
    mp_RcLevelSensorHeatingOvertime = NULL;

    delete mp_RCReport;
    mp_RCReport = NULL;
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
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRSStandbyWithTissueState))
        {
            return SM_ERR_RS_STANDBY_WITH_TISSUE;
        }
        else if (mp_SchedulerMachine->configuration().contains(mp_ErrorRCLevelSensorHeatingOvertimeState))
        {
            return SM_ERR_RC_LEVELSENSOR_HEATING_OVERTIME;
        }
        else if((currentState = mp_RSRvGetOriginalPositionAgain->GetCurrentState(mp_SchedulerMachine->configuration())) != SM_UNDEF);
        else if((currentState = mp_RCReport->GetCurrentState(mp_SchedulerMachine->configuration())) != SM_UNDEF);
        else if((currentState = mp_RSStandby->GetCurrentState(mp_SchedulerMachine->configuration())) != SM_UNDEF);

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

void CSchedulerStateMachine::NotifyResumeToSelftest()
{
    emit sigResumeToSelftest();
}

void CSchedulerStateMachine::NotifyRsRvMoveToInitPositionFinished()
{
    emit sigRsRvMoveToInitPositionFinished();
}

void CSchedulerStateMachine::NotifyRsRvMoveToInitPosition()
{
    emit sigRsRvMoveToInitPosition();
}

void CSchedulerStateMachine::NotifyRCReport()
{
    emit sigRCReport();
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

void CSchedulerStateMachine::NotifyResume()
{
    //emit sigResume(m_PreviousState);
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

void CSchedulerStateMachine::HandleRsStandByWithTissueWorkFlow(bool flag)
{
    mp_RSStandbyWithTissue->OnHandleWorkFlow(flag);
}

void CSchedulerStateMachine::EnterRcLevelsensorHeatingOvertime()
{
    emit SigEnterRcLevelsensorHeatingOvertime();
}


void CSchedulerStateMachine::HandleRcLevelSensorHeatingOvertimeWorkFlow(bool flag)
{
    mp_RcLevelSensorHeatingOvertime->OnHandleWorkFlow(flag);
}

}
