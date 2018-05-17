#include "Scheduler/Include/IEventHandler.h"
#include "Scheduler/Include/TPEvent.h"

namespace Scheduler{
IEventHandler::IEventHandler(const QString &name, EventDispatcher* pEventDispatcher)
    : m_Name(name),
      m_pEventDispatcher(pEventDispatcher),
      m_pStateMachine(nullptr)
{

}

IEventHandler::~IEventHandler()
{
    if(m_pStateMachine != nullptr)
    {
        foreach(auto state, m_pStateMachine->configuration())
        {
            if(state != nullptr)
            {
                delete state;
                state = nullptr;
            }
        }

        delete m_pStateMachine;
        m_pStateMachine = nullptr;
    }
}

void IEventHandler::Start()
{
}

void IEventHandler::Stop()
{    

}


void IEventHandler::RegisterEventFilter()
{

}

bool IEventHandler::HandleEvent(QEvent* event)
{    

    return false;
}

}
