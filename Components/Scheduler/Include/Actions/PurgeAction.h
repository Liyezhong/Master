#ifndef PURGEACTION_H
#define PURGEACTION_H

#include "Scheduler/Include/Actions/IAction.h"
#include "Scheduler/Include/Actions/ActionHelper.h"

namespace Scheduler {

class SchedulerCommandProcessorBase;


class PurgeAction: virtual public IAction, virtual public ActionHelper
{
public:
    PurgeAction(SchedulerCommandProcessorBase* commandProcessor, Session* session);
    virtual ~PurgeAction();

    virtual void Execute(const QString& cmdName, DeviceControl::ReturnCode_t retCode);
private:
    bool m_stateWaitResult;
};

}

#endif // PURGEACTION_H
