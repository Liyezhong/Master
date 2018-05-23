#include "Scheduler/Include/States/TimeoutState.h"

namespace Scheduler{

template <class T>
TimeoutState<T>::TimeoutState(IEventHandler* pHandler, Scheduler::SchedulerMainThreadController* controller)
    :StateBase<T>(pHandler, controller)
{

}

}
