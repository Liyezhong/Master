/****************************************************************************/
/*! \file TestDiagnosticsManufacturing.cpp
 *
 *  \brief Unit test for Diagnostics Manufacturing menu
 *
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-08-06
 *  $Author:    $ Soumya. D
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

#include <QtTest/QTest>
#include <QDebug>
#include <Core/Include/ServiceGUIConnector.h>
#include <Core/Include/ServiceDefines.h>
#include <MainMenu/Include/MainWindow.h>
#include <QObject>
#include "Global/Include/SystemPaths.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"
#include "DataManager/Containers/DeviceConfiguration/Include/DeviceConfigurationInterface.h"
#include "DiagnosticsManufacturing/Include/TestCaseReporter.h"
#include "DiagnosticsManufacturing/Include/OvenManufacturing.h"
#include "DiagnosticsManufacturing/Include/LaSystemManufacturing.h"
#include "DiagnosticsManufacturing/Include/RotaryValveManufacturing.h"
#include "DiagnosticsManufacturing/Include/MainControlManufacturing.h"
#include "DiagnosticsManufacturing/Include/CleaningManufacturing.h"
#include "DiagnosticsManufacturing/Include/RetortManufacturing.h"
#include "DiagnosticsManufacturing/Include/SystemManufacturing.h"
#include "DiagnosticsManufacturing/Include/HeatingTestDialog.h"
#include "Core/Include/ManufacturingDiagnosticsHandler.h"

namespace DiagnosticsManufacturing {

/****************************************************************************/
/**
 * \brief Test class for Diagnostics Manufacturing class.
 */
/****************************************************************************/
class CTestDiagnosticsManufacturing : public QObject {
    Q_OBJECT
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
    /**
     * \brief Test Diagnostics Menu object
     */
    /****************************************************************************/
    void utTestTestCaseReporter();

    /****************************************************************************/
    /**
     * \brief Test  manufacturing
     */
    /****************************************************************************/
    void utTestManufacturing();

    /****************************************************************************/
    /**
     * \brief Test manufacturing diagnostics handler
     */
    /****************************************************************************/
    void utTestManufacturingHandler();

private:
    MainMenu::CMainWindow *mp_MainWindow;
    Core::CServiceGUIConnector *mp_ServiceConnector;
    ServiceDataManager::CModuleDataList *mp_ModuleList;
    ServiceDataManager::CInstrumentHistory* mp_ModuleListArchive;
    DataManager::CDeviceConfigurationInterface* mp_DeviceConfig;

}; // end class CTestDiagnosticsManufacturing

/****************************************************************************/
void CTestDiagnosticsManufacturing::initTestCase() {
    Global::SystemPaths::Instance().SetSettingsPath("../../../Main/Build/Settings");
    Global::SystemPaths::Instance().SetTempPath("../../../Main/Build/Temporary");

    mp_ModuleList = new ServiceDataManager::CModuleDataList;
    mp_ModuleListArchive = new ServiceDataManager::CInstrumentHistory;
    mp_DeviceConfig = new DataManager::CDeviceConfigurationInterface;

    QVERIFY(mp_ModuleList->ReadFile(Global::SystemPaths::Instance().GetSettingsPath()+"/InstrumentHistory.xml"));
    QVERIFY(mp_ModuleListArchive->ReadFile(Global::SystemPaths::Instance().GetSettingsPath()+"/InstrumentHistoryArchive.xml"));
    QVERIFY(mp_DeviceConfig->Read(Global::SystemPaths::Instance().GetSettingsPath()+"/DeviceConfiguration.xml"));

    mp_MainWindow = new MainMenu::CMainWindow;
    mp_ServiceConnector = new Core::CServiceGUIConnector(mp_MainWindow);
    mp_ServiceConnector->SetModuleListContainer(mp_ModuleList, mp_ModuleListArchive);
    mp_ServiceConnector->SetDeviceConfigurationInterface(mp_DeviceConfig);
}

/****************************************************************************/
void CTestDiagnosticsManufacturing::init() {
}

