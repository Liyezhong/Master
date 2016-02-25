/****************************************************************************/
/*! \file   TestSchedulerMainThreadController.cpp
 *
 *  \brief  Implementation file for class TestSchedulerMainThreadController.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2014-11-17
 *  $Author:    $ Songtao Yu
 *
 *  \b Company:
 *
 *       Leica Microsystems Shanghai.
 *
 *  (C) Copyright 2014 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include <QTest>
#include <gmock/gmock.h>
#include "HimalayaMasterThread/Include/ThreadIDs.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "Scheduler/Test/Mock/MockIDeviceProcessing.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "HimalayaMasterThread/Include/HimalayaMasterThreadController.h"
#include "HimalayaDataManager/Include/DataManager.h"

using::testing::Return;
using::testing::AtLeast;
using::testing::_;
using::testing::Lt;

inline void waitTime(qint32 time)
{
    QTime delayTime = QTime::currentTime().addMSecs(time*1000);
    while (QTime::currentTime() < delayTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

namespace Scheduler {

/****************************************************************************/
/**
 * \brief Test class for TestSchedulerMainThreadController class.
 */
/****************************************************************************/
class TestSchedulerMainThreadController : public QObject {
    Q_OBJECT
public:
    TestSchedulerMainThreadController()
        :mp_IDeviceProcessing(new MockIDeviceProcessing()),
         m_pSchedulerMainController(new SchedulerMainThreadController(THREAD_ID_SCHEDULER))
    {
        Global::SystemPaths::Instance().SetSettingsPath("../../../Main/Build/Settings");
        mp_HMThreadController = new Himalaya::HimalayaMasterThreadController();
        mp_DataManager = new DataManager::CDataManager(mp_HMThreadController);
        m_pSchedulerMainController->DataManager(mp_DataManager);
        dynamic_cast<SchedulerCommandProcessor<MockIDeviceProcessing>*>(m_pSchedulerMainController->GetSchedCommandProcessor())->SetIDeviceProcessing(mp_IDeviceProcessing);

        EXPECT_CALL(*mp_IDeviceProcessing, ALSetPressureDrift(_))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, RVReqMoveToInitialPosition(_))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, ALAllStop())
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, ALBreakAllOperation())
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, ALPressure(_))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, ALReleasePressure())
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, ALVaccum(_))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, ALFilling(_,_,_))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, ALDraining(_,_,_))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, ResetActiveCarbonFilterLifeTime())
                .WillRepeatedly(Return());
        EXPECT_CALL(*mp_IDeviceProcessing, PerTurnOnMainRelay())
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));
    }

    ~TestSchedulerMainThreadController()
    {
        delete mp_HMThreadController;
        mp_HMThreadController = NULL;

        delete mp_DataManager;
        mp_DataManager = NULL;

        delete mp_IDeviceProcessing;
        mp_IDeviceProcessing = NULL;

        m_pSchedulerMainController->deleteLater();
        m_pSchedulerMainController = NULL;
    }

private:
    Himalaya::HimalayaMasterThreadController*   mp_HMThreadController;
    DataManager::CDataManager*                  mp_DataManager;
    MockIDeviceProcessing*                      mp_IDeviceProcessing;
    SchedulerMainThreadController*              m_pSchedulerMainController;
	
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

    void TestSlots();
    /****************************************************************************/
    /**
     * \brief Called after each testfunction was executed.
     */
    /****************************************************************************/
    //void cleanup();
    /****************************************************************************/
    /**
     * \brief Called after last testfunction was executed.
     */
    /****************************************************************************/
    void cleanupTestCase();
    /****************************************************************************/

    //void UTAll();

signals:
    void SendStop();

}; // end class TestEventCSVInfo


