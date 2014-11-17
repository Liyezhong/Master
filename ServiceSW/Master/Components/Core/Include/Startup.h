/****************************************************************************/
/*! \file Startup.h
 *
 *  \brief Startup definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-02-28
 *   $Author:  $ Srivathsa
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

#ifndef CORE_STARTUP_H
#define CORE_STARTUP_H

#include "ServiceMasterThreadController/Include/ServiceMasterThreadController.h"
#include "Core/Include/ServiceGUIConnector.h"
#include "ServiceKeyValidator/Include/USBKeyValidator.h"

#include "Core/Include/ServiceDefines.h"

#include "MainMenu/Include/MainWindow.h"
#include "MainMenu/Include/MenuGroup.h"

#include "MainMenu/Include/DateTime.h"
#include "MainMenu/Include/DataManagement.h"
#include "MainMenu/Include/MessageDlg.h"

#include "SystemTracking/Include/HiMenuGroup.h"
#include "SystemTracking/Include/CurrentConfiguration.h"
#include "SystemTracking/Include/AddModifyConfiguration.h"
#include "SystemTracking/Include/ViewHistory.h"
#include "SystemTracking/Include/MainControl.h"
#include "SystemTracking/Include/Retort.h"
#include "SystemTracking/Include/Oven.h"
#include "SystemTracking/Include/RotaryValve.h"
#include "SystemTracking/Include/LaSystem.h"

#include "LogViewer/Include/LogViewer.h"
#include "LogViewerDialog/Include/LogContentDlg.h"
#include "LogViewerDialog/Include/SystemLogViewerDlg.h"

#include "Calibration/Include/PressureSensor.h"
#include "Calibration/Include/Touchscreen.h"

#include "ServiceUpdates/Include/FirmwareUpdate.h"
#include "ServiceUpdates/Include/DataManagement.h"
#include "ServiceUpdates/Include/Settings.h"
#include "ServiceUpdates/Include/System.h"

#include "Settings/Include/DataManagement.h"

#include "Diagnostics/Include/Display.h"
#include "Diagnostics/Include/Retort.h"
#include "Diagnostics/Include/Oven.h"
#include "Diagnostics/Include/RotaryValve.h"
#include "Diagnostics/Include/LaSystem.h"
#include "Diagnostics/Include/System.h"
#include "Diagnostics/Include/MainControl.h"
#include "Core/Include/CalibrationHandler.h"
#include "Core/Include/ManufacturingDiagnosticsHandler.h"
#include "DiagnosticsManufacturing/Include/SealingTestReportDialog.h"
#include <QTimer>
#include "Diagnostics/Include/ServiceDeviceProcess/ServiceDeviceProcess.h"

namespace SVCDiagnostics{
    class CSVCDashboardWidget;
}

namespace SVCScreenLock {
    class CSVCScreenLockWidget;
}

namespace ServiceKeyValidator {
    class CUSBKeyValidator;
}

namespace Core {

/****************************************************************************/
/**
 * \brief This class handles the initialization of the user interface.
 */
/****************************************************************************/
class CStartup : public QObject
{
    Q_OBJECT

public:
    CStartup();
    virtual ~CStartup();
    qint32 NetworkInit();
    void GuiInit();
    void GuiInit(QString debugMode);
    int Mode;

    Core::CServiceGUIConnector *mp_ServiceConnector;
    Core::CManufacturingDiagnosticsHandler *mp_ManaufacturingDiagnosticsHandler;    //!< Manufacturing diagnostics handler object


    void LoadCommonComponenetsOne(bool bReInit = false);    // System Tracking and Log Viewer Tab
    void LoadCommonComponenetsTwo();    // Calibration and Service Updates Tab
    void ServiceGuiInit();
    void ManufacturingGuiInit(bool bReInit = false);
    void InitManufacturingDiagnostic();
  /****************************************************************************/
    /**
     * \brief Sets the current user mode
     * \iparam UserMode = Current user mode
     */
    /****************************************************************************/
    void SetCurrentUserMode(QString UserMode) {
        m_CurrentUserMode = UserMode;
    }

