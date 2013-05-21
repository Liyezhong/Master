/****************************************************************************/
/*! \file Dashboard/Source/DashboardWidget.cpp
 *
 *  \brief Implementation file for class DashboardWidget.
 *
 *  $Version:   $ 0.2
 *  $Date:      $ 2013-01-03, 08-Feb-2013
 *  $Author:    $ Swati Tiwari, Santhosha Kumar Raju
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
#include "Global/Include/Utils.h"
#include "Dashboard/Include/DashboardWidget.h"
#include "ui_DashboardWidget.h"
#include <QDebug>
#include "MainMenu/Include/SliderControl.h"
#include "Dashboard/Include/DashboardProgramStatusWidget.h"
#include "Dashboard/Include/CassetteNumberInputWidget.h"
#include "HimalayaDataContainer/Containers/UserSettings/Include/HimalayaUserSettings.h"


namespace Dashboard {

QString CDashboardWidget::m_SelectedProgramId = "";

CDashboardWidget::CDashboardWidget(Core::CDataConnector *p_DataConnector,
                                   MainMenu::CMainWindow *p_Parent): MainMenu::CPanelFrame(p_Parent),
                                   mp_Ui(new Ui::CDashboardWidget),mp_MainWindow(p_Parent),
                                   mp_DataConnector(p_DataConnector),
                                   m_ProgramNextAction(DataManager::PROGRAM_START),
                                   m_UserRoleChanged(false),
                                   m_TimeProposed(0),
                                   m_ParaffinStepIndex(-1),
                                   m_IsWaitingCleaningProgram(false),
                                   m_ForceRunCleanProgram(false),
                                   m_IsResumeRun(false),
                                   m_CurProgramStepIndex(-1),
                                   m_IsDraining(false),
                                   m_CheckEndDatetimeAgain(false),
                                   m_ProcessRunning(false)

{
     mp_Ui->setupUi(GetContentFrame());
     SetPanelTitle(tr("Dashboard"));

     QPalette Palette = mp_Ui->dashboardView->palette();
     QColor BaseColor = Palette.color(QPalette::Base);
     BaseColor.setAlpha(0);
     Palette.setColor(QPalette::Base, BaseColor);
     mp_Ui->dashboardView->setPalette(Palette);

     mp_ProgramStatusWidget = new Dashboard::CDashboardProgramStatusWidget();
     mp_ProgramStatusWidget->setWindowFlags(Qt::CustomizeWindowHint);
     mp_ProgramStatusWidget->setVisible(false);

     mp_DashboardScene = new CDashboardScene(mp_DataConnector, this, mp_MainWindow);
     mp_Ui->dashboardView->setScene(mp_DashboardScene);
     CONNECTSIGNALSLOT(mp_DashboardScene, OnSelectDateTime(const QDateTime &), this, OnSelectDateTime(const QDateTime&));
     CONNECTSIGNALSIGNAL(this, ProgramActionStarted(DataManager::ProgramActionType_t, int, const QDateTime&, bool),
                         mp_DashboardScene, ProgramActionStarted(DataManager::ProgramActionType_t, int, const QDateTime&, bool));

     CONNECTSIGNALSIGNAL(this, ProgramActionStopped(DataManager::ProgramStatusType_t),
                         mp_DashboardScene, ProgramActionStopped(DataManager::ProgramStatusType_t));

     CONNECTSIGNALSLOT(mp_MainWindow, ProcessStateChanged(), this, OnProcessStateChanged());

     mp_Separator = new QFrame();
     mp_Separator->setParent(this);  // Set Parent of this Frame as the Dashboard Widget.
     DrawSeparatorLine();

     mp_ProgramList = mp_DataConnector->ProgramList;

     m_btnGroup.addButton(mp_Ui->playButton, Dashboard::firstButton);
     m_btnGroup.addButton(mp_Ui->abortButton, Dashboard::secondButton);

     //mp_Ui->abortButton->setEnabled(false);
     EnablePlayButton(true);

     m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
     mp_MessageDlg = new MainMenu::CMessageDlg();

     CONNECTSIGNALSLOT(mp_MainWindow, UserRoleChanged(), this, OnUserRoleChanged());
     CONNECTSIGNALSLOT(mp_Ui->pgmsComboBox, activated(int), this, OnActivated(int));
     CONNECTSIGNALSLOT(mp_Ui->pgmsComboBox, ButtonPress(), this, OnComBoxButtonPress());

     CONNECTSIGNALSLOT(&m_btnGroup, buttonClicked(int), this, OnButtonClicked(int));
     CONNECTSIGNALSLOT(mp_ProgramStatusWidget, AbortClicked(int), this, OnButtonClicked(int));

     CONNECTSIGNALSLOT(this, ProgramSelected(QString&, int, QList<QString>&),
                        mp_DashboardScene, UpdateDashboardSceneReagentsForProgram(QString &, int, QList<QString>&));

     CONNECTSIGNALSLOT(mp_DataConnector, ProgramStartReady(),
                       this, OnProgramStartReadyUpdated());

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

     CONNECTSIGNALSLOT(mp_DataConnector, RetortLockStatusChanged(const MsgClasses::CmdRetortLockStatus &),
                       this, OnRetortLockStatusChanged(const MsgClasses::CmdRetortLockStatus&));

     CONNECTSIGNALSLOT(mp_Ui->retortSlider, positionChanged(MainMenu::CSliderControl::Position_t),
                       this, RetortSliderPositionChanged(MainMenu::CSliderControl::Position_t));

     CONNECTSIGNALSLOT(mp_DataConnector, ProgramSelectedReply(const MsgClasses::CmdProgramSelectedReply &),
                       this, OnProgramSelectedReply(const MsgClasses::CmdProgramSelectedReply&));

     CONNECTSIGNALSLOT(mp_DataConnector, CurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor &),
                       this, OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor &));

     CONNECTSIGNALSLOT(mp_DataConnector, StationSuckDrain(const MsgClasses::CmdStationSuckDrain &),
                       this, OnStationSuckDrain(const MsgClasses::CmdStationSuckDrain &));


}

CDashboardWidget::~CDashboardWidget()
{
    try {
        delete mp_Separator;
        delete mp_ProgramStatusWidget;
        delete mp_DashboardScene;
        delete mp_MessageDlg;
        delete mp_Ui;
    } catch(...) {

    }
}

void CDashboardWidget::RetortSliderPositionChanged(MainMenu::CSliderControl::Position_t Position)
{
    if (Position ==  MainMenu::CSliderControl::PosLeft)
        mp_DataConnector->SendRetortLock(true);
    else
        mp_DataConnector->SendRetortLock(false);
}

void CDashboardWidget::DrawSeparatorLine()
{

    mp_Separator->setFrameShape(QFrame::HLine);
    mp_Separator->setLineWidth(2);
    mp_Separator->setFrameShadow(QFrame::Plain);
    mp_Separator->setGeometry(4,460,685,2);
    mp_Separator->show();
}

void CDashboardWidget::AddItemsToComboBox()
{
    m_FavProgramIDs.clear();
    mp_Ui->pgmsComboBox->clear();

    //if(m_UserRoleChanged) {
        //if((m_CurrentUserRole == MainMenu::CMainWindow::Admin) ||
         //       (m_CurrentUserRole == MainMenu::CMainWindow::Service))
        //{
            m_FavProgramIDs = mp_ProgramList->GetFavoriteProgramIDs(); // get five favorite Programs' ID
            for ( int i = 0; i < m_FavProgramIDs.count(); i++)
            {
                QString ProgramId = m_FavProgramIDs.at(i);
                QString ProgramName = mp_ProgramList->GetProgram(ProgramId)->GetName();
                // Replace this statement later
                QIcon ProgramIcon; // = QIcon(mp_ProgramList->GetProgram(ProgramId)->GetIcon());
                if(ProgramId.at(0) == 'L') {
                    ProgramIcon = QIcon(":/HimalayaImages/Icons/MISC/Icon_Leica.png");
                } else {
                    ProgramIcon = QIcon(":/HimalayaImages/Icons/MISC/TickOk.png");
                }
                mp_Ui->pgmsComboBox->insertItem(i, ProgramIcon, ProgramName);
            }
        //}
    //}
}

/****************************************************************************/
/*!
 *  \brief This slot is called when User Role changes
 */
