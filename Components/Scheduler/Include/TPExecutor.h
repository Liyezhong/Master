#ifndef TPEXECUTOR_H
#define TPEXECUTOR_H

#include <QObject>
#include "Scheduler/Include/IEventHandler.h"
#include "Scheduler/Include/States/TPExecutor/TPExecutorInit.h"
#include "Scheduler/Include/States/TPExecutor/TPExecutorIdle.h"
#include "Scheduler/Include/States/TPExecutor/TPExecutorExecuting.h"
#include "Scheduler/Include/States/TPExecutor/TPExecutorSoaking.h"
#include "Scheduler/Include/States/TPExecutor/TPExecutorNextAction.h"
#include "Scheduler/Include/States/TPExecutor/TPExecutorBusy.h"

namespace Scheduler{

class TPExecutor: public IEventHandler
{
    friend class TestInstrumentManager;
public:
    TPExecutor(const QString& name, EventDispatcher* pEventDispatcher, SchedulerMainThreadController* pController);

    ~TPExecutor();

    bool HandleEvent(QEvent* event) override;

    void CreateStateMachine() override;
    Q_DISABLE_COPY(TPExecutor)

private:
    SchedulerMainThreadController* m_pController;
    TPExecutorStates::Init* m_pInit;
    TPExecutorStates::Idle* m_pIdle;
    TPExecutorStates::Busy* m_pBusy;
    TPExecutorStates::NextAction* m_pNextAction;
    TPExecutorStates::Soaking* m_pSoaking;
    TPExecutorStates::Executing* m_pExecuting;
};
}

#endif // TPEXECUTOR_H
