#include "Dashboard/Include/ProgramRunningPanelWidget.h"
#include "ui_ProgramRunningPanelWidget.h"
#include <QDateTime>
#include <QTimer>
#include <HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdCurrentProgramStepInfor.h>
#include "Global/Include/Utils.h"
#include "DataManager/Containers/UserSettings/Include/UserSettings.h"
#include "MainMenu/Include/MainWindow.h"
#include "Dashboard/Include/DashboardProgramStatusWidget.h"
#include "Core/Include/Startup.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Include/DashboardStation.h"
#include "Dashboard/Include/DashboardWidget.h"
#include "Core/Include/GlobalHelper.h"

namespace Dashboard
{
CProgramRunningPanelWidget::CProgramRunningPanelWidget(QWidget *parent):
    CIconTitlePanelFrame(parent),
    ui(new Ui::CProgramRunningPanelWidget),
    m_remainingTimeTotal(0),
    m_curRemainingTimeTotal(0),
    m_strAborted(tr("Aborted.")),
    m_strCompleted(tr("Completed!")),
    mp_UserSettings(NULL),
    m_DateTimeStr(""),
    m_selectedProgramId(""),
    m_isAborting(false)
{
    qDebug()<<"CProgramRunningPanelWidget::CProgramRunningPanelWidget  m_remainingTimeTotal = "<<m_remainingTimeTotal;

    ui->setupUi(GetContentFrame());
    mp_ProgressTimer = new QTimer(this);
    mp_ProgressTimer->setInterval(1000);
    CONNECTSIGNALSLOT(mp_ProgressTimer, timeout(), this, UpdateProgress());
    CONNECTSIGNALSLOT(ui->btnProgramDetail, clicked(), this, OnProgramFetail());
}

CProgramRunningPanelWidget::~CProgramRunningPanelWidget()
{
    try {
            delete ui;
            delete mp_ProgressTimer;
        }
        catch (...) {
            // to please Lint.
        }
}

void CProgramRunningPanelWidget::OnProgramActionStopped(DataManager::ProgramStatusType_t ProgramStatusType)
{
    qDebug()<<"CProgramRunningPanelWidget::OnProgramActionStopped  ProgramStatusType="<<ProgramStatusType;

    mp_ProgressTimer->stop();//the progress bar and Time countdown will stop
    if (DataManager::PROGRAM_STATUS_PAUSED == ProgramStatusType)
    {
       return;
    }
    else if (DataManager::PROGRAM_STATUS_ABORTED == ProgramStatusType)
    {
        ui->lblReagentName->setText(m_strAborted);
        ui->lblRemainTime->setText("00:00");
    }
    else if (DataManager::PROGRAM_STATUS_COMPLETED == ProgramStatusType)
    {
        ui->lblReagentName->setText(m_strCompleted);
        ui->lblRemainTime->setText("00:00");
    }
    ui->stepTimeLabel->setVisible(false);
    ui->lblStepTime->setVisible(false);
}

void CProgramRunningPanelWidget::OnProgramActionStarted(DataManager::ProgramActionType_t ProgramActionType,
                                                     int remainingTimeTotal, const QDateTime& startDateTime, bool IsResume)
{
    qDebug()<<"CProgramRunningPanelWidget::OnProgramActionStarted  reamainigTimeTotal="<<remainingTimeTotal<<"  IsResume="<<IsResume;

   Q_UNUSED(startDateTime);

   if (!IsResume)
   {
       m_remainingTimeTotal = remainingTimeTotal;
   }

   qDebug()<<"CProgramRunningPanelWidget::OnProgramActionStarted  m_remainingTimeTotal = "<<m_remainingTimeTotal;

   mp_ProgressTimer->start();
   ui->stepTimeLabel->setVisible(true);
   ui->lblStepTime->setVisible(true);
   ui->lblReagentName->setVisible(true);
   if (DataManager::PROGRAM_ABORT == ProgramActionType)
   {
       ui->lblReagentName->setVisible(false);
       ui->reagentLabel->setText(m_strAborting);//only show the first label

       ui->lblStepTime->setVisible(false);
       ui->stepTimeLabel->setVisible(false);
        m_isAborting = true;
        m_curRemainingTimeTotal = m_remainingTimeTotal;
        ui->lblRemainTime->setText(Core::CGlobalHelper::TimeToString(m_curRemainingTimeTotal, true));
   }
}

void CProgramRunningPanelWidget::OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor & cmd)
{
    qDebug()<<"CProgramRunningPanelWidget::OnCurrentProgramStepInforUpdated  cmd="<<cmd.StepName();

    ui->lblReagentName->setText(cmd.StepName());
    QString timeStr = Core::CGlobalHelper::TimeToString(cmd.CurRemainingTime(), true);
    ui->lblStepTime->setText(timeStr);
    m_CurStepRemainingTime = m_CurRemainingTime = cmd.CurRemainingTime();
    m_CurProgramStepIndex = cmd.CurProgramStepIndex();
}


void CProgramRunningPanelWidget::UpdateProgress()
{
    if (!m_isAborting)
    {
        // update the remaining time for single step
        m_CurRemainingTime -= 1;
        ui->lblStepTime->setText(Core::CGlobalHelper::TimeToString(m_CurRemainingTime, true));

        // update the total remaining time
        int elapsed =  m_CurStepRemainingTime - m_CurRemainingTime;
        m_curRemainingTimeTotal = m_remainingTimeTotal - elapsed;
        ui->lblRemainTime->setText(Core::CGlobalHelper::TimeToString(m_curRemainingTimeTotal, false));
    }
    else
    {
        ui->lblRemainTime->setText(Core::CGlobalHelper::TimeToString(m_curRemainingTimeTotal--, true));
        if (m_curRemainingTimeTotal<0)
            m_curRemainingTimeTotal = m_remainingTimeTotal;
    }
}

