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

#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/HimalayaDeviceEventCodes.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToInitialPosition.h"
#include "Scheduler/Commands/Include/CmdALStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdALFilling.h"
#include "Scheduler/Commands/Include/CmdALDraining.h"
#include "Scheduler/Commands/Include/CmdALPressure.h"
#include "Scheduler/Commands/Include/CmdALVaccum.h"
#include "Scheduler/Commands/Include/CmdALAllStop.h"
#include "Scheduler/Commands/Include/CmdRVStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdRTStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdOvenStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdPerTurnOnMainRelay.h"
#include "Scheduler/Commands/Include/CmdPerTurnOffMainRelay.h"
#include "Scheduler/Commands/Include/CmdALTurnOnFan.h"
#include "Scheduler/Commands/Include/CmdALTurnOffFan.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToRVPosition.h"
#include "Scheduler/Commands/Include/CmdIDBottleCheck.h"
#include "Scheduler/Commands/Include/CmdIDSealingCheck.h"
#include "Scheduler/Commands/Include/CmdRVSetTempCtrlOFF.h"
#include "Scheduler/Commands/Include/CmdRTSetTempCtrlOFF.h"
#include "Scheduler/Commands/Include/CmdOvenSetTempCtrlOFF.h"
#include "Scheduler/Commands/Include/CmdALSetTempCtrlOFF.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "HimalayaDataManager/Include/DataManager.h"
#include "HimalayaDataContainer/Containers/ProgramSettings/Include/ProgramSettings.h"
#include "Global/Include/GlobalDefines.h"
#include <Scheduler/Commands/Include/CmdRTLock.h>
#include <Scheduler/Commands/Include/CmdRTUnlock.h>
#include <HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdCurrentProgramStepInfor.h>
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAcknowledge.h"
#include "HimalayaDataContainer/Containers/ReagentStations/Commands/Include/CmdUpdateStationReagentStatus.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdKeepCassetteCount.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelected.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelectedReply.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdStationSuckDrain.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAcknowledge.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdLockStatus.h"
#include "HimalayaDataContainer/Containers/UserSettings/Commands/Include/CmdQuitAppShutdown.h"
#include "NetCommands/Include/CmdSystemAction.h"
#include "float.h"
#include "Global/Include/EventObject.h"
#include "Scheduler/Include/HeatingStrategy.h"


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


    MOCK_METHOD0(ALSetPressureEventHandlerCtrlOFF, ReturnCode_t());

    MOCK_METHOD0(ALReleasePressure, ReturnCode_t());

    MOCK_METHOD0(ALPressure, ReturnCode_t());
    MOCK_METHOD0(ALVaccum, ReturnCode_t());

    MOCK_METHOD1(ALDraining, ReturnCode_t(quint32 DelayTime));

    MOCK_METHOD2(ALFilling, ReturnCode_t(quint32 DelayTime, bool EnableInsufficientCheck));

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
    MOCK_METHOD1(IDSealingCheck, qreal(qreal ThresholdPressure));

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

class TestSchedulerController : public QObject {
    Q_OBJECT
public:
    TestSchedulerController()
        :   mp_IDeviceProcessing(new MockIDeviceProcessing()),
            m_pSchedulerMainController(new SchedulerMainThreadController(THREAD_ID_SCHEDULER)),
            mp_SchdCmdProcessor(new Scheduler::SchedulerCommandProcessor<MockIDeviceProcessing>(m_pSchedulerMainController)),
            mp_HMThreadController(new Himalaya::HimalayaMasterThreadController())
    {
        m_pSchedulerMainController->SetSchedCommandProcessor(mp_SchdCmdProcessor);
        Global::SystemPaths::Instance().SetSettingsPath("../../../Main/Build/Settings");
        mp_DataManager = new DataManager::CDataManager(mp_HMThreadController);

        m_pSchedulerMainController->DataManager(mp_DataManager);

        reinterpret_cast<SchedulerCommandProcessor<MockIDeviceProcessing> *>(mp_SchdCmdProcessor)->SetIDeviceProcessing(mp_IDeviceProcessing);
    }

    void misc()
    {
//        m_pSchedulerMainController->
        m_pSchedulerMainController->OnGoReceived();
//        m_pSchedulerMainController->UpdateStationReagentStatus();
        m_pSchedulerMainController->MoveRVToInit();
        m_pSchedulerMainController->SealingCheck();
        m_pSchedulerMainController->MoveRVToInit();
        m_pSchedulerMainController->ShutdownRetortHeater();
        m_pSchedulerMainController->ReleasePressure();
        m_pSchedulerMainController->Fill();
        m_pSchedulerMainController->StopFill();
        m_pSchedulerMainController->Drain();
        m_pSchedulerMainController->StopDrain();
        m_pSchedulerMainController->Pressure();
        m_pSchedulerMainController->Vaccum();

        m_pSchedulerMainController->Soak();
        m_pSchedulerMainController->Abort();
        m_pSchedulerMainController->SelfTest(DCL_ERR_FCT_CALL_SUCCESS);
        m_pSchedulerMainController->StepStart();

        EXPECT_NE(m_pSchedulerMainController->GetOvenHeatingTime(), true);
        EXPECT_EQ(m_pSchedulerMainController->CheckStepTemperature(), true);


        m_pSchedulerMainController->Pause();
        m_pSchedulerMainController->DequeueNonDeviceCommand();
    }

