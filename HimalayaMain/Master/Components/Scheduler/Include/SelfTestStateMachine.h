#ifndef SELFTESTSTATEMACHINE_H
#define SELFTESTSTATEMACHINE_H

#include <QStateMachine>
#include <QFinalState>

namespace Scheduler{

typedef enum
{
SELF_TEST_INIT,
SELF_TEST_TEMP_READY,
SELF_TEST_BOTTLE_CHECKING,
SELF_TEST_BOTTLE_CHECK_FINISH,
SELF_TEST_FINISH,
SELF_TEST_UNDEF
}SelfTestStateMachine_t;

class SelfTestStateMachine : public QObject
{
    Q_OBJECT
public:
    SelfTestStateMachine();
    ~SelfTestStateMachine();
    SelfTestStateMachine_t GetCurrentState();

    void Start();
    void Stop();
    void NotifyTempsReady();
    void NotifyCheckStation();
    void NotifyGotCheckStationResult();
    void NotifyCheckStaionFinished();

signals:
        void TempsReady();
        void CheckStation();
        void GotStationCheckResult();
        void StationCheckFinished();
    //void TempsReady();
    //void LevelSensorTempS1Ready();
    //void LevelSensorTempS2Ready();
    //void HitTubeBefore();
    //void FillFinished();
    //void HitSeal();
    //void SoakFinished();
    //void HitTubeAfter();
    //void DrainFinished();
    //void Error();
    //void Pause();
    //void ResumeToInit();
    //void ResumeToHeatLevelSensorS1();
    //void ResumeToHeatLevelSensorS2();
    //void ResumeToReadyToFill();
    //void ResumeToSoak();
    //void ResumeToReadyToTubeAfter();
    //void Abort();
//
    //void OnInit();
    //void OnHeatLevelSensorTempS1();
    //void OnHeatLevelSensorTempS2();
    //void OnMoveToTubeBefore();
    //void OnMoveToTubeAfter();
    //void OnMoveToSeal();
    //void OnFill();
    //void OnSoak();
    //void OnDrain();

private slots:
    //void OnStateChanged();

private:
    QStateMachine* mp_SelfTestStateMachine;
    QState* mp_StInit;
    QState* mp_StTempReady;
    QState* mp_StStationChecking;
    QState* mp_StStationCheckFinish;
    QFinalState* mp_StFinish;

};

}


#endif // SELFTESTSTATEMACHINE_H
