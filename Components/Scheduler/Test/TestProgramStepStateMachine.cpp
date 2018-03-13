#include <QTest>
#include <QHistoryState>
#include <QSignalSpy>
#include <QDebug>

#include "Scheduler/Include/SchedulerMainThreadController.h"

// Run exec for a maximum of TIMEOUT msecs
#define QCOREAPPLICATION_EXEC(TIMEOUT) \
{ \
    QTimer timer; \
    timer.setSingleShot(true); \
    timer.setInterval(TIMEOUT); \
    timer.start(); \
    connect(&timer, SIGNAL(timeout()), QCoreApplication::instance(), SLOT(quit())); \
    QCoreApplication::exec(); \
}


namespace Scheduler {
class CMockTestProgramStepStateMachine : public CProgramStepStateMachine {
       Q_OBJECT
public:
    CMockTestProgramStepStateMachine(QState* pParentState, QState* pErrorState):CProgramStepStateMachine(pParentState, pErrorState)
    {
    }

    void emitStInitOK()
    {
        emit StInitOK();
    }

    void emitStTempOK()
    {
        emit StTempOK();
    }

    void emitStCurrentOK()
    {
        emit StCurrentOK();
    }

    void emitStVoltageOK()
    {
        emit StVoltageOK();
    }

    void emitStRVPositionOK()
    {
        emit StRVPositionOK();
    }

    void emitStPressureOK()
    {
        emit StPressureOK();
    }

    void emitStSealingOK()
    {
        emit StSealingOK();
    }

    void emitStGetStationcheckResult()
    {
        emit StGetStationcheckResult();
    }

    void emitStStationLeft()
    {
        emit StStationLeft();
    }

    void emitStStationOK()
    {
        emit StStationOK();
    }

    void emitStDone()
    {
        emit StDone();
    }

    void emitTempsReady()
    {
        emit TempsReady();
    }

    void emitLevelSensorTempS1Ready()
    {
        emit LevelSensorTempS1Ready();
    }

    void emitLevelSensorTempS2Ready()
    {
        emit LevelSensorTempS2Ready();
    }

    void emitHitTubeBefore()
    {
        emit HitTubeBefore();
    }

    void emitFillFinished()
    {
        emit FillFinished();
    }

    void emitHitSeal()
    {
        emit HitSeal();
    }

    void emitSoakFinished()
    {
        emit SoakFinished();
    }

    void emitHitTubeAfter()
    {
        emit HitTubeAfter();
    }

    void emitDrainFinished()
    {
        emit DrainFinished();
    }

    void emitStepFinished()
    {
        emit StepFinished();
    }

    void emitProgramFinished()
    {
        emit ProgramFinished();
    }

    void emitError()
    {
        emit Error();
    }

    void emitPause()
    {
        emit Pause();
    }

    void emitResumeToSelftest()
    {
        emit ResumeToSelftest();
    }

    void emitResumeToInit()
    {
        emit ResumeToInit();
    }

    void emitResumeToHeatLevelSensorS1()
    {
        emit ResumeToHeatLevelSensorS1();
    }

    void emitResumeToHeatLevelSensorS2()
    {
        emit ResumeToHeatLevelSensorS2();
    }

    void emitResumeToReadyToFill()
    {
        emit ResumeToReadyToFill();
    }

    void emitResumeToSoak()
    {
        emit ResumeToSoak();
    }

    void emitResumeToStepFinished()
    {
        emit ResumeToStepFinished();
    }

    void emitResumeToReadyToTubeAfter()
    {
        emit ResumeToReadyToTubeAfter();
    }

    void emitAbort()
    {
        emit Abort();
    }

    void emitOnInit()
    {
        emit OnInit();
    }

    void emitOnHeatLevelSensorTempS1()
    {
        emit OnHeatLevelSensorTempS1();
    }

    void emitOnHeatLevelSensorTempS2()
    {
        emit OnHeatLevelSensorTempS2();
    }

