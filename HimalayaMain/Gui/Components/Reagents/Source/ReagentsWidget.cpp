/****************************************************************************/
/*! \file ReagentWidget.cpp.cpp
 *
 *  \brief ReagentWidget Implementation.
 *
 *   $Version: $ 0.2
 *   $Date:    $ 2013-01-15
 *   $Author:  $ Swati Tiwari
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2012 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include "MainMenu/Include/ScrollWheel.h"
#include "MainMenu/Include/SliderControl.h"
#include "Reagents/Include/ReagentsWidget.h"
#include "Reagents/Include/ModifyReagentDlg.h"
#include "ui_ReagentsWidget.h"
#include <QDebug>

namespace Reagents {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CReagentsWidget::CReagentsWidget(QWidget *p_Parent):
    MainMenu::CPanelFrame(p_Parent),
    mp_Ui(new Ui::CReagentsWidget),
    mp_DataConnector(NULL),
    mp_ReagentList(NULL),
    mp_Reagent(NULL)
{
    mp_Ui->setupUi(GetContentFrame());
    SetPanelTitle(tr("Reagents"));

    m_ProcessRunning = false;
    m_ShowMessageDialog = false;

    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->setModel(&m_ReagentModel);
    mp_Ui->scrollTable->SetContent(mp_TableWidget);
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
    m_ReagentModel.SetRMSOption(Global::RMS_CASSETTES);
    mp_Ui->radioCassettes->setChecked(true);
    mp_Ui->radioOff1->setChecked(true);

    CONNECTSIGNALSLOT(mp_Ui->btnEdit, clicked(), this, OnEdit());
    CONNECTSIGNALSLOT(mp_Ui->btnNew, clicked(), this, OnNew());
    CONNECTSIGNALSLOT(mp_Ui->btnDelete, clicked(), this, OnDelete());
    CONNECTSIGNALSLOT(mp_Ui->radioOff, clicked(), this, OnRMSOFF());
    CONNECTSIGNALSLOT(mp_Ui->radioCassettes, clicked(), this, OnRMSCassettes());
    CONNECTSIGNALSLOT(mp_Ui->radioCycles, clicked(), this, OnRMSCycles());
    CONNECTSIGNALSLOT(mp_Ui->radioDays, clicked(), this, OnRMSDays());
    CONNECTSIGNALSLOT(mp_TableWidget, pressed(QModelIndex), this, SelectionChanged(QModelIndex));
    CONNECTSIGNALSLOT(this, UpdateReagentList(), &m_ReagentModel, UpdateReagentList());
    CONNECTSIGNALSLOT(&m_ReagentModel, modelReset(), this, CloseDialogs());
    CONNECTSIGNALSLOT(mp_Ui->scrollTable,Scrolled(),this,OnContentScrolled());


    PopulateReagentList();
}

/****************************************************************************/
/*!
     *  \brief Destructor
     */
