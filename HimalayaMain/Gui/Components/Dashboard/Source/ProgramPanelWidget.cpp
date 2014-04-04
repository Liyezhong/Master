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
#include "HimalayaDataContainer/Containers/DashboardStations/Include/DashboardStation.h"
#include "HimalayaDataContainer/Helper/Include/Global.h"

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

    CONNECTSIGNALSLOT(this, ProgramSelected(QString&, int, bool, QList<QString>&),
                      ui->favoriteProgramsPanel, ProgramSelected(QString&, int, bool));

    CONNECTSIGNALSLOT(this, ProgramSelected(QString&, int, bool, QList<QString>&),
                      ui->programRunningPanel, ProgramSelected(QString&, int, bool, QList<QString>&));

    CONNECTSIGNALSLOT(this, ProgramSelected(QString&, int, bool, QList<QString>&),
                      this, OnProgramSelected(QString&, int, bool, QList<QString>&));

    CONNECTSIGNALSLOT(this, UndoProgramSelection(),
                      ui->favoriteProgramsPanel, UndoProgramSelection());

    CONNECTSIGNALSLOT(this, ProgramActionStopped(DataManager::ProgramStatusType_t),
                        ui->programRunningPanel, OnProgramActionStopped(DataManager::ProgramStatusType_t));

    CONNECTSIGNALSLOT(this, ProgramActionStopped(DataManager::ProgramStatusType_t),
                        this, OnProgramActionStopped(DataManager::ProgramStatusType_t));


    CONNECTSIGNALSLOT(this, ProgramActionStarted(DataManager::ProgramActionType_t, int, const QDateTime&, bool),
                      ui->programRunningPanel, OnProgramActionStarted(DataManager::ProgramActionType_t, int, const QDateTime&, bool));

    CONNECTSIGNALSLOT(this, ProgramActionStarted(DataManager::ProgramActionType_t, int, const QDateTime&, bool),
                      this, OnProgramActionStarted(DataManager::ProgramActionType_t, int, const QDateTime&, bool));

    CONNECTSIGNALSLOT(this, CurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor &),
                      ui->programRunningPanel, OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor &));

    m_btnGroup.addButton(ui->startButton, Dashboard::firstButton);
    m_btnGroup.addButton(ui->pauseButton, Dashboard::secondButton);

    CONNECTSIGNALSLOT(&m_btnGroup, buttonClicked(int), this, OnButtonClicked(int));
    ui->startButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
    mp_MessageDlg = new MainMenu::CMessageDlg(this);

    CONNECTSIGNALSLOT(ui->programRunningPanel, AbortClicked(int), this, OnButtonClicked(int));

    CONNECTSIGNALSLOT(this, UpdateProgram(DataManager::CProgram&), ui->favoriteProgramsPanel, UpdateProgram(DataManager::CProgram&));
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

    m_strYes = QApplication::translate("Dashboard::CProgramPanelWidget", "Yes", 0, QApplication::UnicodeUTF8);
    m_strNo = QApplication::translate("Dashboard::CProgramPanelWidget", "No", 0, QApplication::UnicodeUTF8);
    m_strOK = QApplication::translate("Dashboard::CProgramPanelWidget", "OK", 0, QApplication::UnicodeUTF8);
    m_strCancel = QApplication::translate("Dashboard::CProgramPanelWidget", "Cancel", 0, QApplication::UnicodeUTF8);
    m_strWarning = QApplication::translate("Dashboard::CProgramPanelWidget", "Warning", 0, QApplication::UnicodeUTF8);
    m_strProgram = QApplication::translate("Dashboard::CProgramPanelWidget", "Program", 0, QApplication::UnicodeUTF8);
    m_strInformation = QApplication::translate("Dashboard::CProgramPanelWidget", "Information", 0, QApplication::UnicodeUTF8);
    m_strNotStartExpiredReagent = QApplication::translate("Dashboard::CProgramPanelWidget", "Reagents needed for this program are expired! You can't operate this program.", 0, QApplication::UnicodeUTF8);
    m_strStartExpiredReagent =  QApplication::translate("Dashboard::CProgramPanelWidget", "Do you want to Start the Program with Expired Reagents?", 0, QApplication::UnicodeUTF8);
    m_strNeedMeltParaffin  = QApplication::translate("Dashboard::CProgramPanelWidget", "Still it will cost some time to melt paraffin, the current selected program can not run now.", 0, QApplication::UnicodeUTF8);

}

