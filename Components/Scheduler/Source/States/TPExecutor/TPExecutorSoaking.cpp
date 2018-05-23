#include "Scheduler/Include/States/TPExecutor/TPExecutorSoaking.h"


namespace Scheduler{
namespace TPExecutorStates{

Soaking::Soaking(IEventHandler* pHandler, SchedulerMainThreadController* controller)
    :StateBase<Global::CommandShPtr_t>(pHandler, controller)
{
    setObjectName("TPExecutor_Soaking_State");
}

Soaking::~Soaking()
{

}

void Soaking::onEntry(QEvent *event)
{
    StateBase<Global::CommandShPtr_t>::onEntry(event);
}

void Soaking::RepeatAction(TPTransition_t &pTransition)
{
    pTransition = TPTransition_t::SoakingDone;
}

bool Soaking::HandleEvent(TPEventArgs<Global::CommandShPtr_t> *event, TPTransition_t &pTransition)
{

}

}
}