    void emitOnMoveToTubeBefore()
    {
        emit OnMoveToTubeBefore();
    }

    void emitOnMoveToTubeAfter()
    {
        emit OnMoveToTubeAfter();
    }

    void emitOnMoveToSeal()
    {
        emit OnMoveToSeal();
    }

    void emitOnFill()
    {
        emit OnFill();
    }

    void emitOnStopFill()
    {
        emit OnStopFill();
    }

    void emitOnSoak()
    {
        emit OnSoak();
    }

    void emitOnDrain()
    {
        emit OnDrain();
    }

    void emitOnStopDrain()
    {
        emit OnStopDrain();
    }

    void emitOnAborting()
    {
        emit OnAborting();
    }

    void emitOnAborted()
    {
        emit OnAborted();
    }

    void emitOnPause()
    {
        emit OnPause();
    }

    void emitOnPauseDrain()
    {
        emit OnPauseDrain();
    }

    void emitOnStateExited()
    {
        emit OnStateExited();
    }

    void emitOnSealingCheck()
    {
        emit OnSealingCheck();
    }

    void emitOnRVPositionCheck()
    {
        emit OnRVPositionCheck();
    }

    void printState(int state)
    {
        switch (state) {
        case PSSM_INIT:
            qDebug() << "state: PSSM_INIT";
            break;
        case PSSM_READY_TO_HEAT_LEVEL_SENSOR_S1:
            qDebug() << "state: PSSM_READY_TO_HEAT_LEVEL_SENSOR_S1";
            break;
        case PSSM_READY_TO_HEAT_LEVEL_SENSOR_S2:
            qDebug() << "state: PSSM_READY_TO_HEAT_LEVEL_SENSOR_S2";
            break;
        case PSSM_READY_TO_TUBE_BEFORE:
            qDebug() << "state: PSSM_READY_TO_TUBE_BEFORE";
            break;
        case PSSM_READY_TO_TUBE_AFTER:
            qDebug() << "state: PSSM_READY_TO_TUBE_AFTER";
            break;
        case PSSM_READY_TO_FILL:
            qDebug() << "state: PSSM_READY_TO_FILL";
            break;
        case PSSM_READY_TO_SEAL:
            qDebug() << "state: PSSM_READY_TO_SEAL";
            break;
        case PSSM_SOAK:
            qDebug() << "state: PSSM_SOAK";
            break;
        case PSSM_READY_TO_DRAIN:
            qDebug() << "state: PSSM_READY_TO_DRAIN";
            break;
        case PSSM_STEP_FINISH:
            qDebug() << "state: PSSM_STEP_FINISH";
            break;
        case PSSM_PROGRAM_FINISH:
            qDebug() << "state: PSSM_PROGRAM_FINISH";
            break;
        case PSSM_PAUSE:
            qDebug() << "state: PSSM_PAUSE";
            break;
        case PSSM_PAUSE_DRAIN:
            qDebug() << "state: PSSM_PAUSE_DRAIN";
            break;
        case PSSM_ABORTED:
            qDebug() << "state: PSSM_ABORTED";
            break;
        case PSSM_ST_INIT:
            qDebug() << "state: PSSM_ST_INIT";
            break;
        case PSSM_ST_TEMP_CHECKING:
            qDebug() << "state: PSSM_ST_TEMP_CHECKING";
            break;
        case PSSM_ST_CURRENT_CHECKING:
            qDebug() << "state: PSSM_ST_CURRENT_CHECKING";
            break;
        case PSSM_ST_VOLTAGE_CHECKING:
            qDebug() << "state: PSSM_ST_VOLTAGE_CHECKING";
            break;
        case PSSM_ST_RV_POSITION_CHECKING:
            qDebug() << "state: PSSM_ST_RV_POSITION_CHECKING";
            break;
        case PSSM_ST_PRESSURE_CHECKING:
            qDebug() << "state: PSSM_ST_PRESSURE_CHECKING";
            break;
        case PSSM_ST_SEALING_CHECKING:
            qDebug() << "state: PSSM_ST_SEALING_CHECKING";
            break;
        case PSSM_ST_STATION_CHECKING:
            qDebug() << "state: PSSM_ST_STATION_CHECKING";
            break;
        case PSSM_ST_STATION_CHECK_FINISH:
            qDebug() << "state: PSSM_ST_STATION_CHECK_FINISH";
            break;
        case PSSM_ST_DONE:
            qDebug() << "state: PSSM_ST_DONE";
            break;
        case SM_UNDEF:
            qDebug() << "state: SM_UNDEF";
            break;
        default:
            qDebug() << "state: unknown , value=" << state;
        }
    }