void CProgramPanelWidget::SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow, Core::CDataConnector *p_DataConnector)
{
    ui->favoriteProgramsPanel->SetPtrToMainWindow(p_MainWindow, p_DataConnector);
    mp_DataConnector = p_DataConnector;
    m_pUserSetting = mp_DataConnector->SettingsInterface->GetUserSettings();
    mp_ProgramList = mp_DataConnector->ProgramList;

    if (!mp_DataConnector)
        return;
    ui->programRunningPanel->SetUserSettings(m_pUserSetting);
    CONNECTSIGNALSLOT(mp_DataConnector, UserSettingsUpdated(), ui->programRunningPanel, OnUserSettingsUpdated());
    CONNECTSIGNALSLOT(p_MainWindow, ProcessStateChanged(), ui->programRunningPanel, OnProcessStateChanged());

}

void CProgramPanelWidget::OnProgramSelected(QString& ProgramId, int asapEndTime, bool bProgramStartReady, QList<QString>& selectedStationList)
{
    m_SelectedProgramId = ProgramId;
    m_EndDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(asapEndTime);
    if (bProgramStartReady)
        OnProgramStartReadyUpdated();
    m_StationList.clear();
    m_StationList = selectedStationList;
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

    DataManager::ReagentStatusType_t reagentStatus = DataManager::REAGENT_STATUS_NORMAL;

    for(int i = 0; i<m_StationList.size(); i++)
    {
        QString stationID = m_StationList.at(i);
        DataManager::CDashboardStation *p_Station = mp_DataConnector->DashboardStationList->GetDashboardStation(stationID);
        QString reagentID = p_Station->GetDashboardReagentID();
        DataManager::CReagent const *p_Reagent = mp_DataConnector->ReagentList->GetReagent(reagentID);

        DataManager::CReagentGroup const *p_ReagentGroup = mp_DataConnector->ReagentGroupList->GetReagentGroup(p_Reagent->GetGroupID());
        bool isClearingGroup = p_ReagentGroup->IsCleaningReagentGroup();
        Global::RMSOptions_t RMSOption = Global::RMS_OFF;
        if ( isClearingGroup )
            RMSOption = userSetting->GetModeRMSCleaning();
        else
            RMSOption = userSetting->GetModeRMSProcessing();

        reagentStatus = p_Station->GetReagentStatus(*p_Reagent, RMSOption);

        if ( reagentStatus == DataManager::REAGENT_STATUS_EXPIRED )
            break;
    }

    if ( reagentStatus == DataManager::REAGENT_STATUS_EXPIRED ) {
        MainMenu::CMainWindow::UserRole_t userRole = MainMenu::CMainWindow::GetCurrentUserRole();
        if (userRole == MainMenu::CMainWindow::Operator)
        {
            mp_MessageDlg->SetIcon(QMessageBox::Warning);
            mp_MessageDlg->SetTitle(m_strWarning);
            mp_MessageDlg->SetText(m_strNotStartExpiredReagent);
            mp_MessageDlg->SetButtonText(1, m_strOK);
            mp_MessageDlg->HideButtons();
            if (mp_MessageDlg->exec())
                return;
        }
        else if(userRole == MainMenu::CMainWindow::Admin ||
            userRole == MainMenu::CMainWindow::Service)
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
    }



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
    qDebug()<<"CProgramPanelWidget::OnButtonClicked btn = "<<whichBtn;

    if ( whichBtn == Dashboard::firstButton ) {
        switch(m_ProgramNextAction)
        {
            ui->startButton->setEnabled(false);//protect to click twice in a short time
            default:
            break;
            case DataManager::PROGRAM_START:
            {
                qDebug()<<"CProgramPanelWidget::OnButtonClicked btn = DataManager::PROGRAM_START";

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
                    ChangeStartButtonToStopState();
                    return;
                }

                CheckPreConditionsToRunProgram();
            }
            break;
            case DataManager::PROGRAM_ABORT:
            {
                qDebug()<<"CProgramPanelWidget::OnButtonClicked btn = DataManager::PROGRAM_ABORT";

                if(CheckPreConditionsToAbortProgram()) {
                    ui->pauseButton->setEnabled(false);
                    qDebug()<<"CProgramPanelWidget::OnButtonClicked send command DataManager::PROGRAM_ABORT  to master";

                    mp_DataConnector->SendProgramAction(m_SelectedProgramId, DataManager::PROGRAM_ABORT);
                    m_ProgramNextAction = DataManager::PROGRAM_START;

                }
            }
            break;
        }
    }
    else if (whichBtn == Dashboard::secondButton)//pause
    {
        ui->pauseButton->setEnabled(false);//protect to click twice in a short time
        ui->startButton->setEnabled(false);
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
    ui->startButton->setText(tr("Abort"));
    ui->startButton->IsStartStatus(false);
    m_ProgramNextAction = DataManager::PROGRAM_ABORT;
}

