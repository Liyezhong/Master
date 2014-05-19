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
#include <DiagnosticsManufacturing/Include/AgitatorManufacturing.h>
#include <DiagnosticsManufacturing/Include/AirExhaustManufacturing.h>
#include <DiagnosticsManufacturing/Include/DrawerLoaderManufacturing.h>
#include <DiagnosticsManufacturing/Include/DrawerUnLoaderManufacturing.h>
#include <DiagnosticsManufacturing/Include/HeatedCuvettesManufacturing.h>
#include <DiagnosticsManufacturing/Include/OvenManufacturing.h>
#include <DiagnosticsManufacturing/Include/XMovementManufacturing.h>
#include <DiagnosticsManufacturing/Include/YZMovementLeftManufacturing.h>
#include <DiagnosticsManufacturing/Include/YZMovementRightManufacturing.h>
#include <DiagnosticsManufacturing/Include/TransferStationManufacturing.h>
#include <QObject>

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
    void utTestDiagnosticsManufacturing();

}; // end class CTestDiagnosticsManufacturing

/****************************************************************************/
void CTestDiagnosticsManufacturing::initTestCase() {
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
void CTestDiagnosticsManufacturing::utTestDiagnosticsManufacturing() {

    MainMenu::CMainWindow *p_MainWindow = new MainMenu::CMainWindow();
    Core::CServiceGUIConnector *mp_DataConnector = new Core::CServiceGUIConnector(p_MainWindow);

    DiagnosticsManufacturing::CAgitator *p_Agitator = new
            DiagnosticsManufacturing::CAgitator(mp_DataConnector, p_MainWindow);

    DiagnosticsManufacturing::CAirExhaust *p_AirExhaust = new
            DiagnosticsManufacturing::CAirExhaust(mp_DataConnector, p_MainWindow);

    DiagnosticsManufacturing::CDrawerLoader *p_DrawerLoader = new
            DiagnosticsManufacturing::CDrawerLoader(mp_DataConnector, p_MainWindow);

    DiagnosticsManufacturing::CDrawerUnLoader *p_DrawerUnloader = new
            DiagnosticsManufacturing::CDrawerUnLoader(mp_DataConnector, p_MainWindow);

    DiagnosticsManufacturing::CHeatedCuvettes *p_HeatedCuvettes = new
            DiagnosticsManufacturing::CHeatedCuvettes(mp_DataConnector, p_MainWindow);

    DiagnosticsManufacturing::COven *p_Oven = new
            DiagnosticsManufacturing::COven(mp_DataConnector, p_MainWindow);

    DiagnosticsManufacturing::CXMovement *p_XAxis = new
            DiagnosticsManufacturing::CXMovement(mp_DataConnector, p_MainWindow);

    DiagnosticsManufacturing::CYZMovementLeft *p_XArmLeft = new
            DiagnosticsManufacturing::CYZMovementLeft(mp_DataConnector, p_MainWindow);

    DiagnosticsManufacturing::CYZMovementRight *p_XArmRight = new
            DiagnosticsManufacturing::CYZMovementRight(mp_DataConnector, p_MainWindow);

    DiagnosticsManufacturing::CTransferStation *p_TransferStation = new
            DiagnosticsManufacturing::CTransferStation(mp_DataConnector, p_MainWindow);

    p_Agitator->AddItem("2", "Velocity and Light Barrier Check");
    p_Agitator->BeginTest();
    p_Agitator->SetTestResult(Service::AGITATOR_REFERENCE_RUN, true);
    p_Agitator->SetTestResult(Service::AGITATOR_VELOCITY_LIGHT_BARRIER_CHECK, false);
    p_Agitator->SendTestReport();

    p_AirExhaust->AddItem("1", "Air Exhaust fan test");
    p_AirExhaust->BeginTest();
    p_AirExhaust->SetTestResult(Service::AIREXHAUST_AIRFLOW_STATUS_CHECK, true);
    p_AirExhaust->SetTestResult(Service::AIREXHAUST_FAN_CURRENT_CHECK, false);
    p_AirExhaust->SetTestResult(Service::AIREXHAUST_FAN_TEST, true);
    p_AirExhaust->SendTestReport();    

    p_DrawerLoader->AddItem("3", "Drawer Loader Reference Run");
    p_DrawerLoader->BeginTest();
    p_DrawerLoader->SetTestResult(Service::DRAWERLOADER_REFERENCE_RUN, true);
    p_DrawerLoader->SetTestResult(Service::DRAWERLOADER_OPENCLOSE_DRAWER_TEST, false);
    p_DrawerLoader->SetTestResult(Service::DRAWERLOADER_LED_TEST, true);
    p_DrawerLoader->SetTestResult(Service::DRAWERLOADER_RFID_TEST, false);
    p_DrawerLoader->SetTestResult(Service::DRAWERLOADER_FIRMWARE_UPDATE, true);
    p_DrawerLoader->SetTestResult(Service::DRAWERLOADER_CANID, false);
    p_DrawerLoader->SendTestReport();

    p_DrawerUnloader->AddItem("1", "Open Close Drawer Test");
    p_DrawerUnloader->BeginTest();
    p_DrawerUnloader->SetTestResult(Service::DRAWERUNLOADER_REFERENCE_RUN, true);
    p_DrawerUnloader->SetTestResult(Service::DRAWERUNLOADER_OPENCLOSE_DRAWER_TEST, false);
    p_DrawerUnloader->SetTestResult(Service::DRAWERUNLOADER_LED_TEST, true);
    p_DrawerUnloader->SetTestResult(Service::DRAWERUNLOADER_RFID_TEST, false);
    p_DrawerUnloader->SetTestResult(Service::DRAWERUNLOADER_FIRMWARE_UPDATE, true);
    p_DrawerUnloader->SetTestResult(Service::DRAWERUNLOADER_CANID, false);
    p_DrawerUnloader->SendTestReport();

    p_HeatedCuvettes->AddItem("4", "Temperature sensor tests");
    p_HeatedCuvettes->BeginTest();
    p_HeatedCuvettes->SetTestResult(Service::HEATEDCUVETTE_TEST, true);
    p_HeatedCuvettes->SetTestResult(Service::HEATEDCUVETTE_FIRMWARE_UPDATE, false);
    p_HeatedCuvettes->SetTestResult(Service::HEATEDCUVETTE_CANID, true);
    p_HeatedCuvettes->SendTestReport();

    p_Oven->AddItem("3", "Oven Lid Calibration");
    p_Oven->BeginTest();
    p_Oven->SetTestResult(Service::OVEN_TEST, true);
    p_Oven->SetTestResult(Service::OVEN_PHOTOSENSOR_TEST, false);
    p_Oven->SetTestResult(Service::OVEN_SLIDECOUNT_TEST, true);
    p_Oven->SetTestResult(Service::OVEN_FIRMWARE_UPDATE, false);
    p_Oven->SetTestResult(Service::OVEN_CANID, true);
    p_Oven->SendTestReport();

    p_XAxis->AddItem("1", "X1 Reference Run");
    p_XAxis->BeginTest();
    p_XAxis->SetTestResult(Service::X1_REFERENCE_RUN, true);
    p_XAxis->SetTestResult(Service::X2_REFERENCE_RUN, false);
    p_XAxis->SendTestReport();

    p_XArmLeft->AddItem("2", "Level Sensor Test");
    p_XArmLeft->BeginTest();
    p_XArmLeft->SetTestResult(Service::XARMLEFT_REFERENCE_RUN, true);
    p_XArmLeft->SetTestResult(Service::XARMLEFT_LEVELSENSOR_TEST, false);
    p_XArmLeft->SetTestResult(Service::XARMLEFT_RFID_TEST, true);
    p_XArmLeft->SetTestResult(Service::XARMLEFT_FIRMWARE_UPDATE, false);
    p_XArmLeft->SetTestResult(Service::XARMLEFT_CANID, true);
    p_XArmLeft->SendTestReport();

    p_XArmRight->AddItem("1", "RFID Test");
    p_XArmRight->BeginTest();
    p_XArmRight->SetTestResult(Service::XARMRIGHT_REFERENCE_RUN, true);
    p_XArmRight->SetTestResult(Service::XARMRIGHT_LEVELSENSOR_TEST, false);
    p_XArmRight->SetTestResult(Service::XARMRIGHT_RFID_TEST, true);
    p_XArmRight->SetTestResult(Service::XARMRIGHT_FIRMWARE_UPDATE, false);
    p_XArmRight->SetTestResult(Service::XARMRIGHT_CANID, true);
    p_XArmRight->SendTestReport();

    p_TransferStation->AddItem("1", "Reference Run");
    p_TransferStation->BeginTest();
    p_TransferStation->SetTestResult(Service::TRANSFERSTATION_REFERENCE_RUN, true);
    p_TransferStation->SetTestResult(Service::TRANSFERSTATION_FIRMWARE_UPDATE, false);
    p_TransferStation->SetTestResult(Service::TRANSFERSTATION_CANID, true);
    p_TransferStation->SendTestReport();

}

} // end namespace DiagnosticsManufacturing

QTEST_MAIN(DiagnosticsManufacturing::CTestDiagnosticsManufacturing)
#include "TestDiagnosticsManufacturing.moc"