    virtual ~CMockTestProgramStepStateMachine()
    {
    }
};

/****************************************************************************/
/**
 * \brief Test class for TestProgramStepStateMachine class.
 */
/****************************************************************************/
class TestProgramStepStateMachine : public QObject {
    Q_OBJECT
public:
    TestProgramStepStateMachine()
    {
    }

    void createObject()
    {
        mp_SchedulerMachine = new QStateMachine();
        mp_BusyState = new QState(mp_SchedulerMachine);
        mp_ErrorState = new QState(mp_SchedulerMachine);
        mp_ErrorWaitState = new QState(mp_ErrorState);
    }

    void deleteObject()
    {
        delete MockTestProgramStepStateMachine;
        delete mp_ErrorWaitState;
        delete mp_BusyState;
        delete mp_ErrorState;
        delete mp_SchedulerMachine;
    }

    ~TestProgramStepStateMachine()
    {
    }

private:
    QStateMachine *mp_SchedulerMachine;
    QState* mp_BusyState;
    QState *mp_ErrorState;
    QState *mp_ErrorWaitState;

    CMockTestProgramStepStateMachine *MockTestProgramStepStateMachine;

private slots:
    void CaseStateMachine1();
    void CaseStateMachine2();
};

void TestProgramStepStateMachine::CaseStateMachine1()
{
    createObject();
    MockTestProgramStepStateMachine = new CMockTestProgramStepStateMachine(mp_BusyState, mp_ErrorState);
    mp_SchedulerMachine->setInitialState(mp_BusyState);
    mp_ErrorState->setInitialState(mp_ErrorWaitState);

    mp_SchedulerMachine->start();
    QCOREAPPLICATION_EXEC(500);
    QVERIFY(mp_SchedulerMachine->isRunning());

    QCOMPARE(mp_SchedulerMachine->configuration().count(), 3);
    QCOMPARE(Scheduler::PSSM_ST_INIT, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));

    // ------>>>
    {
        QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(StInitOK()));
        MockTestProgramStepStateMachine->emitStInitOK();
        QCoreApplication::processEvents();
        QCOMPARE(spy.count(), 1);
        QCOMPARE(Scheduler::PSSM_ST_TEMP_CHECKING, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
    }