    /****************************************************************************/
    /**
     * \brief Gets current user mode
     * \return Current user mode
     */
    /****************************************************************************/
    QString GetCurrentUserMode() {
        return m_CurrentUserMode;
    }

private:

    bool        m_SelfTestFinished;                     //!< Check if system self test is finished.

    QTimer      *mp_Clock;                                  //!< The main clock
    QDateTime   m_DateTime;                                 //!< The current date and time

    // GUI components   
    MainMenu::CMainWindow                   *mp_MainWindow;                 //!< The main window of the application
    ServiceKeyValidator::CUSBKeyValidator                  *mp_USBKeyValidator;

    // System Tracking
    MainMenu::CMenuGroup                  *mp_SystemTrackingGroup;        //!< Menu group containing System Tracking data
    SystemTracking::CCurrentConfiguration   *mp_CurrentConfiguration;       //!< Current Configuration information screen
    SystemTracking::CAddModifyConfig        *mp_AddModifyConfig;            //!< Add or Modify configuration screen
    SystemTracking::CViewHistory            *mp_ViewHistory;                //!< Module History
    MainMenu::CMenuGroup                  *mp_AddModifyConfigGroup;       //!< Add or Modify configuration screen group
    SystemTracking::CMainControl            *mp_MainControlConfig;          //!< Configuration screen for Main control
    SystemTracking::CRetort                 *mp_RetortConfig;               //!< Configuration screen for Retort
    SystemTracking::COven                   *mp_OvenConfig;                 //!< Configuration screen for Oven
    SystemTracking::CRotaryValve            *mp_RotaryValveConfig;          //!< Configuration screen for Rotary Valve
    SystemTracking::CLaSystem               *mp_LaSystemConfig;             //!< Configuration screen for L&A System
    Settings::CDataManagement               *mp_DataMgmt;                   //!< Data Management - import and export

    // Log Viewer
    MainMenu::CMenuGroup                    *mp_LogViewerGroup;             //!< Log information of module data

    LogViewer::CLogViewer             *mp_SystemLogViewer;            //!< System Log information
    LogViewer::CLogViewer              *mp_ServiceHelpText;             //!< Service Help Text.
    LogViewer::CLogViewer            *mp_ServiceLogViewer;           //!< Service Log Viewer
    LogViewer::CLogViewer           *mp_SoftwareUpdateLogViewer;    //!< Software Update Log Viewer
    LogViewer::CLogContentDlg       *mp_LogContentDlg;              //!< Log Content Dialog
    LogViewer::CSystemLogViewerDlg  *mp_SystemLogContentDlg;        //!< System Log Content Dialog



    //Diagnostics
    MainMenu::CMenuGroup                    *mp_DiagnosticsGroup;
    Diagnostics::CDisplay                   *mp_Display;                    //!< Display tests
    Diagnostics::CRetort                    *mp_Retort;                     //!< Retort tests
    Diagnostics::COven                      *mp_Oven;                       //!< Oven tests
    Diagnostics::CRotaryValve               *mp_RotaryValve;                //!< Rotary Valve tests
    Diagnostics::CLaSystem                  *mp_LaSystem;                   //!< L&A System tests
    Diagnostics::CSystem                    *mp_System;                     //!< System tests
    Diagnostics::CMainControl               *mp_MainControl;                //!< Main Control tests

    //SVC Diagnostics
    SVCDiagnostics::CSVCDashboardWidget      *mp_SVCDashboardWidget;

    //SVC Screen lock
    SVCScreenLock::CSVCScreenLockWidget     *mp_SVCSceenLockWidget;

    // Diagnostics1 Manufacturing
    MainMenu::CMenuGroup                    *mp_DiagnosticsManufGroup;

    //Service Updates
    MainMenu::CMenuGroup                    *mp_ServiceUpdateGroup;         //!< Menu group containing Service Update dialogs
    ServiceUpdates::CFirmwareUpdate         *mp_FirmwareUpdate;             //!< Firmware update page
    ServiceUpdates::CSettings               *mp_Setting;                    //!< User settings sub menu
    ServiceUpdates::CSystem                 *mp_UpdateSystem;                   //!< Shut down page


