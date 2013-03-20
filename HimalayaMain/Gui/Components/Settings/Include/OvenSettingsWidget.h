/****************************************************************************/
/*! \file OvenSettingsWidget.h
 *
 *  \brief OvenSettingsWidget definition.
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

#ifndef SETTINGS_OVENSETTINGSWIDGET_H
#define SETTINGS_OVENSETTINGSWIDGET_H

#include "DataManager/Containers/UserSettings/Include/UserSettings.h"
#include "MainMenu/Include/PanelFrame.h"
#include "MainMenu/Include/ScrollWheel.h"
#include "MainMenu/Include/MainWindow.h"

namespace Settings {

namespace Ui {
    class COvenSettingsWidget;
}

/****************************************************************************/
/**
 * \brief This class displays all settings related to the oven of Himalaya
 */
/****************************************************************************/
class COvenSettingsWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT

private:
    Ui::COvenSettingsWidget *mp_Ui;                 //!< User interface
    MainMenu::CScrollWheel *mp_ScrollWheel;         //!< Temperature scroll wheel
    DataManager::CUserSettings *mp_UserSettings;    //!< Data object
    DataManager::CUserSettings m_UserSettingsTemp;  //!< Temporary copy of User Settings
    MainMenu::CMainWindow *mp_MainWindow;           //!< Pointer to MainWindow
    //Flags
    bool m_ProcessRunning;                          //!< Process running state
    //UI related
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;    //! < Current user role

public:
    explicit COvenSettingsWidget(QWidget *p_Parent = NULL);
    ~COvenSettingsWidget();
    void SetUserSettings(DataManager::CUserSettings *p_UserSettings);
    void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow);

private:
    void InitTemperatureWidget();
    void RetranslateUI();
    void ResetButtons();

protected:
    void changeEvent(QEvent *p_Event);
    void showEvent(QShowEvent *p_Event);

private slots:
    void OnProcessStateChanged();
    void OnUserRoleChanged();
    void OnApply();

signals:
    void OvenTemperatureChanged(DataManager::CUserSettings &Settings);
};

} // end namespace Settings

#endif // SETTINGS_OVENSETTINGSWIDGET_H
