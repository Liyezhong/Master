#ifndef SCHEDULING_H
#define SCHEDULING_H
#include "Scheduler/Include/States/StateBase.h"

namespace Scheduler{

class Scheduling: public StateBase<Global::CommandShPtr_t>
{
public:
    Scheduling(IEventHandler* pHandler, SchedulerMainThreadController* controller);
    ~Scheduling();

protected:
    void onEntry(QEvent* event) override;

    bool HandleEvent(TPCmdEvent<Global::CommandShPtr_t>* event, TPTransition_t& pTransition) override;
    void RepeatAction(TPTransition_t& pTransition) override;

};
}
#endif // SCHEDULING_H
