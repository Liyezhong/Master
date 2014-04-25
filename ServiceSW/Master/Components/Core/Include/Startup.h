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
#include "Core/Include/USBKeyValidator.h"

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

#include "Diagnostics/Include/Retort.h"
#include "Diagnostics/Include/Oven.h"
#include "Diagnostics/Include/RotaryValve.h"
#include "Diagnostics/Include/LaSystem.h"
#include "Diagnostics/Include/System.h"
#include "Core/Include/CalibrationHandler.h"
#include <QTimer>

namespace Core {
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

    int Mode;

    Core::CServiceGUIConnector *mp_ServiceConnector;

    void LoadCommonComponenetsOne();    // System Tracking and Log Viewer Tab
    void LoadCommonComponenetsTwo();    // Calibration and Service Updates Tab
    void ServiceGuiInit();
    void ManufacturingGuiInit();

private:

    QTimer      *mp_Clock;                                  //!< The main clock
    QDateTime   m_DateTime;                                 //!< The current date and time

    // GUI components   
    MainMenu::CMainWindow                   *mp_MainWindow;                 //!< The main window of the application
    Core::CUSBKeyValidator                  *mp_USBKeyValidator;

    // System Tracking
    MainMenu::CHiMenuGroup                  *mp_SystemTrackingGroup;        //!< Menu group containing System Tracking data
    SystemTracking::CCurrentConfiguration   *mp_CurrentConfiguration;       //!< Current Configuration information screen
    SystemTracking::CAddModifyConfig        *mp_AddModifyConfig;            //!< Add or Modify configuration screen
    SystemTracking::CViewHistory            *mp_ViewHistory;                //!< Module History
    MainMenu::CHiMenuGroup                  *mp_AddModifyConfigGroup;       //!< Add or Modify configuration screen group
    SystemTracking::CMainControl            *mp_MainControlConfig;          //!< Configuration screen for Main control
    SystemTracking::CRetort                 *mp_RetortConfig;               //!< Configuration screen for Retort
    SystemTracking::COven                   *mp_OvenConfig;                 //!< Configuration screen for Oven
    SystemTracking::CRotaryValve            *mp_RotaryValveConfig;          //!< Configuration screen for Rotary Valve
    SystemTracking::CLaSystem               *mp_LaSystemConfig;             //!< Configuration screen for L&A System

    // Log Viewer
    MainMenu::CMenuGroup                    *mp_LogViewerGroup;             //!< Log information of module data

    LogViewer::CLogViewer             *mp_SystemLogViewer;            //!< System Log information
    LogViewer::CLogViewer              *mp_RecoveryAction;             //!< Provides Recovery action Help pages.
    LogViewer::CLogViewer            *mp_ServiceLogViewer;           //!< Service Log Viewer
    LogViewer::CLogViewer           *mp_SoftwareUpdateLogViewer;    //!< Software Update Log Viewer
    LogViewer::CLogContentDlg       *mp_LogContentDlg;              //!< Log Content Dialog
    LogViewer::CSystemLogViewerDlg  *mp_SystemLogContentDlg;        //!< System Log Content Dialog



    //Diagnostics
    MainMenu::CMenuGroup                    *mp_DiagnosticsGroup;
    Diagnostics::CRetort                    *mp_Retort;                     //!< Retort tests
    Diagnostics::COven                      *mp_Oven;                       //!< Oven tests
    Diagnostics::CRotaryValve               *mp_RotaryValve;                //!< Rotary Valve tests
    Diagnostics::CLaSystem                  *mp_LaSystem;                   //!< L&A System tests
    Diagnostics::CSystem                    *mp_System;                     //!< System tests

    // Diagnostics1 Manufacturing
    MainMenu::CMenuGroup                    *mp_DiagnosticsManufGroup;

    //Service Updates
    MainMenu::CMenuGroup                    *mp_ServiceUpdateGroup;         //!< Menu group containing Service Update dialogs
    ServiceUpdates::CFirmwareUpdate         *mp_FirmwareUpdate;             //!< Firmware update page
    ServiceUpdates::CDataManagement         *mp_DataManagement;             //!< Data management page
    ServiceUpdates::CSettings               *mp_Setting;                    //!< User settings sub menu


    //Message Box, Busy Box, Generic PoP Up.
    MainMenu::CMessageDlg *mp_MessageBox;
    Core::CCalibrationHanlder *mp_CalibrationHandler;

private slots:
    void SetDateTime(QDateTime DateTime);
    void UpdateDateTime();

private:
    bool CurrentlyActive(MainMenu::CMenuGroup *p_Group, QWidget *p_Panel);

public slots:
    void DisplayLogInformation(QString FileName, QString FilePath);

    void OnDeviceInitRequest();

    void OnServiceImportExportRequest(bool IsImport);

    /* Return Message Slots */
    void ShowMessage(const QString &Message);
    void ShowErrorMessage(const QString &Message);
    void ShowCalibrationInitMessagetoMain(const QString &Message, bool OkStatus);

    void ImportFinished(bool Failed);

    void ExportFinished(bool Failed);

    /* General slots */
    void OnGuiAbortTest();

    /* Rotary Valve slots */
    void OnGuiRVHeatingTest();
    void OnGuiRVInitTest();
    void OnGuiRVSelectTest(qint32);
    void OnGuiRVSealTest(qint32);

    /* Retort slots */
    void OnGuiLevelSensorDetectingTest(qint32);
    void OnGuiLevelSensorHeatingTest();
    void OnGuiRetortEmptyHeatingTest();
    void OnGuiRetortLiquidHeatingTest();

    /* Air and Liquid slots */
    void OnGuiTube1HeatingTest();
    void OnGuiTube2HeatingTest();

    /* Oven slots */
    void OnGuiOvenEmptyHeatingTest();
    void RetranslateUI();
signals:
    void DeviceInitRequest(void);

    void ImportExportDataFileRequest(const QString &CommandName,
                                     const QByteArray &CommandData);

    void ImportFinish(bool Failed);

    void ExportFinish(bool Failed);

    /* General signals */
    void AbortTest();

    /* Rotary Valve signals */
    void RVHeatingTest(quint8 HeaterType, quint8 HeatingTestType);
    void RotaryValveTest(qint32 Postion, quint8 RVTestType);

    /* Retort signals */
    void LevelSensorDetectingTest(qint32 Position);
    void LevelSensorHeatingTest(quint8, quint8);
    void RetortHeatingTest(quint8, quint8);

    /* Air and Liquid signals */
    void TubeHeatingTest(quint8, quint8);

    /* Oven signals */
    void OvenHeatingTest(quint8, quint8);

    /****************************************************************************/
    /**
     * \brief Signal emitted for ovenlid init calibration
     */
    /****************************************************************************/
    void OvenLidInitCalibrationRequest();

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
};

} // end namespace Core

#endif // CORE_STARTUP_H