/****************************************************************************/
void CTestDiagnosticsManufacturing::cleanup() {
}

/****************************************************************************/
void CTestDiagnosticsManufacturing::cleanupTestCase() {
}

/****************************************************************************/
void CTestDiagnosticsManufacturing::utTestTestCaseReporter()
{
    QString FileName = Global::SystemPaths::Instance().GetSettingsPath() + "/TestCaseConfig.xml";
    DataManager::CTestCaseFactory::Instance().InitData(FileName);

    DataManager::CTestCase* TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase("OvenHeatingEmpty");
    TestCase->SetStatus(false);
    TestCase->AddResult("Heating Time", "00:30");
    TestCase->AddResult("CurrentTemp", "40");

    //CTestCaseReporter Reporter("Oven", "123456");
    CTestCaseReporter Reporter("Oven");
    Reporter.AddTestCaseId(Service::OVEN_COVER_SENSOR);
    Reporter.AddTestCaseId(Service::OVEN_HEATING_EMPTY);
    Reporter.AddTestCaseId(Service::OVEN_HEATING_WITH_WATER);

    Reporter.SetSerialNumber("12121212");
    QVERIFY(Reporter.GenReportFile());
    //Reporter.GenReportFile();
}

void CTestDiagnosticsManufacturing::utTestManufacturing()
{
    COven* p_OvenManf = new COven(mp_ServiceConnector, mp_MainWindow);
    CLaSystem * p_LaSystemManf = new CLaSystem(mp_ServiceConnector, mp_MainWindow);
    CMainControl* p_MainControlManf = new CMainControl(mp_ServiceConnector, mp_MainWindow);
    CRotaryValve* p_RvManf = new CRotaryValve(mp_ServiceConnector, mp_MainWindow);
    CSystem* p_SystemManf = new CSystem(mp_ServiceConnector, mp_MainWindow);
    CRetort* p_RetortManf = new CRetort(mp_ServiceConnector, mp_MainWindow);
    CCleaning* p_CleanManf = new CCleaning(mp_ServiceConnector, mp_MainWindow);


    p_OvenManf->SetTestResult(Service::OVEN_COVER_SENSOR, true);
    p_OvenManf->ResetTestStatus();

    p_LaSystemManf->SetTestResult(Service::LA_SYSTEM_HEATING_LIQUID_TUBE, false);
    p_LaSystemManf->ResetTestStatus();

    p_MainControlManf->SetTestResult(Service::MAINCONTROL_ASB3, true);
    p_MainControlManf->ResetTestStatus();

    p_RvManf->SetTestResult(Service::ROTARY_VALVE_HEATING_END, true);
    p_RvManf->ResetTestStatus();

    p_SystemManf->SetTestResult(Service::SYSTEM_OVERFLOW, true);
    p_SystemManf->SetTestResult(Service::SYSTEM_SEALING_TEST, false);
    p_SystemManf->SetTestResult(Service::SYSTEM_SPEARKER, true);
    p_SystemManf->EnableButton(true);

    p_RetortManf->SetTestResult(Service::RETORT_LID_LOCK, true);
    p_RetortManf->SetTestResult(Service::RETORT_HEATING_EMPTY, false);

    p_CleanManf->SetTestResult(Service::CLEANING_SYSTEM_TEST, true);
    p_CleanManf->EnableButton(false);

    CHeatingTestDialog* p_HeatingDlg = new DiagnosticsManufacturing::CHeatingTestDialog(Service::OVEN_COVER_SENSOR, mp_MainWindow);
    p_HeatingDlg->BlgProcessProgress(true);

}

void CTestDiagnosticsManufacturing::utTestManufacturingHandler()
{
    Core::CManufacturingDiagnosticsHandler* p_MDH = new Core::CManufacturingDiagnosticsHandler(mp_ServiceConnector, mp_MainWindow);
}

} // end namespace DiagnosticsManufacturing

QTEST_MAIN(DiagnosticsManufacturing::CTestDiagnosticsManufacturing)
#include "TestDiagnosticsManufacturing.moc"

