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
#include "Dashboard/Include/CommonString.h"

namespace Dashboard {

QString CDashboardWidget::m_SelectedProgramId = "";
QString CDashboardWidget::m_strWarning = CDashboardWidget::tr("Warning");
QString CDashboardWidget::m_strYes = CDashboardWidget::tr("Yes");
QString CDashboardWidget::m_strCancel = CDashboardWidget::tr("Cancel");
QString CDashboardWidget::m_strMsgUnselect = CDashboardWidget::tr("As the program \"%1\" is selected, this operation will result in an incorrect program result, if you click \"Yes\", the selected program will unselect.");

CDashboardWidget::CDashboardWidget(Core::CDataConnector *p_DataConnector,
                                   MainMenu::CMainWindow *p_Parent): MainMenu::CPanelFrame(p_Parent),
                                   mp_Ui(new Ui::CDashboardWidget),mp_MainWindow(p_Parent),
                                   mp_DataConnector(p_DataConnector),
                                   m_ProgramNextAction(DataManager::PROGRAM_START),
                                   m_UserRoleChanged(false),
                                   m_TimeProposed(0),
                                   m_ParaffinStepIndex(-1),
                                   m_IsWaitingCleaningProgram(false),
                                   m_IsResumeRun(false),
                                   m_CurProgramStepIndex(-1),
                                   m_IsDraining(false),
                                   m_CheckEndDatetimeAgain(false),
                                   m_ProcessRunning(false),
                                   m_ProgramStartReady(false),
                                   m_strProgram(tr("Program")),
                                   m_strInformation(tr("Information")),
                                   m_strCleanNotRun(tr("Found the cleaning program did not run in last time.")),
                                   m_strOK(tr("OK")),
                                   m_strNo(tr("No")),
                                   m_strConfirmation(tr("Confirmation Message")),
                                   m_strProgramComplete(tr("Program \"%1\" is complete! Would you like to drain the retort?")),
                                   m_strTakeOutSpecimen(tr("Please take out your specimen!")),
                                   m_strNeedMeltParaffin(tr("Still it will cost some time to melt paraffin, the current selected program can not run now.")),
                                   m_strResetEndTime(tr("Please re-set the End Date&Time of the current selected program.")),
                                   m_strNotFoundStation(tr("Program step \"%1\" of \"%2\" can not find the corresponding reagent station, one station only can be used once in the program, please set a station for the reagent in this step.")),
                                   m_strCheckEmptyStation(tr("The Station \"%1\" status is set as Empty in Program step \"%2\" of \"%3\", it can not be executed."))
{
     mp_Ui->setupUi(GetContentFrame());
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

     m_pUserSetting = mp_DataConnector->SettingsInterface->GetUserSettings();

     mp_ProgramList = mp_DataConnector->ProgramList;

     m_btnGroup.addButton(mp_Ui->playButton, Dashboard::firstButton);
     m_btnGroup.addButton(mp_Ui->abortButton, Dashboard::secondButton);

     EnableAbortButton(false);
     EnablePlayButton(false);

     m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
     mp_MessageDlg = new MainMenu::CMessageDlg();

     CONNECTSIGNALSLOT(mp_MainWindow, UserRoleChanged(), this, OnUserRoleChanged());
     CONNECTSIGNALSLOT(mp_Ui->pgmsComboBox, activated(int), this, OnActivated(int));
     CONNECTSIGNALSLOT(mp_Ui->pgmsComboBox, ButtonPress(), this, OnComboBoxButtonPress());

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

     CONNECTSIGNALSLOT(mp_DataConnector, ProgramSelectedReply(const MsgClasses::CmdProgramSelectedReply &),
                       this, OnProgramSelectedReply(const MsgClasses::CmdProgramSelectedReply&));

     CONNECTSIGNALSLOT(mp_DataConnector, CurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor &),
                       this, OnCurrentProgramStepInforUpdated(const MsgClasses::CmdCurrentProgramStepInfor &));

