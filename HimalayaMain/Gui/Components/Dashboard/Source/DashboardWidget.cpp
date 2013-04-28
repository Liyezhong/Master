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

namespace Dashboard {

CDashboardWidget::CDashboardWidget(Core::CDataConnector *p_DataConnector,
                                   MainMenu::CMainWindow *p_Parent): MainMenu::CPanelFrame(p_Parent),
                                   mp_Ui(new Ui::CDashboardWidget),mp_MainWindow(p_Parent),
                                   mp_DataConnector(p_DataConnector),
                                   m_ProgramNextAction(DataManager::PROGRAM_START),
                                   m_UserRoleChanged(false),
                                   m_asapEndTime(0),
                                   m_ParaffinStepIndex(-1),
                                   m_IsWaitingCleaningProgram(false),
                                   m_ForceRunCleanProgram(false)
{
     mp_Ui->setupUi(GetContentFrame());
     SetPanelTitle(tr("Dashboard"));

     QPalette Palette = mp_Ui->dashboardView->palette();
     QColor BaseColor = Palette.color(QPalette::Base);
     BaseColor.setAlpha(0);
     Palette.setColor(QPalette::Base, BaseColor);
     mp_Ui->dashboardView->setPalette(Palette);

     mp_DashboardScene = new CDashboardScene(mp_DataConnector, this, mp_MainWindow);
     mp_Ui->dashboardView->setScene(mp_DashboardScene);
     CONNECTSIGNALSLOT(mp_DashboardScene, OnSelectDateTime(const QDateTime &), this, OnSelectDateTime(const QDateTime&));

     mp_Separator = new QFrame();
     mp_Separator->setParent(this);  // Set Parent of this Frame as the Dashboard Widget.
     DrawSeparatorLine();

     mp_ProgramList = mp_DataConnector->ProgramList;

//     mp_ComboBoxModel = new Dashboard::CDashboardComboBoxModel();
//     mp_ComboBoxModel->SetProgramListContainer(mp_ProgramList);
//     mp_ComboBoxModel->ResetAndUpdateModel();
//     mp_Ui->pgmsComboBox->setModel(mp_ComboBoxModel);

     m_btnGroup.addButton(mp_Ui->playButton, Dashboard::firstButton);
     m_btnGroup.addButton(mp_Ui->abortButton, Dashboard::secondButton);
     EnablePlayButton(false);
     m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
     mp_MessageDlg = new MainMenu::CMessageDlg();

     CONNECTSIGNALSLOT(mp_MainWindow, UserRoleChanged(), this, OnUserRoleChanged());
     CONNECTSIGNALSLOT(mp_Ui->pgmsComboBox, activated(int), this, OnActivated(int));

     CONNECTSIGNALSLOT(&m_btnGroup, buttonClicked(int), this, OnButtonClicked(int));

     CONNECTSIGNALSLOT(this, ProgramSelected(QString&, int),
                        mp_DashboardScene, UpdateDashboardSceneReagentsForProgram(QString &, int));

     CONNECTSIGNALSLOT(mp_DataConnector, ProgramStartReady(),
                       this, OnProgramStartReadyUpdated());

     CONNECTSIGNALSLOT(mp_DataConnector, ProgramWillComplete(),
                       this, OnProgramWillComplete());

     CONNECTSIGNALSLOT(mp_DataConnector, ProgramDrainFinished(),
                       this, OnDrainFinished());

     CONNECTSIGNALSLOT(mp_DataConnector, RetortLockStatusChanged(const MsgClasses::CmdRetortLockStatus &),
                       this, OnRetortLockStatusChanged(const MsgClasses::CmdRetortLockStatus&));

     CONNECTSIGNALSLOT(mp_Ui->retortSlider, positionChanged(MainMenu::CSliderControl::Position_t),
                       this, RetortSliderPositionChanged(MainMenu::CSliderControl::Position_t));

     CONNECTSIGNALSLOT(mp_DataConnector, ProgramSelectedReply(const MsgClasses::CmdProgramSelectedReply &),
                       this, OnProgramSelectedReply(const MsgClasses::CmdProgramSelectedReply&));
}

CDashboardWidget::~CDashboardWidget()
{
    try {
        delete mp_Separator;
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
                    m_ProgramCurrentAction = DataManager::PROGRAM_PAUSE;
                    mp_DataConnector->SendProgramAction(m_SelectedProgramId, m_ProgramCurrentAction);
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
            m_ProgramCurrentAction = DataManager::PROGRAM_ABORT;
            mp_DataConnector->SendProgramAction(m_SelectedProgramId, m_ProgramCurrentAction);

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
    m_ProgramCurrentAction = DataManager::PROGRAM_START;
    mp_DataConnector->SendProgramAction(m_SelectedProgramId, m_ProgramCurrentAction, m_EndDateTime);

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
        m_ProgramCurrentAction = DataManager::PROGRAM_DRAIN;
        mp_DataConnector->SendProgramAction(m_SelectedProgramId, m_ProgramCurrentAction);
        //disable pause and abort
        mp_Ui->playButton->setEnabled(false);
        mp_Ui->abortButton->setEnabled(false);
        return;
    }
}

void CDashboardWidget::OnDrainFinished()
{
    mp_MessageDlg->SetIcon(QMessageBox::Warning);
    mp_MessageDlg->SetTitle(tr("Warning"));
    mp_MessageDlg->SetText(tr("Please take out your specimen!"));
    mp_MessageDlg->SetButtonText(1, tr("OK"));
    mp_MessageDlg->HideButtons();

    if (mp_MessageDlg->exec())
    {
        //todo: need send unlock?
        mp_MessageDlg->SetText(tr("The retort is contaminated, Cleaning Program will run!"));

        mp_MessageDlg->SetButtonText(1, tr("OK"));
        mp_MessageDlg->HideButtons();
        m_IsWaitingCleaningProgram = true;
        if (mp_MessageDlg->exec())
        {
            m_IsWaitingCleaningProgram = false;
            m_ProgramCurrentAction = DataManager::PROGRAM_START;
            m_ForceRunCleanProgram = true;
            m_NewSelectedProgramId = "C01";
            CDashboardDateTimeWidget::SELECTED_PROGRAM_NAME = tr("Cleaning Program");
            PrepareSelectedProgramChecking();

            //enable pause and abort
            //mp_Ui->playButton->setEnabled(true);
            //mp_Ui->abortButton->setEnabled(true);
        }
        return;
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
            //mp_MessageDlg->
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
            strTemp = strTemp + "\" of \"";
            strTemp = strTemp + CDashboardDateTimeWidget::SELECTED_PROGRAM_NAME
                    + "\" can not find the corresponding reagent station, please set a station for the reagent in this step.";
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

    m_SelectedProgramId = m_NewSelectedProgramId;
    //Show program name in the comboBox
    mp_Ui->pgmsComboBox->UpdateSelectedProgramName(CDashboardDateTimeWidget::SELECTED_PROGRAM_NAME);
    //get the proposed program end DateTime
    int asapEndTime = GetASAPTime(mp_ProgramList->GetProgram(m_SelectedProgramId), cmd.TimeProposed(),
                                  cmd.ParaffinWeltCostedTime(), cmd.CostedTimeBeforeParaffin());
    m_EndDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(asapEndTime);
    emit ProgramSelected(m_SelectedProgramId, asapEndTime);//for UI update

    if (m_ForceRunCleanProgram)//for after program completed
    {
        m_ProgramCurrentAction = DataManager::PROGRAM_START;
        mp_DataConnector->SendProgramAction(m_SelectedProgramId, m_ProgramCurrentAction, m_EndDateTime);
        m_ForceRunCleanProgram = false;
    }
}

} // End of namespace Dashboard
