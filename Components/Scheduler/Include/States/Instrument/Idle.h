#ifndef IDLE_H
#define IDLE_H
#include "Scheduler/Include/States/StateBase.h"


namespace Scheduler{
class Idle : public StateBase<Global::CommandShPtr_t>
{
public:
    Idle(IEventHandler* pHandler, Scheduler::SchedulerMainThreadController* controller);

    void onEntry(QEvent* event) override;
    bool HandleEvent(TPCmdEvent<Global::CommandShPtr_t>* event, TPTransition_t& pTransition) override;
};
}
#endif // IDLE_H
