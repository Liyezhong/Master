#ifndef TPEXECUTOR_EXECUTING_H
#define TPEXECUTOR_EXECUTING_H
#include "Scheduler/Include/States/StateBase.h"

namespace Scheduler{
namespace TPExecutorStates{
class Executing: public StateBase<Global::CommandShPtr_t>
{
public:
    Executing(IEventHandler* pHandler, SchedulerMainThreadController* controller);
    ~Executing();

protected:
    void onEntry(QEvent* event) override;

    bool HandleEvent(TPEventArgs<Global::CommandShPtr_t>* event, TPTransition_t& pTransition) override;

    void RepeatAction(TPTransition_t& pTransition) override;

};
}
}
#endif // TPEXECUTOR_EXECUTING_H
