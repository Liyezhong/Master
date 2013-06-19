#ifndef PROGRAMSTEPSTATEMACHINE_H
#define PROGRAMSTEPSTATEMACHINE_H


#include <QObject>
#include <QStateMachine>
#include <QFinalState>

namespace Scheduler{

typedef enum
{
    PSSM_INIT = 0,
    PSSM_READY_TO_HEAT_LEVEL_SENSOR_S1 = 1,
    PSSM_READY_TO_HEAT_LEVEL_SENSOR_S2 = 2,
    PSSM_READY_TO_TUBE_BEFORE = 3,
    PSSM_READY_TO_FILL = 4,
    PSSM_READY_TO_SEAL = 5,
    PSSM_SOAK = 6,
    PSSM_READY_TO_TUBE_AFTER = 7,
    PSSM_READY_TO_DRAIN = 8,
    PSSM_FINISH = 9,
    PSSM_PAUSE = 10,
    PSSM_PAUSE_DRAIN = 11,
    PSSM_ABORTING = 12,
    PSSM_ABORTED = 13,
    PSSM_ERROR = 14,
    PSSM_UNDEF
} ProgramStepStateMachine_t;


class ProgramStepStateMachine : public QObject
{
    Q_OBJECT
public:
    ProgramStepStateMachine();
    ~ProgramStepStateMachine();
    ProgramStepStateMachine_t GetCurrentState();
    ProgramStepStateMachine_t GetPreviousState();

    void Start();
    void Stop();
    void NotifyTempsReady();
    void NotifyLevelSensorTempS1Ready();
    void NotifyLevelSensorTempS2Ready();
    void NotifyHitTubeBefore();
    void NotifyFillFinished();
    void NotifyHitSeal();
    void NotifySoakFinished();
    void NotifyHitTubeAfter();
    void NotifyDrainFinished();
    void NotifyError();
    void NotifyPause(ProgramStepStateMachine_t PreviousState);
    void NotifyResume();
    void NotifyResumeDrain();
    void NotifyAbort();

signals:
    void TempsReady();
    void LevelSensorTempS1Ready();
    void LevelSensorTempS2Ready();
    void HitTubeBefore();
    void FillFinished();
    void HitSeal();
    void SoakFinished();
    void HitTubeAfter();
    void DrainFinished();
    void Error();
    void Pause();
    void ResumeToInit();
    void ResumeToHeatLevelSensorS1();
    void ResumeToHeatLevelSensorS2();
    void ResumeToReadyToFill();
    void ResumeToSoak();
    void ResumeToReadyToTubeAfter();
    void Abort();

    void OnInit();
    void OnHeatLevelSensorTempS1();
    void OnHeatLevelSensorTempS2();
    void OnMoveToTubeBefore();
    void OnMoveToTubeAfter();
    void OnMoveToSeal();
    void OnFill();
    void OnStopFill();
    void OnSoak();
    void OnDrain();
    void OnStopDrain();
    void OnAborting();
    void OnAborted();
    void OnPause();
    void OnPauseDrain();

private slots:
    void OnStateChanged();

private:
    QStateMachine* mp_ProgramStepStateMachine;
    QState* mp_PssmInit;
    QState* mp_PssmReadyToHeatLevelSensorS1;
    QState* mp_PssmReadyToHeatLevelSensorS2;
    QState* mp_PssmReadyToTubeBefore;
    QState* mp_PssmReadyToTubeAfter;
    QState* mp_PssmReadyToSeal;
    QState* mp_PssmReadyToFill;
    QState* mp_PssmReadyToDrain;
    QState* mp_PssmSoak;
    QState* mp_PssmPause;
    QState* mp_PssmPauseDrain;
    QState* mp_PssmAborting;
    QState* mp_PssmAborted;
    QFinalState* mp_PssmFinish;
    QState* mp_PssmError;
    ProgramStepStateMachine_t m_PreviousState;

};











}

#endif // PROGRAMSTEPSTATEMACHINE_H
