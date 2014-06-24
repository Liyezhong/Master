/****************************************************************************/
/*! \file DataConnector.h
 *
 *  \brief DataConnector definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-02-26
 *   $Author:  $ Srivathsa HH
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
#ifndef CORE_SERVICEGUICONNECTOR_H
#define CORE_SERVICEGUICONNECTOR_H

#include "DataManager/Containers/DeviceConfiguration/Include/DeviceConfigurationInterface.h"
#include "DataManager/Containers/UserSettings/Include/UserSettingsInterface.h"
#include "ServiceDataManager/Include/ModuleDataList.h"
#include <ServiceDataManager/Include/ServiceParameters.h>

#include "Global/Include/Translator.h"
#include "MainMenu/Include/MainWindow.h"
#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/WaitDialog.h"
#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/MsgBoxManager.h"

#include "Diagnostics/Include/BasicColor/BasicColorTestDlg.h"

#include <QEventLoop>

namespace Core {

// Message box buttons
enum ClickedButton_t {
    OK_BUTTON,
    YES_BUTTON,
    CONTINUE_BUTTON,
    NO_BUTTON,
    CANCEL_BUTTON,
    STOP_BUTTON
};

/****************************************************************************/
/**
 *  \brief Controller connecting the data model to various views
 *
 *      This class receives commands from the DCP, stores the
 *      data in the data manager and notifies the different views of the
 *      changes.
 */
/****************************************************************************/
class CServiceGUIConnector : public QObject
{
    Q_OBJECT

    MainMenu::CMainWindow   *mp_MainWindow;             //!< Main window of the GUI
    MainMenu::CMessageDlg   *mp_MessageDlg;             //!< Error message box
    MainMenu::CWaitDialog   *mp_WaitDialog;             //!< Displayed when busy

    QFile *mp_LanguageFile;                             //!< Lanugage file object (qm file)
    QFile *mp_OldFile;                                  //!< Old/Previous language file(qm file)

    QEventLoop m_LoopSetAction;                         //!< Loop for blocking commands.

    bool m_GuiAllConfigurationReceived;                 //!< True -if all configuration data (XML)from Main is received
    bool m_GuiInit;                                     //!< True - if Gui is getting initialized
    int  m_CurrentTabIndex;                             //!< To keep track of current tab selected
    bool m_MessageDlg;                                  //!< True if Message Dialog is displayed.

    MainMenu::CMsgBoxManager *mp_MesgBoxManager;        //!< Msg Box manager for CmdEventReport

    ServiceDataManager::CModuleDataList         *mp_ModuleList;                     //!< Container for Instrument History
    DataManager::CDeviceConfigurationInterface  *mp_DeviceConfigurationInterface;   //!< Container for Device configuration
    DataManager::CUserSettingsInterface         *mp_SettingsInterface;              //!< Provides interface to read the Settings info from xml
    DataManager::CServiceParameters             *mp_ServiceParameters;              //!< Container for Service Parameters
    //Diagnostics::CBasicColorTestDlg             *mp_ColorTestDialog;                //!< Displayed when ColorTest needs to be done
    //MainMenu::CBasicColorTestDlg                 *mp_ColorTestDialog;                //!< Displayed when ColorTest needs to be done

public:
    CServiceGUIConnector(MainMenu::CMainWindow *p_Parent);
    virtual ~CServiceGUIConnector();

    MainMenu::CWaitDialog* GetWaitDlgPtr() {return mp_WaitDialog;}

    void SetModuleListContainer(ServiceDataManager::CModuleDataList *ModuleList);

    void SetDeviceConfigurationInterface(
            DataManager::CDeviceConfigurationInterface *DeviceConfigInterface);

    void SetUserSettingInterface(
            DataManager::CUserSettingsInterface *UserSettingInterface);

    void ShowMessageDialog(Global::GUIMessageType MessageType, QString MessageText, bool NeedClickFlag=false);
    void ShowBusyDialog(QString MessageText, bool HideAbort = true);
    void ShowBusyDialog();
    void HideBusyDialog();

    ServiceDataManager::CModuleDataList* GetModuleListContainer();

    DataManager::CDeviceConfigurationInterface *GetDeviceConfigInterface(void);

    DataManager::CUserSettingsInterface *GetUserSettingInterface(void);
    DataManager::CServiceParameters* GetServiceParameters();
    void SetServiceParametersContainer(DataManager::CServiceParameters *ServiceParameters);

public slots:
    void OnCurrentTabChanged(int CurrentTabIndex);

    void onPopUpButtonClicked(qint32 button);

    void onMessageDialogClosed();

    void SendModuleUpdate(ServiceDataManager::CModule &Module);

    void SendAddModule(ServiceDataManager::CModule &Module);
    void ServiceParametersUpdates(DataManager::CServiceParameters *ServiceParameters);
    void HandleTimeout();
signals:
    void ModuleListContainerInitialized(ServiceDataManager::CModuleDataList& ModuleList);

    void ModuleListChanged(void);

    void DeviceConfigurationInterfaceChanged(void);

    void UserSettingInterfaceChanged(void);
    /****************************************************************************/
    /*!
     *  \brief Signal is emitted when ServiceParameters data container is
     *  initialized
     */
    /****************************************************************************/
    void ServiceParametersChanged();
    /*******************************************************************************/
    /*!
     *  \brief Signal is emitted to update ServiceParameters container
     *  \iparam ServiceParameters = Service Parameters object
     */
    /*******************************************************************************/
    void UpdateServiceParameters(DataManager::CServiceParameters *ServiceParameters);
private slots:
    void SetDateTime(const QDateTime& DateTime);
    void RetranslateUI();
    void SendEventReportCommand(const quint32, const Global::tTranslatableStringList &, const bool, quint32 EventKey, const Global::AlternateEventStringUsage);
    void OnEventReportAck(NetCommands::ClickedButton_t ClickedButton, Global::tRefType CmdRef, quint64 EventKey);
};

} // end namespace Core

#endif // CORE_DATACONNECTOR_H
