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
          m_pSchedulerMainController(new SchedulerMainThreadController(THREAD_ID_SCHEDULER)),
          mp_SchdCmdProcessor(new SchedulerCommandProcessor<MockIDeviceProcessing>(m_pSchedulerMainController))
    {
        Global::SystemPaths::Instance().SetSettingsPath("../../../Main/Build/Settings");
        mp_HMThreadController = new Himalaya::HimalayaMasterThreadController();
        mp_DataManager = new DataManager::CDataManager(mp_HMThreadController);
        m_pSchedulerMainController->SetSchedCommandProcessor(mp_SchdCmdProcessor);
        m_pSchedulerMainController->DataManager(mp_DataManager);
        dynamic_cast<SchedulerCommandProcessor<MockIDeviceProcessing>*>(mp_SchdCmdProcessor)->SetIDeviceProcessing(mp_IDeviceProcessing);

        //Set google-mock expections
        EXPECT_CALL(*mp_IDeviceProcessing, StartConfigurationService())
                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, RVReqMoveToInitialPosition())
                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, PerTurnOnMainRelay())
                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, RTStartTemperatureControlWithPID(_, _, _, _, _, _, _))
                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, RVStartTemperatureControlWithPID(_, _, _, _, _, _))
                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, OvenStartTemperatureControlWithPID(_, _, _, _, _, _, _))
                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, ALStartTemperatureControlWithPID(_, _, _, _, _, _, _))
                .Times(AtLeast(1))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, ALGetRecentPressure())
                .Times(AtLeast(1))
                .WillRepeatedly(Return(100));

        EXPECT_CALL(*mp_IDeviceProcessing, ALSetPressureDrift(_))
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

        delete m_pSchedulerMainController;
        m_pSchedulerMainController = NULL;

        delete mp_SchdCmdProcessor;
        mp_SchdCmdProcessor = NULL;
    }

private:
    Himalaya::HimalayaMasterThreadController*   mp_HMThreadController;
    DataManager::CDataManager*                  mp_DataManager;
    MockIDeviceProcessing*                      mp_IDeviceProcessing;
    SchedulerMainThreadController*              m_pSchedulerMainController;
    SchedulerCommandProcessorBase*              mp_SchdCmdProcessor;


	
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

}; // end class TestEventCSVInfo


void TestSchedIDPExchange::UTAll()
{
    sleep(2);
    mp_IDeviceProcessing->InitializationFinished();
    sleep(2);
    mp_IDeviceProcessing->ConfigurationFinished();
    sleep(30);
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
    //CONNECTSIGNALSLOT(m_pSchedulerMainController, destroyed(), schedulerThread, quit());
    //CONNECTSIGNALSLOT(m_pSchedulerMainController, finished(), schedulerThread, deleteLater());

    schedulerThread->start();
}

/****************************************************************************/
void TestSchedIDPExchange::cleanup()
{
}

/****************************************************************************/
void TestSchedIDPExchange::cleanupTestCase()
{

    //exit(0);
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

