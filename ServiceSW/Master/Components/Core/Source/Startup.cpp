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
    mp_HeatingStatusDlg(NULL)
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
    mp_Retort           = new Diagnostics::CRetort;
    mp_Oven             = new Diagnostics::COven;
    mp_RotaryValve      = new Diagnostics::CRotaryValve;
    mp_LaSystem         = new Diagnostics::CLaSystem;
    mp_System           = new Diagnostics::CSystem;

    //Diagnostics1 Manufacturing
    mp_DiagnosticsManufGroup = new MainMenu::CMenuGroup;

    // Software upate
    mp_ServiceUpdateGroup = new MainMenu::CMenuGroup;
    mp_FirmwareUpdate = new ServiceUpdates::CFirmwareUpdate;
    mp_DataManagement = new ServiceUpdates::CDataManagement;
    mp_Setting = new ServiceUpdates::CSettings(mp_ServiceConnector, mp_MainWindow);

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
    /* Calibration Signals */
    CONNECTSIGNALSIGNAL(mp_CalibrationHandler, OvenLidInitCalibrationRequest(), this, OvenLidInitCalibrationRequest());
    CONNECTSIGNALSIGNAL(mp_CalibrationHandler, PressureSensorCalibrationRequest(), this, PressureSensorCalibrationRequest());

    CONNECTSIGNALSLOT(this, SetSettingsButtonStatus(), mp_Setting, ResetButtonStatus());
    CONNECTSIGNALSLOT(this, UpdateGUIConnector(Core::CServiceGUIConnector*, MainMenu::CMainWindow*), mp_Setting, UpdateGUIConnector(Core::CServiceGUIConnector*, MainMenu::CMainWindow*));

    CONNECTSIGNALSLOT(mp_MainWindow, onChangeEvent(), this, RetranslateUI());
    CONNECTSIGNALSLOT(mp_ServiceConnector, ServiceParametersChanged(), this, UpdateParameters());
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
        delete mp_AddModifyConfig;
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
    mp_CalibrationHandler->LoadCalibrationGUIComponenets();

    // Service update
    mp_ServiceUpdateGroup->AddPanel("Firmware Update", mp_FirmwareUpdate);
    mp_ServiceUpdateGroup->AddPanel("Data Management", mp_DataManagement);
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
    mp_USBKeyValidator = new ServiceKeyValidator::CUSBKeyValidator("Himalaya");

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
        MainMenu::CDlgWizardSelectTestOptions* pDlgWizardSelectTestOptions = new MainMenu::CDlgWizardSelectTestOptions(NULL, NULL);
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
    mp_DiagnosticsGroup->AddPanel("Retort",       mp_Retort);
    mp_DiagnosticsGroup->AddPanel("Oven",         mp_Oven);
    mp_DiagnosticsGroup->AddPanel("Rotary Valve", mp_RotaryValve);
    mp_DiagnosticsGroup->AddPanel("L&&A System",   mp_LaSystem);
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
    CONNECTSIGNALSIGNAL(mp_ManaufacturingDiagnosticsHandler, PerformManufacturingTest(Service::ModuleTestCaseID), this, PerformManufacturingTest(Service::ModuleTestCaseID));
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
    qDebug() << "ResetWindowStatusTimer";
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
        m_DeviceName = "Himalaya";
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

/****************************************************************************/
/*!
 *  \brief Refresh heating status for heating test.
 *
 */
/****************************************************************************/
void CStartup::RefreshHeatingStatus(const QString &Message, const Service::ModuleTestStatus &Status)
{
    qDebug()<<"CStartup::RefreshHeatingStatus --"<<Message;

    if (Message == "OvenHeatingEmpty" || Message == "OvenHeatingWater") {
        if (mp_HeatingStatusDlg == NULL) {
            bool Flag = false;
            if (Message == "OvenHeatingEmpty")
                Flag = true;
            mp_HeatingStatusDlg = new DiagnosticsManufacturing::CHeatingTestDialog(Flag, mp_MainWindow);
            mp_HeatingStatusDlg->HideAbort();
            mp_HeatingStatusDlg->show();
            mp_HeatingStatusDlg->UpdateLabel(Status);
        }
        else {
            mp_HeatingStatusDlg->UpdateLabel(Status);
        }
    }
    else if (Message == "OvenCoverSensor") {
        static bool OpenFlag = true;
        QString TestStatus;
        DiagnosticsManufacturing::CStatusConfirmDialog *dlg = new DiagnosticsManufacturing::CStatusConfirmDialog(mp_MainWindow);

        if (OpenFlag) {
            TestStatus ="Open";
        }
        else {
            TestStatus = "Close";
        }
        dlg->SetText(QString("Do you see the coven sensor status shows '%1' ?").arg(TestStatus));

        dlg->UpdateLabel(Status);
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

//    mp_ServiceConnector->HideBusyDialog();
//    mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, Message);
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
