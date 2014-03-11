/****************************************************************************/
/*! \file SoftwareUpdateLogDlg.cpp
 *
 *  \brief Software Update Log Viewer dialog implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-20
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

#include "Global/Include/SystemPaths.h"
#include "LogViewer/Include/SoftwareUpdateLogDlg.h"
#include "ui_SoftwareUpdateLogDlg.h"
#include <QTextStream>
#include <QDebug>

namespace LogViewer {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CSoftwareUpdateLogDlg::CSoftwareUpdateLogDlg(QWidget *p_Parent) : MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CSoftwareUpdateLogDlg)
{
    mp_Ui->setupUi(GetContentFrame());
    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->resize(600,450);

    m_Model.setHorizontalHeaderLabels(QStringList() << "Date" << "TimeStamp" << "Event ID" << "Type" << "" << "" << "");
    mp_TableWidget->horizontalHeader()->show();

    mp_TableWidget->setModel(&m_Model);
    mp_TableWidget->horizontalHeader()->resizeSection(0, 95);
    mp_TableWidget->horizontalHeader()->resizeSection(3, 50);
    mp_TableWidget->horizontalHeader()->resizeSection(4, 60);
    mp_TableWidget->horizontalHeader()->resizeSection(5, 60);
    mp_TableWidget->setWordWrap(true);
    mp_TableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
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
CSoftwareUpdateLogDlg::~CSoftwareUpdateLogDlg()
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
 *  \brief  To add data item to the table
 *  \iparam Date = Date of the log event
 *  \iparam TimeStamp = Time stamp for log event
 *  \iparam EventID = Event Id
 *  \iparam Type = Log event type
 *  \iparam Desc = Log event description
 */
/****************************************************************************/
void CSoftwareUpdateLogDlg::AddItem(QString Date, QString TimeStamp, QString EventID, QString Type, QString Desc,QString Data, QString Info)
{
    QList<QStandardItem *> ItemList;
    ItemList << new QStandardItem(Date);
    ItemList << new QStandardItem(TimeStamp);
    ItemList << new QStandardItem(EventID);
    ItemList << new QStandardItem(Type);
    ItemList << new QStandardItem(Desc);
    ItemList << new QStandardItem(Data);
    ItemList << new QStandardItem(Info);

    m_Model.appendRow(ItemList);
}

/****************************************************************************/
/*!
 *  \brief  To initialize the pop up dialog
 *  \iparam FilePath = Log file path
 *  \return retruns 0, if file open gets failed.
 */
/****************************************************************************/
int CSoftwareUpdateLogDlg::InitDialog(QString FilePath)
{
    QFile File(FilePath);
    if(!File.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return 0;
    }

    QTextStream Text(&File);
    while(!Text.atEnd())
    {
        QString LogData = Text.readLine();
        if(LogData.contains(", ,"))
        {                            
            QStringList List = LogData.split(", ,");

            QStringList DateTime = List.at(0).split(",");
            QStringList Message = List.at(1).split(",");
            AddItem(DateTime.at(0), DateTime.at(1), Message.at(0), Message.at(1), Message.at(2), Message.at(3), Message.at(4));
        }
    }
    File.close();
}

}   // end of namespace LogViewer

