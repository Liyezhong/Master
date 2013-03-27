/****************************************************************************/
/*! \file ModifyLeicaHne.cpp
 *
 *  \brief Implementation of file for class CModifyLeicaHne.
 *
 *  \b Description:
 *          This class implements a widget for displaying Leica program details
 *          which provides user interface to assign color to the program and
 *          intensity of the Leica reagents.
 *
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-16
 *   $Author:  $ M.Scherer, C.Adaragunchi1234, Swati Tiwari
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

#include "Global/Include/Utils.h"
#include "Global/Include/Exception.h"
#include "Programs/Include/ModifyLeicaHne.h"
#include "ui_ModifyLeicaHne.h"

namespace Programs {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 *  \iparam p_MainWindow = Reference to MainWindow
 *  \iparam p_DataConnector = Reference to DataConnector
 */
/****************************************************************************/
CModifyLeicaHne::CModifyLeicaHne(QWidget *p_Parent,
                                 MainMenu::CMainWindow *p_MainWindow,Core::CDataConnector *p_DataConnector) :
                                 MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CModifyLeicaHne),
                                 mp_MainWindow(p_MainWindow), mp_DataConnector(p_DataConnector), m_ProcessRunning(false),
                                 m_ColorReplaced(false)
{
    mp_Ui->setupUi(GetContentFrame());
    SetDialogTitle(tr("Edit Leica Program"));
    mp_ScrollWheelHaemotoxylin = NULL;
    mp_ScrollWheelEosin = NULL;
    mp_DlgRackGripColor = NULL;
    mp_ProgramList = NULL;

    mp_DlgRackGripColor = new CRackGripColorDlg(this,mp_MainWindow);
    mp_DlgRackGripColor->setModal(true);

    CONNECTSIGNALSLOT(mp_Ui->btnCancel, clicked(), this, reject());
//    CONNECTSIGNALSLOT(mp_Ui->btnColor, clicked(), this, OnColor());
    CONNECTSIGNALSLOT(mp_Ui->btnSave, clicked(), this, OnSave());

    CONNECTSIGNALSLOT(mp_DlgRackGripColor, UpdateProgramColor(DataManager::CProgram &,bool),
                      this,OnUpdateProgramColor(DataManager::CProgram &,bool));

    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
    mp_Ui->btnAbbreviation->setEnabled(false);
    mp_Ui->btnProgramName->setEnabled(false);
//    mp_Ui->btnColor->setEnabled(false);
    mp_Ui->btnSave->setEnabled(false);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CModifyLeicaHne::~CModifyLeicaHne()
{
    try {
        delete mp_DlgRackGripColor;
        delete mp_ScrollWheelHaemotoxylin;
        delete mp_ScrollWheelEosin;
        delete mp_Ui;
    }
    catch (...) {
        // to please Lint.
    }
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CModifyLeicaHne::changeEvent(QEvent *p_Event)
{
    MainMenu::CDialogFrame::changeEvent(p_Event);
    switch (p_Event->type()) {
        case QEvent::LanguageChange:
            mp_Ui->retranslateUi(this);
            RetranslateUI();
            break;
        default:
            break;
    }
}

/****************************************************************************/
/*!
 *  \brief Sets the content of the dialog
 *
 *  \iparam p_Program = Program displayed in the dialog
 *  \iparam FirstProgramStep = FirstProgramStep with Leica reagent
 *  \iparam SecondProgramStep = SecondProgramStep with Leica reagent
 */
/****************************************************************************/
void CModifyLeicaHne::InitDailog(DataManager::CProgram const *p_Program,
                                 DataManager::CProgramStep FirstProgramStep,
                                 DataManager::CProgramStep SecondProgramStep)
{
    m_Program = *p_Program;
    m_ProgramListClone = *(mp_DataConnector->ProgramList);
    m_LeicaProgramStepOne = FirstProgramStep;
    m_LeicaProgramStepTwo = SecondProgramStep;
    const DataManager::CReagent *p_Reagent = NULL;
    const DataManager::CReagent *p_ReagentSecond = NULL;

    p_Reagent = mp_DataConnector->ReagentList->GetReagent(FirstProgramStep.GetReagentID());
    p_ReagentSecond = mp_DataConnector->ReagentList->GetReagent(SecondProgramStep.GetReagentID());

//    mp_Ui->groupBox->setTitle(p_Reagent->GetShortName());
//    mp_Ui->groupBox_2->setTitle(p_ReagentSecond->GetShortName());
//    mp_Ui->btnProgramName->setText(tr("%1").arg(m_Program.GetLongName()));
//    mp_Ui->btnAbbreviation->setText(tr("%1").arg(m_Program.GetShortName()));

//    mp_Ui->btnColor->SetColor(m_Program.GetColor());
    CONNECTSIGNALSLOT(mp_MainWindow, ProcessStateChanged(), this, OnProcessStateChanged());

    mp_ScrollWheelEosin = new MainMenu::CScrollWheel();
    mp_ScrollWheelHaemotoxylin = new MainMenu::CScrollWheel();

    for (qint32 I = 1; I <= FirstProgramStep.GetNumberOfSteps(); I++) {
        mp_ScrollWheelHaemotoxylin->AddItem(QString::number(I), I);
    }
    mp_ScrollWheelHaemotoxylin->SetNonContinuous();

    for (qint32 I = 1; I <= SecondProgramStep.GetNumberOfSteps(); I++) {
        mp_ScrollWheelEosin->AddItem(QString::number(I), I);
    }
    mp_ScrollWheelEosin->SetNonContinuous();
    mp_Ui->eosinScrollPanel->Init(1);
    mp_Ui->haemotoxylinScrollPanel->Init(1);
    mp_Ui->eosinScrollPanel->AddScrollWheel(mp_ScrollWheelEosin, 0);
    mp_Ui->haemotoxylinScrollPanel->AddScrollWheel(mp_ScrollWheelHaemotoxylin, 0);
}

/****************************************************************************/
/*!
 *  \brief Displays whether the program is ready to start or not.
 *  \iparam ReadyProgramString = Text for ready program label
 */
/****************************************************************************/
void CModifyLeicaHne::SetReadyProgram (QString ReadyProgramString)
{
    mp_Ui->label_3->setText(ReadyProgramString);
}
/****************************************************************************/
/*!
 *  \brief This slot is called when Process Running state changes
 */
/****************************************************************************/
void CModifyLeicaHne::OnProcessStateChanged()
{
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
         m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
         (!m_ProcessRunning)) {
        //Edit Mode
        mp_Ui->btnCancel->setText(tr("Cancel"));
        mp_Ui->btnSave->setEnabled(true);
        mp_Ui->eosinScrollPanel->SetDisabled(false);
        mp_Ui->haemotoxylinScrollPanel->SetDisabled(false);
    }
    else {
        mp_Ui->btnCancel->setText(tr("Close"));
        mp_Ui->btnSave->setEnabled(false);
        mp_Ui->eosinScrollPanel->SetDisabled(true);
        mp_Ui->haemotoxylinScrollPanel->SetDisabled(true);
    }
}

/****************************************************************************/
/*!
 *  \brief This event is called whenever widget is shown.
 *
 *  \iparam p_Event = Show event
 */
/****************************************************************************/
void CModifyLeicaHne::showEvent(QShowEvent *p_Event)
{
    Q_UNUSED(p_Event);
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
    if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
         m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
            (!m_ProcessRunning)) {
        //Edit Mode
        mp_Ui->btnCancel->setText(tr("Cancel"));
//        mp_Ui->btnColor->setEnabled(true);
        mp_Ui->btnSave->setEnabled(true);
        mp_Ui->eosinScrollPanel->SetDisabled(false);
        mp_Ui->haemotoxylinScrollPanel->SetDisabled(false);
    }
    else {
        mp_Ui->btnCancel->setText(tr("Close"));
//        mp_Ui->btnColor->SetColor("black");
//        mp_Ui->btnColor->setEnabled(false);
        mp_Ui->btnSave->setEnabled(false);
        mp_Ui->eosinScrollPanel->SetDisabled(true);
        mp_Ui->haemotoxylinScrollPanel->SetDisabled(true);
    }
}

