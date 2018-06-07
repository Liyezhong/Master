#include "Scheduler/Include/States/TPExecutor/TPExecutorExecuting.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelected.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAction.h"
#include "Scheduler/Include/TPExecutor.h"
#include "Scheduler/Include/IAction.h"
#include "Scheduler/Include/Session.h"

namespace Scheduler{
namespace TPExecutorStates{

Executing::Executing(IEventHandler* pHandler, SchedulerMainThreadController* controller)
    :Scheduler::TimeoutState<Scheduler::SchedulerCommandShPtr_t>(pHandler, controller)
{
    setObjectName("TPExecutor_Executing_State");
}

Executing::~Executing()
{

}

void Executing::Enter(QEvent *event)
{
    auto executor = dynamic_cast<Scheduler::TPExecutor*>(IState::m_pHandler);

    if(executor != nullptr)
    {
        auto action = executor->GetCurrentSession()->GetActionList()[executor->GetCurrentAction()];
        TimeInterval(action->GetDuration());
        action->Execute("", DeviceControl::DCL_ERR_FCT_CALL_FAILED);
    }
}

bool Executing::HandleEvent(TPEventArgs<Scheduler::SchedulerCommandShPtr_t> *event, TPTransition_t &pTransition)
{
    if(event->Data().isNull())
    {
        event->SetHandled();
        return false;
    }

    auto executor = dynamic_cast<Scheduler::TPExecutor*>(IState::m_pHandler);

    if(executor != nullptr)
    {
        auto action = executor->GetCurrentSession()->GetActionList()[executor->GetCurrentAction()];
        auto retCode = DeviceControl::DCL_ERR_FCT_CALL_FAILED;
        if(event->Data()->GetResult(retCode))
        {
            action->Execute(event->Data()->GetName(), retCode);
            event->SetHandled();
            return true;
        }
    }
    return false;
}

void Executing::Timeout(TPTransition_t &pTransition)
{

    pTransition = TPTransition_t::ActionDone;
}

void Executing::RepeatAction(TPTransition_t &pTransition)
{
    TimeoutState<Scheduler::SchedulerCommandShPtr_t>::RepeatAction(pTransition);

    auto executor = dynamic_cast<Scheduler::TPExecutor*>(IState::m_pHandler);

    if(executor != nullptr)
    {
        auto action = executor->GetCurrentSession()->GetActionList()[executor->GetCurrentAction()];

        // check if session is done
        if(action->IsFinished())
        {
            pTransition = TPTransition_t::ActionDone;
            return;
        }

         action->Execute("", DeviceControl::DCL_ERR_FCT_CALL_FAILED);
    }
}
}
}