void TestSchedulerMainThreadController::TestSlots()
{
    m_pSchedulerMainController->DevProcDestroyed();

    DataManager::CModule module;
    m_pSchedulerMainController->ReportGetServiceInfo(DCL_ERR_FCT_CALL_SUCCESS, module, "TemperatureControl");

    m_pSchedulerMainController->ResetActiveCarbonFilterLifetime();

    //m_pSchedulerMainController->OnReportFillingTimeOut2Min();

    //m_pSchedulerMainController->OnReportDrainingTimeOut2Min();

    QDateTime curTime = Global::AdjustedTime::Instance().GetCurrentDateTime();
    m_pSchedulerMainController->OnReportError(200, 0x01, 0x01, 0x01, curTime);

    m_pSchedulerMainController->CleanupAndDestroyObjects();

    m_pSchedulerMainController->OnPowerFail(Global::POWER_FAIL_STAGE_1);

    Global::AckOKNOK ack;
    m_pSchedulerMainController->OnAcknowledge(1,ack);
    ControlCommandType_t ctrlCmd = m_pSchedulerMainController->PeekNonDeviceCommand();
    SchedulerCommandShPtr_t cmd;
    m_pSchedulerMainController->PopDeviceControlCmdQueue(cmd);
    m_pSchedulerMainController->HandlePowerFailure(ctrlCmd, cmd);

    m_pSchedulerMainController->UpdateStationReagentStatus(true);

    ctrlCmd = m_pSchedulerMainController->PeekNonDeviceCommand();
    m_pSchedulerMainController->PopDeviceControlCmdQueue(cmd);
    //m_pSchedulerMainController->HandleRunState(ctrlCmd, cmd);

    ProgramStepInfor stepInfo;
    m_pSchedulerMainController->GetNextProgramStepInformation("", stepInfo);

    m_pSchedulerMainController->GetStationIDFromProgramStep(0);

    m_pSchedulerMainController->PrepareProgramStationList("",0);

    QList<QString> stationList;
    m_pSchedulerMainController->GetSafeReagentStationList("RG3", stationList);

    m_pSchedulerMainController->SendTissueProtectMsg(false);

    m_pSchedulerMainController->SendCoverLidOpenMsg();

    m_pSchedulerMainController->WhichStepHasNoSafeReagent("");

    //m_pSchedulerMainController->GetLeftProgramStepsNeededTime("", 1);

    m_pSchedulerMainController->GetCurrentProgramStepNeededTime("");

    m_pSchedulerMainController->GetReagentName("RG6");

    m_pSchedulerMainController->GetReagentGroupID("RG3");

    //QCOMPARE(m_pSchedulerMainController->BottleCheck(100), false);

    m_pSchedulerMainController->RcBottleCheckI();

    m_pSchedulerMainController->MoveRVToInit();

    m_pSchedulerMainController->ReleasePressure();

   //m_pSchedulerMainController->OnEnterPssmProcessing();

   m_pSchedulerMainController->IsRVRightPosition(TUBE_POS), true;

   m_pSchedulerMainController->MoveRV(TUBE_POS), false;

    //m_pSchedulerMainController->Fill();

    m_pSchedulerMainController->ShutdownFailedHeaters();

    m_pSchedulerMainController->RestartFailedHeaters();

    //m_pSchedulerMainController->CheckSensorTempOverange();

    //m_pSchedulerMainController->FillRsTissueProtect("S1", false, false);

    //m_pSchedulerMainController->StopFillRsTissueProtect("S1");

   // m_pSchedulerMainController->CheckSlaveTempModulesCurrentRange(2);

    //m_pSchedulerMainController->OnStopFill();

   m_pSchedulerMainController->RCDrain();

  // m_pSchedulerMainController->Drain();

  // m_pSchedulerMainController->RcDrainAtOnce();

   //m_pSchedulerMainController->OnBeginDrain();

   m_pSchedulerMainController->OnStopDrain();

   m_pSchedulerMainController->Pressure();

   m_pSchedulerMainController->HighPressure();

   m_pSchedulerMainController->Vaccum();

   m_pSchedulerMainController->AllStop();

    //m_pSchedulerMainController->Pause();

    m_pSchedulerMainController->m_TempALLevelSensor = 100.0;

    QCOMPARE(m_pSchedulerMainController->CheckLevelSensorTemperature(30.0), true);

    QCOMPARE(m_pSchedulerMainController->GetRVTubePositionByStationID(""), RV_UNDEF);

    QCOMPARE(m_pSchedulerMainController->GetRVSealPositionByStationID(""), RV_UNDEF);

    m_pSchedulerMainController->GetPreTestTime();

    m_pSchedulerMainController->IsLastStep(1,"");

    m_pSchedulerMainController->m_CurErrEventID = DCL_ERR_DEV_RETORT_LEVELSENSOR_HEATING_OVERTIME;
    QCOMPARE(m_pSchedulerMainController->GetFailerHeaterType(), LEVELSENSOR);

    m_pSchedulerMainController->OnSystemError();

    //m_pSchedulerMainController->OnFillingHeatingRV();

    m_pSchedulerMainController->OnPreTestDone();

    m_pSchedulerMainController->SendPowerFailureMsg();
}

/******************************************************************ls**********/
void TestSchedulerMainThreadController::initTestCase()
{
    m_pSchedulerMainController->CreateAndInitializeObjects();


    // Move Scheduler into one new thread and start up
    QThread* schedulerThread = new QThread();
    m_pSchedulerMainController->moveToThread(schedulerThread);
    CONNECTSIGNALSLOT(schedulerThread, started(), m_pSchedulerMainController, Go());
    CONNECTSIGNALSLOT(this, SendStop(), m_pSchedulerMainController, Stop());
    CONNECTSIGNALSLOT(m_pSchedulerMainController, destroyed(), schedulerThread, quit());
    CONNECTSIGNALSLOT(m_pSchedulerMainController, destroyed(), schedulerThread, deleteLater());
    schedulerThread->start();
}

/****************************************************************************/
void TestSchedulerMainThreadController::cleanupTestCase()
{
    emit SendStop();
}

} // end namespace EventHandler

int main(int argc, char*argv[])
{
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    ::testing::InitGoogleMock(&argc, argv);
    QCoreApplication app(argc, argv);
    Scheduler::TestSchedulerMainThreadController tc;
    return QTest::qExec(&tc, argc, argv);
}


#include "TestSchedulerMainThreadController.moc"