    void programAction()
    {
        QDateTime m_EndDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(1);
        QString programID = "L02";

        {
            MsgClasses::CmdProgramSelected command(5000, programID, 1);
            Global::tRefType ref = m_pSchedulerMainController->GetNewCommandRef();
            m_pSchedulerMainController->OnProgramSelected(ref, command);
            QCOREAPPLICATION_EXEC(20000);
        }

        {
            MsgClasses::CmdProgramAction cmd(5000, programID, DataManager::PROGRAM_START, m_EndDateTime);
            Global::tRefType ref = m_pSchedulerMainController->GetNewCommandRef();
            m_pSchedulerMainController->OnProgramAction(ref, cmd);
            QCOREAPPLICATION_EXEC(20000);
        }

        {
            MsgClasses::CmdProgramAction cmd(5000, programID, DataManager::PROGRAM_PAUSE, m_EndDateTime);
            Global::tRefType ref = m_pSchedulerMainController->GetNewCommandRef();
            m_pSchedulerMainController->OnProgramAction(ref, cmd);
            QCOREAPPLICATION_EXEC(5000);
        }

        {
            MsgClasses::CmdProgramAction cmd(5000, programID, DataManager::PROGRAM_DRAIN, m_EndDateTime);
            Global::tRefType ref = m_pSchedulerMainController->GetNewCommandRef();
            m_pSchedulerMainController->OnProgramAction(ref, cmd);
            QCOREAPPLICATION_EXEC(20000);
        }

        {
            MsgClasses::CmdProgramAction cmd(5000, programID, DataManager::PROGRAM_ABORT, m_EndDateTime);
            Global::tRefType ref = m_pSchedulerMainController->GetNewCommandRef();
            m_pSchedulerMainController->OnProgramAction(ref, cmd);
            QCOREAPPLICATION_EXEC(5000);
        }
    }

    void CleanupAndDestroyObjects()
    {
        m_pSchedulerMainController->OnStopReceived();
        m_pSchedulerMainController->CleanupAndDestroyObjects();
    }

    ~TestSchedulerController()
    {
        delete mp_HMThreadController;
        delete mp_IDeviceProcessing;
        delete m_pSchedulerMainController;
        delete mp_SchdCmdProcessor;
        delete mp_DataManager;
    }

private:

public slots:

signals:

public:
    Himalaya::HimalayaMasterThreadController *mp_HMThreadController;
    DataManager::CDataManager                *mp_DataManager;
    MockIDeviceProcessing                    *mp_IDeviceProcessing;
    SchedulerMainThreadController            *m_pSchedulerMainController;
    SchedulerCommandProcessorBase            *mp_SchdCmdProcessor;
private:

};

/****************************************************************************/
/**
 * \brief Test class for TestSchedulerMainThreadController class.
 */
/****************************************************************************/
class TestSchedulerMainThreadController : public QObject {
    Q_OBJECT
public:
    TestSchedulerMainThreadController()
    {
       schedulerController = new TestSchedulerController;
       mp_IDeviceProcessing = schedulerController->mp_IDeviceProcessing;
       m_pSchedulerMainController = schedulerController->m_pSchedulerMainController;
       mp_SchdCmdProcessor = schedulerController->mp_SchdCmdProcessor;
    }

    ~TestSchedulerMainThreadController()
    {
       delete schedulerController;
    }

private:
    TestSchedulerController *schedulerController;
    MockIDeviceProcessing    *mp_IDeviceProcessing;
    SchedulerMainThreadController *m_pSchedulerMainController;
    SchedulerCommandProcessorBase  *mp_SchdCmdProcessor;

private slots:
    void initTestCase();
    void caseMisc();
    void caseProgramAction();
    void cleanupTestCase();
};

