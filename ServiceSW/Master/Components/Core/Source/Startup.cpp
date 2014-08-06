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

#include "Main/Include/HimalayaServiceEventCodes.h"
#include "Core/Include/Startup.h"
#include "LogViewerDialog/Include/LogContentDlg.h"
#include "LogViewerDialog/Include/SystemLogViewerDlg.h"
#include "Core/Include/CMessageString.h"
#include <QTextStream>
#include <QApplication>
#include <ServiceWidget/Include/DlgWizardSelectTestOptions.h>
#include <Core/Include/SelectTestOptions.h>
#include "DiagnosticsManufacturing/Include/StatusConfirmDialog.h"
#include "ServiceDataManager/Include/TestCaseGuide.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"
#include "ServiceDataManager/Include/TestCase.h"
#include "DiagnosticsManufacturing/Include/UserInputDialog.h"
#include <QDesktopWidget>

namespace Core {

/****************************************************************************/
/*!
 *  \brief Constructor
 */
/****************************************************************************/
CStartup::CStartup() : QObject(),
    mp_LogContentDlg(NULL),
    mp_SystemLogContentDlg(NULL),
    m_DeviceName(""),
    m_WindowStatusResetTimer(this),
    m_CurrentUserMode(""),
    mp_ManaufacturingDiagnosticsHandler(NULL),
    mp_HeatingStatusDlg(NULL),
    mp_SealingStatusDlg(NULL),
    m_SelfTestFinished(false)
{
    qRegisterMetaType<Service::ModuleNames>("Service::ModuleNames");
    qRegisterMetaType<Service::ModuleTestCaseID>("Service::ModuleTestCaseID");
    qRegisterMetaType<Service::ModuleTestStatus>("Service::ModuleTestStatus");

    // GUI components
    mp_Clock = new QTimer();    
    mp_MainWindow = new MainMenu::CMainWindow();
    mp_MessageBox = new MainMenu::CMessageDlg(mp_MainWindow);

    CONNECTSIGNALSLOT(this, LogOffSystem(), mp_MainWindow, hide());
    CONNECTSIGNALSLOT(this, LogOnSystem(), mp_MainWindow, show());

    // Initialize Strings
    RetranslateUI();

    m_CurrentUserMode = QString("");

    // ServiceConnector
    mp_ServiceConnector = new Core::CServiceGUIConnector(mp_MainWindow);
    mp_CalibrationHandler = new Core::CCalibrationHanlder(mp_ServiceConnector, mp_MainWindow);

    // System tracking
    //mp_SystemTrackingGroup = new MainMenu::CHiMenuGroup;
    mp_SystemTrackingGroup = new MainMenu::CMenuGroup;
    mp_CurrentConfiguration =
            new SystemTracking::CCurrentConfiguration(mp_ServiceConnector, mp_MainWindow);
    mp_AddModifyConfigGroup = new MainMenu::CMenuGroup;
    //mp_AddModifyConfig = new SystemTracking::CAddModifyConfig;
    mp_ViewHistory = new SystemTracking::CViewHistory(mp_ServiceConnector, mp_MainWindow);
    mp_MainControlConfig =
            new SystemTracking::CMainControl(*mp_ServiceConnector, mp_MainWindow);
    mp_RetortConfig = new SystemTracking::CRetort(*mp_ServiceConnector, mp_MainWindow);
    mp_OvenConfig = new SystemTracking::COven(*mp_ServiceConnector, mp_MainWindow);
    mp_RotaryValveConfig =
            new SystemTracking::CRotaryValve(*mp_ServiceConnector, mp_MainWindow);
    mp_LaSystemConfig = new SystemTracking::CLaSystem(*mp_ServiceConnector, mp_MainWindow);

    CONNECTSIGNALSIGNAL(mp_MainControlConfig, ModuleListChanged(), mp_ServiceConnector, ModuleListChanged());
    CONNECTSIGNALSIGNAL(mp_RetortConfig, ModuleListChanged(), mp_ServiceConnector, ModuleListChanged());
    CONNECTSIGNALSIGNAL(mp_OvenConfig, ModuleListChanged(), mp_ServiceConnector, ModuleListChanged());
    CONNECTSIGNALSIGNAL(mp_RotaryValveConfig, ModuleListChanged(), mp_ServiceConnector, ModuleListChanged());
    CONNECTSIGNALSIGNAL(mp_LaSystemConfig, ModuleListChanged(), mp_ServiceConnector, ModuleListChanged());

    CONNECTSIGNALSIGNAL(mp_MainControlConfig, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID),
                        this, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID));
    CONNECTSIGNALSIGNAL(mp_RetortConfig, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID),
                        this, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID));
    CONNECTSIGNALSIGNAL(mp_OvenConfig, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID),
                        this, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID));
    CONNECTSIGNALSIGNAL(mp_RotaryValveConfig, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID),
                        this, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID));
    CONNECTSIGNALSIGNAL(mp_LaSystemConfig, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID),
                        this, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID));

    (void)connect(mp_ServiceConnector,
                  SIGNAL(ModuleListChanged()),
                  mp_CurrentConfiguration,
                  SLOT(UpdateGUI()));

    (void)connect(mp_ServiceConnector,
                  SIGNAL(ModuleListChanged()),
                  mp_ViewHistory,
                  SLOT(UpdateGUI()));

    CONNECTSIGNALSLOT(mp_MainWindow, CurrentTabChanged(int), mp_MainControlConfig, CurrentTabChanged(int));
    CONNECTSIGNALSLOT(mp_SystemTrackingGroup, PanelChanged(), mp_MainControlConfig, ConfirmModuleConfiguration());
    CONNECTSIGNALSLOT(mp_AddModifyConfigGroup, PanelChanged(), mp_MainControlConfig, ConfirmModuleConfiguration());

    CONNECTSIGNALSLOT(mp_MainWindow, CurrentTabChanged(int), mp_RetortConfig, CurrentTabChanged(int));
    CONNECTSIGNALSLOT(mp_SystemTrackingGroup, PanelChanged(), mp_RetortConfig, ConfirmModuleConfiguration());
    CONNECTSIGNALSLOT(mp_AddModifyConfigGroup, PanelChanged(), mp_RetortConfig, ConfirmModuleConfiguration());

    CONNECTSIGNALSLOT(mp_MainWindow, CurrentTabChanged(int), mp_OvenConfig, CurrentTabChanged(int));
    CONNECTSIGNALSLOT(mp_SystemTrackingGroup, PanelChanged(), mp_OvenConfig, ConfirmModuleConfiguration());
    CONNECTSIGNALSLOT(mp_AddModifyConfigGroup, PanelChanged(), mp_OvenConfig, ConfirmModuleConfiguration());

    CONNECTSIGNALSLOT(mp_MainWindow, CurrentTabChanged(int), mp_RotaryValveConfig, CurrentTabChanged(int));
    CONNECTSIGNALSLOT(mp_SystemTrackingGroup, PanelChanged(), mp_RotaryValveConfig, ConfirmModuleConfiguration());
    CONNECTSIGNALSLOT(mp_AddModifyConfigGroup, PanelChanged(), mp_RotaryValveConfig, ConfirmModuleConfiguration());

    CONNECTSIGNALSLOT(mp_MainWindow, CurrentTabChanged(int), mp_LaSystemConfig, CurrentTabChanged(int));
    CONNECTSIGNALSLOT(mp_SystemTrackingGroup, PanelChanged(), mp_LaSystemConfig, ConfirmModuleConfiguration());
    CONNECTSIGNALSLOT(mp_AddModifyConfigGroup, PanelChanged(), mp_LaSystemConfig, ConfirmModuleConfiguration());

    // Log Viewer
    mp_LogViewerGroup = new MainMenu::CMenuGroup;
