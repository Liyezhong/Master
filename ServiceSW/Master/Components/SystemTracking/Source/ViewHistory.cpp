
/****************************************************************************/
/*! \file ViewHistory.cpp
 *
 *  \brief History for moduleList implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-01-16
 *   $Author:  $ Soumya. D
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "SystemTracking/Include/ViewHistory.h"

#include <QFileInfoList>

#include "Global/Include/SystemPaths.h"
#include "Main/Include/HimalayaServiceEventCodes.h"
#include "SystemTracking/Include/ViewHistoryDiffDlg.h"

#include "ui_ViewHistory.h"

namespace SystemTracking {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CViewHistory::CViewHistory(Core::CServiceGUIConnector *p_DataConnector, QWidget *p_Parent)
    : QWidget(p_Parent)
    , mp_Ui(new Ui::CViewHistory)
    , mp_TableWidget(new MainMenu::CBaseTable)
    , mp_Model(new QStandardItemModel)
    , mp_DataConnector(p_DataConnector)
{
    mp_Ui->setupUi(this);

    mp_TableWidget->setModel(mp_Model);

    mp_TableWidget->resize(380, 380);
    mp_TableWidget->SetVisibleRows(6);
    //mp_TableWidget->selectRow(0);
    mp_TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mp_TableWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    mp_Ui->widget->SetContent(mp_TableWidget);
    mp_Ui->widget->setMinimumSize(mp_TableWidget->width(),
                                  mp_TableWidget->height());

    mp_ViewHistoryDlg = new SystemTracking::CViewHistoryDlg(this);

    mp_MessageDialog = new MainMenu::CMessageDlg(this);
    mp_Ui->showDiffBtn->setEnabled(false);

    CONNECTSIGNALSLOTGUI(mp_TableWidget, pressed(QModelIndex), this, SelectionChanged(QModelIndex));
    CONNECTSIGNALSLOTGUI(mp_Ui->showDetailsBtn, clicked(), this, ExecDialog());
    CONNECTSIGNALSLOTGUI(mp_Ui->showDiffBtn, clicked(), this, ExecDiffDialog());
}


/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CViewHistory::~CViewHistory()
{
    try
    {
        delete mp_ViewHistoryDlg;

        delete mp_Model;
        delete mp_TableWidget;
        delete mp_MessageDialog;
        delete mp_Ui;
    }
    catch (...)
    {
        // to please Lint
    }
}

/****************************************************************************/
/*!
 *  \brief To get the index of selected row from the table
 *  \iparam Index = Index of the row
 */
/****************************************************************************/
void CViewHistory::SelectionChanged(QModelIndex Index)
{   
    QItemSelectionModel* SelectionModel = mp_TableWidget->selectionModel();
    m_SelectedRowValues = SelectionModel->selectedIndexes(); //!< list of "selected" items

    if (m_SelectedRowValues.count() == 1) {
        m_IndexofFirst = Index;
        m_IndexofLast = Index;
    }

    if (m_SelectedRowValues.count() == 2) {
        m_IndexofLast = Index;
    }

    QCoreApplication::processEvents(QEventLoop::AllEvents);
    if (m_SelectedRowValues.count() > 2) {
        mp_TableWidget->selectionModel()->clear();
        mp_TableWidget->selectionModel()->setCurrentIndex(m_IndexofFirst, QItemSelectionModel::Deselect);
        mp_TableWidget->selectionModel()->setCurrentIndex(m_IndexofLast, QItemSelectionModel::Select);
        mp_TableWidget->selectionModel()->setCurrentIndex(Index, QItemSelectionModel::Select);

        m_IndexofFirst = m_IndexofLast;
        m_IndexofLast = Index;
        m_SelectedRowValues.removeFirst();
    }

    if (m_SelectedRowValues.count() == 2) {
        mp_Ui->showDetailsBtn->setEnabled(false);
        mp_Ui->showDiffBtn->setEnabled(true);
    }
    else {
        mp_Ui->showDetailsBtn->setEnabled(true);
        mp_Ui->showDiffBtn->setEnabled(false);
    }
}

void CViewHistory::AddItem(QString TimeStamp)
{
    QList<QStandardItem *> ItemList;
    ItemList << new QStandardItem(TimeStamp);

    mp_Model->appendRow(ItemList);
}

void CViewHistory::UpdateGUI(void)
{
    mp_ModuleList = mp_DataConnector->GetModuleListContainer();
    mp_InstrumentHistoryArchive = mp_DataConnector->GetModuleListArchiveContainer();

    mp_Model->clear();
    if (mp_InstrumentHistoryArchive) {
        for (int i = 0; i < mp_InstrumentHistoryArchive->GetNumberOfModuleLists(); i++) {
            AddItem(mp_InstrumentHistoryArchive->GetModuleList(i)->GetModuleTimeStamp());
        }
    } else {
        qDebug()<<"Data container Instrument History Archive is NULL!!!";
    }

    if (mp_ModuleList != NULL) {
        AddItem(mp_ModuleList->GetModuleTimeStamp());
    }

    m_SelectedRowValues.clear();
}