void CProgramPanelWidget::ChangeStartButtonToStartState()
{
    ui->startButton->setText(tr("Start"));
    ui->startButton->IsStartStatus(true);
    m_ProgramNextAction = DataManager::PROGRAM_START;
}

void CProgramPanelWidget::EnableStartButton(bool bEnable)
{
    ui->startButton->setEnabled(bEnable);
}

void CProgramPanelWidget::EnablePauseButton(bool bEnable)
{
    ui->pauseButton->setEnabled(bEnable);
}

void CProgramPanelWidget::IsResumeRun(bool bSet)
{
    m_IsResumeRun = bSet;
}

bool CProgramPanelWidget::IsResumeRun()
{
    return m_IsResumeRun;
}

bool CProgramPanelWidget::IsAbortEnabled()
{
    return this->ui->startButton->isEnabled();
}

void CProgramPanelWidget::OnProgramStartReadyUpdated()
{
    if (!m_SelectedProgramId.isEmpty())
        this->ui->startButton->setEnabled(true);
}

void CProgramPanelWidget::OnProgramActionStarted(DataManager::ProgramActionType_t ProgramActionType,
                                                     int remainingTimeTotal, const QDateTime& startDateTime, bool IsResume)
{

    qDebug()<<__FUNCTION__<<" remainingTimeTotal = "<< remainingTimeTotal ;

    Q_UNUSED(remainingTimeTotal);
    Q_UNUSED(startDateTime);
    Q_UNUSED(IsResume);
    if (DataManager::PROGRAM_START== ProgramActionType)
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->programRunningPanel->EnableProgramDetailButton(m_SelectedProgramId.at(0) != 'C');
        QString strIconName = ":/HimalayaImages/Icons/Program/"+ mp_ProgramList->GetProgram(m_SelectedProgramId)->GetIcon() + ".png";
        ui->programRunningPanel->SetPanelIcon(strIconName);
        ui->programRunningPanel->SetPanelTitle(QString("%1").arg(CFavoriteProgramsPanelWidget::SELECTED_PROGRAM_NAME));
    }
}

void CProgramPanelWidget::OnProgramActionStopped(DataManager::ProgramStatusType_t ProgramStatusType)
{
    if (DataManager::PROGRAM_STATUS_COMPLETED == ProgramStatusType)
    {
        SwitchToFavoritePanel();
    }
}

void CProgramPanelWidget::SwitchToFavoritePanel()
{
    ui->stackedWidget->setCurrentIndex(0);
}

}