//    mp_SystemLogViewer = new LogViewer::CSystemLogViewer;
//    mp_RecoveryAction = new LogViewer::CRecoveryAction;
//    mp_ServiceLogViewer = new LogViewer::CServiceLogViewer;
//    mp_SoftwareUpdateLogViewer = new LogViewer::CSoftwareUpdateLog;

    mp_SystemLogViewer = new LogViewer::CLogViewer("HimalayaEvents_12345678", Global::SystemPaths::Instance().GetLogfilesPath());
    mp_RecoveryAction = new LogViewer::CLogViewer("RecoveryActionText", Global::SystemPaths::Instance().GetSettingsPath());
    mp_ServiceLogViewer = new LogViewer::CLogViewer("Himalaya_Service", Global::SystemPaths::Instance().GetLogfilesPath());
    mp_SoftwareUpdateLogViewer = new LogViewer::CLogViewer("Himalaya_SW_Update_Events", Global::SystemPaths::Instance().GetLogfilesPath());

    CONNECTSIGNALSLOT(mp_SystemLogViewer, DisplayLogFileContents(QString, QString), this, DisplayLogInformation(QString, QString));
    CONNECTSIGNALSLOT(mp_RecoveryAction , DisplayLogFileContents(QString, QString), this, DisplayLogInformation(QString, QString));
    CONNECTSIGNALSLOT(mp_ServiceLogViewer, DisplayLogFileContents(QString, QString), this, DisplayLogInformation(QString, QString));
    CONNECTSIGNALSLOT(mp_SoftwareUpdateLogViewer, DisplayLogFileContents(QString, QString), this, DisplayLogInformation(QString, QString));


    //Diagnostics
    mp_DiagnosticsGroup = new MainMenu::CMenuGroup;
    mp_Display          = new Diagnostics::CDisplay;
    mp_Retort           = new Diagnostics::CRetort;
    mp_Oven             = new Diagnostics::COven;
    mp_RotaryValve      = new Diagnostics::CRotaryValve;
    mp_LaSystem         = new Diagnostics::CLaSystem;
    mp_System           = new Diagnostics::CSystem;

    //Diagnostics1 Manufacturing
    mp_DiagnosticsManufGroup = new MainMenu::CMenuGroup;

    // Software upate
    mp_ServiceUpdateGroup = new MainMenu::CMenuGroup;
    mp_FirmwareUpdate = new ServiceUpdates::CFirmwareUpdate(mp_ServiceConnector);
    mp_DataManagement = new ServiceUpdates::CDataManagement;
    mp_Setting = new ServiceUpdates::CSettings(mp_ServiceConnector, mp_MainWindow);
    mp_UpdateSystem = new ServiceUpdates::CSystem(mp_MainWindow);

    (void)connect(mp_ServiceConnector,
                  SIGNAL(ModuleListChanged()),
                  mp_FirmwareUpdate,
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

    if (!connect(mp_Display, SIGNAL(BasicColorTest()),
                 this, SLOT(OnBasicColorTest()))) {
        qDebug() << "CStartup: cannot connect 'BasicColorTest' signal";
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
    /* Calibration Signals */
    CONNECTSIGNALSIGNAL(mp_CalibrationHandler, OvenLidInitCalibrationRequest(), this, OvenLidInitCalibrationRequest());
    CONNECTSIGNALSIGNAL(mp_CalibrationHandler, PressureSensorCalibrationRequest(), this, PressureSensorCalibrationRequest());
    CONNECTSIGNALSIGNAL(mp_CalibrationHandler, ShutdownSystem(bool), this, ShutdownSystem(bool));

    CONNECTSIGNALSLOT(this, SetSettingsButtonStatus(), mp_Setting, ResetButtonStatus());
    CONNECTSIGNALSLOT(this, UpdateGUIConnector(Core::CServiceGUIConnector*, MainMenu::CMainWindow*), mp_Setting, UpdateGUIConnector(Core::CServiceGUIConnector*, MainMenu::CMainWindow*));

    CONNECTSIGNALSLOT(mp_MainWindow, onChangeEvent(), this, RetranslateUI());
    CONNECTSIGNALSLOT(mp_ServiceConnector, ServiceParametersChanged(), this, UpdateParameters());

//    CONNECTSIGNALSIGNAL(mp_FirmwareUpdate, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID), this, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID));

    CONNECTSIGNALSIGNAL(mp_CalibrationHandler, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID), this, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID));

    CONNECTSIGNALSIGNAL(mp_UpdateSystem, ShutdownSystem(bool), this, ShutdownSystem(bool));

    /* Network Settings */
    CONNECTSIGNALSLOT(this, SetNetworkSettingsResult(PlatformService::NetworkSettings_t, bool), mp_Setting , SetNetworkSettingsResult(PlatformService::NetworkSettings_t, bool));
    CONNECTSIGNALSLOT(this, SetInformationToNetworkSettings(QString, QString), mp_Setting, SetInformationText(QString, QString));
    CONNECTSIGNALSIGNAL(mp_Setting, PerformNetworkTests(), this, PerformNetworkTests());
    CONNECTSIGNALSIGNAL(mp_Setting, DownloadFirmware(), this, DownloadFirmware());


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
        delete mp_CalibrationHandler;

        // Service Update
        delete mp_Setting;
        delete mp_UpdateSystem;
        delete mp_DataManagement;
        delete mp_FirmwareUpdate;
        delete mp_ServiceUpdateGroup;

        // diagnostics group
        delete mp_System;
        delete mp_LaSystem;
        delete mp_RotaryValve;
        delete mp_Oven;
        delete mp_Display;
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
        if (mp_LogContentDlg) {
            delete mp_LogContentDlg;
        }
        if (mp_SystemLogContentDlg) {
            delete mp_SystemLogContentDlg;
        }

        // System Tracking
        delete mp_LaSystemConfig;
        delete mp_RotaryValveConfig;
        delete mp_OvenConfig;
        delete mp_RetortConfig;
        delete mp_MainControlConfig;
        delete mp_ViewHistory;
