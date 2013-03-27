/****************************************************************************/
/*! \file ProgramWidget.cpp
 *
 *  \brief Implementation file for class CProgramWidget.
 *
 *  \b Description:
 *          This class implements widgets for displaying Program list in a table
 *          and provides the user interfaces to Edit, Add, Copy, Prioritize,
 *          Assigning Color to the program and to Delete a program.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-28
 *   $Author:  $ M.Scherer, N.Kamath, C.Adaragunchi1234
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
#include "Programs/Include/ProgramWidget.h"
#include "Programs/Include/ModifyProgramDlg.h"
#include "Programs/Include/ModifyProgramStepDlg.h"
#include "ui_ProgramWidget.h"
#include <QDebug>

namespace Programs {

#define MAX_PROGRAMS    50 //!< Maximum number of programs
#define MAX_USER_PROGRAMS 10 //!< Maximum number of User Programs

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_DataConnector = Reference to Global data container
 *  \iparam p_Parent = Reference to Parent object
 *  \iparam p_KeyBoard = Reference to Keyboard widget
 *  \iparam p_DataConnector = Reference to DataConnector
 */
/****************************************************************************/
CProgramWidget::CProgramWidget(Core::CDataConnector *p_DataConnector,
                                   MainMenu::CMainWindow *p_Parent,
                                   KeyBoard::CKeyBoard *p_KeyBoard) :
                                   MainMenu::CPanelFrame(p_Parent),
                                   mp_Ui(new Ui::CProgramWidget),
                                   mp_DataConnector(p_DataConnector),
                                   mp_MainWindow(p_Parent),m_UserRoleChanged(false),
                                   m_ProcessRunning(false), m_ColorReplaced(false)
{
    // Added void to please lint
    (void) m_NonStartableProgramErrorMap.insert("-1", tr("BathLayout was not generated for this program"));
    (void) m_NonStartableProgramErrorMap.insert("0", tr("Success"));
    (void) m_NonStartableProgramErrorMap.insert("1", tr("Heated Cuvette not mounted"));
    (void) m_NonStartableProgramErrorMap.insert("2", tr("Distilled Water station not available"));
    (void) m_NonStartableProgramErrorMap.insert("3", tr("Station not available"));
    (void) m_NonStartableProgramErrorMap.insert("4", tr("Oven Defect"));
    (void) m_NonStartableProgramErrorMap.insert("5", tr("Heated cuvette defect"));
    (void) m_NonStartableProgramErrorMap.insert("6", tr("Water Station defect"));
    (void) m_NonStartableProgramErrorMap.insert("7", tr("Reagent not present in bathlayout"));
    (void) m_NonStartableProgramErrorMap.insert("8", tr("Reagent Expired"));

    mp_Ui->setupUi(GetContentFrame());
    m_ProgramModel.SetProgramList(mp_DataConnector->ProgramList,5);
    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->setModel(&m_ProgramModel);
    mp_Ui->scrollTable->SetContent(mp_TableWidget);
    SetPanelTitle(tr("Programs"));

    mp_ModifyLeicaHne = new CModifyLeicaHne(this, p_Parent, mp_DataConnector);
    mp_ModifyLeicaHne->setModal(true);

    mp_ModifyProgramDlg = new CModifyProgramDlg(this, p_KeyBoard , p_Parent, mp_DataConnector);
    mp_ModifyProgramDlg->setModal(true);

    mp_DlgRackGripColor = new CRackGripColorDlg(this,p_Parent);
    mp_DlgRackGripColor->setModal(true);

    //mp_ManualProgramDlg = new CManualProgramDlg(this, p_Parent);
    //mp_ManualProgramDlg->setModal(true);

    CONNECTSIGNALSLOT(mp_Ui->btnEdit, clicked(), this, OnEdit());
    CONNECTSIGNALSLOT(mp_Ui->btnNew, clicked(), this, OnNew());
    CONNECTSIGNALSLOT(mp_Ui->btnCopy, clicked(), this, OnCopy());
    CONNECTSIGNALSLOT(mp_Ui->btnDelete, clicked(), this, OnDelete());
    CONNECTSIGNALSLOT(mp_TableWidget, clicked(QModelIndex), this,
                      SelectionChanged(QModelIndex));
    CONNECTSIGNALSLOT(mp_MainWindow, UserRoleChanged(), this, OnUserRoleChanged());
    CONNECTSIGNALSLOT(mp_MainWindow, ProcessStateChanged(), this, OnProcessStateChanged());
    CONNECTSIGNALSLOT(mp_MainWindow, CurrentTabChanged(int), this, OnCurrentTabChanged(int));
    CONNECTSIGNALSIGNAL(this, ReagentsUpdated(), mp_ModifyProgramDlg, ReagentsUpdated());
//    CONNECTSIGNALSLOT(this, UpdateProgramList(), &m_ProgramModel, UpdateProgramList());
//    CONNECTSIGNALSIGNAL(this, UpdateProgramList(), mp_ModifyProgramDlg, UpdateStepModel());
    CONNECTSIGNALSIGNAL(mp_ModifyProgramDlg, UpdateProgram(DataManager::CProgram &),
                        this, UpdateProgram(DataManager::CProgram &));
    CONNECTSIGNALSIGNAL(mp_ModifyLeicaHne, UpdateProgram(DataManager::CProgram &),
                        this, UpdateProgram(DataManager::CProgram &));
    CONNECTSIGNALSIGNAL(mp_ModifyProgramDlg, ProgramColorReplaced(DataManager::CProgram &,DataManager::CProgram &),
                        this, ProgramColorReplaced(DataManager::CProgram &,DataManager::CProgram &));
    CONNECTSIGNALSIGNAL(mp_ModifyLeicaHne, ProgramColorReplaced(DataManager::CProgram &,DataManager::CProgram &),
                        this, ProgramColorReplaced(DataManager::CProgram &,DataManager::CProgram &));
    CONNECTSIGNALSIGNAL(mp_ModifyProgramDlg, AddProgram(DataManager::CProgram &),
                        this, AddProgram(DataManager::CProgram &));
    CONNECTSIGNALSLOT(&m_ProgramModel, modelReset(), this, CloseDialogs());
    CONNECTSIGNALSLOT(mp_ModifyProgramDlg, CancelClicked(), this, OnCancelClicked());
    CONNECTSIGNALSLOT(mp_DataConnector, UpdateMode(QString), this, OnDeviceModeChanged(QString));
    CONNECTSIGNALSLOT(mp_DataConnector, ShowManualProgramDlg(QString),this , OnShowManualProgramDlg(QString));
    CONNECTSIGNALSLOT(mp_DataConnector, UserSettingsUpdated(), mp_ModifyProgramDlg, UpdateUserSettings());
    //CONNECTSIGNALSLOT(mp_ManualProgramDlg, ProgramSelected(QString), mp_DataConnector,
      //                onProgramSelected(QString));
    CONNECTSIGNALSLOT(mp_DlgRackGripColor, UpdateProgramColor(DataManager::CProgram &, bool),this,
                      OnUpdateProgramColor(DataManager::CProgram &, bool));
//    CONNECTSIGNALSLOT(mp_DataConnector, RevertChangedProgram(), mp_ModifyProgramDlg, UpdateProgramList());

    PopulateProgramList();
    OnUserRoleChanged();
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CProgramWidget::~CProgramWidget()
{
    try {
        delete mp_DlgRackGripColor;
        delete mp_ModifyProgramDlg;
        delete mp_ModifyLeicaHne;
        delete mp_ManualProgramDlg;
        delete mp_TableWidget;
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

void CProgramWidget::changeEvent(QEvent *p_Event)
{
    MainMenu::CPanelFrame::changeEvent(p_Event);
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
 *  \brief Resizes the columns in the program table
 */
/****************************************************************************/
void CProgramWidget::ResizeHorizontalSection()
{
    mp_TableWidget->horizontalHeader()->resizeSection(0, 70);
    mp_TableWidget->horizontalHeader()->resizeSection(1, 60);
    mp_TableWidget->horizontalHeader()->resizeSection(2, 210);
    mp_TableWidget->horizontalHeader()->resizeSection(3, 100);
    mp_TableWidget->horizontalHeader()->resizeSection(3, 70);
}

/****************************************************************************/
/*!
 *  \brief Fills the program list with content
 */
/****************************************************************************/
void CProgramWidget::PopulateProgramList()
{
    mp_TableWidget->SetVisibleRows(8);
    m_ProgramModel.SetVisibleRowCount(8);
    mp_TableWidget->horizontalHeader()->show();
    ResizeHorizontalSection();
}

/****************************************************************************/
/*!
 *  \brief Displays a dialog for the modification of a staining program
 */
/****************************************************************************/
void CProgramWidget::OnEdit()
{
    DataManager::CProgramStep FirstProgramStepWithLeicaReagent;
    DataManager::CProgramStep SecondProgramStepWithLeicaReagent;
    m_MessageDlg.SetText(tr("Staining Process has started, editing is no longer possible."
                            "\nPlease close the dialog."));
    if (mp_Program->GetID().at(0) != 'L') {
        if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
             m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
                (!m_ProcessRunning)) {
            //Edit Mode
            mp_ModifyProgramDlg->SetButtonType(EDIT_BTN_CLICKED);
            mp_ModifyProgramDlg->SetDialogTitle(tr("Edit Program"));
        }
        else {
            mp_ModifyProgramDlg->SetButtonType(EDIT_BTN_CLICKED);
            mp_ModifyProgramDlg->SetDialogTitle(tr("View Program"));
        }
        mp_ModifyProgramDlg->InitDialog(mp_Program);
        mp_ModifyProgramDlg->move(88, 50);
        mp_ModifyProgramDlg->show();
    }
    else {
        if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
             m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
                (!m_ProcessRunning)) {
            //Edit Mode
            mp_ModifyLeicaHne->SetDialogTitle(tr("Edit Leica Program"));
        }
        else {
            mp_ModifyLeicaHne->SetDialogTitle(tr("View Leica Program"));
    }
 /* //vinay     bool LeicaReagentsFound;
        LeicaReagentsFound = const_cast <DataManager::CProgram *>
                (mp_Program)->GetLeicaReagents(FirstProgramStepWithLeicaReagent, SecondProgramStepWithLeicaReagent);

        if(LeicaReagentsFound) {

            mp_ModifyLeicaHne->InitDailog(mp_Program, FirstProgramStepWithLeicaReagent,
                                          SecondProgramStepWithLeicaReagent);
            mp_ModifyLeicaHne->SetIntensity(FirstProgramStepWithLeicaReagent.GetIntensity(),
                                            SecondProgramStepWithLeicaReagent.GetIntensity());
            mp_ModifyLeicaHne->move(88, 50);
            mp_ModifyLeicaHne->show();
        }
        else {
            m_MessageDlg.SetTitle(tr("Information Message"));
            m_MessageDlg.SetIcon(QMessageBox::Information);
            m_MessageDlg.SetButtonText(1, tr("Ok"));
            m_MessageDlg.HideButtons();
            m_MessageDlg.SetText(tr("Leica Reagents not found"));
            (void) m_MessageDlg.exec();
        }*/
    }
}

/****************************************************************************/
/*!
 *  \brief Creates a new staining program and opens its edit dialog
 */
/****************************************************************************/
void CProgramWidget::OnNew()
{

     m_MessageDlg.SetText(tr("Staining Process has started, Editing is no longer possible."
                            "\nPlease close the dialog."));
    mp_ModifyProgramDlg->SetDialogTitle(tr("New Program"));
    mp_ModifyProgramDlg->SetButtonType(NEW_BTN_CLICKED);
    mp_ModifyProgramDlg->InitDialog(NULL);
    mp_ModifyProgramDlg->move(88, 50);
    mp_ModifyProgramDlg->show();

}

/****************************************************************************/
/*!
 *  \brief Copies the currently selected staining program
 */
/****************************************************************************/
void CProgramWidget::OnCopy()
{
    m_MessageDlg.SetText(tr("Staining Process has started, Editing is no longer possible."
                            "\nPlease close the dialog."));
    mp_ModifyProgramDlg->SetDialogTitle(tr("Edit Program"));
    mp_ModifyProgramDlg->SetButtonType(COPY_BTN_CLICKED);
    mp_ModifyProgramDlg->InitDialog(mp_Program);
    mp_ModifyProgramDlg->move(88, 50);
    mp_ModifyProgramDlg->show();
}

/****************************************************************************/
/*!
 *  \brief Deletes the currently selected staining program
 */
/****************************************************************************/
void CProgramWidget::OnDelete()
{
    m_MessageDlg.SetText(tr("Staining Process has started, Editing is no longer possible."
                            "\nPlease close the dialog."));

    MainMenu::CMessageDlg ConfirmationMessageDlg;
    ConfirmationMessageDlg.SetTitle(tr("Confirmation Message"));
    ConfirmationMessageDlg.SetIcon(QMessageBox::Question);
    ConfirmationMessageDlg.SetButtonText(1, tr("Yes"));
    ConfirmationMessageDlg.SetButtonText(3,tr("Cancel"));
    ConfirmationMessageDlg.HideCenterButton();
    ConfirmationMessageDlg.SetText(tr("Do you really want to delete the selected program?"));

    if (ConfirmationMessageDlg.exec() == (int)QDialog::Accepted) {
        QString ProgramID = mp_Program->GetID();
        emit DeleteProgram(ProgramID);
    }
}

/****************************************************************************/
/*!
 *  \brief Shows the color selection dialog
 */
/****************************************************************************/
//void CProgramWidget::OnColor()
//{
//    m_MessageDlg.SetText(tr("Staining Process has started, Editing is no longer possible."
//                            "\nPlease close the dialog."));
//    if (mp_DlgRackGripColor->Init(mp_DataConnector->ProgramList, const_cast<DataManager::CProgram*>
//                                  (mp_Program)) == true) {
//        mp_DlgRackGripColor->SetSaveButton(tr("Save"));
//        mp_DlgRackGripColor->show();
//    }
//}

/****************************************************************************/
/*!
 *  \brief This slot is called when User Role changes
 */
/****************************************************************************/
void CProgramWidget::OnUserRoleChanged()
{
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    m_ProgramModel.SetUserRole(m_CurrentUserRole);

    if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
         m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
            (!m_ProcessRunning)) {
        //Edit Mode
        mp_Ui->btnEdit->setText(tr("Edit"));

        m_UserProgramCount = GetNumberOfUserPrograms();
        if(m_UserProgramCount > MAX_USER_PROGRAMS)
        {
            mp_Ui->btnNew->setEnabled(false);
        }
        else
        {
            mp_Ui->btnNew->setEnabled(true);
        }
        m_ProgramModel.ResetandUpdateModel();
    }
    else {
        //View Mode
        mp_Ui->btnEdit->setText(tr("View"));
        mp_Ui->btnNew->setEnabled(false);
    }
    m_UserRoleChanged = true;

}

/****************************************************************************/
/*!
 *  \brief This slot is called when Process state changes
 */
/****************************************************************************/
void CProgramWidget::OnProcessStateChanged()
{
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
         m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
            (!m_ProcessRunning)) {
        //Edit Mode
        mp_Ui->btnEdit->setText(tr("Edit"));
        mp_Ui->btnDelete->setEnabled(false);
        mp_Ui->btnNew->setEnabled(true);
        mp_Ui->btnCopy->setEnabled(false);
        mp_Ui->btnEdit->setEnabled(false);
    }
    else {
        //View Mode
        mp_Ui->btnEdit->setText(tr("View"));
        mp_Ui->btnNew->setEnabled(false);
        mp_Ui->btnCopy->setEnabled(false);
        mp_Ui->btnDelete->setEnabled(false);

        if(m_ProcessRunning && m_ShowMessageDialog){
            m_MessageDlg.SetTitle(tr("Information Message"));
            m_MessageDlg.SetIcon(QMessageBox::Information);
            m_MessageDlg.SetButtonText(1, tr("Ok"));
            m_MessageDlg.HideButtons();
            m_MessageDlg.SetText(tr("Staining Process has started.Editing is no longer possible."));
            (void) m_MessageDlg.exec();
        }
    }
}

