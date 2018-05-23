#include "Scheduler/Include/TPEvent.h"

namespace Scheduler{
quint32 TPEventArgsBase::m_SeqNo = 0;



template class TPEventArgs<Global::CommandShPtr_t>;
template class TPEventArgs<Scheduler::SchedulerCommandShPtr_t>;
}
