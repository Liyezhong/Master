/****************************************************************************/
/*! \file Startup.cpp
 *
 *  \brief Startup implementation.
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

#include "Core/Include/Startup.h"
#include <Global/Include/GlobalDefines.h>
#include <QDebug>
#include <QMessageBox>
#include <Core/Include/ServiceDefines.h>

namespace Core {

/****************************************************************************/
/*!
 *  \brief Constructor
 */
/****************************************************************************/
CStartup::CStartup() : QObject()
{
    // GUI components
    mp_Clock = new QTimer();    
    mp_MainWindow = new MainMenu::CMainWindow();
    mp_MessageBox = new MainMenu::CMessageDlg(mp_MainWindow);

    // ServiceConnector
    mp_ServiceConnector = new Core::CServiceGUIConnector(mp_MainWindow);

    // System tracking
    mp_SystemTrackingGroup = new MainMenu::CHiMenuGroup;
    mp_CurrentConfiguration =
            new SystemTracking::CCurrentConfiguration(mp_ServiceConnector);
    mp_AddModifyConfigGroup = new MainMenu::CHiMenuGroup;
    mp_AddModifyConfig = new SystemTracking::CAddModifyConfig;
    mp_ViewHistory = new SystemTracking::CViewHistory;
    mp_MainControlConfig =
            new SystemTracking::CMainControl(*mp_ServiceConnector);
    mp_RetortConfig = new SystemTracking::CRetort(*mp_ServiceConnector);
    mp_OvenConfig = new SystemTracking::COven(*mp_ServiceConnector);
    mp_RotaryValveConfig =
            new SystemTracking::CRotaryValve(*mp_ServiceConnector);
    mp_LaSystemConfig = new SystemTracking::CLaSystem(*mp_ServiceConnector);

    (void)connect(mp_ServiceConnector,
                  SIGNAL(ModuleListChanged()),
                  mp_CurrentConfiguration,
                  SLOT(UpdateGUI()));

    (void)connect(mp_MainControlConfig,
                  SIGNAL(ModuleListChanged()),
                  mp_ViewHistory,
                  SLOT(UpdateGUI()));

    (void)connect(mp_RetortConfig,
                  SIGNAL(ModuleListChanged()),
                  mp_ViewHistory,
                  SLOT(UpdateGUI()));

    (void)connect(mp_OvenConfig,
                  SIGNAL(ModuleListChanged()),
                  mp_ViewHistory,
                  SLOT(UpdateGUI()));

    (void)connect(mp_RotaryValveConfig,
                  SIGNAL(ModuleListChanged()),
                  mp_ViewHistory,
                  SLOT(UpdateGUI()));

    (void)connect(mp_LaSystemConfig,
                  SIGNAL(ModuleListChanged()),
                  mp_ViewHistory,
                  SLOT(UpdateGUI()));

    (void)connect(mp_ServiceConnector,
                  SIGNAL(ModuleListChanged()),
                  mp_ViewHistory,
                  SLOT(UpdateGUI()));
    // Log Viewer
    mp_LogViewerGroup = new MainMenu::CMenuGroup;
    mp_SystemLogViewer = new LogViewer::CSystemLogViewer;
    mp_RecoveryAction = new LogViewer::CRecoveryAction;
    mp_ServiceLogViewer = new LogViewer::CServiceLogViewer;
    mp_SoftwareUpdateLogViewer = new LogViewer::CSoftwareUpdateLog;

    //Diagnostics
    mp_DiagnosticsGroup = new MainMenu::CMenuGroup;
    mp_Retort           = new Diagnostics::CRetort;
    mp_Oven             = new Diagnostics::COven;
    mp_RotaryValve      = new Diagnostics::CRotaryValve;
    mp_LaSystem         = new Diagnostics::CLaSystem;
    mp_System           = new Diagnostics::CSystem;

    //Diagnostics1 Manufacturing
    mp_DiagnosticsManufGroup = new MainMenu::CMenuGroup;

    // Calibration
    mp_CalibrationGroup = new MainMenu::CMenuGroup;
    mp_PressureSensor = new Calibration::CPressureSensor;
    mp_Touchscreen = new Calibration::CTouchscreen;

    // Software upate
    mp_ServiceUpdateGroup = new MainMenu::CHiMenuGroup;
    mp_FirmwareUpdate = new ServiceUpdates::CFirmwareUpdate;
    mp_DataManagement = new ServiceUpdates::CDataManagement;
    mp_Settings = new ServiceUpdates::CSettingsWidget(*mp_ServiceConnector);

    (void)connect(mp_ServiceConnector,
                  SIGNAL(UserSettingInterfaceChanged()),
                  mp_Settings,
                  SLOT(UpdateGUI()));

    (void)connect(mp_DataManagement,
                  SIGNAL(ServiceImportExportRequested(bool)),
                  this,
                  SLOT(OnServiceImportExportRequest(bool)));

    (void)connect(this,
                  SIGNAL(ImportFinish(bool)),
                  mp_DataManagement,
                  SLOT(ImportFinished(bool)));

    (void)connect(this,
                  SIGNAL(ExportFinish(bool)),
                  mp_DataManagement,
                  SLOT(ExportFinished(bool)));


    if (!connect(mp_Clock, SIGNAL(timeout()), this, SLOT(UpdateDateTime())))
    {
        qDebug() << "CStartup: cannot connect 'timeout' signal";
    }

    if (!connect(mp_RotaryValve, SIGNAL(GuiRVHeatingTest()),
                 this, SLOT(OnGuiRVHeatingTest()))) {
        qDebug() << "CStartup: cannot connect 'GuiRVHeatingTest' signal";
    }

    if (!connect(mp_RotaryValve, SIGNAL(GuiRVInitTest()),
                 this, SLOT(OnGuiRVInitTest()))) {
        qDebug() << "CStartup: cannot connect 'GuiRVInitTest' signal";
    }

    if (!connect(mp_RotaryValve, SIGNAL(GuiRVSelectTest(qint32)),
                 this, SLOT(OnGuiRVSelectTest(qint32)))) {
        qDebug() << "CStartup: cannot connect 'GuiRVSelectTest' signal";
    }

    if (!connect(mp_RotaryValve, SIGNAL(GuiRVSealTest(qint32)),
                 this, SLOT(OnGuiRVSealTest(qint32)))) {
        qDebug() << "CStartup: cannot connect 'GuiRVSealTest' signal";
    }

    if (!connect(mp_Retort, SIGNAL(GuiLevelSensorDetectingTest(qint32)),
                 this, SLOT(OnGuiLevelSensorDetectingTest(qint32)))) {
        qDebug() << "CStartup: cannot connect 'GuiLevelSensorDetectingTest' signal";
    }

    if (!connect(mp_Retort, SIGNAL(GuiLevelSensorHeatingTest()),
                 this, SLOT(OnGuiLevelSensorHeatingTest()))) {
        qDebug() << "CStartup: cannot connect 'GuiLevelSensorHeatingTest' signal";
    }

    if (!connect(mp_Retort, SIGNAL(GuiRetortEmptyHeatingTest()),
                 this, SLOT(OnGuiRetortEmptyHeatingTest()))) {
        qDebug() << "CStartup: cannot connect 'GuiRetortEmptyHeatingTest' signal";
    }

    if (!connect(mp_Retort, SIGNAL(GuiRetortLiquidHeatingTest()),
                 this, SLOT(OnGuiRetortLiquidHeatingTest()))) {
        qDebug() << "CStartup: cannot connect 'GuiRetortLiquidHeatingTest' signal";
    }

    if (!connect(mp_LaSystem, SIGNAL(GuiTube1HeatingTest()),
                 this, SLOT(OnGuiTube1HeatingTest()))) {
        qDebug() << "CStartup: cannot connect 'GuiTube1HeatingTest' signal";
    }

    if (!connect(mp_LaSystem, SIGNAL(GuiTube2HeatingTest()),
                 this, SLOT(OnGuiTube2HeatingTest()))) {
        qDebug() << "CStartup: cannot connect 'GuiTube2HeatingTest' signal";
    }

    if (!connect(mp_LaSystem, SIGNAL(GuiOvenEmptyHeatingTest()),
                 this, SLOT(OnGuiOvenEmptyHeatingTest()))) {
        qDebug() << "CStartup: cannot connect 'GuiOvenEmptyHeatingTest' signal";
    }

    if (!connect(mp_ServiceConnector->GetWaitDlgPtr(), SIGNAL(rejected()),
                 this, SLOT(OnGuiAbortTest()))) {
        qDebug() << "CStartup: cannot connect 'GuiAbortTest' signal";
    }

    m_DateTime.setTime_t(0);
    mp_Clock->start(60000);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CStartup::~CStartup()
{
    try {
        // Calibration
        delete mp_Touchscreen;
        delete mp_PressureSensor;
        delete mp_CalibrationGroup;

        // Service Update
        delete mp_Settings;
        delete mp_DataManagement;
        delete mp_FirmwareUpdate;
        delete mp_ServiceUpdateGroup;

        // diagnostics group
        delete mp_System;
        delete mp_LaSystem;
        delete mp_RotaryValve;
        delete mp_Oven;
        delete mp_Retort;
        delete mp_DiagnosticsGroup;

        //Diagnostics1 Manufacturing
        delete mp_DiagnosticsManufGroup;

        // Log viewer
        delete mp_SoftwareUpdateLogViewer;
        delete mp_ServiceLogViewer;
        delete mp_RecoveryAction;
        delete mp_SystemLogViewer;
        delete mp_LogViewerGroup;

        // System Tracking
        delete mp_LaSystemConfig;
        delete mp_RotaryValveConfig;
        delete mp_OvenConfig;
        delete mp_RetortConfig;
        delete mp_MainControlConfig;
        delete mp_ViewHistory;
        delete mp_AddModifyConfig;
        delete mp_AddModifyConfigGroup;
        delete mp_CurrentConfiguration;
        delete mp_SystemTrackingGroup;

        // GUI Components
        delete mp_MainWindow;
        delete mp_Clock;
    }
    catch (...) {}
}

/****************************************************************************/
/*!
 *  \brief Initializes the network layers
 */
/****************************************************************************/
qint32 CStartup::NetworkInit()
{
    return 0;
}

/****************************************************************************/
/*!
 *  \brief System Tracking and LogViewer Tab
 */
/****************************************************************************/
void CStartup::LoadCommonComponenetsOne()
{
    mp_MainWindow->setWindowTitle("ST8200 Service Software");
    mp_MainWindow->setFixedSize(800, 600);

    // System Tracking
    mp_SystemTrackingGroup->AddPanel("Current Config", mp_CurrentConfiguration);

    mp_AddModifyConfigGroup->SetTitle("Add/Modify Config");
    mp_AddModifyConfigGroup->AddPanel("Main Control", mp_MainControlConfig);
    mp_AddModifyConfigGroup->AddPanel("Retort", mp_RetortConfig);
    mp_AddModifyConfigGroup->AddPanel("Oven", mp_OvenConfig);
    mp_AddModifyConfigGroup->AddPanel("Rotary Valve", mp_RotaryValveConfig);
    mp_AddModifyConfigGroup->AddPanel("L&&A System", mp_LaSystemConfig);

    mp_SystemTrackingGroup->AddPanel("Add/Modify Config",
                                     mp_AddModifyConfigGroup,
                                     false);

    mp_SystemTrackingGroup->AddPanel("View History", mp_ViewHistory);

    mp_MainWindow->AddMenuGroup(mp_SystemTrackingGroup, "System Tracking");

    // Log Viewer
    mp_LogViewerGroup->AddPanel("System Log Viewer", mp_SystemLogViewer);
    mp_LogViewerGroup->AddPanel("Recovery Action", mp_RecoveryAction);
    mp_LogViewerGroup->AddPanel("Service Log Viewer", mp_ServiceLogViewer);
    mp_LogViewerGroup->AddPanel("Software Update Log", mp_SoftwareUpdateLogViewer);
    mp_MainWindow->AddMenuGroup(mp_LogViewerGroup, "Log Viewer");

}

/****************************************************************************/
/*!
 *  \brief Calibration and ServiceUpate Tab
 */
/****************************************************************************/
void CStartup::LoadCommonComponenetsTwo()
{
    // Calibration
    mp_CalibrationGroup->AddPanel("Pressure Sensor", mp_PressureSensor);
    mp_CalibrationGroup->AddPanel("Touchscreen", mp_Touchscreen);
    mp_MainWindow->AddMenuGroup(mp_CalibrationGroup, "Calibration");

    // Service update
    mp_ServiceUpdateGroup->AddPanel("Firmware Update", mp_FirmwareUpdate);
    mp_ServiceUpdateGroup->AddPanel("Data Management", mp_DataManagement);
    mp_ServiceUpdateGroup->AddPanel("Settings", mp_Settings, false);
    mp_MainWindow->AddMenuGroup(mp_ServiceUpdateGroup, "Service Updates");


#ifdef Q_WS_QWS
    mp_MainWindow->showFullScreen();
#else
    mp_MainWindow->show();
#endif

}

/****************************************************************************/
/*!
 *  \brief Initializes the KeyBoard
 */
/****************************************************************************/
void CStartup::GuiInit()
{
    mp_USBKeyValidator = new Core::CUSBKeyValidator(this);
}

/****************************************************************************/
/*!
 *  \brief Initializes the Service user interface
 */
/****************************************************************************/
void CStartup::ServiceGuiInit()
{
    LoadCommonComponenetsOne();

    mp_MainWindow->SetUserIcon(MainMenu::CMainWindow::Service);

    //Diagnostics
    mp_MainWindow->AddMenuGroup(mp_DiagnosticsGroup, "Diagnostics");
    mp_DiagnosticsGroup->AddPanel("Retort",       mp_Retort);
    mp_DiagnosticsGroup->AddPanel("Oven",         mp_Oven);
    mp_DiagnosticsGroup->AddPanel("Rotary Valve", mp_RotaryValve);
    mp_DiagnosticsGroup->AddPanel("L&&A System",   mp_LaSystem);
    mp_DiagnosticsGroup->AddPanel("System",       mp_System);

    LoadCommonComponenetsTwo();
}

/****************************************************************************/
/*!
 *  \brief Initializes the Manufacturing interface
 */
/****************************************************************************/
void CStartup::ManufacturingGuiInit()
{
    LoadCommonComponenetsOne();

    mp_MainWindow->AddMenuGroup(mp_DiagnosticsManufGroup, "Diagnostics");

    LoadCommonComponenetsTwo();
}

/****************************************************************************/
/*!
 *  \brief Sets time and and date of the program
 *
 *  \iparam DateTime = New date and time
 */
/****************************************************************************/
void CStartup::SetDateTime(QDateTime DateTime)
{
    mp_Clock->stop();
    qDebug("CStartup: SetDateTime called");
    m_DateTime = DateTime;

    mp_Clock->start(60000 - 1000 * m_DateTime.time().second());
}

/****************************************************************************/
/*!
 *  \brief Updates time and and date of the program
 */
/****************************************************************************/
void CStartup::UpdateDateTime()
{
    mp_Clock->start(60000);
    m_DateTime = m_DateTime.addSecs(60);

}

/****************************************************************************/
/*!
 *  \brief Checks if a given panel is active on the screen
 *
 *  \iparam p_Group = The menu group
 *  \iparam p_Panel = The panel inside the menu group
 *
 *  \return Is the panel active (true) or not (false)
 */
/****************************************************************************/
bool CStartup::CurrentlyActive(MainMenu::CMenuGroup *p_Group, QWidget *p_Panel)
{
    //lint -esym(593, p_Group)
    //lint -esym(593, p_Panel)
    if (mp_MainWindow->GetCurrentGroup() != p_Group) {
        return false;
    }
    if (p_Group->GetCurrentPanel() != p_Panel) {
        return false;
    }
    return true;
}

/****************************************************************************/
/*!
 *  \brief Requtests DCP for Device Init
 *
 */
/****************************************************************************/
void CStartup::OnDeviceInitRequest()
{
    qDebug()<<"CStartup::OnDeviceInitRequest() called!!!";
    emit DeviceInitRequest();
}

/****************************************************************************/
/*!
 *  \brief Throws open a Pop Up Message with Custom Messages
 *
 */
/****************************************************************************/
void CStartup::ShowMessage(const QString &Message)
{
    mp_ServiceConnector->HideBusyDialog();
    mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_INFO, Message);
}

