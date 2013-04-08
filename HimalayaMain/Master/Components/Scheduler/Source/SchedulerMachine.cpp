#include "../Include/SchedulerMachine.h"

namespace Scheduler
{
SchedulerMachine::SchedulerMachine(SchedulerMainThreadController *schedulerMainController)
{
    mp_SchedulerMachine = new QStateMachine();
    mp_InitState = new QState(mp_SchedulerMachine);
    mp_IdleState = new QState(mp_SchedulerMachine);
    mp_RunState = new QState(mp_SchedulerMachine);
    mp_ErrorState = new QState(mp_SchedulerMachine);

    mp_SchedulerMachine->setInitialState(mp_InitState);
    mp_InitState->addTransition(schedulerMainController, SIGNAL(SchedulerInitComplete()), mp_IdleState);
    mp_IdleState->addTransition(schedulerMainController, SIGNAL(RunSignal()), mp_RunState);
    mp_RunState->addTransition(schedulerMainController, SIGNAL(RunComplete()), mp_IdleState);
    mp_InitState->addTransition(schedulerMainController, SIGNAL(ErrorSiganl()), mp_ErrorState);
    mp_IdleState->addTransition(schedulerMainController, SIGNAL(ErrorSiganl()), mp_ErrorState);
    mp_RunState->addTransition(schedulerMainController, SIGNAL(ErrorSiganl()), mp_ErrorState);
}

SchedulerMachine::~SchedulerMachine()
{
    delete mp_ErrorState;
    delete mp_RunState;
    delete mp_IdleState;
    delete mp_InitState;
    delete mp_SchedulerMachine;
}
}
