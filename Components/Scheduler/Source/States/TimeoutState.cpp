#include "Scheduler/Include/States/TimeoutState.h"
#include "Scheduler/Include/TPTransition.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"

namespace Scheduler{

template <class T>
TimeoutState<T>::TimeoutState(IEventHandler* pHandler, Scheduler::SchedulerMainThreadController* controller)
    :StateBase<T>(pHandler, controller),
      m_CurrentTickCount(-1),
      m_TickTimerCount(0)
{
    QState::addTransition(new EventTransition(TPTransition_t::Timeout, this));
}

template <class T>
void TimeoutState<T>::Timeout(TPTransition_t &transition)
{

}

template <class T>
void TimeoutState<T>::TimeInterval(qint32 msecs)
{
    m_CurrentTickCount = msecs*1000/TICKTIMERINTERVAL;
}

template <class T>
void TimeoutState<T>::Enter(QEvent *event)
{

}

template <class T>
void TimeoutState<T>::onEntry(QEvent *event)
{
    m_TickTimerCount = 0;
    Enter(event);
}

template <class T>
void TimeoutState<T>::RepeatAction(TPTransition_t &transition)
{
    StateBase<T>::RepeatAction(transition);

    m_TickTimerCount++;
    if(m_TickTimerCount == m_CurrentTickCount)
    {
        Timeout(transition);
    }
}

template class TimeoutState<Scheduler::SchedulerCommandShPtr_t>;
template class TimeoutState<Global::CommandShPtr_t>;
template class TimeoutState<ControlCommandType_t>;

}
