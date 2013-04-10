#ifndef PROGRAMSTEPSTATEMACHINE_H
#define PROGRAMSTEPSTATEMACHINE_H


#include <QObject>
#include <QStateMachine>
#include <QFinalState>

namespace Scheduler{

typedef enum
{
    PSSM_INIT,
    PSSM_READY_TO_HEAT_LEVEL_SENSOR_S1,
    PSSM_READY_TO_HEAT_LEVEL_SENSOR_S2,
    PSSM_READY_TO_TUBE_BEFORE,
    PSSM_READY_TO_FILL,
    PSSM_READY_TO_SEAL,
    PSSM_SOAK,
    PSSM_READY_TO_TUBE_AFTER,
    PSSM_READY_TO_DRAIN,
    PSSM_FINISH,
    PSSM_ERROR
} ProgramStepStateMachine_t;


class ProgramStepStateMachine : public QObject
{
    Q_OBJECT
public:
    ProgramStepStateMachine();
    ~ProgramStepStateMachine();
    ProgramStepStateMachine_t GetCurrentState();
    void Start();
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

    void OnInit();
    void OnHeatLevelSensorTempS1();
    void OnHeatLevelSensorTempS2();
    void OnMoveToTubeBefore();
    void OnMoveToTubeAfter();
    void OnMoveToSeal();
    void OnFill();
    void OnSoak();
    void OnDrain();

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
    QFinalState* mp_PssmFinish;
    QState* mp_PssmError;

};











}

#endif // PROGRAMSTEPSTATEMACHINE_H
