/****************************************************************************/
/*! \file TestSchedIDPExchange.cpp
 *
 *  \brief Implementation file for class TestSchedIDPExchange.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2014-01-27
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

inline void waitTime(qint8 time)
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
 * \brief Test class for TestSchedIDPExchange class.
 */
/****************************************************************************/
class TestSchedIDPExchange : public QObject {
    Q_OBJECT
public:
    TestSchedIDPExchange()
        :mp_IDeviceProcessing(new MockIDeviceProcessing()),
        m_pSchedulerMainController(new SchedulerMainThreadController(THREAD_ID_SCHEDULER))
    {
        Global::SystemPaths::Instance().SetSettingsPath("../../../Main/Build/Settings");
        mp_HMThreadController = new Himalaya::HimalayaMasterThreadController();
        mp_DataManager = new DataManager::CDataManager(mp_HMThreadController);
        m_pSchedulerMainController->DataManager(mp_DataManager);
        dynamic_cast<SchedulerCommandProcessor<MockIDeviceProcessing>*>(m_pSchedulerMainController->GetSchedCommandProcessor())->SetIDeviceProcessing(mp_IDeviceProcessing);

        //Set google-mock expections
        EXPECT_CALL(*mp_IDeviceProcessing, StartConfigurationService())
  //              .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, RVReqMoveToInitialPosition(_))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, PerTurnOnMainRelay())
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, RTStartTemperatureControlWithPID(_, _, _, _, _, _, _))
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, RVStartTemperatureControlWithPID(_, _, _, _, _, _))
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, OvenStartTemperatureControlWithPID(_, _, _, _, _, _, _))
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, ALStartTemperatureControlWithPID(_, _, _, _, _, _, _))
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, ALGetRecentPressure())
                .Times(AtLeast(1))
                .WillRepeatedly(Return(2.0));

        EXPECT_CALL(*mp_IDeviceProcessing, ALGetRecentTemperature(_,_))
                .Times(AtLeast(1))
                .WillRepeatedly(Return(80));

        EXPECT_CALL(*mp_IDeviceProcessing, RVGetRecentTemperature(_))
                .Times(AtLeast(1))
                .WillRepeatedly(Return(80));

        EXPECT_CALL(*mp_IDeviceProcessing, RVReqActRVPosition())
                .Times(AtLeast(1))
                .WillRepeatedly(Return(RV_TUBE_1));

        EXPECT_CALL(*mp_IDeviceProcessing, RTGetRecentTemperature(_,_))
                .Times(AtLeast(1))
                .WillRepeatedly(Return(60));

        EXPECT_CALL(*mp_IDeviceProcessing, OvenGetRecentTemperature(_,_))
                .Times(AtLeast(1))
                .WillRepeatedly(Return(60));

        EXPECT_CALL(*mp_IDeviceProcessing, OvenGetRecentLidStatus())
                .Times(AtLeast(1))
                .WillRepeatedly(Return(0));

        EXPECT_CALL(*mp_IDeviceProcessing, RTGetRecentLockStatus())
                .Times(AtLeast(1))
                .WillRepeatedly(Return(1));

        EXPECT_CALL(*mp_IDeviceProcessing, GetSensorCurrent(_,_))
                .Times(AtLeast(1))
                .WillRepeatedly(Return(2400));

        EXPECT_CALL(*mp_IDeviceProcessing, IDGetSlaveVoltage(_))
                .Times(AtLeast(1))
                .WillRepeatedly(Return(24000));

        EXPECT_CALL(*mp_IDeviceProcessing, IDGetSlaveCurrent(_))
                .Times(AtLeast(1))
                .WillRepeatedly(Return(2400));

        EXPECT_CALL(*mp_IDeviceProcessing, OvenGetHeatingStatus(_))
                .Times(AtLeast(1))
                .WillRepeatedly(Return(1));

        EXPECT_CALL(*mp_IDeviceProcessing, ALSetPressureDrift(_))
//                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        ReportError_t reportErr;
        memset(&reportErr, 0,sizeof(reportErr));
        EXPECT_CALL(*mp_IDeviceProcessing, GetSlaveModuleReportError(_,_,_))
                .Times(AtLeast(1))
                .WillRepeatedly(Return(reportErr));

        EXPECT_CALL(*mp_IDeviceProcessing, ALTurnOnFan())
                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));
    }

    ~TestSchedIDPExchange()
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
    void init();
    /****************************************************************************/
    /**
     * \brief Called after each testfunction was executed.
     */
    /****************************************************************************/
    void cleanup();
    /****************************************************************************/
    /**
     * \brief Called after last testfunction was executed.
     */
    /****************************************************************************/
    void cleanupTestCase();
    /****************************************************************************/

    void UTAll();
signals:
    void SendStop();

}; // end class TestEventCSVInfo


void TestSchedIDPExchange::UTAll()
{
    mp_IDeviceProcessing->InitializationFinished();
    waitTime(2);
    mp_IDeviceProcessing->ConfigurationFinished();
    waitTime(10);
    emit SendStop();
}

/******************************************************************ls**********/
void TestSchedIDPExchange::initTestCase()
{

}

/****************************************************************************/
void TestSchedIDPExchange::init()
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
void TestSchedIDPExchange::cleanup()
{
}

/****************************************************************************/
void TestSchedIDPExchange::cleanupTestCase()
{
   // emit SendStop();
   // waitTime(20);
}

} // end namespace EventHandler

//QTEST_MAIN(Scheduler::TestSchedulerCommand)
int main(int argc, char*argv[])
{
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    ::testing::InitGoogleMock(&argc, argv);
    QCoreApplication app(argc, argv);
    Scheduler::TestSchedIDPExchange tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "TestSchedIDPExchange.moc"

