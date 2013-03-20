/****************************************************************************/
/*! \file TestPrograms.cpp
 *
 *  \brief Unit test for Programs menu
 *
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-12-18
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
#include <Programs/Include/ProgramWidget.h>
#include <Programs/Include/ModifyProgramDlg.h>
#include <Programs/Include/ColorButton.h>
#include <Programs/Include/ModifyLeicaHne.h>
#include <Programs/Include/ManualProgramDlg.h>
#include <QObject>

namespace Programs {

/****************************************************************************/
/**
 * \brief Test class for Programs class.
 */
/****************************************************************************/
class CTestPrograms : public QObject {
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
     * \brief Test ProgramWidget object
     */
    /****************************************************************************/
    void utTestPrograms();

}; // end class TestPrograms

/****************************************************************************/
void CTestPrograms::initTestCase() {
}

/****************************************************************************/
void CTestPrograms::init() {
}

/****************************************************************************/
void CTestPrograms::cleanup() {
}

/****************************************************************************/
void CTestPrograms::cleanupTestCase() {
}

/****************************************************************************/
void CTestPrograms::utTestPrograms()
{
    MainMenu::CMainWindow MainWindow;   
    KeyBoard::CKeyBoard KeyBoard;
    DataManager::CProgram Program;
    DataManager::CProgramStep FirstProgramStep;
    DataManager::CProgramStep SecondProgramStep;
    DataManager::CProgramStep LeicaProgramStepOne;
    DataManager::CProgramStep LeicaProgramStepTwo;

    Core::CDataConnector *p_DataConnector = new Core::CDataConnector(&MainWindow);

    Programs::CProgramWidget *p_ProgramWidget = new
            Programs::CProgramWidget(p_DataConnector, &MainWindow, &KeyBoard);

    Programs::CModifyProgramDlg *p_ModifyProgramDlg = new
            Programs::CModifyProgramDlg(p_ProgramWidget, &KeyBoard, &MainWindow, p_DataConnector);

    Programs::CModifyProgramStepDlg *p_ModifyProgramStepDlg = new
            Programs::CModifyProgramStepDlg(p_ModifyProgramDlg, &MainWindow);

    Programs::CModifyLeicaHne *p_ModifyLeicaHneDlg = new
            Programs::CModifyLeicaHne(p_ProgramWidget,  &MainWindow, p_DataConnector);

    Programs::CRackGripColorDlg *p_RackGripColorDlg = new
            Programs::CRackGripColorDlg(p_ProgramWidget, &MainWindow);

    Programs::CColorButton *p_ColorButton = new Programs::CColorButton(p_ProgramWidget);

    Programs::CManualProgramDlg *p_ManualProgramDlg = new CManualProgramDlg(p_ProgramWidget, &MainWindow);

    DataManager::CDataProgramList ProgramList = *(p_DataConnector->ProgramList);
    DataManager::CDataStationList StationList = *(p_DataConnector->StationList);
    DataManager::CDataReagentList ReagentList = *(p_DataConnector->ReagentList);
    DataManager::CProgramSequenceList ProgramSequenceList = *(p_DataConnector->ProgramSequenceList);

    //  changed method by setting the ProcessRunning to true in MainWindow
    MainWindow.SetStatusIcons(MainMenu::CMainWindow::ProcessRunning);

    // Set User role to Administrator in MainWindow
    MainWindow.SetUserRole(MainMenu::CMainWindow::Admin);
    const QModelIndex Index;

    //Set Program attributes
    Program.SetLongName("TestProgram");
    Program.SetShortName("TES");
    Program.SetID("L1");
    Program.SetColor("blue");

    //Test ProgramWidget methods
    p_ProgramWidget->PopulateProgramList();
    p_ProgramWidget->OnDeviceModeChanged("Himalaya");
    p_ProgramWidget->SelectionChanged(Index);
    p_ProgramWidget->OnUserRoleChanged();

    //Set all the ProgramStep parameters for ProgramSteps
    FirstProgramStep.SetStepID("0");
    FirstProgramStep.SetReagentID("S4");
    FirstProgramStep.SetExclusive(false);
    FirstProgramStep.SetMaxDurationInPercent("50%");
    FirstProgramStep.SetMinDuration("1m");

    SecondProgramStep.SetStepID("3");
    SecondProgramStep.SetReagentID("S7");
    SecondProgramStep.SetExclusive(false);
    SecondProgramStep.SetMaxDurationInPercent("100%");
    SecondProgramStep.SetMinDuration("0");

    LeicaProgramStepOne.SetStepID("1");
    LeicaProgramStepOne.SetReagentID("L1");
    LeicaProgramStepOne.SetExclusive(true);
    LeicaProgramStepOne.SetMaxDurationInPercent("50%");
    LeicaProgramStepOne.SetMinDuration("3m");
    LeicaProgramStepOne.SetIntensity(2);
    LeicaProgramStepTwo.SetNumberOfParallelStations(0);

    LeicaProgramStepTwo.SetStepID("2");
    LeicaProgramStepTwo.SetReagentID("L2");
    LeicaProgramStepTwo.SetExclusive(true);
    LeicaProgramStepTwo.SetMaxDurationInPercent("25%");
    LeicaProgramStepTwo.SetMinDuration("5m");
    LeicaProgramStepTwo.SetIntensity(4);
    LeicaProgramStepTwo.SetNumberOfParallelStations(0);

    // Adding ProgramSteps to a program
    Program.AddProgramStep(&FirstProgramStep);
    Program.AddProgramStep(&LeicaProgramStepOne);
    Program.AddProgramStep(&LeicaProgramStepTwo);
    Program.AddProgramStep(&SecondProgramStep);

    //Add Program to ProgramList
    ProgramList.AddProgram(&Program);

    //Test ModifyProgramDlg class member functions
    p_ModifyProgramDlg->InitDialog(&Program);
    p_ModifyProgramDlg->NewProgram();
    p_ModifyProgramDlg->SetRowFocus(1);
    p_ModifyProgramDlg->SetButtonType(EDIT_BTN_CLICKED);
    QCOMPARE(p_ModifyProgramDlg->m_ButtonType, EDIT_BTN_CLICKED);

    p_ModifyProgramDlg->SetCurrentDeviceMode("Himalaya");   
    p_ModifyProgramDlg->VerifyLastProgramStep(&Program, "Himalaya");
    p_ModifyProgramDlg->VerifyLastStepForHimalayaMode(&Program);
    p_ModifyProgramDlg->VerifyLastStepForWorkStationMode(&Program);

    //Test RackGripColorDlg class member functions
    p_RackGripColorDlg->Init(&ProgramList, &Program);
    p_RackGripColorDlg->SetSaveButton("Save");

     //Test ModifyProgramStepDlg class member functions
    p_ModifyProgramStepDlg->SetProgramStep(&FirstProgramStep, &ReagentList, &StationList);
    p_ModifyProgramStepDlg->NewProgramStep(&ReagentList, &StationList);
    p_ModifyProgramStepDlg->ResetButtons(false);
    p_ModifyProgramStepDlg->SelectRow(3);

    //Test ModifyLeicaHneDlg class member functions
    p_ModifyLeicaHneDlg->SetReadyProgram("Program ready to start");

   //Test ColorButton class member functions
    p_ColorButton->SetColor("blue");

    //Test ManualProgramDlg class member functions
    p_ManualProgramDlg->SetProgramList(&ProgramList, &ProgramSequenceList);
    p_ManualProgramDlg->OnBtnStartClicked();
    p_ManualProgramDlg->PopulateManualProgramList();
}

} // end namespace Programs

QTEST_MAIN(Programs::CTestPrograms)
#include "TestPrograms.moc"

