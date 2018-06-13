#ifndef TPEXECUTOR_EXECUTING_H
#define TPEXECUTOR_EXECUTING_H
#include "Scheduler/Include/States/TimeoutState.h"

namespace Scheduler{
namespace TPExecutorStates{
class Executing: public Scheduler::TimeoutState<Scheduler::SchedulerCommandShPtr_t>
{
public:
    Executing(IEventHandler* pHandler, SchedulerMainThreadController* controller);
    ~Executing();

protected:
    void Enter(QEvent* event) override;

    bool HandleEvent(TPEventArgs<Scheduler::SchedulerCommandShPtr_t>* event, TPTransition_t& pTransition) override;

    void Timeout(TPTransition_t& pTransition) override;

    void RepeatAction(TPTransition_t& pTransition) override;

    void onExit(QEvent* event) override;
};
}
}
#endif // TPEXECUTOR_EXECUTING_H