//        delete mp_AddModifyConfig;
        delete mp_AddModifyConfigGroup;
        delete mp_CurrentConfiguration;
        delete mp_SystemTrackingGroup;

        // GUI Components
        delete mp_MainWindow;
        delete mp_Clock;

        delete mp_USBKeyValidator;
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
    mp_MainWindow->setWindowTitle("Himalaya Service Software");
    mp_MainWindow->setFixedSize(800, 600);

    // System Tracking
    mp_SystemTrackingGroup->AddPanel("Current Config", mp_CurrentConfiguration);

    //mp_AddModifyConfigGroup->SetTitle("Add/Modify Config");
    mp_AddModifyConfigGroup->AddPanel("Main Control", mp_MainControlConfig);
    mp_AddModifyConfigGroup->AddPanel("Retort", mp_RetortConfig);
    mp_AddModifyConfigGroup->AddPanel("Oven", mp_OvenConfig);
    mp_AddModifyConfigGroup->AddPanel("Rotary Valve", mp_RotaryValveConfig);
    mp_AddModifyConfigGroup->AddPanel("L&&A System", mp_LaSystemConfig);

    mp_SystemTrackingGroup->AddPanel("Add/Modify Config", mp_AddModifyConfigGroup);

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
    mp_CalibrationHandler->LoadCalibrationGUIComponenets();

    // Service update
    mp_ServiceUpdateGroup->AddPanel("Firmware Update", mp_FirmwareUpdate);
    mp_ServiceUpdateGroup->AddPanel("Data Management", mp_DataManagement);
    mp_ServiceUpdateGroup->AddPanel("System", mp_UpdateSystem);
    mp_ServiceUpdateGroup->AddSettingsPanel("Settings", mp_Setting);
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
    mp_USBKeyValidator = new ServiceKeyValidator::CUSBKeyValidator("Primaris");

    CONNECTSIGNALSLOT(mp_USBKeyValidator, SetSoftwareMode(PlatformService::SoftwareModeType_t,QString),
                       this, InitializeGui(PlatformService::SoftwareModeType_t,QString));

    CONNECTSIGNALSLOT(this, SetDeviceName(QString), mp_USBKeyValidator, SetDeviceName(QString));
}

/****************************************************************************/
/*!
 *  \brief Initializes the GUI in debug mode
 *  \iparam debugMode = Software mode
 */
/****************************************************************************/
void CStartup::GuiInit(QString debugMode)
{
    if (debugMode.startsWith("Service"))
    {
        ServiceGuiInit();
        (void) FileExistanceCheck();
    }
    else if (debugMode.startsWith("ts_Service"))
    {
        ServiceGuiInit();
        (void) FileExistanceCheck();
    }
    else if (debugMode.startsWith("ts_Manufacturing"))
    {
        InitManufacturingDiagnostic();
        (void) FileExistanceCheck();
    }
    else
    {
        if (debugMode.startsWith("Manufacturing"))
        {
            InitManufacturingDiagnostic();
            (void) FileExistanceCheck();
        }
        else
        {
            if (mp_USBKeyValidator)
            {
                delete mp_USBKeyValidator;
            }
            mp_USBKeyValidator = new ServiceKeyValidator::CUSBKeyValidator(m_DeviceName);
            CONNECTSIGNALSLOT(mp_USBKeyValidator, SetSoftwareMode(PlatformService::SoftwareModeType_t,QString),
                               this, InitializeGui(PlatformService::SoftwareModeType_t,QString));
            CONNECTSIGNALSLOT(this, SetDeviceName(QString), mp_USBKeyValidator, SetDeviceName(QString));
        }
    }
}

/****************************************************************************/
/*!
 *  \brief Slot called aftr 20 minutes of idle time
 *
 */
/****************************************************************************/
void CStartup::IdleTimeout()
{
    //Global::EventObject::Instance().RaiseEvent(EVENT_SERVICE_TIMEOUT_LOGOFF_SYSTEM);
    m_WindowStatusResetTimer.stop();
    emit LogOffSystem();

    if(NULL != mp_USBKeyValidator)
    {
        delete mp_USBKeyValidator;
        mp_USBKeyValidator = NULL;
    }
    mp_USBKeyValidator = new ServiceKeyValidator::CUSBKeyValidator(m_DeviceName, true);
    CONNECTSIGNALSLOT(mp_USBKeyValidator, SetSoftwareMode(PlatformService::SoftwareModeType_t,QString),
                       this, InitializeGui(PlatformService::SoftwareModeType_t,QString));
    CONNECTSIGNALSLOT(this, SetDeviceName(QString), mp_USBKeyValidator, SetDeviceName(QString));
}

/****************************************************************************/
/*!
 *  \brief Slot called to start timer
 *
 */
/****************************************************************************/
void CStartup::StartTimer()
{
    //Global::EventObject::Instance().RaiseEvent(EVENT_SERVICE_SYSTEM_IDLE_TIMER_START);
    if (!connect(mp_MainWindow, SIGNAL(OnWindowActivated()), this, SLOT(ResetWindowStatusTimer()))) {
        qDebug() << "CStartup: cannot connect 'OnWindowActivated' signal";
    }

    if(!QObject::connect(&m_WindowStatusResetTimer, SIGNAL(timeout()), this, SLOT(IdleTimeout()))) {
        /// \todo error. throw exception?
        qDebug() << "Can not connect m_WindowStatusResetTimer::timeout() with this::RefreshDateTime()";
    }

    m_WindowStatusResetTimer.setInterval(1200000); /// for Test
    m_WindowStatusResetTimer.start();
}

void CStartup::OnSelectTestOptions(int index)
{
    CSelectTestOptions::SetCurTestMode(ManufacturalTestMode_t(index));
}

/****************************************************************************/
/*!
 *  \brief Initializes the GUI
 *  \iparam SoftwareMode = Software mode
 *  \iparam UserMode = Usser name
 */
/****************************************************************************/
void CStartup::InitializeGui(PlatformService::SoftwareModeType_t SoftwareMode, QString UserMode)
{
    qDebug()<<"CStartup::InitializeGui  "<<SoftwareMode;

    SetCurrentUserMode(UserMode);
    if (SoftwareMode == PlatformService::SERVICE_MODE)
    {
        if(!mp_USBKeyValidator->GetSystemLogOff())
        {
            ServiceGuiInit();
            QTimer::singleShot(50, this, SLOT(FileExistanceCheck()));
            StartTimer();
        }
        else
        {
            emit LogOnSystem();
        }
    }
    else if (SoftwareMode == PlatformService::MANUFACTURING_MODE)
    {
        mp_USBKeyValidator->HideKeyBoard();
        MainMenu::CDlgWizardSelectTestOptions* pDlgWizardSelectTestOptions = new MainMenu::CDlgWizardSelectTestOptions(NULL, QApplication::desktop()->screen());
        CONNECTSIGNALSLOT(pDlgWizardSelectTestOptions, ClickedNextButton(int), this, OnSelectTestOptions(int));
        pDlgWizardSelectTestOptions->exec();
        if (QDialog::Rejected == pDlgWizardSelectTestOptions->result())
        {
            delete pDlgWizardSelectTestOptions;
            return;
        }

        delete pDlgWizardSelectTestOptions;

        InitManufacturingDiagnostic();

        QTimer::singleShot(50, this, SLOT(FileExistanceCheck()));
    }
}

/****************************************************************************/
/*!
 *  \brief Initializes the Service user interface
 */