/****************************************************************************/
void CDashboardWidget::OnUserRoleChanged()
{
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
    m_UserRoleChanged = true;
    if (m_CurProgramStepIndex > 2 && m_CurrentUserRole == MainMenu::CMainWindow::Operator)
    {
        this->EnablePlayButton(false);//in fact, it will disable pause button when runing
    }
    else
    if (m_ProcessRunning && m_CurrentUserRole == MainMenu::CMainWindow::Admin || m_CurrentUserRole == MainMenu::CMainWindow::Service)
        EnablePlayButton(true);

}


void CDashboardWidget::OnButtonClicked(int whichBtn)
{
    if ( whichBtn == Dashboard::firstButton ) {
        switch(m_ProgramNextAction)
        {
            case DataManager::PROGRAM_START:
            {
                if (m_IsResumeRun)
                {
                    mp_DataConnector->SendProgramAction(m_SelectedProgramId, DataManager::PROGRAM_START, m_EndDateTime);
                    mp_Ui->playButton->setIcon(QIcon(":/HimalayaImages/Icons/Dashboard/Operation/Operation_Pause.png"));
                    m_ProgramNextAction = DataManager::PROGRAM_PAUSE;
                    return;
                }

                CheckPreConditionsToRunProgram();
            }
            break;
            case DataManager::PROGRAM_PAUSE:
            {
                if(CheckPreConditionsToPauseProgram())
                {
                    mp_DataConnector->SendProgramAction(m_SelectedProgramId, DataManager::PROGRAM_PAUSE);
                    m_ProgramNextAction = DataManager::PROGRAM_START;
                    mp_Ui->playButton->setIcon(QIcon(":/HimalayaImages/Icons/Dashboard/Operation/Operation_Start_Resume.png"));

                } else {
                    // Take Necessary Action
                }
            }
            break;
        }
    }
    else if (whichBtn == Dashboard::secondButton)
    {
        if(CheckPreConditionsToAbortProgram()) {
            mp_DataConnector->SendProgramAction(m_SelectedProgramId, DataManager::PROGRAM_ABORT);

        }
    }

}

