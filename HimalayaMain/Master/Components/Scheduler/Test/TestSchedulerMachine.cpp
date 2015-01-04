/****************************************************************************/
/*! \file   TestSchedulerMachine.cpp
 *
 *  \brief  Implementation file for class TestSchedulerMachine.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2014-11-18
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
#include "Scheduler/Include/SchedulerMachine.h"

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
class TestSchedulerMachine : public QObject {
    Q_OBJECT
public:
    TestSchedulerMachine()
        :mp_IDeviceProcessing(new MockIDeviceProcessing()),
         m_pSchedulerMainController(new SchedulerMainThreadController(THREAD_ID_SCHEDULER)),
         m_pSchedulerMachine(new CSchedulerStateMachine(m_pSchedulerMainController))
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

        EXPECT_CALL(*mp_IDeviceProcessing, ALFilling(_,_))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, ALDraining(_,_,_))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));
    }

    ~TestSchedulerMachine()
    {
        delete mp_HMThreadController;
        mp_HMThreadController = NULL;

        delete mp_DataManager;
        mp_DataManager = NULL;

        delete mp_IDeviceProcessing;
        mp_IDeviceProcessing = NULL;

        m_pSchedulerMainController->deleteLater();
        m_pSchedulerMainController = NULL;

        delete m_pSchedulerMachine;
        m_pSchedulerMachine = NULL;
    }

private:
    Himalaya::HimalayaMasterThreadController*   mp_HMThreadController;
    DataManager::CDataManager*                  mp_DataManager;
    MockIDeviceProcessing*                      mp_IDeviceProcessing;
    SchedulerMainThreadController*              m_pSchedulerMainController;
    CSchedulerStateMachine*                     m_pSchedulerMachine;
	
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

    void TestAPIs();
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


void TestSchedulerMachine::TestAPIs()
{
    m_pSchedulerMachine->Stop();
    //m_pSchedulerMachine->SendRunSignal();
    //m_pSchedulerMachine->SendCleaningSignal();
    //m_pSchedulerMachine->SendRunComplete();
    m_pSchedulerMachine->SendResumeFillingRVRodHeating();
    m_pSchedulerMachine->SendResumeFillingLevelSensorHeating();
    m_pSchedulerMachine->SendResumeFiling();
    m_pSchedulerMachine->SendResumeRVMoveToSeal();
    m_pSchedulerMachine->SendResumeProcessing();
    m_pSchedulerMachine->SendResumeProcessingSR();
    m_pSchedulerMachine->SendResumeRVMoveTube();
    m_pSchedulerMachine->SendResumeDraining();
    m_pSchedulerMachine->SendResumeRVPosChange();
    m_pSchedulerMachine->SendResumeDryStep();
    m_pSchedulerMachine->SendResumeAborting();
   // m_pSchedulerMachine->SendErrorSignal();
    m_pSchedulerMachine->GetPreviousState();
    m_pSchedulerMachine->NotifyStInitOK();
    m_pSchedulerMachine->NotifyStTempOK();
    m_pSchedulerMachine->NotifyStCurrentOK();
    m_pSchedulerMachine->NotifyStVoltageOK();
    m_pSchedulerMachine->NotifyStRVPositionOK();
    m_pSchedulerMachine->NotifyStPressureOK();
    m_pSchedulerMachine->NotifyStSealingOK();
    m_pSchedulerMachine->NotifyStGetStationcheckResult();
    m_pSchedulerMachine->NotifyStStationLeft();
    m_pSchedulerMachine->NotifyStStationOK();
    m_pSchedulerMachine->NotifyStDone();
    m_pSchedulerMachine->NotifyTempsReady();
    m_pSchedulerMachine->NotifyLevelSensorHeatingReady();
    m_pSchedulerMachine->NotifyRVRodHeatingReady();
    m_pSchedulerMachine->NotifyRVMoveToSealReady();
    m_pSchedulerMachine->NotifyRVMoveToTubeReady();
    m_pSchedulerMachine->NotifyHitTubeBefore();
    m_pSchedulerMachine->NotifyFillFinished();
    m_pSchedulerMachine->NotifyHitSeal();
    m_pSchedulerMachine->NotifyProcessingFinished();
    m_pSchedulerMachine->NotifyHitTubeAfter();
    m_pSchedulerMachine->NotifyDrainFinished();
    m_pSchedulerMachine->NotifyStepFinished();
    m_pSchedulerMachine->NotifyProgramFinished();
    m_pSchedulerMachine->NotifyEnterCleaningDryStep();
    m_pSchedulerMachine->NotifyStepProgramFinished();
    m_pSchedulerMachine->NotifyError();
    m_pSchedulerMachine->NotifyAbort();
    m_pSchedulerMachine->NotifyRsRvMoveToInitPositionFinished();
    m_pSchedulerMachine->NotifyRcReport();
    m_pSchedulerMachine->NotifyRsReleasePressure();
    m_pSchedulerMachine->NotifyRsShutdownFailedHeater();
    m_pSchedulerMachine->NotifyRsShutdownFailedHeaterFinished();
    m_pSchedulerMachine->NotifyResumeDrain();
    m_pSchedulerMachine->EnterRsStandBy();
    m_pSchedulerMachine->EnterRsHeatingErr30SRetry();
    m_pSchedulerMachine->EnterRsPressureOverRange3SRetry();
    m_pSchedulerMachine->EnterRsTSensorErr3MinRetry();
    m_pSchedulerMachine->EnterRsStandByWithTissue();
    m_pSchedulerMachine->EnterRcPressure();
    m_pSchedulerMachine->EnterRcVacuum();
    m_pSchedulerMachine->EnterRcFilling();
    m_pSchedulerMachine->EnterRcDraining();
    m_pSchedulerMachine->EnterRsDrainAtOnce();
    m_pSchedulerMachine->EnterRcBottleCheckI();
    m_pSchedulerMachine->EnterRsFillingAfterFlush();
    m_pSchedulerMachine->EnterRsCheckBlockage();
    m_pSchedulerMachine->EnterRsPause();
    m_pSchedulerMachine->EnterRsRVWaitingTempUp();
    m_pSchedulerMachine->EnterRsTissueProtect();
    m_pSchedulerMachine->EnterRcCheckRTLock();
    m_pSchedulerMachine->EnterRsReagentCheck();
    m_pSchedulerMachine->EnterRsRVMoveToSealPosition();
}

/****************************************************************************/
void TestSchedulerMachine::initTestCase()
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
void TestSchedulerMachine::cleanupTestCase()
{
    emit SendStop();
}

} // end namespace EventHandler

//QTEST_MAIN(Scheduler::TestSchedulerMainThreadController)
int main(int argc, char*argv[])
{
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    ::testing::InitGoogleMock(&argc, argv);
    QCoreApplication app(argc, argv);
    Scheduler::TestSchedulerMachine tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "TestSchedulerMachine.moc"

