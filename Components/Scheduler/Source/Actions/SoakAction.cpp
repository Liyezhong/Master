#include "Scheduler/Include/Actions/SoakAction.h"
#include "Scheduler/Include/SchedulerLogging.h"
#include "Scheduler/Include/Session.h"

namespace Scheduler{

SoakAction::SoakAction(SchedulerCommandProcessorBase* commandProcessor, Session* session)
    :IAction(session),
     ActionHelper(commandProcessor),
     m_soakStartTime(0)
{
    m_type = SOAKING;
}

SoakAction::~SoakAction()
{

}

void SoakAction::Execute(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    Q_UNUSED(cmdName);
    Q_UNUSED(retCode);

    qint64 now    = QDateTime::currentDateTime().toMSecsSinceEpoch();/*lint -e647 */
    if (0 == m_soakStartTime)
    {
        SchedulerLogging::getInstance().Log4DualRetort(mp_session->GetRetortID(), QString("Start soaking, druation seconds:%1").arg(m_durationSec));
        m_soakStartTime = now;
    }

    qint64 period = m_durationSec * 1000;
    if((now - m_soakStartTime ) > period)//Will finish Soaking
    {
        m_finished = true;
        SchedulerLogging::getInstance().Log4DualRetort(mp_session->GetRetortID(), QString("Soaking finished"));
    }
}

}
