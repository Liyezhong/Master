#include <QTest>
#include <QHistoryState>
#include <QSignalSpy>
#include <QDebug>

#include "Scheduler/Include/SchedulerMainThreadController.h"

namespace Scheduler {

/****************************************************************************/
/**
 * \brief Test class for TestProgramStepStateMachine class.
 */
/****************************************************************************/
class TestSchedulerStateMachine : public QObject {
    Q_OBJECT
public:
    TestSchedulerStateMachine()
    {
    }


    ~TestSchedulerStateMachine()
    {
    }

private:
    CSchedulerStateMachine machine;

private slots:
    void CaseStateMachine();
};

void TestSchedulerStateMachine::CaseStateMachine()
{
    machine.Start();
    machine.Stop();
    machine.SendSchedulerInitComplete();
    machine.SendRunSignal();
    machine.SendRunComplete();
    machine.SendErrorSignal();

    machine.NotifyStInitOK();
    machine.NotifyStTempOK();
    machine.NotifyStCurrentOK();
    machine.NotifyStVoltageOK();
    machine.NotifyStRVPositionOK();
    machine.NotifyStPressureOK();
    machine.NotifyStSealingOK();
    machine.NotifyStGetStationcheckResult();
    machine.NotifyStStationLeft();
    machine.NotifyStStationOK();
    machine.NotifyStDone();
    machine.NotifyTempsReady();
    machine.NotifyLevelSensorTempS1Ready();
    machine.NotifyLevelSensorTempS2Ready();
    machine.NotifyHitTubeBefore();
    machine.NotifyFillFinished();
    machine.NotifyHitSeal();
    machine.NotifySoakFinished();
    machine.NotifyHitTubeAfter();
    machine.NotifyDrainFinished();
    machine.NotifyStepFinished();
    machine.NotifyProgramFinished();
    machine.NotifyError();
    machine.NotifyResume();
    machine.NotifyResumeDrain();
    machine.NotifyAbort();
    machine.NotifyResumeToSelftest();
    machine.NotifyRsRvMoveToInitPosition();
    machine.NotifyRsRvMoveToInitPositionFinished();
    machine.NotifyRCReport();
    machine.NotifyRsReleasePressure();
    machine.NotifyRsShutdownFailedHeater();
    machine.NotifyRsShutdownFailedHeaterFinished();
}


} // end namespace EventHandler


QTEST_MAIN(Scheduler::TestSchedulerStateMachine)

#include "TestSchedulerStateMachine.moc"