/********************************************************************************/
/*!
 *  \brief Sets the intensity of Leica Reagents.
 *  \iparam Reagent1Intensity = Intensity of First Leica reagent.
 *  \iparam Reagent2Intensity = Intensity of Second Leica reagent.
 */
/********************************************************************************/
void CModifyLeicaHne::SetIntensity(int Reagent1Intensity, int Reagent2Intensity)
{
    if (mp_ScrollWheelHaemotoxylin) {
        mp_ScrollWheelHaemotoxylin->SetCurrentData(Reagent1Intensity);
    }
    if (mp_ScrollWheelEosin) {
        mp_ScrollWheelEosin->SetCurrentData(Reagent2Intensity);
    }
}

/********************************************************************************/
/*!
 *  \brief This slot is called when UpdateProgramColor signal in RackColorGrip
 *         is emitted.
 *
 *  \iparam Program = Selected/New Program
 *  \iparam ColorReplaced = True(Color of existing program replaced)/ False
 */
/********************************************************************************/
void CModifyLeicaHne::OnUpdateProgramColor(DataManager::CProgram &Program,
                                           bool ColorReplaced)
{
    if (ColorReplaced) {
        m_ColorReplacedProgram = Program;
        (void) m_ProgramListClone.UpdateProgram(&m_ColorReplacedProgram);
        m_ColorReplaced = true;
    }
    else {
        m_Program.SetColor(Program.GetColor());
        (void) m_ProgramListClone.UpdateProgram(&m_Program);
    }
    mp_Ui->btnColor->SetColor(Program.GetColor());
}

