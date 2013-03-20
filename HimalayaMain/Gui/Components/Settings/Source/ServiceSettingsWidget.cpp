/****************************************************************************/
/*! \file ServiceSettingsWidget.cpp
 *
 *  \brief ServiceSettingsWidget implementation.
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
#include "Settings/Include/ServiceSettingsWidget.h"
#include "ui_ServiceSettingsWidget.h"

namespace Settings {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CServiceSettingsWidget::CServiceSettingsWidget(QWidget *p_Parent) :  MainMenu::CPanelFrame(p_Parent),
    mp_Ui(new Ui::CServiceSettingsWidget)
{
    mp_Ui->setupUi(GetContentFrame());
    SetPanelTitle(tr("Service"));
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CServiceSettingsWidget::~CServiceSettingsWidget()
{
    delete mp_Ui;
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/

void CServiceSettingsWidget::changeEvent(QEvent *p_Event)
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
 *  \brief Updates the widget content everytime it is displayed
 *
 *  \iparam p_Event = Show event
 */
/****************************************************************************/
void CServiceSettingsWidget::showEvent(QShowEvent *p_Event)
{
    Q_UNUSED(p_Event);
    ResetButtons();
}

/****************************************************************************/
/*!
 *  \brief This slot is called when User Role changes
 */
/****************************************************************************/
void CServiceSettingsWidget::OnUserRoleChanged()
{
    ResetButtons();
}

/****************************************************************************/
/*!
 *  \brief This slot is called when Process state changes
 */
/****************************************************************************/
void CServiceSettingsWidget::OnProcessStateChanged()
{
    ResetButtons();
}

/****************************************************************************/
/*!
 *  \brief Enables/Disables the apply button based on the user role/process
 *         running status
 */
/****************************************************************************/
void CServiceSettingsWidget::ResetButtons()
{
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if ((m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
         (!m_ProcessRunning)) {
        //Edit Mode
        mp_Ui->btnService->setEnabled(true);
    }
    else {
        mp_Ui->btnService->setEnabled(false);
    }
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CServiceSettingsWidget::RetranslateUI()
{
   MainMenu::CPanelFrame::SetPanelTitle(QApplication::translate("Settings::CServiceSettingsWidget", "Service", 0, QApplication::UnicodeUTF8));
}

/****************************************************************************/
/*!
 *  \brief Used to set pointer to mainwindow, used to retreive user role and
 *         process state changed.
 */
/****************************************************************************/
void CServiceSettingsWidget::SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow)
{
    mp_MainWindow = p_MainWindow;
    CONNECTSIGNALSLOT(mp_MainWindow, UserRoleChanged(), this, OnUserRoleChanged());
    CONNECTSIGNALSLOT(mp_MainWindow, ProcessStateChanged(), this, OnProcessStateChanged());
}

} // end namespace Settings
