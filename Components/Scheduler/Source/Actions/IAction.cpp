#include "Scheduler/Include/Actions/IAction.h"
#include "Scheduler/Include/Session.h"
#include <QDebug>


using namespace DeviceControl;
namespace Scheduler{
IAction::IAction(Session* session):
     mp_session(session),
     m_finished(false),
     m_reagentID(""),
     m_stationID("")
{

}

IAction::~IAction()
{

}

QString IAction::toString()
{
    QString out;
    out.append(QString("Action:%1").arg(GetActionName()));
    out.append(QString("\tReagentID:%1").arg(m_reagentID));
    out.append(QString("\tStationID:%1").arg(m_stationID));
    out.append(QString("\tStartTime:%1, EndTime:%2").arg(m_startTime.toString()).arg(m_endTime.toString()));
    out.append(QString("\tDuration:%1").arg(m_durationSec));

    return out;
}

}
