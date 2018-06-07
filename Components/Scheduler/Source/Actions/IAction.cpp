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

}
