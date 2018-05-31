#include "Scheduler/Include/States/Instrument/Scheduling.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelectedReply.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelected.h"

namespace Scheduler{
namespace Instrument{

Scheduling::Scheduling(IEventHandler* pHandler, SchedulerMainThreadController* controller)
    :StateBase<Global::CommandShPtr_t>(pHandler, controller)
{
    setObjectName("Instrument_Scheduling_State");
}

Scheduling::~Scheduling()
{

}

void Scheduling::onEntry(QEvent *event)
{
    StateBase<Global::CommandShPtr_t>::onEntry(event);
}

void Scheduling::RepeatAction(TPTransition_t &pTransition)
{
    
}

void Scheduling::ProgramSelectedReply(Global::tRefType ref, const MsgClasses::CmdProgramSelected& cmd, unsigned int timeProposed)
{
    unsigned int paraffinMeltCostedtime = 0;
    unsigned int costedTimeBeforeParaffin = 0;
    int whichStep = 0;

    //send back the proposed program end time
    QList<QString> stationList;
    MsgClasses::CmdProgramSelectedReply* commandPtr(new MsgClasses::CmdProgramSelectedReply(5000, cmd.GetRetortId(), timeProposed,
                                                                                paraffinMeltCostedtime,
                                                                                costedTimeBeforeParaffin,
                                                                                whichStep,
                                                                                0,
                                                                                stationList,
                                                                                0));
    Q_ASSERT(commandPtr);
    IState::m_pController->SendCommand(ref, Global::CommandShPtr_t(commandPtr));
}

bool Scheduling::HandleEvent(TPEventArgs<Global::CommandShPtr_t> *event, TPTransition_t &pTransition)
{
    auto pSelectedcmd = dynamic_cast<MsgClasses::CmdProgramSelected*>(event->Data().GetPointerToUserData());
    if(pSelectedcmd != nullptr)
    {
        auto timeProposed = 500;
        ProgramSelectedReply(event->Ref(), *pSelectedcmd, timeProposed);
        pTransition = TPTransition_t::Done;
        event->SetHandled();
        return true;
    }

    return false;
}

}
}