    {
        MockTestProgramStepStateMachine->emitStTempOK();
        QCoreApplication::processEvents();
        QCOMPARE(Scheduler::PSSM_ST_CURRENT_CHECKING, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
    }

    {
        QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(StCurrentOK()));
        MockTestProgramStepStateMachine->emitStCurrentOK();
        QCoreApplication::processEvents();
        QCOMPARE(spy.count(), 1);
        QCOMPARE(Scheduler::PSSM_ST_VOLTAGE_CHECKING, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
    }

    {
        QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(StVoltageOK()));
        MockTestProgramStepStateMachine->emitStVoltageOK();
        QCoreApplication::processEvents();
        QCOMPARE(spy.count(), 1);
        QCOMPARE(Scheduler::PSSM_ST_RV_POSITION_CHECKING, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
    }

    {
        QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(StRVPositionOK()));
        MockTestProgramStepStateMachine->emitStRVPositionOK();
        QCoreApplication::processEvents();
        QCOMPARE(spy.count(), 1);
        QCOMPARE(Scheduler::PSSM_ST_PRESSURE_CHECKING, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
    }

    {
        QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(StPressureOK()));
        MockTestProgramStepStateMachine->emitStPressureOK();
        QCoreApplication::processEvents();
        QCOMPARE(spy.count(), 1);
        QCOMPARE(Scheduler::PSSM_ST_SEALING_CHECKING, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
    }

    {
        QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(StSealingOK()));
        MockTestProgramStepStateMachine->emitStSealingOK();
        QCoreApplication::processEvents();
        QCOMPARE(spy.count(), 1);
        QCOMPARE(Scheduler::PSSM_ST_STATION_CHECKING, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
    }

    {
        QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(StGetStationcheckResult()));
        MockTestProgramStepStateMachine->emitStGetStationcheckResult();
        QCoreApplication::processEvents();
        QCOMPARE(spy.count(), 1);
        QCOMPARE(Scheduler::PSSM_ST_STATION_CHECK_FINISH, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
    }

    {
        QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(StStationLeft()));
        MockTestProgramStepStateMachine->emitStStationLeft();
        QCoreApplication::processEvents();
        QCOMPARE(spy.count(), 1);
        QCOMPARE(Scheduler::PSSM_ST_STATION_CHECKING, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
    }

    {
        QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(StGetStationcheckResult()));
        MockTestProgramStepStateMachine->emitStGetStationcheckResult();
        QCoreApplication::processEvents();
        QCOMPARE(spy.count(), 1);
        QCOMPARE(Scheduler::PSSM_ST_STATION_CHECK_FINISH, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
    }

    {
        QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(StStationOK()));
        MockTestProgramStepStateMachine->emitStStationOK();
        QCoreApplication::processEvents();
        QCOMPARE(spy.count(), 1);
        QCOMPARE(Scheduler::PSSM_ST_DONE, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
    }

    //------------------------------------------------------

    {
        QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(Pause()));
        MockTestProgramStepStateMachine->emitPause();
        QCoreApplication::processEvents();
        QCOMPARE(spy.count(), 1);
        QCOMPARE(Scheduler::PSSM_PAUSE, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
    }

    {
        QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(ResumeToSelftest()));
        MockTestProgramStepStateMachine->emitResumeToSelftest();
        QCoreApplication::processEvents();
        QCOMPARE(spy.count(), 1);
        QCOMPARE(Scheduler::PSSM_ST_DONE, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
//        MockTestProgramStepStateMachine->printState(MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
    }

    {
        QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(Error()));
        MockTestProgramStepStateMachine->emitError();
        QCoreApplication::processEvents();
        QCOMPARE(spy.count(), 1);
        QCOMPARE(Scheduler::SM_UNDEF, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
    }

    {
        QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(ResumeToSelftest()));
        MockTestProgramStepStateMachine->emitResumeToSelftest();
        QCoreApplication::processEvents();
        QCOMPARE(spy.count(), 1);
        QCOMPARE(Scheduler::PSSM_ST_DONE, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
    }

    {
        QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(Abort()));
        MockTestProgramStepStateMachine->emitAbort();
        QCoreApplication::processEvents();
        QCOMPARE(spy.count(), 1);
        QCOMPARE(Scheduler::PSSM_ABORTED, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
    }

    //------------------------------------------------------

    deleteObject();
}


void TestProgramStepStateMachine::CaseStateMachine2()
{
    createObject();
    MockTestProgramStepStateMachine = new CMockTestProgramStepStateMachine(mp_BusyState, mp_ErrorState);
    mp_SchedulerMachine->setInitialState(mp_BusyState);
    mp_ErrorState->setInitialState(mp_ErrorWaitState);

    mp_SchedulerMachine->start();
    QCOREAPPLICATION_EXEC(500);
    QVERIFY(mp_SchedulerMachine->isRunning());

    QCOMPARE(mp_SchedulerMachine->configuration().count(), 3);
    QCOMPARE(Scheduler::PSSM_ST_INIT, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));

