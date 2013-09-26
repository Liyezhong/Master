#ifndef SCHEDULERMACHINE_H
#define SCHEDULERMACHINE_H

#include <QStateMachine>
#include "SchedulerMainThreadController.h"

namespace Scheduler{

typedef enum
{
    INIT_STATE,
    IDLE_STATE,
    ERROR_STATE,
    PAUSE_STATE,
    RUN_STATE,
    STATE_NOT_EXISTED
} SchedulerState_t;

class SchedulerMainThreadController;

class SchedulerMachine : public QObject
{
    Q_OBJECT
private:
    QStateMachine* mp_SchedulerMachine;
    QState* mp_InitState;
    QState* mp_IdleState;
    QState* mp_ErrorState;
    QState* mp_PauseState;
    QState* mp_RunState;

public:
    SchedulerMachine();
    ~SchedulerMachine();
    void Start();
    void SendSchedulerInitComplete();
    void SendRunSignal();
    void SendRunComplete();
    void SendPauseSignal();
    void SendRecoverToPause();
    void SendErrorSignal();

    SchedulerState_t GetCurrentState();
private slots:
    void OnStateChanged();
signals:
    void SchedulerInitComplete();
    void RunSignal();
    void RunComplete();
    void PauseSignal();
    void RecoverToPause();
    void ErrorSignal();
};
}

#endif // SCHEDULERMACHINE_H
