#include "Dashboard/Include/ProgramRunningPanelWidget.h"
#include "ui_ProgramRunningPanelWidget.h"
#include <QDateTime>


namespace Dashboard
{
CProgramRunningPanelWidget::CProgramRunningPanelWidget(QWidget *parent):
    CIconTitlePanelFrame(parent),
    ui(new Ui::CProgramRunningPanelWidget)
{
    ui->setupUi(GetContentFrame());
    this->SetPanelIcon(":/HimalayaImages/Icons/MISC/LeicaLogo.png");//get the current program icon
    SetPanelTitle(tr("Test"));//get the current program name
}

CProgramRunningPanelWidget::~CProgramRunningPanelWidget()
{
    try {
            delete ui;
        }
        catch (...) {
            // to please Lint.
        }
}

void CProgramRunningPanelWidget::OnProgramActionStopped(DataManager::ProgramStatusType_t ProgramStatusType)
{

}

void CProgramRunningPanelWidget::OnProgramActionStarted(DataManager::ProgramActionType_t ProgramActionType,
                                                     int remainingTimeTotal, const QDateTime& startDateTime, bool IsResume)
{
  /* if (!IsResume)
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
       mp_Ui->progressBar->setStyleSheet(mp_Ui->progressBar->property("defaultStyleSheet").toString() +
                                          "QProgressBar { border-image: url(:/HimalayaImages/Icons/Dashboard/ProgressLine/ProgressLine_Background.png);"
                                         + "background-origin: margin;}"
                                         + "QProgressBar::chunk { background-origin: margin;border-image: url(:/HimalayaImages/Icons/Dashboard/ProgressLine/ProgressLineRed.png); margin: 4.0px;}");

   }
   */
}

}
