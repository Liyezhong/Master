#include "Scheduler/Include/States/Instrument/Busy.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelected.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAction.h"
#include "Scheduler/Include/InstrumentManager.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAcknowledge.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/Session.h"

namespace Scheduler{
namespace Instrument{

Busy::Busy(IEventHandler* pHandler, SchedulerMainThreadController* controller)
    :Idle(pHandler, controller)
{
    setObjectName("Instrument_Busy_State");
}

Busy::~Busy()
{

}

void Busy::onEntry(QEvent *event)
{
    StateBase<Global::CommandShPtr_t>::onEntry(event);

    foreach(auto session, Idle::m_pManager->GetInitialSession())
    {
        MsgClasses::CmdProgramAcknowledge* commandPtr(
                    new MsgClasses::CmdProgramAcknowledge(5000, DataManager::PROGRAM_READY, session->GetRetortID()));
        Q_ASSERT(commandPtr);
        Global::tRefType Ref = IState::m_pController->GetNewCommandRef();
        IState::m_pController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
    }
}

}
}
