#include "Scheduler/Include/States/Instrument/Scheduling.h"

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

bool Scheduling::HandleEvent(TPEventArgs<Global::CommandShPtr_t> *event, TPTransition_t &pTransition)
{
     pTransition = TPTransition_t::Done;
    return false;
}

}
}
