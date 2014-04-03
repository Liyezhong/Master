#include <QTest>
#include <QHistoryState>
#include <QSignalSpy>

#include "Scheduler/Include/SchedulerMainThreadController.h"

namespace Scheduler {
class CMockTestRcReport : public CRCReport {
       Q_OBJECT
public:
    CMockTestRcReport(QStateMachine* pStateMachine, QState* pParentState):CRCReport(pStateMachine, pParentState)
    {
    }

    void testSignal()
    {
        emit RCReport();
    }

    virtual ~CMockTestRcReport()
    {
    }
};

/****************************************************************************/
/**
 * \brief Test class for TestRcReport class.
 */
/****************************************************************************/
class TestRcReport : public QObject {
    Q_OBJECT
public:
    TestRcReport()
    {
        mp_SchedulerMachine = new QStateMachine();
        mp_ErrorState = new QState(mp_SchedulerMachine);
        mp_ErrorWaitState = new QState(mp_ErrorState);
        finalState = new QFinalState(mp_SchedulerMachine);
    }

    ~TestRcReport()
    {
        delete RCReport;
        delete mp_ErrorWaitState;
        delete mp_ErrorState;
        delete mp_SchedulerMachine;
    }

private:
    QStateMachine *mp_SchedulerMachine;
    QState *mp_ErrorState;
    QState *mp_ErrorWaitState;
    QFinalState *finalState;

    Scheduler::CMockTestRcReport *RCReport;

private slots:
    void CaseSignal();
};

void TestRcReport::CaseSignal()
{
    mp_SchedulerMachine->setInitialState(mp_ErrorState);
    mp_ErrorState->setInitialState(mp_ErrorWaitState);

    RCReport = new CMockTestRcReport(mp_SchedulerMachine, mp_ErrorState);

    mp_SchedulerMachine->start();

    QCoreApplication::processEvents();

    QCOMPARE(mp_SchedulerMachine->configuration().count(), 2);

    QSignalSpy spy(RCReport, SIGNAL(OnRCReport()));
    QVERIFY(spy.isValid());

    RCReport->testSignal();
    QCoreApplication::processEvents();
    QCOMPARE(spy.count(), 1);

    QSet<QAbstractState *> statesList = mp_SchedulerMachine->configuration();
    QCOMPARE(Scheduler::SM_ERR_RC_REPORT, RCReport->GetCurrentState(statesList));
}


} // end namespace EventHandler


QTEST_MAIN(Scheduler::TestRcReport)

#include "TestRcReport.moc"
