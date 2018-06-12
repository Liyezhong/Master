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

    machine()->postDelayedEvent(new TPEvent(TPTransition_t::Done), 500);
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

bool Scheduling::HandleEvent(TPEventArgs<Global::CommandShPtr_t> *event, TPTransition_t &pTransition)
{    
    pTransition = TPTransition_t::Done;
    return false;
}


}
}
