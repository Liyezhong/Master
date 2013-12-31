/****************************************************************************/
/*! \file Dashboard/Source/ContainerPanelWidget.cpp
 *
 *  \brief Implementation file for class ContainerPanelWidget.
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
#include "Dashboard/Include/ContainerPanelWidget.h"
#include "ui_ContainerPanelWidget.h"
#include "MainMenu/Include/SliderControl.h"
#include "Dashboard/Include/DashboardProgramStatusWidget.h"
#include "Dashboard/Include/CassetteNumberInputWidget.h"
#include "Dashboard/Include/CommonString.h"
#include "Dashboard/Include/FavoriteProgramsPanelWidget.h"

namespace Dashboard {


QString CContainerPanelWidget::m_strWarning = CContainerPanelWidget::tr("Warning");
QString CContainerPanelWidget::m_strYes = CContainerPanelWidget::tr("Yes");
QString CContainerPanelWidget::m_strCancel = CContainerPanelWidget::tr("Cancel");

CContainerPanelWidget::CContainerPanelWidget(QWidget *p_Parent): MainMenu::CPanelFrame(p_Parent),
                                   mp_Ui(new Ui::CContainerPanelWidget),mp_MainWindow(NULL),
                                   mp_DataConnector(NULL),
                                   m_UserRoleChanged(false),
                                   m_ParaffinStepIndex(-1),
                                   m_strProgram(tr("Program")),
                                   m_strInformation(tr("Information")),
                                   m_strOK(tr("OK")),
                                   m_strNo(tr("No")),
                                   m_strNeedMeltParaffin(tr("Still it will cost some time to melt paraffin, the current selected program can not run now."))
{
     mp_Ui->setupUi(GetContentFrame());
     SetPanelTitle(QApplication::translate("Dashboard::CContainerPanelWidget", "Dashboard", 0, QApplication::UnicodeUTF8));
     QPalette Palette = mp_Ui->dashboardView->palette();
     QColor BaseColor = Palette.color(QPalette::Base);
     BaseColor.setAlpha(0);
     Palette.setColor(QPalette::Base, BaseColor);
     mp_Ui->dashboardView->setPalette(Palette);
}


CContainerPanelWidget::~CContainerPanelWidget()
{
    try {
        delete mp_ProgramStatusWidget;
        delete mp_DashboardScene;
        delete mp_MessageDlg;
        delete mp_Ui;
    } catch(...) {

    }
}

void  CContainerPanelWidget::SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow, Core::CDataConnector *p_DataConnector)
{
    mp_MainWindow = p_MainWindow;
    mp_DataConnector = p_DataConnector;
    this->Initialize();
}

void CContainerPanelWidget::Initialize()
{
    mp_ProgramStatusWidget = new Dashboard::CDashboardProgramStatusWidget();
    mp_ProgramStatusWidget->setWindowFlags(Qt::CustomizeWindowHint);
    mp_ProgramStatusWidget->setVisible(false);

    mp_DashboardScene = new CDashboardScene(mp_DataConnector, this, mp_MainWindow);
    mp_Ui->dashboardView->setScene(mp_DashboardScene);
    //mp_Ui->dashboardView->setRenderHint(QPainter::Antialiasing);

    CONNECTSIGNALSLOT(mp_MainWindow, ProcessStateChanged(), this, OnProcessStateChanged());

    m_pUserSetting = mp_DataConnector->SettingsInterface->GetUserSettings();

    mp_ProgramList = mp_DataConnector->ProgramList;

    mp_MessageDlg = new MainMenu::CMessageDlg();

    //CONNECTSIGNALSLOT(mp_ProgramStatusWidget, AbortClicked(int), this, OnButtonClicked(int));

    CONNECTSIGNALSLOT(this, ProgramSelected(QString&, QList<QString>&),
                       mp_DashboardScene, UpdateDashboardSceneReagentsForProgram(QString&, QList<QString>&));

    CONNECTSIGNALSLOT(mp_DataConnector, RetortLockStatusChanged(const MsgClasses::CmdRetortLockStatus &),
                      this, OnRetortLockStatusChanged(const MsgClasses::CmdRetortLockStatus&));

    CONNECTSIGNALSLOT(mp_DataConnector, StationSuckDrain(const MsgClasses::CmdStationSuckDrain &),
                      this, OnStationSuckDrain(const MsgClasses::CmdStationSuckDrain &));
}

void CContainerPanelWidget::changeEvent(QEvent *p_Event)
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

void CContainerPanelWidget::RetranslateUI()
{
    SetPanelTitle(QApplication::translate("Dashboard::CContainerPanelWidget", "Dashboard", 0, QApplication::UnicodeUTF8));
    m_strYes = QApplication::translate("Dashboard::CContainerPanelWidget", "Yes", 0, QApplication::UnicodeUTF8);
    m_strNo = QApplication::translate("Dashboard::CContainerPanelWidget", "No", 0, QApplication::UnicodeUTF8);
    m_strOK = QApplication::translate("Dashboard::CContainerPanelWidget", "OK", 0, QApplication::UnicodeUTF8);
    m_strCancel = QApplication::translate("Dashboard::CContainerPanelWidget", "Cancel", 0, QApplication::UnicodeUTF8);
    m_strWarning = QApplication::translate("Dashboard::CContainerPanelWidget", "Warning", 0, QApplication::UnicodeUTF8);
    m_strProgram = QApplication::translate("Dashboard::CContainerPanelWidget", "Program", 0, QApplication::UnicodeUTF8);
    m_strInformation = QApplication::translate("Dashboard::CContainerPanelWidget", "Information", 0, QApplication::UnicodeUTF8);
    m_strNotStartExpiredReagent = QApplication::translate("Dashboard::CContainerPanelWidget", "Reagents needed for this program are expired! You can't operate this program.", 0, QApplication::UnicodeUTF8);
    m_strStartExpiredReagent =  QApplication::translate("Dashboard::CContainerPanelWidget", "Do you want to Start the Program with Expired Reagents?", 0, QApplication::UnicodeUTF8);
    m_strNeedMeltParaffin  = QApplication::translate("Dashboard::CContainerPanelWidget", "Still it will cost some time to melt paraffin, the current selected program can not run now.", 0, QApplication::UnicodeUTF8);
}

void CContainerPanelWidget::UpdateRetortStatus(DataManager::ContainerStatusType_t retortStatusType)
{
    mp_DashboardScene->UpdateRetortStatus(retortStatusType);
}

void CContainerPanelWidget::AddItemsToComboBox(bool bOnlyAddCleaningProgram)
{
    m_FavProgramIDs.clear();
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
       // QIcon ProgramIcon(strIconName);
        //mp_Ui->pgmsComboBox->insertItem(i, ProgramIcon, ProgramName);
    }
}

void CContainerPanelWidget::OnProcessStateChanged()
{
    bool processRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if (!processRunning)
    {
        mp_DashboardScene->OnPauseStationSuckDrain();
    }
}

void CContainerPanelWidget::OnRetortLockStatusChanged(const MsgClasses::CmdRetortLockStatus& cmd)
{
    if (cmd.IsLocked())
    {
		mp_DashboardScene->UpdateRetortLockedStatus(true);
    }
    else
    {
		mp_DashboardScene->UpdateRetortLockedStatus(false);
    }
}


void CContainerPanelWidget::OnStationSuckDrain(const MsgClasses::CmdStationSuckDrain & cmd)
{
    mp_DashboardScene->OnStationSuckDrain(cmd.StationID(), cmd.IsStart(), cmd.IsSuck());
}

} // End of namespace Dashboard
