#include "Scheduler/Include/TPEvent.h"

namespace Scheduler{
quint32 TPInternalEventBase::m_SeqNo = 0;



template class TPCmdEvent<Global::CommandShPtr_t>;
template class TPCmdEvent<Scheduler::SchedulerCommandShPtr_t>;
}
