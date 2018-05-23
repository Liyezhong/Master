#include "Scheduler/Include/States/TPExecutor/TPExecutorInit.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"


namespace Scheduler{
namespace TPExecutorStates{
Init::Init(IEventHandler* pHandler, SchedulerMainThreadController* controller)
    :StateBase<Scheduler::SchedulerCommandShPtr_t>(pHandler, controller)
{
    setObjectName("TPExecutor_Init_State");

}

Init::~Init()
{

}

void Init::onEntry(QEvent *event)
{
    StateBase<Scheduler::SchedulerCommandShPtr_t>::onEntry(event);

}

bool Init::HandleEvent(TPEventArgs<Scheduler::SchedulerCommandShPtr_t> *event, TPTransition_t& pTransition)
{
    pTransition = TPTransition_t::Initial;

    event->SetHandled();

    return true;
}
void Init::RepeatAction(TPTransition_t& pTransition)
{
    StateBase::RepeatAction(pTransition);   
}


}
}