/****************************************************************************/
/*!
 *  \brief Throws open a Pop Up Message with Custom Messages
 *
 */
/****************************************************************************/
void CStartup::ShowErrorMessage(const QString &Message)
{
    mp_ServiceConnector->HideBusyDialog();
    mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, Message);
}

/****************************************************************************/
/*!
 *  \brief Throws open a Pop Up Message with Custom Messages
 *
 */
/****************************************************************************/
void CStartup::ShowCalibrationInitMessagetoMain(const QString &Message, bool OkStatus)
{
    qint32 ret = -1;
    if(OkStatus)
    {
        ret = 1;
    }
    if (m_LoopCalibrationStart.isRunning()) {
        m_LoopCalibrationStart.exit(ret);
    } else {
        qDebug()<<"NOTICE: Unexpected action acknowledgement for Calibration";
    }
}

void CStartup::OnServiceImportExportRequest(bool IsImport)
{
    if (IsImport)
    {
        emit ImportExportDataFileRequest("Import", "Service");
    }
    else
    {
        emit ImportExportDataFileRequest("Export", "Service");
    }
}

void CStartup::ImportFinished(bool Failed)
{
    qDebug() << "CStartup::ImportFinished";

    emit ImportFinish(Failed);
}

void CStartup::ExportFinished(bool Failed)
{
    qDebug() << "CStartup::ExportFinished";

    emit ExportFinish(Failed);
}