/****************************************************************************/
CReagentsWidget::~CReagentsWidget()
{
    try {
        delete mp_TableWidget;
        delete mp_Ui;
        delete mp_ModifiyReagentDlg;
        delete mp_KeyBoard;
        delete mp_MainWindow;
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
void CReagentsWidget::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
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
 *  \brief Resizes the columns in the reagents table
 */
/****************************************************************************/
void CReagentsWidget::ResizeHorizontalSection()
{
    if (mp_TableWidget == NULL)
    {
        return;
    }
    mp_TableWidget->horizontalHeader()->resizeSection(0, 120);
    mp_TableWidget->horizontalHeader()->resizeSection(1, 150);
    mp_TableWidget->horizontalHeader()->resizeSection(2, 140);

}
/****************************************************************************/
/*!
 *  \brief Fills the reagents list with content
 */
/****************************************************************************/
void CReagentsWidget::PopulateReagentList()
{
    mp_TableWidget->SetVisibleRows(7,28,32);
    m_ReagentModel.SetVisibleRowCount(7);
    mp_TableWidget->horizontalHeader()->show();
    mp_TableWidget->verticalHeader()->setDefaultSectionSize(32);

    ResizeHorizontalSection();
}
/****************************************************************************/
/*!
 *  \brief This slot is used to close all open dialogs of this widget
 */
/****************************************************************************/
void CReagentsWidget::CloseDialogs()
{
    ResetButtons();
    if (mp_ModifiyReagentDlg) {
        mp_ModifiyReagentDlg->accept();
    }
}

/****************************************************************************/
/*!
 *  \brief Displays a dialog for the modification of a staining reagent
 */
/****************************************************************************/
void CReagentsWidget::OnRMSOFF()
{
    m_ReagentModel.SetRMSOption(Global::RMS_OFF);
    mp_TableWidget->setColumnHidden(2, true);
    m_ReagentModel.ResetAndUpdateModel();
    emit RMSChanged(Global::RMS_OFF);
}

/****************************************************************************/
/*!
 *  \brief Displays a dialog for the modification of a staining reagent
 */
/****************************************************************************/
void CReagentsWidget::OnRMSCassettes()
{
    m_ReagentModel.SetRMSOption(Global::RMS_CASSETTES);
    mp_TableWidget->setColumnHidden(2, false);
    m_ReagentModel.ResetAndUpdateModel();
    ResizeHorizontalSection();
    emit RMSChanged(Global::RMS_CASSETTES);
}

/****************************************************************************/
/*!
 *  \brief Displays a dialog for the modification of a staining reagent
 */
/****************************************************************************/
void CReagentsWidget::OnRMSCycles()
{
    m_ReagentModel.SetRMSOption(Global::RMS_CYCLES);
    mp_TableWidget->setColumnHidden(2, false);
    m_ReagentModel.ResetAndUpdateModel();
    ResizeHorizontalSection();
    emit RMSChanged(Global::RMS_CYCLES);
}

/****************************************************************************/
/*!
 *  \brief Displays a dialog for the modification of a staining reagent
 */
/****************************************************************************/
void CReagentsWidget::OnRMSDays()
{
    m_ReagentModel.SetRMSOption(Global::RMS_DAYS);
    mp_TableWidget->setColumnHidden(2, false);
    m_ReagentModel.ResetAndUpdateModel();
    ResizeHorizontalSection();
    emit RMSChanged(Global::RMS_DAYS);
}

/****************************************************************************/
/*!
 *  \brief Displays a dialog for the modification of a staining reagent
 */
/****************************************************************************/
void CReagentsWidget::OnEdit()
{
    m_MessageDlg.SetText(tr("Staining Process has started, Editing is no longer possible."
                            "\nPlease close the dialog with \"Close\""));
    mp_ModifiyReagentDlg->SetDialogTitle(tr("Edit Reagent"));
    mp_ModifiyReagentDlg->SetButtonType(EDIT_BTN_CLICKED);
    mp_ModifiyReagentDlg->InitDialog(mp_Reagent, mp_DataConnector->ReagentGroupList, m_ReagentModel.GetRMSOption());
    mp_ModifiyReagentDlg->move(96,70);
    mp_ModifiyReagentDlg->show();
}

/****************************************************************************/
/*!
 *  \brief Creates a new staining reagent and opens its edit dialog
 */
/****************************************************************************/
void CReagentsWidget::OnNew()
{
    m_MessageDlg.SetText(tr("Staining Process has started, editing is no longer possible."
                            "\nPlease close the dialog with \"Close\""));
    mp_ModifiyReagentDlg->SetDialogTitle(tr("New Reagent"));
    mp_ModifiyReagentDlg->SetButtonType(NEW_BTN_CLICKED);
    mp_Reagent = NULL;
    mp_ModifiyReagentDlg->InitDialog(mp_Reagent, mp_DataConnector->ReagentGroupList, m_ReagentModel.GetRMSOption());
    mp_ModifiyReagentDlg->move(96,70);
    mp_ModifiyReagentDlg->show();
}

/****************************************************************************/
/*!
 *  \brief Deletes the currently selected staining reagent
 */
/****************************************************************************/
void CReagentsWidget::OnDelete()
{
    m_MessageDlg.SetText(tr("Staining Process has started, Editing is no longer possible."));
    m_MessageDlg.SetTitle(tr("Confirmation Message"));
    m_MessageDlg.SetIcon(QMessageBox::Information);
    m_MessageDlg.SetButtonText(1, tr("Yes"));
    m_MessageDlg.SetButtonText(3, tr("Cancel"));
    m_MessageDlg.HideCenterButton();
    m_MessageDlg.SetText(tr("Do you really want to delete the selected reagent"));

    if(m_MessageDlg.exec()==(int)QDialog::Accepted){
        emit RemoveReagent(mp_Reagent->GetReagentID());
    }
}

/****************************************************************************/
/*!
 *  \brief This slot is called when Cancel button in ModifyReagentDlg is pressed.
 */
/****************************************************************************/
void CReagentsWidget::OnCancelPressed()
{
    mp_TableWidget->clearSelection();
    ResetButtons();
}

/****************************************************************************/
/*!
 *  \brief Sets the settings data object for this class
 *
 *  \iparam p_UserSettings = Global user settings data
 */
/****************************************************************************/
void CReagentsWidget::SetUserSettings(DataManager::CUserSettings *p_UserSettings)
{
    mp_UserSettings = p_UserSettings;
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
//void CReagentsWidget::RetranslateUI()
//{
//   MainMenu::CPanelFrame::SetPanelTitle(QApplication::translate("Reagents::CReagentsWidget", "Reagents", 0, QApplication::UnicodeUTF8));
//}

/****************************************************************************/
/*!
 *  \brief Called when the selection of the table is changed
 *
 *  \iparam Index = Model Index
 */
/****************************************************************************/
void CReagentsWidget::SelectionChanged(QModelIndex Index)
{
    QString Id = m_ReagentModel.data(Index, (int)Qt::UserRole).toString();
    // change the background color when the row is selected for the reagent group column

    mp_Reagent = mp_ReagentList->GetReagent(Id);
    if (mp_Reagent) {
        bool  IsLeicaReagent = mp_Reagent->IsLeicaReagent();
        if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
             m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
                (!m_ProcessRunning)) {
            //Edit Mode
            mp_Ui->btnNew->setEnabled(true);
            mp_Ui->btnEdit->setEnabled(true);
            mp_Ui->btnDelete->setEnabled(true);

        }
        else {
            mp_Ui->btnNew->setEnabled(false);
            mp_Ui->btnDelete->setEnabled(false);
        }

        if (mp_DataConnector->ProgramList->GetReagentIDList().contains(Id))
        {
            mp_Ui->btnDelete->setEnabled(false);
        }
    }
}
/****************************************************************************/
/*!
 *  \brief This slot is called when User Role changes
 */
/****************************************************************************/
void CReagentsWidget::OnUserRoleChanged()
{
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
         m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
            (!m_ProcessRunning)) {
        //Edit Mode
        mp_Ui->btnNew->setEnabled(true);
    }
    else {
        mp_Ui->btnNew->setEnabled(false);
    }
    m_UserRoleChanged = true;
}

/****************************************************************************/
/*!
 *  \brief This slot is called when Process state changes
 */
/****************************************************************************/
void CReagentsWidget::OnProcessStateChanged()
{
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
         m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
            (!m_ProcessRunning)) {
        //Edit Mode
        bool m_IsLeicaReagent = m_SelectedReagent.IsLeicaReagent();
        if(m_IsLeicaReagent) {
            mp_Ui->btnNew->setEnabled(true);
            mp_Ui->btnEdit->setEnabled(false);
            mp_Ui->btnDelete->setEnabled(false);
        }
        else {
            mp_Ui->btnNew->setEnabled(true);
            mp_Ui->btnEdit->setEnabled(true);
            mp_Ui->btnDelete->setEnabled(true);
        }
    }
    else {
        mp_Ui->btnNew->setEnabled(false);
        mp_Ui->btnEdit->setEnabled(false);
        mp_Ui->btnDelete->setEnabled(false);

        if(m_ProcessRunning && m_ShowMessageDialog){
            m_MessageDlg.SetTitle(tr("Information Message"));
            m_MessageDlg.SetIcon(QMessageBox::Information);
            m_MessageDlg.SetButtonText(1, tr("Ok"));
            m_MessageDlg.HideButtons();
            (void)m_MessageDlg.exec();
        }
    }
}