     CONNECTSIGNALSLOT(mp_DataConnector, StationSuckDrain(const MsgClasses::CmdStationSuckDrain &),
                       this, OnStationSuckDrain(const MsgClasses::CmdStationSuckDrain &));
     CONNECTSIGNALSLOT(this, UpdateUserSetting(DataManager::CUserSettings&), mp_DataConnector, SendUpdatedSettings(DataManager::CUserSettings&));
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

void CDashboardWidget::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
        case QEvent::LanguageChange:
            mp_Ui->retranslateUi(this);
            CommonString::RetranslateUIString();
            this->RetranslateUI();
            break;
        default:
            break;
    }
}

void CDashboardWidget::RetranslateUI()
{
    SetPanelTitle(QApplication::translate("Dashboard::CDashboardWidget", "Dashboard", 0, QApplication::UnicodeUTF8));
    m_strYes = QApplication::translate("Dashboard::CDashboardWidget", "Yes", 0, QApplication::UnicodeUTF8);
    m_strNo = QApplication::translate("Dashboard::CDashboardWidget", "No", 0, QApplication::UnicodeUTF8);
    m_strOK = QApplication::translate("Dashboard::CDashboardWidget", "OK", 0, QApplication::UnicodeUTF8);
    m_strCancel = QApplication::translate("Dashboard::CDashboardWidget", "Cancel", 0, QApplication::UnicodeUTF8);
    m_strWarning = QApplication::translate("Dashboard::CDashboardWidget", "Warning", 0, QApplication::UnicodeUTF8);
    m_strMsgUnselect = QApplication::translate("Dashboard::CDashboardWidget", "As the program \"%1\" is selected, this operation will result in an incorrect program result, if you click \"Yes\", the selected program will unselect.", 0, QApplication::UnicodeUTF8);
    m_strProgram = QApplication::translate("Dashboard::CDashboardWidget", "Program", 0, QApplication::UnicodeUTF8);
    m_strInformation = QApplication::translate("Dashboard::CDashboardWidget", "Information", 0, QApplication::UnicodeUTF8);
    m_strCleanNotRun = QApplication::translate("Dashboard::CDashboardWidget", "Found the cleaning program did not run in last time.", 0, QApplication::UnicodeUTF8);
    m_strNotStartRMSOFF = QApplication::translate("Dashboard::CDashboardWidget", "Leica Program can't be operated with RMS OFF.", 0, QApplication::UnicodeUTF8);
    m_strNotStartExpiredReagent = QApplication::translate("Dashboard::CDashboardWidget", "Reagents needed for this program are expired! You can't operate this program.", 0, QApplication::UnicodeUTF8);
    m_strStartExpiredReagent =  QApplication::translate("Dashboard::CDashboardWidget", "Do you want to Start the Program with Expired Reagents?", 0, QApplication::UnicodeUTF8);
    m_strConfirmation = QApplication::translate("Dashboard::CDashboardWidget", "Confirmation Message", 0, QApplication::UnicodeUTF8);
    m_strAbortProgram = QApplication::translate("Dashboard::CDashboardWidget", "Do you want to abort the program?", 0, QApplication::UnicodeUTF8);
    m_strProgramComplete = QApplication::translate("Dashboard::CDashboardWidget", "Program \"%1\" is complete! Would you like to drain the retort?", 0, QApplication::UnicodeUTF8);
    m_strTakeOutSpecimen = QApplication::translate("Dashboard::CDashboardWidget", "Please take out your specimen!", 0, QApplication::UnicodeUTF8);
    m_strRetortContaminated  = QApplication::translate("Dashboard::CDashboardWidget", "The retort is contaminated, please lock the retort and select Cleaning Program to run!", 0, QApplication::UnicodeUTF8);
    m_strProgramIsAborted  = QApplication::translate("Dashboard::CDashboardWidget", "Program \"%1\" is aborted!", 0, QApplication::UnicodeUTF8);
    m_strNeedMeltParaffin  = QApplication::translate("Dashboard::CDashboardWidget", "Still it will cost some time to melt paraffin, the current selected program can not run now.", 0, QApplication::UnicodeUTF8);
    m_strResetEndTime = QApplication::translate("Dashboard::CDashboardWidget", "Please re-set the End Date&Time of the current selected program.", 0, QApplication::UnicodeUTF8);
    m_strInputCassetteBoxTitle = QApplication::translate("Dashboard::CDashboardWidget", "Please enter cassette number:", 0, QApplication::UnicodeUTF8);
    m_strNotFoundStation = QApplication::translate("Dashboard::CDashboardWidget", "Program step \"%1\" of \"%2\" can not find the corresponding reagent station, one station only can be used once in the program, please set a station for the reagent in this step.", 0, QApplication::UnicodeUTF8);
    m_strCheckEmptyStation = QApplication::translate("Dashboard::CDashboardWidget", "The Station \"%1\" status is set as Empty in Program step \"%2\" of \"%3\", it can not be executed.", 0, QApplication::UnicodeUTF8);

}

