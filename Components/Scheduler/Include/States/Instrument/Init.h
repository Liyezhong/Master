#ifndef INIT_H
#define INIT_H
#include <QDebug>
#include "Scheduler/Include/States/StateBase.h"
#include "Scheduler/Include/ProgramSelfTest.h"

namespace Scheduler{

class Init: public StateBase<Scheduler::SchedulerCommandShPtr_t>
{
public:
    Init(IEventHandler* pHandler, SchedulerMainThreadController* controller);
    ~Init();

protected:
    void onEntry(QEvent* event) override;

    bool HandleEvent(TPCmdEvent<Scheduler::SchedulerCommandShPtr_t>* event, TPTransition_t& pTransition) override;

    void RepeatAction(TPTransition_t& pTransition) override;

    void onExit(QEvent* event) override;
    Q_DISABLE_COPY(Init)

signals:
    void OnSelfTestDone(bool);
public:
    CProgramSelfTest* m_pSelfTest;
};

}
#endif // INIT_H
