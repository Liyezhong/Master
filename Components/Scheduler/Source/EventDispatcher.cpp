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
    m_pDummyEvent = new TPSMEvent(TPTransition_t::Self, nullptr);
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
    m_Timer.setInterval(500);
    m_Timer.start();
}


void EventDispatcher::Stop()
{
    m_Timer.stop();
}

void EventDispatcher::IncomingEvent(TPSMEvent* event)
{
    TPInternalEventBase* eventArgs = nullptr;
    if(event != nullptr)
    {
        eventArgs = static_cast<TPInternalEventBase*>(event->m_pData);
        if(eventArgs == nullptr)
            return;
    }

    QMutexLocker lock(&m_EventQueueMutex);
    m_EventQueue.enqueue(event);
}

void EventDispatcher::OnTickTimer()
{

    qDebug() << QDateTime::currentDateTime().toString() << " OnTickTimer";


    if(!m_EventQueue.isEmpty())
    {
        QMutexLocker lock(&m_EventQueueMutex);
        auto evt = m_EventQueue.dequeue();
        auto eventArgs = dynamic_cast<TPSMEvent*>(evt);
        if(eventArgs != nullptr && eventArgs->m_pData->Sender().isEmpty())
        {
            qDebug() << "EventDispatcher: sender is empty";
            return;
        }

        foreach(auto handler, m_EventHandlerList)
        {
            if(handler->Name() == eventArgs->m_pData->Sender())
            {
                qDebug() << " OnTickTimer Active";
                 handler->HandleEvent(evt);
                return;
            }
        }
    }

    foreach(auto handler, m_EventHandlerList)
    {
        qDebug() << " OnTickTimer Dummy";
        handler->HandleEvent(new TPSMEvent(TPTransition_t::Self, nullptr));
    }

}

}
