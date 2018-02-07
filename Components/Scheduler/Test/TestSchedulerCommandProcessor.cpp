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



#include "HimalayaMasterThread/Include/ThreadIDs.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "Scheduler/Source/SchedulerCommandProcessor.cpp"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "HimalayaMasterThread/Include/HimalayaMasterThreadController.h"
#include "HimalayaDataManager/Include/DataManager.h"




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

class MockIDeviceProcessing : public QObject
{
        Q_OBJECT
public:
    void InitializationFinished()
    {
        emit ReportInitializationFinished(DEVICE_INSTANCE_ID_DEVPROC, DCL_ERR_FCT_CALL_SUCCESS);
    }

    void ConfigurationFinished()
    {
        emit ReportConfigurationFinished(DEVICE_INSTANCE_ID_DEVPROC, DCL_ERR_FCT_CALL_SUCCESS);
    }

    void emitReportStartNormalOperationMode(quint32 id, ReturnCode_t info)
    {
        emit ReportStartNormalOperationMode(id, info);
    }


signals:
    //! Forward the 'intitialisation finished' notification
    void ReportInitializationFinished(quint32, ReturnCode_t);
    //! Forward the 'configuration finished' notification
    void ReportConfigurationFinished(quint32, ReturnCode_t);
    //! Forward the 'normal operation mode started' notification
    void ReportStartNormalOperationMode(quint32, ReturnCode_t);
    //! Forward error information
    void ReportError(quint32 instanceID, quint16 usErrorGroup, quint16 usErrorID,
                     quint16 usErrorData, QDateTime timeStamp);
    //! Forward error information
    void ReportErrorWithInfo(quint32 instanceID, quint16 usErrorGroup, quint16 usErrorID,
                             quint16 usErrorData, QDateTime timeStamp, QString strErrorInfo);
    //! Forward the 'Destroy finished' to IDeviceProcessing
    void ReportDestroyFinished();

public:
   //! Start device control layer configuration
    MOCK_METHOD0(StartConfigurationService, ReturnCode_t());

    //! Restart device control layer configuration
    MOCK_METHOD0(RestartConfigurationService, ReturnCode_t());

    //! Start device control layer diagnstic service
    MOCK_METHOD0(StartDiagnosticService, ReturnCode_t());

    //! Finisch device control layer diagnostic service
    MOCK_METHOD0(CloseDiagnosticService, ReturnCode_t());

    //! Start adjustment service
    MOCK_METHOD0(StartAdjustmentService, ReturnCode_t());

    //Air liquid device funcs
    MOCK_METHOD0(ALSetPressureCtrlON, ReturnCode_t());

    MOCK_METHOD0(ALSetPressureCtrlOFF, ReturnCode_t());

    MOCK_METHOD0(ALReleasePressure, ReturnCode_t());

    MOCK_METHOD0(ALPressure, ReturnCode_t());
    MOCK_METHOD0(ALVaccum, ReturnCode_t());

    MOCK_METHOD1(ALDraining, ReturnCode_t(quint32 DelayTime));

    MOCK_METHOD3(ALFilling, ReturnCode_t(quint32 DelayTime, bool EnableInsufficientCheck, bool SafeReagent4Paraffin));

    MOCK_METHOD0(ALGetRecentPressure, qreal());

    MOCK_METHOD1(ALSetTempCtrlON, ReturnCode_t(ALTempCtrlType_t Type));

    MOCK_METHOD5(ALSetTemperaturePid, ReturnCode_t(ALTempCtrlType_t Type, quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime));

    MOCK_METHOD3(ALStartTemperatureControl, ReturnCode_t(ALTempCtrlType_t Type, qreal NominalTemperature, quint8 SlopeTempChange));

    MOCK_METHOD2(ALGetRecentTemperature, qreal(ALTempCtrlType_t Type, quint8 Index));

    MOCK_METHOD1(ALGetTemperatureControlState, TempCtrlState_t(ALTempCtrlType_t Type));

    MOCK_METHOD0(ALTurnOnFan, ReturnCode_t());
    MOCK_METHOD0(ALTurnOffFan, ReturnCode_t());
    MOCK_METHOD0(ALAllStop, ReturnCode_t());
    MOCK_METHOD0(ALBreakAllOperation, ReturnCode_t());
    MOCK_METHOD1(ALSetPressureDrift, ReturnCode_t(qreal pressureDrift));
    MOCK_METHOD1(IDSealingCheck, ReturnCode_t(qreal ThresholdPressure));

