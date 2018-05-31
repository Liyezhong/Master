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
class Session;

class InstrumentManager: public IEventHandler
{
    friend class TestInstrumentManager;
public:
    static InstrumentManager* Create(EventDispatcher* pParent, const QList<QString>& config,
                                     SchedulerMainThreadController* pController = nullptr, SessionManager* pSessionManager = nullptr);

    ~InstrumentManager();

    void Initialize();

    TPExecutor* GetTPExecutor(const QString& Id);

    void CreateStateMachine() override;

    QList<Session*>&  GetActiveSession();

    int CreateSession(const QString& retortId, const QString& protocolId);

    void StartProtocol(const QString& retortId);

    SchedulerMainThreadController* Controller(){return m_pController;}

private:
    InstrumentManager(const QString& name, EventDispatcher* pParent, const QList<QString>& config,
                      SchedulerMainThreadController* pController = nullptr, SessionManager* pSessionManager = nullptr);

    Q_DISABLE_COPY(InstrumentManager)


private:
    QHash<QString, QSharedPointer<TPExecutor>> m_TPExecutorList;

    SchedulerMainThreadController* m_pController;
    SessionManager* m_pSessionManager;

    Instrument::Init* m_pInit;
    Instrument::Idle* m_pIdle;
    Instrument::Busy* m_pBusy;
    Instrument::Scheduling* m_pScheduling;

    static InstrumentManager* m_pInstance;
    QList<QString> m_pConfig;
    QList<Session*> m_SessionList;
};

}

#endif // INSTRUMENTMANAGER_H