/****************************************************************************/
/*!
 *  \brief This event is called whenever widget is shown
 */
/****************************************************************************/
void CProgramWidget::showEvent(QShowEvent *)
{
    m_ShowMessageDialog = true ;
    mp_TableWidget->clearSelection();
    mp_Ui->scrollTable->Reset();
    ResetButtons();
}

/****************************************************************************/
/*!
 *  \brief Called when the selection of the table is changed
 *
 *  \iparam Index = Currently selected cell
 */
/****************************************************************************/
void CProgramWidget::SelectionChanged(QModelIndex Index)
{
    m_ID = m_ProgramModel.data(Index, (int)Qt::UserRole).toString();
    int SelectedIndex = Index.row();
    m_ProgramModel.SelectedRowIndex(SelectedIndex);
    mp_Program = mp_DataConnector->ProgramList->GetProgram(m_ID);
    if (SelectedIndex + 1 > mp_DataConnector->ProgramList->GetNumberOfPrograms()) {
        mp_TableWidget->clearSelection();
        ResetButtons();
    }
    else {
        if (mp_Program) {
            bool IsLeicaProgram = false;
            if (mp_Program->GetID().at(0) == 'L') {
                IsLeicaProgram = true;
                if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
                     m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
                        (!m_ProcessRunning)) {
                    //Edit Mode
                    mp_Ui->btnEdit->setEnabled(IsLeicaProgram);
                    mp_Ui->btnDelete->setEnabled(!IsLeicaProgram);
                    mp_Ui->btnCopy->setEnabled(!IsLeicaProgram);
                } else {
                    //View Mode
                    mp_Ui->btnEdit->setEnabled(IsLeicaProgram);
                }
            }
            else {
                if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
                     m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
                        (!m_ProcessRunning)) {
                    //Edit Mode
                    mp_Ui->btnEdit->setEnabled(!IsLeicaProgram);
                    mp_Ui->btnCopy->setEnabled(!IsLeicaProgram);
                    mp_Ui->btnDelete->setEnabled(!IsLeicaProgram);
                }
                else {
                    mp_Ui->btnEdit->setEnabled(!IsLeicaProgram);
                }
            }
        }
        if (mp_DataConnector->ProgramList->GetNumberOfPrograms() >= MAX_PROGRAMS) {
            mp_Ui->btnNew->setEnabled(false);
            mp_Ui->btnCopy->setEnabled(false);
        }
    }
}

