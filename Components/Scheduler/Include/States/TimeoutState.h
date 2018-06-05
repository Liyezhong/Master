#ifndef TIMEOUTSTATE_H
#define TIMEOUTSTATE_H
#include "Scheduler/Include/States/StateBase.h"

namespace Scheduler{

template <class T>
class TimeoutState : public StateBase<T>
{
public:
    TimeoutState(IEventHandler* pHandler, Scheduler::SchedulerMainThreadController* controller);

protected:    
    void onEntry(QEvent* event) override;

    void RepeatAction(TPTransition_t& transition) override;

    virtual void Timeout(TPTransition_t& transition);

    virtual void TimeInterval(qint32 secs);

    virtual void Enter(QEvent* event);

private:
    int m_CurrentTickCount;
    int m_TickTimerCount;
};

}
#endif // TIMEOUTSTATE_H