/****************************************************************************/
void CStartup::ServiceGuiInit()
{
    Global::EventObject::Instance().RaiseEvent(EVENT_LOGIN_SERVICEUSER, Global::tTranslatableStringList() << GetCurrentUserMode());
    LoadCommonComponenetsOne();

    mp_MainWindow->SetUserIcon(MainMenu::CMainWindow::Service);
    mp_MainWindow->SetUserMode("SERVICE");
    emit UpdateGUIConnector(mp_ServiceConnector, mp_MainWindow);

    //Diagnostics
    mp_MainWindow->AddMenuGroup(mp_DiagnosticsGroup, "Diagnostics");
    mp_DiagnosticsGroup->AddPanel("Display",      mp_Display);
    mp_DiagnosticsGroup->AddPanel("Retort",       mp_Retort);
    mp_DiagnosticsGroup->AddPanel("Oven",         mp_Oven);
    mp_DiagnosticsGroup->AddPanel("Rotary Valve", mp_RotaryValve);
    mp_DiagnosticsGroup->AddPanel("L&&A System",  mp_LaSystem);
    mp_DiagnosticsGroup->AddPanel("System",       mp_System);

    emit SetSettingsButtonStatus();
    LoadCommonComponenetsTwo();
}

void CStartup::InitManufacturingDiagnostic()
{
    if (mp_ManaufacturingDiagnosticsHandler) {
        delete mp_ManaufacturingDiagnosticsHandler;
        mp_ManaufacturingDiagnosticsHandler = NULL;
    }
    mp_ManaufacturingDiagnosticsHandler = new Core::CManufacturingDiagnosticsHandler(mp_ServiceConnector, mp_MainWindow);

    /* Manufacturing Tests */
    CONNECTSIGNALSIGNAL(mp_ManaufacturingDiagnosticsHandler, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID), this, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID));
    CONNECTSIGNALSLOTGUI(mp_FirmwareUpdate, BeginModuleTest(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>), mp_ManaufacturingDiagnosticsHandler, BeginManufacturingSWTests(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>));

    ManufacturingGuiInit();
}

/****************************************************************************/
/*!
 *  \brief Initializes the Manufacturing interface
 */
/****************************************************************************/
void CStartup::ManufacturingGuiInit()
{
    qDebug()<<"CStartup::ManufacturingGuiInit";

    Global::EventObject::Instance().RaiseEvent(EVENT_LOGIN_MANUFACTURING, Global::tTranslatableStringList() << GetCurrentUserMode());
    LoadCommonComponenetsOne();
    mp_MainWindow->SetUserMode("MANUFACTURING");
    emit UpdateGUIConnector(mp_ServiceConnector, mp_MainWindow);


    mp_ManaufacturingDiagnosticsHandler->LoadManufDiagnosticsComponents();

    LoadCommonComponenetsTwo();

    if (m_SelfTestFinished == false) {
        mp_ManaufacturingDiagnosticsHandler->ShowMessage(QApplication::translate("Core::Startup", "System is initializing ...", 0, QApplication::UnicodeUTF8));
    }

    //mp_ServiceConnector->SetLanguage(PlatformService::DEUTSCH);
    qDebug()<<"CStartup::ManufacturingGuiInit finished";
}

/****************************************************************************/
/*!
 *  \brief Xml file existance check
 */
/****************************************************************************/
int CStartup::FileExistanceCheck()
{
    return 1;

    int Result = 0;
    QDir Directory(Global::SystemPaths::Instance().GetSettingsPath());
    Directory.setFilter(QDir::AllEntries);

    QStringList FileNames, MissingFileNames;

    QFileInfoList List = Directory.entryInfoList();
    for (int i = 0; i < List.size(); i++)
    {
        QFileInfo fileInfo = List.at(i);
        FileNames.append(fileInfo.fileName());
    }

    if(!FileNames.contains("DeviceConfiguration.xml"))
        MissingFileNames.append("DeviceConfiguration.xml");
    if(!FileNames.contains("InstrumentHistory.xml"))
        MissingFileNames.append("InstrumentHistory.xml");
    if(!FileNames.contains("InstrumentHistoryArchive.xml"))
        MissingFileNames.append("InstrumentHistoryArchive.xml");
    if(!FileNames.contains("ServiceParameters.xml"))
        MissingFileNames.append("ServiceParameters.xml");
    if(!FileNames.contains("hw_specification.xml"))
        MissingFileNames.append("hw_specification.xml");

    if (!Directory.cd("Instrument"))
    {
         MissingFileNames.append("Instrument/ProcessSettings.xml");
         MissingFileNames.append("Instrument/Adjustment.xml");
    }
    else
    {
        QFileInfoList FileList = Directory.entryInfoList();
        for (int i = 0; i < FileList.size(); i++)
        {
            QFileInfo fileInfor = FileList.at(i);
            FileNames.append(fileInfor.fileName());
        }

        if(!FileNames.contains("Adjustment.xml"))
            MissingFileNames.append("Adjustment.xml");
        if(!FileNames.contains("ProcessSettings.xml"))
            MissingFileNames.append("ProcessSettings.xml");
     }

    if(MissingFileNames.count() > 0)
    {
        mp_MessageBox->setModal(true);
        mp_MessageBox->SetTitle(tr("Missing Xml files"));
        mp_MessageBox->SetButtonText(1, "Ok");
        mp_MessageBox->HideButtons();
        QString Text = QApplication::translate("Core::CStartup", "The following XML files are not present.\n", 0, QApplication::UnicodeUTF8);
        for(int i=0; i<MissingFileNames.count(); i++)
        {
            Text.append(MissingFileNames[i]);
            Text.append("\n");
        }
        mp_MessageBox->SetText(Text);
        mp_MessageBox->show();
        Result = mp_MessageBox->exec();
        //Global::EventObject::Instance().RaiseEvent(EVENT_SERVICE_XML_FILES_MISSING, Global::tTranslatableStringList() << Text);
        if(Result)
        {
            return 1;
        }
    }
    return 0;
}

/****************************************************************************/
/*!
 *  \brief Slot called to reset window status timer
 *
 */
/****************************************************************************/
void CStartup::ResetWindowStatusTimer()
{
//    Global::EventObject::Instance().RaiseEvent(EVENT_SERVICE_RESET_WINDOW_TIMER);
    //qDebug() << "ResetWindowStatusTimer";
    if (!mp_MainWindow->isHidden())
    {
        m_WindowStatusResetTimer.start();
    }
}
/****************************************************************************/
/*!
 *  \brief Slot called to updating timer and device name
 */
/****************************************************************************/
void CStartup::UpdateParameters()
{
    if (mp_ServiceConnector != NULL)
    {
        //m_WindowStatusResetTimer.setInterval(mp_ServiceConnector->GetServiceParameters()->GetSystemLogOffTime());
        m_WindowStatusResetTimer.setInterval(60000);
        //m_DeviceName = mp_ServiceConnector->GetDeviceConfigurationInterface()->GetDeviceConfiguration()->GetValue("DEVICENAME");
        m_DeviceName = "Primaris";
    }
    emit SetDeviceName(m_DeviceName);
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

void CStartup::RefreshTestStatus4RetortCoverSensor(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status)
{
    static bool openFlag = true;
    DiagnosticsManufacturing::CStatusConfirmDialog *dlg = new DiagnosticsManufacturing::CStatusConfirmDialog(mp_MainWindow);
    dlg->SetDialogTitle(DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id));

    dlg->SetText(QString("Do you see the retort lid '%1' ?").arg(openFlag ? "Open" : "Close"));
    dlg->UpdateRetortLabel(Status);

    int result = dlg->exec();
    qDebug()<<"StatusconfirmDlg return : " << result;
    delete dlg;
    if (result == 0) { // yes
        mp_ManaufacturingDiagnosticsHandler->OnReturnManufacturingMsg(true);
        openFlag = !openFlag;
    }
    else {
        mp_ManaufacturingDiagnosticsHandler->OnReturnManufacturingMsg(false);
        openFlag = true; // initial to open status.
    }
}

