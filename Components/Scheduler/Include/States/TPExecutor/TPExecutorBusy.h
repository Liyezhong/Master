#ifndef TPEXECUTOR_BUSY_H
#define TPEXECUTOR_BUSY_H
#include "Scheduler/Include/States/StateBase.h"
#include "Scheduler/Include/SchedulerStateMachineStates.h"

namespace Scheduler{
namespace TPExecutorStates{
class Busy : public StateBase<ControlCommandType_t>
{
public:
    Busy(IEventHandler* pHandler, SchedulerMainThreadController* controller);

    void onEntry(QEvent* event) override;
    bool HandleEvent(TPEventArgs<ControlCommandType_t>* event, TPTransition_t& pTransition) override;
    void RepeatAction(TPTransition_t& pTranstion) override;
};
}
}
#endif // TPEXECUTOR_BUSY_H
