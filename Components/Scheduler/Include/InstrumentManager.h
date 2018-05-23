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
class SessionManager;

class InstrumentManager: public IEventHandler
{
    friend class TestInstrumentManager;
public:
    InstrumentManager(const QString& name, EventDispatcher* pParent,
                      SchedulerMainThreadController* pController = nullptr, SessionManager* pSessionManager = nullptr);

    ~InstrumentManager();

    void Initialize(QList<QString>);

    TPExecutor* GetTPExecutor(const QString& Id);

    void CreateStateMachine() override;

    int CreateSession(const QString& retortId, const QString& protocolId);

    void StartProtocol(const QString& retortId);

    SchedulerMainThreadController* Controller(){return m_pController;}

private:
    Q_DISABLE_COPY(InstrumentManager)


private:
    QHash<QString, QSharedPointer<TPExecutor>> m_TPExecutorList;

    SchedulerMainThreadController* m_pController;
    SessionManager* m_pSessionManager;

    Instrument::Init* m_pInit;
    Instrument::Idle* m_pIdle;
    Instrument::Busy* m_pBusy;
    Instrument::Scheduling* m_pScheduling;

};

}

#endif // INSTRUMENTMANAGER_H
