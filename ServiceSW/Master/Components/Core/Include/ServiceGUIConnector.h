/****************************************************************************/
/*! \file ServiceGUIConnector.h
 *
 *  \brief ServiceGUIConnector definition.
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
#include "ServiceDataManager/Include/InstrumentHistory.h"
#include "ServiceDataManager/Include/ServiceParameters.h"
#include "../Include/PlatformServiceDefines.h"

#include "Global/Include/Translator.h"
#include "MainMenu/Include/MainWindow.h"
#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/WaitDialog.h"
#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/MsgBoxManager.h"

#include "Diagnostics/Include/BasicColor/BasicColorTestDlg.h"

#include <QEventLoop>
#include <QTranslator>

namespace Core {

/****************************************************************************/
/**
 * \brief Enum Clicked button Types.
 */
/****************************************************************************/
enum ClickedButton_t {
    OK_BUTTON,      //!< OK
    YES_BUTTON,     //!< Yes
    CONTINUE_BUTTON,//!< Continue
    NO_BUTTON,      //!< No
    CANCEL_BUTTON,  //!< Cancel
    STOP_BUTTON     //!< Stop
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

    PlatformService::Languages_t m_Language;            //!< Language enu object
    QTranslator m_Translator;                           //!< Language translator object

    QEventLoop m_LoopSetAction;                         //!< Loop for blocking commands.

    //bool m_GuiAllConfigurationReceived;                 //!< True -if all configuration data (XML)from Main is received
    bool m_GuiInit;                                     //!< True - if Gui is getting initialized
    int  m_CurrentTabIndex;                             //!< To keep track of current tab selected
    bool m_MessageDlg;                                  //!< True if Message Dialog is displayed.
    bool m_Archive;                                     //!< True to write InstrumentHistoryArchive.xml

    MainMenu::CMsgBoxManager *mp_MesgBoxManager;        //!< Msg Box manager for CmdEventReport

    ServiceDataManager::CModuleDataList         *mp_ModuleList;                     //!< Container for Instrument History
    ServiceDataManager::CInstrumentHistory      *mp_ModuleListArchive;                 //!< Container for Instrument History Archive
    DataManager::CDeviceConfigurationInterface  *mp_DeviceConfigurationInterface;   //!< Container for Device configuration
    DataManager::CUserSettingsInterface         *mp_SettingsInterface;              //!< Provides interface to read the Settings info from xml
    DataManager::CServiceParameters             *mp_ServiceParameters;              //!< Container for Service Parameters
    //Diagnostics::CBasicColorTestDlg             *mp_ColorTestDialog;                //!< Displayed when ColorTest needs to be done
    //MainMenu::CBasicColorTestDlg                 *mp_ColorTestDialog;                //!< Displayed when ColorTest needs to be done

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_Parent = Parent widget
     */
    /****************************************************************************/
    CServiceGUIConnector(MainMenu::CMainWindow *p_Parent);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    virtual ~CServiceGUIConnector();

    /****************************************************************************/
    /*!
     *  \brief To get wait dialog pointer.
     *  \return the pointer of wait dialog
     */
    /****************************************************************************/
    MainMenu::CWaitDialog* GetWaitDlgPtr() {return mp_WaitDialog;}

    /****************************************************************************/
    /*!
     *  \brief To set module list container
     *  \iparam ModuleList = module list
     *  \iparam ModuleListArchive = module list archive
     */
    /****************************************************************************/
    void SetModuleListContainer(ServiceDataManager::CModuleDataList *ModuleList, ServiceDataManager::CInstrumentHistory *ModuleListArchive);

    /****************************************************************************/
    /*!
     *  \brief To set device configuration interface
     *  \iparam DeviceConfigInterface = the pointer of device config
     */
    /****************************************************************************/
    void SetDeviceConfigurationInterface(
            DataManager::CDeviceConfigurationInterface *DeviceConfigInterface);

    /****************************************************************************/
    /*!
     *  \brief To Set User setting interface
     *  \iparam UserSettingInterface = the pointer of user setting interface.
     */
    /****************************************************************************/
    void SetUserSettingInterface(
            DataManager::CUserSettingsInterface *UserSettingInterface);

    /****************************************************************************/
    /*!
     *  \brief To show message dialog
     *  \iparam MessageType = message type
     *  \iparam MessageText = message text
     *  \iparam NeedClickFlag = flag of need click
     */
    /****************************************************************************/
    void ShowMessageDialog(Global::GUIMessageType MessageType, QString MessageText, bool NeedClickFlag=false);

    /****************************************************************************/
    /*!
     *  \brief To Show Busy dialog
     *  \iparam MessageText = message text
     *  \iparam HideAbort = flag of hide abort
     */
    /****************************************************************************/
    void ShowBusyDialog(QString MessageText, bool HideAbort = true);