    //Message Box, Busy Box, Generic PoP Up.
    MainMenu::CMessageDlg *mp_MessageBox;
    Core::CCalibrationHanlder *mp_CalibrationHandler;
    QString m_CurrentUserMode;                                              //!< Stores current user mode

    QString m_DeviceName;                                                  //!< Name of the device
    QTimer m_WindowStatusResetTimer;                                        //!< Timer for Window status reset

    DiagnosticsManufacturing::CHeatingTestDialog *mp_HeatingStatusDlg;
    DiagnosticsManufacturing::CSealingTestReportDialog* mp_SealingStatusDlg;

    Diagnostics::ServiceDeviceProcess* mp_ServDevProc;

private slots:
    void SetDateTime(QDateTime DateTime);
    void UpdateDateTime();
    void OnSelectTestOptions(int Index);
	void InitializeGui(PlatformService::SoftwareModeType_t, QString);
    int FileExistanceCheck();

    void OnCurrentTabChanged(int TabIndex);
private:
    bool CurrentlyActive(MainMenu::CMenuGroup *p_Group, QWidget *p_Panel);
    void RefreshTestStatus4RetortCoverSensor(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status);
    void RefreshTestStatus4RetortHeatingEmpty(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status);
    void RefreshTestStatus4RetortHeatingWithWater(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status);
    void RefreshTestStatus4RetortLevelSensorHeating(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status);
    void RefreshTestStatus4RetortLevelSensorDetecting(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status);

    void RefreshTestStatus4OvenCoverSensor(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status);
    void RefreshTestStatus4OvenHeatingEmpty(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status);
    void RefreshTestStatus4OvenHeatingWater(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status);
    void RefreshTestStatus4LAHeatingBelt(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status);
    void RefreshTestStatus4RVHeating(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status);
    void RefreshTestStatus4SystemSpeaker(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status);
    void RefreshTestStatus4SystemAlarm(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status);
    void RefreshTestStatus4SystemMainsRelay(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status);
    void RefreshTestStatus4SystemExhaustFan(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status);
    void RefreshTestStatus4SystemOverflow(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status);
    void RefreshTestStatus4SystemSealing(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status);
    void RefreshTestStatus4CleaningSystem(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status);
    void RefreshTestStatus4FirmwareUpdate(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status);
    void RefreshTestStatus4FirmwareGetSlaveInfo(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status);


public slots:
    void DisplayLogInformation(QString FileName, QString FilePath);

    void OnDeviceInitRequest();

    /* Return Message Slots */
    void ShowMessage(const QString &Message);
    void ShowErrorMessage(const QString &Message);
    void ShowCalibrationInitMessagetoMain(const QString &Message, bool OkStatus);

    /* Refresh heating status */
    void RefreshTestStatus(const QString &Message, const Service::ModuleTestStatus &Status);
    void OnReturnManufacturingMsg(bool Result);

    /* General slots */
    void OnGuiAbortTest();

    /* Display slots*/
    void OnBasicColorTest();


    void RetranslateUI();

    void IdleTimeout();
    void ResetWindowStatusTimer();
    void UpdateParameters();
    void StartTimer();

    void StartImportExportProcess(QString Name, QString Type);
    void ImportExportCompleted(int ExitCode, bool IsImport);
    void SendFileSelectionToGUI(QStringList FileList);
    void SendFilesToMaster(QStringList FileList);

    void OnReturnServiceRequestResult(QString ReqName, int ErrorCode, QStringList Results);

signals:
    /****************************************************************************/
    /**
     * \brief Signal is emitted to Device Init request
     */
    /****************************************************************************/
    void DeviceInitRequest(void);

    /****************************************************************************/
    /**
     * \brief Signal is emitted to Import&Export data file Request
     * \iparam CommandName = Import/Export
     * \iparam CommandData = data of Import/Export
     */
    /****************************************************************************/
    void ImportExportDataFileRequest(const QString &CommandName,
                                     const QByteArray &CommandData);

