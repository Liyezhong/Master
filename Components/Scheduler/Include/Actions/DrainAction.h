#ifndef DRAINACTION_H
#define DRAINACTION_H

#include "Scheduler/Include/Actions/IAction.h"
#include "Scheduler/Include/Actions/ActionHelper.h"

namespace Scheduler {

class SchedulerCommandProcessorBase;

typedef enum
{
    STATE_UNDEFIN = 0,
    STATE_DRAINING_RVROD,
    STATE_DRAINING
}DrainState_t;

class DrainAction: public IAction, public ActionHelper
{
public:
    DrainAction(SchedulerCommandProcessorBase* commandProcessor, Session* session);
    virtual ~DrainAction();

    virtual void Execute(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

private:
    DrainState_t m_currentState;
    bool m_stateWaitResult;
};

}

#endif // DRAINACTION_H
