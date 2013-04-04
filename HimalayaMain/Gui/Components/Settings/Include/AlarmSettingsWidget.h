/****************************************************************************/
/*! \file AlarmSettingsWidget.h
 *
 *  \brief AlarmSettingsWidget definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-20
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

#ifndef SETTINGS_ALARMSETTINGSWIDGET_H
#define SETTINGS_ALARMSETTINGSWIDGET_H

#include "Core/Include/DataConnector.h"
#include "MainMenu/Include/MainWindow.h"
#include "MainMenu/Include/PanelFrame.h"
#include "MainMenu/Include/MainWindow.h"
#include "Settings/Include/AlarmSettingsDlg.h"
#include <QShowEvent>

namespace Settings {

namespace Ui {
    class CAlarmSettingsWidget;
}

/****************************************************************************/
/**
 * \brief Main widget for the configuration of alarm sounds
 */
/****************************************************************************/
class CAlarmSettingsWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT

public:
    explicit CAlarmSettingsWidget(QWidget *p_Parent = 0);
    ~CAlarmSettingsWidget();
    void SetUserSettings(DataManager::CUserSettings *p_UserSettings);
    void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow);
    /****************************************************************************/
    /*!
     *  \brief Sets pointer to DataConnector object
     *
     *  \iparam p_DataConnecotr
     */
    /****************************************************************************/
    void SetDataConnector(Core::CDataConnector *p_DataConnector)
    {
        mp_DataConnector = p_DataConnector;
        CONNECTSIGNALSLOT(mp_DataConnector, UserSettingsUpdated(), this, UpdateLabels());
    }

private:
    Ui::CAlarmSettingsWidget *mp_Ui;    //!< User interface
    Settings::CAlarmSettingsDlg *mp_Error;  //!< Error alarm dialog
    Settings::CAlarmSettingsDlg *mp_Information;   //!< Note alarm dialog
    Settings::CAlarmSettingsDlg *mp_Warning;   //!< Note alarm dialog
    DataManager::CUserSettings m_UserSettings;  //!< Data object
    Core::CDataConnector *mp_DataConnector; //!< DataConnector object
    void RetranslateUI();
    void ResetButtons();
    MainMenu::CMainWindow *mp_MainWindow;           //!< Pointer to MainWindow
    //Flags
    bool m_ProcessRunning;                  //!< Process running state
    //UI related
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;    //! < Current user role

private slots:
    void OnNoteEdit();
    void OnWarningEdit();
    void OnErrorEdit();
    void OnProcessStateChanged();
    void UpdateLabels();
    void AlarmSettingsChange(DataManager::CUserSettings &Settings);

protected:
    void changeEvent(QEvent *p_Event);
    void showEvent(QShowEvent *p_Event);

signals:
    void AlarmSettingsChanged(DataManager::CUserSettings &Settings);
    void PlayTestTone(quint8 , quint8 , bool );
};

} // end namespace Settings

#endif // SETTINGS_ALARMSETTINGSWIDGET_H
