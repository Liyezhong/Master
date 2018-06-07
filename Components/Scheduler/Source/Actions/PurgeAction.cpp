#include "Scheduler/Include/Actions/PurgeAction.h"

using namespace DeviceControl;
namespace Scheduler{

PurgeAction::PurgeAction(SchedulerCommandProcessorBase* commandProcessor, Session* session):
    IAction(session),
    ActionHelper(commandProcessor),
    m_stateWaitResult(false)
{

}

PurgeAction::~PurgeAction()
{

}

void PurgeAction::Execute(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{

}

}
