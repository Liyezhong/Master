#ifndef IDLE_H
#define IDLE_H
#include "Scheduler/Include/States/StateBase.h"

namespace MsgClasses{
class CmdProgramSelected;
class CmdProgramSelectedReply;
}
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

    void onExit(QEvent* event) override;

private:
    void ProgramSelectedReply(Global::tRefType ref, const MsgClasses::CmdProgramSelected& cmd, unsigned int timeProposed);

protected:
    InstrumentManager* m_pManager;
    int start;
    Global::tRefType ref;
    MsgClasses::CmdProgramSelectedReply* commandPtr;
};
}
}
#endif // IDLE_H
