/****************************************************************************/
/*! \file NetworkSettingsWidget.h
 *
 *  \brief NetworkSettingsWidget definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-08-26
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

#ifndef SETTINGS_NETWORKSETTINGSWIDGET_H
#define SETTINGS_NETWORKSETTINGSWIDGET_H

#include "MainMenu/Include/PanelFrame.h"
#include "MainMenu/Include/MainWindow.h"
#include "KeyBoard/Include/KeyBoard.h"
#include "DataManager/Containers/UserSettings/Include/UserSettings.h"
#include <QRegExpValidator>

namespace Settings {

namespace Ui {
    class CNetworkSettingsWidget;
}

//!< Button Type -UserName, Password, IP Address or Port
typedef enum {
    USERNAME_BTN_CLICKED = 1,
    PASSWORD_BTN_CLICKED,
    IP_ADDRESS_BTN_CLICKED,
    PORT_BTN_CLICKED
}ButtonType_t;
/****************************************************************************/
/**
 * \brief This class contains the proxy settings for remote care
 */
/****************************************************************************/
class CNetworkSettingsWidget : public MainMenu::CPanelFrame,
                               public KeyBoard::CKeyBoardObserver
{
    Q_OBJECT

public:
    explicit CNetworkSettingsWidget(QWidget *p_Parent = 0);
    ~CNetworkSettingsWidget();
    void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow);
    void SetKeyBoardInstance(KeyBoard::CKeyBoard *p_KeyBoard);

private:
    Ui::CNetworkSettingsWidget *mp_Ui;              //!< User interface    
    MainMenu::CMainWindow *mp_MainWindow;           //!< Pointer to MainWindow
    //Flags
    bool m_ProcessRunning;                          //!< Process running state
    //UI related
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;    //! < Current user role
    KeyBoard::CKeyBoard *mp_KeyBoardWidget;         //!< Reference to Keyboard widget
    KeyBoard::ValidationType_t m_ValidationType;    //!< Entered text validation type
    ButtonType_t m_ButtonType;                      //!< Button Type
    DataManager::CUserSettings m_UserSettings;      //!< UserSettings object
    QString m_Password;                             //!< String to hold Password
    void RetranslateUI();
    void ResetButtons();

protected:
    void changeEvent(QEvent *p_Event);
    void showEvent(QShowEvent *p_Event);
    void Update();
    void UpdateOnESC();

public:
     void SetUserSettings(DataManager::CUserSettings *p_UserSettings);

private slots:    
    void OnOkClicked();
    void OnProcessStateChanged();
    void OnDirectConnectionStateChanged(int State);
    void OnProxyUserName();
    void OnProxyPassword();
    void OnProxyIPAddress();
    void OnProxyPort();
    void OnSave();

signals:
     void SettingsChanged(DataManager::CUserSettings &Settings);
};

} // end namespace Settings

#endif // SETTINGS_NETWORKSETTINGSWIDGET_H
