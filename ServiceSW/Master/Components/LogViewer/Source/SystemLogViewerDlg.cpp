/****************************************************************************/
/*! \file SystemLogViewerDlg.cpp
 *
 *  \brief System Log Viewer implementation.
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

#include "Global/Include/SystemPaths.h"
#include "LogViewer/Include/SystemLogViewerDlg.h"
#include "ui_SystemLogViewerDlg.h"
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
CSystemLogViewerDlg::CSystemLogViewerDlg(QWidget *p_Parent) : MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CSystemLogViewerDlg)
{
    mp_Ui->setupUi(GetContentFrame());
    m_LogFilePath = "";
    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->resize(600,350);

    m_Model.setHorizontalHeaderLabels(QStringList() << "Date" << "TimeStamp" << "Event ID" << "Type" << "Description");
    mp_TableWidget->horizontalHeader()->show();

    mp_TableWidget->setModel(&m_Model);
    mp_TableWidget->horizontalHeader()->resizeSection(0, 95);
    mp_TableWidget->horizontalHeader()->resizeSection(3, 75);
//    mp_TableWidget->setWordWrap(true);
//    mp_TableWidget->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    mp_TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mp_Ui->widget->setMinimumSize(mp_TableWidget->width(), mp_TableWidget->height());
    mp_Ui->widget->SetContent(mp_TableWidget);

    mp_RecoveryActionDlg = new MainMenu::CTextDialog(this);
    mp_RecoveryActionDlg->SetCaption(tr(""));
    mp_Ui->allBtn->setChecked(true);    

    mp_MessageDlg = new MainMenu::CMessageDlg(this);

    connect(mp_Ui->allBtn, SIGNAL(clicked()), this, SLOT(CompleteLogInfo()));
    connect(mp_Ui->errorBtn, SIGNAL(clicked()), this, SLOT(FilteredErrorLog()));
    connect(mp_Ui->infoBtn, SIGNAL(clicked()), this, SLOT(FilteredInfoLog()));
    connect(mp_Ui->undefinedBtn, SIGNAL(clicked()), this, SLOT(FilteredUndefinedLog()));
    connect(mp_Ui->warningBtn, SIGNAL(clicked()), this, SLOT(FilteredWarningLog()));
    connect(mp_Ui->infoErrorBtn, SIGNAL(clicked()), this, SLOT(FilteredInfoErrorLog()));

    connect(mp_TableWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(SelectionChanged(QModelIndex)));
    connect(mp_Ui->showDetailsBtn, SIGNAL(clicked()), this, SLOT(ShowRecoveryActionDetails()));
    connect(mp_Ui->recoveryActionBtn, SIGNAL(clicked()), this, SLOT(RecoveryActionDialog()));
    connect(mp_Ui->closeBtn, SIGNAL(clicked()), this, SLOT(close()));

}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CSystemLogViewerDlg::~CSystemLogViewerDlg()
{
    try {
        delete mp_Ui;
        delete mp_TableWidget;
        delete mp_RecoveryActionDlg;
        delete mp_MessageDlg;
        m_SelectedRowValues.clear();
    }
    catch (...) {
        // to please Lint
    }
}

/****************************************************************************/
/*!
 *  \brief To get the index of selected row from the table
 *  \iparam Index = Index of the row
 */
