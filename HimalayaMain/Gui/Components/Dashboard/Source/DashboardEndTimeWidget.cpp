/****************************************************************************/
/*! \file DashboardEndTimeWidget.cpp
 *
 *  \brief DashboardComboBox implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 19-Mar-2013
 *   $Author:  $ Santhosha Kumar Raju
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2012 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "ui_DashboardEndTimeWidget.h"
#include <QPainter>
#include "Dashboard/Include/DashboardEndTimeWidget.h"
#include <QPlastiqueStyle>

namespace Dashboard {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/

CDashboardEndTimeWidget::CDashboardEndTimeWidget(Core::CDataConnector *p_DataConnector,
                                                 MainMenu::CMainWindow *p_MainWindow,
                                                 QWidget *p_Parent) :
                                                    QWidget(p_Parent),
                                                    mp_Ui(new Ui::CDashboardEndTimeWidget),
                                                    mp_DataConnector(p_DataConnector),
                                                    m_UserRoleChanged(false),
                                                    m_remainingTimeTotal(0),
                                                    m_curRemainingTimeTotal(0),
                                                    m_DateTimeStr(""),
                                                    mp_MainWindow(p_MainWindow),
                                                    m_ProcessRunning(false),
                                                    m_strEndTime(tr("End Time :")),
                                                    m_strAborting(tr("Aborting...")),
                                                    m_strAborted(tr("Aborted.")),
                                                    m_strCompleted(tr("Completed!")),
                                                    m_strReagentName(tr("Reagent Name :"))

{
    mp_Ui->setupUi(this);
    mp_Program = NULL;

    m_backgroundPixmap.load(":/HimalayaImages/Icons/Dashboard/EndTime/EndTime_Background.png");
    m_btnPixmap.load(":/HimalayaImages/LongButton/LongButton_Disabled.png");
    m_progBarPixmap.load(":/HimalayaImages/Icons/Dashboard/ProgressLine/ProgressLine_Background.png");

    mp_UserSettings = new DataManager::CUserSettings();

    mp_wdgtDateTime = new Dashboard::CDashboardDateTimeWidget();
    mp_wdgtDateTime->setWindowFlags(Qt::CustomizeWindowHint);
    mp_PlastiqueStyle = new QPlastiqueStyle();

    mp_ProgressTimer = new QTimer(this);
    mp_ProgressTimer->setInterval(1000);
    CONNECTSIGNALSLOT(mp_ProgressTimer, timeout(), this, UpdateProgress());

    CONNECTSIGNALSLOT(mp_DataConnector, UserSettingsUpdated(), this, OnUserSettingsUpdated());
    CONNECTSIGNALSLOT(mp_DataConnector, CurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor &),
                      this, OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor &));

    CONNECTSIGNALSLOT(mp_Ui->btnEndTime, clicked(), this, OnEndTimeButtonClicked());
    CONNECTSIGNALSLOT(mp_MainWindow, ProcessStateChanged(), this, OnProcessStateChanged());
}

CDashboardEndTimeWidget::~CDashboardEndTimeWidget()
{
    try {
        delete mp_UserSettings;
        delete mp_wdgtDateTime;
        delete mp_Ui;
        delete mp_PlastiqueStyle;
        delete mp_ProgressTimer;
    } catch(...) {

    }

}

void CDashboardEndTimeWidget::InitEndTimeWidgetItems()
{
    mp_Ui->btnEndTime->setText("");
    mp_Ui->btnEndTime->setDisabled(true);
    mp_Ui->lblName->hide();
    mp_Ui->lblReagentName->hide();
    mp_Ui->lblStepTime->hide();
    mp_Ui->lblTime->hide();
    mp_Ui->progressBar->setStyle(mp_PlastiqueStyle);
    mp_Ui->progressBar->setStyleSheet(mp_Ui->progressBar->property("defaultStyleSheet").toString() +
                                       "QProgressBar { border-image: url(:/HimalayaImages/Icons/Dashboard/ProgressLine/ProgressLine_Background.png);"
                                      + "background-origin: margin;}"
                                      + "QProgressBar::chunk { background-origin: margin;border-image: url(:/HimalayaImages/Icons/Dashboard/ProgressLine/ProgressLine.png); margin: 4.0px;}");

    mp_Ui->progressBar->setMinimum(0);
    mp_Ui->progressBar->hide();
    mp_ProgressTimer->stop();
}

void CDashboardEndTimeWidget::OnUserSettingsUpdated()
{
    mp_UserSettings = mp_DataConnector->SettingsInterface->GetUserSettings() ;
    m_CurDateFormat = mp_UserSettings->GetDateFormat();
    m_CurTimeFormat = mp_UserSettings->GetTimeFormat();
    if(mp_Program) {
        UpdateDateTime(m_ProgramEndDateTime);
   }
}

//Set from the proposed end time
void CDashboardEndTimeWidget::UpdateEndTimeWidgetItems(DataManager::CProgram const *p_Program, int ProgramEndTimeInSecs)
{
    mp_Program = p_Program;

    mp_Ui->btnEndTime->setStyleSheet(
                 "border-image: url(:/HimalayaImages/LongButton/LongButton_Enabled.png);"
                 "font: 75 regular 14pt \"cmr10\";"
                 "text-align: left;"
             );
    mp_Ui->btnEndTime->setEnabled(true);
    m_ProgramEndDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(ProgramEndTimeInSecs);
    UpdateDateTime(m_ProgramEndDateTime);
    mp_Ui->lblName->show();
    mp_Ui->lblTime->setText("00:00:00");
    mp_Ui->lblReagentName->setText(m_strReagentName);
    mp_Ui->lblReagentName->show();
    mp_Ui->lblStepTime->show();
    mp_Ui->lblTime->show();
    int TimeTotal = 100;
    mp_Ui->progressBar->setMaximum(TimeTotal);
    mp_Ui->progressBar->show();

}

/****************************************************************************/
/*!
 *  \brief This slot is called when User Role changes
 */
