#ifndef SCHEDULERMACHINE_H
#define SCHEDULERMACHINE_H

#include <QObject>
#include <QStateMachine>
#include "SchedulerMainThreadController.h"

namespace Scheduler{

class SchedulerMainThreadController;

class SchedulerMachine : public QObject
{
    Q_OBJECT
private:
    QStateMachine* mp_SchedulerMachine;
    QState* mp_InitState;
    QState* mp_IdleState;
    QState* mp_ErrorState;
    QState* mp_RunState;
public:
    SchedulerMachine(SchedulerMainThreadController* schedulerMainController);
    ~SchedulerMachine();

signals:
    void SchedulerInitComplete();
    void RunSignal();
    void RunComplete();
    void ErrorSignal();
};
}

#endif // SCHEDULERMACHINE_H
