#include <QTest>
#include <QHistoryState>
#include <QSignalSpy>
#include <QDebug>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QDateTime>

using::testing::Return;
using::testing::AtLeast;
using::testing::_;
using::testing::Lt;
using::testing::ReturnArg;
using::testing::Invoke;
using::testing::WithArgs;



#include "HimalayaMasterThread/Include/ThreadIDs.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "Scheduler/Source/SchedulerCommandProcessor.cpp"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "HimalayaMasterThread/Include/HimalayaMasterThreadController.h"
#include "HimalayaDataManager/Include/DataManager.h"
#include "Global/Include/SystemPaths.h"
#include "DeviceControl/Test/Mock/MockDeviceControl.h"



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



/****************************************************************************/
/**
 * \brief Test class for TestProgramStepStateMachine class.
 */
/****************************************************************************/
class TestSchedulerCommandProcessor : public QObject {
    Q_OBJECT
public:
    TestSchedulerCommandProcessor()
    {

    }

    ~TestSchedulerCommandProcessor()
    {
    }

    Himalaya::HimalayaMasterThreadController*   mp_HMThreadController;
    DataManager::CDataManager*                  mp_DataManager;
    MockDeviceControl*                      mp_IDeviceControl;
    SchedulerMainThreadController*              m_pSchedulerMainController;
    Scheduler::SchedulerCommandProcessor<DeviceControl::MockDeviceControl>*                     mp_SchdCmdProcessor;

private slots:
    /****************************************************************************/
    /**
     * \brief Called before the first testfunction is executed.
     */
    /****************************************************************************/
    void initTestCase();
    /****************************************************************************/
    /**
     * \brief Called before each testfunction is executed.
     */
    /****************************************************************************/
    //void init();

    void Case();
    /****************************************************************************/
    /**
     * \brief Called after each testfunction was executed.
     */
    /****************************************************************************/
    void retortId_is_recevied();
    /****************************************************************************/
    /**
     * \brief Called after last testfunction was executed.
     */
    /****************************************************************************/
    void cleanupTestCase();
};

//class SchedulerCommandProcessorTestClass: public Scheduler::SchedulerCommandProcessor<DeviceControl::MockIDeviceControl>
//{
//  public:
//    SchedulerCommandProcessorTestClass(Scheduler::SchedulerMainThreadController *schedulerMainController)
//        :SchedulerCommandProcessor<DeviceControl::MockIDeviceControl>(schedulerMainController)
//    {}
//    void Start(DeviceControl::MockIDeviceControl *IDeviceProcessing)
//    {
//        mp_IDeviceProcessing = IDeviceProcessing;
//    SchedulerCommandProcessor::run4Slot();
//    }
//};

void TestSchedulerCommandProcessor::initTestCase()
{    
    // create and connect scheduler main controller
    m_pSchedulerMainController = new Scheduler::SchedulerMainThreadController(THREAD_ID_SCHEDULER);
     mp_SchdCmdProcessor = dynamic_cast<Scheduler::SchedulerCommandProcessor<DeviceControl::MockDeviceControl>*>(m_pSchedulerMainController->m_SchedulerCommandProcessor);


//    mp_HMThreadController = new Himalaya::HimalayaMasterThreadController();
//    Global::SystemPaths::Instance().SetSettingsPath("../../../Main/Build/Settings");
//    mp_DataManager = new DataManager::CDataManager(mp_HMThreadController);
//    m_pSchedulerMainController->DataManager(mp_DataManager);

   mp_SchdCmdProcessor->run();

    mp_IDeviceControl = new DeviceControl::MockDeviceControl();


     mp_SchdCmdProcessor->mp_IDeviceProcessing = mp_IDeviceControl;
//    reinterpret_cast<SchedulerCommandProcessor<MockIDeviceProcessing> *>(mp_SchdCmdProcessor)->SetIDeviceProcessing(&IDeviceProcessing);

//    m_pSchedulerMainController->CreateAndInitializeObjects();

//    // Move Scheduler into one new thread and start up
//    QThread* schedulerThread = new QThread();
//    m_pSchedulerMainController->moveToThread(schedulerThread);
//    CONNECTSIGNALSLOT(schedulerThread, started(), mp_SchdCmdProcessor, run());
//    schedulerThread->start();


    QCOREAPPLICATION_EXEC(500);


//    AddAndConnectController(schedulerMainController, &m_CommandChannelSchedulerMain, static_cast<int>(SCHEDULER_MAIN_THREAD));
//    schedulerMainController->DataManager(mp_DataManager);
    {
        EXPECT_CALL(*mp_IDeviceControl, StartConfigurationService())
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceControl, RVReqMoveToInitialPosition(_))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceControl, PerTurnOnMainRelay())
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceControl, RTStartTemperatureControlWithPID(_, _, _, _, _, _, _))
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceControl, RVStartTemperatureControlWithPID(_, _, _, _, _, _))
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceControl, OvenStartTemperatureControlWithPID(_, _, _, _, _, _, _))
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceControl, ALStartTemperatureControlWithPID(_, _, _, _, _, _, _))
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

