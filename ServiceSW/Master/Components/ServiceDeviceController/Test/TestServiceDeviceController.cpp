    /****************************************************************************/
/*! \file TestServiceDeviceController.cpp
 *
 *  \brief Implementation file for class TestServiceDeviceController.
 *
 *  \b Description:
 *         Checks the CTestServiceDeviceController class implementation
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2014-01-22
 *  $Author:    $ Srivathsa HH
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
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

#include <QFile>
#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"
#include "ServiceDataManager/Include/TestCaseGuide.h"
#include "Global/Include/SystemPaths.h"
#include "ServiceDeviceController/Include/DeviceProcessor/DeviceProcessor.h"
#include "ServiceDeviceController/Include/DeviceProcessor/ManufacturingTestHandler.h"
#include "DeviceControl/Include/Interface/IDeviceProcessing.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"

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

namespace DeviceControl {

class MockIDeviceProcessing : public IDeviceProcessing
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
    MOCK_METHOD2(GetFunctionModuleRef, CFunctionModule*(quint32 InstanceID, const QString &Key));
};

/****************************************************************************/
/**
 * \brief Test class for CTestServiceDeviceController class.
 */
/****************************************************************************/
class CTestServiceDeviceController : public QObject {
    Q_OBJECT
public:
    CTestServiceDeviceController() {
        mp_IDevProc = new MockIDeviceProcessing;
        mp_DeviceControl= new DeviceProcessor(*mp_IDevProc);
        mp_ManufacturingHandler = new ManufacturingTestHandler(*mp_IDevProc);
    }

    ~CTestServiceDeviceController(){
        delete mp_IDevProc;
        delete mp_DeviceControl;
        delete mp_ManufacturingHandler;
    }

private:
    IDeviceProcessing* mp_IDevProc;
    DeviceProcessor* mp_DeviceControl;
    ManufacturingTestHandler* mp_ManufacturingHandler;

private slots:
    void initTestCase();
    void caseTestDeviceProcessor();
    void caseTestManufacturingTestHandler();
};

void CTestServiceDeviceController::initTestCase()
{
    Global::SystemPaths::Instance().SetSettingsPath("../../../Main/Build/Settings");
    QString ConfigFile = Global::SystemPaths::Instance().GetSettingsPath() + "/TestCaseConfig.xml";
    QString GuideFile  = Global::SystemPaths::Instance().GetSettingsPath() + "/TestCaseGuide_en.xml";
    qDebug()<<"config file :"<<ConfigFile;
    qDebug()<<"guide file :"<<GuideFile;
    ASSERT_TRUE(DataManager::CTestCaseFactory::Instance().InitData(ConfigFile));
    ASSERT_TRUE(DataManager::CTestCaseGuide::Instance().InitData(GuideFile));
}

void CTestServiceDeviceController::caseTestDeviceProcessor()
{
    mp_DeviceControl->OnCalibrateDevice(Service::DEVICE_PRESSURE_SENSOR);
}

void CTestServiceDeviceController::caseTestManufacturingTestHandler()
{
    mp_ManufacturingHandler->Initialize();
    ASSERT_TRUE(mp_ManufacturingHandler->IsInitialized());
    //mp_ManufacturingHandler->PerformModuleManufacturingTest(Service::MAINCONTROL_ASB3);
    //mp_ManufacturingHandler->TestRVInitialization();
    //QCOREAPPLICATION_EXEC(500);

    //EXPECT_CALL(*mp_IDevProc, StartConfigurationService())
            //.Times(AtLeast(1))
            //.WillRepeatedly(Return(DCL_ERR_FCT_CALL_SUCCESS));

    //ASSERT_NE(mp_ManufacturingHandler->TestMainControlASB(Service::MAINCONTROL_ASB3), 0);
    //ASSERT_NE(mp_ManufacturingHandler->TestRVInitialization(), -1);
    //mp_ManufacturingHandler->TestRVInitialization();
}
}

int main(int argc, char*argv[])
{
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    ::testing::InitGoogleMock(&argc, argv);

    QCoreApplication app(argc, argv);
    CTestServiceDeviceController ts;
    return QTest::qExec(&ts, argc, argv);
    //return RUN_ALL_TESTS();
}

#include "TestServiceDeviceController.moc"
