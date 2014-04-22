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
    void SetUserSettings(DataManager::CHimalayaUserSettings *p_UserSettings);
    void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow);
    /****************************************************************************/
    /*!
     *  \brief Sets pointer to DataConnector object
     *
     *  \iparam p_DataConnector
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
    DataManager::CHimalayaUserSettings m_UserSettings;  //!< Data object
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;    //!< Current user role
    Core::CDataConnector *mp_DataConnector; //!< DataConnector object
    void RetranslateUI();
    void ResetButtons();
    MainMenu::CMainWindow *mp_MainWindow;           //!< Pointer to MainWindow
    //Flags
    bool m_ProcessRunning;                  //!< Process running state
    //UI related

    // texts to be displayed on the widget
    QString m_TextSound;       ///<  Definition/Declaration of variable m_TextSound
    QString m_TextVolume;       ///<  Definition/Declaration of variable m_TextVolume
    QString m_TextPeriodicOff;       ///<  Definition/Declaration of variable m_TextPeriodicOff
    QString m_TextPeriodicTime;       ///<  Definition/Declaration of variable m_TextPeriodicTime

private slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnNoteEdit();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnWarningEdit();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnErrorEdit();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void OnProcessStateChanged();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void UpdateLabels();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot UpdateReagentList
     */
    /****************************************************************************/
    void AlarmSettingsChange(DataManager::CUserSettings &Settings);

protected:
    void changeEvent(QEvent *p_Event);
    void showEvent(QShowEvent *p_Event);

signals:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal AlarmSettingsChanged
     */
    /****************************************************************************/
    void AlarmSettingsChanged(DataManager::CUserSettings &Settings);
    void PlayTestTone(quint8 , quint8 , bool );
};

} // end namespace Settings

#endif // SETTINGS_ALARMSETTINGSWIDGET_H
