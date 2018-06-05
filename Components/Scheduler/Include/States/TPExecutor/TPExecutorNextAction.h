#ifndef TPEXECUTOR_NEXTACTION_H
#define TPEXECUTOR_NEXTACTION_H
#include "Scheduler/Include/States/StateBase.h"

namespace Scheduler{
class Session;
class TPExecutor;
namespace TPExecutorStates{

class NextAction: public StateBase<Global::CommandShPtr_t>
{
public:
    NextAction(IEventHandler* pHandler, SchedulerMainThreadController* controller);
    ~NextAction();

protected:
    void onEntry(QEvent* event) override;

    bool HandleEvent(TPEventArgs<Global::CommandShPtr_t>* event, TPTransition_t& pTransition) override;
    void RepeatAction(TPTransition_t& pTransition) override;

private:
    int actionNum;
    Session* m_pSession;
    TPExecutor* m_pExecutor;
};
}
}
#endif // TPEXECUTOR_NEXTACTION_H
