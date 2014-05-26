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
#include "DiagnosticsManufacturing/Include/TestCaseReporter.h"
#include "DiagnosticsManufacturing/Include/OvenManufacturing.h"
#include "DiagnosticsManufacturing/Include/HeatingTestDialog.h"
//#include "Core/Include/ManufacturingDiagnosticsHandler.h"

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
     * \brief Test Oven manufacturing
     */
    /****************************************************************************/
    void utTestOvenManufacturing();

}; // end class CTestDiagnosticsManufacturing

/****************************************************************************/
void CTestDiagnosticsManufacturing::initTestCase() {
    Global::SystemPaths::Instance().SetSettingsPath("../../../Main/Build/Settings");
    Global::SystemPaths::Instance().SetTempPath("../Temporary");
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

    CTestCaseReporter Reporter("Oven", "123456");
    Reporter.GenReportFile();
}

void CTestDiagnosticsManufacturing::utTestOvenManufacturing()
{
    MainMenu::CMainWindow MainWindow;
    Core::CServiceGUIConnector *p_ServiceConnector = new Core::CServiceGUIConnector(&MainWindow);

    COven* p_OvenManf = new COven(p_ServiceConnector, &MainWindow);

    p_OvenManf->SetTestResult(Service::OVEN_COVER_SENSOR, true);

    CHeatingTestDialog* p_HeatingDlg = new DiagnosticsManufacturing::CHeatingTestDialog(Service::OVEN_COVER_SENSOR, &MainWindow);
    p_HeatingDlg->BlgProcessProgress(true);

}

} // end namespace DiagnosticsManufacturing

QTEST_MAIN(DiagnosticsManufacturing::CTestDiagnosticsManufacturing)
#include "TestDiagnosticsManufacturing.moc"

