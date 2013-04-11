#include "../Include/SchedulerMachine.h"

namespace Scheduler
{
SchedulerMachine::SchedulerMachine()
{
    mp_SchedulerMachine = new QStateMachine();
    mp_InitState = new QState(mp_SchedulerMachine);
    mp_IdleState = new QState(mp_SchedulerMachine);
    mp_RunState = new QState(mp_SchedulerMachine);
    mp_PauseState = new QState(mp_SchedulerMachine);
    mp_ErrorState = new QState(mp_SchedulerMachine);

    mp_SchedulerMachine->setInitialState(mp_InitState);
    mp_InitState->addTransition(this, SIGNAL(SchedulerInitComplete()), mp_IdleState);
    mp_IdleState->addTransition(this, SIGNAL(RunSignal()), mp_RunState);
    mp_RunState->addTransition(this, SIGNAL(RunComplete()), mp_IdleState);
    mp_RunState->addTransition(this, SIGNAL(PauseSignal()), mp_PauseState);
    mp_RunState->addTransition(this, SIGNAL(ErrorSignal()), mp_ErrorState);
    mp_PauseState->addTransition(this, SIGNAL(RunSignal()), mp_RunState);
    mp_PauseState->addTransition(this, SIGNAL(ErrorSignal()), mp_ErrorState);
    mp_InitState->addTransition(this, SIGNAL(ErrorSignal()), mp_ErrorState);
    mp_IdleState->addTransition(this, SIGNAL(ErrorSignal()), mp_ErrorState);
    mp_ErrorState->addTransition(this, SIGNAL(RecoverToPause()), mp_PauseState);
}

SchedulerMachine::~SchedulerMachine()
{
    delete mp_ErrorState;
    delete mp_PauseState;
    delete mp_RunState;
    delete mp_IdleState;
    delete mp_InitState;
    delete mp_SchedulerMachine;
}

void SchedulerMachine::Start()
{
    mp_SchedulerMachine->start();
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

void SchedulerMachine::SendPauseSignal()
{
    emit PauseSignal();
}

void SchedulerMachine::SendRecoverToPause()
{
    emit RecoverToPause();
}

SchedulerState_t SchedulerMachine::GetCurrentState()
{
    if(mp_SchedulerMachine->configuration().contains(mp_InitState))
    {
        return INIT_STATE;
    }else if(mp_SchedulerMachine->configuration().contains(mp_IdleState))
    {
        return IDLE_STATE;
    }else if(mp_SchedulerMachine->configuration().contains(mp_ErrorState))
    {
        return ERROR_STATE;
    }else if(mp_SchedulerMachine->configuration().contains(mp_RunState))
    {
        return RUN_STATE;
    }else if(mp_SchedulerMachine->configuration().contains(mp_PauseState))
    {
        return PAUSE_STATE;
    }else
    {
        return STATE_NOT_EXISTED;
    }

}

}
