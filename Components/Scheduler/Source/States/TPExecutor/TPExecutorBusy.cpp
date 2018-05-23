#include "Scheduler/Include/States/TPExecutor/TPExecutorBusy.h"



namespace Scheduler{
namespace TPExecutorStates{
Busy::Busy(IEventHandler* pHandler, SchedulerMainThreadController* controller)
    :StateBase<ControlCommandType_t>(pHandler, controller)
{
    setObjectName("TPExecutor_Busy_State");
}

void Busy::onEntry(QEvent *event)
{
    StateBase<ControlCommandType_t>::onEntry(event);
}

bool Busy::HandleEvent(TPEventArgs<ControlCommandType_t> *event, TPTransition_t& pTransition)
{

    return false;
}

void Busy::RepeatAction(TPTransition_t &pTransition)
{

}
}
}
