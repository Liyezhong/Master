/****************************************************************************/
/*! \file TestDiagnostics.cpp
 *
 *  \brief Unit test for Diagnostics menu
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
#include <MainMenu/Include/MainWindow.h>
//#include <Diagnostics/Include/Agitator.h>
//#include <Diagnostics/Include/AirExhaust.h>
//#include <Diagnostics/Include/Consumables.h>
//#include <Diagnostics/Include/CoverHood.h>
//#include <Diagnostics/Include/Display.h>
//#include <Diagnostics/Include/DrawerLoader.h>
//#include <Diagnostics/Include/DrawerUnloader.h>
//#include <Diagnostics/Include/HeatedCuvettes.h>
//#include <Diagnostics/Include/Oven.h>
//#include <Diagnostics/Include/SoftSwitch.h>
//#include <Diagnostics/Include/WaterStations.h>
//#include <Diagnostics/Include/XMovement.h>
#include <QObject>

namespace Diagnostics {

/****************************************************************************/
/**
 * \brief Test class for Diagnostics class.
 */
/****************************************************************************/
class CTestDiagnostics : public QObject {
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
    void utTestDiagnostics();

}; // end class CTestDiagnostics

/****************************************************************************/
void CTestDiagnostics::initTestCase() {
}

/****************************************************************************/
void CTestDiagnostics::init() {
}

/****************************************************************************/
void CTestDiagnostics::cleanup() {
}

/****************************************************************************/
void CTestDiagnostics::cleanupTestCase() {
}

/****************************************************************************/
void CTestDiagnostics::utTestDiagnostics() {
    MainMenu::CMainWindow *p_MainWindow = new MainMenu::CMainWindow();

//    Diagnostics::CAgitator *p_Agitator = new Diagnostics::CAgitator(p_MainWindow);

//    Diagnostics::CAirExhaust *p_AirExhaust = new Diagnostics::CAirExhaust(p_MainWindow);

//    Diagnostics::CConsumables *p_Consumables = new Diagnostics::CConsumables(p_MainWindow);

//    Diagnostics::CCoverHood *p_Coverhood = new Diagnostics::CCoverHood(p_MainWindow);

//    Diagnostics::CDisplay *p_Display = new Diagnostics::CDisplay(p_MainWindow);

//    Diagnostics::CDrawerLoader *p_DrawerLoader = new Diagnostics::CDrawerLoader(p_MainWindow);

//    Diagnostics::CDrawerUnloader *p_DrawerUnloader = new Diagnostics::CDrawerUnloader(p_MainWindow);

//    Diagnostics::CHeatedCuvettes *p_HeatedCuvettes = new Diagnostics::CHeatedCuvettes(p_MainWindow);

//    Diagnostics::COven *p_Oven = new Diagnostics::COven(p_MainWindow);

//    Diagnostics::CSoftSwitch *p_Softswitch = new Diagnostics::CSoftSwitch(p_MainWindow);

//    Diagnostics::CWaterStation *p_WaterStation = new Diagnostics::CWaterStation(p_MainWindow);

//    Diagnostics::CXMovement *p_XAxis = new Diagnostics::CXMovement(p_MainWindow);

//    p_Agitator->ReferenceRun();
//    p_Agitator->TestAgitatorSpeed();

//    p_AirExhaust->AirFlowStatusDialog(QString("Air flow status dialog"), false);

//    p_Consumables->DisplayCANID();
//    p_Consumables->DisplayFirmwareInfo();
//    p_Consumables->KitInformation();
//    p_Consumables->BottleInformation();

//    p_Coverhood->DisplayStatus();
//    p_Coverhood->HoodStatusDialog("Cover Hood Status", true);

//    p_Display->OnBasicColorTest();

//    p_DrawerLoader->AddItem("1", "Drawer loader reference run");
//    p_DrawerLoader->TestRun();

//    p_DrawerUnloader->AddItem("1", "Teach drawer");
//    p_DrawerUnloader->TestRun();

//    p_HeatedCuvettes->AddItem("2", "Maximum temperature test");
//    p_HeatedCuvettes->TestRun();

//    p_Oven->AddItem("2", "Photo Sensor test");
//    p_Oven->TestRun();

//    p_Softswitch->SoftSwitchTest();
//    p_Softswitch->LEDTest();

//    p_WaterStation->AddItem("1", "Water valves test");
//    p_WaterStation->WaterValvesTest();
//    p_WaterStation->DrainSensorTest();

//    p_XAxis->MoveYZArmLeft();
//    p_XAxis->MoveYZArmRight();
//    p_XAxis->XLeftReferenceRun();
//    p_XAxis->XRightReferenceRun();

}

} // end namespace Diagnostics

QTEST_MAIN(Diagnostics::CTestDiagnostics)
#include "TestDiagnostics.moc"

