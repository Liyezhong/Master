#include "Dashboard/Include/ProgramPanelWidget.h"
#include "ui_ProgramPanelWidget.h"
#include "Global/Include/Utils.h"
#include "MainMenu/Include/MainWindow.h"
#include "Dashboard/Include/DashboardDataTypes.h"
#include "HimalayaDataContainer/Containers/UserSettings/Include/HimalayaUserSettings.h"
#include "Core/Include/DataConnector.h"
#include "MainMenu/Include/MessageDlg.h"
#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramList.h"
#include "Dashboard/Include/CassetteNumberInputWidget.h"

namespace Dashboard {
CProgramPanelWidget::CProgramPanelWidget(QWidget *parent) :
    CPanelFrame(parent),
    ui(new Ui::CProgramPanelWidget),
    m_ProgramNextAction(DataManager::PROGRAM_START),
    m_IsResumeRun(false),
    m_SelectedProgramId(""),
    mp_DataConnector(NULL),
    m_pUserSetting(NULL),
    mp_ProgramList(NULL)
{
    ui->setupUi(GetContentFrame());
    SetPanelTitle(tr("Programs"));
    CONNECTSIGNALSLOT(this, AddItemsToFavoritePanel(bool), ui->favoriteProgramsPanel, AddItemsToFavoritePanel(bool));
    CONNECTSIGNALSIGNAL(ui->favoriteProgramsPanel, PrepareSelectedProgramChecking(const QString&), this, PrepareSelectedProgramChecking(const QString&));

    CONNECTSIGNALSIGNAL(ui->favoriteProgramsPanel, OnSelectEndDateTime(const QDateTime&), this, OnSelectEndDateTime(const QDateTime &));
    //CONNECTSIGNALSLOT(ui->favoriteProgramsPanel, OnSelectEndDateTime(const QDateTime&), this, OnSelectEndDateTime(const QDateTime &));

    CONNECTSIGNALSLOT(this, ProgramSelected(QString&, int),
                      ui->favoriteProgramsPanel, ProgramSelected(QString&, int));

    CONNECTSIGNALSLOT(&m_btnGroup, buttonClicked(int), this, OnButtonClicked(int));

    m_btnGroup.addButton(ui->startButton, Dashboard::firstButton);
    m_btnGroup.addButton(ui->pauseButton, Dashboard::secondButton);

    CONNECTSIGNALSLOT(this, ProgramSelected(QString&, int),
                      this, OnProgramSelected(QString&, int));

    mp_MessageDlg = new MainMenu::CMessageDlg(this);

}

CProgramPanelWidget::~CProgramPanelWidget()
{
    try {
            delete ui;
            delete mp_MessageDlg;
        }
        catch (...) {
            // to please Lint.
        }
}

void CProgramPanelWidget::changeEvent(QEvent *p_Event)
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

void CProgramPanelWidget::RetranslateUI()
{
    SetPanelTitle(QApplication::translate("Dashboard::CProgramPanelWidget", "Programs",
                                                                 0, QApplication::UnicodeUTF8));
    m_strOK = QApplication::translate("Dashboard::CProgramPanelWidget", "OK", 0, QApplication::UnicodeUTF8);
    m_strWarning = QApplication::translate("Dashboard::CProgramPanelWidget", "Warning", 0, QApplication::UnicodeUTF8);
    m_strNotStartRMSOFF = QApplication::translate("Dashboard::CProgramPanelWidget", "Leica Program can't be operated with RMS OFF.", 0, QApplication::UnicodeUTF8);


}

void CProgramPanelWidget::SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow, Core::CDataConnector *p_DataConnector)
{
    ui->favoriteProgramsPanel->SetPtrToMainWindow(p_MainWindow, p_DataConnector);
    mp_DataConnector = p_DataConnector;
    m_pUserSetting = mp_DataConnector->SettingsInterface->GetUserSettings();
    mp_ProgramList = mp_DataConnector->ProgramList;
}

void CProgramPanelWidget::OnProgramSelected(QString& ProgramId, int asapEndTime)
{
    m_SelectedProgramId = ProgramId;
    m_EndDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(asapEndTime);
}

void CProgramPanelWidget::SelectEndDateTime(const QDateTime & dateTime)
{
    m_EndDateTime = dateTime;
}