bool CDashboardWidget::CheckSelectedProgram(bool& bRevertSelectProgram, QString ProgramID)
{
    bRevertSelectProgram = false;
    if (!SelectedProgramId().isEmpty())
    {
        if (!ProgramID.isEmpty())
        {
            if (ProgramID != SelectedProgramId())
                return true;
        }

        MainMenu::CMessageDlg ConfirmationMessageDlg;
        ConfirmationMessageDlg.SetTitle(tr("Warning"));
        QString strMsg;
        strMsg.append(tr("As the program \""));
        strMsg.append(Dashboard::CDashboardDateTimeWidget::SELECTED_PROGRAM_NAME);
        strMsg.append(tr("\" is selected, this operation will reusult in an incorrect program result,"));
        strMsg.append(tr("if you click \"Yes\", the selected program will unselect."));
        ConfirmationMessageDlg.SetText(strMsg);
        ConfirmationMessageDlg.SetIcon(QMessageBox::Warning);
        ConfirmationMessageDlg.SetButtonText(1, tr("Yes"));
        ConfirmationMessageDlg.SetButtonText(3, tr("Cancel"));
        ConfirmationMessageDlg.HideCenterButton();
        if (!ConfirmationMessageDlg.exec())
            return false;
        else
            bRevertSelectProgram = true;

    }
    return true;
}

void CDashboardWidget::OnUnselectProgram()
{
    if (!m_SelectedProgramId.isEmpty())
    {
        m_SelectedProgramId = "";
        EnablePlayButton(false);
        m_StationList.clear();
        //Show program name in the comboBox
        CDashboardDateTimeWidget::SELECTED_PROGRAM_NAME = tr("Program");
        mp_Ui->pgmsComboBox->UpdateSelectedProgramName(CDashboardDateTimeWidget::SELECTED_PROGRAM_NAME);
        int asapEndTime = 0;
        emit ProgramSelected(m_SelectedProgramId, asapEndTime, m_StationList);//for UI update
    }
}

