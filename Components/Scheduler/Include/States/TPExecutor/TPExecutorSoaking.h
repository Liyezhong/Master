#ifndef TPEXECUTOR_SOAKING_H
#define TPEXECUTOR_SOAKING_H
#include "Scheduler/Include/States/StateBase.h"

namespace Scheduler{
namespace TPExecutorStates{
class Soaking: public StateBase<Global::CommandShPtr_t>
{
public:
    Soaking(IEventHandler* pHandler, SchedulerMainThreadController* controller);
    ~Soaking();

protected:
    void onEntry(QEvent* event) override;

    bool HandleEvent(TPEventArgs<Global::CommandShPtr_t>* event, TPTransition_t& pTransition) override;
    void RepeatAction(TPTransition_t& pTransition) override;

};
}
}
#endif // TPEXECUTOR_SOAKING_H
