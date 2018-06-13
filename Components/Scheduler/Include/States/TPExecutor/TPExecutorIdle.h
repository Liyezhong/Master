#ifndef TPEXECUTOR_IDLE_H
#define TPEXECUTOR_IDLE_H
#include "Scheduler/Include/States/StateBase.h"
#include "Scheduler/Include/SchedulerStateMachineStates.h"

namespace MsgClasses {
class CmdProgramAcknowledge;
}
namespace Scheduler{
namespace TPExecutorStates{
class Idle : public StateBase<ControlCommandType_t>
{
public:
    Idle(IEventHandler* pHandler, SchedulerMainThreadController* controller);

    void onEntry(QEvent* event) override;
    bool HandleEvent(TPEventArgs<ControlCommandType_t>* event, TPTransition_t& pTransition) override;
    void onExit(QEvent* event) override;

private:
    Global::tRefType ref;
    MsgClasses::CmdProgramAcknowledge* commandStartPtr;
};
}
}
#endif // TPEXECUTOR_IDLE_H