/****************************************************************************/
void CDashboardEndTimeWidget::OnUserRoleChanged()
{
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
    m_UserRoleChanged = true;
}

void CDashboardEndTimeWidget::OnProcessStateChanged()
{
   m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
   if (!m_ProcessRunning)//only for pause
    mp_ProgressTimer->stop();//the progress bar and Time countdown will stop
}

void CDashboardEndTimeWidget::paintEvent(QPaintEvent *)
{

    QPainter        Painter(this);
    Painter.drawPixmap(rect(), m_backgroundPixmap);
    //m_Painter.fillRect(QRect(10, 140, m_progBarPixmap.width(), m_progBarPixmap.height()), QBrush(Qt::green));
}

void CDashboardEndTimeWidget::OnEndTimeButtonClicked()
{
    if (!m_ProcessRunning)
    {
        mp_wdgtDateTime->UpdateProgramName();
        mp_wdgtDateTime->SetASAPDateTime(m_ProgramEndDateTime);
        mp_wdgtDateTime->show();
        CONNECTSIGNALSLOT(mp_wdgtDateTime, OnSelectDateTime(const QDateTime &), this, UpdateDateTime(const QDateTime &));
        CONNECTSIGNALSIGNAL(mp_wdgtDateTime, OnSelectDateTime(const QDateTime &), this, OnSelectDateTime(const QDateTime &));
        update();
    }
}


void CDashboardEndTimeWidget::UpdateDateTime(const QDateTime &selDateTime)
{
    QString DateStr;
    QString TimeStr;

    switch(m_CurDateFormat) {
        case Global::DATE_INTERNATIONAL:
        {
            DateStr = QString("%1").arg(selDateTime.date().toString("dd.MM.yyyy"));
        }
        break;
        case Global::DATE_ISO:
        {
            DateStr = QString("%1").arg(selDateTime.date().toString("yyyy-MM-dd"));
        }
        break;
        case Global::DATE_US:
        {
            DateStr = QString("%1").arg(selDateTime.date().toString("MM/dd/yyyy"));
        }
        break;
        case Global::DATE_UNDEFINED:
        default:
        {
            qDebug() << " Date format is Invalid";
        }
    }

    switch(m_CurTimeFormat) {
        case Global::TIME_12:
        {
            TimeStr = QString("%1").arg(selDateTime.time().toString("hh:mm:ss p.m"));
        }
            break;
        case Global::TIME_24:
        {
            TimeStr = QString("%1").arg(selDateTime.time().toString("hh:mm:ss"));
        }
            break;
        case Global::TIME_UNDEFINED:
        default:
        {
            qDebug() << " Time format is Invalid";
        }
    }

    QString DateTimeStr;
    DateTimeStr.append("  ");
    DateTimeStr.append(m_strEndTime);
    DateTimeStr.append(TimeStr);
    DateTimeStr.append("\n");
    DateTimeStr.append("\t\t\t");
    DateTimeStr.append(DateStr);
    m_DateTimeStr = "";
    m_DateTimeStr.append(TimeStr);
    m_DateTimeStr.append(" ");
    m_DateTimeStr.append(DateStr);

    mp_Ui->btnEndTime->setText(DateTimeStr);
}