    MOCK_METHOD7(ALStartTemperatureControlWithPID, ReturnCode_t(ALTempCtrlType_t Type, qreal NominalTemperature, quint8 SlopeTempChange, \
                quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime));

    //Rotary Valve device func
    MOCK_METHOD0(RVSetTempCtrlON, ReturnCode_t());

    MOCK_METHOD0(RVSetTempCtrlOFF, ReturnCode_t());

    MOCK_METHOD4(RVSetTemperaturePid, ReturnCode_t(quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime));

    MOCK_METHOD2(RVStartTemperatureControl, ReturnCode_t(qreal NominalTemperature, quint8 SlopeTempChange));

    MOCK_METHOD1(RVGetRecentTemperature, qreal(quint32 Index));

    MOCK_METHOD0(RVGetTemperatureControlState, TempCtrlState_t());

    //! Execute the move to intial position of the RV
    MOCK_METHOD0(RVReqMoveToInitialPosition, ReturnCode_t());

    //! Position the oven cover
    MOCK_METHOD1(RVReqMoveToRVPosition, ReturnCode_t(RVPosition_t RVPosition));

    //! Request actual oven cover position
    MOCK_METHOD0(RVReqActRVPosition, RVPosition_t());

    MOCK_METHOD6(RVStartTemperatureControlWithPID, ReturnCode_t(qreal NominalTemperature, quint8 SlopeTempChange, quint16 MaxTemperature, \
                quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime));

    //Oven device func
    MOCK_METHOD1(OvenSetTempCtrlON, ReturnCode_t(OVENTempCtrlType_t Type));

    MOCK_METHOD5(OvenSetTemperaturePid, ReturnCode_t(OVENTempCtrlType_t Type, quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime));
    MOCK_METHOD3(OvenStartTemperatureControl, ReturnCode_t(OVENTempCtrlType_t Type, qreal NominalTemperature, quint8 SlopeTempChange));
    MOCK_METHOD2(OvenGetRecentTemperature, qreal(OVENTempCtrlType_t Type, quint8 Index));

    MOCK_METHOD0(OvenGetRecentLidStatus, quint16());

    MOCK_METHOD1(OvenGetTemperatureControlState, TempCtrlState_t(OVENTempCtrlType_t Type));

    MOCK_METHOD7(OvenStartTemperatureControlWithPID, ReturnCode_t(OVENTempCtrlType_t Type, qreal NominalTemperature, quint8 SlopeTempChange, \
                quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime));

    //Retort device func
    MOCK_METHOD1(RTSetTempCtrlON, ReturnCode_t(RTTempCtrlType_t Type));

    MOCK_METHOD1(RTSetTempCtrlOFF, ReturnCode_t(RTTempCtrlType_t Type));

    MOCK_METHOD5(RTSetTemperaturePid, ReturnCode_t(RTTempCtrlType_t Type, quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime));

    MOCK_METHOD3(RTStartTemperatureControl, ReturnCode_t(RTTempCtrlType_t Type, qreal NominalTemperature, quint8 SlopeTempChange));

    MOCK_METHOD7(RTStartTemperatureControlWithPID, ReturnCode_t(RTTempCtrlType_t Type, qreal NominalTemperature, quint8 SlopeTempChange, \
                quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime));
    MOCK_METHOD2(RTGetRecentTemperature, qreal(RTTempCtrlType_t Type, quint8 Index));

    MOCK_METHOD1(RTGetTemperatureControlState, TempCtrlState_t(RTTempCtrlType_t Type));

    MOCK_METHOD0(RTUnlock, ReturnCode_t());
    MOCK_METHOD0(RTLock, ReturnCode_t());
    MOCK_METHOD0(RTGetRecentLockStatus, quint16());

    //Periphery device func
    MOCK_METHOD0(PerTurnOffMainRelay, ReturnCode_t());
    MOCK_METHOD0(PerTurnOnMainRelay, ReturnCode_t());

    MOCK_METHOD2(IDBottleCheck, ReturnCode_t(QString ReagentGrpID, RVPosition_t TubePos));
};

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

private:

private slots:
    void Case();

};

