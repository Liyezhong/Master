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
    m_selectedProgramId("")
{
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
   Q_UNUSED(startDateTime);
   if (!IsResume)
   {
       m_remainingTimeTotal = remainingTimeTotal;
   }
   mp_ProgressTimer->start();

   if (DataManager::PROGRAM_ABORT == ProgramActionType)
   {
       ui->reagentLabel->setVisible(false);
       ui->lblReagentName->setText(m_strAborting);//only show the first label
   }
}

void CProgramRunningPanelWidget::OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor & cmd)
{
    ui->lblReagentName->setText(cmd.StepName());
    ui->lblStepTime->setText(cmd.CurRemainingTime().toString("hh:mm:ss"));
    m_CurStepRemainingTime = m_CurRemainingTime = cmd.CurRemainingTime();
    m_CurProgramStepIndex = cmd.CurProgramStepIndex();
}

void CProgramRunningPanelWidget::UpdateProgress()
{
   //update the remaining time for single step
   m_CurRemainingTime = m_CurRemainingTime.addSecs(-1);
   ui->lblStepTime->setText(m_CurRemainingTime.toString("hh:mm:ss"));

   //update the total remaining time
   int elapsed = m_CurRemainingTime.secsTo(m_CurStepRemainingTime);
   m_curRemainingTimeTotal = m_remainingTimeTotal - elapsed;

   QTime n(0, 0, 0);
   ui->lblRemainTime->setText(n.addSecs(m_curRemainingTimeTotal).toString("hh:mm"));
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

const QTime& CProgramRunningPanelWidget::GetStepRemainingTime()
{
   return m_CurRemainingTime;
}

const QTime CProgramRunningPanelWidget::GetProgramRemainingTime()
{
    QTime leftTime(0,0,0);
    return leftTime = leftTime.addSecs(m_curRemainingTimeTotal);
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

    QTime programleftTime(0,0,0);
    programleftTime = programleftTime.addSecs(m_curRemainingTimeTotal);

    bool bAboutEnable = pStartup->Dashboard()->IsAbortEnabled();
    pProgramStatusWidget->InitDialog(const_cast<DataManager::CProgram*>(pProgram), pStartup->DataConnector(),
                                       stationNameList, m_CurProgramStepIndex,
                                        m_CurRemainingTime,
                                       programleftTime,
                                       m_DateTimeStr, bAboutEnable);

    //position the window of ProgramStatusWidget
    MainMenu::CMainWindow * pMainWin = pStartup->MainWindow();
    QRect scr = pMainWin->rect();
    pProgramStatusWidget->move( scr.center() - pProgramStatusWidget->rect().center());
    QObject::connect(pProgramStatusWidget, SIGNAL(AbortClicked(int)), this, SIGNAL(AbortClicked(int)));
    pProgramStatusWidget->exec();
    QObject::disconnect(pProgramStatusWidget, SIGNAL(AbortClicked(int)), this, SIGNAL(AbortClicked(int)));
    delete pProgramStatusWidget;
}

}