/****************************************************************************/
/*!
 *  \brief  Pop's up a dialog to view Instrument History
 */
/****************************************************************************/
void CViewHistory::ExecDialog(void)
{
    if (m_SelectedRowValues.count() == 0)
    {
        mp_MessageDialog->setModal(true);
        mp_MessageDialog->SetTitle(tr("Select Module List"));
        mp_MessageDialog->SetButtonText(1, tr("OK"));
        mp_MessageDialog->HideButtons();
        mp_MessageDialog->SetText(tr("Please select a Module List.."));
        mp_MessageDialog->SetIcon(QMessageBox::Critical);
        mp_MessageDialog->show();

        return;
    }
    QString ModuleTimeStamp = m_SelectedRowValues.at(0).data((int)Qt::DisplayRole).toString();
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_VIEWHISTORY_TIMESTAMP_SHOWDETAILS,
                                               Global::tTranslatableStringList()<<ModuleTimeStamp);
    mp_ViewHistoryDlg->SetDialogTitle(tr("Module History"));
    mp_ViewHistoryDlg->setModal(true);
    mp_ViewHistoryDlg->resize(600,550);

    ServiceDataManager::CModuleDataList *ModuleList = NULL;
    if (mp_ModuleList && mp_ModuleList->GetModuleTimeStamp() == ModuleTimeStamp) {
        ModuleList = mp_ModuleList;
    }
    else if (mp_InstrumentHistoryArchive){
        ModuleList = mp_InstrumentHistoryArchive->GetModuleList(ModuleTimeStamp);
    }
    mp_ViewHistoryDlg->SetModuleList(ModuleList);
    mp_ViewHistoryDlg->UpdateGUI();

    mp_ViewHistoryDlg->show();
}

void CViewHistory::ExecDiffDialog()
{
    if (m_SelectedRowValues.count() == 0) {
        if (mp_MessageDialog != NULL) {
            delete mp_MessageDialog;
        }
        mp_MessageDialog = new MainMenu::CMessageDlg();
        mp_MessageDialog->setModal(true);
        mp_MessageDialog->SetTitle(QApplication::translate("SystemTracking::CViewHistory",
                                                           "Select Rows", 0, QApplication::UnicodeUTF8));
        mp_MessageDialog->SetButtonText(1, QApplication::translate("SystemTracking::CViewHistory",
                                                                   "Ok", 0, QApplication::UnicodeUTF8));
        mp_MessageDialog->HideButtons();
        mp_MessageDialog->SetText(QApplication::translate("SystemTracking::CViewHistory",
                                  "Please select two ModuleList timestamp.", 0, QApplication::UnicodeUTF8));
        mp_MessageDialog->SetIcon(QMessageBox::Critical);
        mp_MessageDialog->show();
    } else {

        QString TimeStampOne = m_SelectedRowValues.at(0).data((int)Qt::DisplayRole).toString();
        QString TimeStampTwo = m_SelectedRowValues.at(1).data((int)Qt::DisplayRole).toString();

        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_VIEWHISTORY_TIMESTAMP_SHOWDIFFERENCE,
                                                   Global::tTranslatableStringList() << TimeStampOne << TimeStampTwo);


        ServiceDataManager::CModuleDataList ModuleListOne, ModuleListTwo;

        if (!mp_ModuleList || !mp_InstrumentHistoryArchive) {
            qDebug()<<"SystemTracking::CViewHistory: invalid ModuleList or InstrumentHistoryArchive";
            return;
        }
        if (mp_ModuleList->GetModuleTimeStamp() == TimeStampOne) {
            ModuleListOne = *mp_ModuleList;
            (void) mp_InstrumentHistoryArchive->GetModuleList(TimeStampTwo, ModuleListTwo);
        } else if (mp_ModuleList->GetModuleTimeStamp() == TimeStampTwo) {
            (void) mp_InstrumentHistoryArchive->GetModuleList(TimeStampOne, ModuleListOne);
            ModuleListTwo = *mp_ModuleList;
        } else {
            (void) mp_InstrumentHistoryArchive->GetModuleList(TimeStampOne, ModuleListOne);
            (void) mp_InstrumentHistoryArchive->GetModuleList(TimeStampTwo, ModuleListTwo);
        }
        SystemTracking::CViewHistoryDiffDlg::Instance(this).Show(ModuleListOne, ModuleListTwo);
    }
}

} // end namespace SystemTracking