/****************************************************************************/
/*!
 *  \brief This resets the buttons on the right of the screen
 */
/****************************************************************************/
void CReagentsWidget::ResetButtons()
{
    if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
         m_CurrentUserRole == MainMenu::CMainWindow::Service) && (!m_ProcessRunning)) {
        //Edit Mode
        mp_Ui->btnNew->setEnabled(true);
        mp_Ui->btnEdit->setEnabled(false);
        mp_Ui->btnDelete->setEnabled(false);
    }
    else {
        //View Mode
        mp_Ui->btnEdit->setEnabled(false);
        mp_Ui->btnDelete->setEnabled(false);
        mp_Ui->btnNew->setEnabled(false);
    }
}

/*****************************************************************************/
/*!
  *  \brief Translates the strings in UI to the selected language
  */
/****************************************************************************/
void CReagentsWidget::RetranslateUI()
{
    MainMenu::CPanelFrame::SetPanelTitle(QApplication::translate("Reagents::CReagentsWidget",
                                                                 "Reagents", 0, QApplication::UnicodeUTF8));
    (void) m_ReagentModel.setHeaderData(0,Qt::Horizontal,QApplication::translate("Core::CReagentModel",
                                                                                 "Reagent", 0, QApplication::UnicodeUTF8),0);
    (void) m_ReagentModel.setHeaderData(1,Qt::Horizontal,QApplication::translate("Core::CReagentModel",
                                                                                 "ReagentGroup", 0, QApplication::UnicodeUTF8),0);


    QString SecondColumnName("");
    switch(m_ReagentModel.GetRMSOption()) {
        case Global::RMS_CASSETTES:
            SecondColumnName = "Cassettes until change";
            break;
        case Global::RMS_CYCLES:
            SecondColumnName = "Cycles until change";
            break;
        case Global::RMS_DAYS:
            SecondColumnName = "Days until change";
            break;
    }
    (void) m_ReagentModel.setHeaderData(2,Qt::Horizontal,QApplication::translate("Core::CReagentModel",
                                                                                 SecondColumnName.toUtf8(), 0, QApplication::UnicodeUTF8),0);

}

