/****************************************************************************/
/*! \file AlarmSettingsDlg.h
 *
 *  \brief AlarmSettingsDlg definition.
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

#ifndef SETTINGS_ALARMSETTINGSDLG_H
#define SETTINGS_ALARMSETTINGSDLG_H

#include "DataManager/Containers/UserSettings/Include/UserSettings.h"
#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/MainWindow.h"
#include "MainMenu/Include/ScrollWheel.h"
#include <QButtonGroup>

namespace Settings {

namespace Ui {
    class CAlarmSettingsDlg;
}

/****************************************************************************/
/**
 * \brief Dialog for changing the alarm settings
 */
/****************************************************************************/
class CAlarmSettingsDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    explicit CAlarmSettingsDlg(bool Error, QWidget *p_Parent = 0);

   CAlarmSettingsDlg();

    ~CAlarmSettingsDlg();
    void UpdateDisplay(qint32 Volume, qint32 Sound);
    void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow);
    bool m_Type;

    /****************************************************************************/
    /*!
     *  \brief Sets pointer to UserSettings
     *
     *  \iparam p_UserSettings
     */
    /****************************************************************************/
    void SetUserSettings(DataManager::CUserSettings *p_UserSettings) { mp_UserSettings = p_UserSettings;}
    /****************************************************************************/
    /*!
     *  \brief Used by Alarm Settings widget to set the dialog type -warning/ error
     *
     *  \iparam DialogType = True - error, False - warning
     */
    /****************************************************************************/
    void SetDialogType(bool DialogType) { m_ErrorAlarmScreen = DialogType; }

private:
    Ui::CAlarmSettingsDlg *mp_Ui;   //!< User interface
    MainMenu::CScrollWheel *mp_ScrollWheel; //!< Volume scroll wheel
    MainMenu::CScrollWheel *mp_SoundScrollWheel; //!< Volume scroll wheel
    MainMenu::CScrollWheel *mp_SecondWheel;   //!< Second scroll wheel
    MainMenu::CScrollWheel *mp_MinWheel;        //!< Minut scroll wheel
    QButtonGroup m_ButtonGroup; //!< Groups radio buttons for sound selection
    MainMenu::CMainWindow *mp_MainWindow;           //!< Pointer to MainWindow
    DataManager::CUserSettings *mp_UserSettings;    //!< Data object
    DataManager::CUserSettings m_UserSettingsTemp;  //!< Temporary copy of User Settings
    //Flags
    bool m_ProcessRunning;  //!< Process running state
    bool m_ErrorAlarmScreen;  //!< Tells if the screen shown is error alarm setting.
    void RetranslateUI();
    void ResetButtons();

protected:
    void changeEvent(QEvent *p_Event);
    void showEvent(QEvent *p_Event);

private slots:
    void OnProcessStateChanged();
    void OnApply();
    void OnPlayTone();

signals:
    void AlarmSettingsChanged(DataManager::CUserSettings &Settings);
    void PlayTestTone( quint8, quint8, bool );
};

} // end namespace Settings

#endif // SETTINGS_ALARMSETTINGSDLG_H
