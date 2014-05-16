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
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "SystemTracking/Include/ViewHistory.h"

#include <QFileInfoList>

#include "Global/Include/SystemPaths.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "ui_ViewHistory.h"

namespace SystemTracking {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CViewHistory::CViewHistory(QWidget *p_Parent)
    : QWidget(p_Parent)
    , mp_Ui(new Ui::CViewHistory)
    , mp_TableWidget(new MainMenu::CBaseTable)
    , mp_Model(new QStandardItemModel)
{
    mp_Ui->setupUi(this);

    mp_TableWidget->setModel(mp_Model);

    mp_TableWidget->resize(380, 380);
    mp_TableWidget->SetVisibleRows(6);
    mp_TableWidget->selectRow(0);
    mp_TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mp_Ui->widget->SetContent(mp_TableWidget);
    mp_Ui->widget->setMinimumSize(mp_TableWidget->width(),
                                  mp_TableWidget->height());

    mp_ViewHistoryDlg = new SystemTracking::CViewHistoryDlg(this);

    mp_MessageDialog = new MainMenu::CMessageDlg(this);

    (void)connect(mp_TableWidget,
                  SIGNAL(clicked(QModelIndex)),
                  this,
                  SLOT(SelectionChanged(QModelIndex)));

    (void)connect(mp_Ui->showDetailsBtn,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ExecDialog()));
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
    m_InstrumentHistoryName = Index.data(Qt::UserRole + 1).toString();
}

/****************************************************************************/
/*!
 *  \brief  To add data item to the table
 *  \iparam FileName = XML file name
 *  \iparam FilePath = XML file path
 */
/****************************************************************************/
void CViewHistory::AddItem(const QString &FileName, const QString &FilePath)
{
    QStandardItem *Item = new QStandardItem;
    Item->setText(FileName);
    Item->setData(FilePath);

    QList<QStandardItem *> ItemList;
    ItemList << Item;

    mp_Model->appendRow(ItemList);
}

void CViewHistory::UpdateGUI(void)
{
    mp_Model->clear();

    QDir LogDir(Global::SystemPaths::Instance().GetLogfilesPath());
    LogDir.setFilter(QDir::Files);

    QFileInfoList List = LogDir.entryInfoList();
    for (QFileInfoList::iterator itr = List.begin();
         List.end() != itr;
         ++itr)
    {
        QString FileName = itr->baseName();
        QString FilePath = itr->filePath();
        if(FileName.startsWith("ModuleList"))
        {
            AddItem(FileName, FilePath);
        }
    }
}

/****************************************************************************/
/*!
 *  \brief  Pop's up a dialog to view Instrument History
 */
/****************************************************************************/
void CViewHistory::ExecDialog(void)
{
    if(m_InstrumentHistoryName.isEmpty())
    {
        mp_MessageDialog->SetTitle(tr("Select Module List"));
        mp_MessageDialog->SetButtonText(1, tr("OK"));
        mp_MessageDialog->HideButtons();
        mp_MessageDialog->SetText(tr("Please select a Module List.."));
        mp_MessageDialog->SetIcon(QMessageBox::Critical);
        mp_MessageDialog->show();

        return;
    }

    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_VIEWHISTORY_INSTRUMENT_SHOWDETAILS,
                                               Global::tTranslatableStringList()<<m_InstrumentHistoryName);
    mp_ViewHistoryDlg->SetDialogTitle(tr("Module History"));
    mp_ViewHistoryDlg->resize(600,550);

    mp_ViewHistoryDlg->SetModuleListPath(m_InstrumentHistoryName);
    mp_ViewHistoryDlg->UpdateGUI();

    mp_ViewHistoryDlg->show();
}

} // end namespace SystemTracking
