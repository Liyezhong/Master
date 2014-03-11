/****************************************************************************/
/*! \file SoftwareUpdateLog.cpp
 *
 *  \brief Software Update Log implementation
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-07
 *   $Author:  $ Soumya.D
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

#include "LogViewer/Include/SoftwareUpdateLog.h"
#include "Global/Include/SystemPaths.h"
#include "ui_SystemLogViewer.h"
#include <QDir>

namespace LogViewer {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CSoftwareUpdateLog::CSoftwareUpdateLog(QWidget *p_Parent) : QWidget(p_Parent), mp_Ui(new Ui::CSystemLogViewer)
{
    mp_Ui->setupUi(this);
    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->resize(380, 380);

    QDir LogDir(Global::SystemPaths::Instance().GetLogfilesPath());
    LogDir.setFilter(QDir::Files);

    QFileInfoList List = LogDir.entryInfoList();
    for (int i = 0; i < List.size(); i++)
    {
        QFileInfo fileInfo = List.at(i);
        QString FileName = fileInfo.fileName();
        m_SoftwareUpateFilePath = fileInfo.filePath();
        if(FileName.startsWith("TC000411"))
        {
            AddItem(fileInfo.fileName());
        }
    }

    mp_TableWidget->setModel(&m_Model);
    mp_TableWidget->selectRow(0);

    mp_TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mp_Ui->widget->setMinimumSize(mp_TableWidget->width(), mp_TableWidget->height());
    mp_Ui->widget->SetContent(mp_TableWidget);

    mp_Ui->showLogBtn->setText(tr("Show Details"));

    mp_SoftwareUpdateDlg = new LogViewer::CSoftwareUpdateLogDlg(this);

    connect(mp_Ui->showLogBtn, SIGNAL(clicked()), this, SLOT(ExecDialog()));

}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CSoftwareUpdateLog::~CSoftwareUpdateLog()
{
    try {
        delete mp_Ui;
        delete mp_TableWidget;
        delete mp_SoftwareUpdateDlg;
    }
    catch (...) {
        // to please Lint
    }
}

/****************************************************************************/
/*!
 *  \brief  To add data item to the table
 *  \iparam LogFileName = Software update log file name
 */
/****************************************************************************/
void CSoftwareUpdateLog::AddItem(QString LogFileName)
{
    QList<QStandardItem *> ItemList;
    ItemList << new QStandardItem(LogFileName);

    m_Model.appendRow(ItemList);
}

/****************************************************************************/
/*!
 *  \brief  Pop up dialog for Software update log.
 */
/****************************************************************************/
void CSoftwareUpdateLog::ExecDialog()
{
    mp_SoftwareUpdateDlg->SetDialogTitle(tr("Service Events"));
    mp_SoftwareUpdateDlg->resize(750, 500);
    mp_SoftwareUpdateDlg->InitDialog(m_SoftwareUpateFilePath);
    mp_SoftwareUpdateDlg->show();
}

} // end namespace LogViewer

