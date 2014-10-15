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
    SchedulerMainThreadController*              mp_SchedulerMainController;
    HeatingStrategy*                            mp_HeatingStrategy;

}; // end class TestHeatingStrategy


TestHeatingStrategy::TestHeatingStrategy()
    :mp_SchedulerMainController(new SchedulerMainThreadController(THREAD_ID_SCHEDULER))
{
    Global::SystemPaths::Instance().SetSettingsPath("../../../Main/Build/Settings");

    mp_HMThreadController = new Himalaya::HimalayaMasterThreadController();
    mp_DataManager = new DataManager::CDataManager(mp_HMThreadController);
    mp_SchedulerMainController->DataManager(mp_DataManager);

    mp_HeatingStrategy = new HeatingStrategy(mp_SchedulerMainController, mp_SchedulerMainController->GetSchedCommandProcessor(), mp_DataManager);
}

TestHeatingStrategy::~TestHeatingStrategy()
{
    delete mp_HMThreadController;
    mp_HMThreadController = NULL;

    delete mp_DataManager;
    mp_DataManager = NULL;

    delete mp_HeatingStrategy;
    mp_HeatingStrategy = NULL;
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


    //test set the temperature
    scenario = 251;
    ReturnCode_t ret = mp_HeatingStrategy->RunHeatingStrategy(strctHWMonitor, scenario);
    ASSERT_EQ(ret, DCL_ERR_FCT_CALL_SUCCESS);

    //Test The current temperature exceeds max temperature for each sensor
    strctHWMonitor.TempALLevelSensor = 135.0;
    scenario = 1;
    ret = mp_HeatingStrategy->RunHeatingStrategy(strctHWMonitor, scenario);
    ASSERT_EQ(ret, DCL_ERR_DEV_LEVELSENSOR_TEMPERATURE_OVERRANGE);

    strctHWMonitor.TempOvenTop = 133.0;
    scenario = 4;
    ret = mp_HeatingStrategy->RunHeatingStrategy(strctHWMonitor, scenario);
    ASSERT_EQ(ret, DCL_ERR_DEV_WAXBATH_SENSORUP_HEATING_ABNORMAL);

    strctHWMonitor.TempOvenBottom1 = 133.0;
    scenario = 200;
    ret = mp_HeatingStrategy->RunHeatingStrategy(strctHWMonitor, scenario);
    ASSERT_EQ(ret, DCL_ERR_DEV_WAXBATH_SENSORDOWN1_HEATING_ABNORMAL);

    strctHWMonitor.TempRV1 = 129.0;
    scenario = 200;
    ret = mp_HeatingStrategy->RunHeatingStrategy(strctHWMonitor, scenario);
    ASSERT_EQ(ret, DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR1_OUTOFRANGE);

    strctHWMonitor.TempALTube2 = 133.0;
    scenario = 200;
    ret = mp_HeatingStrategy->RunHeatingStrategy(strctHWMonitor, scenario);
    ASSERT_EQ(ret, DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR2_OUTOFRANGE);
}

void TestHeatingStrategy::initTestCase()
{
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

