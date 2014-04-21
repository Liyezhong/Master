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

#include <QTextStream>
#include <QDebug>

#include "Global/Include/SystemPaths.h"
#include "LogViewerDialog/Include/SystemLogViewerDlg.h"
#include "ui_SystemLogViewerDlg.h"
#include "Global/Include/GlobalDefines.h"
#include "LogViewerDialog/Include/RecoveryActionFilter.h"

namespace LogViewer {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CSystemLogViewerDlg::CSystemLogViewerDlg(QWidget *p_Parent) : MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CSystemLogViewerDlg),
    mp_LogFilter(NULL),
    mp_RecoveryActionFilter(NULL)
{

    m_EventTypes = LogViewer::CLogFilter::m_AllTypes;

    mp_Ui->setupUi(GetContentFrame());
    m_LogFilePath = "";
    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->resize(600,350);

    mp_TableWidget->horizontalHeader()->show();

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
    ResetButtons(false);
    mp_Ui->showDetailsBtn->setEnabled(false);

    mp_Ui->recoveryActionBtn->setEnabled(false);

    mp_MessageDlg = new MainMenu::CMessageDlg(this);

    connect(mp_Ui->allBtn, SIGNAL(clicked()), this, SLOT(CompleteLogInfo()));
    connect(mp_Ui->errorBtn, SIGNAL(clicked()), this, SLOT(FilteredErrorLog()));
    connect(mp_Ui->infoBtn, SIGNAL(clicked()), this, SLOT(FilteredInfoLog()));
    connect(mp_Ui->undefinedBtn, SIGNAL(clicked()), this, SLOT(FilteredUndefinedLog()));
    connect(mp_Ui->warningBtn, SIGNAL(clicked()), this, SLOT(FilteredWarningLog()));

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
        if (mp_LogFilter) {
            delete mp_LogFilter;
        }
        if (mp_RecoveryActionFilter) {
            delete mp_RecoveryActionFilter;
        }
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

    QString Type = m_SelectedRowValues.at(3).data((int)Qt::DisplayRole).toString();
    if (Type.compare("Error")==0 || Type.compare("Fatal Error")==0) {
        mp_Ui->recoveryActionBtn->setEnabled(true);
    }
    else {
        mp_Ui->recoveryActionBtn->setEnabled(false);
    }
    mp_Ui->showDetailsBtn->setEnabled(true);
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
            QString Path = Global::SystemPaths::Instance().GetSettingsPath() + "/RecoveryActionText.txt";

            if (mp_RecoveryActionFilter == NULL) {
                mp_RecoveryActionFilter = new LogViewer::CRecoveryActionFilter(Path);
            }

            QString EventId = m_SelectedRowValues.at(2).data((int)Qt::DisplayRole).toString();
            QString Line = QString(mp_RecoveryActionFilter->GetRecoveryAtionText(EventId));
            QStringList List = Line.split(";");
            if (List.size()>0) {
                QString InputText = QApplication::translate("LogViewer::CSystemLogViewerDlg", "Error Code: ",
                                                            0, QApplication::UnicodeUTF8) + List.at(0) + "\n\n";
                InputText.append(QApplication::translate("LogViewer::CSystemLogViewerDlg", "Description: ",
                                                         0, QApplication::UnicodeUTF8));
                if (List.count() > 1)
                    InputText.append(List.at(1));
                InputText.append("\n\n");
                InputText.append(QApplication::translate("LogViewer::CSystemLogViewerDlg", "Recovery Action Text: ",
                                                         0, QApplication::UnicodeUTF8));
                if (List.count() > 2)
                    InputText.append(List.at(2));
                mp_RecoveryActionDlg->SetDialogTitle(QApplication::translate("LogViewer::CSystemLogViewerDlg",
                                                      "Recovery Action Text", 0, QApplication::UnicodeUTF8));
                mp_RecoveryActionDlg->resize(428, 428);
                mp_RecoveryActionDlg->SetText(InputText);
                mp_RecoveryActionDlg->show();
            }
            else {
                mp_MessageDlg->SetTitle(QApplication::translate("LogViewer::CSystemLogViewerDlg",
                                                                "Service Help Text", 0, QApplication::UnicodeUTF8));
                mp_MessageDlg->SetButtonText(1, QApplication::translate("LogViewer::CSystemLogViewerDlg",
                                                                        "Ok", 0, QApplication::UnicodeUTF8));
                mp_MessageDlg->HideButtons();
                mp_MessageDlg->SetText(QApplication::translate("LogViewer::CSystemLogViewerDlg",
                                                               "Recovery Action Text is not available.",
                                                               0, QApplication::UnicodeUTF8));
                mp_MessageDlg->SetIcon(QMessageBox::Warning);
                mp_MessageDlg->show();

            }
        }
    }
}