/****************************************************************************/
/*!
 *  \brief Closes opened dialogs when master sends updated programs xml data.
 */
/****************************************************************************/
void CProgramWidget::CloseDialogs()
{
    ResetButtons();
    mp_ModifyLeicaHne->accept();
    mp_ModifyProgramDlg->CloseDialogModifyStepDlg();
    mp_ModifyProgramDlg->accept();
}

/****************************************************************************/
/*!
 *  \brief This slot is called when Cancel is clicked in ModifyProgramDialog
 */
/****************************************************************************/
void CProgramWidget::OnCancelClicked()
{
    mp_TableWidget->clearSelection();
    ResetButtons();
}

/****************************************************************************/
/*!
 *  \brief This resets the buttons on the right of the screen
 */
/****************************************************************************/
void CProgramWidget::ResetButtons()
{
    if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
         m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
            (!m_ProcessRunning)) {
        //Edit Mode
        mp_Ui->btnEdit->setEnabled(false);
        mp_Ui->btnDelete->setEnabled(false);
        mp_Ui->btnCopy->setEnabled(false);
        if (mp_DataConnector->ProgramList->GetNumberOfPrograms() < MAX_PROGRAMS) {
            mp_Ui->btnNew->setEnabled(true);
        }
        else {
            mp_Ui->btnNew->setEnabled(false);
        }
    }
    else {
        //View Mode
        mp_Ui->btnEdit->setEnabled(false);
        mp_Ui->btnDelete->setEnabled(false);
        mp_Ui->btnCopy->setEnabled(false);
        mp_Ui->btnNew->setEnabled(false);
    }
}
/****************************************************************************/
/*!
 *  \brief This Slot is called when the Ok button on the Message box is clicked.
 *
 *  \iparam CurrentIndex = Currently selected row/cell
 */
