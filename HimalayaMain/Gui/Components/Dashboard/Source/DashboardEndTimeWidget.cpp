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
namespace Dashboard {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/

CDashboardEndTimeWidget::CDashboardEndTimeWidget(Core::CDataConnector *p_DataConnector, QWidget *p_Parent) : QWidget(p_Parent),
    mp_Ui(new Ui::CDashboardEndTimeWidget), mp_DataConnector(p_DataConnector), m_UserRoleChanged(false)

{
    mp_Ui->setupUi(this);
    mp_Program = NULL;

    m_backgroundPixmap.load(":/HimalayaImages/Icons/Dashboard/EndTime/EndTime_Background.png");
    m_btnPixmap.load(":/HimalayaImages/LongButton/LongButton_Disabled.png");
    m_progBarPixmap.load(":/HimalayaImages/Icons/Dashboard/ProgressLine/ProgressLine_Background.png");

    mp_UserSettings = new DataManager::CUserSettings();

    mp_wdgtDateTime = new Dashboard::CDashboardDateTimeWidget();
    mp_wdgtDateTime->setWindowFlags(Qt::CustomizeWindowHint);

    CONNECTSIGNALSLOT(mp_DataConnector, UserSettingsUpdated(), this, OnUserSettingsUpdated());
    CONNECTSIGNALSLOT(mp_DataConnector, CurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor &),
                      this, OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor &));

    CONNECTSIGNALSLOT(mp_Ui->btnEndTime, clicked(), this, OnEndTimeButtonClicked());
}

CDashboardEndTimeWidget::~CDashboardEndTimeWidget()
{
    try {
        delete mp_UserSettings;
        delete mp_wdgtDateTime;
        delete mp_Ui;
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
    mp_Ui->progressBar->hide();
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
    mp_Ui->lblReagentName->show();
    mp_Ui->lblStepTime->show();
    mp_Ui->lblTime->show();
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

void CDashboardEndTimeWidget::paintEvent(QPaintEvent *)
{

    QPainter        Painter(this);
    Painter.drawPixmap(rect(), m_backgroundPixmap);
    //m_Painter.fillRect(QRect(10, 140, m_progBarPixmap.width(), m_progBarPixmap.height()), QBrush(Qt::green));
}

void CDashboardEndTimeWidget::OnEndTimeButtonClicked()
{
    mp_wdgtDateTime->UpdateProgramName();
    mp_wdgtDateTime->show();
    CONNECTSIGNALSLOT(mp_wdgtDateTime, OnSelectDateTime(const QDateTime &), this, UpdateDateTime(const QDateTime &));
    CONNECTSIGNALSIGNAL(mp_wdgtDateTime, OnSelectDateTime(const QDateTime &), this, OnSelectDateTime(const QDateTime &));
    update();
}


void CDashboardEndTimeWidget::UpdateDateTime(const QDateTime &selDateTime)
{
    QString DateStr;
    QString TimeStr;

    switch(m_CurDateFormat) {
        case Global::DATE_INTERNATIONAL:
        {
            DateStr = QString(tr("%1").arg(selDateTime.date().toString("dd.MM.yyyy")));
        }
        break;
        case Global::DATE_ISO:
        {
            DateStr = QString(tr("%1").arg(selDateTime.date().toString("yyyy-MM-dd")));
        }
        break;
        case Global::DATE_US:
        {
            DateStr = QString(tr("%1").arg(selDateTime.date().toString("MM/dd/yyyy")));
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
            TimeStr = QString(tr("%1").arg(selDateTime.time().toString("hh:mm:ss p.m")));
        }
            break;
        case Global::TIME_24:
        {
            TimeStr = QString(tr("%1").arg(selDateTime.time().toString("hh:mm:ss")));
        }
            break;
        case Global::TIME_UNDEFINED:
        default:
        {
            qDebug() << " Time format is Invalid";
        }
    }

    QString DateTimeStr;
    DateTimeStr.append(tr("End Time :"));
    DateTimeStr.append(TimeStr);
    DateTimeStr.append("\n");
    DateTimeStr.append("\t\t\t");
    DateTimeStr.append(DateStr);

    mp_Ui->btnEndTime->setText(DateTimeStr);


}

 void CDashboardEndTimeWidget::OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor& cmd)
 {
     mp_Ui->lblName->setText(cmd.StepName());
     mp_Ui->lblTime->setText(cmd.CurRemainingTime().toString("hh:mm:ss"));
 }

}    // end of namespace Dashboard