void TestSchedulerCommandProcessor::Case()
{    
    // create and connect scheduler main controller
    Scheduler::SchedulerMainThreadController *schedulerMainController = new Scheduler::SchedulerMainThreadController(THREAD_ID_SCHEDULER);
    SchedulerCommandProcessorBase *mp_SchdCmdProcessor = new Scheduler::SchedulerCommandProcessor<MockIDeviceProcessing>(schedulerMainController);
    schedulerMainController->SetSchedCommandProcessor(mp_SchdCmdProcessor);

    Himalaya::HimalayaMasterThreadController *mp_HMThreadController = new Himalaya::HimalayaMasterThreadController();
    Global::SystemPaths::Instance().SetSettingsPath("../../../Main/Build/Settings");
    DataManager::CDataManager *mp_DataManager = new DataManager::CDataManager(mp_HMThreadController);
    schedulerMainController->DataManager(mp_DataManager);

    MockIDeviceProcessing IDeviceProcessing;
    reinterpret_cast<SchedulerCommandProcessor<MockIDeviceProcessing> *>(mp_SchdCmdProcessor)->SetIDeviceProcessing(&IDeviceProcessing);

    schedulerMainController->CreateAndInitializeObjects();

    // Move Scheduler into one new thread and start up
//    QThread* schedulerThread = new QThread();
//    schedulerMainController->moveToThread(schedulerThread);
//    CONNECTSIGNALSLOT(schedulerThread, started(), mp_SchdCmdProcessor, run());
//    schedulerThread->start();


    QCOREAPPLICATION_EXEC(500);


//    AddAndConnectController(schedulerMainController, &m_CommandChannelSchedulerMain, static_cast<int>(SCHEDULER_MAIN_THREAD));
//    schedulerMainController->DataManager(mp_DataManager);
    {
        EXPECT_CALL(IDeviceProcessing, StartConfigurationService())
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(IDeviceProcessing, RVReqMoveToInitialPosition())
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(IDeviceProcessing, PerTurnOnMainRelay())
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(IDeviceProcessing, RTStartTemperatureControlWithPID(_, _, _, _, _, _, _))
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(IDeviceProcessing, RVStartTemperatureControlWithPID(_, _, _, _, _, _))
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(IDeviceProcessing, OvenStartTemperatureControlWithPID(_, _, _, _, _, _, _))
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(IDeviceProcessing, ALStartTemperatureControlWithPID(_, _, _, _, _, _, _))
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

//        EXPECT_CALL(IDeviceProcessing, ALGetRecentPressure())
//                .Times(AtLeast(1))
//                .WillRepeatedly(Return(100));

        EXPECT_CALL(IDeviceProcessing, ALSetPressureDrift(_))
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

//-----------------------------------------------------------------------
        EXPECT_CALL(IDeviceProcessing, ALTurnOnFan())
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(IDeviceProcessing, ALTurnOffFan())
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));
    }


    {
        // Test SchedulerCommandProcessor->HardwareMonitor()

        EXPECT_CALL(IDeviceProcessing, ALGetRecentPressure())
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(10.0));

        EXPECT_CALL(IDeviceProcessing, ALGetRecentTemperature(_, _))
//                .Times(AtLeast(3))
                .WillRepeatedly(Return(10.0));

        EXPECT_CALL(IDeviceProcessing, RVGetRecentTemperature(_))
//                .Times(AtLeast(2))
                .WillRepeatedly(Return(10.0));

        EXPECT_CALL(IDeviceProcessing, RTGetRecentTemperature(_, _))
//                .Times(AtLeast(2))
                .WillRepeatedly(Return(10.0));

        EXPECT_CALL(IDeviceProcessing, RVReqActRVPosition())
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(RV_TUBE_1));

        EXPECT_CALL(IDeviceProcessing, OvenGetRecentTemperature(_, _))
//                .Times(AtLeast(2))
                .WillRepeatedly(Return(10.0));

        EXPECT_CALL(IDeviceProcessing, OvenGetRecentLidStatus())
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(10.0));

        EXPECT_CALL(IDeviceProcessing, RTGetRecentLockStatus())
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(1));

        HardwareMonitor_t monitor;

        monitor = mp_SchdCmdProcessor->HardwareMonitor();
    }

    {
        EXPECT_CALL(IDeviceProcessing, ALBreakAllOperation())
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));
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
        IDeviceProcessing.InitializationFinished();
        QCOREAPPLICATION_EXEC(1000);
    }

    {
        IDeviceProcessing.ConfigurationFinished();
        QCOREAPPLICATION_EXEC(1000);
    }

    {
        IDeviceProcessing.emitReportStartNormalOperationMode(1, DCL_ERR_FCT_CALL_SUCCESS);
    }

    QCOREAPPLICATION_EXEC(1000 * 20);

    delete mp_SchdCmdProcessor;
    delete schedulerMainController;
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
