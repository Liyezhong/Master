/****************************************************************************/
/*! \file RecoveryActionDlg.cpp
 *
 *  \brief Recovery Action Dialog implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-07
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

#include "LogViewer/Include/RecoveryActionDlg.h"
#include "ui_RecoveryActionDlg.h"

namespace LogViewer {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CRecoveryActionDlg::CRecoveryActionDlg(QWidget *p_Parent) : MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CRecoveryActionDlg)
{
    mp_Ui->setupUi(GetContentFrame());
    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->resize(650,450);

    m_Model.setHorizontalHeaderLabels(QStringList() << "Error" << "Description" << "Recovery Action");
    mp_TableWidget->horizontalHeader()->show();

    mp_TableWidget->setModel(&m_Model);
    mp_TableWidget->horizontalHeader()->resizeSection(1, 220);
    mp_TableWidget->setWordWrap(true);
    mp_TableWidget->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    mp_TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mp_Ui->widget->setMinimumSize(mp_TableWidget->width(), mp_TableWidget->height());
    mp_Ui->widget->SetContent(mp_TableWidget);

    connect(mp_Ui->closeBtn, SIGNAL(clicked()), this, SLOT(close()));

}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CRecoveryActionDlg::~CRecoveryActionDlg()
{
    try {
        delete mp_Ui;
        delete mp_TableWidget;
    }
    catch (...) {
        // to please Lint
    }
}

/****************************************************************************/
/*!
 *  \brief  To initialize the pop up dialog
 *  \iparam FilePath = Log file path
 *  \return retruns 0, if file open gets failed.
 */
/****************************************************************************/
int CRecoveryActionDlg::InitDialog(QString FilePath)
{
    QFile File(FilePath);
    if(!File.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return 0;
    }
    QString Title;
    QTextStream Text(&File);
    while(!Text.atEnd())
    {
        QString Data = Text.readLine();
        if(Data.startsWith("Version"))
        {
            Title = Data;
        }
        if(Data.startsWith("System"))
        {
            QStringList List = Data.split(" ");
            Title.append(List.at(1));
        }
        if(Data.contains(";"))
        {
            QStringList RecoveryInfo = Data.split(";");
            AddItem(RecoveryInfo.at(0), RecoveryInfo.at(1), RecoveryInfo.at(2));
        }
    }
    SetDialogTitle(Title);
    File.close();
}

/****************************************************************************/
/*!
 *  \brief  To add data item to the table
 *  \iparam EventID = Log Event ID
 *  \iparam Description = Event description
 *  \iparam RecoveryAction = Recovery action to be taken
 */
/****************************************************************************/
void CRecoveryActionDlg::AddItem(QString EventID, QString Description, QString RecoveryAction)
{
    QList<QStandardItem *> ItemList;
    ItemList << new QStandardItem(EventID);
    ItemList << new QStandardItem(Description);
    ItemList << new QStandardItem(RecoveryAction);

    m_Model.appendRow(ItemList);
}

}   // end of namespace LogViewer