/****************************************************************************/
/*!
 *  \brief Test abort
 *
 */
/****************************************************************************/
void CStartup::OnGuiAbortTest()
{
    //ShowErrorMessage("Test failed");
    emit AbortTest();
}


/****************************************************************************/
/*!
 *  \brief Rotary valve heating test
 *
 */
/****************************************************************************/
void CStartup::OnGuiRVHeatingTest()
{

    mp_ServiceConnector->ShowBusyDialog("Heating rotary valve in progress...", false);
    emit RVHeatingTest(Service::HEATER_ROTARY_VALVE, Service::HEATING_ROTARY_VALVE);
}


/****************************************************************************/
/*!
 *  \brief Rotary valve initialization test
 *
 */
/****************************************************************************/
void CStartup::OnGuiRVInitTest()
{

    mp_ServiceConnector->ShowBusyDialog("Initializing rotary valve in progress...", true);
    emit RotaryValveTest(0, Service::RV_MOVE_TO_INIT_POS);
}


/****************************************************************************/
/*!
 *  \brief Rotary valve select test
 *
 */
/****************************************************************************/
void CStartup::OnGuiRVSelectTest(qint32 Position)
{

    mp_ServiceConnector->ShowBusyDialog("Rotary valve selecting test in progress...", true);
    emit RotaryValveTest(Position, Service::RV_MOVE_TO_TUBE_POS);
}