void TestSchedulerMainThreadController::initTestCase()
{
    m_pSchedulerMainController->CreateAndInitializeObjects();
    QCOREAPPLICATION_EXEC(500);

    EXPECT_CALL(*mp_IDeviceProcessing, StartConfigurationService())
            .Times(AtLeast(1))
            .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

    EXPECT_CALL(*mp_IDeviceProcessing, RVReqMoveToInitialPosition())
            .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

    EXPECT_CALL(*mp_IDeviceProcessing, PerTurnOnMainRelay())
//            .Times(AtLeast(1))
            .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

    EXPECT_CALL(*mp_IDeviceProcessing, RTStartTemperatureControlWithPID(_, _, _, _, _, _, _))
//            .Times(AtLeast(1))
            .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

    EXPECT_CALL(*mp_IDeviceProcessing, RVStartTemperatureControlWithPID(_, _, _, _, _, _))
//            .Times(AtLeast(1))
            .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

    EXPECT_CALL(*mp_IDeviceProcessing, OvenStartTemperatureControlWithPID(_, _, _, _, _, _, _))
//            .Times(AtLeast(1))
            .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

    EXPECT_CALL(*mp_IDeviceProcessing, ALStartTemperatureControlWithPID(_, _, _, _, _, _, _))
//            .Times(AtLeast(1))
            .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

    EXPECT_CALL(*mp_IDeviceProcessing, ALGetRecentPressure())
//                .Times(AtLeast(1))
            .WillRepeatedly(Return(100));

    EXPECT_CALL(*mp_IDeviceProcessing, ALSetPressureDrift(_))
//            .Times(AtLeast(1))
            .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

//-----------------------------------------------------------------------
    EXPECT_CALL(*mp_IDeviceProcessing, ALTurnOnFan())
//            .Times(AtLeast(1))
            .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

    EXPECT_CALL(*mp_IDeviceProcessing, ALTurnOffFan())
//                .Times(AtLeast(1))
            .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

    EXPECT_CALL(*mp_IDeviceProcessing, ALGetRecentPressure())
//            .Times(AtLeast(1))
            .WillRepeatedly(Return(10.0));

    EXPECT_CALL(*mp_IDeviceProcessing, ALGetRecentTemperature(_, _))
//            .Times(AtLeast(3))
            .WillRepeatedly(Return(10.0));

    EXPECT_CALL(*mp_IDeviceProcessing, RVGetRecentTemperature(_))
//            .Times(AtLeast(2))
            .WillRepeatedly(Return(10.0));

    EXPECT_CALL(*mp_IDeviceProcessing, RTGetRecentTemperature(_, _))
//            .Times(AtLeast(2))
            .WillRepeatedly(Return(10.0));

    EXPECT_CALL(*mp_IDeviceProcessing, RVReqActRVPosition())
//            .Times(AtLeast(1))
            .WillRepeatedly(Return(RV_TUBE_1));

    EXPECT_CALL(*mp_IDeviceProcessing, OvenGetRecentTemperature(_, _))
//            .Times(AtLeast(2))
            .WillRepeatedly(Return(10.0));

    EXPECT_CALL(*mp_IDeviceProcessing, OvenGetRecentLidStatus())
//            .Times(AtLeast(1))
            .WillRepeatedly(Return(10.0));

    EXPECT_CALL(*mp_IDeviceProcessing, RTGetRecentLockStatus())
//            .Times(AtLeast(1))
            .WillRepeatedly(Return(1));
    EXPECT_CALL(*mp_IDeviceProcessing, IDSealingCheck(_))
//            .Times(AtLeast(1))
            .WillRepeatedly(Return(1.0));

    EXPECT_CALL(*mp_IDeviceProcessing, ALBreakAllOperation())
//            .Times(AtLeaCRsStandbyWithTissuest(1))
            .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

    EXPECT_CALL(*mp_IDeviceProcessing, ALAllStop())
//            .Times(AtLeast(1))
            .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

    EXPECT_CALL(*mp_IDeviceProcessing, ALFilling(_, _))
//            .Times(AtLeast(1))
            .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

    EXPECT_CALL(*mp_IDeviceProcessing, ALDraining(_))
//            .Times(AtLeast(1))
            .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

    EXPECT_CALL(*mp_IDeviceProcessing, ALPressure())
//            .Times(AtLeast(1))
            .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

    EXPECT_CALL(*mp_IDeviceProcessing, ALVaccum())
//            .Times(AtLeast(1))
            .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

    EXPECT_CALL(*mp_IDeviceProcessing, IDBottleCheck(_, _))
            .Times(AtLeast(1))
            .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

    // --------------------------------------------------------------------

    mp_IDeviceProcessing->InitializationFinished();
    QCOREAPPLICATION_EXEC(1000 * 5);
    mp_IDeviceProcessing->ConfigurationFinished();
    QCOREAPPLICATION_EXEC(1000 * 5);
}

void TestSchedulerMainThreadController::caseMisc()
{
    schedulerController->misc();
}

void TestSchedulerMainThreadController::caseProgramAction()
{
    schedulerController->programAction();
}


void TestSchedulerMainThreadController::cleanupTestCase()
{
    schedulerController->CleanupAndDestroyObjects();
}

} // end namespace Scheduler

int main(int argc, char*argv[])
{
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    ::testing::InitGoogleMock(&argc, argv);
    QCoreApplication app(argc, argv);
    Scheduler::TestSchedulerMainThreadController tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "TestSchedulerMainThreadController.moc"
