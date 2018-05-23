#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <QObject>
#include <QMutex>
#include <QTimer>
#include <QQueue>
#include <QSharedPointer>
#include <QStateMachine>

#define TICKTIMERINTERVAL 500
namespace Scheduler{
class TPEvent;
class IEventHandler;
class TestInstrumentManager;
class TPEvent;


class EventDispatcher: public QObject
{
    friend class TestInstrumentManager;
    Q_OBJECT
public:
    EventDispatcher();
    ~EventDispatcher();

    void Register(QSharedPointer<IEventHandler> pEventHandler);
    void Remove(QSharedPointer<IEventHandler> pEventHandler);

    void Start();

    void Stop();

    void IncomingEvent(TPEvent* event);

    bool RemovePendingEvent(TPEvent* event);

protected slots:
    void OnTickTimer();   

private:
    QList<QSharedPointer<IEventHandler>> m_EventHandlerList;
    QQueue<TPEvent*> m_EventQueue;
    QMutex m_EventDispatcherMutex;
    QMutex m_EventQueueMutex;
    QTimer m_Timer;
    TPEvent* m_pDummyEvent;

};
}

#endif // EVENTDISPATCHER_H