void CDashboardWidget::OnSelectDateTime(const QDateTime& dateTime)
{
    m_EndDateTime = dateTime;
}

bool CDashboardWidget::IsParaffinInProgram(const DataManager::CProgram* p_Program)
{
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
            m_ParaffinStepIndex= i;
            return true;
        }
    }
    return false;
}

//TimeActual--- get from Scheduler
//TimeDelta
//EndTimeAsap = TimeActual + TimeDelta;
int CDashboardWidget::GetASAPTime(const DataManager::CProgram* p_Program,
                                  int TimeActual,//TimeActual is seconds
                                  int timeBeforeUseParaffin,
                                  int TimeCostedParaffinMelting,
                                  bool& bCanotRun)
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
    bCanotRun = false;
    int TimeDelta; //seconds
    if (-1 == m_ParaffinStepIndex)//No Paraffin in all program steps
    {
        TimeDelta = 0;
    }
    else
    {
        //calculate the timeBeforeUseParraffin
        //RemainingTimeMeltParraffin = 12 hour - TimeCosted
        int RemainingTimeMeltParaffin = 12 * 60 * 60 - TimeCostedParaffinMelting;
        if (RemainingTimeMeltParaffin > 0)
        {
          if (RemainingTimeMeltParaffin <= timeBeforeUseParaffin)
              TimeDelta = 0;
           else
           {
              TimeDelta = RemainingTimeMeltParaffin - timeBeforeUseParaffin;
              bCanotRun = true;
           }
        }
        else
            TimeDelta = 0;  //Paraffin is Melted
    }

    return (TimeActual + TimeDelta);//seconds
}

void CDashboardWidget::OnActivated(int index)
{
    qDebug() << "Index Recieved : " << index;
    if(-1 != index) {
        m_NewSelectedProgramId = m_FavProgramIDs.at(index);
        CDashboardDateTimeWidget::SELECTED_PROGRAM_NAME = mp_ProgramList->GetProgram(m_NewSelectedProgramId)->GetName();
        PrepareSelectedProgramChecking();
    } else {
        mp_Ui->pgmsComboBox->showPopup();
    }
}

void CDashboardWidget::OnComBoxButtonPress()
{
    const DataManager::CProgram* pProgram = mp_ProgramList->GetProgram(m_NewSelectedProgramId);
    if (pProgram)
    {
        QList<QString> stationNameList;
        for (int i = 0; i < m_StationList.count(); i++)
        {
            DataManager::CDashboardStation* pStation = mp_DataConnector->DashboardStationList->GetDashboardStation(m_StationList.at(i));
            if (!pStation)
            {
                Q_ASSERT(0);
            }
            stationNameList.append(pStation->GetDashboardStationName());
        }

        mp_ProgramStatusWidget->InitDialog(const_cast<DataManager::CProgram*>(pProgram), mp_DataConnector,
                                           stationNameList, m_CurProgramStepIndex,
                                           mp_DashboardScene->GetStepRemainingTime(),
                                           mp_DashboardScene->GetProgramRemainingTime(),
                                           mp_DashboardScene->GetEndDateTime());
        mp_ProgramStatusWidget->move(80,50);
        mp_ProgramStatusWidget->show();
    }
}

const QString& CDashboardWidget::SelectedProgramId()
{
    return m_SelectedProgramId;
}

void CDashboardWidget::PrepareSelectedProgramChecking()
{
    this->IsParaffinInProgram(mp_ProgramList->GetProgram(m_NewSelectedProgramId));//to get m_ParaffinStepIndex
    //Notify Master, to get the time costed for paraffin Melting
    mp_DataConnector->SendProgramSelected(m_NewSelectedProgramId, m_ParaffinStepIndex);

}

