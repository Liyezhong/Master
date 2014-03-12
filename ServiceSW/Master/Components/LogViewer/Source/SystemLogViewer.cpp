/****************************************************************************/
/*! \file SystemLogViewer.cpp
 *
 *  \brief System Log Viewer implementation.
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

#include "LogViewer/Include/SystemLogViewer.h"
#include "ui_SystemLogViewer.h"
#include "Global/Include/SystemPaths.h"
#include <QDebug>
namespace LogViewer {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CSystemLogViewer::CSystemLogViewer(QWidget *p_Parent) : QWidget(p_Parent), mp_Ui(new Ui::CSystemLogViewer)
{
    mp_Ui->setupUi(this);
    m_CurrentIndex = 0;
    m_LogFileName =  "";
    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->resize(380, 380);

    QDir LogDir(Global::SystemPaths::Instance().GetLogfilesPath());
    LogDir.setFilter(QDir::Files);

    QFileInfoList List = LogDir.entryInfoList();
    for (int i = 0; i < List.size(); i++)
    {
        QFileInfo fileInfo = List.at(i);
        QString FileName = fileInfo.fileName();
        if(FileName.startsWith("HimalayaEvents"))
        {
            AddItem(fileInfo.fileName());
        }
    }

    mp_TableWidget->setModel(&m_Model);    

    mp_TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mp_Ui->widget->setMinimumSize(mp_TableWidget->width(), mp_TableWidget->height());
    mp_Ui->widget->SetContent(mp_TableWidget);       

    mp_SystemLogViewerDlg = new LogViewer::CSystemLogViewerDlg(this);
    mp_MessageDlg = new MainMenu::CMessageDlg(this);

    connect(mp_TableWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(SelectionChanged(QModelIndex)));
    connect(mp_Ui->showLogBtn, SIGNAL(clicked()), this, SLOT(ExecDialog()));
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CSystemLogViewer::~CSystemLogViewer()
{
    try {
        delete mp_Ui;
        delete mp_TableWidget;
        delete mp_SystemLogViewerDlg;
        delete mp_MessageDlg;
    }
    catch (...) {
        // to please Lint
    }
}

/****************************************************************************/
/*!
 *  \brief  To add data item to the table
 *  \iparam LogFileName = System log file name
 */
/****************************************************************************/
void CSystemLogViewer::AddItem(QString LogFileName)
{
    QList<QStandardItem *> ItemList;
    ItemList << new QStandardItem(LogFileName);

    m_Model.appendRow(ItemList);
}

/****************************************************************************/
/*!
 *  \brief To get the index of selected row from the table
 *  \iparam Index = Index of the row
 */
/****************************************************************************/
void CSystemLogViewer::SelectionChanged(QModelIndex Index)
{
    m_CurrentIndex = Index.row();
    m_LogFileName = Index.data((int)Qt::DisplayRole);
}

/****************************************************************************/
/*!
 *  \brief  Pop's up a dialog for service log.
 */
/****************************************************************************/
void CSystemLogViewer::ExecDialog()
{
    if(m_LogFileName.toString().isEmpty())
    {
        mp_MessageDlg->SetTitle(tr("Waring"));
        mp_MessageDlg->SetButtonText(1, tr("OK"));
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(tr("Please Select a log file to continue.."));
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        mp_MessageDlg->show();
    }
    else
    {
        QString Path = Global::SystemPaths::Instance().GetLogfilesPath() + "/" + m_LogFileName.toString();

        mp_SystemLogViewerDlg->SetDialogTitle(m_LogFileName.toString()); //tr("Himalaya Events"));
        mp_SystemLogViewerDlg->resize(720,500);
        mp_SystemLogViewerDlg->InitDialog(Path);
        mp_SystemLogViewerDlg->show();
    }
}

} // end namespace LogViewer
