/****************************************************************************/
/*! \file Himalaya/HimalayaMain/Gui/Components/Dashboard/Source/DashboardWidget.cpp
 *
 *  \brief ReagentWidget Implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-10-22
 *   $Author:  $ Abe Yang
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

#include "Dashboard/Include/DashboardWidget.h"
#include "ui_DashboardWidget.h"
#include "Core/Include/DataConnector.h"
#include "MainMenu/Include/MainWindow.h"
#include <Dashboard/Include/CommonString.h>
#include <Dashboard/Include/FavoriteProgramsPanelWidget.h>
#include "Dashboard/Include/CassetteNumberInputWidget.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdLockStatus.h"
#include "Core/Include/GlobalHelper.h"

//lint -e613

using namespace Dashboard;

QString CDashboardWidget::m_SelectedProgramId = "";

CDashboardWidget::CDashboardWidget(Core::CDataConnector *p_DataConnector,
                                     MainMenu::CMainWindow *p_Parent) :
    ui(new Ui::CDashboardWidget),
    mp_DataConnector(p_DataConnector),
    mp_MainWindow(p_Parent),
    m_ParaffinStepIndex(-1),
    m_TimeProposedForProgram(0),
    m_CostedTimeBeforeParaffin(0),
    m_ProgramStartReady(false),
    m_IsWaitingCleaningProgram(false),
    m_CurProgramStepIndex(-1),
    m_ProcessRunning(false),
    m_IsDraining(false),
    m_bIsFirstStepFixation(false),
    m_TotalCassette(0)
{
    ui->setupUi(this);
    CONNECTSIGNALSLOT(mp_MainWindow, UserRoleChanged(), this, OnUserRoleChanged());
    CONNECTSIGNALSLOT(mp_MainWindow, ProcessStateChanged(), this, OnProcessStateChanged());

    ui->containerPanelWidget->SetPtrToMainWindow(mp_MainWindow, mp_DataConnector);
    ui->programPanelWidget->SetPtrToMainWindow(mp_MainWindow, mp_DataConnector);
    m_pUserSetting = mp_DataConnector->SettingsInterface->GetUserSettings();
    mp_ProgramList = mp_DataConnector->ProgramList;

    CONNECTSIGNALSIGNAL(this, ResetFocus(bool), ui->programPanelWidget, ResetFocus(bool));
    CONNECTSIGNALSIGNAL(this, AddItemsToFavoritePanel(bool), ui->programPanelWidget, AddItemsToFavoritePanel(bool));
    CONNECTSIGNALSLOT(ui->programPanelWidget, PrepareSelectedProgramChecking(const QString&), this, PrepareSelectedProgramChecking(const QString&));
    CONNECTSIGNALSLOT(mp_DataConnector, ProgramSelectedReply(const MsgClasses::CmdProgramSelectedReply &),
                      this, OnProgramSelectedReply(const MsgClasses::CmdProgramSelectedReply&));

    CONNECTSIGNALSLOT(ui->programPanelWidget, OnSelectEndDateTime(const QDateTime&),
                        this, OnSelectEndDateTime(const QDateTime &));

    CONNECTSIGNALSIGNAL(this, SendAsapDateTime(int, bool), ui->programPanelWidget, SendAsapDateTime(int, bool));

    CONNECTSIGNALSLOT(ui->programPanelWidget, RequestAsapDateTime(),
                        this, RequestAsapDateTime());

    CONNECTSIGNALSIGNAL(this, ProgramSelected(QString&, int, bool, bool, QList<QString>&),
                       ui->programPanelWidget, ProgramSelected(QString&, int, bool, bool, QList<QString>&));

    CONNECTSIGNALSIGNAL(this, UndoProgramSelection(),
                       ui->programPanelWidget, UndoProgramSelection());

    CONNECTSIGNALSLOT(mp_DataConnector, ProgramStartReady(),
                      ui->programPanelWidget, OnProgramStartReadyUpdated());

    CONNECTSIGNALSLOT(mp_DataConnector, ProgramStartReady(),
                      this, OnProgramStartReadyUpdated());

    CONNECTSIGNALSLOT(mp_DataConnector, RetortLockStatusChanged(const MsgClasses::CmdLockStatus &),
                      this, OnRetortLockStatusChanged(const MsgClasses::CmdLockStatus&));


    CONNECTSIGNALSIGNAL(this, ProgramSelected(QString&, QList<QString>&),
                       ui->containerPanelWidget, ProgramSelected(QString&, QList<QString>&));
    mp_MessageDlg = new MainMenu::CMessageDlg(this);

    CONNECTSIGNALSIGNAL(this, ProgramActionStarted(DataManager::ProgramActionType_t, int, const QDateTime&, bool),
                        ui->programPanelWidget, ProgramActionStarted(DataManager::ProgramActionType_t, int,
                                                                     const QDateTime&, bool));

    CONNECTSIGNALSIGNAL(this, ProgramActionStopped(DataManager::ProgramStatusType_t),
                        ui->programPanelWidget, ProgramActionStopped(DataManager::ProgramStatusType_t));

    CONNECTSIGNALSLOT(ui->programPanelWidget, CheckPreConditionsToRunProgram(),
                      this, CheckPreConditionsToRunProgram());

    CONNECTSIGNALSLOT(mp_DataConnector, ProgramWillComplete(),
                      this, OnProgramWillComplete());

    CONNECTSIGNALSLOT(mp_DataConnector, ProgramAborted(),
                      this, OnProgramAborted());

    CONNECTSIGNALSLOT(mp_DataConnector, ProgramBeginAbort(),
                              this, OnProgramBeginAbort());

    CONNECTSIGNALSLOT(mp_DataConnector, ProgramCompleted(),
                              this, OnProgramCompleted());

    CONNECTSIGNALSLOT(mp_DataConnector, ProgramRunBegin(),
                              this, OnProgramRunBegin());

    CONNECTSIGNALSLOT(mp_DataConnector, ProgramPaused(),
                              this, OnProgramPaused());

    CONNECTSIGNALSIGNAL(mp_DataConnector, CurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor &),
                      ui->programPanelWidget, CurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor &));

    CONNECTSIGNALSLOT(this, SwitchToFavoritePanel(),
                      ui->programPanelWidget, SwitchToFavoritePanel());

    CONNECTSIGNALSLOT(mp_DataConnector, CurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor &),
                      this, OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor &));

    CONNECTSIGNALSLOT(this, UpdateUserSetting(DataManager::CUserSettings&), mp_DataConnector, SendUpdatedSettings(DataManager::CUserSettings&));
    CONNECTSIGNALSLOT(mp_DataConnector, StationSuckDrain(const MsgClasses::CmdStationSuckDrain &),
                      this, OnStationSuckDrain(const MsgClasses::CmdStationSuckDrain &));
    CONNECTSIGNALSIGNAL(this, OnInteractStart(), ui->containerPanelWidget, OnInteractStart());
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();

    CONNECTSIGNALSIGNAL(this, UpdateProgram(DataManager::CProgram&), ui->programPanelWidget, UpdateProgram(DataManager::CProgram&));
}

CDashboardWidget::~CDashboardWidget()
{
    try {
            delete ui;
            delete mp_MessageDlg;
        }
        catch (...) {
            // to please Lint.
        }
}

void CDashboardWidget::OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor & cmd)
{
    m_CurProgramStepIndex = cmd.CurProgramStepIndex();
    if (cmd.CurProgramStepIndex() >=1 && m_CurrentUserRole == MainMenu::CMainWindow::Operator)
    {
        ui->programPanelWidget->EnableStartButton(false);
    }
    if (cmd.CurProgramStepIndex() > 2 && m_CurrentUserRole == MainMenu::CMainWindow::Operator)
    {
          ui->programPanelWidget->EnablePauseButton(false);
    }
}

void CDashboardWidget::OnRetortLockStatusChanged(const MsgClasses::CmdLockStatus& cmd)
{
    m_bRetortLocked = cmd.IsLocked();

    if (cmd.IsLocked())
    {
        //enable the "OK"
        if (m_IsWaitingCleaningProgram && mp_MessageDlg->isVisible())
        {
            mp_MessageDlg->EnableButton(1, true);
        }

        if (Core::CGlobalHelper::GetProgramPaused() && (m_pUserSetting->GetModeRMSProcessing() == Global::RMS_CASSETTES) && (m_SelectedProgramId.at(0) != 'C'))
        {
            mp_MessageDlg->SetIcon(QMessageBox::Information);
            mp_MessageDlg->SetTitle(CommonString::strInforMsg);
            mp_MessageDlg->SetText(m_strAddCassete);
            mp_MessageDlg->SetButtonText(1, CommonString::strYes);
            mp_MessageDlg->SetButtonText(3, CommonString::strNo);
            mp_MessageDlg->HideCenterButton();
            if (mp_MessageDlg->exec())
            {
                SetCassetteNumber();
            }
        }
    }
}

void CDashboardWidget::OnProgramStartReadyUpdated()
{
    m_ProgramStartReady = true;
}

void CDashboardWidget::OnProgramWillComplete()
{
    //log the reagent ID in last step
    const DataManager::CProgram* pProgram = mp_ProgramList->GetProgram(m_SelectedProgramId);
    QString strReagentIDOfLastStep = pProgram->GetProgramStep(pProgram->GetNumberOfSteps()-1)->GetReagentID();
    m_pUserSetting->SetReagentIdOfLastStep(strReagentIDOfLastStep);
    emit UpdateUserSetting(*m_pUserSetting);

    mp_MessageDlg->SetIcon(QMessageBox::Information);
    mp_MessageDlg->SetTitle(CommonString::strConfirmMsg);
    QString strTemp(m_strProgramComplete);
    strTemp = strTemp.arg(CFavoriteProgramsPanelWidget::SELECTED_PROGRAM_NAME);
    mp_MessageDlg->SetText(strTemp);
    mp_MessageDlg->SetButtonText(1, CommonString::strOK);
    mp_MessageDlg->HideButtons();

    emit ProgramActionStopped(DataManager::PROGRAM_STATUS_PAUSED);//pause ProgressBar and EndTime countdown

    if (mp_MessageDlg->exec())
    {
        m_IsDraining = true;

        //Resume EndTime countdown
        emit ProgramActionStarted(DataManager::PROGRAM_START, 0, Global::AdjustedTime::Instance().GetCurrentDateTime(), true);

        mp_DataConnector->SendProgramAction(m_SelectedProgramId, DataManager::PROGRAM_DRAIN);
        //disable pause and abort
        ui->programPanelWidget->EnableStartButton(false);
        ui->programPanelWidget->EnablePauseButton(false);
        return;
    }
}

void CDashboardWidget::OnProgramBeginAbort()
{
    //show "aborting"
    //time countdown
    //Todo:20, Abort time, will be given a rough value later;
    emit ProgramActionStarted(DataManager::PROGRAM_ABORT, 20, Global::AdjustedTime::Instance().GetCurrentDateTime(), false);
    ui->programPanelWidget->EnableStartButton(false);
}

//this function will be invoked after program Abort and completed
void CDashboardWidget::TakeOutSpecimenAndWaitRunCleaning()
{
    mp_MessageDlg->SetIcon(QMessageBox::Information);
    mp_MessageDlg->SetTitle(CommonString::strConfirmMsg);
    mp_MessageDlg->SetText(m_strTakeOutSpecimen);
    mp_MessageDlg->SetButtonText(1, CommonString::strOK);
    mp_MessageDlg->HideButtons();
    if (mp_MessageDlg->exec())
    {
        //represent the retort as contaminated status
        ui->containerPanelWidget->UpdateRetortStatus(DataManager::CONTAINER_STATUS_CONTAMINATED);

        mp_MessageDlg->SetText(m_strRetortContaminated);
        mp_MessageDlg->SetButtonText(1, CommonString::strOK);
        mp_MessageDlg->HideButtons();
        //mp_MessageDlg->EnableButton(1, false);//when lock is locked, "OK" will be enable
        mp_MessageDlg->EnableButton(1, true);//6.6 for test

        m_IsWaitingCleaningProgram = true;
        if (mp_MessageDlg->exec())
        {
            ui->programPanelWidget->ChangeStartButtonToStartState();
            //only show Cleaning program in the favorite panel
            emit AddItemsToFavoritePanel(true);
            //switch to the dashboard page
            mp_MainWindow->SetTabWidgetIndex();
            emit SwitchToFavoritePanel();
        }
    }
}

void CDashboardWidget::SetCassetteNumber()
{
    if (mp_DataConnector)
    {
        //input cassette number
        CCassetteNumberInputWidget *pCassetteInput = new Dashboard::CCassetteNumberInputWidget();
        pCassetteInput->setWindowFlags(Qt::CustomizeWindowHint);
        pCassetteInput->SetCurrentCassette(m_TotalCassette);
        pCassetteInput->ShowLabel();
        pCassetteInput->SetDialogTitle(m_strChangeCassetteBoxTitle);
        QRect scr = mp_MainWindow->geometry();
        pCassetteInput->move( scr.center() - pCassetteInput->rect().center());
        (void)pCassetteInput->exec();

        int cassetteNumber = pCassetteInput->CassetteNumber();
        if (-1 == cassetteNumber)
            return;//clicked Cancel button

        m_TotalCassette += cassetteNumber;
        mp_DataConnector->SendKeepCassetteCount(cassetteNumber);
        delete pCassetteInput;
    }
}

void CDashboardWidget::OnProgramAborted()
{
    //progress aborted;
    //aborting time countdown is hidden.

    //save the ReagentIdOfLastStep in case of: user restarts machine at this time
    const DataManager::CProgram* pProgram = mp_ProgramList->GetProgram(m_SelectedProgramId);
    QString strReagentIDOfLastStep = pProgram->GetProgramStep(m_CurProgramStepIndex)->GetReagentID();
    m_pUserSetting->SetReagentIdOfLastStep(strReagentIDOfLastStep);
    emit UpdateUserSetting(*m_pUserSetting);

    ui->programPanelWidget->IsResumeRun(false);
    m_CurProgramStepIndex = -1;

    emit ProgramActionStopped(DataManager::PROGRAM_STATUS_ABORTED);

    ui->programPanelWidget->EnableStartButton(false);

    QDateTime  endDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime();
    if (m_StartDateTime.isValid())
    {
        int days = m_StartDateTime.daysTo(endDateTime);
        m_pUserSetting->SetOperationHours(m_pUserSetting->GetOperationHours() + days * 24);
        m_pUserSetting->SetActiveCarbonHours(m_pUserSetting->GetActiveCarbonHours() + days * 24);
        emit UpdateUserSetting(*m_pUserSetting);
    }

    mp_MessageDlg->SetIcon(QMessageBox::Warning);
    mp_MessageDlg->SetTitle(CommonString::strWarning);
    QString strTemp;
    strTemp = m_strProgramIsAborted.arg(CFavoriteProgramsPanelWidget::SELECTED_PROGRAM_NAME);
    mp_MessageDlg->SetText(strTemp);
    mp_MessageDlg->SetButtonText(1, CommonString::strOK);
    mp_MessageDlg->HideButtons();
    if (mp_MessageDlg->exec())
    {
        this->TakeOutSpecimenAndWaitRunCleaning();
    }
}

void CDashboardWidget::OnProgramCompleted()
{
    ui->programPanelWidget->IsResumeRun(false);
    m_CurProgramStepIndex = -1;

    if (m_SelectedProgramId.at(0) == 'C')
    {
        QString reagentID("");
        m_pUserSetting->SetReagentIdOfLastStep(reagentID);//Clear CleaningProgram flag
        emit UpdateUserSetting(*m_pUserSetting);
        ui->programPanelWidget->EnableStartButton(false);
        emit AddItemsToFavoritePanel();
        ui->programPanelWidget->ChangeStartButtonToStartState();
    }

    emit ProgramActionStopped(DataManager::PROGRAM_STATUS_COMPLETED);

    QDateTime  endDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime();
    if (m_StartDateTime.isValid())
    {
        int days = m_StartDateTime.daysTo(endDateTime);
        m_pUserSetting->SetOperationHours(m_pUserSetting->GetOperationHours() + days * 24);
        m_pUserSetting->SetActiveCarbonHours(m_pUserSetting->GetActiveCarbonHours() + days * 24);
        emit UpdateUserSetting(*m_pUserSetting);
    }
}

void CDashboardWidget::OnProgramRunBegin()
{
    bool isResumeRun = ui->programPanelWidget->IsResumeRun();
    QDateTime curDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime();
    int remainingTime = curDateTime.secsTo(m_EndDateTime);
    emit ProgramActionStarted(DataManager::PROGRAM_START, remainingTime, curDateTime, isResumeRun);
    ui->programPanelWidget->IsResumeRun(true);

    if (m_SelectedProgramId.at(0) == 'C')
    {
        ui->programPanelWidget->EnablePauseButton(false);//disable pause button
        ui->programPanelWidget->EnableStartButton(false);//disable Stop button
    }
    else
    {
        ui->programPanelWidget->EnablePauseButton(true);//enable pause button

        if (m_CurProgramStepIndex>0 && m_CurrentUserRole == MainMenu::CMainWindow::Operator) // operator can't abort program when beginning the second step.
            ui->programPanelWidget->EnableStartButton(false);
        else
            ui->programPanelWidget->EnableStartButton(true);//enable stop button
    }

    m_StartDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime();
}

void CDashboardWidget::OnProgramPaused()
{
    ui->programPanelWidget->EnableStartButton(true);//enable stop button
    ui->programPanelWidget->ChangeStartButtonToStartState();
}

void CDashboardWidget::OnUnselectProgram()
{
    if (!m_SelectedProgramId.isEmpty())
    {
        m_SelectedProgramId = "";
        ui->programPanelWidget->EnableStartButton(false);
        m_StationList.clear();
        int asapEndTime = 0;

        emit ProgramSelected(m_SelectedProgramId, m_StationList);
        emit ProgramSelected(m_SelectedProgramId, asapEndTime, m_ProgramStartReady, m_bIsFirstStepFixation, m_StationList);
        emit UpdateSelectedStationList(m_StationList);
        emit UndoProgramSelection();
    }
}

bool CDashboardWidget::IsParaffinInProgram(const DataManager::CProgram* p_Program)
{
    m_ParaffinStepIndex = -1;
    if (!mp_DataConnector)
    {
        Q_ASSERT(0);
        return false;
    }

    QStringList ReagentIDList = p_Program->GetReagentIDList();
    for (int i = 0; i < ReagentIDList.size(); ++i)
    {
        QString ReagentGroupID = mp_DataConnector->ReagentList->GetReagent(ReagentIDList.at(i))->GetGroupID();
        DataManager::CReagentGroup* pReagentGroup = mp_DataConnector->ReagentGroupList->GetReagentGroup(ReagentGroupID);
        if(pReagentGroup->IsParraffin()){
            m_ParaffinStepIndex = i;
            return true;
        }
    }
    return false;
}

int CDashboardWidget::GetParaffinHeatingDuration()
{
    if (!mp_DataConnector)
        return 0;
    DataManager::CHimalayaUserSettings* userSetting = mp_DataConnector->SettingsInterface->GetUserSettings();
    int paraffinMeltingPoint = userSetting->GetTemperatureParaffinBath();
    if (paraffinMeltingPoint <= 64)
    {
        return 12;
    } else
    {
        return 15;
    }
}

//TimeActual--- get from Scheduler
//TimeDelta
//EndTimeAsap = TimeActual + TimeDelta;
int CDashboardWidget::GetASAPTime(int TimeActual,//TimeActual is seconds
                                  int TimeBeforeUseParaffin,
                                  int TimeCostedParaffinMelting,
                                  int& TimeDelta,
                                  bool& bCanNotRun)
{
    //IsParaffinInProgram()
    //Yes
      //calculate the timeBeforeUseParraffin
      //RemainingTimeMeltParraffin = 12 hour - TimeCosted
      //if RemainingTimeMeltParraffin > 0
        //if RemainingTimeMeltParraffin <= timeBeforeUseParraffin , TDelta = 0;
            //else TDelta = RemainingTimeMeltParraffin - timeBeforeUseParraffin
    //if RemainingTimeMeltParraffin <= 0 Paraffin is Melted, TDelta = 0;

    //No Paraffin TimeDelta = 0;
    bCanNotRun = false;
    if (-1 == m_ParaffinStepIndex)//No Paraffin in all program steps
    {
        TimeDelta = 0;
    }
    else
    {
        //calculate the timeBeforeUseParraffin
        //RemainingTimeMeltParraffin = 12 hour - TimeCosted
        //int RemainingTimeMeltParaffin = 12 * 60 * 60 - TimeCostedParaffinMelting;
        //int meltingDuration= GetParaffinHeatingDuration();
        int RemainingTimeMeltParaffin = 10 * 60 - TimeCostedParaffinMelting;
        if (RemainingTimeMeltParaffin > 0)
        {
          if (RemainingTimeMeltParaffin <= TimeBeforeUseParaffin)
              TimeDelta = 0;
           else
           {
              TimeDelta = RemainingTimeMeltParaffin - TimeBeforeUseParaffin;
              bCanNotRun = true;
           }
        }
        else
            TimeDelta = 0;  //Paraffin is Melted
    }

    return (TimeActual + TimeDelta);//seconds
}

bool CDashboardWidget::IsOKPreConditionsToRunProgram()
{

    if ("" == m_SelectedProgramId)
        return false;

    if (!m_bRetortLocked){
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        mp_MessageDlg->SetTitle(CommonString::strWarning);
        mp_MessageDlg->SetText(m_strRetortNotLock);
        mp_MessageDlg->SetButtonText(1, CommonString::strOK);
        mp_MessageDlg->HideButtons();
        if (mp_MessageDlg->exec())
        return false;
    }

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
        return false;
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
            mp_MessageDlg->SetTitle(CommonString::strWarning);
            mp_MessageDlg->SetText(m_strNotStartExpiredReagent);
            mp_MessageDlg->SetButtonText(1, CommonString::strOK);
            mp_MessageDlg->HideButtons();
            if (mp_MessageDlg->exec())
                return false;
        }
        else if(userRole == MainMenu::CMainWindow::Admin ||
            userRole == MainMenu::CMainWindow::Service)
        {
            mp_MessageDlg->SetIcon(QMessageBox::Warning);
            mp_MessageDlg->SetTitle(CommonString::strWarning);
            mp_MessageDlg->SetText(m_strStartExpiredReagent);
            mp_MessageDlg->SetButtonText(3, CommonString::strNo);
            mp_MessageDlg->SetButtonText(1, CommonString::strYes);
            mp_MessageDlg->HideCenterButton();    // Hiding First Two Buttons in the Message Dialog

            if (!mp_MessageDlg->exec())
                return false;
        }
    }


    //Check safe reagent
    if ((m_SelectedProgramId.at(0) != 'C') && (m_iWhichStepHasNoSafeReagent  != -1))
    {
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        mp_MessageDlg->SetTitle(CommonString::strWarning);
        QString strTemp = m_strCheckSafeReagent.arg(QString::number(m_iWhichStepHasNoSafeReagent +1)).arg(CFavoriteProgramsPanelWidget::SELECTED_PROGRAM_NAME);
        mp_MessageDlg->SetText(strTemp);
        mp_MessageDlg->SetButtonText(1, CommonString::strYes);
        mp_MessageDlg->SetButtonText(3, CommonString::strNo);
        mp_MessageDlg->HideCenterButton();
        if (!mp_MessageDlg->exec())
        {
            return false;
        }
    }

    bool bCanNotRun = true;
    if (m_SelectedProgramId.at(0) != 'C')
    {
        int paraffinMeltCostedTime = Global::AdjustedTime::Instance().GetCurrentDateTime().secsTo(m_ParaffinStartHeatingTime);
        int TimeDelta = 0;
        int asapEndTime = GetASAPTime(m_TimeProposedForProgram,
                                      m_CostedTimeBeforeParaffin, -paraffinMeltCostedTime, TimeDelta, bCanNotRun);

        asapEndTime = asapEndTime - 60;//60 seconds: buffer time for "select program" and "Run" operation.
        QDateTime newAsapEndDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(asapEndTime);
        if (newAsapEndDateTime > m_EndDateTime)
        {
            mp_MessageDlg->SetIcon(QMessageBox::Warning);
            mp_MessageDlg->SetTitle(CommonString::strWarning);
            mp_MessageDlg->SetText(m_strResetEndTime);
            mp_MessageDlg->SetButtonText(1, CommonString::strOK);
            mp_MessageDlg->HideButtons();
            if (mp_MessageDlg->exec())
            {
                return false;
            }
            return false;
        }
    }
    else
    {
        bCanNotRun = false;
    }

    if (!m_bIsFirstStepFixation && bCanNotRun)
    {
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        mp_MessageDlg->SetTitle(CommonString::strWarning);
        mp_MessageDlg->SetText(m_strCannotStartParaffinMelt);
        mp_MessageDlg->SetButtonText(1, CommonString::strOK);
        mp_MessageDlg->HideButtons();
        if (mp_MessageDlg->exec())
        {
            return false;
        }
    }

    if ( mp_DataConnector)
    {
        //input cassette number
        if (m_SelectedProgramId.at(0) != 'C' && Global::RMS_CASSETTES == mp_DataConnector->SettingsInterface->GetUserSettings()->GetModeRMSProcessing())
        {
            CCassetteNumberInputWidget *pCassetteInput = new Dashboard::CCassetteNumberInputWidget(NULL, mp_MainWindow);

            pCassetteInput->ShowLabel(false);
            pCassetteInput->setWindowFlags(Qt::CustomizeWindowHint);
            pCassetteInput->SetDialogTitle(m_strInputCassetteBoxTitle);
            (void)pCassetteInput->exec();

            int cassetteNumber = pCassetteInput->CassetteNumber();
            if (-1 == cassetteNumber)
                return false;//clicked Cancel button

            m_TotalCassette += cassetteNumber;
            mp_DataConnector->SendKeepCassetteCount(cassetteNumber);
            delete pCassetteInput;
        }
    }
    return true;
}

void CDashboardWidget::CheckPreConditionsToRunProgram()
{
    if (IsOKPreConditionsToRunProgram())
    {
        QString strTempProgramId(m_SelectedProgramId);
        if (m_SelectedProgramId.at(0) == 'C')
        {
            strTempProgramId.append("_");
            QString strReagentIDOfLastStep = m_pUserSetting->GetReagentIdOfLastStep();
            strTempProgramId.append(strReagentIDOfLastStep);
        }
        int EndTimeDelta = m_AsapEndDateTime.secsTo(m_EndDateTime);
        int delayTime = EndTimeDelta + m_TimeDelta;
        mp_DataConnector->SendProgramAction(strTempProgramId, DataManager::PROGRAM_START, delayTime);
        ui->programPanelWidget->ChangeStartButtonToStopState();
    }
    else
    {
        ui->programPanelWidget->EnableStartButton(true);
    }
}

void CDashboardWidget::PrepareSelectedProgramChecking(const QString& selectedProgramId)
{
    m_NewSelectedProgramId = selectedProgramId;
    (void)this->IsParaffinInProgram(mp_ProgramList->GetProgram(selectedProgramId));//to get m_ParaffinStepIndex
    //Notify Master, to get the time costed for paraffin Melting
    QString strTempProgramId(selectedProgramId);
    if (selectedProgramId.at(0) == 'C')
    {
      strTempProgramId.append("_");
      QString strReagentIDOfLastStep = m_pUserSetting->GetReagentIdOfLastStep();
      strTempProgramId.append(strReagentIDOfLastStep);
    }
    mp_DataConnector->SendProgramSelected(strTempProgramId, m_ParaffinStepIndex);
}

void CDashboardWidget::OnProgramSelectedReply(const MsgClasses::CmdProgramSelectedReply& cmd)
{
    const QList<QString>& stationList = cmd.StationList();
    if (stationList.count() == 0)
    {
        if (m_NewSelectedProgramId.at(0) == 'C')
        {
            mp_MessageDlg->SetIcon(QMessageBox::Warning);
            mp_MessageDlg->SetTitle(CommonString::strWarning);
            mp_MessageDlg->SetText("CleaningProgram does not need to be run as the reagent compatibility");
            mp_MessageDlg->SetButtonText(1, CommonString::strOK);
            mp_MessageDlg->HideButtons();
            if (mp_MessageDlg->exec())
            {
                QString reagentID("");
                m_pUserSetting->SetReagentIdOfLastStep(reagentID);//Clear CleaningProgram flag
                emit UpdateUserSetting(*m_pUserSetting);
                emit AddItemsToFavoritePanel();
                ui->programPanelWidget->ChangeStartButtonToStartState();
                return;
            }
        }
    }

    //get the proposed program end DateTime
    m_TimeProposedForProgram = cmd.TimeProposed();
    m_ParaffinStartHeatingTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(-cmd.ParaffinMeltCostedTime());
    m_CostedTimeBeforeParaffin = cmd.CostedTimeBeforeParaffin();
    m_iWhichStepHasNoSafeReagent = cmd.WhichStepHasNoSafeReagent();
    bool bCanotRun = true;
    m_TimeDelta = 0;
    int asapEndTime = GetASAPTime(cmd.TimeProposed(),
                                  cmd.CostedTimeBeforeParaffin(), cmd.ParaffinMeltCostedTime(), m_TimeDelta, bCanotRun);

    //firstly check whether there is any station not existing for some program steps
    for (int i = 0; i < stationList.count(); i++)
    {
        if ("" == stationList.at(i))
        {
            mp_MessageDlg->SetIcon(QMessageBox::Warning);
            mp_MessageDlg->SetTitle(CommonString::strWarning);
            QString strTemp = m_strNotFoundStation.arg(QString::number(i+1)).arg(CFavoriteProgramsPanelWidget::SELECTED_PROGRAM_NAME);
            mp_MessageDlg->SetText(strTemp);
            mp_MessageDlg->SetButtonText(1, CommonString::strOK);
            mp_MessageDlg->HideButtons();

            ui->programPanelWidget->EnableStartButton(false);

            if (mp_MessageDlg->exec())
            {
                emit ResetFocus(true);
                return;
            }
            return;
        }
        else //then check station's status is empty?
        {
            DataManager::CDashboardStation* pStation = mp_DataConnector->DashboardStationList->GetDashboardStation(stationList.at(i));
            if (!pStation)
            {
                Q_ASSERT(0);
            }

            if ("Empty" == pStation->GetDashboardReagentStatus())
            {
                mp_MessageDlg->SetIcon(QMessageBox::Warning);
                mp_MessageDlg->SetTitle(CommonString::strWarning);
                QString strTemp = m_strCheckEmptyStation.arg(pStation->GetDashboardStationName()).arg(QString::number(i+1)).arg(CFavoriteProgramsPanelWidget::SELECTED_PROGRAM_NAME);
                mp_MessageDlg->SetText(strTemp);
                mp_MessageDlg->SetButtonText(1, CommonString::strOK);
                mp_MessageDlg->HideButtons();

                if (mp_MessageDlg->exec())
                {
                    emit ResetFocus(true);
                    return;
                }
                return;
            }
        }
    }

    emit ResetFocus(false);

    m_SelectedProgramId = m_NewSelectedProgramId;
    m_StationList.clear();
    m_StationList = stationList;

    m_AsapEndDateTime = m_EndDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(asapEndTime);

    m_bIsFirstStepFixation = IsFixationInFirstStep();
    emit ProgramSelected(m_SelectedProgramId, asapEndTime, m_ProgramStartReady, m_bIsFirstStepFixation, m_StationList);
    emit ProgramSelected(m_SelectedProgramId, m_StationList);
    emit UpdateSelectedStationList(m_StationList);
}

bool CDashboardWidget::IsFixationInFirstStep()
{
    bool bIsFixationInFirstStep = true;
    QString stationID = m_StationList.at(0);
    DataManager::CDashboardStation *p_Station = mp_DataConnector->DashboardStationList->GetDashboardStation(stationID);
    QString reagentID = p_Station->GetDashboardReagentID();
    DataManager::CReagent const *p_Reagent = mp_DataConnector->ReagentList->GetReagent(reagentID);

    DataManager::CReagentGroup const *p_ReagentGroup = mp_DataConnector->ReagentGroupList->GetReagentGroup(p_Reagent->GetGroupID());
    if ("RG1" != p_ReagentGroup->GetGroupID())
    {
        bIsFixationInFirstStep = false;
    }
    return bIsFixationInFirstStep;
}

void CDashboardWidget::changeEvent(QEvent *p_Event)
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

void CDashboardWidget::RetranslateUI()
{
    m_strCheckSafeReagent = QApplication::translate("Dashboard::CDashboardWidget", "No safe reagent for Program step \"%1\" of \"%2\" in case of error happen.Would you like to continue?", 0, QApplication::UnicodeUTF8);
    m_strNotFoundStation = QApplication::translate("Dashboard::CDashboardWidget", "Program step \"%1\" of \"%2\" can not find the corresponding reagent station, one station only can be used once in the program, please set a station for the reagent in this step.", 0, QApplication::UnicodeUTF8);
    m_strCheckEmptyStation = QApplication::translate("Dashboard::CDashboardWidget", "The Station \"%1\" status is set as Empty in Program step \"%2\" of \"%3\", it can not be executed.", 0, QApplication::UnicodeUTF8);
    m_strResetEndTime = QApplication::translate("Dashboard::CDashboardWidget", "Please re-set the End Date&Time of the current selected program.", 0, QApplication::UnicodeUTF8);
    m_strCannotStartParaffinMelt = QApplication::translate("Dashboard::CDashboardWidget", "Program cannot start as paraffin is not melted completely, as well as the first program step is not fixation reagent.", 0, QApplication::UnicodeUTF8);
    m_strInputCassetteBoxTitle = QApplication::translate("Dashboard::CDashboardWidget", "Please enter cassette number:", 0, QApplication::UnicodeUTF8);
    m_strProgramComplete = QApplication::translate("Dashboard::CDashboardWidget", "Program \"%1\" is complete! Would you like to drain the retort?", 0, QApplication::UnicodeUTF8);
    m_strTakeOutSpecimen = QApplication::translate("Dashboard::CDashboardWidget", "Please take out your specimen!", 0, QApplication::UnicodeUTF8);
    m_strRetortContaminated  = QApplication::translate("Dashboard::CDashboardWidget", "The retort is contaminated, please lock the retort and select Cleaning Program to run!", 0, QApplication::UnicodeUTF8);
    m_strProgramIsAborted  = QApplication::translate("Dashboard::CDashboardWidget", "Program \"%1\" is aborted!", 0, QApplication::UnicodeUTF8);
    m_strRetortNotLock = QApplication::translate("Dashboard::CDashboardWidget", "Please close and lock the retort, then try again!", 0, QApplication::UnicodeUTF8);
    m_strNotStartRMSOFF = QApplication::translate("Dashboard::CDashboardWidget", "Leica Program can't be operated with RMS OFF.", 0, QApplication::UnicodeUTF8);
    m_strNotStartExpiredReagent = QApplication::translate("Dashboard::CDashboardWidget", "Reagents needed for this program are expired! You can't operate this program.", 0, QApplication::UnicodeUTF8);
    m_strStartExpiredReagent =  QApplication::translate("Dashboard::CDashboardWidget", "Do you want to Start the Program with Expired Reagents?", 0, QApplication::UnicodeUTF8);
    //m_strChangeCassetteBoxTitle = QApplication::translate("Dashboard::CDashboardWidget", "Please enter the total number of cassetts in the retort:", 0, QApplication::UnicodeUTF8);
    m_strChangeCassetteBoxTitle = QApplication::translate("Dashboard::CDashboardWidget", "Please enter the new-added number of cassetts in the retort:", 0, QApplication::UnicodeUTF8);
    m_strAddCassete = QApplication::translate("Dashboard::CDashboardWidget", "Did you add new cassetts?", 0, QApplication::UnicodeUTF8);
}

void CDashboardWidget::OnSelectEndDateTime(const QDateTime& dateTime)
{
    m_EndDateTime = dateTime;
}

void CDashboardWidget::RequestAsapDateTime()
{
    bool bCanotRun = true;
    int paraffinMeltCostedTime = Global::AdjustedTime::Instance().GetCurrentDateTime().secsTo(m_ParaffinStartHeatingTime);
    m_TimeDelta = 0;
    int asapEndTime = GetASAPTime(m_TimeProposedForProgram,
                                  m_CostedTimeBeforeParaffin, -paraffinMeltCostedTime, m_TimeDelta, bCanotRun);

    m_AsapEndDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(asapEndTime);

    emit SendAsapDateTime(asapEndTime, m_bIsFirstStepFixation);
}

void CDashboardWidget::OnStationSuckDrain(const MsgClasses::CmdStationSuckDrain & cmd)
{
    if (m_IsDraining && !cmd.IsStart() && !cmd.IsSuck())
    {
        emit ProgramActionStopped(DataManager::PROGRAM_STATUS_ABORTED);
        this->TakeOutSpecimenAndWaitRunCleaning();//pause ProgressBar and EndTime countdown
        m_IsDraining = false;//when abort or pause, set this too?
    }
}

/****************************************************************************/
/*!
 *  \brief This slot is called when User Role changes
 */