        {
            MockTestProgramStepStateMachine->emitStDone();
            QCOREAPPLICATION_EXEC(500);
            QCOMPARE(Scheduler::PSSM_INIT, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
        }

      for (int i = 0; i < 2; i++) {
        {
            QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(TempsReady()));
            MockTestProgramStepStateMachine->emitTempsReady();
            QCoreApplication::processEvents();
            QCOMPARE(spy.count(), 1);
            QCOMPARE(Scheduler::PSSM_READY_TO_HEAT_LEVEL_SENSOR_S1, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
        }

        {
            QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(LevelSensorTempS1Ready()));
            MockTestProgramStepStateMachine->emitLevelSensorTempS1Ready();
            QCoreApplication::processEvents();
            QCOMPARE(spy.count(), 1);
            QCOMPARE(Scheduler::PSSM_READY_TO_HEAT_LEVEL_SENSOR_S2, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
        }

        {
            QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(LevelSensorTempS2Ready()));
            MockTestProgramStepStateMachine->emitLevelSensorTempS2Ready();
            QCoreApplication::processEvents();
            QCOMPARE(spy.count(), 1);
            QCOMPARE(Scheduler::PSSM_READY_TO_TUBE_BEFORE, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
        }

        {
            QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(HitTubeBefore()));
            MockTestProgramStepStateMachine->emitHitTubeBefore();
            QCoreApplication::processEvents();
            QCOMPARE(spy.count(), 1);
            QCOMPARE(Scheduler::PSSM_READY_TO_FILL, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
        }

        {
            QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(FillFinished()));
            MockTestProgramStepStateMachine->emitFillFinished();
            QCoreApplication::processEvents();
            QCOMPARE(spy.count(), 1);
            QCOMPARE(Scheduler::PSSM_READY_TO_SEAL, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
        }

        {
            QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(HitSeal()));
            MockTestProgramStepStateMachine->emitHitSeal();
            QCoreApplication::processEvents();
            QCOMPARE(spy.count(), 1);
            QCOMPARE(Scheduler::PSSM_SOAK, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
        }

        {
            QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(SoakFinished()));
            MockTestProgramStepStateMachine->emitSoakFinished();
            QCoreApplication::processEvents();
            QCOMPARE(spy.count(), 1);
            QCOMPARE(Scheduler::PSSM_READY_TO_TUBE_AFTER, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
        }

        {
            QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(HitTubeAfter()));
            MockTestProgramStepStateMachine->emitHitTubeAfter();
            QCoreApplication::processEvents();
            QCOMPARE(spy.count(), 1);
            QCOMPARE(Scheduler::PSSM_READY_TO_DRAIN, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
        }

        {
            QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(DrainFinished()));
            MockTestProgramStepStateMachine->emitDrainFinished();
            QCoreApplication::processEvents();
            QCOMPARE(spy.count(), 1);
            QCOMPARE(Scheduler::PSSM_STEP_FINISH, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
        }

        if (i == 0) {
            QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(StepFinished()));
            MockTestProgramStepStateMachine->emitStepFinished();
            QCoreApplication::processEvents();
            QCOMPARE(spy.count(), 1);
            QCOMPARE(Scheduler::PSSM_INIT, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
        } else {
            QSignalSpy spy(MockTestProgramStepStateMachine, SIGNAL(ProgramFinished()));
            MockTestProgramStepStateMachine->emitProgramFinished();
            QCoreApplication::processEvents();
            QCOMPARE(spy.count(), 1);
            QCOMPARE(Scheduler::PSSM_PROGRAM_FINISH, MockTestProgramStepStateMachine->GetCurrentState(mp_SchedulerMachine->configuration()));
        }
    }

    deleteObject();
}


} // end namespace EventHandler


QTEST_MAIN(Scheduler::TestProgramStepStateMachine)

#include "TestProgramStepStateMachine.moc"