void CDashboardWidget::CheckPreConditionsToRunProgram()
{
    if ("" == m_SelectedProgramId)
        return;


    //Check cleaning program run in last time?
    bool isCleaningProgramRun = true; // get this from a log file.
    if (!isCleaningProgramRun)
    {
        mp_MessageDlg->SetIcon(QMessageBox::Information);
        mp_MessageDlg->SetTitle(tr("Information"));
        mp_MessageDlg->SetText(tr("Found cleaning program did not run in last time."));
        mp_MessageDlg->SetButtonText(1, tr("OK"));
        mp_MessageDlg->HideButtons();
        if (mp_MessageDlg->exec())
        {
            TakeOutSpecimenAndRunCleaning();
        }
        return;
     }

    //Check if Leica program and RMS OFF?
    DataManager::CHimalayaUserSettings* userSetting = mp_DataConnector->SettingsInterface->GetUserSettings();
    bool isRMSOFF = false;
    if (Global::RMS_OFF == userSetting->GetModeRMSCleaning() || Global::RMS_OFF == userSetting->GetModeRMSProcessing())
    {
        isRMSOFF = true;
    }

    if (isRMSOFF && mp_ProgramList->GetProgram(m_SelectedProgramId)->IsLeicaProgram())
    {
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        mp_MessageDlg->SetTitle(tr("Warning"));
        mp_MessageDlg->SetText(tr("Can not start Leica Program With RMS OFF, please trun on RMS."));
        mp_MessageDlg->SetButtonText(1, tr("OK"));
        mp_MessageDlg->HideButtons();
        if (mp_MessageDlg->exec())
        return;
    }

    //Check Expired?
    if (!isRMSOFF && mp_DashboardScene->HaveExpiredReagent())
    {
        if (m_CurrentUserRole == MainMenu::CMainWindow::Operator)
        {
            mp_MessageDlg->SetIcon(QMessageBox::Warning);
            mp_MessageDlg->SetTitle(tr("Warning"));
            mp_MessageDlg->SetText(tr("Some expired reagents are used in this selected program, you can not start this program."));
            mp_MessageDlg->SetButtonText(1, tr("OK"));
            mp_MessageDlg->HideButtons();
            if (mp_MessageDlg->exec())
            return;
        }
        else
        if(m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
        m_CurrentUserRole == MainMenu::CMainWindow::Service)
        {
            mp_MessageDlg->SetIcon(QMessageBox::Warning);
            mp_MessageDlg->SetTitle(tr("Warning"));
            mp_MessageDlg->SetText(tr("Do you want to Start the Program with Expired Reagents?"));
            mp_MessageDlg->SetButtonText(3, tr("Yes"));
            mp_MessageDlg->SetButtonText(1, tr("No"));
            mp_MessageDlg->HideCenterButton();    // Hiding First Two Buttons in the Message Dialog

            if (!mp_MessageDlg->exec())
            return;
        }
    }

    //check End Datetime again
    m_NewSelectedProgramId = m_SelectedProgramId;
    m_CheckEndDatetimeAgain = true;
    PrepareSelectedProgramChecking();
}

bool CDashboardWidget::CheckPreConditionsToPauseProgram()
{
    return true;

}


bool CDashboardWidget::CheckPreConditionsToAbortProgram()
{
    MainMenu::CMessageDlg ConfirmationMessageDlg;

    ConfirmationMessageDlg.SetTitle(tr("Confirmation Message"));
    ConfirmationMessageDlg.SetText(tr("Do you want to abort the program?"));
    ConfirmationMessageDlg.SetIcon(QMessageBox::Information);
    ConfirmationMessageDlg.SetButtonText(1, tr("Yes"));
    ConfirmationMessageDlg.SetButtonText(3, tr("Cancel"));
    ConfirmationMessageDlg.HideCenterButton();

    return ConfirmationMessageDlg.exec() == (int)QDialog::Accepted;
}

//Need the gray button!
void CDashboardWidget::EnablePlayButton(bool bSetEnable)
{
    mp_Ui->playButton->setEnabled(bSetEnable);
}

void CDashboardWidget::OnProgramStartReadyUpdated()
{
    this->EnablePlayButton(true);
}

