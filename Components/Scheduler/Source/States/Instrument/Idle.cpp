#include "Scheduler/Include/States/Instrument/Idle.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelected.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAction.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAcknowledge.h"
#include "Scheduler/Include/InstrumentManager.h"
#include "Scheduler/Include/Session.h"

namespace Scheduler{
namespace Instrument{

Idle::Idle(IEventHandler* pHandler, SchedulerMainThreadController* controller)
    :StateBase<Global::CommandShPtr_t>(pHandler, controller)
{
    setObjectName("Instrument_Idle_State");
    m_pManager = dynamic_cast<InstrumentManager*>(pHandler);
    start = 1;
}

void Idle::onEntry(QEvent *event)
{
    StateBase<Global::CommandShPtr_t>::onEntry(event);

    if(start)
    {
        MsgClasses::CmdProgramAcknowledge* commandPtr(new MsgClasses::CmdProgramAcknowledge(5000, DataManager::PROGRAM_SELFTEST_PASSED));
        Q_ASSERT(commandPtr);
        Global::tRefType Ref = IState::m_pController->GetNewCommandRef();
        IState::m_pController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        start--;
        return;
    }

    foreach(auto session, m_pManager->GetActiveSession())
    {
        MsgClasses::CmdProgramAcknowledge* commandPtr(
                    new MsgClasses::CmdProgramAcknowledge(5000, DataManager::PROGRAM_READY, session->GetRetortID()));
        Q_ASSERT(commandPtr);
        Global::tRefType Ref = IState::m_pController->GetNewCommandRef();
        IState::m_pController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
    }
}

bool Idle::HandleEvent(TPEventArgs<Global::CommandShPtr_t> *event, TPTransition_t& pTransition)
{
    auto pSelectedcmd = dynamic_cast<MsgClasses::CmdProgramSelected*>(event->Data().GetPointerToUserData());
    if(pSelectedcmd != nullptr)
    {
        m_pManager->CreateSession(pSelectedcmd->GetRetortId(), pSelectedcmd->GetProgramID());

//        event->SetHandled();
        pTransition = TPTransition_t::Load;
        return true;
    }

    auto actionCmd = dynamic_cast<MsgClasses::CmdProgramAction*>(event->Data().GetPointerToUserData());
    if(actionCmd != nullptr)
    {
        if(actionCmd->ProgramActionType() == DataManager::ProgramActionType_t::PROGRAM_START)
        {
            event->SetHandled();

            m_pManager->StartProtocol(actionCmd->GetRetortName());
            pTransition = TPTransition_t::Start;
            return true;
        }
    }
    return false;
}

void Idle::RepeatAction(TPTransition_t &pTransition)
{
    StateBase<Global::CommandShPtr_t>::RepeatAction(pTransition);
}


}

}