void CStartup::RefreshTestStatus4RetortHeatingWithWater(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status)
{
    QString CurStatus ;
    if (Status.value("CurrentStatus") != NULL) {
        if ( mp_HeatingStatusDlg != NULL ) {
            mp_HeatingStatusDlg->close();
            delete mp_HeatingStatusDlg ;
            mp_HeatingStatusDlg = NULL;
        }
        qDebug()<<"Get Message: "<<Status.value("CurrentStatus");

        CurStatus = Status.value("CurrentStatus");
        if (CurStatus == "WaitConfirm") {
            mp_ManaufacturingDiagnosticsHandler->OnReturnManufacturingMsg(true);
            return;
        }
        else if (CurStatus == "WaitConfirm2") {
            if (mp_HeatingStatusDlg) {
                mp_HeatingStatusDlg->close();
                delete mp_HeatingStatusDlg;
                mp_HeatingStatusDlg = NULL;
            }
            DiagnosticsManufacturing::CUserInputDialog *dlg = new DiagnosticsManufacturing::CUserInputDialog(Id, mp_MainWindow);
            dlg->exec();
            QString InputValueStr = dlg->GetInputValue(0);

            delete dlg;

            QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
            DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
            p_TestCase->AddResult("RetortTemp", InputValueStr);
            qreal MinValue = p_TestCase->GetParameter("TargetTemp").toDouble()+p_TestCase->GetParameter("DepartureLow").toDouble();
            qreal MaxValue = p_TestCase->GetParameter("TargetTemp").toDouble()+p_TestCase->GetParameter("DepartureHigh").toDouble();
            qreal InputValue = InputValueStr.toDouble();

            int result = false;
            if (InputValue >= MinValue && InputValue <= MaxValue) {
                result = true;
            }
            mp_ManaufacturingDiagnosticsHandler->OnReturnManufacturingMsg(result);
        }
        else if (CurStatus == "HideMessage") {
            mp_ManaufacturingDiagnosticsHandler->HideMessage();
            return ;
        }
        else if (CurStatus =="InformDone") {

            mp_ManaufacturingDiagnosticsHandler->OnReturnManufacturingMsg(true);
        }
        else {
            mp_ManaufacturingDiagnosticsHandler->HideMessage();
            mp_ManaufacturingDiagnosticsHandler->ShowMessage(Status.value("CurrentStatus"));
        }

        return ;
    }

    if (mp_HeatingStatusDlg == NULL) {
        mp_ManaufacturingDiagnosticsHandler->HideMessage();

        mp_HeatingStatusDlg = new DiagnosticsManufacturing::CHeatingTestDialog(Id, mp_MainWindow);
        mp_HeatingStatusDlg->HideAbort();
        mp_HeatingStatusDlg->show();
        mp_HeatingStatusDlg->UpdateLabel(Status);
        CONNECTSIGNALSIGNAL(mp_HeatingStatusDlg, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID), this, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID));

    }
    else {
        mp_HeatingStatusDlg->UpdateLabel(Status);
    }
}

void CStartup::RefreshTestStatus4OvenCoverSensor(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status)
{
    static bool OpenFlag = true;
    QString TestStatus;
    DiagnosticsManufacturing::CStatusConfirmDialog *dlg = new DiagnosticsManufacturing::CStatusConfirmDialog(mp_MainWindow);
    dlg->SetDialogTitle(DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id));

    if (OpenFlag) {
        TestStatus ="Open";
    }
    else {
        TestStatus = "Close";
    }
    dlg->SetText(QString("Do you see the cover sensor status shows '%1' ?").arg(TestStatus));

    dlg->UpdateOvenLabel(Status);
    int result = dlg->exec();
    qDebug()<<"StatusconfirmDlg return : "<<result;
    delete dlg;
    if (result == 0) { // yes
        mp_ManaufacturingDiagnosticsHandler->OnReturnManufacturingMsg(true);
        OpenFlag = !OpenFlag;
    }
    else {
        mp_ManaufacturingDiagnosticsHandler->OnReturnManufacturingMsg(false);
        OpenFlag = true; // initial to open status.
    }
}

void CStartup::RefreshTestStatus4RetortHeatingEmpty(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status)
{
    if (mp_HeatingStatusDlg == NULL) {
        mp_HeatingStatusDlg = new DiagnosticsManufacturing::CHeatingTestDialog(Id, mp_MainWindow);
        mp_HeatingStatusDlg->HideAbort();
        mp_HeatingStatusDlg->show();
        mp_HeatingStatusDlg->UpdateLabel(Status);
        CONNECTSIGNALSIGNAL(mp_HeatingStatusDlg, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID), this, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID));
    }
    else {
        mp_HeatingStatusDlg->UpdateLabel(Status);
    }
}

void CStartup::RefreshTestStatus4RetortLevelSensorHeating(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status)
{
    if (mp_HeatingStatusDlg == NULL) {
        mp_HeatingStatusDlg = new DiagnosticsManufacturing::CHeatingTestDialog(Id, mp_MainWindow);
        mp_HeatingStatusDlg->DisplayLSensorLabel();
        mp_HeatingStatusDlg->HideAbort();
        mp_HeatingStatusDlg->show();
        mp_HeatingStatusDlg->UpdateLabel(Status);
        CONNECTSIGNALSIGNAL(mp_HeatingStatusDlg, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID), this, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID));
    }
    else {
        mp_HeatingStatusDlg->UpdateLabel(Status);
    }
}

void CStartup::RefreshTestStatus4RetortLevelSensorDetecting(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &status)
{
    qDebug() << "RefreshTestStatus4RetortLevelSensorDetecting";

    mp_ManaufacturingDiagnosticsHandler->HideMessage();
    QString msg = status.value("CurrentStatus");
    if (msg.compare("WaitConfirm") == 0) {
        mp_ManaufacturingDiagnosticsHandler->OnReturnManufacturingMsg(true);
    }
    else if (msg.compare("InformDone") == 0) {
        mp_ManaufacturingDiagnosticsHandler->OnReturnManufacturingMsg(true);
    }
    else if (msg.compare("HideMessage") == 0) {
        mp_ManaufacturingDiagnosticsHandler->HideMessage();
    }
    else {
        mp_ManaufacturingDiagnosticsHandler->ShowMessage(status.value("CurrentStatus"));
    }
}

void CStartup::RefreshTestStatus4OvenHeatingEmpty(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status)
{
    if (mp_HeatingStatusDlg == NULL) {
        mp_HeatingStatusDlg = new DiagnosticsManufacturing::CHeatingTestDialog(Id, mp_MainWindow);
        mp_HeatingStatusDlg->HideAbort();
        mp_HeatingStatusDlg->show();
        mp_HeatingStatusDlg->UpdateLabel(Status);
        CONNECTSIGNALSIGNAL(mp_HeatingStatusDlg, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID), this, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID));
    }
    else {
        mp_HeatingStatusDlg->UpdateLabel(Status);
    }
}

