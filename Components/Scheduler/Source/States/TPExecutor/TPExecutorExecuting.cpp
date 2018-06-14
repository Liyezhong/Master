#include "Scheduler/Include/States/TPExecutor/TPExecutorExecuting.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelected.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdCurrentProgramStepInfor.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAction.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAcknowledge.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/TPExecutor.h"
#include "Scheduler/Include/Actions/IAction.h"
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

        MsgClasses::CmdCurrentProgramStepInfor* commandPtr
                (new MsgClasses::CmdCurrentProgramStepInfor(5000, IState::m_pHandler->objectName(), IState::m_pController->GetReagentName(action->GetReagentID()),
                                                            action->GetActionName(), 0, action->GetDuration()));
        Q_ASSERT(commandPtr);
        Global::tRefType Ref = IState::m_pController->GetNewCommandRef();
        IState::m_pController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

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

void Executing::onExit(QEvent *event)
{
    auto executor = dynamic_cast<Scheduler::TPExecutor*>(IState::m_pHandler);

    if(executor != nullptr)
    {
        auto action = executor->GetCurrentSession()->GetActionList()[executor->GetCurrentAction()];

        // check if session is done
        if(action->IsFinished())
        {
//            bool isLastStep = (executor->GetCurrentAction() == (executor->GetCurrentSession()->GetActionList().size() - 2));
//            if ( isLastStep && ActionType_t::SOAKING == action->GetActionType())
//            {
//                MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_WILL_COMPLETE,
//                                                                                                          executor->GetCurrentSession()->GetRetortID()));
//                Q_ASSERT(commandPtrFinish);
//                Global::tRefType fRef = IState::m_pController->GetNewCommandRef();
//                IState::m_pController->SendCommand(fRef, Global::CommandShPtr_t(commandPtrFinish));
//            }
            return;
        }

    }
}
}
}
