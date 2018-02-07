#include <QTest>
#include <QHistoryState>
#include <QSignalSpy>

#include "Scheduler/Include/SchedulerMainThreadController.h"

namespace Scheduler {
class CMockTestRsRvGetOriginalPositionAgain : public CRsRvGetOriginalPositionAgain {
       Q_OBJECT
public:
    CMockTestRsRvGetOriginalPositionAgain(QStateMachine* pStateMachine, QState* pParentState):CRsRvGetOriginalPositionAgain(pStateMachine, pParentState)
    {
    }

    void emitRvMoveToInitPosition()
    {
        emit RvMoveToInitPosition();
    }

    void emitRvMoveToInitPositionFinished()
    {
        emit RvMoveToInitPositionFinished();
    }

    void emitOnRvMoveToInitPosition()
    {
        emit OnRvMoveToInitPosition();
    }

    virtual ~CMockTestRsRvGetOriginalPositionAgain()
    {
    }
};

/****************************************************************************/
/**
 * \brief Test class for TestRsRvGetOriginalPositionAgain class.
 */
/****************************************************************************/
class TestRsRvGetOriginalPositionAgain : public QObject {
    Q_OBJECT
public:
    TestRsRvGetOriginalPositionAgain()
    {
        mp_SchedulerMachine = new QStateMachine();
        mp_ErrorState = new QState(mp_SchedulerMachine);
        mp_ErrorWaitState = new QState(mp_ErrorState);
        finalState = new QFinalState(mp_SchedulerMachine);
    }

    ~TestRsRvGetOriginalPositionAgain()
    {
        delete MockTestRsRvGetOriginalPositionAgain;
        delete mp_ErrorWaitState;
        delete mp_ErrorState;
        delete mp_SchedulerMachine;
    }

private:
    QStateMachine *mp_SchedulerMachine;
    QState *mp_ErrorState;
    QState *mp_ErrorWaitState;
    QFinalState *finalState;

    CMockTestRsRvGetOriginalPositionAgain *MockTestRsRvGetOriginalPositionAgain;

private slots:
    void CaseSignal();
};

void TestRsRvGetOriginalPositionAgain::CaseSignal()
{
    mp_SchedulerMachine->setInitialState(mp_ErrorState);
    mp_ErrorState->setInitialState(mp_ErrorWaitState);

    MockTestRsRvGetOriginalPositionAgain = new CMockTestRsRvGetOriginalPositionAgain(mp_SchedulerMachine, mp_ErrorState);
    QSignalSpy spy(MockTestRsRvGetOriginalPositionAgain, SIGNAL(OnRvMoveToInitPosition()));
    QVERIFY(spy.isValid());

    mp_SchedulerMachine->start();

    QCoreApplication::processEvents();
    QCOMPARE(Scheduler::SM_UNDEF, MockTestRsRvGetOriginalPositionAgain->GetCurrentState(mp_SchedulerMachine->configuration()));
    QVERIFY(mp_SchedulerMachine->isRunning());

    QCOMPARE(mp_SchedulerMachine->configuration().count(), 2);
    QVERIFY(mp_SchedulerMachine->configuration().contains(mp_ErrorState));
    QVERIFY(mp_SchedulerMachine->configuration().contains(mp_ErrorWaitState));

    MockTestRsRvGetOriginalPositionAgain->emitRvMoveToInitPosition();
    QCoreApplication::processEvents();
    QCOMPARE(mp_SchedulerMachine->configuration().count(), 2);
    QCOMPARE(Scheduler::SM_ERR_RS_RV_MOVING_TO_INIT_POS, MockTestRsRvGetOriginalPositionAgain->GetCurrentState(mp_SchedulerMachine->configuration()));
    QCOMPARE(spy.count(), 1);

    MockTestRsRvGetOriginalPositionAgain->emitRvMoveToInitPositionFinished();
    QCoreApplication::processEvents();
    QVERIFY(mp_SchedulerMachine->configuration().contains(mp_ErrorWaitState));
    QCOMPARE(mp_SchedulerMachine->configuration().count(), 2);
}

} // end namespace Scheduler


QTEST_MAIN(Scheduler::TestRsRvGetOriginalPositionAgain)

#include "TestRsRvGetOriginalPositionAgain.moc"
