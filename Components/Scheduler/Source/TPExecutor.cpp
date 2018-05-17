#include "Scheduler/Include/TPExecutor.h"
#include "Scheduler/Include/TPEvent.h"

namespace Scheduler {

TPExecutor::TPExecutor(const QString& name, EventDispatcher* pEventDispatcher)
    : IEventHandler(name, pEventDispatcher)
{
}

bool TPExecutor::HandleEvent(QEvent* event)
{
    auto tpsmEvent = dynamic_cast<TPSMEvent*>(event);
    if(tpsmEvent != nullptr && m_pStateMachine->isRunning())
    {
        qDebug() << tpsmEvent->toString();

        m_pStateMachine->postEvent(event);
    }

    return IEventHandler::HandleEvent(event);
}

void TPExecutor::CreateStateMachine()
{

}

}
