#include "Scheduler/Include/States/TPExecutor/TPExecutorIdle.h"
#include "Scheduler/Include/Session.h"
#include "Scheduler/Include/TPExecutor.h"


namespace Scheduler{
namespace TPExecutorStates{
Idle::Idle(IEventHandler* pHandler, SchedulerMainThreadController* controller)
    :StateBase<ControlCommandType_t>(pHandler, controller)
{
    setObjectName("TPExecutor_Idle_State");
}

void Idle::onEntry(QEvent *event)
{
    StateBase<ControlCommandType_t>::onEntry(event);
}

bool Idle::HandleEvent(TPEventArgs<ControlCommandType_t> *event, TPTransition_t& pTransition)
{
    auto ctrlCmd = event->Data();
    if(event->Data() == ControlCommandType_t::CTRL_CMD_START)
    {
        auto handler = dynamic_cast<Scheduler::TPExecutor*>(IState::m_pHandler);
        if(handler != nullptr)
        {
            auto session = handler->GetCurrentSession();
            //todo: check if session is not scheduled
        }
        event->SetHandled();
        pTransition = TPTransition_t::Start;
        return true;
    }
    return false;
}

//void Idle::RepeatAction(TPTransition_t &pTransition)
//{
////    pTransition = TPTransition_t::Start;
//}
}
}
