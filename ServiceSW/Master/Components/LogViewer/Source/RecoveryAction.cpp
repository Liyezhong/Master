/****************************************************************************/
/*! \file RecoveryAction.cpp
 *
 *  \brief Recovery action help pages implementation.
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

#include "LogViewer/Include/RecoveryAction.h"
#include "ui_SystemLogViewer.h"

namespace LogViewer {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CRecoveryAction::CRecoveryAction(QWidget *p_Parent) : QWidget(p_Parent), mp_Ui(new Ui::CSystemLogViewer)
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
        if(FileName.startsWith("Recovery"))
        {
            m_TroubleShootInfoFile = fileInfo.filePath();
            AddItem(fileInfo.fileName());
        }
    }

    mp_TableWidget->setModel(&m_Model);
    mp_TableWidget->selectRow(0);

    mp_TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mp_Ui->widget->setMinimumSize(mp_TableWidget->width(), mp_TableWidget->height());
    mp_Ui->widget->SetContent(mp_TableWidget);

    mp_Ui->showLogBtn->setText(tr("Show Details"));

    mp_RecoveryActionDlg = new LogViewer::CRecoveryActionDlg(this);
    connect(mp_Ui->showLogBtn, SIGNAL(clicked()), this, SLOT(ExecDialog()));
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CRecoveryAction::~CRecoveryAction()
{
    try {
        delete mp_Ui;
        delete mp_TableWidget;
        delete mp_RecoveryActionDlg;
    }
    catch (...) {
        // to please Lint
    }
}

/****************************************************************************/
/*!
 *  \brief  To add data item to the table
 *  \iparam LogFileName = Recovery Action file name
 */
/****************************************************************************/
void CRecoveryAction::AddItem(QString LogFileName)
{
    QList<QStandardItem *> ItemList;
    ItemList << new QStandardItem(LogFileName);

    m_Model.appendRow(ItemList);
}

/****************************************************************************/
/*!
 *  \brief  Pop's up a dialog with troubleshooting information.
 */
/****************************************************************************/
void CRecoveryAction::ExecDialog()
{
    mp_RecoveryActionDlg->SetDialogTitle(tr("Recovery Action Version 0.1 ST8200"));
    mp_RecoveryActionDlg->resize(700,500);
    mp_RecoveryActionDlg->InitDialog(m_TroubleShootInfoFile);
    mp_RecoveryActionDlg->show();
}

} // end namespace LogViewer