void CDashboardEndTimeWidget::OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor& cmd)
{
    mp_Ui->lblName->setText(cmd.StepName());
    mp_Ui->lblTime->setText(cmd.CurRemainingTime().toString("hh:mm:ss"));
    m_CurStepRemainingTime = cmd.CurRemainingTime();
    QTime curTime = QTime::currentTime();
    m_Elapsed = curTime.secsTo(m_CurStepRemainingTime);
}

void CDashboardEndTimeWidget::UpdateProgress()
{
    //update the remaining time for single step
    QTime curTime = QTime::currentTime();
    int elapsed = curTime.secsTo(m_CurStepRemainingTime);
    int diff = elapsed - m_Elapsed;
    m_CurRemainingTime = m_CurStepRemainingTime.addSecs(diff);
    mp_Ui->lblTime->setText(m_CurRemainingTime.toString("hh:mm:ss"));

    //update progress bar
    QDateTime curDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime();
    int  elapsedTime = m_startDateTime.secsTo(curDateTime);
    m_curRemainingTimeTotal = m_remainingTimeTotal - elapsedTime;
    mp_Ui->progressBar->setValue(elapsedTime);
}

 void CDashboardEndTimeWidget::OnProgramActionStarted(DataManager::ProgramActionType_t ProgramActionType,
                                                      int remainingTimeTotal, const QDateTime& startDateTime, bool IsResume)
 {
    if (!IsResume)
    {
        m_remainingTimeTotal = remainingTimeTotal;
        mp_Ui->progressBar->setMaximum(remainingTimeTotal);
        m_startDateTime = startDateTime;
    }
    mp_ProgressTimer->start();

    if (DataManager::PROGRAM_ABORT == ProgramActionType)
    {
        mp_Ui->lblName->setVisible(false);
        mp_Ui->lblReagentName->setText(m_strAborting);//only show the first label
    }
}

 void CDashboardEndTimeWidget::OnProgramActionStopped(DataManager::ProgramStatusType_t ProgramStatusType)
 {
    mp_ProgressTimer->stop();//the progress bar and Time countdown will stop
    if (DataManager::PROGRAM_STATUS_PAUSED == ProgramStatusType)
    {
       return;
    }
    else if (DataManager::PROGRAM_STATUS_ABORTED == ProgramStatusType)
    {
        mp_Ui->lblReagentName->setText(m_strAborted);
    }
    else if (DataManager::PROGRAM_STATUS_COMPLETED == ProgramStatusType)
    {
        mp_Ui->lblReagentName->setText(m_strCompleted);
        mp_Ui->progressBar->setValue(m_remainingTimeTotal);
    }
    mp_Ui->lblTime->setVisible(false);
    mp_Ui->lblStepTime->setVisible(false);
 }

 const QTime& CDashboardEndTimeWidget::GetStepRemainingTime()
 {
    return m_CurRemainingTime;
 }

 const QTime CDashboardEndTimeWidget::GetProgramRemainingTime()
 {
     QTime leftTime(0,0,0);
     return leftTime = leftTime.addSecs(m_curRemainingTimeTotal);
 }

 const QString CDashboardEndTimeWidget::GetEndDateTime()
 {
    return m_DateTimeStr;
 }

 void CDashboardEndTimeWidget::changeEvent(QEvent *p_Event)
 {
     QWidget::changeEvent(p_Event);
     switch (p_Event->type()) {
         case QEvent::LanguageChange:
             mp_Ui->retranslateUi(this);
             this->RetranslateUI();
             break;
         default:
             break;
     }
 }

 void CDashboardEndTimeWidget::RetranslateUI()
 {
     m_strEndTime = QApplication::translate("Dashboard::CDashboardEndTimeWidget", "End Time :", 0, QApplication::UnicodeUTF8);
     m_strAborting = QApplication::translate("Dashboard::CDashboardEndTimeWidget", "Aborting...", 0, QApplication::UnicodeUTF8);
     m_strAborted = QApplication::translate("Dashboard::CDashboardEndTimeWidget", "Aborted.", 0, QApplication::UnicodeUTF8);
     m_strCompleted = QApplication::translate("Dashboard::CDashboardEndTimeWidget", "Completed!", 0, QApplication::UnicodeUTF8);
     m_strReagentName = QApplication::translate("Dashboard::CDashboardEndTimeWidget", "Reagent Name :", 0, QApplication::UnicodeUTF8);

 }

}    // end of namespace Dashboard
