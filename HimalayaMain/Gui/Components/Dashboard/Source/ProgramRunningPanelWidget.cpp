#include "Dashboard/Include/ProgramRunningPanelWidget.h"
#include "ui_ProgramRunningPanelWidget.h"
#include <QDateTime>
#include <HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdCurrentProgramStepInfor.h>
#include <QTimer>
#include "Global/Include/Utils.h"

namespace Dashboard
{
CProgramRunningPanelWidget::CProgramRunningPanelWidget(QWidget *parent):
    CIconTitlePanelFrame(parent),
    ui(new Ui::CProgramRunningPanelWidget),
    m_remainingTimeTotal(0),
    m_curRemainingTimeTotal(0),
    m_strAborted(tr("Aborted.")),
    m_strCompleted(tr("Completed!"))
{
    ui->setupUi(GetContentFrame());
    mp_ProgressTimer = new QTimer(this);
    mp_ProgressTimer->setInterval(1000);
    CONNECTSIGNALSLOT(mp_ProgressTimer, timeout(), this, UpdateProgress());
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
    //m_strEndTime = QApplication::translate("Dashboard::CDashboardEndTimeWidget", "End Time :", 0, QApplication::UnicodeUTF8);
    m_strAborting = QApplication::translate("Dashboard::CProgramRunningPanelWidget", "Aborting...", 0, QApplication::UnicodeUTF8);
    m_strAborted = QApplication::translate("Dashboard::CProgramRunningPanelWidget", "Aborted.", 0, QApplication::UnicodeUTF8);
    m_strCompleted = QApplication::translate("Dashboard::CProgramRunningPanelWidget", "Completed!", 0, QApplication::UnicodeUTF8);
    //m_strReagentName = QApplication::translate("Dashboard::CDashboardEndTimeWidget", "Reagent Name :", 0, QApplication::UnicodeUTF8);

}

void CProgramRunningPanelWidget::ProgramSelected(QString& ProgramId, int asapEndTime, bool bProgramStartReady)
{
    Q_UNUSED(ProgramId);
    Q_UNUSED(bProgramStartReady);
    m_ProgramEndDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(asapEndTime);
    //ui->lblEndtime->setText();
}

}
