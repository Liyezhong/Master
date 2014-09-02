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
#include "Core/Include/GlobalHelper.h"

//lint -e613

namespace Dashboard {
CProgramPanelWidget::CProgramPanelWidget(QWidget *p) :
    CPanelFrame(p),
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
    CONNECTSIGNALSLOT(this, ResetFocus(bool), ui->favoriteProgramsPanel, OnResetFocus(bool));
    CONNECTSIGNALSLOT(this, AddItemsToFavoritePanel(bool), ui->favoriteProgramsPanel, AddItemsToFavoritePanel(bool));
    CONNECTSIGNALSIGNAL(ui->favoriteProgramsPanel, PrepareSelectedProgramChecking(const QString&), this, PrepareSelectedProgramChecking(const QString&));

    CONNECTSIGNALSIGNAL(ui->favoriteProgramsPanel, OnSelectEndDateTime(const QDateTime&), this, OnSelectEndDateTime(const QDateTime &));
    CONNECTSIGNALSLOT(ui->favoriteProgramsPanel, OnSelectEndDateTime(const QDateTime&),
                      ui->programRunningPanel, OnUserSetEndDateTime(const QDateTime&));
    CONNECTSIGNALSIGNAL(ui->favoriteProgramsPanel, RequestAsapDateTime(), this, RequestAsapDateTime());
    CONNECTSIGNALSLOT(ui->favoriteProgramsPanel, UpdateFavProgram(), this, OnUpdatePanelProgram());

    CONNECTSIGNALSIGNAL(this, SendAsapDateTime(int, bool), ui->favoriteProgramsPanel, SendAsapDateTime(int, bool));

    CONNECTSIGNALSLOT(this, ProgramSelected(QString&, int, bool, bool, QList<QString>&),
                      ui->favoriteProgramsPanel, ProgramSelected(QString&, int, bool, bool));

    CONNECTSIGNALSLOT(this, ProgramSelected(QString&, int, bool, bool, QList<QString>&),
                      ui->programRunningPanel, ProgramSelected(QString&, int, bool, bool, QList<QString>&));

    CONNECTSIGNALSLOT(this, ProgramSelected(QString&, int, bool, bool, QList<QString>&),
                      this, OnProgramSelected(QString&, int, bool, bool, QList<QString>&));

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

    m_Timer = new QTimer(this);
    m_Timer->setInterval(2*60*1000);
    //(void)connect(m_Timer, SIGNAL(timeout()), this, SLOT(OnResumeProgramReminder()));
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
    m_strConfirmation = QApplication::translate("Dashboard::CProgramPanelWidget", "Confirmation Message", 0, QApplication::UnicodeUTF8);
    m_strAbortProgram = QApplication::translate("Dashboard::CProgramPanelWidget", "Do you want to abort the program?", 0, QApplication::UnicodeUTF8);
    m_strPauseProgram = QApplication::translate("Dashboard::CProgramPanelWidget", "The program should be resumed in two minutes!", 0, QApplication::UnicodeUTF8);

    m_strYes = QApplication::translate("Dashboard::CProgramPanelWidget", "Yes", 0, QApplication::UnicodeUTF8);
    m_strNo = QApplication::translate("Dashboard::CProgramPanelWidget", "No", 0, QApplication::UnicodeUTF8);
    m_strOK = QApplication::translate("Dashboard::CProgramPanelWidget", "OK", 0, QApplication::UnicodeUTF8);
    m_strCancel = QApplication::translate("Dashboard::CProgramPanelWidget", "Cancel", 0, QApplication::UnicodeUTF8);
    m_strWarning = QApplication::translate("Dashboard::CProgramPanelWidget", "Warning", 0, QApplication::UnicodeUTF8);
    m_strProgram = QApplication::translate("Dashboard::CProgramPanelWidget", "Program", 0, QApplication::UnicodeUTF8);
    m_strInformation = QApplication::translate("Dashboard::CProgramPanelWidget", "Information", 0, QApplication::UnicodeUTF8);
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
    CONNECTSIGNALSLOT(mp_DataConnector, UpdateProgramEndTime(int), ui->programRunningPanel, UpdateEndDateTime(int));
    CONNECTSIGNALSLOT(mp_DataConnector, UpdateProgramTimerStatus(bool), ui->programRunningPanel, UpdateProgramTimerStatus(bool));
}

void CProgramPanelWidget::OnProgramSelected(QString& ProgramId, int asapEndTime, bool bProgramStartReady, bool bIsFirstStepFixation, QList<QString>& selectedStationList)
{
    Q_UNUSED(bIsFirstStepFixation);
    m_SelectedProgramId = ProgramId;
    m_EndDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(asapEndTime);
    if (bProgramStartReady)
        OnProgramStartReadyUpdated();
    m_StationList.clear();
    m_StationList = selectedStationList;
    Core::CGlobalHelper::SetStationList(selectedStationList);
}

bool CProgramPanelWidget::CheckPreConditionsToPauseProgram()
{
    m_Timer->stop();
    m_Timer->start();
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
        Core::CGlobalHelper::SetProgramPaused(false);
        ui->startButton->setEnabled(false);//protect to click twice in a short time
        m_Timer->stop();

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

                    mp_DataConnector->SendProgramAction(strTempProgramId, DataManager::PROGRAM_START, 0);
                    ChangeStartButtonToStopState();
                    return;
                }

                emit CheckPreConditionsToRunProgram();
            }
            break;
            case DataManager::PROGRAM_ABORT:
            {
                if(CheckPreConditionsToAbortProgram()) {
                    ui->pauseButton->setEnabled(false);
                    mp_DataConnector->SendProgramAction(m_SelectedProgramId, DataManager::PROGRAM_ABORT);
                    m_ProgramNextAction = DataManager::PROGRAM_START;

                }
                else {
                    // cancel button pressed.
                    ui->startButton->setEnabled(true);
                }
            }
            break;
        }
    }
    else if (whichBtn == Dashboard::secondButton)//pause
    {
        Core::CGlobalHelper::SetProgramPaused(true);
        ui->pauseButton->setEnabled(false);//protect to click twice in a short time
        //ui->startButton->setEnabled(false);
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
    ui->startButton->update();
    ui->startButton->IsStartStatus(false);
    m_ProgramNextAction = DataManager::PROGRAM_ABORT;
}

void CProgramPanelWidget::ChangeStartButtonToStartState()
{
    ui->startButton->setText(tr("Start"));
    ui->startButton->update();
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

void CProgramPanelWidget::OnResumeProgramReminder()
{
    MainMenu::CMessageDlg ConfirmationMessageDlg;

    ConfirmationMessageDlg.SetTitle(m_strConfirmation);
    ConfirmationMessageDlg.SetText(m_strPauseProgram);
    ConfirmationMessageDlg.SetIcon(QMessageBox::Information);
    ConfirmationMessageDlg.SetButtonText(1, CommonString::strOK);
    ConfirmationMessageDlg.HideButtons();
    (void)ConfirmationMessageDlg.exec();
}

void CProgramPanelWidget::OnUpdatePanelProgram()
{
    EnableStartButton(false);
}

}
