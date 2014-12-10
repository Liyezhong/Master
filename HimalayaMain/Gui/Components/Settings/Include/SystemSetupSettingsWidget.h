/****************************************************************************/
/*! \file SystemSetupSettingsWidget.h
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

#ifndef SETTINGS_SYSTEMSETUPSETTINGSWIDGET_H
#define SETTINGS_SYSTEMSETUPSETTINGSWIDGET_H

#include "DataManager/Containers/UserSettings/Include/UserSettings.h"
#include "HimalayaDataContainer/Containers/UserSettings/Include/HimalayaUserSettings.h"
#include "MainMenu/Include/MainWindow.h"
#include "MainMenu/Include/PanelFrame.h"
#include "MainMenu/Include/ScrollWheel.h"
#include <QShowEvent>
#include "Settings/Include/PrecheckProgramModel.h"
#include "MainMenu/Include/BaseTable.h"

namespace Core {
    class CDataConnector;
}

namespace Settings {

namespace Ui {
    class CSystemSetupSettingsWidget;
}

/****************************************************************************/
/**
 * \brief Setup panel for the agitation device of Himalaya
 */
/****************************************************************************/
class CSystemSetupSettingsWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT
    friend class CTestSettings;

private:
    Ui::CSystemSetupSettingsWidget *mp_Ui;                 //!< User interface
    MainMenu::CScrollWheel *mp_ScrollWheel;         //!< Temperature scroll wheel
    DataManager::CHimalayaUserSettings *mp_UserSettings;    //!< Data object
    DataManager::CHimalayaUserSettings m_UserSettingsTemp;  //!< Temporary copy of User Settings
    MainMenu::CMainWindow *mp_MainWindow;           //!< Pointer to MainWindow
    //Flags
    bool m_ProcessRunning;                          //!< Process running state
    //UI related
    MainMenu::CMainWindow::UserRole_t m_CurrentUserRole;    //!< Current user role
    QString m_strChangeMeltPointConfirm12Hrs;
    QString m_strConfirmChangeMeltingPoint;
    CPrecheckProgramModel m_PrecheckProgramModel;
    MainMenu::CBaseTable *mp_TableWidget;           //!< Language table
    Core::CDataConnector *mp_DataConnector;


public:
    explicit CSystemSetupSettingsWidget(QWidget *p_Parent = NULL);
    ~CSystemSetupSettingsWidget();
    void SetUserSettings(DataManager::CHimalayaUserSettings *p_UserSettings);
    void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow);
    void SetDataConnector(Core::CDataConnector *p_DataConnector);

private:
    void InitTemperatureWidget();
    void RetranslateUI();
    void ResetButtons();

protected:
    void changeEvent(QEvent *p_Event);
    void showEvent(QShowEvent *p_Event);

private slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnProcessStateChanged
     */
    /****************************************************************************/
    void OnProcessStateChanged();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnUserRoleChanged
     */
    /****************************************************************************/
    void OnUserRoleChanged();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of OnApply
     */
    /****************************************************************************/
    void OnApply();

signals:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal TemperatureChanged
     *  \param Settings = the user settings
     */
    /****************************************************************************/
    void TemperatureChanged(DataManager::CUserSettings &Settings);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ParaffinMeltPointchanged
     *  \param lastMeltPoint = the paraffin melt point in last time
     *  \param currentMeltPoint = the new set paraffin melt point
     */
    /****************************************************************************/
    void ParaffinMeltPointchanged(int lastMeltPoint, int currentMeltPoint);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal UpdateProgram
     */
    /****************************************************************************/
    void UpdateProgram(DataManager::CProgram &);
};


} // end namespace Settings

#endif // SETTINGS_AGITATIONSETTINGSWIDGET_H

