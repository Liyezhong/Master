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
#include "Dashboard/Include/CommonString.h"
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

    CONNECTSIGNALSLOT(this, ProgramSelected(QString&, int),
                      ui->favoriteProgramsPanel, ProgramSelected(QString&, int));

    CONNECTSIGNALSLOT(this, ProgramActionStopped(DataManager::ProgramStatusType_t),
                        ui->programRunningPanel, OnProgramActionStopped(DataManager::ProgramStatusType_t));

    CONNECTSIGNALSLOT(this, ProgramActionStarted(DataManager::ProgramActionType_t, int, const QDateTime&, bool),
                      ui->programRunningPanel, OnProgramActionStarted(DataManager::ProgramActionType_t, int, const QDateTime&, bool));

    m_btnGroup.addButton(ui->startButton, Dashboard::firstButton);
    m_btnGroup.addButton(ui->pauseButton, Dashboard::secondButton);

    CONNECTSIGNALSLOT(&m_btnGroup, buttonClicked(int), this, OnButtonClicked(int));
    CONNECTSIGNALSLOT(this, ProgramSelected(QString&, int),
                      this, OnProgramSelected(QString&, int));

    ui->startButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
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
            CommonString::RetranslateUIString();
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
    m_strNotStartRMSOFF = QApplication::translate("Dashboard::CProgramPanelWidget", "Leica Program can't be operated with RMS OFF.", 0, QApplication::UnicodeUTF8);
    m_strConfirmation = QApplication::translate("Dashboard::CProgramPanelWidget", "Confirmation Message", 0, QApplication::UnicodeUTF8);
    m_strAbortProgram = QApplication::translate("Dashboard::CProgramPanelWidget", "Do you want to abort the program?", 0, QApplication::UnicodeUTF8);

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
        mp_MessageDlg->SetTitle(CommonString::strWarning);
        mp_MessageDlg->SetText(m_strNotStartRMSOFF);
        mp_MessageDlg->SetButtonText(1, CommonString::strOK);
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

    //We should improve the expired later
    /*if (!isRMSOFF && mp_DashboardScene->HaveExpiredReagent())
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
    //m_NewSelectedProgramId = m_SelectedProgramId;
    emit PrepareSelectedProgramChecking(m_SelectedProgramId, true);
}


bool CProgramPanelWidget::CheckPreConditionsToPauseProgram()
{
    return true;
}

bool CProgramPanelWidget::CheckPreConditionsToAbortProgram()
{
    MainMenu::CMessageDlg ConfirmationMessageDlg;

    ConfirmationMessageDlg.SetTitle(m_strConfirmation);
    ConfirmationMessageDlg.SetText(m_strAbortProgram);
    ConfirmationMessageDlg.SetIcon(QMessageBox::Information);
    ConfirmationMessageDlg.SetButtonText(1, CommonString::strYes);
    ConfirmationMessageDlg.SetButtonText(3, CommonString::strCancel);
    ConfirmationMessageDlg.HideCenterButton();
    return ConfirmationMessageDlg.exec() == (int)QDialog::Accepted;
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
                if(CheckPreConditionsToAbortProgram()) {
                    mp_DataConnector->SendProgramAction(m_SelectedProgramId, DataManager::PROGRAM_ABORT);
                    m_ProgramNextAction = DataManager::PROGRAM_START;
                }
            }
            break;
        }
    }
    else if (whichBtn == Dashboard::secondButton)//pause
    {
        if(CheckPreConditionsToPauseProgram())
        {
            mp_DataConnector->SendProgramAction(m_SelectedProgramId, DataManager::PROGRAM_PAUSE);
            emit ProgramActionStopped(DataManager::PROGRAM_STATUS_PAUSED);//pause EndTime countdown
        } else {
            // Take Necessary Action
        }

    }
}

void CProgramPanelWidget::ChangeStartButtonToStopState()
{
    ui->startButton->setText(tr("Stop"));
    m_ProgramNextAction = DataManager::PROGRAM_ABORT;
}

void CProgramPanelWidget::OnProgramStartReadyUpdated()
{
    if (!m_SelectedProgramId.isEmpty())
        this->ui->startButton->setEnabled(true);

    //m_ProgramStartReady = true;
}

void CProgramPanelWidget::SetProgramNextActionAsStart()
{
    m_ProgramNextAction = DataManager::PROGRAM_START;
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

void CProgramPanelWidget::OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor & cmd)
{
    /*if (cmd.CurProgramStepIndex() > 2 && m_CurrentUserRole == MainMenu::CMainWindow::Operator)
    {
        //  EnablePlayButton(true);
    }*/
}

}