//        EXPECT_CALL(IDeviceProcessing, ALGetRecentPressure())
//                .Times(AtLeast(1))
//                .WillRepeatedly(Return(100));

        EXPECT_CALL(*mp_IDeviceControl, ALSetPressureDrift(_))
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

//-----------------------------------------------------------------------
        EXPECT_CALL(*mp_IDeviceControl, ALTurnOnFan())
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceControl, ALTurnOffFan())
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));
    }


    {
        // Test SchedulerCommandProcessor->HardwareMonitor()

        EXPECT_CALL(*mp_IDeviceControl, ALGetRecentPressure())
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(10.0));

        EXPECT_CALL(*mp_IDeviceControl, ALGetRecentTemperature(_, _))
//                .Times(AtLeast(3))
                .WillRepeatedly(Return(10.0));

        EXPECT_CALL(*mp_IDeviceControl, RVGetRecentTemperature(_))
//                .Times(AtLeast(2))
                .WillRepeatedly(Return(10.0));

        EXPECT_CALL(*mp_IDeviceControl, RTGetRecentTemperature(_, _))
//                .Times(AtLeast(2))
                .WillRepeatedly(Return(10.0));

        EXPECT_CALL(*mp_IDeviceControl, RVReqActRVPosition())
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(RV_TUBE_1));

        EXPECT_CALL(*mp_IDeviceControl, OvenGetRecentTemperature(_, _))
//                .Times(AtLeast(2))
                .WillRepeatedly(Return(10.0));

        EXPECT_CALL(*mp_IDeviceControl, OvenGetRecentLidStatus())
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(10.0));

        EXPECT_CALL(*mp_IDeviceControl, RTGetRecentLockStatus())
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(1));


        HardwareMonitor_t monitor;

        monitor = mp_SchdCmdProcessor->HardwareMonitor();
    }

    {
        EXPECT_CALL(*mp_IDeviceControl, ALBreakAllOperation()).
//                .Times(AtLeast(1))
                WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));
        EXPECT_EQ(mp_SchdCmdProcessor->ALBreakAllOperation(), DCL_ERR_FCT_CALL_SUCCESS);

    }

    // Call ALGetRecentPressure()
    {
        EXPECT_EQ(mp_SchdCmdProcessor->ALGetRecentPressure(), 10.0);
    }

    {
        mp_SchdCmdProcessor->ALSetPressureDrift(10.0);
    }

    {
        emit mp_IDeviceControl->ReportDrainingTimeOut2Min();
        QCOREAPPLICATION_EXEC(1000);
    }

    {
//        IDeviceProcessing.ConfigurationFinished();
//        QCOREAPPLICATION_EXEC(1000);
    }

    {
//        IDeviceProcessing.emitReportStartNormalOperationMode(1, DCL_ERR_FCT_CALL_SUCCESS);
    }

    QCOREAPPLICATION_EXEC(1000 * 20);

}

void TestSchedulerCommandProcessor::Case(){

    // arrange

//    EXPECT_CALL(*mp_IDeviceControl, ALGetRecentTemperature(_))
//            .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

    Scheduler::SchedulerCommandShPtr_t cmd = Scheduler::SchedulerCommandShPtr_t(new CmdALGetRecentTemperature(500, "TestSender"));

    // act

    mp_SchdCmdProcessor->ExecuteCmd((cmd));

    // assert
    qreal result;
    cmd.data()->GetResult(result);
    QCOMPARE(result, 10.0);

}

void TestSchedulerCommandProcessor::retortId_is_recevied()
{
    // arrange
    static QString x;

    EXPECT_CALL(*mp_IDeviceControl, ALReleasePressure()).WillOnce((Return(DCL_ERR_FCT_CALL_SUCCESS)));
    Scheduler::SchedulerCommandShPtr_t cmd = Scheduler::SchedulerCommandShPtr_t(new CmdALReleasePressure(500, "TestSender"));

    // act
    mp_SchdCmdProcessor->pushCmd(cmd.data());
    mp_SchdCmdProcessor->OnTickTimer();

    // assert
    ReturnCode_t result;
    dynamic_cast<CmdALReleasePressure*>(cmd.data())->GetResult(result);
    QCOMPARE(result, DCL_ERR_FCT_CALL_SUCCESS);
}

void TestSchedulerCommandProcessor::cleanupTestCase(){

    delete mp_SchdCmdProcessor;
//    delete m_pSchedulerMainController;
//    delete mp_HMThreadController;
    delete mp_IDeviceControl;

}


} // end namespace Scheduler

int main(int argc, char*argv[])
{
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    ::testing::InitGoogleMock(&argc, argv);
    QCoreApplication app(argc, argv);
    Scheduler::TestSchedulerCommandProcessor tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "TestSchedulerCommandProcessor.moc"