void CSystemLogViewerDlg::SetTableModel()
{
    QStringList HeaderLabels;


    qDebug()<<"m_EventTypes = "<<m_EventTypes;


    mp_Model = mp_LogFilter->GetItemModel(m_EventTypes);

    HeaderLabels.append(QApplication::translate("LogViewer::CSystemLogViewerDlg", "Date", 0, QApplication::UnicodeUTF8));
    HeaderLabels.append(QApplication::translate("LogViewer::CSystemLogViewerDlg", "TimeStamp", 0, QApplication::UnicodeUTF8));
    HeaderLabels.append(QApplication::translate("LogViewer::CSystemLogViewerDlg", "Event ID", 0, QApplication::UnicodeUTF8));
    HeaderLabels.append(QApplication::translate("LogViewer::CSystemLogViewerDlg", "Type", 0, QApplication::UnicodeUTF8));
    HeaderLabels.append(QApplication::translate("LogViewer::CSystemLogViewerDlg", "Description", 0, QApplication::UnicodeUTF8));

    mp_Model->setHorizontalHeaderLabels(HeaderLabels);


    mp_TableWidget->setModel(mp_Model);

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
    QList<int> Columns;
    Columns.append(0);
    Columns.append(1);
    Columns.append(2);
    Columns.append(3);

    m_LogFilePath = Path;
    mp_LogFilter = new CLogFilter(Path, Columns, true);
    if (mp_LogFilter->InitData() == false )
    {
        return false;
    }
    SetTableModel();
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
 *  \brief Reset Buttons
 *  \iparam EnableFlag = true:enable, false:disable
 */
/****************************************************************************/
void CSystemLogViewerDlg::ResetButtons(bool EnableFlag)
{
    mp_Ui->errorBtn->setEnabled(EnableFlag);
    mp_Ui->infoBtn->setEnabled(EnableFlag);
    mp_Ui->warningBtn->setEnabled(EnableFlag);
    mp_Ui->undefinedBtn->setEnabled(EnableFlag);

    mp_Ui->errorBtn->setChecked(false);
    mp_Ui->infoBtn->setChecked(false);
    mp_Ui->warningBtn->setChecked(false);
    mp_Ui->undefinedBtn->setChecked(false);
}


/****************************************************************************/
/*!
 *  \brief To display complete log information
 */
/****************************************************************************/
void CSystemLogViewerDlg::CompleteLogInfo()
{
    mp_Ui->showDetailsBtn->setEnabled(false);
    mp_Ui->recoveryActionBtn->setEnabled(false);
    mp_TableWidget->clearSelection();

    if (mp_Ui->allBtn->isChecked()) {
        ResetButtons(false);

        m_EventTypes = LogViewer::CLogFilter::m_AllTypes;
        SetTableModel();
    }
    else {
        ResetButtons(true);
        mp_TableWidget->setModel(NULL);
        m_EventTypes = 0;
    }

}

/****************************************************************************/
/*!
 *  \brief To display filtered log information for event type Error
 */
/****************************************************************************/
void CSystemLogViewerDlg::FilteredErrorLog()
{
    mp_Ui->showDetailsBtn->setEnabled(false);
    mp_Ui->recoveryActionBtn->setEnabled(false);
    mp_TableWidget->clearSelection();

    mp_TableWidget->clearSelection();
    if (mp_Ui->errorBtn->isChecked()) {
        m_EventTypes |= (1<<Global::EVTTYPE_ERROR);
        m_EventTypes |= (1<<Global::EVTTYPE_FATAL_ERROR);
    }
    else {
        m_EventTypes &= (0xFF-(1<<Global::EVTTYPE_ERROR));
        m_EventTypes &= (0xFF-(1<<Global::EVTTYPE_FATAL_ERROR));
    }
    if (m_EventTypes == 0) {
        mp_TableWidget->setModel(NULL);
    }
    else {
       SetTableModel();
    }
}

/****************************************************************************/
/*!
 *  \brief To display filtered log information for event type Info
 */
/****************************************************************************/
void CSystemLogViewerDlg::FilteredInfoLog()
{
    mp_Ui->showDetailsBtn->setEnabled(false);
    mp_Ui->recoveryActionBtn->setEnabled(false);
    mp_TableWidget->clearSelection();

    if (mp_Ui->infoBtn->isChecked()) {
        m_EventTypes |= (1<<Global::EVTTYPE_INFO);
    }
    else {
        m_EventTypes &= (0xFF-(1<<Global::EVTTYPE_INFO));
    }
    if (m_EventTypes == 0) {
        mp_TableWidget->setModel(NULL);
    }
    else {
       SetTableModel();
    }
}

/****************************************************************************/
/*!
 *  \brief To display filtered log information for event type Undefined
 */
/****************************************************************************/
void CSystemLogViewerDlg::FilteredUndefinedLog()
{
    mp_Ui->showDetailsBtn->setEnabled(false);
    mp_Ui->recoveryActionBtn->setEnabled(false);
    mp_TableWidget->clearSelection();

    if (mp_Ui->undefinedBtn->isChecked()) {
        m_EventTypes |= (1<<Global::EVTTYPE_UNDEFINED);
    }
    else {
        m_EventTypes &= (0xFF-(1<<Global::EVTTYPE_UNDEFINED));
    }
    if (m_EventTypes == 0) {
        mp_TableWidget->setModel(NULL);
    }
    else {
       SetTableModel();
    }
}
/****************************************************************************/
/*!
 *  \brief To display filtered log information for event type Warning
 */
/****************************************************************************/
void CSystemLogViewerDlg::FilteredWarningLog()
{
    mp_Ui->showDetailsBtn->setEnabled(false);
    mp_Ui->recoveryActionBtn->setEnabled(false);
    mp_TableWidget->clearSelection();

    if (mp_Ui->warningBtn->isChecked()) {
        m_EventTypes |= (1<<Global::EVTTYPE_WARNING);
    }
    else {
        m_EventTypes &= (0xFF-(1<<Global::EVTTYPE_WARNING));
    }
    if (m_EventTypes == 0) {
        mp_TableWidget->setModel(NULL);
    }
    else {
       SetTableModel();
    }
}
}