void CDashboardWidget::OnProgramWillComplete()
{
    mp_MessageDlg->SetIcon(QMessageBox::Warning);
    mp_MessageDlg->SetTitle(tr("Warning"));
    QString strTemp(tr("Program \""));
    strTemp = strTemp + CDashboardDateTimeWidget::SELECTED_PROGRAM_NAME
            + tr("\" is complete! Would you like to drain the retort?");
    mp_MessageDlg->SetText(strTemp);
    mp_MessageDlg->SetButtonText(1, tr("OK"));
    mp_MessageDlg->HideButtons();

    if (mp_MessageDlg->exec())
    {
        m_IsDraining = true;
        mp_DataConnector->SendProgramAction(m_SelectedProgramId, DataManager::PROGRAM_DRAIN);
        //disable pause and abort
        mp_Ui->playButton->setEnabled(false);
        mp_Ui->abortButton->setEnabled(false);
        return;
    }
}


void CDashboardWidget::TakeOutSpecimenAndRunCleaning()
{
    mp_DataConnector->SendRetortLock(false);

    mp_MessageDlg->SetIcon(QMessageBox::Warning);
    mp_MessageDlg->SetTitle(tr("Warning"));
    mp_MessageDlg->SetText(tr("Please take out your specimen!"));
    mp_MessageDlg->SetButtonText(1, tr("OK"));
    mp_MessageDlg->HideButtons();
    if (mp_MessageDlg->exec())
    {
        //represent the retort as contaminated status
        mp_DashboardScene->UpdateRetortStatus(DataManager::CONTAINER_STATUS_CONTAMINATED);

        mp_MessageDlg->SetText(tr("The retort is contaminated, Cleaning Program will run! Please lock the retort then click \"OK\"."));
        mp_MessageDlg->SetButtonText(1, tr("OK"));
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->EnableButton(1, false);
        m_IsWaitingCleaningProgram = true;
        if (mp_MessageDlg->exec())
        {
            m_IsWaitingCleaningProgram = false;
            m_ForceRunCleanProgram = true;
            m_NewSelectedProgramId = "C01";
            CDashboardDateTimeWidget::SELECTED_PROGRAM_NAME = tr("Cleaning Program");
            PrepareSelectedProgramChecking();

            //enable pause and abort
            mp_Ui->playButton->setEnabled(true);
            mp_Ui->abortButton->setEnabled(true);
        }
    }
}

void CDashboardWidget::OnProgramBeginAbort()
{
    //show "aborting"
    //time countdown
    //Todo:20, Abort time, will be given a rough value later;
    emit ProgramActionStarted(DataManager::PROGRAM_ABORT, 20, Global::AdjustedTime::Instance().GetCurrentDateTime(), false);
}

void CDashboardWidget::OnProgramAborted()
{
    //progress aborted;
    //aborting time countdown is hidden.
    m_IsResumeRun = false;
    m_CurProgramStepIndex = -1;
    mp_Ui->pgmsComboBox->WorkAsButton(false);

    emit ProgramActionStopped(DataManager::PROGRAM_STATUS_ABORTED);

    mp_MessageDlg->SetIcon(QMessageBox::Warning);
    mp_MessageDlg->SetTitle(tr("Warning"));
    QString strTemp(tr("Program \""));
    strTemp = strTemp + CDashboardDateTimeWidget::SELECTED_PROGRAM_NAME
            + tr("\" is aborted! Would you like to start a new Program?");
    mp_MessageDlg->SetText(strTemp);
    mp_MessageDlg->SetButtonText(1, tr("Yes"));
    mp_MessageDlg->SetButtonText(3, tr("No"));
    mp_MessageDlg->HideCenterButton();
    if (mp_MessageDlg->exec())//yes
    {
        //switch to the dashboard page
        mp_MainWindow->SetTabWidgetIndex();
        //show the comboBox
        mp_Ui->pgmsComboBox->showPopup();
        //disable "Start" button and "Abort" button, enable Retort lock button, hide End time button
        mp_Ui->playButton->setEnabled(false);
        mp_Ui->abortButton->setEnabled(false);
        mp_Ui->retortSlider->setEnabled(true);
    }
    else//no
    {
        this->TakeOutSpecimenAndRunCleaning();
    }
}

