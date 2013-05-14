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


namespace Dashboard {

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
                                   m_IsDraining(false)
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

     CONNECTSIGNALSIGNAL(this, ProgramActionStopped(DataManager::ProgramActionType_t),
                         mp_DashboardScene, ProgramActionStopped(DataManager::ProgramActionType_t));

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

     CONNECTSIGNALSLOT(this, ProgramSelected(QString&, int),
                        mp_DashboardScene, UpdateDashboardSceneReagentsForProgram(QString &, int));

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

}


void CDashboardWidget::OnButtonClicked(int whichBtn)
{
    if ( whichBtn == Dashboard::firstButton ) {
        switch(m_ProgramNextAction)
        {
            case DataManager::PROGRAM_START:
            {
                if (CheckPreConditionsToRunProgram()) {
                    OnProgramStartConfirmation();
                    mp_Ui->playButton->setIcon(QIcon(":/HimalayaImages/Icons/Dashboard/Operation/Operation_Pause.png"));
                    m_ProgramNextAction = DataManager::PROGRAM_PAUSE;
                } else {
                    // Take Necessary Action
                }
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

void CDashboardWidget::OnRMSValueChanged(Global::RMSOptions_t state)
{
    m_RMSState = state;
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
                                  int TimeCostedParaffinWelting)
{
    //IsParaffinInProgram()
    //Yes
      //calculate the timeBeforeUseParraffin
      //RemainingTimeWeltParraffin = 12 hour - TimeCosted
      //if RemainingTimeWeltParraffin > 0
        //if RemainingTimeWeltParraffin <= timeBeforeUseParraffin , TDelta = 0;
            //else TDelta = RemainingTimeWeltParraffin - timeBeforeUseParraffin
    //if RemainingTimeWeltParraffin <= 0 Paraffin is welted, TDelta = 0;

    //No Paraffin TimeDelta = 0;

    int TimeDelta; //seconds
    if (-1 == m_ParaffinStepIndex)//No Paraffin in all program steps
    {
        TimeDelta = 0;
    }
    else
    {
        //calculate the timeBeforeUseParraffin
        //RemainingTimeWeltParraffin = 12 hour - TimeCosted
        int RemainingTimeWeltParaffin = 12 * 60 * 60 - TimeCostedParaffinWelting;
        if (RemainingTimeWeltParaffin > 0)
        {
          if (RemainingTimeWeltParaffin <= timeBeforeUseParaffin)
              TimeDelta = 0;
           else
              TimeDelta = RemainingTimeWeltParaffin - timeBeforeUseParaffin;
        }
        else
            TimeDelta = 0;  //Paraffin is welted
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

void CDashboardWidget::PrepareSelectedProgramChecking()
{
    this->IsParaffinInProgram(mp_ProgramList->GetProgram(m_NewSelectedProgramId));//to get m_ParaffinStepIndex
    //Notify Master, to get the time costed for paraffin welting
    mp_DataConnector->SendProgramSelected(m_NewSelectedProgramId, m_ParaffinStepIndex);

}

bool CDashboardWidget::CheckPreConditionsToRunProgram()
{
    if ("" == m_SelectedProgramId)
        return false;

    if (m_IsResumeRun)
        return true;

    //Todo: We should give the Expired conditions
   /* if((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
        m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
            Global::RMS_OFF != m_RMSState)
    {*/
        qDebug() << "RMS IS ON && User = Admin | Service";
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        mp_MessageDlg->SetTitle(tr("Warning"));
        mp_MessageDlg->SetText(tr("Do you want to Start the Program with Expired Reagents."));
        mp_MessageDlg->SetButtonText(3, tr("Cancel"));
        mp_MessageDlg->SetButtonText(1, tr("OK"));
        mp_MessageDlg->HideCenterButton();    // Hiding First Two Buttons in the Message Dialog

        if (mp_MessageDlg->exec())
        {
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
                return false;
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
                        return false;//clicked Cancel button

                    mp_DataConnector->SendKeepCassetteCount(cassetteNumber);
                    delete pCassetteInput;
                }
            }
            return true;
        }
        else
            return false;

    //}

    //return false;

}

bool CDashboardWidget::CheckPreConditionsToPauseProgram()
{
    return true;

}


bool CDashboardWidget::CheckPreConditionsToAbortProgram()
{
    return true;

}

void CDashboardWidget::OnProgramStartConfirmation()
{
    qDebug() << " On Confirmation";
    // Send Command to Master
    mp_DataConnector->SendProgramAction(m_SelectedProgramId, DataManager::PROGRAM_START, m_EndDateTime);
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
            + "\" is complete! Would you like to drain the retort?";
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
    mp_Ui->pgmsComboBox->WorkAsButton(false);

    emit ProgramActionStopped(DataManager::PROGRAM_ABORT);

    mp_MessageDlg->SetIcon(QMessageBox::Warning);
    mp_MessageDlg->SetTitle(tr("Warning"));
    QString strTemp(tr("Program \""));
    strTemp = strTemp + CDashboardDateTimeWidget::SELECTED_PROGRAM_NAME
            + "\" is aborted! Would you like to start a new Program?";
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
    mp_Ui->pgmsComboBox->WorkAsButton(false);
    emit ProgramActionStopped(DataManager::PROGRAM_START);
}

void CDashboardWidget::OnProgramRunBegin()
{
    emit ProgramActionStarted(DataManager::PROGRAM_START, m_TimeProposed, Global::AdjustedTime::Instance().GetCurrentDateTime(), m_IsResumeRun);
    m_IsResumeRun = true;
    mp_Ui->pgmsComboBox->WorkAsButton(true);
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
    //firstly check whether there is any empty station for some program steps
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
                    + tr("\" can not find the corresponding reagent station, please set a station for the reagent in this step.");
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
    int asapEndTime = GetASAPTime(mp_ProgramList->GetProgram(m_SelectedProgramId), cmd.TimeProposed(),
                                  cmd.ParaffinWeltCostedTime(), cmd.CostedTimeBeforeParaffin());
    m_TimeProposed = cmd.TimeProposed();
    m_EndDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(asapEndTime);
    emit ProgramSelected(m_SelectedProgramId, asapEndTime);//for UI update

    if (m_ForceRunCleanProgram)//for after program completed
    {
        mp_DataConnector->SendProgramAction(m_SelectedProgramId, DataManager::PROGRAM_START, m_EndDateTime);
        m_ForceRunCleanProgram = false;
    }
}

void CDashboardWidget::OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor & cmd)
{
    m_CurProgramStepIndex = cmd.CurProgramStepIndex();
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
