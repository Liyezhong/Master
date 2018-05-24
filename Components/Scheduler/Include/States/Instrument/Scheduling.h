#ifndef SCHEDULING_H
#define SCHEDULING_H
#include "Scheduler/Include/States/StateBase.h"

namespace MsgClasses{
class CmdProgramSelected;
}
namespace Scheduler{
namespace Instrument{

class Scheduling: public StateBase<Global::CommandShPtr_t>
{
public:
    Scheduling(IEventHandler* pHandler, SchedulerMainThreadController* controller);
    ~Scheduling();

protected:
    void onEntry(QEvent* event) override;

    bool HandleEvent(TPEventArgs<Global::CommandShPtr_t>* event, TPTransition_t& pTransition) override;
    void RepeatAction(TPTransition_t& pTransition) override;

private:
    void ProgramSelectedReply(Global::tRefType ref, const MsgClasses::CmdProgramSelected& cmd, unsigned int timeProposed);
};

}
}
#endif // SCHEDULING_H
