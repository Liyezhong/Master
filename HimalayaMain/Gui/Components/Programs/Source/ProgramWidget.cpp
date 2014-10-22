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
#include "Core/Include/GlobalHelper.h"



namespace Programs {

#define MAX_PROGRAMS    13 //!< Maximum number of programs
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
                                   m_ProcessRunning(false), m_ColorReplaced(false),
                                   m_strEditProgram(tr("Edit Program")),
                                   m_strViewProgram(tr("View Program")),
                                   m_strNewProgram(tr("New Program")),
                                   m_strDelProgram(tr("Do you really want to delete the selected program?")),
                                   m_strNotEditProgram(tr("Program Process has started.Editing is no longer possible.")),
                                   m_strView(tr("View")),
                                   m_strEdit(tr("Edit"))
{
    //DataManager::CUserSettings *p_Settings = mp_DataConnector->SettingsInterface->GetUserSettings();

    mp_Ui->setupUi(GetContentFrame());
    m_ProgramModel.SetProgramList(mp_DataConnector->ProgramList,5);    
    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->setModel(&m_ProgramModel);
    mp_Ui->scrollTable->SetContent(mp_TableWidget);
    SetPanelTitle(tr("Programs"));

    mp_ModifyProgramDlg = new CModifyProgramDlg(this, p_KeyBoard , p_Parent, mp_DataConnector);
    mp_ModifyProgramDlg->setModal(true);

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
    CONNECTSIGNALSLOT(this, UpdateProgramList(), &m_ProgramModel, OnUpdateProgramList());
    CONNECTSIGNALSIGNAL(this, UpdateProgramList(), mp_ModifyProgramDlg, UpdateStepModel());
    CONNECTSIGNALSIGNAL(mp_ModifyProgramDlg, UpdateProgram(DataManager::CProgram &),
                        this, UpdateProgram(DataManager::CProgram &));

    CONNECTSIGNALSIGNAL(mp_ModifyProgramDlg, ProgramColorReplaced(DataManager::CProgram &,DataManager::CProgram &),
                        this, ProgramColorReplaced(DataManager::CProgram &,DataManager::CProgram &));

    CONNECTSIGNALSIGNAL(mp_ModifyProgramDlg, AddProgram(DataManager::CProgram &),
                        this, AddProgram(DataManager::CProgram &));
    CONNECTSIGNALSLOT(&m_ProgramModel, modelReset(), this, CloseDialogs());
    CONNECTSIGNALSLOT(mp_ModifyProgramDlg, CancelClicked(), this, OnCancelClicked());

    CONNECTSIGNALSIGNAL(&m_ProgramModel, FavoriteProgramListUpdated(DataManager::CProgram &), this, FavoriteProgramListUpdated(DataManager::CProgram &));
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
        delete mp_ModifyProgramDlg;
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
 *  \brief Displays a dialog for the modification of a program
 */
/****************************************************************************/
void CProgramWidget::OnEdit()
{
        if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
             m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
                (!m_ProcessRunning))
        {
            // View Mode
            if (mp_Ui->btnEdit->text() == m_strView) {
                mp_ModifyProgramDlg->SetButtonType(EDIT_BTN_CLICKED);
                mp_ModifyProgramDlg->SetDialogTitle(m_strViewProgram);
            }
            else {

                //Edit Mode
                bool bRevertSelectedProgram = false;
                if (!Core::CGlobalHelper::CheckSelectedProgram(bRevertSelectedProgram, mp_Program->GetID()))
                   return;
                if (bRevertSelectedProgram)
                    emit UnselectProgram();

                mp_ModifyProgramDlg->SetButtonType(EDIT_BTN_CLICKED);
                mp_ModifyProgramDlg->SetDialogTitle(m_strEditProgram);
            }
        }
        else
        {
            mp_ModifyProgramDlg->SetButtonType(EDIT_BTN_CLICKED);
            mp_ModifyProgramDlg->SetDialogTitle(m_strViewProgram);
        }
        mp_ModifyProgramDlg->InitDialog(mp_Program);
        mp_ModifyProgramDlg->show();
}

/****************************************************************************/
/*!
 *  \brief Creates a new program and opens its edit dialog
 */
/****************************************************************************/
void CProgramWidget::OnNew()
{
    ResetButtons();
    mp_ModifyProgramDlg->SetDialogTitle(m_strNewProgram);
    mp_ModifyProgramDlg->SetButtonType(NEW_BTN_CLICKED);
    mp_ModifyProgramDlg->InitDialog(NULL);
    mp_ModifyProgramDlg->show();
}

/****************************************************************************/
/*!
 *  \brief Copies the currently selected program
 */
/****************************************************************************/
void CProgramWidget::OnCopy()
{
    /*m_MessageDlg.SetText(tr("Process has started, Editing is no longer possible."
                            "\nPlease close the dialog."));*/
    mp_ModifyProgramDlg->SetDialogTitle(m_strEditProgram);
    mp_ModifyProgramDlg->SetButtonType(COPY_BTN_CLICKED);
    mp_ModifyProgramDlg->InitDialog(mp_Program);
    mp_ModifyProgramDlg->show();
}

/****************************************************************************/
/*!
 *  \brief Deletes the currently selected program
 */
/****************************************************************************/
void CProgramWidget::OnDelete()
{
    MainMenu::CMessageDlg dlg;
    dlg.SetTitle(m_strConfirmMsg);
    dlg.SetIcon(QMessageBox::Warning);
    dlg.SetButtonText(1, m_strYes);
    dlg.SetButtonText(3, m_strCancel);
    dlg.HideCenterButton();
    dlg.SetText(m_strDelProgram);

    if (dlg.exec() == (int)QDialog::Accepted) {
        QString ProgramID = mp_Program->GetID();

        bool bRevertSelectedProgram = false;
        if (!Core::CGlobalHelper::CheckSelectedProgram(bRevertSelectedProgram, ProgramID))
            return;
        if (bRevertSelectedProgram)
            emit UnselectProgram();

        emit DeleteProgram(ProgramID);
        ResetButtons();
    }
}

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
        mp_Ui->btnEdit->setText(m_strEdit);
        m_UserProgramCount = GetNumberOfUserPrograms();
        if(m_UserProgramCount >= MAX_USER_PROGRAMS)
        {
            mp_Ui->btnNew->setEnabled(false);
            mp_Ui->btnCopy->setEnabled(false);
        }
        else
        {
            mp_Ui->btnNew->setEnabled(true);
            mp_Ui->btnCopy->setEnabled(true);
        }
        m_ProgramModel.ResetandUpdateModel();
    }
    else {
        //View Mode
        mp_Ui->btnEdit->setText(m_strView);
        mp_Ui->btnNew->setEnabled(false);
        mp_Ui->btnDelete->setEnabled(false);
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
        mp_Ui->btnEdit->setText(m_strEdit);
        mp_Ui->btnDelete->setEnabled(false);
        m_UserProgramCount = GetNumberOfUserPrograms();
        if(m_UserProgramCount< MAX_USER_PROGRAMS)
        {
            mp_Ui->btnNew->setEnabled(true);
            mp_Ui->btnCopy->setEnabled(true);

        }
        else
        {
            mp_Ui->btnNew->setEnabled(false);
            mp_Ui->btnCopy->setEnabled(false);

        }
        mp_Ui->btnEdit->setEnabled(false);
    }
    else {
        //View Mode
        mp_Ui->btnEdit->setText(m_strView);
        mp_Ui->btnNew->setEnabled(false);
        mp_Ui->btnCopy->setEnabled(false);
        mp_Ui->btnDelete->setEnabled(false);
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
    if (!Index.isValid()) {
        Index = mp_TableWidget->currentIndex();
        if (!Index.isValid()) {
            mp_TableWidget->clearSelection();
            ResetButtons();
            return;
        }
    }

    if (Index.column() == 1)  // apply column
    {
        Qt::ItemFlags flags = m_ProgramModel.flags(Index);
        if (flags == Qt::ItemIsSelectable)  // if the column is disabled, no action will be done.
            return ;
    }

    if (Core::CGlobalHelper::CheckIfCanEdit() == false) {
        mp_Ui->btnEdit->setText(m_strView);

        m_ProgramID = m_ProgramModel.data(Index, (int)Qt::UserRole).toString();
        int SelectedIndex = Index.row();
        m_ProgramModel.SelectedRowIndex(SelectedIndex);
        mp_Program = mp_DataConnector->ProgramList->GetProgram(m_ProgramID);


        mp_Ui->btnEdit->setEnabled(true);
        mp_Ui->btnNew->setEnabled(false);
        mp_Ui->btnCopy->setEnabled(false);
        mp_Ui->btnDelete->setEnabled(false);
        return ;
    }

    m_ProgramID = m_ProgramModel.data(Index, (int)Qt::UserRole).toString();
    int SelectedIndex = Index.row();
    m_ProgramModel.SelectedRowIndex(SelectedIndex);
    mp_Program = mp_DataConnector->ProgramList->GetProgram(m_ProgramID);
    if (SelectedIndex + 1 > mp_DataConnector->ProgramList->GetNumberOfPrograms()) {
        mp_TableWidget->clearSelection();
        ResetButtons();
    }
    else {
        if (mp_Program) {
            bool IsLeicaProgram = mp_Program->IsLeicaProgram();
            if (IsLeicaProgram) {
                if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
                     m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
                        (!m_ProcessRunning)) {
                    //Edit Mode
                    //mp_Ui->btnEdit->setEnabled(!IsLeicaProgram);
                    mp_Ui->btnEdit->setText(m_strView);
                    mp_Ui->btnDelete->setEnabled(!IsLeicaProgram);

                    if (mp_Program->GetID().at(0) == 'C')//Cleaning program
                    {
                        mp_Ui->btnEdit->setEnabled(true);
                        mp_Ui->btnCopy->setEnabled(!IsLeicaProgram);
                    }
                    else
                    {
                        mp_Ui->btnEdit->setEnabled(true);
                        mp_Ui->btnCopy->setEnabled(IsLeicaProgram);
                    }
                } else {
                    //View Mode
                    mp_Ui->btnEdit->setText(m_strView);
                    if (mp_Program->GetID().at(0) == 'C')
                    {
                        mp_Ui->btnCopy->setEnabled(false);
                    }
                    mp_Ui->btnEdit->setEnabled(IsLeicaProgram);
                }
            }
            else {
                if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
                     m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
                        (!m_ProcessRunning)) {

                    // if the program has run and be paused, edit or delete program is not permitted.
                    if (Core::CGlobalHelper::CheckIfCanEdit(mp_Program->
                                                            GetID(), 1) == false) {
                        mp_Ui->btnEdit->setText(m_strView);
                        mp_Ui->btnEdit->setEnabled(!IsLeicaProgram);
                        mp_Ui->btnCopy->setEnabled(true);
                        mp_Ui->btnDelete->setEnabled(false);
                    }
                    else {
                        //Edit Mode
                        mp_Ui->btnEdit->setText(m_strEdit);
                        mp_Ui->btnEdit->setEnabled(!IsLeicaProgram);
                        mp_Ui->btnCopy->setEnabled(!IsLeicaProgram);
                        mp_Ui->btnDelete->setEnabled(!IsLeicaProgram);
                    }
                }
                else {
                    mp_Ui->btnEdit->setText(m_strView);
                    mp_Ui->btnEdit->setEnabled(!IsLeicaProgram);
                }
            }
        }
        m_UserProgramCount = GetNumberOfUserPrograms();
        if (mp_DataConnector->ProgramList->GetNumberOfPrograms() >= MAX_PROGRAMS || m_UserProgramCount>= MAX_USER_PROGRAMS) {
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
        m_UserProgramCount = GetNumberOfUserPrograms();
        if (mp_DataConnector->ProgramList->GetNumberOfPrograms() < MAX_PROGRAMS
                &&  m_UserProgramCount < MAX_USER_PROGRAMS) {
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
 *  \brief Calculates the number of User Programs
 *  \return the count of user program
 */
/****************************************************************************/
int CProgramWidget::GetNumberOfUserPrograms()
{
    int ProgramCount;
    int UserProgramCount = 0;
    for (ProgramCount=0; ProgramCount< mp_DataConnector->ProgramList->GetNumberOfPrograms(); ProgramCount++)
    {
        DataManager::CProgram *p_Program = mp_DataConnector->ProgramList->GetProgram(ProgramCount);

        if(!p_Program->IsLeicaProgram())
        {
            UserProgramCount += 1;
        }
    }
    return UserProgramCount;
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
    (void) m_ProgramModel.setHeaderData(0,Qt::Horizontal,QApplication::translate("Programs::CProgramModel",
    "Apply", 0, QApplication::UnicodeUTF8),0);
    (void) m_ProgramModel.setHeaderData(1,Qt::Horizontal,QApplication::translate("Programs::CProgramModel",
    "Name", 0, QApplication::UnicodeUTF8),0);
    (void) m_ProgramModel.setHeaderData(2,Qt::Horizontal,QApplication::translate("Programs::CProgramModel",
    "Duration", 0, QApplication::UnicodeUTF8),0);
    (void) m_ProgramModel.setHeaderData(3,Qt::Horizontal,QApplication::translate("Programs::CProgramModel",
    "Icon", 0, QApplication::UnicodeUTF8),0);

    m_strEditProgram = QApplication::translate("Programs::CProgramWidget", "Edit Program",
                                                                   0, QApplication::UnicodeUTF8);

    m_strViewProgram = QApplication::translate("Programs::CProgramWidget", "View Program",
                                                                   0, QApplication::UnicodeUTF8);

    m_strNewProgram = QApplication::translate("Programs::CProgramWidget", "New Program",
                                                                   0, QApplication::UnicodeUTF8);

    m_strDelProgram = QApplication::translate("Programs::CProgramWidget", "Do you really want to delete the selected program?",
                                              0, QApplication::UnicodeUTF8);

    m_strNotEditProgram = QApplication::translate("Programs::CProgramWidget", "Program Process has started.Editing is no longer possible.",
                                              0, QApplication::UnicodeUTF8);


    m_strView = QApplication::translate("Programs::CProgramWidget", "View",
                                              0, QApplication::UnicodeUTF8);

    m_strEdit = QApplication::translate("Programs::CProgramWidget", "Edit",
                                              0, QApplication::UnicodeUTF8);
    m_strConfirmMsg = QApplication::translate("Programs::CProgramWidget", "Confirmation Message", 0, QApplication::UnicodeUTF8);

    m_strYes = QApplication::translate("Programs::CProgramWidget", "Yes", 0, QApplication::UnicodeUTF8);
    m_strCancel = QApplication::translate("Programs::CProgramWidget", "Cancel", 0, QApplication::UnicodeUTF8);


}


} // end namespace Programs
