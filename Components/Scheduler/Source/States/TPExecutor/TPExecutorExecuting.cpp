#include "Scheduler/Include/States/TPExecutor/TPExecutorExecuting.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelected.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAction.h"

namespace Scheduler{
namespace TPExecutorStates{

Executing::Executing(IEventHandler* pHandler, SchedulerMainThreadController* controller)
    :StateBase<Global::CommandShPtr_t>(pHandler, controller)
{
    setObjectName("TPExecutor_Busy_State");
}

Executing::~Executing()
{

}

void Executing::onEntry(QEvent *event)
{
    StateBase<Global::CommandShPtr_t>::onEntry(event);
}

bool Executing::HandleEvent(TPEventArgs<Global::CommandShPtr_t> *event, TPTransition_t &pTransition)
{

    return false;
}

void Executing::RepeatAction(TPTransition_t &pTransition)
{
    // check if session is done
    pTransition = TPTransition_t::ActionDone;
}
}
}
