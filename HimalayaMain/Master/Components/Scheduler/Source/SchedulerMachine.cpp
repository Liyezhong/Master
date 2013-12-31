#include "../Include/SchedulerMachine.h"
#include "EventHandler/Include/CrisisEventHandler.h"

namespace Scheduler
{
SchedulerMachine::SchedulerMachine()
{
    mp_SchedulerMachine = new QStateMachine();
    mp_InitState = new QState(mp_SchedulerMachine);
    mp_IdleState = new QState(mp_SchedulerMachine);
    mp_BusyState = new QState(mp_SchedulerMachine);
    mp_ErrorState = new QState(mp_SchedulerMachine);
    mp_ProgramStepStates = new ProgramStepStateMachine(mp_BusyState);

    mp_SchedulerMachine->setInitialState(mp_InitState);
    mp_InitState->addTransition(this, SIGNAL(SchedulerInitComplete()), mp_IdleState);
    mp_IdleState->addTransition(this, SIGNAL(RunSignal()), mp_BusyState);
    mp_BusyState->addTransition(this, SIGNAL(RunComplete()), mp_IdleState);
    mp_BusyState->addTransition(this, SIGNAL(ErrorSignal()), mp_ErrorState);
    mp_InitState->addTransition(this, SIGNAL(ErrorSignal()), mp_ErrorState);
    mp_IdleState->addTransition(this, SIGNAL(ErrorSignal()), mp_ErrorState);

    connect(mp_InitState, SIGNAL(entered()), this, SLOT(OnStateChanged()));
    connect(mp_IdleState, SIGNAL(entered()), this, SLOT(OnStateChanged()));
    connect(mp_BusyState, SIGNAL(entered()), this, SLOT(OnStateChanged()));
    connect(mp_ErrorState, SIGNAL(entered()), this, SLOT(OnStateChanged()));

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
    connect(this, SIGNAL(sigResumeToHeatLevelSensorS1()), mp_ProgramStepStates, SIGNAL(ResumeToHeatLevelSensorS1()));
    connect(this, SIGNAL(sigResumeToHeatLevelSensorS2()), mp_ProgramStepStates, SIGNAL(ResumeToHeatLevelSensorS2()));
    connect(this, SIGNAL(sigResumeToReadyToFill()), mp_ProgramStepStates, SIGNAL(ResumeToReadyToFill()));
    connect(this, SIGNAL(sigResumeToSoak()), mp_ProgramStepStates, SIGNAL(ResumeToSoak()));
    connect(this, SIGNAL(sigResumeToStepFinished()), mp_ProgramStepStates, SIGNAL(ResumeToStepFinished()));
    connect(this, SIGNAL(sigResumeToReadyToTubeAfter()), mp_ProgramStepStates, SIGNAL(ResumeToReadyToTubeAfter()));
    connect(this, SIGNAL(sigAbort()), mp_ProgramStepStates, SIGNAL(Abort()));

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
}

void SchedulerMachine::OnStateChanged()
{
    SchedulerStateMachine_t currentState = GetCurrentState();
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
    LOG_STR_ARG(STR_SCHDEULER_MAIN_CONTROLLER_STATE,Global::tTranslatableStringList()<<Global::TranslatableString(stateid));
}

SchedulerMachine::~SchedulerMachine()
{
    delete mp_ErrorState;
    delete mp_BusyState;
    delete mp_IdleState;
    delete mp_InitState;
    delete mp_ProgramStepStates;
    delete mp_SchedulerMachine;
}

void SchedulerMachine::Start()
{
    mp_SchedulerMachine->start();
}

void SchedulerMachine::Stop()
{
    mp_SchedulerMachine->stop();
}

void SchedulerMachine::SendSchedulerInitComplete()
{
    emit SchedulerInitComplete();
}

void SchedulerMachine::SendRunSignal()
{
    emit RunSignal();
}

void SchedulerMachine::SendRunComplete()
{
    emit RunComplete();
}

void SchedulerMachine::SendErrorSignal()
{
    emit ErrorSignal();
}

SchedulerStateMachine_t SchedulerMachine::GetCurrentState()
{
    if(mp_SchedulerMachine->configuration().contains(mp_InitState))
    {
        return SM_INIT;
    }else if(mp_SchedulerMachine->configuration().contains(mp_IdleState))
    {
        return SM_IDLE;
    }else if(mp_SchedulerMachine->configuration().contains(mp_ErrorState))
    {
        return SM_ERROR;
    }else if(mp_SchedulerMachine->configuration().contains(mp_BusyState))
    {
        return mp_ProgramStepStates->GetCurrentState(mp_SchedulerMachine->configuration());
    }else
    {
        return SM_UNDEF;
    }
}

SchedulerStateMachine_t SchedulerMachine::GetPreviousState()
{
    return mp_ProgramStepStates->GetPreviousState();
}

void SchedulerMachine::NotifyStInitOK()
{
    emit sigStInitOK();
}

void SchedulerMachine::NotifyStTempOK()
{
    emit sigStTempOK();
}

void SchedulerMachine::NotifyStCurrentOK()
{
    emit sigStCurrentOK();
}

void SchedulerMachine::NotifyStVoltageOK()
{
    emit sigStVoltageOK();
}

void SchedulerMachine::NotifyStRVPositionOK()
{
    emit sigStRVPositionOK();
}

void SchedulerMachine::NotifyStPressureOK()
{
    emit sigStPressureOK();
}

void SchedulerMachine::NotifyStSealingOK()
{
    emit sigStSealingOK();
}

void SchedulerMachine::NotifyStGetStationcheckResult()
{
    emit sigStGetStationcheckResult();
}

void SchedulerMachine::NotifyStStationLeft()
{
    emit sigStStationLeft();
}

void SchedulerMachine::NotifyStStationOK()
{
    emit sigStStationOK();
}

void SchedulerMachine::NotifyStDone()
{
    emit sigStDone();
}
void SchedulerMachine::NotifyTempsReady()
{
    emit sigTempsReady();
}

void SchedulerMachine::NotifyLevelSensorTempS1Ready()
{
    emit sigLevelSensorTempS1Ready();
}

void SchedulerMachine::NotifyLevelSensorTempS2Ready()
{
    emit sigLevelSensorTempS2Ready();
}

void SchedulerMachine::NotifyHitTubeBefore()
{
    emit sigHitTubeBefore();
}

void SchedulerMachine::NotifyFillFinished()
{
    emit sigFillFinished();
}

void SchedulerMachine::NotifyHitSeal()
{
    emit sigHitSeal();
}

void SchedulerMachine::NotifySoakFinished()
{
    emit sigSoakFinished();
}

void SchedulerMachine::NotifyHitTubeAfter()
{
    emit sigHitTubeAfter();
}

void SchedulerMachine::NotifyDrainFinished()
{
    emit sigDrainFinished();
}

void SchedulerMachine::NotifyStepFinished()
{
    emit sigStepFinished();
}

void SchedulerMachine::NotifyProgramFinished()
{
    emit sigProgramFinished();
}

void SchedulerMachine::NotifyError()
{
    emit sigError();
}

void SchedulerMachine::NotifyPause(SchedulerStateMachine_t PreviousState)
{
    mp_ProgramStepStates->SetPreviousState(PreviousState);
    emit sigPause();
}

void SchedulerMachine::NotifyAbort()
{
    mp_ProgramStepStates->SetPreviousState(this->GetCurrentState());
    emit sigAbort();
}

void SchedulerMachine::NotifyResumeToSelftest()
{
    emit sigResumeToSelftest();
}

void SchedulerMachine::NotifyResume()
{
    //emit sigResume(m_PreviousState);
    if( mp_ProgramStepStates->GetPreviousState() == (PSSM_INIT))
    {
        emit sigResumeToInit();
    }
    else if( mp_ProgramStepStates->GetPreviousState()== (PSSM_READY_TO_HEAT_LEVEL_SENSOR_S1))
    {
        emit sigResumeToHeatLevelSensorS1();
    }
    else if( mp_ProgramStepStates->GetPreviousState() == (PSSM_READY_TO_HEAT_LEVEL_SENSOR_S2))
    {
        emit sigResumeToHeatLevelSensorS2();
    }
    else if( mp_ProgramStepStates->GetPreviousState() == (PSSM_READY_TO_TUBE_BEFORE))
    {
        emit sigResumeToReadyToFill();
    }
    else if( mp_ProgramStepStates->GetPreviousState() == (PSSM_READY_TO_FILL))
    {
        emit sigResumeToReadyToFill();
    }
    else if( mp_ProgramStepStates->GetPreviousState() == (PSSM_READY_TO_SEAL))
    {
        emit sigResumeToSoak();
    }
    else if( mp_ProgramStepStates->GetPreviousState() == (PSSM_SOAK))
    {
        emit sigResumeToSoak();
    }
    else if( mp_ProgramStepStates->GetPreviousState() == (PSSM_STEP_FINISH))
    {
        emit sigResumeToStepFinished();
    }
    else
    {
        //should not enter here
    }
     mp_ProgramStepStates->SetPreviousState(PSSM_PAUSE);
}

void SchedulerMachine::NotifyResumeDrain()
{
    if( mp_ProgramStepStates->GetPreviousState() == (PSSM_READY_TO_TUBE_AFTER))
    {
        emit sigResumeToReadyToTubeAfter();
    }
    else if( mp_ProgramStepStates->GetPreviousState() == (PSSM_READY_TO_DRAIN))
    {
        emit sigResumeToReadyToTubeAfter();
    }
    mp_ProgramStepStates->SetPreviousState(PSSM_PAUSE_DRAIN);
}


}
