#include "Scheduler/Include/EventDispatcher.h"
#include "Global/Include/Utils.h"
#include "Scheduler/Include/IEventHandler.h"
#include "Scheduler/Include/TPEvent.h"
#include "Scheduler/Include/TPTransition.h"

namespace Scheduler {

EventDispatcher::EventDispatcher()
    : m_Timer(this)
{
    m_EventHandlerList.clear();
    m_pDummyEvent = new TPEvent(TPTransition_t::Self, nullptr);
}

EventDispatcher::~EventDispatcher()
{
    if(m_pDummyEvent != nullptr )
    {
        delete m_pDummyEvent;
        m_pDummyEvent = nullptr;

    }
}

void EventDispatcher::Register(QSharedPointer<IEventHandler> pEventHandler)
{
    if(pEventHandler.isNull())
        return;

    QMutexLocker lock(&m_EventDispatcherMutex);
    if(!m_EventHandlerList.contains(pEventHandler))
    {
        m_EventHandlerList.push_back(pEventHandler);
    }
}

void EventDispatcher::Remove(QSharedPointer<IEventHandler> pEventHandler)
{
    QMutexLocker lock(&m_EventDispatcherMutex);
    m_EventHandlerList.removeOne(pEventHandler);

    Start();
}

void EventDispatcher::Start()
{
    CONNECTSIGNALSLOT(&m_Timer, timeout(), this, OnTickTimer());
    m_Timer.setInterval(TICKTIMERINTERVAL);
    m_Timer.start();
}


void EventDispatcher::Stop()
{
    m_Timer.stop();
}

void EventDispatcher::IncomingEvent(TPEvent* event)
{
    if(event != nullptr)
    {        
        if(event->EventArgs() == nullptr)
        {
            qDebug() << "Invalid event args";
            return;
        }
    }
    qDebug() << "IncomingEvent: " << event->EventArgs()->toString();
    QMutexLocker lock(&m_EventQueueMutex);
    m_EventQueue.push_back(event);
}

bool EventDispatcher::RemovePendingEvent(TPEvent *event)
{
    QMutexLocker lock(&m_EventQueueMutex);
    if(!m_EventQueue.contains(event))
    {
        return false;
    }

    auto* data = dynamic_cast<TPEventArgsBase*>(event->EventArgs());
    if(data == nullptr)
    {
        qDebug() << "Garbage detected!!!";
        m_EventQueue.removeOne(event);
        return true;
    }
    else if (data->Sender().isEmpty() || data->Handled())
    {
        qDebug() << "Handled event removed";
        m_EventQueue.removeOne(event);
        delete event;
        event = nullptr;
        return true;
    }

    lock.unlock();

    return false;
}

void EventDispatcher::OnTickTimer()
{
    qDebug() << QDateTime::currentDateTime().toString() << " OnTickTimer";

    foreach(auto handler, m_EventHandlerList)
    {
        handler->HandleEvent(new TPEvent(TPTransition_t::Timeout, nullptr));
    }


    if(!m_EventQueue.isEmpty())
    {        
        auto evt = m_EventQueue.first();

        if(RemovePendingEvent(evt))
        {
            return;
        }

        foreach(auto handler, m_EventHandlerList)
        {
            if(handler->objectName() == evt->EventArgs()->Sender())
            {
                if(handler->HandleEvent(evt))
                {
                    ;
                }
            }
        }
    }
}

}
