#ifndef PROGRAMSTEPSTATEMACHINE_H
#define PROGRAMSTEPSTATEMACHINE_H


#include <QStateMachine>
#include <QFinalState>
#include "SchedulerStateMachineStates.h"
namespace Scheduler{
#if 0
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
    PSSM_STEP_FINISH = 9,
    PSSM_PROGRAM_FINISH = 10,
    PSSM_PAUSE = 11,
    PSSM_PAUSE_DRAIN = 12,
    PSSM_ABORTING = 13,
    PSSM_ABORTED = 14,
    PSSM_ERROR = 15,
    PSSM_ST = 19,
    PSSM_ST_INIT = 20,
    PSSM_ST_TEMP_CHECKING = 21,
    PSSM_ST_CURRENT_CHECKING = 22,
    PSSM_ST_VOLTAGE_CHECKING = 23,
    PSSM_ST_RV_POSITION_CHECKING = 24,
    PSSM_ST_PRESSURE_CHECKING = 25,
    PSSM_ST_SEALING_CHECKING = 26,
    PSSM_ST_STATION_CHECKING = 27,
    PSSM_ST_STATION_CHECK_FINISH = 28,
    PSSM_ST_DONE = 29,
    PSSM_UNDEF
} ProgramStepStateMachine_t;
#endif

class CProgramStepStateMachine : public QObject
{
    Q_OBJECT
public:
#if 0 //delete later
    ProgramStepStateMachine();
#endif
    CProgramStepStateMachine(QState* pParentState, QState* pErrorState);
    ~CProgramStepStateMachine();
    SchedulerStateMachine_t GetCurrentState(QSet<QAbstractState*> statesList);
    void SetPreviousState(SchedulerStateMachine_t state);
    SchedulerStateMachine_t GetPreviousState();

    void Start();
    void Stop();

#if 0 //delete later
    void NotifyStInitOK();
    void NotifyStTempOK();
    void NotifyStCurrentOK();
    void NotifyStVoltageOK();
    void NotifyStRVPositionOK();
    void NotifyStPressureOK();
    void NotifyStSealingOK();
    void NotifyStGetStationcheckResult();
    void NotifyStStationLeft();
    void NotifyStStationOK();
    void NotifyStDone();

    void NotifyTempsReady();
    void NotifyLevelSensorTempS1Ready();
    void NotifyLevelSensorTempS2Ready();
    void NotifyHitTubeBefore();
    void NotifyFillFinished();
    void NotifyHitSeal();
    void NotifySoakFinished();
    void NotifyHitTubeAfter();
    void NotifyDrainFinished();
    void NotifyStepFinished();
    void NotifyProgramFinished();
    void NotifyError();
    void NotifyPause(ProgramStepStateMachine_t PreviousState);
    void NotifyResume();
    void NotifyResumeDrain();
    void NotifyAbort();
    void NotifyResumeToSelftest();
#endif

signals:
    void StInitOK();
    void StTempOK();
    void StCurrentOK();
    void StVoltageOK();
    void StRVPositionOK();
    void StPressureOK();
    void StSealingOK();
    void StGetStationcheckResult();
    void StStationLeft();
    void StStationOK();
    void StDone();
    void TempsReady();
    void LevelSensorTempS1Ready();
    void LevelSensorTempS2Ready();
    void HitTubeBefore();
    void FillFinished();
    void HitSeal();
    void SoakFinished();
    void HitTubeAfter();
    void DrainFinished();
    void StepFinished();
    void ProgramFinished();
    void Error();
    void Pause();
    void ResumeToSelftest();
    void ResumeToInit();
    void ResumeToHeatLevelSensorS1();
    void ResumeToHeatLevelSensorS2();
    void ResumeToReadyToFill();
    void ResumeToSoak();
    void ResumeToStepFinished();
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

    void OnStateExited();

private slots:
    void OnStateChanged();

private:
    QState* mp_PssmSelfTest;
    QState* mp_PssmStInit;
    QState* mp_PssmStTempChecking;
    QState* mp_PssmStCurrentChecking;
    QState* mp_PssmStVoltageChecking;
    QState* mp_PssmStRVPositionChecking;
    QState* mp_PssmStPressureChecking;
    QState* mp_PssmStSealingChecking;
    QState* mp_PssmStStationChecking;
    QState* mp_PssmStStationCheckFinish;
    QState* mp_PssmStDone;
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
    QState* mp_PssmStepFinish;
    QFinalState* mp_PssmProgramFinish;
    QState* mp_PssmError;
    SchedulerStateMachine_t m_PreviousState;

};











}

#endif // PROGRAMSTEPSTATEMACHINE_H