void CStartup::RefreshTestStatus4OvenHeatingWater(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status)
{
    if (mp_HeatingStatusDlg == NULL) {
        mp_HeatingStatusDlg = new DiagnosticsManufacturing::CHeatingTestDialog(Id, mp_MainWindow);
        mp_HeatingStatusDlg->HideAbort();
        mp_HeatingStatusDlg->show();
        mp_HeatingStatusDlg->UpdateLabel(Status);
        CONNECTSIGNALSIGNAL(mp_HeatingStatusDlg, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID), this, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID));
    }
    else if (Status.value("CurrentStatus")=="InformDone") {
        if (mp_HeatingStatusDlg) {
            mp_HeatingStatusDlg->close();
            delete mp_HeatingStatusDlg;
            mp_HeatingStatusDlg = NULL;
        }
        DiagnosticsManufacturing::CUserInputDialog *dlg = new DiagnosticsManufacturing::CUserInputDialog(Id, mp_MainWindow);
        dlg->exec();
        QString LeftInputValueStr = dlg->GetInputValue(0);
        QString MiddleInputValueStr = dlg->GetInputValue(1);
        QString RightInputValueStr = dlg->GetInputValue(2);

        delete dlg;

        QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
        DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
        p_TestCase->AddResult("LeftSensorTemp", LeftInputValueStr);
        p_TestCase->AddResult("MiddleSensorTemp", MiddleInputValueStr);
        p_TestCase->AddResult("RightSensorTemp", RightInputValueStr);
        qreal MinValue = p_TestCase->GetParameter("TargetTemp").toDouble()+p_TestCase->GetParameter("DepartureLow").toDouble();
        qreal MaxValue = p_TestCase->GetParameter("TargetTemp").toDouble()+p_TestCase->GetParameter("DepartureHigh").toDouble();
        qreal LeftInputValue = LeftInputValueStr.toDouble();
        qreal MiddleInputValue = MiddleInputValueStr.toDouble();
        qreal RightInputValue = RightInputValueStr.toDouble();

        int result = false;
        if (LeftInputValue >= MinValue && LeftInputValue <= MaxValue &&
            MiddleInputValue >= MinValue && MiddleInputValue <= MaxValue &&
            RightInputValue >= MinValue && RightInputValue <= MaxValue) {
            result = true;
        }
        mp_ManaufacturingDiagnosticsHandler->OnReturnManufacturingMsg(result);

    }
    else {
        mp_HeatingStatusDlg->UpdateLabel(Status);
    }
}

void CStartup::RefreshTestStatus4LAHeatingBelt(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status)
{
    if (mp_HeatingStatusDlg == NULL) {
        mp_HeatingStatusDlg = new DiagnosticsManufacturing::CHeatingTestDialog(Id, mp_MainWindow);
        mp_HeatingStatusDlg->HideAbort();
        mp_HeatingStatusDlg->show();
        mp_HeatingStatusDlg->UpdateLabel(Status);
        CONNECTSIGNALSIGNAL(mp_HeatingStatusDlg, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID), this, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID));

    }
    else {
        mp_HeatingStatusDlg->UpdateLabel(Status);
    }
}

void CStartup::RefreshTestStatus4RVHeating(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status)
{
    if (Status.value("CurrentStatus") != NULL) {
        if ( mp_HeatingStatusDlg != NULL ) {
            mp_HeatingStatusDlg->close();
            delete mp_HeatingStatusDlg ;
            mp_HeatingStatusDlg = NULL;
        }
        qDebug()<<"Get Message: "<<Status.value("CurrentStatus");


        mp_ManaufacturingDiagnosticsHandler->HideMessage();
        if (Status.value("CurrentStatus") == "HideMessage") {
            return ;
        }
        mp_ManaufacturingDiagnosticsHandler->ShowMessage(Status.value("CurrentStatus"));

        return ;
    }

    if (mp_HeatingStatusDlg == NULL) {
        mp_ManaufacturingDiagnosticsHandler->HideMessage();

        mp_HeatingStatusDlg = new DiagnosticsManufacturing::CHeatingTestDialog(Id, mp_MainWindow);
        mp_HeatingStatusDlg->HideAbort();
        mp_HeatingStatusDlg->show();
        mp_HeatingStatusDlg->UpdateLabel(Status);
        CONNECTSIGNALSIGNAL(mp_HeatingStatusDlg, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID), this, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID));

    }
    else {
        mp_HeatingStatusDlg->UpdateLabel(Status);
    }
}

void CStartup::RefreshTestStatus4SystemSpeaker(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status)
{
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    mp_MessageBox->SetTitle("System speaker test");

    if (p_TestCase->GetParameter("VolumeFlag").toInt()) {
        mp_MessageBox->SetText(QString("Do you hear the system speak noice?"));
    }
    else {
        mp_MessageBox->SetText(QString("Do you hear the system speak noice which is higher than the previous ?"));
    }
    mp_MessageBox->SetButtonText(1, "No");
    mp_MessageBox->SetButtonText(3, "Yes");
    mp_MessageBox->HideCenterButton();

    if (!mp_MessageBox->exec()) { // yes
        mp_ManaufacturingDiagnosticsHandler->OnReturnManufacturingMsg(true);
    }
    else {
        mp_ManaufacturingDiagnosticsHandler->OnReturnManufacturingMsg(false);
    }

    emit PerformManufacturingTest(Service::TEST_ABORT, Id);
}

void CStartup::RefreshTestStatus4SystemAlarm(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status)
{
    QString TestStatus;
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    DiagnosticsManufacturing::CStatusConfirmDialog *dlg = new DiagnosticsManufacturing::CStatusConfirmDialog(mp_MainWindow);
    dlg->SetDialogTitle(DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id));

    bool ConnectFlag = p_TestCase->GetParameter("ConnectFlag").toInt();
    p_TestCase->SetParameter("ConnectFlag", QString::number(!ConnectFlag));
    if (ConnectFlag) {
        TestStatus = "Connected";
    }
    else {
        TestStatus = "DisConnected";
    }
    dlg->SetText(QString("Please confirm the alarm status '%1' ?").arg(TestStatus));

    dlg->UpdateLabel("Alarm Status", Status.value("AlarmStatus"));
    int result = dlg->exec();
    qDebug()<<"StatusconfirmDlg return : "<<result;
    delete dlg;
    emit PerformManufacturingTest(Service::TEST_ABORT, Id);

    if (result == 0) { // yes
        mp_ManaufacturingDiagnosticsHandler->OnReturnManufacturingMsg(true);
    }
    else {
        mp_ManaufacturingDiagnosticsHandler->OnReturnManufacturingMsg(false);
    }

}

void CStartup::RefreshTestStatus4SystemMainsRelay(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status)
{
    QString MessagetText;
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    mp_MessageBox->SetTitle("Mains relay test");
    mp_MessageBox->SetButtonText(3, "OK");
    mp_MessageBox->HideButtonsOneAndTwo();

    bool RelaySwitchStatus = p_TestCase->GetParameter("RelaySwitchStatus").toInt();

    p_TestCase->SetParameter("RelaySwitchStatus", QString::number(!RelaySwitchStatus));
    if (RelaySwitchStatus) {
        MessagetText = "relay switch on Spec.0.3A-1.3A";
    }
    else {
        MessagetText = "relay switch off Spec.&lt;0.15A";
    }

    MessagetText.append(QString("<br>ASB3 current:%1<br>Result:%2").arg(p_TestCase->GetResult().value("ASB3Current"), Status.value("Result")));

    mp_MessageBox->SetText(MessagetText);
    (void)mp_MessageBox->exec();

}

