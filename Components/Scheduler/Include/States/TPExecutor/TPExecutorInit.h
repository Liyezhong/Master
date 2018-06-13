#ifndef TPEXECUTOR_INIT_H
#define TPEXECUTOR_INIT_H
#include "Scheduler/Include/States/StateBase.h"

namespace Scheduler{
namespace TPExecutorStates{
class Init: public StateBase<Scheduler::SchedulerCommandShPtr_t>
{
public:
    Init(IEventHandler* pHandler, SchedulerMainThreadController* controller);
    ~Init();

protected:
    void onEntry(QEvent* event) override;

    bool HandleEvent(TPEventArgs<Scheduler::SchedulerCommandShPtr_t>* event, TPTransition_t& pTransition) override;

    void RepeatAction(TPTransition_t& pTransition) override;

    Q_DISABLE_COPY(Init)

};

}
}
#endif // TPEXECUTOR_INIT_H
