// ================================================================================================
/*! @file TestHeatingStrategy.cpp
 *
 *  @brief Implementation of class Scheduler::TestRcReheating.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2014-11-5
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

#include "Scheduler/Include/RcReHeating.h"
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "Scheduler/Test/Mock/MockIDeviceProcessing.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "HimalayaMasterThread/Include/ThreadIDs.h"
#include "HimalayaMasterThread/Include/HimalayaMasterThreadController.h"
#include "HimalayaDataManager/Include/DataManager.h"

using::testing::Return;
using::testing::AtLeast;
using::testing::_;
using::testing::Lt;

namespace Scheduler {

class TestRcReheating : public QObject {
    Q_OBJECT
public:
    TestRcReheating();
    ~TestRcReheating();

private slots:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();

    void UTAllCase();
private:
    Himalaya::HimalayaMasterThreadController*   mp_HMThreadController;
    SchedulerMainThreadController*              mp_SchedulerMainController;
    CSchedulerStateMachine*                     m_pSchedulerMachine;
    MockIDeviceProcessing*                      mp_IDeviceProcessing;
    DataManager::CDataManager*                  mp_DataManager;
    CRcReHeating*                           mp_ReHeating;

}; // end class TestRcReheating


TestRcReheating::TestRcReheating()
    :mp_IDeviceProcessing(new MockIDeviceProcessing())
    ,mp_SchedulerMainController(new SchedulerMainThreadController(THREAD_ID_SCHEDULER))
{
    Global::SystemPaths::Instance().SetSettingsPath("../../../Main/Build/Settings");

    mp_HMThreadController = new Himalaya::HimalayaMasterThreadController();
    mp_DataManager = new DataManager::CDataManager(mp_HMThreadController);
    mp_SchedulerMainController->DataManager(mp_DataManager);
    dynamic_cast<SchedulerCommandProcessor<MockIDeviceProcessing>*>(mp_SchedulerMainController->GetSchedCommandProcessor())->SetIDeviceProcessing(mp_IDeviceProcessing);

    m_pSchedulerMachine = new CSchedulerStateMachine(mp_SchedulerMainController);
    mp_ReHeating = new CRcReHeating(mp_SchedulerMainController, m_pSchedulerMachine);
    m_pSchedulerMachine->Start();
}

TestRcReheating::~TestRcReheating()
{
    delete mp_HMThreadController;
    mp_HMThreadController = NULL;

    delete mp_DataManager;
    mp_DataManager = NULL;

    delete mp_IDeviceProcessing;
    mp_IDeviceProcessing = NULL;

    delete mp_ReHeating;
    mp_ReHeating = NULL;
}

void TestRcReheating::UTAllCase()
{
    if(mp_ReHeating)
    {
        mp_ReHeating->HandleWorkFlow("", DCL_ERR_FCT_CALL_SUCCESS);

        mp_ReHeating->HandleInint();
        //mp_ReHeating->GetRvPosition("Scheduler::RVReqMoveToInitialPosition", DCL_ERR_FCT_CALL_SUCCESS);
        //mp_ReHeating->ProcessDraining("Scheduler::IDForceDraining", DCL_ERR_FCT_CALL_SUCCESS);
    }
}

void TestRcReheating::initTestCase()
{
    mp_SchedulerMainController->CreateAndInitializeObjects();
    //mp_SchedulerMainController->OnDCLConfigurationFinished(DCL_ERR_FCT_CALL_SUCCESS);
    sleep(1);
    {
        //Set google-mock expections
        EXPECT_CALL(*mp_IDeviceProcessing, StartConfigurationService())
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, RVReqMoveToInitialPosition(_))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, IDForceDraining(_, _, _))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, RVReqMoveToRVPosition(_))
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

        EXPECT_CALL(*mp_IDeviceProcessing, PerTurnOnMainRelay())
                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

//        EXPECT_CALL(*mp_IDeviceProcessing, RTStartTemperatureControlWithPID(_, _, _, _, _, _, _))
//                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

//        EXPECT_CALL(*mp_IDeviceProcessing, RVStartTemperatureControlWithPID(_, _, _, _, _, _))
//                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

//        EXPECT_CALL(*mp_IDeviceProcessing, OvenStartTemperatureControlWithPID(_, _, _, _, _, _, _))
//                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

//        EXPECT_CALL(*mp_IDeviceProcessing, ALStartTemperatureControlWithPID(_, _, _, _, _, _, _))
//                .WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

    }

    sleep(1);
    mp_IDeviceProcessing->InitializationFinished();
    sleep(1);
    mp_IDeviceProcessing->ConfigurationFinished();
}

void TestRcReheating::init()
{
}

void TestRcReheating::cleanup()
{
}

void TestRcReheating::cleanupTestCase()
{
}

} // end namespace Scheduler

//QTEST_MAIN(Scheduler::TestRcReheating)

int main(int argc, char*argv[])
{
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    ::testing::InitGoogleMock(&argc, argv);
    QCoreApplication app(argc, argv);
    Scheduler::TestRcReheating tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "TestRcReheating.moc"