    /****************************************************************************/
    /**
     * \brief Signal is emitted to Master from GUI
     * \iparam Name = Import/Export
     * \iparam Type = Type of Import/Export
     */
    /****************************************************************************/
    void ImportExportProcess(QString Name, QString Type);

    /****************************************************************************/
    /**
     * \brief Signal is emitted to GUI
     * \iparam EventCode = Event code
     * \iparam IsImport = True for Import and False for Export
     */
    /****************************************************************************/
    void SendSignaltoGUI(int EventCode, bool IsImport);

    /****************************************************************************/
    /**
     * \brief Signal is emitted to send import files to GUI
     * \iparam FileNames = File names
     */
    /****************************************************************************/
    void SendFilesToGUI(QStringList FileNames);

    /****************************************************************************/
    /**
     * \brief Signal is emitted to Master from GUI
     * \iparam SelectedFileName = Selected filenames
     */
    /****************************************************************************/
    void SendSignalToMaster(QStringList SelectedFileName);

    /****************************************************************************/
    /**
     * \brief Signal is emitted to abort test
     */
    /****************************************************************************/
    void AbortTest();


    /****************************************************************************/
    /**
     * \brief Signal emitted for PressureSensor calibration
     */
    /****************************************************************************/
    void PressureSensorCalibrationRequest();

    /****************************************************************************/
    /**
     * \brief Signal emitted for setting button status
     */
    /****************************************************************************/
    void SetSettingsButtonStatus();
    /*******************************************************************************/
    /*!
     *  \brief Signal is emitted to update Service GUI Connector object
     *  \iparam DataConnector = Data connector object
     *  \iparam MainWindow = Main window object
     */
    /*******************************************************************************/
    void UpdateGUIConnector(Core::CServiceGUIConnector *DataConnector, MainMenu::CMainWindow *MainWindow);
  /*******************************************************************************/
    /*!
     *  \brief Signal emitted for logging off the system
     */
    /*******************************************************************************/
    void LogOffSystem();

    /*******************************************************************************/
    /*!
     *  \brief Signal is emitted for log on to the system
     */
    /*******************************************************************************/
    void LogOnSystem();
    /*******************************************************************************/
    /*!
     *  \brief Signal emitted to set the deviceName
     */
    /*******************************************************************************/
    void SetDeviceName(QString);
	
    /****************************************************************************/
    /**
       * \brief Signal emitted to perform manufacturing tests
       * \iparam Test = Test ID
       * \iparam AbortTestID = Test abort ID
       */
    /****************************************************************************/
    void PerformManufacturingTest(Service::ModuleTestCaseID Test, Service::ModuleTestCaseID AbortTestID=Service::TEST_CASE_ID_UNUSED);

    /*******************************************************************************/
    /*!
     *  \brief Signal emitted to shut down system
     */
    /*******************************************************************************/
    void ShutdownSystem(bool NeedUpdate=true);

    /****************************************************************************/
    /**
     * \brief Signal is emitted to set network settings result
     * \iparam Param = PlatformService::NetworkSettings_t
     * \iparam Result  = bool
     */
    /****************************************************************************/
    void SetNetworkSettingsResult(PlatformService::NetworkSettings_t Param, bool Result);

    /****************************************************************************/
    /**
     * \brief Signal is emitted to set network settings infomation
     * \iparam Text = Text of information
     * \iparam Color = The text color
     */
    /****************************************************************************/
    void SetInformationToNetworkSettings(QString Text, QString Color);

    /*******************************************************************************/
    /*!
     *  \brief Signal is emitted to perform network tests.
     */
    /*******************************************************************************/
    void PerformNetworkTests();

    /*******************************************************************************/
    /*!
     *  \brief Signal is emitted to download firmware.
     */
    /*******************************************************************************/
    void DownloadFirmware();

    void SendServRequest(QString ReqName, QStringList Params);

    void ReturnServiceRequestResult(QString ReqName, int ErrorCode, QStringList Result);
};

} // end namespace Core

#endif // CORE_STARTUP_H