void CDashboardWidget::OnProgramCompleted()
{
    m_IsResumeRun = false;
    m_CurProgramStepIndex = -1;
    mp_Ui->pgmsComboBox->WorkAsButton(false);
    emit ProgramActionStopped(DataManager::PROGRAM_STATUS_COMPLETED);
}

void CDashboardWidget::OnProgramRunBegin()
{
    emit ProgramActionStarted(DataManager::PROGRAM_START, m_TimeProposed, Global::AdjustedTime::Instance().GetCurrentDateTime(), m_IsResumeRun);
    m_IsResumeRun = true;
    mp_Ui->pgmsComboBox->WorkAsButton(true);
}

void CDashboardWidget::OnProcessStateChanged()
{
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if (m_ProcessRunning)
    {
        mp_Ui->retortSlider->setEnabled(false);
    }
    else
    {
        mp_Ui->retortSlider->setEnabled(true);
        mp_DashboardScene->OnPauseStationSuckDrain();
    }
}

void CDashboardWidget::OnRetortLockStatusChanged(const MsgClasses::CmdRetortLockStatus& cmd)
{
    if (cmd.IsLocked())
        mp_Ui->retortSlider->SetPosition(MainMenu::CSliderControl::PosLeft);
    else
    {
        mp_Ui->retortSlider->SetPosition(MainMenu::CSliderControl::PosRight);

        //enable the "OK"
        if (m_IsWaitingCleaningProgram && mp_MessageDlg->isVisible())
        {
            mp_MessageDlg->EnableButton(1, true);
        }
    }
}

