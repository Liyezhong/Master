#ifndef BUSY_H
#define BUSY_H
#include "Scheduler/Include/States/StateBase.h"

namespace Scheduler{

class Busy: public StateBase<Global::CommandShPtr_t>
{
public:
    Busy(IEventHandler* pHandler, SchedulerMainThreadController* controller);
    ~Busy();

protected:
    void onEntry(QEvent* event) override;

    bool HandleEvent(TPCmdEvent<Global::CommandShPtr_t>* event, TPTransition_t& pTransition) override;

    void RepeatAction(TPTransition_t& pTransition) override;

};
}

#endif // BUSY_H
