#include "Scheduler/Include/States/Instrument/Idle.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelected.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAction.h"

namespace Scheduler{
Idle::Idle(IEventHandler* pHandler, SchedulerMainThreadController* controller)
:StateBase<Global::CommandShPtr_t>(pHandler, controller)
{
    setObjectName("Instrument_Idle_State");
}

void Idle::onEntry(QEvent *event)
{
    StateBase<Global::CommandShPtr_t>::onEntry(event);
}

bool Idle::HandleEvent(TPCmdEvent<Global::CommandShPtr_t> *event, TPTransition_t& pTransition)
{
    auto pSelectedcmd = dynamic_cast<MsgClasses::CmdProgramSelected*>(event->Data().GetPointerToUserData());
    if(pSelectedcmd != nullptr)
    {
        pTransition = TPTransition_t::Load;
        return true;
    }

    auto actionCmd = dynamic_cast<MsgClasses::CmdProgramAction*>(event->Data().GetPointerToUserData());
    if(actionCmd != nullptr)
    {
        if(actionCmd->ProgramActionType() == DataManager::ProgramActionType_t::PROGRAM_START)
        {
            pTransition = TPTransition_t::Start;
        }
    }
    return false;
}
}