/****************************************************************************/
/*!
 *  \brief Rotary valve seal test
 *
 */
/****************************************************************************/
void CStartup::OnGuiRVSealTest(qint32 Position)
{

    mp_ServiceConnector->ShowBusyDialog("Rotary valve sealing test in progress...", true);
    emit RotaryValveTest(Position, Service::RV_MOVE_TO_SEAL_POS);
}


/****************************************************************************/
/*!
 *  \brief Level senosor detecting test
 *
 */
/****************************************************************************/
void CStartup::OnGuiLevelSensorDetectingTest(qint32 Position)
{

    mp_ServiceConnector->ShowBusyDialog("Detecting level sensor in progress...", false);
    emit LevelSensorDetectingTest(Position);
}


/****************************************************************************/
/*!
 *  \brief Level senosor heating test
 *
 */
/****************************************************************************/
void CStartup::OnGuiLevelSensorHeatingTest()
{

    mp_ServiceConnector->ShowBusyDialog("Heating level sensor in progress...", false);
    emit LevelSensorHeatingTest(Service::HEATER_LEVEL_SENSOR, Service::HEATING_LEVEL_SENSOR);
}


/****************************************************************************/
/*!
 *  \brief Retort(empty) heating test
 *
 */
/****************************************************************************/
void CStartup::OnGuiRetortEmptyHeatingTest()
{
    mp_ServiceConnector->ShowBusyDialog("Heating retort(empty) in progress...", false);
    emit RetortHeatingTest(Service::HEATER_RETORT, Service::HEATING_RETORT_EMMPTY);
}

