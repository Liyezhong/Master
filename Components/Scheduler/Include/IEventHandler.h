#ifndef IEVENTHANDLER_H
#define IEVENTHANDLER_H
#include <QString>
#include <QStateMachine>
#include "EventDispatcher.h"

namespace Scheduler{
class TestInstrumentManager;
class TPEvent;
class IEventHandler: public QObject
{
    Q_OBJECT
    friend class TestInstrumentManager;
public:
    IEventHandler(const QString& name, EventDispatcher* pEventDispatcher);

    virtual ~IEventHandler();

    virtual void Start();

    virtual void Stop();

    virtual bool HandleEvent(QEvent* event);

    virtual void RegisterEventFilter();

    virtual void CreateStateMachine() = 0;

protected:
    EventDispatcher* m_pEventDispatcher;
    QStateMachine* m_pStateMachine;

private:
    Q_DISABLE_COPY(IEventHandler)
};
}
#endif // IEVENTHANDLER_H
