#ifndef INSTRUMENTMANAGER_H
#define INSTRUMENTMANAGER_H

#include <QObject>
#include <QHash>
#include <QSharedPointer>
#include "Scheduler/Include/IEventHandler.h"
#include "Scheduler/Include/States/Instrument/Init.h"
#include "Scheduler/Include/States/Instrument/Idle.h"
#include "Scheduler/Include/States/Instrument/Busy.h"
#include "Scheduler/Include/States/Instrument/Scheduling.h"

namespace Scheduler{

class TestInstrumentManager;
class TPExecutor;
class SchedulerMainThreadController;


class InstrumentManager: public IEventHandler
{
    friend class TestInstrumentManager;
public:
    InstrumentManager(const QString& name, EventDispatcher* pParent, SchedulerMainThreadController* pController = nullptr);

    void Initialize(QList<QString>);

    TPExecutor* GetTPExecutor(const QString& Id) {return m_TPExecutorList.find(Id).value().data();}

    void Start() override;

    void Stop() override;

    void CreateStateMachine() override;

    bool HandleEvent(QEvent* event) override;

private:
    Q_DISABLE_COPY(InstrumentManager)


private:
    QHash<QString, QSharedPointer<TPExecutor>> m_TPExecutorList;

    SchedulerMainThreadController* m_pController;

    Init* m_pInit;
    Idle* m_pIdle;
    Busy* m_pBusy;
    Scheduling* m_pScheduling;

};

}

#endif // INSTRUMENTMANAGER_H