void CProgramRunningPanelWidget::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            this->RetranslateUI();
            break;
        default:
            break;
    }
}

void CProgramRunningPanelWidget::RetranslateUI()
{
    m_strAborting = QApplication::translate("Dashboard::CProgramRunningPanelWidget", "Aborting...", 0, QApplication::UnicodeUTF8);
    m_strAborted = QApplication::translate("Dashboard::CProgramRunningPanelWidget", "Aborted.", 0, QApplication::UnicodeUTF8);
    m_strCompleted = QApplication::translate("Dashboard::CProgramRunningPanelWidget", "Completed!", 0, QApplication::UnicodeUTF8);
}

void CProgramRunningPanelWidget::SetUserSettings(DataManager::CUserSettings *pUserSettings)
{
    mp_UserSettings = pUserSettings;
}

void CProgramRunningPanelWidget::OnUserSettingsUpdated()
{
   if (mp_UserSettings)
   {
        m_CurDateFormat = mp_UserSettings->GetDateFormat();
        m_CurTimeFormat = mp_UserSettings->GetTimeFormat();
        UpdateDateTime(m_ProgramEndDateTime);
   }
}

void CProgramRunningPanelWidget::UpdateDateTime(const QDateTime &selDateTime)
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
    DateTimeStr.append(TimeStr);
    DateTimeStr.append("\n");
    DateTimeStr.append(DateStr);
    m_DateTimeStr = "";
    m_DateTimeStr.append(TimeStr);
    m_DateTimeStr.append(" ");
    m_DateTimeStr.append(DateStr);

    ui->lblEndtime->setText(DateTimeStr);
}

void CProgramRunningPanelWidget::ProgramSelected(QString& programId, int asapEndTime, bool bProgramStartReady, QList<QString>& selectedStationList)
{
    m_selectedProgramId = programId;
    m_selectedStationList = selectedStationList;
    Q_UNUSED(bProgramStartReady);
    m_ProgramEndDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(asapEndTime);
    UpdateDateTime(m_ProgramEndDateTime);
}

int CProgramRunningPanelWidget::GetStepRemainingTime()
{
   return m_CurRemainingTime;
}

int CProgramRunningPanelWidget::GetProgramRemainingTime()
{
   return m_curRemainingTimeTotal;
}

const QString CProgramRunningPanelWidget::GetEndDateTime()
{
   return m_DateTimeStr;
}

void CProgramRunningPanelWidget::EnableProgramDetailButton(bool bEnable)
{
    ui->btnProgramDetail->setEnabled(bEnable);
}

void CProgramRunningPanelWidget::OnProcessStateChanged()
{
   bool bProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
   if (!bProcessRunning)
    mp_ProgressTimer->stop();//the progress bar and Time countdown will stop
}

void CProgramRunningPanelWidget::GetStationNameList(QList<QString>& stationNameList)
{
    Core::CStartup* pStartup = Core::CStartup::instance();
    const DataManager::CProgram* pProgram = pStartup->DataConnector()->ProgramList ->GetProgram(m_selectedProgramId);
    if (pProgram)
    {
        for (int i = 0; i < m_selectedStationList.count(); i++)
        {
            DataManager::CDashboardStation* pStation = pStartup->DataConnector()->DashboardStationList->GetDashboardStation(m_selectedStationList.at(i));
            if (!pStation)
            {
                Q_ASSERT(0);
            }
            stationNameList.append(pStation->GetDashboardStationName());
        }
    }
}

void CProgramRunningPanelWidget::OnProgramFetail()
{
    CDashboardProgramStatusWidget *pProgramStatusWidget = new Dashboard::CDashboardProgramStatusWidget();
    pProgramStatusWidget->setWindowFlags(Qt::CustomizeWindowHint);

    Core::CStartup* pStartup = Core::CStartup::instance();
    const DataManager::CProgram* pProgram = pStartup->DataConnector()->ProgramList ->GetProgram(m_selectedProgramId);
    Q_ASSERT(pProgram);
    QList<QString> stationNameList;
    GetStationNameList(stationNameList);


    bool bAboutEnable = pStartup->Dashboard()->IsAbortEnabled();
    pProgramStatusWidget->InitDialog(const_cast<DataManager::CProgram*>(pProgram), pStartup->DataConnector(),
                                       stationNameList, m_CurProgramStepIndex,
                                        m_CurRemainingTime,
                                       m_curRemainingTimeTotal,
                                       m_DateTimeStr, bAboutEnable);

    //position the window of ProgramStatusWidget
    MainMenu::CMainWindow * pMainWin = pStartup->MainWindow();
    QRect scr = pMainWin->geometry();
    pProgramStatusWidget->move( scr.center() - pProgramStatusWidget->rect().center());
    QObject::connect(pProgramStatusWidget, SIGNAL(AbortClicked(int)), this, SIGNAL(AbortClicked(int)));
    pProgramStatusWidget->exec();
    QObject::disconnect(pProgramStatusWidget, SIGNAL(AbortClicked(int)), this, SIGNAL(AbortClicked(int)));
    delete pProgramStatusWidget;
}

}
