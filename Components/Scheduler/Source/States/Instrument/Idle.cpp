#include "Scheduler/Include/States/Instrument/Idle.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelected.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelectedReply.h"
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
    commandPtr = nullptr;
    ref = -1;
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

    foreach(auto session, m_pManager->GetReadySession())
    {
        MsgClasses::CmdProgramAcknowledge* commandPtr(
                    new MsgClasses::CmdProgramAcknowledge(5000, DataManager::PROGRAM_READY, session->GetRetortID()));
        Q_ASSERT(commandPtr);
        Global::tRefType Ref = IState::m_pController->GetNewCommandRef();
        IState::m_pController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        session->Log2File();
    }
}

bool Idle::HandleEvent(TPEventArgs<Global::CommandShPtr_t> *event, TPTransition_t& pTransition)
{
    qDebug() << " Instrument HandleEvent threadid: " << QThread::currentThreadId();
    auto pSelectedcmd = dynamic_cast<MsgClasses::CmdProgramSelected*>(event->Data().GetPointerToUserData());
    if(pSelectedcmd != nullptr)
    {
        auto id = m_pManager->CreateSession(pSelectedcmd->GetRetortId(), pSelectedcmd->GetProgramID());
        auto session = m_pManager->GetInitialSession();

        if(session.count() > 0)
            ProgramSelectedReply(ref, *pSelectedcmd, session.first()->GetProposedTime());
        event->SetHandled();
        pTransition = TPTransition_t::Load;
        return true;
    }

    auto actionCmd = dynamic_cast<MsgClasses::CmdProgramAction*>(event->Data().GetPointerToUserData());
    if(actionCmd != nullptr)
    {
        if(actionCmd->ProgramActionType() == DataManager::PROGRAM_START)
        {
            event->SetHandled();

            QString RetortName = actionCmd->GetRetortName();
            m_pManager->StartProtocol(RetortName);
            pTransition = TPTransition_t::Start;
            return true;
        }
    }
    return false;
}

void Idle::ProgramSelectedReply(Global::tRefType ref, const MsgClasses::CmdProgramSelected& cmd, unsigned int timeProposed)
{
    unsigned int paraffinMeltCostedtime = 0;
    unsigned int costedTimeBeforeParaffin = 0;
    int whichStep = 0;

    //send back the proposed program end time
    QList<QString> stationList;
    stationList << "S6";
    commandPtr = new MsgClasses::CmdProgramSelectedReply(5000, cmd.GetRetortId(), timeProposed,
                                                                                paraffinMeltCostedtime,
                                                                                costedTimeBeforeParaffin,
                                                                                whichStep,
                                                                                0,
                                                                                stationList,
                                                                                0);

}

void Idle::RepeatAction(TPTransition_t &pTransition)
{
    StateBase<Global::CommandShPtr_t>::RepeatAction(pTransition);

}

void Idle::onExit(QEvent *event)
{
    qDebug() << " Instrument threadid: " << QThread::currentThreadId();
    if(commandPtr != nullptr)
    {
        IState::m_pController->SendCommand(ref, Global::CommandShPtr_t(commandPtr));
        commandPtr = nullptr;
    }

}


}

}
