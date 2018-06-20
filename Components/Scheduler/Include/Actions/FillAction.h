#ifndef FILLACTION_H
#define FILLACTION_H

#include "Scheduler/Include/Actions/IAction.h"
#include "Scheduler/Include/Actions/ActionHelper.h"

namespace Scheduler {

class SchedulerCommandProcessorBase;

typedef enum
{
    STATE_FILLING_UNDEFIN = 0,
    STATE_FILLING_RVROD_HEATING,
    STATE_FILLING_LEVELSENSOR_HEATING,
    STATE_FILLING
}FillState_t;

class FillAction: public IAction, public ActionHelper
{
public:
    FillAction(SchedulerCommandProcessorBase* commandProcessor, Session* session);
    virtual ~FillAction();

    virtual void Execute(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

private:
    FillState_t m_currentState;
    bool m_stateWaitResult;
};

}

#endif // FILLACTION_H