void CStartup::RefreshTestStatus4SystemExhaustFan(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status)
{
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
    QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);
    QString Text;

    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(mp_MainWindow);
    dlg->SetTitle(TestCaseDescription);
    dlg->SetIcon(QMessageBox::Information);

    Text = QString("Please check if the exhaust fan is runing, and check if the air flow direction is out of device.");

    dlg->SetText(Text);
    dlg->HideCenterButton();
    dlg->SetButtonText(3, tr("Pass"));
    dlg->SetButtonText(1, tr("Fail"));

    int ret = dlg->exec();

    delete dlg;

    if ( ret == 0 ) {
        mp_ManaufacturingDiagnosticsHandler->OnReturnManufacturingMsg(true);

    }
    else {
        mp_ManaufacturingDiagnosticsHandler->OnReturnManufacturingMsg(false);
    }
    emit PerformManufacturingTest(Service::TEST_ABORT, Id);
}

void CStartup::RefreshTestStatus4SystemOverflow(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status)
{
    mp_ManaufacturingDiagnosticsHandler->HideMessage();
    mp_ManaufacturingDiagnosticsHandler->ShowMessage(Status.value("CurrentStatus"));
}

void CStartup::RefreshTestStatus4SystemSealing(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status)
{
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
    int CurStep = p_TestCase->GetParameter("CurStep").toInt();

    if (CurStep == 3) {
        mp_ManaufacturingDiagnosticsHandler->HideMessage();
        if (mp_SealingStatusDlg == NULL) {
            mp_SealingStatusDlg = new DiagnosticsManufacturing::CSealingTestReportDialog(mp_MainWindow);

            CONNECTSIGNALSIGNAL(mp_SealingStatusDlg, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID), this, PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID));
            mp_SealingStatusDlg->UpdateLabel(Status);
            mp_SealingStatusDlg->show();
        }
        else {
            mp_SealingStatusDlg->UpdateLabel(Status);
        }
    }
    else if (Status.value("CurrentStatus") != NULL) {
        if (mp_SealingStatusDlg != NULL) {
            mp_SealingStatusDlg->close();
            delete mp_SealingStatusDlg ;
            mp_SealingStatusDlg = NULL;
        }

        if (Status.value("CurrentStatus") == "HideMessage") {
            mp_ManaufacturingDiagnosticsHandler->HideMessage();
        }
        else {
            mp_ManaufacturingDiagnosticsHandler->ShowMessage(Status.value("CurrentStatus"));
        }
    }
}

void CStartup::RefreshTestStatus4CleaningSystem(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status)
{
    mp_ManaufacturingDiagnosticsHandler->HideMessage();
    QString CleaningStatus = Status.value("CurrentStatus");
    mp_ManaufacturingDiagnosticsHandler->ShowMessage(CleaningStatus);
}

void CStartup::RefreshTestStatus4FirmwareUpdate(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status)
{
    int Index = Status.value("Index").toInt();
    if (Status.value("Result") == "true") {
        mp_ManaufacturingDiagnosticsHandler->OnReturnManufacturingMsg(true);
        mp_FirmwareUpdate->SetUpdateResult(Index, true);
    }
    else {
        mp_ManaufacturingDiagnosticsHandler->OnReturnManufacturingMsg(false);
        mp_FirmwareUpdate->SetUpdateResult(Index, false);
    }
}

void CStartup::RefreshTestStatus4FirmwareGetSlaveInfo(Service::ModuleTestCaseID Id, const Service::ModuleTestStatus &Status)
{
    qDebug()<<Status;
    ServiceDataManager::CModule* EboxModule = NULL;
    ServiceDataManager::CSubModule* SlaveModule = NULL;
    mp_ManaufacturingDiagnosticsHandler->OnReturnManufacturingMsg(true);

    if (mp_ServiceConnector->GetModuleListContainer()) {
        EboxModule = mp_ServiceConnector->GetModuleListContainer()->GetModule("Main Control");
    }

    int SlaveType = Status.value("SlaveType").toInt();
    //Status.remove("SlaveType");
    QString SlaveName;
    switch (SlaveType) {
    case 3:
        SlaveName = "ASB3";
        break;
    case 5:
        SlaveName = "ASB5";
        break;
    case 15:
        SlaveName = "ASB15";
        break;
    default:
        qDebug()<<"CStartup:RefreshTestStatus4FirmwareGetSlaveInfo: error slave type :"<<SlaveType;
        return;
    }

    if (EboxModule) {
        SlaveModule = EboxModule->GetSubModuleInfo(SlaveName);
    }
    if (SlaveModule) {
        QMap<QString, QString>::const_iterator itr = Status.constBegin();
        for (; itr != Status.constEnd(); ++itr) {
            if (itr.key() != "SlaveType" && !SlaveModule->UpdateParameterInfo(itr.key(), itr.value())) {
                qDebug()<<"Update "<<SlaveName<<" info "<<itr.key()
                       <<"to "<<itr.value()<<" failed.";
            }
        }
    }
}

/****************************************************************************/
/*!
 *  \brief Refresh heating status for heating test.
 *
 */
/****************************************************************************/
void CStartup::RefreshTestStatus(const QString &message, const Service::ModuleTestStatus &status)
{
    Service::ModuleTestCaseID id = DataManager::CTestCaseGuide::Instance().GetTestCaseId(message);

    switch(id) {
    case Service::OVEN_COVER_SENSOR:
        RefreshTestStatus4OvenCoverSensor(id, status);
        break;
    case Service::OVEN_HEATING_EMPTY:
        RefreshTestStatus4OvenHeatingEmpty(id, status);
        break;
    case Service::OVEN_HEATING_WITH_WATER:
        RefreshTestStatus4OvenHeatingWater(id, status);
        break;
    case Service::LA_SYSTEM_HEATING_LIQUID_TUBE:
    case Service::LA_SYSTEM_HEATING_AIR_TUBE:
        RefreshTestStatus4LAHeatingBelt(id, status);
        break;
    case Service::ROTARY_VALVE_HEATING_STATION:
    case Service::ROTARY_VALVE_HEATING_END:
        RefreshTestStatus4RVHeating(id, status);
        break;
    case Service::RETORT_HEATING_EMPTY:
        RefreshTestStatus4RetortHeatingEmpty(id, status);
        break;
    case Service::RETORT_LEVEL_SENSOR_HEATING:
        RefreshTestStatus4RetortLevelSensorHeating(id, status);
        break;
    case Service::RETORT_LEVEL_SENSOR_DETECTING:
        RefreshTestStatus4RetortLevelSensorDetecting(id, status);
        break;
    case Service::RETORT_LID_LOCK:
        RefreshTestStatus4RetortCoverSensor(id, status);
        break;
    case Service::RETORT_HEATING_WITH_WATER:
        RefreshTestStatus4RetortHeatingWithWater(id, status);
        break;
    case Service::SYSTEM_SPEARKER:
        RefreshTestStatus4SystemSpeaker(id, status);
        break;
    case Service::SYSTEM_REMOTE_LOCAL_ALARM:
        RefreshTestStatus4SystemAlarm(id, status);
        break;
    case Service::SYSTEM_MAINS_RELAY:
        RefreshTestStatus4SystemMainsRelay(id, status);
        break;
    case Service::SYSTEM_EXHAUST_FAN:
        RefreshTestStatus4SystemExhaustFan(id, status);
        break;
    case Service::SYSTEM_OVERFLOW:
        RefreshTestStatus4SystemOverflow(id, status);
        break;
    case Service::SYSTEM_SEALING_TEST:
        RefreshTestStatus4SystemSealing(id, status);
        break;
    case Service::CLEANING_SYSTEM_TEST:
        RefreshTestStatus4CleaningSystem(id, status);
        break;
    case Service::FIRMWARE_UPDATE:
        RefreshTestStatus4FirmwareUpdate(id, status);
        break;
    case Service::FIRMWARE_GET_SLAVE_INFO:
        RefreshTestStatus4FirmwareGetSlaveInfo(id, status);
        break;
    case Service::PRESSURE_CALIBRATION:
        mp_CalibrationHandler->RefreshCalibrationMessagetoMain(status);
        break;
    case Service::SYSTEM_SELF_TEST:
    {
        if (status.value("CurState")=="Begin") {
            m_SelfTestFinished = false;
        }
        else {
            m_SelfTestFinished = true;
            if (mp_ManaufacturingDiagnosticsHandler) {
                mp_ManaufacturingDiagnosticsHandler->HideMessage();
            }
        }
        break;
    }
    default:
        break;
    }
}

