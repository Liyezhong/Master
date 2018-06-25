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



int IAction::GetActionIndex()
{
    for (int i = 0; i < mp_session->GetActionList().size(); i++) {
        IAction *action = mp_session->GetActionList()[i].data();
        if (action == this)
            return i;
    }

    return -1;
}

QTime IAction::GetStartTime()
{
   QTime startTime = mp_session->GetProgramStartTime();
   QTime t = startTime;
   for (int i = 0; i < mp_session->GetActionList().size(); i++) {
       IAction *action = mp_session->GetActionList()[i].data();
       if (action == this)
           break;
       else
            t = t.addSecs(action->GetDuration());
   }

   return t;
}

QTime IAction::GetEndTime()
{
   return GetStartTime().addSecs(this->GetDuration());
}

QTime IAction::GetStepStartTime()
{
    QTime startTime = mp_session->GetProgramStartTime();
    QTime t = startTime;

    int index = GetActionIndex();
    if (index == -1)
        return startTime;

    for (int i = index - 1; i >= 0; i--) {
        IAction *action = mp_session->GetActionList()[i].data();
        if (action->m_stationID != this->m_stationID)
            return action->GetEndTime();
    }

    return mp_session->GetProgramStartTime();
}

QTime IAction::GetStepEndTime()
{
    QTime startTime = mp_session->GetProgramStartTime();
    QTime t = startTime;

    int index = GetActionIndex();
    if (index == -1)
        return startTime;

    for (int i = index + 1; i < mp_session->GetActionList().size(); i++) {
        IAction *action = mp_session->GetActionList()[i].data();
        if (action->m_stationID != this->m_stationID)
            return action->GetStartTime();
    }

    return mp_session->GetProgramEndTime();
}
}