/****************************************************************************/
void CSystemLogViewerDlg::SelectionChanged(QModelIndex Index)
{
    QItemSelectionModel* SelectionModel = mp_TableWidget->selectionModel();
    m_SelectedRowValues = SelectionModel->selectedIndexes(); //!< list of "selected" items
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
void CSystemLogViewerDlg::AddItem(QString Date, QString TimeStamp, QString EventID, QString Type, QString Desc)
{
    QList<QStandardItem *> ItemList;
    ItemList << new QStandardItem(Date);
    ItemList << new QStandardItem(TimeStamp);
    ItemList << new QStandardItem(EventID);
    ItemList << new QStandardItem(Type);
    ItemList << new QStandardItem(Desc);

    m_Model.appendRow(ItemList);
}

/****************************************************************************/
/*!
 *  \brief  To initialize the pop up dialog
 *  \return retruns 0, if file open gets failed.
 */
/****************************************************************************/
int CSystemLogViewerDlg::RecoveryActionDialog()
{
    if(m_SelectedRowValues.count() == 0)
    {
        mp_MessageDlg->SetTitle(tr("Waring"));
        mp_MessageDlg->SetButtonText(1, tr("OK"));
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(tr("Please Select a row to continue.."));
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        mp_MessageDlg->show();
    }
    else
    {
        QString Type = m_SelectedRowValues.at(3).data((int)Qt::DisplayRole).toString();
        if ( Type == QString("Info") || Type == QString("Undefined") || Type == QString("Warning")) {
            mp_MessageDlg->SetTitle(tr("Recovery Action"));
            mp_MessageDlg->SetButtonText(1, tr("OK"));
            mp_MessageDlg->HideButtons();
            QString Text = "Recovery Action is not applicable for the type " + Type + ".";
            mp_MessageDlg->SetText(Text);
            mp_MessageDlg->SetIcon(QMessageBox::Warning);
            mp_MessageDlg->show();
        }
        else {

            QString Path = Global::SystemPaths::Instance().GetLogfilesPath() + "/Recovery_Action.txt";

            QFile File(Path);
            if(!File.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                return 0;
            }

            QTextStream Text(&File);
            while(!Text.atEnd())
            {
                QString Description = m_SelectedRowValues.at(4).data((int)Qt::DisplayRole).toString();
                QString DescriptionInFile = Text.readLine();
                if(DescriptionInFile.contains(Description))
                {
                    QStringList List = DescriptionInFile.split(";");
                    QString Text = List.at(0) + "\n\n";
                    Text.append(List.at(1));
                    Text.append("\n\n");
                    Text.append(List.at(2));
                    mp_RecoveryActionDlg->SetDialogTitle(tr("Recovery Action"));
                    mp_RecoveryActionDlg->resize(428, 428);
                    mp_RecoveryActionDlg->SetText(Text);
                    mp_RecoveryActionDlg->show();
                }
            }
            File.close();
        }
    }
}

/****************************************************************************/
/*!
 *  \brief  To initialize the pop up dialog
 *  \iparam Path = Log file path
 *  \return retruns 0, if file open gets failed.
 */
/****************************************************************************/
int CSystemLogViewerDlg::InitDialog(QString Path)
{
    m_LogFilePath = Path;
    QFile File(Path);
    if(!File.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return 0;
    }

    QTextStream Text(&File);
    while(!Text.atEnd())
    {
        QString LogData = Text.readLine();
        if(LogData.contains(";"))
        {
            QStringList List = LogData.split(";");
            QStringList DateTime = List.at(0).split(" ");
            SetLogInformation(DateTime.at(0), DateTime.at(1), List.at(1), List.at(2), List.at(3));
            AddItem(DateTime.at(0), DateTime.at(1), List.at(1), List.at(2), List.at(3));
        }
    }
    File.close();
}

/****************************************************************************/
/*!
 *  \brief  To display selected EventID information
 */
/****************************************************************************/
void CSystemLogViewerDlg::ShowRecoveryActionDetails()
{
    if(m_SelectedRowValues.count() == 0)
    {
        mp_MessageDlg->SetTitle(tr("Waring"));
        mp_MessageDlg->SetButtonText(1, tr("OK"));
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(tr("Please Select a row to continue.."));
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        mp_MessageDlg->show();
    }
    else
    {
        QString Text;
        for(int i=0; i<m_SelectedRowValues.count(); i++)
        {
            Text.append(m_SelectedRowValues.at(i).data((int)Qt::DisplayRole).toString());
            Text.append("\n\n");
        }
        mp_RecoveryActionDlg->SetDialogTitle(tr("Log Information"));
        mp_RecoveryActionDlg->SetText(Text);
        mp_RecoveryActionDlg->resize(428,428);
        mp_RecoveryActionDlg->show();
    }
}

/****************************************************************************/
/*!
 *  \brief  To set the Log information
 *  \iparam Date = Date of the log event
 *  \iparam TimeStamp = Time stamp for log event
 *  \iparam EventID = Event Id
 *  \iparam Type = Log event type
 *  \iparam Desc = Log event description
 */
/****************************************************************************/
void CSystemLogViewerDlg::SetLogInformation(QString Date, QString TimeStamp, QString EventID, QString Type, QString Desc)
{
    LogInfo_t* LogInfo = new LogInfo_t;
    LogInfo->Date = Date;
    LogInfo->TimeStamp = TimeStamp;
    LogInfo->EventID = EventID;
    LogInfo->Type = Type;
    LogInfo->Desc = Desc;

    m_LogInformation.append(LogInfo);
}

/****************************************************************************/
/*!
 *  \brief To display complete log information
 */
/****************************************************************************/
void CSystemLogViewerDlg::CompleteLogInfo()
{
    mp_Ui->recoveryActionBtn->setEnabled(true);
    m_Model.clear();
    m_Model.setHorizontalHeaderLabels(QStringList() << "Date" << "TimeStamp" << "Event ID" << "Type" << "Description");
    InitDialog(m_LogFilePath);
}

/****************************************************************************/
/*!
 *  \brief To display filtered log information for event type Error
 */
/****************************************************************************/
void CSystemLogViewerDlg::FilteredErrorLog()
{
    mp_Ui->recoveryActionBtn->setEnabled(true);
    m_Model.clear();
    m_Model.setHorizontalHeaderLabels(QStringList() << "Date" << "TimeStamp" << "Event ID" << "Type" << "Description");
    for(int i=0;i < m_LogInformation.count(); i++)
    {
        LogInfo_t* LogInfo = new LogInfo_t;
        LogInfo = m_LogInformation.at(i);
        if(LogInfo->Type == QString("Error"))
        {
            AddItem(LogInfo->Date, LogInfo->TimeStamp, LogInfo->EventID, LogInfo->Type, LogInfo->Desc);
        }
    }
}

/****************************************************************************/
/*!
 *  \brief To display filtered log information for event type Info
 */
/****************************************************************************/
void CSystemLogViewerDlg::FilteredInfoLog()
{
    mp_Ui->recoveryActionBtn->setEnabled(false);
    m_Model.clear();
    m_Model.setHorizontalHeaderLabels(QStringList() << "Date" << "TimeStamp" << "Event ID" << "Type" << "Description");
    for(int i=0;i < m_LogInformation.count(); i++)
    {
        LogInfo_t* LogInfo = new LogInfo_t;
        LogInfo = m_LogInformation.at(i);
        if(LogInfo->Type == QString("Info"))
        {
            AddItem(LogInfo->Date, LogInfo->TimeStamp, LogInfo->EventID, LogInfo->Type, LogInfo->Desc);
        }
    }
}

/****************************************************************************/
/*!
 *  \brief To display filtered log information for event type Undefined
 */
/****************************************************************************/
void CSystemLogViewerDlg::FilteredUndefinedLog()
{
    mp_Ui->recoveryActionBtn->setEnabled(false);
    m_Model.clear();
    m_Model.setHorizontalHeaderLabels(QStringList() << "Date" << "TimeStamp" << "Event ID" << "Type" << "Description");
    for(int i=0;i < m_LogInformation.count(); i++)
    {
        LogInfo_t* LogInfo = new LogInfo_t;
        LogInfo = m_LogInformation.at(i);
        if(LogInfo->Type == QString("Undefined"))
        {
            AddItem(LogInfo->Date, LogInfo->TimeStamp, LogInfo->EventID, LogInfo->Type, LogInfo->Desc);
        }
    }
}

/****************************************************************************/
/*!
 *  \brief To display filtered log information for event type Warning
 */
/****************************************************************************/
void CSystemLogViewerDlg::FilteredWarningLog()
{
    mp_Ui->recoveryActionBtn->setEnabled(false);
    m_Model.clear();
    m_Model.setHorizontalHeaderLabels(QStringList() << "Date" << "TimeStamp" << "Event ID" << "Type" << "Description");
    for(int i=0;i < m_LogInformation.count(); i++)
    {
        LogInfo_t* LogInfo = new LogInfo_t;
        LogInfo = m_LogInformation.at(i);
        if(LogInfo->Type == QString("Warning"))
        {
            AddItem(LogInfo->Date, LogInfo->TimeStamp, LogInfo->EventID, LogInfo->Type, LogInfo->Desc);
        }
    }
}

/****************************************************************************/
/*!
 *  \brief To display filtered log information for event type Info and Error
 */
/****************************************************************************/
void CSystemLogViewerDlg::FilteredInfoErrorLog()
{
    mp_Ui->recoveryActionBtn->setEnabled(true);
    m_Model.clear();
    m_Model.setHorizontalHeaderLabels(QStringList() << "Date" << "TimeStamp" << "Event ID" << "Type" << "Description");
    for(int i=0;i < m_LogInformation.count(); i++)
    {
        LogInfo_t* LogInfo = new LogInfo_t;
        LogInfo = m_LogInformation.at(i);
        if(LogInfo->Type == QString("Info") || LogInfo->Type == QString("Error"))
        {
            AddItem(LogInfo->Date, LogInfo->TimeStamp, LogInfo->EventID, LogInfo->Type, LogInfo->Desc);
        }
    }
}

}   // end of namespace LogViewer
