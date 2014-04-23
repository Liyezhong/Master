/****************************************************************************/
/*! \file LogContentDlg.cpp
 *
 *  \brief Log content dialog implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-04-14
 *   $Author:  $ Sunny Qu
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

#include "Global/Include/SystemPaths.h"
#include "LogViewerDialog/Include/LogContentDlg.h"
#include "LogViewerDialog/Include/LogFilter.h"
#include "ui_LogContentDlg.h"
#include <QTextStream>
#include <QDebug>

namespace LogViewer {


CLogContentDlg::CLogContentDlg(const QStringList& HeaderLabels, QList<int> &Columns, QWidget *p_Parent) :
    MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CLogContentDlg),
    mp_Model(NULL),
    mp_LogFilter(NULL)
{
    m_HeaderLabels = HeaderLabels;
    m_Columns = Columns;

    mp_Ui->setupUi(GetContentFrame());
    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->resize(600,400);

    mp_TableWidget->setWordWrap(true);

    mp_TableWidget->horizontalHeader()->show();
    mp_TableWidget->resizeColumnsToContents();
    mp_TableWidget->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    mp_TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mp_TableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    mp_Ui->widget->setMinimumSize(mp_TableWidget->width(), mp_TableWidget->height());
    mp_Ui->widget->SetContent(mp_TableWidget);

    (void)connect(mp_Ui->closeBtn, SIGNAL(clicked()), this, SLOT(close()));

}

CLogContentDlg::~CLogContentDlg()
{
    try {
        delete mp_Ui;
        delete mp_TableWidget;
    }
    catch (...) {
        // to please Lint
    }
}

int CLogContentDlg::InitDialog(QString FilePath)
{
    mp_LogFilter = new CLogFilter(FilePath, m_Columns);
    if (mp_LogFilter->InitData() == false )
    {
        return false;
    }
    mp_Model = mp_LogFilter->GetItemModel();
    mp_Model->setHorizontalHeaderLabels(m_HeaderLabels);

    mp_TableWidget->setModel(mp_Model);

    if (FilePath.contains("RecoveryAction")) {
        QMap<QString, QString> FileInfo = mp_LogFilter->GetFileInfo();
        QString Title ;
        qDebug()<<"FileInfo:"<<FileInfo;

        Title.append("Recovery Action");
        Title.append(" Version:");
        Title.append(FileInfo.value("Version"));
        Title.append(" ");
        Title.append(FileInfo.value("System"));

        this->SetDialogTitle(Title);
        mp_TableWidget->horizontalHeader()->resizeSection(0, 95);
        mp_TableWidget->horizontalHeader()->resizeSection(1, 200);
        mp_TableWidget->horizontalHeader()->resizeSection(2, 200);

    }

    return true;
}
}   // end of namespace LogViewer

