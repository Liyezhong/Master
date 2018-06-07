#include "Scheduler/Include/Actions/PurgeAction.h"
#include "Scheduler/Include/Session.h"
#include "Scheduler/Commands/Include/CmdALPurge.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"

using namespace DeviceControl;
namespace Scheduler{

PurgeAction::PurgeAction(SchedulerCommandProcessorBase* commandProcessor, Session* session):
    IAction(session),
    ActionHelper(commandProcessor),
    m_stateWaitResult(false)
{
    m_type = PURGE;
}

PurgeAction::~PurgeAction()
{

}

void PurgeAction::Execute(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    if (m_stateWaitResult)
    {
        if ("Scheduler::ALPurge" == cmdName)
        {
            qDebug()<<"************************ purge result:"<<retCode;
            if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                m_stateWaitResult = false;
                m_finished = true;
            }
            else
            {
                //retry
                return;
            }
        }
    }
    else
    {
        CmdALPurge* purgeCmd = new CmdALPurge(500, mp_session->GetRetortID());
        mp_SchedulerCommandProcessor->pushCmd(purgeCmd);
        m_stateWaitResult = true;
    }
}

}