/****************************************************************************/
void CDashboardWidget::OnUserRoleChanged()
{
#if 0
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
    if (m_CurProgramStepIndex > 2 && m_CurrentUserRole == MainMenu::CMainWindow::Operator) {
        ui->programPanelWidget->EnablePauseButton(false);//in fact, it will disable pause button when running
    }
    else if ((m_ProcessRunning || m_IsDraining) && (m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
                                  m_CurrentUserRole == MainMenu::CMainWindow::Service)) {
        ui->programPanelWidget->EnablePauseButton(true);

    }

    if (m_IsDraining == true && m_CurrentUserRole == MainMenu::CMainWindow::Operator) {
         ui->programPanelWidget->EnableStartButton(false); // disable abort button
    }
#endif

    // the matrix:
    /*
     *  user role               1st step        2nd~3rd step	after 3steps        Comment
        --------------------------------------------------------------------------------------------------------------
        Standard User
                                Pause enable	Pause enable	Pause disable       the last step's draining,Pause&Abort is disable
                                Abort enable	Abort disable	Abort disable       the last step's draining,Pause&Abort is disable

        Supervisor& Service
                                Pause enable	Pause enable	Pause enable        the last step's draining,Pause&Abort is disable
                                Abort enable	Abort enable	Abort enable        the last step's draining,Pause&Abort is disable

     * */
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
    if (m_CurrentUserRole == MainMenu::CMainWindow::Operator) {
        switch (m_CurProgramStepIndex) {
        case 1:
            ui->programPanelWidget->EnablePauseButton(true);
            ui->programPanelWidget->EnableStartButton(true);
            break;
        case 2:
            ui->programPanelWidget->EnablePauseButton(true);
            ui->programPanelWidget->EnableStartButton(false);
            break;
        case 3:
            ui->programPanelWidget->EnablePauseButton(false);
            ui->programPanelWidget->EnableStartButton(false);
            break;
        default:
            ui->programPanelWidget->EnablePauseButton(false);
            ui->programPanelWidget->EnableStartButton(false);
            break;
        }

    }
    else if (m_CurrentUserRole == MainMenu::CMainWindow::Admin || m_CurrentUserRole == MainMenu::CMainWindow::Service) {
        ui->programPanelWidget->EnablePauseButton(true);
        ui->programPanelWidget->EnableStartButton(true);
    }

    if (m_IsDraining == true) {
        ui->programPanelWidget->EnablePauseButton(false);
        ui->programPanelWidget->EnableStartButton(false); // disable abort button
    }
}

void CDashboardWidget::OnProcessStateChanged()
{
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
}

bool CDashboardWidget::IsAbortEnabled()
{
    return ui->programPanelWidget->IsAbortEnabled();
}



