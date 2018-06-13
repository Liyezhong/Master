#include "Scheduler/Include/States/TPExecutor/TPExecutorIdle.h"
#include "Scheduler/Include/Session.h"
#include "Scheduler/Include/TPExecutor.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAcknowledge.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"


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

    commandStartPtr = nullptr;
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

        commandStartPtr = new MsgClasses::CmdProgramAcknowledge
                (500, DataManager::ProgramAcknownedgeType_t::PROGRAM_RUN_BEGIN, IState::m_pHandler->objectName());
        event->SetHandled();
        pTransition = TPTransition_t::Start;
        return true;
    }
    return false;
}

void Idle::onExit(QEvent* event)
{
    if(commandStartPtr != nullptr)
    {
        Global::tRefType fRef = IState::m_pController->GetNewCommandRef();
        IState::m_pController->SendCommand(ref, Global::CommandShPtr_t(commandStartPtr));
        commandStartPtr = nullptr;
    }
}
}
}
