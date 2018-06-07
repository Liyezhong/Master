#ifndef SOAKACTION_H
#define SOAKACTION_H

#include "Scheduler/Include/Actions/IAction.h"
#include "Scheduler/Include/Actions/ActionHelper.h"

namespace Scheduler {

class SchedulerCommandProcessorBase;

class SoakAction: public IAction, public ActionHelper
{
public:
    SoakAction(SchedulerCommandProcessorBase* commandProcessor, Session* session);
    virtual ~SoakAction();

    virtual void Execute(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

private:
    qint64 m_soakStartTime;

};

}

#endif // SOAKACTION_H