    /****************************************************************************/
    /*!
     *  \brief To Show busy dialog
     */
    /****************************************************************************/
    void ShowBusyDialog();

    /****************************************************************************/
    /*!
     *  \brief To Hide busy dialog
     */
    /****************************************************************************/
    void HideBusyDialog();

    /****************************************************************************/
    /*!
     *  \brief To Get module list container
     *  \return The pointer of Module Data list
     */
    /****************************************************************************/
    ServiceDataManager::CModuleDataList* GetModuleListContainer();

    /****************************************************************************/
    /*!
     *  \brief To Get module list archive container
     *  \return The pointer of CInstrumentHistory
     */
    /****************************************************************************/
    ServiceDataManager::CInstrumentHistory* GetModuleListArchiveContainer();

    /****************************************************************************/
    /*!
     *  \brief To Get Device config interface
     *  \return The pointer of CDeviceConfigurationInterface
     */
    /****************************************************************************/
    DataManager::CDeviceConfigurationInterface *GetDeviceConfigInterface(void);

    /****************************************************************************/
    /*!
     *  \brief To Get user setting interface
     *  \return The pointer of CUserSettingsInterface
     */
    /****************************************************************************/
    DataManager::CUserSettingsInterface *GetUserSettingInterface(void);

    /****************************************************************************/
    /*!
     *  \brief To get service parameters
     *  \return The pointer of CServiceParameters
     */
    /****************************************************************************/
    DataManager::CServiceParameters* GetServiceParameters();

    /****************************************************************************/
    /*!
     *  \brief Set service parameters container
     *  \iparam ServiceParameters = The pointer of CServiceParameters
     */
    /****************************************************************************/
    void SetServiceParametersContainer(DataManager::CServiceParameters *ServiceParameters);

public slots:
    /****************************************************************************/
    /**
     * \brief Sets the current language
     * \iparam  SelectedLanguage = Current language selected
     */
    /****************************************************************************/
    void SetLanguage(PlatformService::Languages_t SelectedLanguage);

    /****************************************************************************/
    /*!
     *  \brief Slot of current tab changed
     *  \iparam CurrentTabIndex = Index of current tab
     */
    /****************************************************************************/
    void OnCurrentTabChanged(int CurrentTabIndex);

    /****************************************************************************/
    /*!
     *  \brief Slot of pop up button clicked
     *  \iparam button = button index
     */
    /****************************************************************************/
    void onPopUpButtonClicked(qint32 button);

    /****************************************************************************/
    /*!
     *  \brief Slot of message dialog closed
     */
    /****************************************************************************/
    void onMessageDialogClosed();

    /****************************************************************************/
    /*!
     *  \brief Slot of send Module update
     *  \iparam Module = The module data
     */
    /****************************************************************************/
    void SendModuleUpdate(ServiceDataManager::CModule &Module);

    /****************************************************************************/
    /*!
     *  \brief Slot for send add module
     *  \iparam Module = The module data
     */
    /****************************************************************************/
    void SendAddModule(ServiceDataManager::CModule &Module);

    /****************************************************************************/
    /*!
     *  \brief Slot for service parameters update
     *  \iparam ServiceParameters = The pointer of CServiceParameters
     */
    /****************************************************************************/
    void ServiceParametersUpdates(DataManager::CServiceParameters *ServiceParameters);

    /****************************************************************************/
    /*!
     *  \brief Slot for handle timeout
     */
    /****************************************************************************/
    void HandleTimeout();

    /****************************************************************************/
    /*!
     *  \brief Slot for send device configuration update
     *  \iparam DeviceConfiguration = The pointer of Device Configuration.
     */
    /****************************************************************************/
    void SendDeviceConfigurationUpdate(DataManager::CDeviceConfiguration* DeviceConfiguration);

    /****************************************************************************/
    /*!
     *  \brief  To update the Instrument History xml file
     *  \iparam ModuleList = Module list date container
     */
    /****************************************************************************/
    bool UpdateInstrumentHistory(ServiceDataManager::CModuleDataList& ModuleList);
signals:
    /****************************************************************************/
    /*!
     *  \brief Signal is emitted when ModuleList data container is
     *  initialized
     */
    /****************************************************************************/
    void ModuleListContainerInitialized(ServiceDataManager::CModuleDataList& ModuleList);

    /****************************************************************************/
    /*!
     *  \brief Signal is emitted when ModuleList data container is changed
     */
    /****************************************************************************/
    void ModuleListChanged(void);

    /****************************************************************************/
    /*!
     *  \brief Signal is emitted when DeviceConfiguration data container is changed
     */
    /****************************************************************************/
    void DeviceConfigurationInterfaceChanged(void);

    /****************************************************************************/
    /*!
     *  \brief Signal is emitted when UserSettings data container is changed
     */
    /****************************************************************************/
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
