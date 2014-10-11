// ================================================================================================
/*! @file TestHeatingStrategy.cpp
 *
 *  @brief Implementation of class Scheduler::TestHeatingStrategy.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 09.05.2014
 *  $Author:    $ Shufa Li
 *
 *  @b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2014 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 * ================================================================================================
*/
#include <QTest>
#include <gmock/gmock.h>

#include "Scheduler/Include/HeatingStrategy.h"

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
 * \brief Test class for TestHeatingStrategy class.
 */
/****************************************************************************/
class TestHeatingStrategy : public QObject {
    Q_OBJECT
public:
    TestHeatingStrategy();
    ~TestHeatingStrategy();

private slots:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();

    void UTAllCase();

private:
    Himalaya::HimalayaMasterThreadController*   mp_HMThreadController;
    DataManager::CDataManager*                  mp_DataManager;
    MockIDeviceProcessing*                      mp_IDeviceProcessing;
    SchedulerMainThreadController*              mp_SchedulerMainController;
    HeatingStrategy*                            mp_HeatingStrategy;

}; // end class TestHeatingStrategy


TestHeatingStrategy::TestHeatingStrategy()
 :mp_IDeviceProcessing(new MockIDeviceProcessing())
 ,mp_SchedulerMainController(new SchedulerMainThreadController(THREAD_ID_SCHEDULER))
{
    Global::SystemPaths::Instance().SetSettingsPath("../../../Main/Build/Settings");

    mp_HMThreadController = new Himalaya::HimalayaMasterThreadController();
    mp_DataManager = new DataManager::CDataManager(mp_HMThreadController);

    mp_SchedulerMainController->DataManager(mp_DataManager);
    dynamic_cast<SchedulerCommandProcessor<MockIDeviceProcessing>*>(m_pSchedulerMainController->GetSchedCommandProcessor())->SetIDeviceProcessing(mp_IDeviceProcessing);

    mp_HeatingStrategy = new HeatingStrategy(mp_SchedulerMainController, mp_SchdCmdProcessor, mp_DataManager);
}

TestHeatingStrategy::~TestHeatingStrategy()
{
    delete mp_HMThreadController;
    mp_HMThreadController = NULL;

    delete mp_DataManager;
    mp_DataManager = NULL;

    delete mp_HeatingStrategy;
    mp_HeatingStrategy = NULL;

    delete mp_IDeviceProcessing;
    mp_IDeviceProcessing = NULL;

    m_pSchedulerMainController->deleteLater();
    m_pSchedulerMainController = NULL;
}

void TestHeatingStrategy::UTAllCase()
{
    HardwareMonitor_t strctHWMonitor;
    strctHWMonitor.TempRTBottom1 = 0.0;
    strctHWMonitor.TempRTBottom2 = 0.0;
    strctHWMonitor.TempOvenBottom1 = 0.0;
    strctHWMonitor.TempOvenBottom2 = 0.0;
    strctHWMonitor.PressureAL = 0.0;
    strctHWMonitor.TempALLevelSensor = 0.0;
    strctHWMonitor.TempALTube1 = 0.0;
    strctHWMonitor.TempALTube2 = 0.0;
    strctHWMonitor.TempRV1 = 0.0;
    strctHWMonitor.TempRV2 = 0.0;
    strctHWMonitor.TempRTSide = 0.0;
    strctHWMonitor.TempOvenTop = 0.0;
    strctHWMonitor.OvenLidStatus = 0;
    strctHWMonitor.RetortLockStatus = 0;
    qint32 scenario = 0;

    {
        //test set the temperature
        scenario = 251;
        ReturnCode_t ret = mp_HeatingStrategy->RunHeatingStrategy(strctHWMonitor, scenario);
        ASSERT_EQ(ret, DCL_ERR_FCT_CALL_SUCCESS);

        //Test The current temperature exceeds max temperature for each sensor
        strctHWMonitor.TempALLevelSensor = 135.0;
        scenario = 1;
        ret = mp_HeatingStrategy->RunHeatingStrategy(strctHWMonitor, scenario);
        ASSERT_EQ(ret, DCL_ERR_DEV_RETORT_LEVELSENSOR_TEMPERATURE_OVERRANGE);

        strctHWMonitor.TempALLevelSensor = 110.0;
        strctHWMonitor.TempOvenTop = 133.0;
        scenario = 4;
        ret = mp_HeatingStrategy->RunHeatingStrategy(strctHWMonitor, scenario);
        ASSERT_EQ(ret, DCL_ERR_DEV_WAXBATH_TSENSORUP_HEATING_OUTOFTARGETRANGE);

        strctHWMonitor.TempOvenBottom1 = 133.0;
        strctHWMonitor.TempOvenTop = 100;
        scenario = 5;
        ret = mp_HeatingStrategy->RunHeatingStrategy(strctHWMonitor, scenario);
        ASSERT_EQ(ret, DCL_ERR_DEV_WAXBATH_TSENSORDOWN1_HEATING_OUTOFTARGETRANGE);

        strctHWMonitor.TempOvenBottom1 = 99.0;
        strctHWMonitor.TempALTube1 = 133.0;
        strctHWMonitor.TempRV1 = 129.0;
        scenario = 7;
        ret = mp_HeatingStrategy->RunHeatingStrategy(strctHWMonitor, scenario);
        ASSERT_EQ(ret, DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR1_OUTOFRANGE);

        strctHWMonitor.TempALTube1 = 98.0;
        strctHWMonitor.TempALTube2 = 133.0;
        scenario = 8;
        ret = mp_HeatingStrategy->RunHeatingStrategy(strctHWMonitor, scenario);
        ASSERT_EQ(ret, DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR2_OUTOFRANGE);
    }

}

void TestHeatingStrategy::initTestCase()
{
    mp_SchedulerMainController->CreateAndInitializeObjects();
    sleep(1);
    {
        //Set google-mock expections
        EXPECT_CALL(*mp_IDeviceProcessing, StartConfigurationService())
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, RVReqMoveToInitialPosition())
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, PerTurnOnMainRelay())
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, RTStartTemperatureControlWithPID(_, _, _, _, _, _, _))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, RVStartTemperatureControlWithPID(_, _, _, _, _, _))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, OvenStartTemperatureControlWithPID(_, _, _, _, _, _, _))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, ALStartTemperatureControlWithPID(_, _, _, _, _, _, _))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, ALGetRecentPressure())
                .WillRepeatedly(Return(100));

        EXPECT_CALL(*mp_IDeviceProcessing, ALSetPressureDrift(_))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));
    }

    sleep(2);
    mp_IDeviceProcessing->InitializationFinished();
    sleep(2);
    mp_IDeviceProcessing->ConfigurationFinished();
}

void TestHeatingStrategy::init()
{
}

void TestHeatingStrategy::cleanup()
{
}

void TestHeatingStrategy::cleanupTestCase()
{
}

} // end namespace Scheduler

//QTEST_MAIN(Scheduler::TestHeatingStrategy)

int main(int argc, char*argv[])
{
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    ::testing::InitGoogleMock(&argc, argv);
    QCoreApplication app(argc, argv);
    Scheduler::TestHeatingStrategy tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "TestHeatingStrategy.moc"

