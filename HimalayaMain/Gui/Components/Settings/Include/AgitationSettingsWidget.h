/****************************************************************************/
/*! \file AgitationSettingsWidget.h
 *
 *  \brief AgitationSettingsWidget definition.
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

#ifndef SETTINGS_AGITATIONSETTINGSWIDGET_H
#define SETTINGS_AGITATIONSETTINGSWIDGET_H

#include "DataManager/Containers/UserSettings/Include/UserSettings.h"
#include "MainMenu/Include/MainWindow.h"
#include "MainMenu/Include/PanelFrame.h"
#include "MainMenu/Include/ScrollWheel.h"
#include <QShowEvent>

namespace Settings {

namespace Ui {
    class CAgitationSettingsWidget;
}

/****************************************************************************/
/**
 * \brief Setup panel for the agitation device of Himalaya
 */
/****************************************************************************/
class CAgitationSettingsWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT

private:
    Ui::CAgitationSettingsWidget *mp_Ui;    //!< User interface
    MainMenu::CScrollWheel *mp_ScrollWheel; //!< Agitation speed
    DataManager::CUserSettings *mp_UserSettings;  //!< Data object
    DataManager::CUserSettings m_UserSettingsTemp;  //!< Temporary copy of User Settings
    MainMenu::CMainWindow *mp_MainWindow;         //!< Pointer to MainWindow
    //Flags
    bool m_ProcessRunning;                  //!< Process running state
    //UI related
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;    //! < Current user role

public:
    explicit CAgitationSettingsWidget(QWidget *p_Parent = NULL);
    ~CAgitationSettingsWidget();
    void SetUserSettings(DataManager::CUserSettings *p_UserSettings);
    void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow);

private:
    void InitSpeedWidget();
    void RetranslateUI();
    void ResetButtons();

protected:
    void changeEvent(QEvent *p_Event);
    void showEvent(QShowEvent *p_Event);

private slots:
    void OnProcessStateChanged();
    void OnUserRoleChanged();
    void OnSave();
signals:
    void AgitationSettingsChanged(DataManager::CUserSettings &Settings);
};

} // end namespace Settings

#endif // SETTINGS_AGITATIONSETTINGSWIDGET_H