/****************************************************************************/
/*!
 *  \brief Returns message for manufacturing tests
 *  \iparam Result = true or false
 */
/****************************************************************************/
void CStartup::OnReturnManufacturingMsg(bool Result)
{
    if (mp_HeatingStatusDlg) {
        mp_HeatingStatusDlg->close();
        delete mp_HeatingStatusDlg;
        mp_HeatingStatusDlg = NULL;
    }
    qDebug()<<"CStartup::OnReturnManufacturingMsg Result="<<Result;
    mp_ManaufacturingDiagnosticsHandler->OnReturnManufacturingMsg(Result);
}


/****************************************************************************/
/*!
 *  \brief Throws open a Pop Up Message with Custom Messages
 *
 */
/****************************************************************************/
void CStartup::ShowCalibrationInitMessagetoMain(const QString &Message, bool OkStatus)
{
    qDebug()<<"CStartup::ShowCalibrationInitMessagetoMain";
    mp_CalibrationHandler->ShowCalibrationInitMessagetoMain(Message, OkStatus);
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
    qDebug()<<"CStartup::OnGuiAbortTest -- emit AbortTest";

// Disabled by Sunny on May, 21, 2014 for test
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
 *  \brief Basic color test
 *
 */
/****************************************************************************/
void CStartup::OnBasicColorTest()
{
    mp_Display->DoBasicColorTest();
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

/****************************************************************************/
/*!
 *  \brief Slot called to display log information
 *  \iparam FileName = Log file name
 *  \iparam FilePath = Log file path
 */
/****************************************************************************/
void CStartup::DisplayLogInformation(QString FileName, QString FilePath)
{
    QString Path = FilePath + "/" + FileName;
    if (FileName.startsWith("HimalayaEvents_12345678")) {  // System log
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_LOGVIEWER_SYSTEMLOG_DISPLAY_INFO);
        if (mp_SystemLogContentDlg != NULL) {
            delete mp_SystemLogContentDlg;
            mp_SystemLogContentDlg = NULL;
        }
        mp_SystemLogContentDlg = new LogViewer::CSystemLogViewerDlg(mp_MainWindow);
        mp_SystemLogContentDlg->setModal(true);
        mp_SystemLogContentDlg->resize(720, 450);
        //mp_SystemLogContentDlg->setGeometry(110, 300, 720, 480);
        mp_SystemLogContentDlg->SetDialogTitle(FileName.remove(".log", Qt::CaseSensitive));
        mp_SystemLogContentDlg->InitDialog(Path);
        mp_SystemLogContentDlg->show();
    }
    else {
        QStringList HeaderLabels;
        QList<int> Columns;

        if (FileName.startsWith("Himalaya_Service")) {  // Service log
            Global::EventObject::Instance().RaiseEvent(EVENT_GUI_LOGVIEWER_SERVICELOG_DISPLAY_INFO);
            HeaderLabels.append(m_strDate);
            HeaderLabels.append(m_strTimeStamp);
            HeaderLabels.append(m_strDescription);
            Columns.append(0);
            Columns.append(3);
        }
        else if (FileName.startsWith("RecoveryActionText")) { // Recovery Action
            Global::EventObject::Instance().RaiseEvent(EVENT_GUI_LOGVIEWER_SERVICERECOVERYACTION_DISPLAY_INFO);
            HeaderLabels.append(m_strError);
            HeaderLabels.append(m_strDescription);
            HeaderLabels.append(m_strRecoveryActionText);
            Columns.append(0);
            Columns.append(1);
            Columns.append(2);
        }
        else if (FileName.startsWith("Himalaya_SW_Update_Events"))  {// SW Update log
            Global::EventObject::Instance().RaiseEvent(EVENT_GUI_LOGVIEWER_SOFTWAREUPDATELOG_DISPLAY_INFO);
            HeaderLabels.append(m_strDate);
            HeaderLabels.append(m_strTimeStamp);
            HeaderLabels.append(m_strType);
            HeaderLabels.append(m_strDescription);
            Columns.append(0);
            Columns.append(3);
            Columns.append(5);
        }

        if (mp_LogContentDlg != NULL) {
            delete mp_LogContentDlg;
            mp_LogContentDlg = NULL;
        }

        mp_LogContentDlg = new LogViewer::CLogContentDlg(HeaderLabels, Columns, mp_MainWindow);
        mp_LogContentDlg->setModal(true);
        mp_LogContentDlg->resize(720, 450);
        mp_LogContentDlg->SetDialogTitle(FileName.remove(".log", Qt::CaseSensitive));
        (void) mp_LogContentDlg->InitDialog(Path);
        mp_LogContentDlg->show();
    }
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CStartup::RetranslateUI()
{
    Service::CMessageString::RetranslateUI();

    m_strDate = QApplication::translate("Core::CStartup", "Date", 0, QApplication::UnicodeUTF8);
    m_strError = QApplication::translate("Core::CStartup", "Error", 0, QApplication::UnicodeUTF8);
    m_strType = QApplication::translate("Core::CStartup", "Type", 0, QApplication::UnicodeUTF8);
    m_strTimeStamp = QApplication::translate("Core::CStartup", "TimeStamp", 0, QApplication::UnicodeUTF8);
    m_strDescription = QApplication::translate("Core::CStartup", "Description", 0, QApplication::UnicodeUTF8);
    m_strRecoveryActionText = QApplication::translate("Core::CStartup", "Recovery Action Text", 0, QApplication::UnicodeUTF8);
    /*if (mp_ServiceConnector->GetSoftwareMode() == PlatformService::SERVICE_MODE) {
        ServiceGuiInit();
    } else {
        ManufacturingGuiInit();
    }*/
}

} // end namespace Core
