
/****************************************************************************/
/*! \file CTestReagents.cpp
 *
 *  \brief Unit test for Reagents menu
 *
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-02-02
 *  $Author:    $ C.Adaragunchi
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

#include <QtTest>
#include <QDebug>
#include <Reagents/Include/ReagentsWidget.h>
#include <Reagents/Include/ModifyReagentDlg.h>
#include <Programs/Include/ModifyProgramStepDlg.h>
#include <QObject>


namespace Reagents {

/****************************************************************************/
/**
 * \brief Test class for Reagents class.
 */
/****************************************************************************/
class CTestReagents : public QObject {
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
     * \brief Test ReagentWidget object
     */
    /****************************************************************************/
    void utTestReagentWidget();

}; // end class TestReagents

/****************************************************************************/
void CTestReagents::initTestCase() {
}

/****************************************************************************/
void CTestReagents::init() {
}

/****************************************************************************/
void CTestReagents::cleanup() {
}

/****************************************************************************/
void CTestReagents::cleanupTestCase() {
}

/****************************************************************************/
void CTestReagents::utTestReagentWidget() {

    MainMenu::CMainWindow MainWindow;
    Core::CDataConnector *p_DataConnector = new Core::CDataConnector(&MainWindow);
    KeyBoard::CKeyBoard KeyBoard;

    Reagents::CReagentsWidget *p_ReagentWidget = new
            Reagents::CReagentsWidget(p_DataConnector, p_DataConnector->ReagentList,
                                      &MainWindow, &KeyBoard);
    Reagents::CModifyReagentDlg *p_ModifyReagentDlg = new
           Reagents::CModifyReagentDlg(p_ReagentWidget, &KeyBoard, &MainWindow, p_DataConnector);
    Reagents::CHeatedCuvette *p_HeatedCuvette = new
           Reagents::CHeatedCuvette(p_ReagentWidget, &MainWindow, p_DataConnector);
    DataManager::CReagent Reagent;

    //Set Reagent attributes
    Reagent.SetID("U1");
    Reagent.Set5RackMode(true);
    Reagent.SetClass("3");
    Reagent.SetLongName("Test Reagent");
    Reagent.SetShortName("Test");
    Reagent.SetVisibleState(false);
    Reagent.SetLockState(false);
    Reagent.SetMaxSlides(2000);
    Reagent.SetMaxTime("2d");
    Reagent.SetHeatingStartMode(AT_DEVICE_START);
    Reagent.SetReagentTemp(0);

    //changed method by setting the ProcessRunning to true in MainWindow
    MainWindow.SetStatusIcons(MainMenu::CMainWindow::ProcessRunning);

    // Set User role to Administrator in MainWindow
    MainWindow.SetUserRole(MainMenu::CMainWindow::Admin);
    p_ReagentWidget->OnProcessStateChanged();
    p_ReagentWidget->OnUserRoleChanged();
    p_ReagentWidget->PopulateReagentList();
    p_ReagentWidget->ResizeHorizontalSection();

    // Test ModifyReagentDlg class member functions
    p_ModifyReagentDlg->SetButtonType(EDIT_BTN_CLICKED);
    p_ModifyReagentDlg->SetDialogTitle("Edit Reagent");
    p_ModifyReagentDlg->SetDialogTitle("New Reagent");
    p_ModifyReagentDlg->Init();
    p_ModifyReagentDlg->InitDialog(&Reagent);

    // Test HeatedCuvette class member functions
    p_HeatedCuvette->InitDialog(&Reagent);
    p_HeatedCuvette->OnTemperatureFormatChanged();
    p_HeatedCuvette->OnProcessStateChanged();
 }

} // end namespace Programs

QTEST_MAIN(Reagents::CTestReagents)
#include "TestReagents.moc"

