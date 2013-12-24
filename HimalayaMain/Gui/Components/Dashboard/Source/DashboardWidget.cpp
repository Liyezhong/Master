/****************************************************************************/
/*! \file ReagentWidget.cpp.cpp
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


using namespace Dashboard;

CDashboardWidget::CDashboardWidget(Core::CDataConnector *p_DataConnector,
                                     MainMenu::CMainWindow *p_Parent) :
    ui(new Ui::CDashboardWidget),
    mp_DataConnector(p_DataConnector),
    mp_MainWindow(p_Parent),
    m_ParaffinStepIndex(-1),
    m_TimeProposed(0)
{
    ui->setupUi(this);
    ui->containerPanelWidget->SetPtrToMainWindow(mp_MainWindow, mp_DataConnector);
    ui->programPanelWidget->SetPtrToMainWindow(mp_MainWindow, mp_DataConnector);
    m_pUserSetting = mp_DataConnector->SettingsInterface->GetUserSettings();
    mp_ProgramList = mp_DataConnector->ProgramList;
    CONNECTSIGNALSIGNAL(this, AddItemsToFavoritePanel(bool), ui->programPanelWidget, AddItemsToFavoritePanel(bool));
    CONNECTSIGNALSLOT(ui->programPanelWidget, PrepareSelectedProgramChecking(const QString&), this, PrepareSelectedProgramChecking(const QString&));
    CONNECTSIGNALSLOT(mp_DataConnector, ProgramSelectedReply(const MsgClasses::CmdProgramSelectedReply &),
                      this, OnProgramSelectedReply(const MsgClasses::CmdProgramSelectedReply&));

    CONNECTSIGNALSLOT(ui->programPanelWidget, OnSelectEndDateTime(const QDateTime&),
                        this, OnSelectEndDateTime(const QDateTime &));

    CONNECTSIGNALSIGNAL(this, ProgramSelected(QString&, int),
                       ui->programPanelWidget, ProgramSelected(QString&, int));

    CONNECTSIGNALSIGNAL(this, ProgramSelected(QString&, QList<QString>&),
                       ui->containerPanelWidget, ProgramSelected(QString&, QList<QString>&));
    mp_MessageDlg = new MainMenu::CMessageDlg(this);


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

void CDashboardWidget::OnUnselectProgram()
{
    ui->containerPanelWidget->OnUnselectProgram();
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
    //Check safe reagent
    int iWhichStepHasNoSafeReagent = cmd.WhichStepHasNoSafeReagent();
    if (iWhichStepHasNoSafeReagent  != -1)
    {
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        mp_MessageDlg->SetTitle(CommonString::strWarning);
        QString strTemp = m_strCheckSafeReagent.arg(QString::number(iWhichStepHasNoSafeReagent +1)).arg(CFavoriteProgramsPanelWidget::SELECTED_PROGRAM_NAME);
        mp_MessageDlg->SetText(strTemp);
        mp_MessageDlg->SetButtonText(1, CommonString::strYes);
        mp_MessageDlg->SetButtonText(3, CommonString::strNo);
        mp_MessageDlg->HideCenterButton();
        if (!mp_MessageDlg->exec())
        {
            return;
        }
    }

    //get the proposed program end DateTime
    bool bCanotRun = true;
    int asapEndTime = GetASAPTime(cmd.TimeProposed(),
                                  cmd.ParaffinMeltCostedTime(), cmd.CostedTimeBeforeParaffin(), bCanotRun);

    //firstly check whether there is any station not existing for some program steps
    const QList<QString>& stationList = cmd.StationList();
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
                mp_MessageDlg->SetTitle(CommonString::strWarning);
                QString strTemp = m_strCheckEmptyStation.arg(pStation->GetDashboardStationName()).arg(QString::number(i+1)).arg(CFavoriteProgramsPanelWidget::SELECTED_PROGRAM_NAME);
                mp_MessageDlg->SetText(strTemp);
                mp_MessageDlg->SetButtonText(1, CommonString::strOK);
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

    //get the proposed program end DateTime
    m_TimeProposed = cmd.TimeProposed();
    m_EndDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(asapEndTime);


    emit ProgramSelected(m_SelectedProgramId, asapEndTime);
    emit ProgramSelected(m_SelectedProgramId, m_StationList);
    emit UpdateSelectedStationList(m_StationList);
    //if (m_ProgramStartReady)
    {
        //  EnablePlayButton(true);
    }
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
    m_strCheckEmptyStation = QApplication::translate("Dashboard::CContainerPanelWidget", "The Station \"%1\" status is set as Empty in Program step \"%2\" of \"%3\", it can not be executed.", 0, QApplication::UnicodeUTF8);
}

void CDashboardWidget::OnSelectEndDateTime(const QDateTime& dateTime)
{
    m_EndDateTime = dateTime;
}

