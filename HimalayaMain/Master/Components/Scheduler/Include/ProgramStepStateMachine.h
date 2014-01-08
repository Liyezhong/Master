#ifndef PROGRAMSTEPSTATEMACHINE_H
#define PROGRAMSTEPSTATEMACHINE_H


#include <QStateMachine>
#include <QFinalState>
#include "SchedulerStateMachineStates.h"
namespace Scheduler{

class CProgramStepStateMachine : public QObject
{
    Q_OBJECT
public:
    CProgramStepStateMachine(QState* pParentState, QState* pErrorState);
    ~CProgramStepStateMachine();
    SchedulerStateMachine_t GetCurrentState(QSet<QAbstractState*> statesList);

    void Start();
    void Stop();

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