/****************************************************************************/
/*!
 *  \brief Retort(liquid) heating test
 *
 */
/****************************************************************************/
void CStartup::OnGuiRetortLiquidHeatingTest()
{
    mp_ServiceConnector->ShowBusyDialog("Heating retort(liquid) in progress...", false);
    emit RetortHeatingTest(Service::HEATER_RETORT, Service::HEATING_RETORT_LIQUID);
}

/****************************************************************************/
/*!
 *  \brief Tube1 heating test
 *
 */
/****************************************************************************/
void CStartup::OnGuiTube1HeatingTest()
{
    mp_ServiceConnector->ShowBusyDialog("Heating Belt1 in progress...", false);
    emit TubeHeatingTest(Service::HEATER_TUBE1, Service::HEATING_TUBE1);
}

/****************************************************************************/
/*!
 *  \brief Tube2 heating test
 *
 */
/****************************************************************************/
void CStartup::OnGuiTube2HeatingTest()
{
    mp_ServiceConnector->ShowBusyDialog("Heating Belt2 in progress...", false);
    emit TubeHeatingTest(Service::HEATER_TUBE2, Service::HEATING_TUBE2);
}

/****************************************************************************/
/*!
 *  \brief Oven(empty) heating test
 *
 */
/****************************************************************************/
void CStartup::OnGuiOvenEmptyHeatingTest()
{
    mp_ServiceConnector->ShowBusyDialog("Heating Oven(empty) in progress...", false);
    emit OvenHeatingTest(Service::HEATER_OVEN, Service::HEATING_OVEN_EMPTY);
}
} // end namespace Core
