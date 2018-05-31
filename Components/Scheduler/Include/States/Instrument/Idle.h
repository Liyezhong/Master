#ifndef IDLE_H
#define IDLE_H
#include "Scheduler/Include/States/StateBase.h"


namespace Scheduler{
class InstrumentManager;
namespace Instrument{
class Idle : public StateBase<Global::CommandShPtr_t>
{
public:
    Idle(IEventHandler* pHandler, Scheduler::SchedulerMainThreadController* controller);

    void onEntry(QEvent* event) override;

    bool HandleEvent(TPEventArgs<Global::CommandShPtr_t>* event, TPTransition_t& pTransition) override;

    void RepeatAction(TPTransition_t& pTransition) override;    

private:
    InstrumentManager* m_pManager;
    int start;
};
}
}
#endif // IDLE_H
