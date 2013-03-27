/****************************************************************************/
/*! \file ModifyReagentStatusDlg.cpp
 *
 *  \brief ModifyReagentStatusDlg Implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-01-29
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

#include "Global/Include/Utils.h"
#include "Global/Include/Exception.h"
#include "Reagents/Include/ModifyReagentStationDlg.h"
#include "ui_ModifyReagentStatusDlg.h"
#include <QDebug>

namespace Reagents {
/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CModifyReagentStationDlg::CModifyReagentStationDlg(QWidget *p_Parent, MainMenu::CMainWindow *p_MainWindow, Core::CDataConnector *p_DataConnector) :
    MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CModifyReagentStationDlg), mp_MainWindow(p_MainWindow), mp_DataConnector(p_DataConnector)
{
    mp_Ui->setupUi(GetContentFrame());
    m_ProcessRunning = false ;

    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->setModel(&m_ReagentEditModel);

    mp_Ui->scrollTable->SetContent(mp_TableWidget);

    mp_TableWidget->SetVisibleRows(8);
    m_ReagentEditModel.SetVisibleRowCount(8);
    m_ReagentEditModel.SetRequiredContainers(mp_DataConnector->ReagentList,mp_DataConnector->ReagentGroupList,mp_DataConnector->DashboardStationList, 2);
    mp_TableWidget->horizontalHeader()->show();
    ResizeHorizontalSection();

    CONNECTSIGNALSLOT(mp_TableWidget, pressed(QModelIndex), this, SelectionChanged(QModelIndex));
    CONNECTSIGNALSLOT(mp_Ui->btnCancel, clicked(), this, OnCancel());
    CONNECTSIGNALSLOT(mp_Ui->btnOk, clicked(), this, OnOk());
    CONNECTSIGNALSLOT(mp_MainWindow, ProcessStateChanged(), this, OnProcessStateChanged());
    // Init Message dialog
    m_MessageDlg.SetTitle(tr("Information Message"));
    m_MessageDlg.SetIcon(QMessageBox::Information);
    m_MessageDlg.SetButtonText(1, tr("Ok"));
    m_MessageDlg.HideButtons();
}

/****************************************************************************/
/*!
     *  \brief Destructor
     */
/****************************************************************************/
CModifyReagentStationDlg::~CModifyReagentStationDlg()
{
    try {
        delete mp_TableWidget;
        delete mp_Ui;
    }
    catch (...) {
        // to please Lint.
    }
}

/****************************************************************************/
/*!
     *  \brief Destructor
     */
/****************************************************************************/
void CModifyReagentStationDlg::SetDashboardStation(DataManager::CDashboardStation* p_Station)
{
    m_DashboardStation = *p_Station;
    m_ReagentEditModel.SetReagentTypeParaffin(p_Station->GetDashboardParaffinBath());
    m_ReagentEditModel.UpdateReagentList();

    const DataManager::CReagent* pReagent = mp_DataConnector->ReagentList->GetReagent(m_DashboardStation.GetDashboardReagentID());
    QString ReagentID("");
    if (pReagent)
    ReagentID = mp_DataConnector->ReagentList->GetReagent(m_DashboardStation.GetDashboardReagentID())->GetReagentID();
    mp_TableWidget->selectRow(m_ReagentEditModel.GetReagentPositionOfReagent(ReagentID));

}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CModifyReagentStationDlg::changeEvent(QEvent *p_Event)
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
void CModifyReagentStationDlg::ResizeHorizontalSection()
{
    if (mp_TableWidget == NULL)
    {
        return;
    }
    mp_TableWidget->horizontalHeader()->resizeSection(0, 120);
    mp_TableWidget->horizontalHeader()->resizeSection(1, 150);

}


/****************************************************************************/
/*!
 *  \brief This slot is called when Cancel button in ModifyReagentDlg is pressed.
 */
/****************************************************************************/
void CModifyReagentStationDlg::OnCancel()
{
    emit CancelPressed();
    reject();
}

/****************************************************************************/
/*!
 *  \brief Called when the selection of the table is changed
 *
 *  \iparam Index = Model Index
 */
/****************************************************************************/
void CModifyReagentStationDlg::SelectionChanged(QModelIndex Index)
{
    QString Id = m_ReagentEditModel.data(Index, (int)Qt::UserRole).toString();
    m_DashboardStation.SetDashboardReagentID(Id);
}
/****************************************************************************/
/*!
 *  \brief Saves the changes in the dialog
 */
/****************************************************************************/
void CModifyReagentStationDlg::OnOk()
{    
    m_DashboardStation.SetDashboardReagentStatus("Empty");
    emit UpdateStationChangeReagent(m_DashboardStation.GetDashboardStationID(),
                                    m_DashboardStation.GetDashboardReagentID());
    emit UpdateStationSetAsEmpty(m_DashboardStation.GetDashboardStationID());
    accept();
}


/****************************************************************************/
/*!
 *  \brief This slot is called when Process state changes
 */
/****************************************************************************/
void CModifyReagentStationDlg::OnProcessStateChanged()
{
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if (!m_ProcessRunning) {
        //Edit Mode
        mp_Ui->btnOk->setEnabled(true);
        mp_Ui->btnCancel->setText(tr("Cancel"));
    }
    else {
        //View Mode
        mp_Ui->btnOk->setEnabled(false);
        mp_Ui->btnCancel->setText(tr("Close"));
    }
}

/*****************************************************************************/
/*!
  *  \brief Translates the strings in UI to the selected language
  */
/****************************************************************************/
void CModifyReagentStationDlg::RetranslateUI()
{
    (void)m_ReagentEditModel.setHeaderData(0,Qt::Horizontal,QApplication::translate("Core::CReagentStationEditModel",
                                                                                 "Reagent", 0, QApplication::UnicodeUTF8), 0);
    (void)m_ReagentEditModel.setHeaderData(1,Qt::Horizontal,QApplication::translate("Core::CReagentStationEditModel",
                                                                                 "Group", 0, QApplication::UnicodeUTF8),0);
}


}
