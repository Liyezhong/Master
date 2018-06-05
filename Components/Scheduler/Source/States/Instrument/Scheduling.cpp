#include "Scheduler/Include/States/Instrument/Scheduling.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelectedReply.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelected.h"
#include "Scheduler/Include/ProtocolParser.h"
#include "Scheduler/Include/InstrumentManager.h"

namespace Scheduler{
namespace Instrument{

Scheduling::Scheduling(IEventHandler* pHandler, SchedulerMainThreadController* controller)
    :StateBase<Global::CommandShPtr_t>(pHandler, controller),
      commandPtr(nullptr)
{
    setObjectName("Instrument_Scheduling_State");
}

Scheduling::~Scheduling()
{

}

void Scheduling::onEntry(QEvent *event)
{
    StateBase<Global::CommandShPtr_t>::onEntry(event);
    ProtocolParser* parser = IState::m_pController->GetProtocolParser();
    parser->GenerateActionList(((InstrumentManager*)IState::m_pHandler)->GetInitialSession().first());
    commandPtr = nullptr;
    ref = -1;
}

void Scheduling::onExit(QEvent *event)
{
    if(commandPtr != nullptr)
    {
        IState::m_pController->SendCommand(ref, Global::CommandShPtr_t(commandPtr));
    }
}

void Scheduling::RepeatAction(TPTransition_t &pTransition)
{

}

//void Scheduling::ProgramSelectedReply(Global::tRefType ref, const MsgClasses::CmdProgramSelected& cmd, unsigned int timeProposed)
//{
//    unsigned int paraffinMeltCostedtime = 0;
//    unsigned int costedTimeBeforeParaffin = 0;
//    int whichStep = 0;

//    //send back the proposed program end time
//    QList<QString> stationList;
//    stationList << "S6";
//    commandPtr = new MsgClasses::CmdProgramSelectedReply(5000, cmd.GetRetortId(), timeProposed,
//                                                                                paraffinMeltCostedtime,
//                                                                                costedTimeBeforeParaffin,
//                                                                                whichStep,
//                                                                                0,
//                                                                                stationList,
//                                                                                0);

//}

bool Scheduling::HandleEvent(TPEventArgs<Global::CommandShPtr_t> *event, TPTransition_t &pTransition)
{
    auto pSelectedcmd = dynamic_cast<MsgClasses::CmdProgramSelected*>(event->Data().GetPointerToUserData());
    if(pSelectedcmd != nullptr)
    {


        ref = event->Ref();
        auto timeProposed = 500;
//        ProgramSelectedReply(ref, *pSelectedcmd, timeProposed);
        event->SetHandled();
        pTransition = TPTransition_t::Done;
        return true;
    }
    pTransition = TPTransition_t::Done;
    return true;
    return false;
}


}
}