void CDashboardWidget::DrawSeparatorLine()
{

    mp_Separator->setFrameShape(QFrame::HLine);
    mp_Separator->setLineWidth(2);
    mp_Separator->setFrameShadow(QFrame::Plain);
    mp_Separator->setGeometry(4,460,685,2);
    mp_Separator->show();
}

void CDashboardWidget::AddItemsToComboBox(bool bOnlyAddCleaningProgram)
{
    m_FavProgramIDs.clear();
    mp_Ui->pgmsComboBox->clear();
    if (bOnlyAddCleaningProgram)
    {
        m_FavProgramIDs.append("C01");
    }
    else
    {
        m_FavProgramIDs = mp_ProgramList->GetFavoriteProgramIDs(); // get five favorite Programs' ID
    }

    for ( int i = 0; i < m_FavProgramIDs.count(); i++)
    {
        QString ProgramId = m_FavProgramIDs.at(i);
        QString ProgramName = mp_ProgramList->GetProgram(ProgramId)->GetName();
        QString strIconName;
        if (mp_ProgramList->GetProgram(ProgramId)->GetIcon().isEmpty())
        {
            strIconName = ":/HimalayaImages/Icons/Program/IconEmpty.png";
        }
        else
            strIconName = ":/HimalayaImages/Icons/Program/"+ mp_ProgramList->GetProgram(ProgramId)->GetIcon() + ".png";
        QIcon ProgramIcon(strIconName);
        mp_Ui->pgmsComboBox->insertItem(i, ProgramIcon, ProgramName);
    }
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
    if (m_ProcessRunning && (m_CurrentUserRole == MainMenu::CMainWindow::Admin || m_CurrentUserRole == MainMenu::CMainWindow::Service))
        EnablePlayButton(true);

}