/****************************************************************************/
void CProgramWidget::OnCurrentTabChanged(int CurrentIndex)
{
    if (CurrentIndex == 2) {
        m_ShowMessageDialog = true;
    }
    else {
        m_ShowMessageDialog = false;
    }
}

/****************************************************************************/
/*!
 *  \brief This slot is called when the device mode is changed.
 *
 *  \iparam DeviceMode = Device Mode (Standalone/Workstation mode)
 */
/****************************************************************************/
void CProgramWidget::OnDeviceModeChanged(QString DeviceMode)
{
    mp_ModifyProgramDlg->SetCurrentDeviceMode(DeviceMode);
}

/********************************************************************************/
/*!
 *  \brief This slot is called when UpdateProgramColor in RackColorGrip is emitted.
 *
 *  \iparam Program = Selected/New Program
 *  \iparam ColorReplaced = True(Color of existing program replaced)/ False
 */
/********************************************************************************/
void CProgramWidget::OnUpdateProgramColor(DataManager::CProgram &Program, bool ColorReplaced)
{
    DataManager::CDataProgramList ProgramListClone;
    ProgramListClone = *(mp_DataConnector->ProgramList);
    bool Result;
    bool UpdateResult;
    m_MessageDlg.SetTitle(tr("Information Message"));
    m_MessageDlg.SetIcon(QMessageBox::Information);
    m_MessageDlg.SetButtonText(1, tr("Ok"));
    m_MessageDlg.HideButtons();

    if (ColorReplaced) {
        m_ColorReplacedProgram = Program;
        m_ColorReplaced = true ;
    }
    else {
        if (m_ColorReplaced) {
            Result = ProgramListClone.UpdateProgram(&m_ColorReplacedProgram);
            UpdateResult = ProgramListClone.UpdateProgram(&Program);
            if (Result && UpdateResult) {

                emit ProgramColorReplaced(m_ColorReplacedProgram, Program);
            }
            else {
                m_MessageDlg.SetText(tr("Program Color Update failed"));
                m_MessageDlg.show();
            }
            m_ColorReplaced = false;
        }
        else {
            Result = ProgramListClone.UpdateProgram(&Program);
            if (Result) {
                emit UpdateProgram(Program);
            }
            else {
                m_MessageDlg.SetText(tr("Program Color Update failed"));
                m_MessageDlg.show();
            }
        }
    }
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CProgramWidget::RetranslateUI()
{
    MainMenu::CPanelFrame::SetPanelTitle(QApplication::translate("Programs::CProgramWidget", "Programs",
                                                                 0, QApplication::UnicodeUTF8));
    //Added void to please lint
    (void) m_ProgramModel.setHeaderData(0,Qt::Horizontal,QApplication::translate("Programs::CProgramModel",
    "Number", 0, QApplication::UnicodeUTF8),0);
    (void) m_ProgramModel.setHeaderData(1,Qt::Horizontal,QApplication::translate("Programs::CProgramModel",
    "Name", 0, QApplication::UnicodeUTF8),0);
    (void) m_ProgramModel.setHeaderData(2,Qt::Horizontal,QApplication::translate("Programs::CProgramModel",
    "Duration", 0, QApplication::UnicodeUTF8),0);
    (void) m_ProgramModel.setHeaderData(3,Qt::Horizontal,QApplication::translate("Programs::CProgramModel",
    "Icon.", 0, QApplication::UnicodeUTF8),0);
//    (void) m_ProgramModel.setHeaderData(4,Qt::Horizontal,QApplication::translate("Programs::CProgramModel",
//    "Leica", 0, QApplication::UnicodeUTF8),0);
    mp_ModifyProgramDlg->SetDialogTitle(QApplication::translate("Programs::CProgramWidget", "New Program",
    0, QApplication::UnicodeUTF8));

    if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
         m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
            (!m_ProcessRunning)) {
        mp_Ui->btnEdit->setText(QApplication::translate("Programs::CProgramWidget","Edit", 0,
                                                        QApplication::UnicodeUTF8));
    }
    else {
        mp_Ui->btnEdit->setText(QApplication::translate("Programs::CProgramWidget","View", 0,
                                                        QApplication::UnicodeUTF8));
    }
}

/****************************************************************************/
/*!
 *  \brief Diaplying Startable programs list When White Racks Inserted
 *
 *  \iparam m_RackStationID = Station Id in which Rack is present.
 */
/****************************************************************************/
void CProgramWidget::OnShowManualProgramDlg(QString m_RackStationID)
{
    //(void) mp_ManualProgramDlg->SetProgramList(mp_DataConnector->ProgramList);
    //emit ManualProgramShow(m_RackStationID);
}

int CProgramWidget::GetNumberOfUserPrograms()
{
    int Count;
    int ProgramCount = 0;
    for (Count=0; Count<mp_DataConnector->ProgramList->GetNumberOfPrograms(); Count++)
    {
        mp_Program = mp_DataConnector->ProgramList->GetProgram(Count);

        if(mp_Program->GetID().at(0) != 'L')
        {
            ProgramCount += 1;
        }
    }
    return ProgramCount;
}
} // end namespace Programs
