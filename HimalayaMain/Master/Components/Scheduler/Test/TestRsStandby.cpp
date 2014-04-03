#include <QTest>
#include <QHistoryState>
#include <QSignalSpy>

#include "Scheduler/Include/SchedulerMainThreadController.h"

namespace Scheduler {
class CMockTestRsStandby : public CRsStandby {
       Q_OBJECT
public:
    CMockTestRsStandby(QStateMachine* pStateMachine, QState* pParentState):CRsStandby(pStateMachine, pParentState)
    {
    }

    void emitReleasePressure()
    {
        emit ReleasePressure();
    }

    void emitShutdownFailedHeater()
    {
        emit ShutdownFailedHeater();
    }

    void emitShutdownFailedHeaterFinished()
    {
        emit ShutdownFailedHeaterFinished();
    }

    void emitOnReleasePressure()
    {
        emit OnReleasePressure();
    }

    void emitOnShutdownFailedHeater()
    {
        emit OnShutdownFailedHeater();
    }

    virtual ~CMockTestRsStandby()
    {
    }
};

/****************************************************************************/
/**
 * \brief Test class for TestRcReport class.
 */
/****************************************************************************/
class TestRsStandby : public QObject {
    Q_OBJECT
public:
    TestRsStandby()
    {
        mp_SchedulerMachine = new QStateMachine();
        mp_ErrorState = new QState(mp_SchedulerMachine);
        mp_ErrorWaitState = new QState(mp_ErrorState);
        finalState = new QFinalState(mp_SchedulerMachine);
    }

    ~TestRsStandby()
    {
//        delete MockTestRsStandby;
//        delete mp_ErrorWaitState;
//        delete mp_ErrorState;
//        delete mp_SchedulerMachine;
    }

private:
    QStateMachine *mp_SchedulerMachine;
    QState *mp_ErrorState;
    QState *mp_ErrorWaitState;
    QFinalState *finalState;

    Scheduler::CMockTestRsStandby *MockTestRsStandby;

private slots:
    void CaseSignal();
};

void TestRsStandby::CaseSignal()
{
    mp_SchedulerMachine->setInitialState(mp_ErrorState);
    mp_ErrorState->setInitialState(mp_ErrorWaitState);

    MockTestRsStandby = new CMockTestRsStandby(mp_SchedulerMachine, mp_ErrorState);
    QSignalSpy spy(MockTestRsStandby, SIGNAL(OnShutdownFailedHeater()));
    QVERIFY(spy.isValid());

    mp_SchedulerMachine->start();

    QCoreApplication::processEvents();
    QCOMPARE(Scheduler::SM_UNDEF, MockTestRsStandby->GetCurrentState(mp_SchedulerMachine->configuration()));
    QVERIFY(mp_SchedulerMachine->isRunning());

    QCOMPARE(mp_SchedulerMachine->configuration().count(), 2);
    QVERIFY(mp_SchedulerMachine->configuration().contains(mp_ErrorState));
    QVERIFY(mp_SchedulerMachine->configuration().contains(mp_ErrorWaitState));

    MockTestRsStandby->emitReleasePressure();
    QCoreApplication::processEvents();
    QCOMPARE(mp_SchedulerMachine->configuration().count(), 2);
    QCOMPARE(Scheduler::SM_ERR_RS_RELEASE_PRESSURE, MockTestRsStandby->GetCurrentState(mp_SchedulerMachine->configuration()));

    QCoreApplication::processEvents();

    MockTestRsStandby->emitShutdownFailedHeater();
    QCoreApplication::processEvents();
    QCOMPARE(Scheduler::SM_ERR_RS_SHUTDOWN_FAILED_HEATER, MockTestRsStandby->GetCurrentState(mp_SchedulerMachine->configuration()));
    QCOMPARE(spy.count(), 1);
}

} // end namespace Scheduler


QTEST_MAIN(Scheduler::TestRsStandby)

#include "TestRsStandby.moc"