void CProgramPanelWidget::CheckPreConditionsToRunProgram()
{

    if ("" == m_SelectedProgramId)
        return;

    //Check if Leica program and RMS OFF?
    DataManager::CHimalayaUserSettings* userSetting = mp_DataConnector->SettingsInterface->GetUserSettings();
    bool bShowRMSOffWarning = false;
    bool isLeicaProgram = mp_ProgramList->GetProgram(m_SelectedProgramId)->IsLeicaProgram();
    if (m_SelectedProgramId.at(0) == 'C')
    {
        if ((Global::RMS_OFF == userSetting->GetModeRMSCleaning()) && isLeicaProgram)
        {
            bShowRMSOffWarning = true;
        }
    }
    else
    {
        if ((Global::RMS_OFF == userSetting->GetModeRMSProcessing()) && isLeicaProgram)
        {
            bShowRMSOffWarning = true;
        }
    }

    if (bShowRMSOffWarning)
    {
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        mp_MessageDlg->SetTitle(m_strWarning);
        mp_MessageDlg->SetText(m_strNotStartRMSOFF);
        mp_MessageDlg->SetButtonText(1, m_strOK);
        mp_MessageDlg->HideButtons();
        if (mp_MessageDlg->exec())
        return;
    }

    //Check Expired?
    bool isRMSOFF = false;
    if ((Global::RMS_OFF == userSetting->GetModeRMSCleaning()) || (Global::RMS_OFF == userSetting->GetModeRMSProcessing()))
    {
        isRMSOFF = true;
    }

    /*Improve the expired.
     *if (!isRMSOFF && mp_DashboardScene->HaveExpiredReagent())
    {
        if (m_CurrentUserRole == MainMenu::CMainWindow::Operator)
        {
            mp_MessageDlg->SetIcon(QMessageBox::Warning);
            mp_MessageDlg->SetTitle(m_strWarning);
            mp_MessageDlg->SetText(m_strNotStartExpiredReagent);
            mp_MessageDlg->SetButtonText(1, m_strOK);
            mp_MessageDlg->HideButtons();
            if (mp_MessageDlg->exec())
            return;
        }
        else
        if(m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
        m_CurrentUserRole == MainMenu::CMainWindow::Service)
        {
            mp_MessageDlg->SetIcon(QMessageBox::Warning);
            mp_MessageDlg->SetTitle(m_strWarning);
            mp_MessageDlg->SetText(m_strStartExpiredReagent);
            mp_MessageDlg->SetButtonText(3, m_strNo);
            mp_MessageDlg->SetButtonText(1, m_strYes);
            mp_MessageDlg->HideCenterButton();    // Hiding First Two Buttons in the Message Dialog

            if (!mp_MessageDlg->exec())
            return;
        }
    }*/


    //check End Datetime again
    /*m_NewSelectedProgramId = m_SelectedProgramId;
    m_CheckEndDatetimeAgain = true;
    PrepareSelectedProgramChecking();

    //set endtime of program
    mp_wdgtDateTime->UpdateProgramName();
    mp_wdgtDateTime->SetASAPDateTime(m_EndDateTime);
    mp_wdgtDateTime->setFixedSize(625,483);
    scr.translate(mp_MainWindow->rect().center() - mp_wdgtDateTime->rect().center());
    mp_wdgtDateTime->move(scr.left(), scr.top());
    if (!mp_wdgtDateTime->exec())
        return;

    QString strTempProgramId(m_SelectedProgramId);
    if (m_SelectedProgramId.at(0) == 'C')
    {
        strTempProgramId.append("_");
        QString reagentIDOfLastStep = m_pUserSetting->GetReagentIdOfLastStep();
        strTempProgramId.append(reagentIDOfLastStep);
    }

    mp_DataConnector->SendProgramAction(strTempProgramId, DataManager::PROGRAM_START, m_EndDateTime);
    m_ProgramNextAction = DataManager::PROGRAM_PAUSE;*/
}

void CProgramPanelWidget::OnButtonClicked(int whichBtn)
{
    if ( whichBtn == Dashboard::firstButton ) {
        switch(m_ProgramNextAction)
        {
            default:
            break;
            case DataManager::PROGRAM_START:
            {
                if (m_IsResumeRun)
                {
                    QString strTempProgramId;
                    if (m_SelectedProgramId.at(0) == 'C')//Cleaning program
                    {
                        strTempProgramId = m_SelectedProgramId;
                        strTempProgramId.append("_");
                        QString strReagentIDOfLastStep = m_pUserSetting->GetReagentIdOfLastStep();
                        strTempProgramId.append(strReagentIDOfLastStep);
                    }

                    mp_DataConnector->SendProgramAction(strTempProgramId, DataManager::PROGRAM_START, m_EndDateTime);
                    m_ProgramNextAction = DataManager::PROGRAM_ABORT;
                    return;
                }

                CheckPreConditionsToRunProgram();
            }
            break;
            case DataManager::PROGRAM_ABORT:
            {
                /*if(CheckPreConditionsToPauseProgram())
                {
                    mp_DataConnector->SendProgramAction(m_SelectedProgramId, DataManager::PROGRAM_ABORT);
                    emit ProgramActionStopped(DataManager::PROGRAM_STATUS_ABORTED);//pause ProgressBar and EndTime countdown
                    m_ProgramNextAction = DataManager::PROGRAM_START;
                } else {
                    // Take Necessary Action
                }*/
            }
            break;
        }
    }
    else if (whichBtn == Dashboard::secondButton)//pause
    {
        /*if(CheckPreConditionsToAbortProgram()) {
            mp_DataConnector->SendProgramAction(m_SelectedProgramId, DataManager::PROGRAM_PAUSE);
        }*/
    }
}

void CProgramPanelWidget::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->programRunningPanel->SetPanelTitle(tr("Test"));
}

void CProgramPanelWidget::on_pushButton_2_clicked()
{
   ui->stackedWidget->setCurrentIndex(0);
}

}