/****************************************************************************/
/*!
 *  \brief This slot is called when Save button is clicked.
 */
/****************************************************************************/
void CModifyLeicaHne::OnSave()
{
    if (mp_ScrollWheelHaemotoxylin) {
        m_LeicaProgramStepOne.SetIntensity(mp_ScrollWheelHaemotoxylin->
                                           GetCurrentData().toInt());
    }

    if (mp_ScrollWheelEosin) {
        m_LeicaProgramStepTwo.SetIntensity(mp_ScrollWheelEosin->GetCurrentData().toInt());
    }

    (void) m_Program.UpdateProgramStep(&m_LeicaProgramStepOne);
    (void) m_Program.UpdateProgramStep(&m_LeicaProgramStepTwo);

    if (m_ProgramListClone.UpdateProgram(&m_Program) == true) {
        if(m_ColorReplaced == true) {
            emit ProgramColorReplaced(m_ColorReplacedProgram, m_Program);
        }
        else {
            emit UpdateProgram(m_Program);
        }
        accept();
    }
    else {
        MainMenu::CMessageDlg MessageDlg;
        MessageDlg.SetTitle(tr("Information Message"));
        MessageDlg.SetIcon(QMessageBox::Information);
        MessageDlg.SetText(tr("Program data could not be saved."));
        (void) MessageDlg.exec();
    }
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CModifyLeicaHne::RetranslateUI()
{
   MainMenu::CDialogFrame::SetDialogTitle(QApplication::translate("Programs::CModifyLeicaHne",
   "Edit Leica Program", 0, QApplication::UnicodeUTF8));
   mp_Ui->btnCancel->setText(QApplication::translate("Programs::CModifyLeicaHne",
   "Close", 0, QApplication::UnicodeUTF8));
}

} // end namespace Programs