void CDashboardWidget::OnButtonClicked(int whichBtn)
{
    if ( whichBtn == Dashboard::firstButton ) {
        switch(m_ProgramNextAction)
        {
            this->EnablePlayButton(false);//protect to click twice in a short time
            default:
            break;
            case DataManager::PROGRAM_START:
            {
                if (m_IsResumeRun)
                {
                    QString strTempProgramId;
                    if (m_SelectedProgramId.at(0) == 'C')
                    {
                        strTempProgramId = m_SelectedProgramId;
                        strTempProgramId.append("_");
                        QString strReagentIDOfLastStep = m_pUserSetting->GetReagentIdOfLastStep();
                        strTempProgramId.append(strReagentIDOfLastStep);
                    }

                    mp_DataConnector->SendProgramAction(strTempProgramId, DataManager::PROGRAM_START, m_EndDateTime);
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
                    emit ProgramActionStopped(DataManager::PROGRAM_STATUS_PAUSED);//pause ProgressBar and EndTime countdown
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
            EnableAbortButton(false);
            EnablePlayButton(false);
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
        ConfirmationMessageDlg.SetTitle(m_strWarning);
        QString strMsg;
        strMsg = m_strMsgUnselect.arg(Dashboard::CDashboardDateTimeWidget::SELECTED_PROGRAM_NAME);
        ConfirmationMessageDlg.SetText(strMsg);
        ConfirmationMessageDlg.SetIcon(QMessageBox::Warning);
        ConfirmationMessageDlg.SetButtonText(1, m_strYes);
        ConfirmationMessageDlg.SetButtonText(3, m_strCancel);
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
        CDashboardDateTimeWidget::SELECTED_PROGRAM_NAME = m_strProgram;
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
int CDashboardWidget::GetASAPTime(int TimeActual,//TimeActual is seconds
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
        //int RemainingTimeMeltParaffin = 12 * 60 * 60 - TimeCostedParaffinMelting;
        int RemainingTimeMeltParaffin = 60 - TimeCostedParaffinMelting;
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

void CDashboardWidget::OnComboBoxButtonPress()
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
                                           mp_DashboardScene->GetEndDateTime(), mp_Ui->playButton->isEnabled());
        mp_ProgramStatusWidget->move(80,50);
        mp_ProgramStatusWidget->exec();
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
    QString strTempProgramId(m_NewSelectedProgramId);
    if (m_NewSelectedProgramId.at(0) == 'C')
    {
      strTempProgramId.append("_");
      QString strReagentIDOfLastStep = m_pUserSetting->GetReagentIdOfLastStep();
      strTempProgramId.append(strReagentIDOfLastStep);
    }
    mp_DataConnector->SendProgramSelected(strTempProgramId, m_ParaffinStepIndex);

}

void CDashboardWidget::CheckPreConditionsToRunProgram()
{
    if ("" == m_SelectedProgramId)
        return;

    //Check cleaning program run in last time?
    bool hasRunCleaningProgram = true;
    QString strReagentIDOfLastStep("");
    if (m_SelectedProgramId.at(0) != 'C')
    {
        strReagentIDOfLastStep = m_pUserSetting->GetReagentIdOfLastStep();
        hasRunCleaningProgram = strReagentIDOfLastStep == "";
    }

    if (!hasRunCleaningProgram)
    {
        mp_MessageDlg->SetIcon(QMessageBox::Information);
        mp_MessageDlg->SetTitle(m_strInformation);
        mp_MessageDlg->SetText(m_strCleanNotRun);
        mp_MessageDlg->SetButtonText(1, CommonString::strOK);
        mp_MessageDlg->HideButtons();
        if (mp_MessageDlg->exec())
        {
            TakeOutSpecimenAndWaitRunCleaning();
        }
        return;
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
    if (!isRMSOFF && mp_DashboardScene->HaveExpiredReagent())
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

    ConfirmationMessageDlg.SetTitle(m_strConfirmation);
    ConfirmationMessageDlg.SetText(m_strAbortProgram);
    ConfirmationMessageDlg.SetIcon(QMessageBox::Information);
    ConfirmationMessageDlg.SetButtonText(1, m_strYes);
    ConfirmationMessageDlg.SetButtonText(3, m_strCancel);
    ConfirmationMessageDlg.HideCenterButton();

    return ConfirmationMessageDlg.exec() == (int)QDialog::Accepted;
}

void CDashboardWidget::EnablePlayButton(bool bSetEnable)
{
    mp_Ui->playButton->setEnabled(bSetEnable);
    if (bSetEnable)
        mp_Ui->playButton->setStyleSheet(QString::fromUtf8("border-image: url(:/HimalayaImages/IconPushButton/IconPushButton-Enabled.png);"));
    else
        mp_Ui->playButton->setStyleSheet(QString::fromUtf8("border-image: url(:/HimalayaImages/IconPushButton/IconPushButton-Disabled.png);"));

}

void CDashboardWidget::EnableAbortButton(bool bSetEnable)
{
    mp_Ui->abortButton->setEnabled(bSetEnable);
    if (bSetEnable)
        mp_Ui->abortButton->setStyleSheet(QString::fromUtf8("border-image: url(:/HimalayaImages/IconPushButton/IconPushButton-Enabled.png);"));
    else
        mp_Ui->abortButton->setStyleSheet(QString::fromUtf8("border-image: url(:/HimalayaImages/IconPushButton/IconPushButton-Disabled.png);"));
}

void CDashboardWidget::OnProgramStartReadyUpdated()
{
    if (!m_SelectedProgramId.isEmpty())
        this->EnablePlayButton(true);
    m_ProgramStartReady = true;
}

void CDashboardWidget::OnProgramWillComplete()
{
    //log the reagent ID in last step
    const DataManager::CProgram* pProgram = mp_ProgramList->GetProgram(m_SelectedProgramId);
    QString strReagentIDOfLastStep = pProgram->GetProgramStep(pProgram->GetNumberOfSteps()-1)->GetReagentID();
    m_pUserSetting->SetReagentIdOfLastStep(strReagentIDOfLastStep);
    emit UpdateUserSetting(*m_pUserSetting);

    mp_MessageDlg->SetIcon(QMessageBox::Warning);
    mp_MessageDlg->SetTitle(m_strWarning);
    QString strTemp(m_strProgramComplete);
    strTemp = strTemp.arg(CDashboardDateTimeWidget::SELECTED_PROGRAM_NAME);
    mp_MessageDlg->SetText(strTemp);
    mp_MessageDlg->SetButtonText(1, m_strOK);
    mp_MessageDlg->HideButtons();

    emit ProgramActionStopped(DataManager::PROGRAM_STATUS_PAUSED);//pause ProgressBar and EndTime countdown

    if (mp_MessageDlg->exec())
    {
        m_IsDraining = true;

        //Resume ProgressBar and EndTime countdown
        emit ProgramActionStarted(DataManager::PROGRAM_START, m_TimeProposed, Global::AdjustedTime::Instance().GetCurrentDateTime(), true);

        mp_DataConnector->SendProgramAction(m_SelectedProgramId, DataManager::PROGRAM_DRAIN);
        //disable pause and abort
        EnablePlayButton(false);
        EnableAbortButton(false);
        return;
    }
}

//this function will be invoked after program Abort and completed
void CDashboardWidget::TakeOutSpecimenAndWaitRunCleaning()
{
    mp_MessageDlg->SetIcon(QMessageBox::Warning);
    mp_MessageDlg->SetTitle(m_strWarning);
    mp_MessageDlg->SetText(m_strTakeOutSpecimen);
    mp_MessageDlg->SetButtonText(1, m_strOK);
    mp_MessageDlg->HideButtons();
    if (mp_MessageDlg->exec())
    {
        //represent the retort as contaminated status
        mp_DashboardScene->UpdateRetortStatus(DataManager::CONTAINER_STATUS_CONTAMINATED);

        mp_MessageDlg->SetText(m_strRetortContaminated);
        mp_MessageDlg->SetButtonText(1, m_strOK);
        mp_MessageDlg->HideButtons();
        //mp_MessageDlg->EnableButton(1, false);//when lock is locked, "OK" will be enable
        mp_MessageDlg->EnableButton(1, true);//6.6 for test

        m_IsWaitingCleaningProgram = true;
        if (mp_MessageDlg->exec())
        {
            //Change UI to prepare start Cleaning Program
            mp_Ui->pgmsComboBox->WorkAsButton(false);

            m_ProgramNextAction = DataManager::PROGRAM_START;
            mp_Ui->playButton->setIcon(QIcon(":/HimalayaImages/Icons/Dashboard/Operation/Operation_Start_Resume.png"));

            //only show Cleaning program in the ComboBox list
            this->AddItemsToComboBox(true);

            //switch to the dashboard page
            mp_MainWindow->SetTabWidgetIndex();
            //show the comboBox
            mp_Ui->pgmsComboBox->showPopup();

        }
    }
}

void CDashboardWidget::OnProgramBeginAbort()
{
    //show "aborting"
    //time countdown
    //Todo:20, Abort time, will be given a rough value later;
    emit ProgramActionStarted(DataManager::PROGRAM_ABORT, 20, Global::AdjustedTime::Instance().GetCurrentDateTime(), false);
    this->EnablePlayButton(false);
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

    m_IsResumeRun = false;
    m_CurProgramStepIndex = -1;
    mp_Ui->pgmsComboBox->WorkAsButton(false);

    emit ProgramActionStopped(DataManager::PROGRAM_STATUS_ABORTED);

    //disable "Start" button, enable Retort lock button, hide End time button, now Abort button is still in "disable" status
    EnablePlayButton(false);

    QDateTime  endDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime();
    if (m_StartDateTime.isValid())
    {
        int days = m_StartDateTime.daysTo(endDateTime);
        m_pUserSetting->SetOperationHours(m_pUserSetting->GetOperationHours() + days * 24);
        m_pUserSetting->SetActiveCarbonHours(m_pUserSetting->GetActiveCarbonHours() + days * 24);
		emit UpdateUserSetting(*m_pUserSetting);
    }

    mp_MessageDlg->SetIcon(QMessageBox::Warning);
    mp_MessageDlg->SetTitle(m_strWarning);
    QString strTemp;
    strTemp = m_strProgramIsAborted.arg(CDashboardDateTimeWidget::SELECTED_PROGRAM_NAME);
    mp_MessageDlg->SetText(strTemp);
    mp_MessageDlg->SetButtonText(1, m_strOK);
    mp_MessageDlg->HideButtons();
    if (mp_MessageDlg->exec())
    {
        this->TakeOutSpecimenAndWaitRunCleaning();
    }
}

void CDashboardWidget::OnProgramCompleted()
{
    m_IsResumeRun = false;
    m_CurProgramStepIndex = -1;
    mp_Ui->pgmsComboBox->WorkAsButton(false);//now it is ComboBox, not a long button

    if (m_SelectedProgramId.at(0) == 'C')
    {
        QString reagentID("");
        m_pUserSetting->SetReagentIdOfLastStep(reagentID);//Clear CleaningProgram flag
        emit UpdateUserSetting(*m_pUserSetting);
        EnableAbortButton(false);
        AddItemsToComboBox();

        m_ProgramNextAction = DataManager::PROGRAM_START;
        mp_Ui->playButton->setIcon(QIcon(":/HimalayaImages/Icons/Dashboard/Operation/Operation_Start_Resume.png"));
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
    emit ProgramActionStarted(DataManager::PROGRAM_START, m_TimeProposed, Global::AdjustedTime::Instance().GetCurrentDateTime(), m_IsResumeRun);
    m_IsResumeRun = true;
    mp_Ui->pgmsComboBox->WorkAsButton(true);
    if (m_SelectedProgramId.at(0) == 'C')
    {
        EnablePlayButton(false);//enable pause button
        EnableAbortButton(false);
    }
    else
    {
        EnablePlayButton(true);//enable pause button
        EnableAbortButton(true);
    }

    m_StartDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime();

}

void CDashboardWidget::OnProcessStateChanged()
{
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if (m_ProcessRunning)
    {

    }
    else
    {
        mp_DashboardScene->OnPauseStationSuckDrain();
    }
}

void CDashboardWidget::OnRetortLockStatusChanged(const MsgClasses::CmdRetortLockStatus& cmd)
{
    if (cmd.IsLocked())
    {
        mp_Ui->labelLockIndicator->lock(true);
    }
    else
    {
        mp_Ui->labelLockIndicator->lock(false);
        //enable the "OK"
        if (m_IsWaitingCleaningProgram && mp_MessageDlg->isVisible())
        {
            mp_MessageDlg->EnableButton(1, true);
        }
    }
}

void CDashboardWidget::OnProgramSelectedReply(const MsgClasses::CmdProgramSelectedReply& cmd)
{
    //get the proposed program end DateTime
    bool bCanotRun = true;
    int asapEndTime = GetASAPTime(cmd.TimeProposed(),
                                  cmd.ParaffinMeltCostedTime(), cmd.CostedTimeBeforeParaffin(), bCanotRun);

    if (m_CheckEndDatetimeAgain)
    {
        m_CheckEndDatetimeAgain = false;
        asapEndTime = asapEndTime - 60;//60 seconds: buffer time for "select program" and "Run" operation.
        QDateTime endDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(asapEndTime);
        if (endDateTime > m_EndDateTime)
        {
            mp_MessageDlg->SetIcon(QMessageBox::Warning);
            mp_MessageDlg->SetTitle(m_strWarning);
            mp_MessageDlg->SetText(m_strResetEndTime);
            mp_MessageDlg->SetButtonText(1, m_strOK);
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
            if (m_SelectedProgramId.at(0) != 'C' && Global::RMS_CASSETTES == mp_DataConnector->SettingsInterface->GetUserSettings()->GetModeRMSProcessing())
            {
                CCassetteNumberInputWidget *pCassetteInput = new Dashboard::CCassetteNumberInputWidget();
                pCassetteInput->setWindowFlags(Qt::CustomizeWindowHint);
                pCassetteInput->SetDialogTitle(m_strInputCassetteBoxTitle);
                QRect scr = mp_MainWindow->geometry();
                pCassetteInput->move( scr.center() - pCassetteInput->rect().center());
                pCassetteInput->exec();

                int cassetteNumber = pCassetteInput->CassetteNumber();
                if (-1 == cassetteNumber)
                    return;//clicked Cancel button

                mp_DataConnector->SendKeepCassetteCount(cassetteNumber);
                delete pCassetteInput;
            }
        }

        QString strTempProgramId(m_SelectedProgramId);
        if (m_SelectedProgramId.at(0) == 'C')
        {
            strTempProgramId.append("_");
            QString strReagentIDOfLastStep = m_pUserSetting->GetReagentIdOfLastStep();
            strTempProgramId.append(strReagentIDOfLastStep);
        }

        mp_DataConnector->SendProgramAction(strTempProgramId, DataManager::PROGRAM_START, m_EndDateTime);
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
            mp_MessageDlg->SetTitle(m_strWarning);
            QString strTemp = m_strNotFoundStation.arg(QString::number(i+1)).arg(CDashboardDateTimeWidget::SELECTED_PROGRAM_NAME);
            mp_MessageDlg->SetText(strTemp);
            mp_MessageDlg->SetButtonText(1, m_strOK);
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
                mp_MessageDlg->SetTitle(m_strWarning);
                QString strTemp = m_strCheckEmptyStation.arg(pStation->GetDashboardStationName()).arg(QString::number(i+1)).arg(CDashboardDateTimeWidget::SELECTED_PROGRAM_NAME);
                mp_MessageDlg->SetText(strTemp);
                mp_MessageDlg->SetButtonText(1, m_strOK);
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
    m_TimeProposed = cmd.TimeProposed();
    m_EndDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(asapEndTime);


    emit ProgramSelected(m_SelectedProgramId, asapEndTime, m_StationList);//for UI update

    if (m_ProgramStartReady)
    {
        EnablePlayButton(true);
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
    mp_DashboardScene->OnStationSuckDrain(cmd.StationID(), cmd.IsStart(), cmd.IsSuck());

    if (m_IsDraining && !cmd.IsStart() && !cmd.IsSuck())
    {
        this->TakeOutSpecimenAndWaitRunCleaning();//pause ProgressBar and EndTime countdown
        m_IsDraining = false;//when abort or pause, set this too?
    }
}

} // End of namespace Dashboard
