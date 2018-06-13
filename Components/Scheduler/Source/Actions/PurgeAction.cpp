#include "Scheduler/Include/Actions/PurgeAction.h"
#include "Scheduler/Include/Session.h"
#include "Scheduler/Commands/Include/CmdALPurge.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "Scheduler/Include/SchedulerLogging.h"

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
            SchedulerLogging::getInstance().Log4DualRetort(mp_session->GetRetortID(), QString("Purge finished, result:%1").arg(retCode));
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
        SchedulerLogging::getInstance().Log4DualRetort(mp_session->GetRetortID(), QString("Start purge"));
        CmdALPurge* purgeCmd = new CmdALPurge(500, mp_session->GetRetortID());
        purgeCmd->SetPurgeTimes(5);
        mp_SchedulerCommandProcessor->pushCmd(purgeCmd);
        m_stateWaitResult = true;
    }
}

}
