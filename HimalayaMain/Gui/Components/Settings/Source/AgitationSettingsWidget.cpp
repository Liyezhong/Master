/****************************************************************************/
/*! \file AgitationSettingsWidget.cpp
 *
 *  \brief AgitationSettingsWidget implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-29
 *   $Author:  $ M.Scherer
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/


#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include "Settings/Include/AgitationSettingsWidget.h"
#include "ui_AgitationSettingsWidget.h"

namespace Settings {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CAgitationSettingsWidget::CAgitationSettingsWidget(QWidget *p_Parent) : MainMenu::CPanelFrame(p_Parent),
    mp_Ui(new Ui::CAgitationSettingsWidget), mp_UserSettings(NULL), mp_MainWindow(NULL)
{
    mp_Ui->setupUi(GetContentFrame());
    SetPanelTitle(tr("Agitation"));

    mp_ScrollWheel = new MainMenu::CScrollWheel();
    CONNECTSIGNALSLOT(mp_Ui->btnApply, clicked(), this, OnSave());

    InitSpeedWidget();
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CAgitationSettingsWidget::~CAgitationSettingsWidget()
{
    try {
        delete mp_ScrollWheel;
        delete mp_Ui;
    }
    catch (...) {
        // to please Lint.
    }
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/

void CAgitationSettingsWidget::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
        case QEvent::LanguageChange:
            mp_Ui->retranslateUi(this);
            RetranslateUI();
            break;
        default:
            break;
    }
}

/****************************************************************************/
/*!
 *  \brief Sets the settings data object for this class
 *
 *  \iparam p_UserSettings = Global user settings data
 */
/****************************************************************************/
void CAgitationSettingsWidget::SetUserSettings(DataManager::CUserSettings *p_UserSettings)
{
    mp_UserSettings = p_UserSettings;
}

/****************************************************************************/
/*!
 *  \brief Initializes the scroll wheel for agitation speed
 */
/****************************************************************************/
void CAgitationSettingsWidget::InitSpeedWidget()
{
    mp_Ui->scrollPanelWidget->Init(1);

    for (qint32 i = 0; i <= 5; i++) {
        mp_ScrollWheel->AddItem(QString("%1").arg(i), i);
    }
    mp_ScrollWheel->SetNonContinuous();
    mp_Ui->scrollPanelWidget->AddScrollWheel(mp_ScrollWheel, 0);
}

/****************************************************************************/
/*!
 *  \brief Updates the widget content everytime it is displayed
 *
 *  \iparam p_Event = Show event
 */
/****************************************************************************/
void CAgitationSettingsWidget::showEvent(QShowEvent *p_Event)
{
    if((mp_UserSettings != NULL) && (p_Event != NULL) && !p_Event->spontaneous()) {
        mp_ScrollWheel->SetCurrentData(mp_UserSettings->GetValue("Agitation_Speed").toInt());
        ResetButtons();
    }
}

/****************************************************************************/
/*!
 *  \brief This slot is called when User Role changes
 */
/****************************************************************************/
void CAgitationSettingsWidget::OnUserRoleChanged()
{
    ResetButtons();
}

/****************************************************************************/
/*!
 *  \brief This slot is called when Process state changes
 */
/****************************************************************************/
void CAgitationSettingsWidget::OnProcessStateChanged()
{
    ResetButtons();
}

/****************************************************************************/
/*!
 *  \brief Enables/Disables the apply button based on the user role/process
 *         running status
 */
/****************************************************************************/
void CAgitationSettingsWidget::ResetButtons()
{
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
         m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
         (!m_ProcessRunning)) {
        //Edit Mode
        mp_Ui->btnApply->setEnabled(true);
    }
    else {
        mp_Ui->btnApply->setEnabled(false);
    }
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CAgitationSettingsWidget::RetranslateUI()
{
   MainMenu::CPanelFrame::SetPanelTitle(QApplication::translate("Settings::CAgitationSettingsWidget", "Agitation", 0, QApplication::UnicodeUTF8));
}

/****************************************************************************/
/*!
 *  \brief Used to set pointer to mainwindow, used to retreive user role and
 *         process state changed.
 */
/****************************************************************************/
void CAgitationSettingsWidget::SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow)
{
    mp_MainWindow = p_MainWindow;
    CONNECTSIGNALSLOT(mp_MainWindow, UserRoleChanged(), this, OnUserRoleChanged());
    CONNECTSIGNALSLOT(mp_MainWindow, ProcessStateChanged(), this, OnProcessStateChanged());
}

/****************************************************************************/
/*!
 *  \brief Called when the Save button is clicked
 */
/****************************************************************************/
void CAgitationSettingsWidget::OnSave()
{
    m_UserSettingsTemp = *mp_UserSettings;
    m_UserSettingsTemp.SetValue("Agitation_Speed", QString::number(mp_ScrollWheel->GetCurrentData().toInt()));
    emit AgitationSettingsChanged(m_UserSettingsTemp);
}


} // end namespace Settings
