#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <QObject>
#include <QMutex>
#include <QTimer>
#include <QQueue>
#include <QSharedPointer>
#include <QStateMachine>

namespace Scheduler{
class TPEvent;
class IEventHandler;
class TestInstrumentManager;
struct TPSMEvent;

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

    void IncomingEvent(TPSMEvent* event);

protected slots:
    void OnTickTimer();

private:
    QList<QSharedPointer<IEventHandler>> m_EventHandlerList;
    QQueue<TPSMEvent*> m_EventQueue;
    QMutex m_EventDispatcherMutex;
    QMutex m_EventQueueMutex;
    QTimer m_Timer;
    TPSMEvent* m_pDummyEvent;
};
}

#endif // EVENTDISPATCHER_H