void CDashboardWidget::OnProgramSelectedReply(const MsgClasses::CmdProgramSelectedReply& cmd)
{
    if (m_CheckEndDatetimeAgain)
    {
        m_CheckEndDatetimeAgain = false;
        //get the proposed program end DateTime
        bool bCanotRun = true;
        int asapEndTime = GetASAPTime(mp_ProgramList->GetProgram(m_SelectedProgramId), cmd.TimeProposed(),
                                      cmd.ParaffinMeltCostedTime(), cmd.CostedTimeBeforeParaffin(), bCanotRun);
        if (bCanotRun)
        {
            mp_MessageDlg->SetIcon(QMessageBox::Warning);
            mp_MessageDlg->SetTitle(tr("Warning"));
            mp_MessageDlg->SetText(tr("Still it will cost some time to melt paraffin, the current selected program can not run now."));
            mp_MessageDlg->SetButtonText(1, tr("OK"));
            mp_MessageDlg->HideButtons();
            if (mp_MessageDlg->exec())
            {
                return;
            }
            return;
        }
        asapEndTime = asapEndTime - 60;//60 seconds: buffer time for "select program" and "Run" operation.
        QDateTime endDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(asapEndTime);
        if (endDateTime > m_EndDateTime)
        {
            mp_MessageDlg->SetIcon(QMessageBox::Warning);
            mp_MessageDlg->SetTitle(tr("Warning"));
            mp_MessageDlg->SetText(tr("Please re-set the End Date&Time of the current selected program."));
            mp_MessageDlg->SetButtonText(1, tr("OK"));
            mp_MessageDlg->HideButtons();
            if (mp_MessageDlg->exec())
            {
                return;
            }
            return;
        }

        if ( mp_DataConnector)
        {
            //input cassette number
            if (Global::RMS_CASSETTES == mp_DataConnector->SettingsInterface->GetUserSettings()->GetModeRMSProcessing())
            {
                CCassetteNumberInputWidget *pCassetteInput = new Dashboard::CCassetteNumberInputWidget();
                pCassetteInput->setWindowFlags(Qt::CustomizeWindowHint);
                pCassetteInput->move(80,50);
                pCassetteInput->SetDialogTitle(tr("Please set numbers of cassettes:"));
                pCassetteInput->exec();

                int cassetteNumber = pCassetteInput->CassetteNumber();
                if (-1 == cassetteNumber)
                    return;//clicked Cancel button

                mp_DataConnector->SendKeepCassetteCount(cassetteNumber);
                delete pCassetteInput;
            }
        }
        mp_DataConnector->SendProgramAction(m_SelectedProgramId, DataManager::PROGRAM_START, m_EndDateTime);
        mp_Ui->playButton->setIcon(QIcon(":/HimalayaImages/Icons/Dashboard/Operation/Operation_Pause.png"));
        m_ProgramNextAction = DataManager::PROGRAM_PAUSE;
        return;
    }

    //firstly check whether there is any station not existing for some program steps
    const QList<QString>& stationList = cmd.StationList();
    for (int i = 0; i < stationList.count(); i++)
    {
        if ("" == stationList.at(i))
        {
            mp_MessageDlg->SetIcon(QMessageBox::Warning);
            mp_MessageDlg->SetTitle(tr("Warning"));
            QString strTemp(tr("Program step \""));
            strTemp = strTemp + QString::number(i+1);
            strTemp = strTemp + tr("\" of \"");
            strTemp = strTemp + CDashboardDateTimeWidget::SELECTED_PROGRAM_NAME
                    + tr("\" can not find the corresponding reagent station, one station only can be used once in the program, please set a station for the reagent in this step.");
            mp_MessageDlg->SetText(strTemp);
            mp_MessageDlg->SetButtonText(1, tr("OK"));
            mp_MessageDlg->HideButtons();

            if (mp_MessageDlg->exec())
            {
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
                mp_MessageDlg->SetTitle(tr("Warning"));
                QString strTemp(tr("The Station \""));
                strTemp = strTemp + pStation->GetDashboardStationName();
                strTemp = strTemp + tr("\" status is set as Empty in Program step \"");
                strTemp = strTemp + QString::number(i+1);
                strTemp = strTemp + tr("\" of \"");
                strTemp = strTemp + CDashboardDateTimeWidget::SELECTED_PROGRAM_NAME
                        + tr("\" , it can not be executed.");
                mp_MessageDlg->SetText(strTemp);
                mp_MessageDlg->SetButtonText(1, tr("OK"));
                mp_MessageDlg->HideButtons();

                if (mp_MessageDlg->exec())
                {
                        return;
                }
                return;
            }
        }
    }

    m_SelectedProgramId = m_NewSelectedProgramId;
    m_StationList.clear();
    m_StationList = stationList;
    //Show program name in the comboBox
    mp_Ui->pgmsComboBox->UpdateSelectedProgramName(CDashboardDateTimeWidget::SELECTED_PROGRAM_NAME);
    //get the proposed program end DateTime
    bool bCanotRun = true;
    int asapEndTime = GetASAPTime(mp_ProgramList->GetProgram(m_SelectedProgramId), cmd.TimeProposed(),
                                  cmd.ParaffinMeltCostedTime(), cmd.CostedTimeBeforeParaffin(), bCanotRun);
    m_TimeProposed = cmd.TimeProposed();
    m_EndDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(asapEndTime);

    emit ProgramSelected(m_SelectedProgramId, asapEndTime, m_StationList);//for UI update

    if (m_ForceRunCleanProgram)//for after program completed
    {
        mp_DataConnector->SendProgramAction(m_SelectedProgramId, DataManager::PROGRAM_START, m_EndDateTime);
        m_ForceRunCleanProgram = false;
    }
}

void CDashboardWidget::OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor & cmd)
{
    m_CurProgramStepIndex = cmd.CurProgramStepIndex();
    if (m_CurProgramStepIndex > 2 && m_CurrentUserRole == MainMenu::CMainWindow::Operator)
    {
        this->EnablePlayButton(false);//disable pause button when runing
    }
}

void CDashboardWidget::OnStationSuckDrain(const MsgClasses::CmdStationSuckDrain & cmd)
{
    if (m_IsDraining && !cmd.IsStart() && !cmd.IsSuck())
    {
        this->TakeOutSpecimenAndRunCleaning();
        m_IsDraining = false;//when abort or pause, set this too?
    }
    mp_DashboardScene->OnStationSuckDrain(cmd.StationID(), cmd.IsStart(), cmd.IsSuck());
}

} // End of namespace Dashboard
