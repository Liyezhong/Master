/****************************************************************************/
/*! \file SystemLogViewerDlg.cpp
 *
 *  \brief System Log Viewer implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-05-22
 *   $Author:  $ Sunny Qu
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include <QTextStream>
#include <QDebug>

#include "Main/Include/HimalayaServiceEventCodes.h"
#include "Global/Include/SystemPaths.h"
#include "LogViewerDialog/Include/SystemLogViewerDlg.h"
#include "ui_SystemLogViewerDlg.h"
#include "Global/Include/GlobalDefines.h"
#include "Global/Include/EventObject.h"

namespace LogViewer {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CSystemLogViewerDlg::CSystemLogViewerDlg(QWidget *p_Parent) : MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CSystemLogViewerDlg),
    mp_LogFilter(NULL)
{
    RetranslateUI();
    m_EventTypes = LogViewer::CLogFilter::m_AllTypes;

    mp_Ui->setupUi(GetContentFrame());
    m_LogFilePath = "";
    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->resize(600,330);

    mp_TableWidget->horizontalHeader()->show();

    mp_TableWidget->horizontalHeader()->resizeSection(0, 95);
    mp_TableWidget->horizontalHeader()->resizeSection(3, 75);
    mp_TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mp_Ui->widget->setMinimumSize(mp_TableWidget->width(), mp_TableWidget->height());
    mp_Ui->widget->SetContent(mp_TableWidget);

    mp_ServiceHelpTextDlg = new MainMenu::CTextDialog(this);
    mp_ServiceHelpTextDlg->SetCaption(tr(""));
    mp_Ui->allBtn->setChecked(true);
    mp_Ui->showDetailsBtn->setEnabled(false);

    mp_Ui->serviceHelpTextBtn->setEnabled(false);

    mp_MessageDlg = new MainMenu::CMessageDlg(this);

    (void)connect(mp_Ui->allBtn, SIGNAL(clicked()), this, SLOT(CompleteLogInfo()));
    (void)connect(mp_Ui->errorBtn, SIGNAL(clicked()), this, SLOT(FilteredErrorLog()));
    (void)connect(mp_Ui->infoBtn, SIGNAL(clicked()), this, SLOT(FilteredInfoLog()));
//    (void)connect(mp_Ui->undefinedBtn, SIGNAL(clicked()), this, SLOT(FilteredUndefinedLog()));
    (void)connect(mp_Ui->warningBtn, SIGNAL(clicked()), this, SLOT(FilteredWarningLog()));

    (void)connect(mp_TableWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(SelectionChanged(QModelIndex)));
    (void)connect(mp_Ui->showDetailsBtn, SIGNAL(clicked()), this, SLOT(ShowServiceHelpTextDetails()));
    (void)connect(mp_Ui->serviceHelpTextBtn, SIGNAL(clicked()), this, SLOT(ServiceHelpTextDialog()));
    (void)connect(mp_Ui->closeBtn, SIGNAL(clicked()), this, SLOT(close()));

    (void)connect(mp_Ui->errorWarningBtn, SIGNAL(clicked()), this, SLOT(FilteredErrorWarningLog()));
    (void)connect(mp_Ui->infoErrorBtn, SIGNAL(clicked()), this, SLOT(FilteredInfoErrorLog()));
    (void)connect(mp_ServiceHelpTextDlg, SIGNAL(clicked()), this, SLOT(CloseHelpTextDialog()));

    // read data from ServiceHelpText.txt to hash table
    QString path = Global::SystemPaths::Instance().GetSettingsPath() + "/ServiceHelpText.txt";
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.contains(";"))
            continue;
        bool ok;
        quint64 id = line.split(';').at(0).toULongLong(&ok, 10);
        if (ok == false)
            continue;
        line.replace(QString("\\n"), QString(" "));
        m_ServiceHelpTextHash.insert(id, line);
    }

    file.close();
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
        delete mp_ServiceHelpTextDlg;
        delete mp_MessageDlg;
        if (mp_LogFilter) {
            delete mp_LogFilter;
        }
        m_SelectedRowValues.clear();
    }
    catch (...) {
        // to please Lint
    }
}

void CSystemLogViewerDlg::SelectionChanged(QModelIndex Index)
{
    Q_UNUSED(Index);

    QItemSelectionModel* SelectionModel = mp_TableWidget->selectionModel();
    m_SelectedRowValues = SelectionModel->selectedIndexes(); //!< list of "selected" items

    QString Type = m_SelectedRowValues.at(3).data((int)Qt::DisplayRole).toString();

    if (Type.compare("Error") == 0 || Type.compare("Fatal Error") == 0) {
        quint64 id = m_SelectedRowValues.at(2).data((int)Qt::DisplayRole).toULongLong();
        mp_Ui->serviceHelpTextBtn->setEnabled(m_ServiceHelpTextHash.contains(id));
    } else {
        mp_Ui->serviceHelpTextBtn->setEnabled(false);
    }
    mp_Ui->showDetailsBtn->setEnabled(true);
}

void CSystemLogViewerDlg::ServiceHelpTextDialog()
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_LOGVIEWER_SYSTEMLOG_SERVICESERVICEHELPTEXT_FOR_ERROR);

    if(m_SelectedRowValues.count() == 0)
    {
        mp_MessageDlg->SetTitle(m_strWarning);
        mp_MessageDlg->SetButtonText(1, m_strOK);
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(m_strMsgSelectRowText);
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        mp_MessageDlg->show();
    }
    else
    {
        QString Type = m_SelectedRowValues.at(3).data((int)Qt::DisplayRole).toString();
        if ( Type == QString("Info") || Type == QString("Undefined") || Type == QString("Warning")) {
            mp_MessageDlg->SetTitle(m_strMsgServiceHelpTextTitle);
            mp_MessageDlg->SetButtonText(1, m_strOK);
            mp_MessageDlg->HideButtons();
            QString Text = m_strMsgServiceHelpTextText + Type + ".";
            mp_MessageDlg->SetText(Text);
            mp_MessageDlg->SetIcon(QMessageBox::Warning);
            mp_MessageDlg->show();
        }
        else {
            quint64 EventId = m_SelectedRowValues.at(2).data((int)Qt::DisplayRole).toULongLong();
            if (!m_ServiceHelpTextHash.contains(EventId))
                return;

            // TODO
            QString Line = m_ServiceHelpTextHash.value(EventId);
            QStringList List = Line.split(";");
            if (List.size()>0) {
                QString InputText = m_strErrorCode;
                InputText.append(m_strDescription);
                if (List.count() > 1)
                    InputText.append(List.at(1));
                InputText.append("\n\n");
                InputText.append(m_strServiceHelpText);
                if (List.count() > 2)
                    InputText.append(List.at(2));
                mp_ServiceHelpTextDlg->SetDialogTitle(m_strServiceHelpTextTitle);
                mp_ServiceHelpTextDlg->resize(428, 428);
                mp_ServiceHelpTextDlg->SetText(InputText);
                mp_ServiceHelpTextDlg->show();
            }
            else {
                mp_MessageDlg->SetTitle(m_strMsgServerHelpTitle);
                mp_MessageDlg->SetButtonText(1, m_strOK);
                mp_MessageDlg->HideButtons();
                mp_MessageDlg->SetText(m_strMsgServerHelpText);
                mp_MessageDlg->SetIcon(QMessageBox::Warning);
                mp_MessageDlg->show();

            }
        }
    }
}

void CSystemLogViewerDlg::SetTableModel()
{
    QStringList HeaderLabels;

    mp_Model = mp_LogFilter->GetItemModel(m_EventTypes);
    if (mp_Model->rowCount() == 0)
        return ;

    HeaderLabels.append(m_strDate);
    HeaderLabels.append(m_strTimeStamp);
    HeaderLabels.append(m_strEventID);
    HeaderLabels.append(m_strType);
    HeaderLabels.append(m_strDescriptonTitle);

    mp_Model->setHorizontalHeaderLabels(HeaderLabels);


    mp_TableWidget->setModel(mp_Model);

}

void CSystemLogViewerDlg::RetranslateUI()
{
    m_strOK = QApplication::translate("LogViewer::CSystemLogViewerDlg", "OK", 0, QApplication::UnicodeUTF8);
    m_strWarning = QApplication::translate("LogViewer::CSystemLogViewerDlg", "Warning", 0, QApplication::UnicodeUTF8);
    m_strMsgSelectRowText = QApplication::translate("LogViewer::CSystemLogViewerDlg", "Please Select a row to continue..",
                                                    0, QApplication::UnicodeUTF8);
    m_strMsgServiceHelpTextTitle = QApplication::translate("LogViewer::CSystemLogViewerDlg", "Service Help Text",
                                                          0, QApplication::UnicodeUTF8);
    m_strMsgServiceHelpTextText = QApplication::translate("LogViewer::CSystemLogViewerDlg",
                                                         "Service Help Text is not applicable for the type ",
                                                         0, QApplication::UnicodeUTF8);
    m_strErrorCode = QApplication::translate("LogViewer::CSystemLogViewerDlg", "Error Code: ", 0, QApplication::UnicodeUTF8);
    m_strDescription = QApplication::translate("LogViewer::CSystemLogViewerDlg", "Description: ",
                                               0, QApplication::UnicodeUTF8);
    m_strServiceHelpText = QApplication::translate("LogViewer::CSystemLogViewerDlg",
                                                      "Service Help Text Text", 0, QApplication::UnicodeUTF8);
    m_strServiceHelpTextTitle = QApplication::translate("LogViewer::CSystemLogViewerDlg",
                                                       "Service Help Text Text", 0, QApplication::UnicodeUTF8);
    m_strMsgServerHelpTitle = QApplication::translate("LogViewer::CSystemLogViewerDlg",
                                                      "Service Help Text", 0, QApplication::UnicodeUTF8);
    m_strMsgServerHelpText = QApplication::translate("LogViewer::CSystemLogViewerDlg",
                                                     "Service Help Text Text is not available.",
                                                     0, QApplication::UnicodeUTF8);
    m_strDate = QApplication::translate("LogViewer::CSystemLogViewerDlg", "Date", 0, QApplication::UnicodeUTF8);
    m_strTimeStamp = QApplication::translate("LogViewer::CSystemLogViewerDlg", "TimeStamp", 0, QApplication::UnicodeUTF8);
    m_strEventID = QApplication::translate("LogViewer::CSystemLogViewerDlg", "Event ID", 0, QApplication::UnicodeUTF8);
    m_strType = QApplication::translate("LogViewer::CSystemLogViewerDlg", "Type", 0, QApplication::UnicodeUTF8);
    m_strDescriptonTitle = QApplication::translate("LogViewer::CSystemLogViewerDlg", "Description",
                                                           0, QApplication::UnicodeUTF8);
}

bool CSystemLogViewerDlg::InitDialog(QString Path)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_LOGVIEWER_SYSTEMLOG_DETAILED_INFO);
    QList<int> Columns;
    Columns.append(0);
    Columns.append(1);
    Columns.append(2);
    Columns.append(4);

    m_LogFilePath = Path;
    mp_LogFilter = new CLogFilter(Path, Columns, true);
    if (mp_LogFilter->InitData() == false )
    {
        return false;
    }
    SetTableModel();
    return true;
}

void CSystemLogViewerDlg::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
        case QEvent::LanguageChange:
            this->RetranslateUI();
            break;
        default:
            break;
    }
}

void CSystemLogViewerDlg::ShowServiceHelpTextDetails()
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_LOGVIEWER_SERVICESERVICEHELPTEXT_DISPLAY_INFO);
    if(m_SelectedRowValues.count() == 0)
    {
        mp_MessageDlg->SetTitle(m_strWarning);
        mp_MessageDlg->SetButtonText(1, m_strOK);
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(m_strMsgSelectRowText);
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
        mp_ServiceHelpTextDlg->SetDialogTitle(m_strDescriptonTitle);
        mp_ServiceHelpTextDlg->SetText(Text);
        mp_ServiceHelpTextDlg->resize(428,428);
        mp_ServiceHelpTextDlg->show();
    }
}

void CSystemLogViewerDlg::CompleteLogInfo()
{
    mp_Ui->showDetailsBtn->setEnabled(false);
    mp_Ui->serviceHelpTextBtn->setEnabled(false);
    mp_TableWidget->clearSelection();

    m_EventTypes = LogViewer::CLogFilter::m_AllTypes;
    SetTableModel();
}

void CSystemLogViewerDlg::FilteredErrorLog()
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_LOGVIEWER_SYSTEMLOG_FILTERERING,
                                               Global::tTranslatableStringList() << "Error");
    mp_Ui->showDetailsBtn->setEnabled(false);
    mp_Ui->serviceHelpTextBtn->setEnabled(false);
    mp_TableWidget->clearSelection();

    m_EventTypes  = (1<<(int)Global::EVTTYPE_ERROR);
    m_EventTypes |= (1<<(int)Global::EVTTYPE_FATAL_ERROR);
    SetTableModel();
}

void CSystemLogViewerDlg::FilteredInfoLog()
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_LOGVIEWER_SYSTEMLOG_FILTERERING,
                                               Global::tTranslatableStringList() << "Info");
    mp_Ui->showDetailsBtn->setEnabled(false);
    mp_Ui->serviceHelpTextBtn->setEnabled(false);
    mp_TableWidget->clearSelection();

    m_EventTypes = (1<<(int)Global::EVTTYPE_INFO);
    SetTableModel();
}

void CSystemLogViewerDlg::FilteredUndefinedLog()
{
//    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_LOGVIEWER_SYSTEMLOG_FILTERERING,
//                                               Global::tTranslatableStringList() << "Undefined");
//    mp_Ui->showDetailsBtn->setEnabled(false);
//    mp_Ui->serviceHelpTextBtn->setEnabled(false);
//    mp_TableWidget->clearSelection();

//    if (mp_Ui->undefinedBtn->isChecked()) {
//        m_EventTypes |= (1<<(int)Global::EVTTYPE_UNDEFINED);
//    }
//    else {
//        m_EventTypes &= (0xFF-(1<<(int)Global::EVTTYPE_UNDEFINED));
//    }
//    if (m_EventTypes == 0) {
//        mp_TableWidget->setModel(NULL);
//    }
//    else {
//       SetTableModel();
//    }
}

void CSystemLogViewerDlg::FilteredWarningLog()
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_LOGVIEWER_SYSTEMLOG_FILTERERING,
                                               Global::tTranslatableStringList() << "Warning");
    mp_Ui->showDetailsBtn->setEnabled(false);
    mp_Ui->serviceHelpTextBtn->setEnabled(false);
    mp_TableWidget->clearSelection();

    m_EventTypes = (1<<(int)Global::EVTTYPE_WARNING);

    SetTableModel();
}

void CSystemLogViewerDlg::FilteredErrorWarningLog()
{
    mp_Ui->showDetailsBtn->setEnabled(false);
    mp_Ui->serviceHelpTextBtn->setEnabled(false);
    mp_TableWidget->clearSelection();

    m_EventTypes  = (1<<(int)Global::EVTTYPE_ERROR);
    m_EventTypes |= (1<<(int)Global::EVTTYPE_FATAL_ERROR);
    m_EventTypes |= (1<<(int)Global::EVTTYPE_WARNING);
    SetTableModel();
}

void CSystemLogViewerDlg::FilteredInfoErrorLog()
{
    mp_Ui->showDetailsBtn->setEnabled(false);
    mp_Ui->serviceHelpTextBtn->setEnabled(false);
    mp_TableWidget->clearSelection();

    m_EventTypes  = (1<<(int)Global::EVTTYPE_ERROR);
    m_EventTypes |= (1<<(int)Global::EVTTYPE_FATAL_ERROR);
    m_EventTypes |= (1<<(int)Global::EVTTYPE_INFO);
    SetTableModel();
}

}
