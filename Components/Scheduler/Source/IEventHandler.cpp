#include "Scheduler/Include/IEventHandler.h"
#include "Scheduler/Include/TPEvent.h"
#include "Scheduler/Include/States/StateBase.h"

namespace Scheduler{
IEventHandler::IEventHandler(const QString &name, EventDispatcher* pEventDispatcher)
    : m_pEventDispatcher(pEventDispatcher),
      m_pStateMachine(nullptr)
{
    setObjectName(name);
}

IEventHandler::~IEventHandler()
{
    if(m_pStateMachine != nullptr)
    {
        delete m_pStateMachine;
        m_pStateMachine = nullptr;
    }
}

void IEventHandler::Start()
{
    if(m_pStateMachine != nullptr && !m_pStateMachine->isRunning())
    {
        m_pStateMachine->start();
    }
}

void IEventHandler::Stop()
{
    if(m_pStateMachine != nullptr && m_pStateMachine->isRunning())
    {
        m_pStateMachine->stop();
    }
}


void IEventHandler::RegisterEventFilter()
{

}

bool IEventHandler::HandleEvent(QEvent* event)
{
    auto* tpEvent = dynamic_cast<TPEvent*>(event);
    if(tpEvent != nullptr && m_pStateMachine->isRunning())
    {
//        qDebug() << tpEvent->toString();
        if(tpEvent->Value() != TPTransition_t::Timeout)
        {
            qDebug() << "Handle external request/response";
            auto* pCurrent = dynamic_cast<IState*>(*m_pStateMachine->configuration().begin());
            return pCurrent->HandleEvent(tpEvent);
        }
        else
        {
//            qDebug() << "Broadcast timer tick event";
            m_pStateMachine->postEvent(event);
        }
    }
    return true;
}

}
