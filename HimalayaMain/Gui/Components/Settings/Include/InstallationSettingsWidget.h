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

#ifndef SETTINGS_INSTALLATIONSETTINGSWIDGET_H
#define SETTINGS_INSTALLATIONSETTINGSWIDGET_H

#include "DataManager/Containers/UserSettings/Include/UserSettings.h"
#include "KeyBoard/Include/KeyBoardObserver.h"//!< User interface
#include "KeyBoard/Include/KeyBoard.h"
#include "MainMenu/Include/PanelFrame.h"
#include "MainMenu/Include/ScrollWheel.h"
#include "MainMenu/Include/MainWindow.h"

#include "ui_InstallationSettingsWidget.h"

namespace Settings {

namespace Ui {
    class CInstallationSettingsWidget;
}

/****************************************************************************/
/**
 * \brief This class displays all settings related to the oven of Himalaya
 */
/****************************************************************************/
class CInstallationSettingsWidget : public MainMenu::CPanelFrame,public KeyBoard::CKeyBoardObserver
{
    Q_OBJECT

private:
    Ui::CInstallationSettingsWidget *mp_Ui;                 //!< User interface
    KeyBoard::CKeyBoard *mp_KeyBoardWidget;         //!< Reference to Keyboard widget
    KeyBoard::ValidationType_t m_ValidationType;    //!< Keyboard input type validation
    DataManager::CUserSettings *mp_UserSettings;    //!< Data object
    DataManager::CUserSettings m_UserSettingsTemp;  //!< Temporary copy of User Settings
    MainMenu::CMainWindow *mp_MainWindow;           //!< Pointer to MainWindow
    //Flags
    bool m_ProcessRunning;                          //!< Process running state
    //UI related
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;    //! < Current user role

public:
    explicit CInstallationSettingsWidget(QWidget *p_Parent = NULL);
    ~CInstallationSettingsWidget();
    void SetUserSettings(DataManager::CUserSettings *p_UserSettings);
    void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow);
    void SetKeyBoardInstance(KeyBoard::CKeyBoard *p_KeyBoard);
    void Update();


private:
    void RetranslateUI();
    void ResetButtons();


protected:
    void changeEvent(QEvent *p_Event);
    void showEvent(QShowEvent *p_Event);
    void UpdateOnESC();

 private slots:
    void OnEditclicked();
    void OnOkClicked();
    void OnUpdateclicked();

};

} // end namespace Settings

#endif // SETTINGS_OVENSETTINGSWIDGET_H