/****************************************************************************/
/*!
 *  \brief This event is called whenever widget is shown
 */
/****************************************************************************/
void CReagentsWidget::showEvent(QShowEvent *)
{
    m_MessageDlg.SetText(tr("Staining Process has started, Editing is no longer possible."));
    m_ShowMessageDialog = true ;
    mp_TableWidget->clearSelection();
    mp_Ui->scrollTable->Reset();
    ResetButtons();
}


/****************************************************************************/
/*!
 *  \brief Used to set pointer to mainwindow, used to retreive user role and
 *         process state changed.
 */
/****************************************************************************/
void CReagentsWidget::SetPtrToMainWindow(Core::CDataConnector *p_DataConnector,
                                         DataManager::CDataReagentList *p_ReagentList,
                                         MainMenu::CMainWindow *p_MainWindow,
                                         KeyBoard::CKeyBoard *p_KeyBoard)
{
    mp_MainWindow = p_MainWindow;
    mp_DataConnector = p_DataConnector;
    mp_ReagentList = p_ReagentList;
    mp_KeyBoard = p_KeyBoard;
    // set the reagent ist to the model
    m_ReagentModel.SetReagentList(mp_ReagentList, 3);
    m_ReagentModel.SetReagentGroupList(mp_DataConnector->ReagentGroupList);
    ResizeHorizontalSection();

    mp_ModifiyReagentDlg = new CModifyReagentDlg(this, mp_KeyBoard, mp_MainWindow, mp_DataConnector);
    mp_ModifiyReagentDlg->setModal(true);

    CONNECTSIGNALSLOT(mp_ModifiyReagentDlg, UpdateReagent(DataManager::CReagent &), mp_DataConnector,
                        SendReagentUpdate(DataManager::CReagent &));
    CONNECTSIGNALSLOT(mp_ModifiyReagentDlg, AddReagent(DataManager::CReagent &) ,
                        mp_DataConnector, SendReagentAdd(DataManager::CReagent &));
    CONNECTSIGNALSLOT(this, RemoveReagent(QString) ,
                      mp_DataConnector, SendReagentRemove(QString));


    CONNECTSIGNALSLOT(mp_ModifiyReagentDlg, CancelPressed(), this, OnCancelPressed());
}

/****************************************************************************/
/*!
 *  \brief This slot is called whenever widget tablecontent is scrolled
 */
/****************************************************************************/
void CReagentsWidget:: OnContentScrolled()
{

}

} // end namespace Reagents
