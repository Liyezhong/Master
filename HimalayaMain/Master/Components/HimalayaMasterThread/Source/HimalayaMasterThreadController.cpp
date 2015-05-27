/****************************************************************************/
/*! \file HimalayaMasterThread/Source/HimalayaMasterThreadController.cpp
 *
 *  \brief Implementation file for class HimalayaMasterThreadController.
 *
 *  $Version:   $ 0.2
 *  $Date:      $ 2010-07-12, 2012-07-12, 2013-03-15
 *  $Author:    $ J.Bugariu, Raju, AbeYang
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

#include <HimalayaMasterThread/Include/HimalayaMasterThreadController.h>
#include <DataManager/Containers/UserSettings/Include/UserSettingsInterface.h>
#include <DataManager/Helper/Include/XmlConfigFilePasswords.h>

#include <HimalayaGuiController/Include/HimalayaGuiController.h>
#include <ExportController/Include/ExportController.h>
#include <Scheduler/Include/SchedulerMainThreadController.h>
#include <Global/Include/SystemPaths.h>
#include <Global/Include/Utils.h>
#include <Global/Include/GlobalDefines.h>

#include <NetCommands/Include/CmdScheduledStations.h>
#include <NetCommands/Include/CmdProcessState.h>
#include <NetCommands/Include/CmdExecutionStateChanged.h>
#include <NetCommands/Include/CmdDayRunLogRequest.h>
#include <NetCommands/Include/CmdLanguageFile.h>
#include <HimalayaDataContainer/Containers/Programs/Commands/Include/CmdProgramDeleteItem.h>
#include <NetCommands/Include/CmdProblemReportReply.h>
#include <NetCommands/Include/CmdEventReport.h>
#include <HimalayaDataContainer/Containers/Programs/Commands/Include/CmdProgramUpdate.h>
#include <NetCommands/Include/CmdSwitchToService.h>
#include <NetCommands/Include/CmdConfigurationFile.h>
#include <HimalayaDataContainer/Containers/DashboardStations/Include/DashboardStation.h>

#include <DataManager/Containers/Stations/Include/StationBase.h>
#include <NetCommands/Include/CmdExternalProcessState.h>
#include <DeviceCommandProcessor/Include/Commands/CmdDeviceProcessingInit.h>
#include <DeviceCommandProcessor/Include/Commands/CmdDeviceProcessingCleanup.h>

#include <Global/Include/Commands/CmdEventUpdate.h>
#include <Global/Include/Translator.h>
#include <Global/Include/UITranslator.h>

#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramListVerifier.h"
#include "HimalayaDataContainer/Containers/Reagents/Include/DataReagentListVerifier.h"
#include "DataManager/Containers/UserSettings/Include/UserSettingsVerifier.h"
#include "DataManager/Containers/DeviceConfiguration/Include/DeviceConfigurationInterface.h"

#include "HimalayaDataManager/Include/DataManager.h"

#include <Global/Include/EventObject.h>
#include <EventHandler/Include/StateHandler.h>

#include <HimalayaEventCodes.h>
#include <HimalayaProcessExitCodes.h>
#include <EventHandler/Include/EventHandlerEventCodes.h>
#include <EventHandler/Include/StateHandler.h>
#include <NetCommands/Include/CmdCriticalActionCheck.h>
#include <NetCommands/Include/CmdCriticalActionStatus.h>
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAction.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdLockStatus.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdRetortStatus.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdStationSuckDrain.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdCurrentProgramStepInfor.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelectedReply.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelected.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdKeepCassetteCount.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdTakeOutSpecimenFinished.h"
#include "Scheduler/Commands/Include/CmdSystemState.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAcknowledge.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdEnterCleaningProgram.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAborted.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdRecoveryFromPowerFailure.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdUpdateProgramEndTime.h"
#include "HimalayaDataContainer/Containers/UserSettings/Commands/Include/CmdParaffinMeltPointChanged.h"
#include "HimalayaDataContainer/Containers/UserSettings/Commands/Include/CmdBottleCheck.h"
#include "HimalayaDataContainer/Containers/UserSettings/Commands/Include/CmdBottleCheckReply.h"
#include "HimalayaDataContainer/Helper/Include/Global.h"

#include <SWUpdateManager/Include/SWUpdateManager.h>
#include <Global/Include/Commands/CmdShutDown.h>
#include <Global/Include/Commands/Command.h>

#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "Scheduler/Commands/Include/CmdRmtLocAlarm.h"
#include "EventHandler/Include/HimalayaEventHandlerThreadController.h"
#include <DataManager/CommandInterface/Include/UserSettingsCommandInterface.h>

//lint -e578

namespace Himalaya {
const quint32 ERROR_CODE_HIMALAYA_CONSTRUCTION_FAILED = 1;       ///<  Definition/Declaration of variable ERROR_CODE_HIMALAYA_CONSTRUCTION_FAILED
const Global::gSubComponentType SUBCOMPONENT_ERRORHANDLER   = 0x0001;   ///< Subcomponent id for error handler.

/****************************************************************************/
HimalayaMasterThreadController::HimalayaMasterThreadController() try:
    MasterThreadController("HimalayaShutdown"),
    /// \todo hash of master password (now master password is "Himalaya")
//    m_PasswordManager("6C7F722B0C5AC1BD70BE4ECC6FC0653E"),
    m_CommandChannelGui(this, "Gui", Global::EVENTSOURCE_NONE),
    m_CommandChannelSchedulerMain(this, "SchedulerMain", Global::EVENTSOURCE_SCHEDULER),
    m_CommandChannelEventHandler(this, "EventHandler", Global::EVENTSOURCE_EVENTHANDLER),
    m_CommandChannelExport(this, "Export", Global::EVENTSOURCE_EXPORT),
    m_CommandChannelImportExport(this, "ImportExport", Global::EVENTSOURCE_IMPORTEXPORT),
    mp_DataManager(NULL),
    mp_ImportExportAckChannel(NULL),
    m_ExportProcessIsFinished(false),
    m_ImportExportThreadIsRunning(false),
    m_RemoteCareExportRequest(false),
    m_RemoteCareExportRequestInitiated(false),
 // m_Simulation(true),
    m_Simulation(false),
    m_AuthenticatedLevel(Global::OPERATOR),
    m_ControllerCreationFlag(false),
    m_CurrentUserActionState(NORMAL_USER_ACTION_STATE),
    m_ExportTargetFileName(""),
    //mp_SchdCmdProcessor(NULL),
    m_ExpectedShutDownRef(Global::RefManager<Global::tRefType>::INVALID),
    m_bQuitApp(false)
{
}
catch (...) {
    //!< We Log to a temporary file any exception occured in the construction and  rethrow.
   Global::EventObject::Instance().LogToTemporaryFileAndThrowArgs(ERROR_CODE_HIMALAYA_CONSTRUCTION_FAILED);
}

/****************************************************************************/
HimalayaMasterThreadController::~HimalayaMasterThreadController()
{
}


void HimalayaMasterThreadController::CreateAlarmHandler()
{
//    mp_alarmHandler = new HimalayaErrorHandler::HimalayaAlarmHandler(5000, this);
//    CONNECTSIGNALSLOT(mp_alarmHandler, sigSetTimeout(quint16), this, SetAlarmHandlerTimeout(quint16));
//    CONNECTSIGNALSLOT(mp_alarmHandler, FireAlarmLocalRemote(bool), this, OnFireAlarmLocalRemote(bool));
}

/****************************************************************************/
void HimalayaMasterThreadController::CreateAndInitializeObjects() {
    try {
    //Move Event Object to Master thread, so that there is an event loop to process events
    // MTL: Moving singletons to other threads leads to an unstable system and segmentation fault
//    Global::EventObject::Instance().moveToThread(this->thread());
//    EventHandler::StateHandler::Instance().moveToThread(this->thread());


    SetEventStringFileList(QStringList() << Global::SystemPaths::Instance().GetSettingsPath() + QDir::separator() + "PlatformEventConfig.xml"
                           << Global::SystemPaths::Instance().GetSettingsPath() + QDir::separator() + "EventConfig.xml");

    //CreateBasic threads
    /*! Basic threads cannot be constructed in MasterThread contructor
     *  since , the objects created in constructor will have affinity
     *  to Main thread not Master thread. Hence, we wont be able to
     *  use main thread to move the threadcontrollers to their own threads
     */
    CreateBasicControllersAndThreads();

    /*!  We create Datamanager not in the constructor  of Master thread
     *   but here since, the objects allocated on the Heap in the
     *   constructor of Himalaya Master thread controller
     *   have to be moved to thread again ,else all the events will be
     *   handled by the Main Thread not the Himalaya Master Thread.
     */

    //! Create DataManager
    /*! If DataManager Initialization is failed, appropriate event is raised
     *  and it will be caught by event handler. These events will be processed
     *  once EventHandler is intialized, since at this moment , Event Handler is
     *  just created, and would not have read EventConfig.csv.
     */

    mp_DataManager = new DataManager::CDataManager(this);
    //initialize the DataManagerBase pointer in MasterThread
    mp_DataManagerBase = mp_DataManager;

    mp_SWUpdateManager = new SWUpdate::SWUpdateManager(*this);
    CONNECTSIGNALSLOT(mp_DataManager->mp_SettingsCommandInterface, UserSettingsChanged(const bool), this , OnLanguageChanged(const bool));
    CONNECTSIGNALSLOT(mp_SWUpdateManager, RollBackComplete(), this, SWUpdateRollbackComplete());
    CONNECTSIGNALSLOT(mp_SWUpdateManager, WaitDialog(bool, Global::WaitDialogText_t),
                      this, ShowWaitDialog(bool,Global::WaitDialogText_t));
    CONNECTSIGNALSLOT(mp_SWUpdateManager, SWUpdateStatus(bool),
                      this, SWUpdateProgress(bool));
    CONNECTSIGNALSLOT(this, UpdateSoftwareFromRC(), mp_SWUpdateManager, OnSWUpdateFromRC());

    //Initialize objects in Master and base threads.
    if(mp_DataManager && mp_DataManager->GetSWVersion())
    {
        SetSWVersion(mp_DataManager->GetSWVersion()->GetSWReleaseVersion());
    }
    MasterThreadController::CreateAndInitializeObjects();

    RegisterCommandForProcessing<NetCommands::CmdExternalProcessState, HimalayaMasterThreadController>
            (&HimalayaMasterThreadController::ExternalProcessConnectionHandler, this);

    //EventHandler Commands
    RegisterCommandForProcessing<NetCommands::CmdSystemAction, HimalayaMasterThreadController>
        (&HimalayaMasterThreadController::EventCmdSystemAction,this);
    RegisterAcknowledgeForProcessing<Global::AckOKNOK, HimalayaMasterThreadController>
        (&HimalayaMasterThreadController::OnAckOKNOK, this);
    RegisterCommandForRouting<NetCommands::CmdEventReport>(&m_CommandChannelGui);
    RegisterCommandForProcessing<Global::CmdShutDown, HimalayaMasterThreadController>
                (&HimalayaMasterThreadController::ShutdownHandler, this);
		CONNECTSIGNALSLOT(this, RemoteCareExport(const quint8 &), this, RemoteCareExportData(const quint8 &));

        CONNECTSIGNALSLOT(this, SendRCCmdToGui(const Global::CommandShPtr_t &),
                                     this, SendRCCmdToGuiChannel(const Global::CommandShPtr_t &));
        CONNECTSIGNALSLOT(this, SendCmdToGui(const Global::CommandShPtr_t &),
                                     this, SendRCCmdToGuiChannel(const Global::CommandShPtr_t &));
    }
    catch (...) {
        qDebug()<<"Create And Initialize Failed";
    }
}

/****************************************************************************/
void HimalayaMasterThreadController::CreateControllersAndThreads() {
  // let base class create controllers and threads
    MasterThreadController::CreateControllersAndThreads();
    CHECKPTR(mp_DataManager);
    CHECKPTR(mp_DataManager->GetDeviceConfigurationInterface());

    if (!mp_DataManager)
        return;

    if (!mp_DataManager->GetDeviceConfigurationInterface())
        return;

    DataManager::CDeviceConfiguration *p_DeviceConfiguration = mp_DataManager->GetDeviceConfigurationInterface()->GetDeviceConfiguration();
    CHECKPTR(p_DeviceConfiguration);

    // create and connect scheduler main controller
    Scheduler::SchedulerMainThreadController* schedulerMainController = new Scheduler::SchedulerMainThreadController(THREAD_ID_SCHEDULER);
    AddAndConnectController(schedulerMainController, &m_CommandChannelSchedulerMain, static_cast<int>(SCHEDULER_MAIN_THREAD));
    schedulerMainController->DataManager(mp_DataManager);
    // register all commands for processing and routing
    RegisterCommands();

    // read password file
    DataManager::XmlConfigFilePasswords PwdFile(GetSerialNumber());
    PwdFile.ReadPasswords(Global::SystemPaths::Instance().GetSettingsPath() + "/Password.xml", m_PasswordManager);
    (void)connect(&EventHandler::StateHandler::Instance(), SIGNAL(stateChanged(QString)), this, SLOT(SendStateChange(QString)));
}


/************************************************************************************************************************************/
void HimalayaMasterThreadController::OnPowerFail(const Global::PowerFailStages PowerFailStage) {
    Q_UNUSED(PowerFailStage)
    if(mp_DataManager) {
        mp_DataManager->SaveDataOnShutdown();
    }
//    if (PowerFailStage == Global::POWER_FAIL_STAGE_2) {
//        m_PowerFailed = true;
//        if (EventHandler::StateHandler::Instance().getCurrentOperationState() != "SoftSwitchMonitorState") {
//            m_RebootFileContent.insert("PowerFailed", "Yes");
//            UpdateRebootFile();
//        }
//        if (mp_SWUpdateManager) {
//            //Halt SW update
//            mp_SWUpdateManager->PowerFailed();
//        }
//        QTimer::singleShot(2000, this, SLOT(ShutdownOnPowerFail()));
//    }
}

/****************************************************************************/
void HimalayaMasterThreadController::OnGoReceived() {
    CreateAndInitializeObjects();
    MasterThreadController::OnGoReceived();
    Scheduler::SchedulerLogging::getInstance().InitLog(m_SerialNumber,mp_DataManager->GetSWVersion()->GetSWReleaseVersion());
    DataManager::CDeviceConfigurationInterface *deviceConfigInterface = mp_DataManager->GetDeviceConfigurationInterface();
    QString SerialNumber;
    QString DeviceName;
    if (deviceConfigInterface) {
        DataManager::CDeviceConfiguration *deviceConfig = deviceConfigInterface->GetDeviceConfiguration();

        if (deviceConfig) {
            SerialNumber = deviceConfig->GetValue("SerialNumber");
            DeviceName = deviceConfig->GetValue("DeviceName");
        }
    }
    Global::EventObject::Instance().RaiseEvent(EVENT_HIMALAYA_START, Global::FmtArgs() << SerialNumber << DeviceName, true);
    EventHandler::StateHandler::Instance().setStateToSoftSwitchMonitorState();
    EventHandler::StateHandler::Instance().setInitState();
    InitializeGUI();
}

/************************************************************************************************************************************/
void HimalayaMasterThreadController::InitiateShutdown(bool Reboot) {
    qDebug() << "HimalayaMasterThreadController::InitiateShutdown";
    //save data to the file system
    if(!m_PowerFailed && mp_DataManager) { //if power fails we would have already written the data
        mp_DataManager->SaveDataOnShutdown();
    }

    if (!Reboot) {
        m_BootConfigFileContent.insert("Main_Rebooted", "No");
        m_BootConfigFileContent.insert("Reboot_Count", "0");
        Global::UpdateRebootFile(m_BootConfigFileContent);
    }
    if (!m_PowerFailed && mp_DataManager && mp_DataManager->IsInitialized()) {
        // shutdown device command processor
        m_ExpectedShutDownRef = SendCommand(Global::CommandShPtr_t(new MsgClasses::CmdQuitAppShutdown(Global::Command::MAXTIMEOUT, DataManager::QUITAPPSHUTDOWNACTIONTYPE_UNDEFINED)),
                                            m_CommandChannelSchedulerMain);
       //! \note Shutdown() is called when ShutDownDevice() is completed
    }

    //! \todo Remove this once Command channels are allocated on heap-> Added by N.Kamath
    m_CommandChannelExport.setParent(NULL);
    m_CommandChannelImportExport.setParent(NULL);

    if (m_ExpectedShutDownRef == 0) { //!< In case DCP was not created, the Dev clean up command wont be sent, hence we shutdown here !
        Global::EventObject().Instance().RaiseEvent(Global::EVENT_SHUTDOWN_ON_CHECK_FLAG2, true);
        Shutdown();
    }
}

/****************************************************************************/
void HimalayaMasterThreadController::SetDateTime(Global::tRefType Ref, const Global::CmdDateAndTime &Cmd,
                                                 Threads::CommandChannel &AckCommandChannel) {

    QString DateTime;
    if(!MasterThreadController::SetAdjustedDateTimeOffset(Cmd.GetDateTime())) {

        // remove the offset seconds from the Adjusted time
        DateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs
                (-Global::AdjustedTime::Instance().GetOffsetSeconds()).toString();
        // raise the event
        Global::EventObject::Instance().RaiseEvent(EVENT_DATE_TIME_CANNOT_BE_MORE_THAN_24_HOURS,
                                                   Global::FmtArgs() << DateTime, true);

        QString TranslatedString = Global::UITranslator::TranslatorInstance().Translate
                (Global::TranslatableString(EVENT_DATE_TIME_CANNOT_BE_MORE_THAN_24_HOURS,
                                            Global::tTranslatableStringList()
                                            << DateTime));
        // send NACK
        SendAcknowledgeNOK(Ref, AckCommandChannel, TranslatedString, Global::GUIMSGTYPE_WARNING);
    }
    else {
        // send ACK
        SendAcknowledgeOK(Ref, AckCommandChannel);
        DateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        Global::EventObject::Instance().RaiseEvent(EVENT_DATE_TIME_SET_DATE_TIME,Global::tTranslatableStringList()<< DateTime);
    }

}
void HimalayaMasterThreadController::ShutdownOnPowerFail()
{
    Global::EventObject().Instance().RaiseEvent(Global::EVENT_SHUTDOWN_ON_CHECK_FLAG13, true);
    InitiateShutdown();
}
/****************************************************************************/
void HimalayaMasterThreadController::RegisterCommands() {
    //Dashboard
    RegisterCommandForRouting<MsgClasses::CmdProgramAction>(&m_CommandChannelSchedulerMain);//GUI-->master->Scheduler
    RegisterCommandForRouting<MsgClasses::CmdLockStatus>(&m_CommandChannelGui);
    RegisterCommandForRouting<MsgClasses::CmdRetortStatus>(&m_CommandChannelGui);
    RegisterCommandForRouting<MsgClasses::CmdStationSuckDrain>(&m_CommandChannelGui);
    RegisterCommandForRouting<MsgClasses::CmdProgramSelectedReply>(&m_CommandChannelGui);
    RegisterCommandForRouting<MsgClasses::CmdCurrentProgramStepInfor>(&m_CommandChannelGui);
    RegisterCommandForRouting<MsgClasses::CmdProgramAcknowledge>(&m_CommandChannelGui);
    RegisterCommandForRouting<MsgClasses::CmdEnterCleaningProgram>(&m_CommandChannelGui);
    RegisterCommandForRouting<MsgClasses::CmdProgramAborted>(&m_CommandChannelGui);
    RegisterCommandForRouting<MsgClasses::CmdProgramSelected>(&m_CommandChannelSchedulerMain);
    RegisterCommandForRouting<MsgClasses::CmdKeepCassetteCount>(&m_CommandChannelSchedulerMain);
    RegisterCommandForRouting<MsgClasses::CmdTakeOutSpecimenFinished>(&m_CommandChannelSchedulerMain);
    RegisterCommandForRouting<MsgClasses::CmdParaffinMeltPointChanged>(&m_CommandChannelSchedulerMain);
    RegisterCommandForRouting<MsgClasses::CmdBottleCheck>(&m_CommandChannelSchedulerMain);
    RegisterCommandForRouting<MsgClasses::CmdUpdateProgramEndTime>(&m_CommandChannelGui);
    RegisterCommandForRouting<MsgClasses::CmdRecoveryFromPowerFailure>(&m_CommandChannelGui);

    // -> Datalogging
    RegisterCommandForRouting<NetCommands::CmdDayRunLogRequest>(&m_CommandChannelDataLogging);    
    RegisterCommandForRouting<NetCommands::CmdDayRunLogRequestFile>(&m_CommandChannelDataLogging);
    RegisterCommandForRouting<NetCommands::CmdDayRunLogReply>(&m_CommandChannelGui);
    RegisterCommandForRouting<NetCommands::CmdExecutionStateChanged>(&m_CommandChannelGui);
    RegisterCommandForRouting<NetCommands::CmdDayRunLogReplyFile>(&m_CommandChannelGui);

    RegisterCommandForRouting<MsgClasses::CmdBottleCheckReply>(&m_CommandChannelGui);
    // -> device

    // so far the only registration with real function:
    RegisterCommandForProcessing<Global::CmdDateAndTime, HimalayaMasterThreadController>(&HimalayaMasterThreadController::SetDateTime, this);

    RegisterCommandForProcessing<NetCommands::CmdExportDayRunLogReply, HimalayaMasterThreadController>
            (&HimalayaMasterThreadController::ExportDayRunLogHandler, this);




    //Import-Export commands
    RegisterCommandForProcessing<MsgClasses::CmdDataExport, HimalayaMasterThreadController>
            (&HimalayaMasterThreadController::ImportExportDataFileHandler, this);
    RegisterCommandForProcessing<MsgClasses::CmdDataImport, HimalayaMasterThreadController>
            (&HimalayaMasterThreadController::ImportExportDataFileHandler, this);
    RegisterCommandForProcessing<MsgClasses::CmdDataImportFiles, HimalayaMasterThreadController>
                (&HimalayaMasterThreadController::ImportFilesHandler, this);

    //User level functions
    RegisterCommandForProcessing<NetCommands::CmdChangeAdminPassword, HimalayaMasterThreadController>
            (&HimalayaMasterThreadController::ChangeAdminPasswordHandler, this);
    RegisterCommandForProcessing<NetCommands::CmdChangeUserLevel, HimalayaMasterThreadController>
            (&HimalayaMasterThreadController::ChangeUserLevelHandler, this);

    //GUI - Sent by Scheduler
    //RegisterCommandForRouting<NetCommands::CmdEventReport>(&m_CommandChannelGui);


    //Update and keep the current system state
   RegisterCommandForProcessing<NetCommands::CmdSystemState, HimalayaMasterThreadController>
            (&HimalayaMasterThreadController::OnCmdSysState, this);

   RegisterCommandForProcessing<MsgClasses::CmdResetOperationHours, HimalayaMasterThreadController>
           (&HimalayaMasterThreadController::ResetOperationHoursHandler, this);//for remover maintainance reminder warning

   RegisterCommandForProcessing<MsgClasses::CmdQuitAppShutdown, HimalayaMasterThreadController>
           (&HimalayaMasterThreadController::PrepareShutdownHandler, this);
}

/****************************************************************************/
void HimalayaMasterThreadController::StartStatemachine() {
    // start own state machine and start to work.
    /// \todo implement

}


void HimalayaMasterThreadController::InitializeGUI() {
    HimalayaGui::HimalayaGuiController *p_GuiController = new HimalayaGui::HimalayaGuiController(THREAD_ID_GUI);
    // Send XML files to GUI when its connected to Main
    CONNECTSIGNALSLOT(p_GuiController, SendConfigurationFiles(), this, SendXML());
    RegisterCommandForProcessing<NetCommands::CmdGuiInit, HimalayaMasterThreadController>
            (&HimalayaMasterThreadController::OnCmdGuiInitHandler, this);
    AddAndConnectController(p_GuiController, &m_CommandChannelGui, static_cast<int>(HIMALAYA_GUI_THREAD));
    StartSpecificThreadController((int)HIMALAYA_GUI_THREAD);
}

/****************************************************************************/
void HimalayaMasterThreadController::OnAckOKNOK(Global::tRefType Ref, const Global::AckOKNOK &Ack) {
    if ((Global::RefManager<Global::tRefType>::INVALID != m_ExpectedShutDownRef) && (m_ExpectedShutDownRef == Ref) && Ack.GetStatus()) {
        Global::EventObject().Instance().RaiseEvent(Global::EVENT_SHUTDOWN_ON_CHECK_FLAG1, true);
        Shutdown();
    }
}


/****************************************************************************/
void HimalayaMasterThreadController:: OnCmdGuiInitHandler(Global::tRefType Ref, const NetCommands::CmdGuiInit &Cmd, Threads::CommandChannel &AckCommandChannel)  {
    Q_UNUSED(Ref);
    Q_UNUSED(Cmd);
    //Send Ack to GUI
    SendAcknowledgeOK(Ref, AckCommandChannel);
    //create controllers only if they are not created
    if (!m_ControllerCreationFlag) {
    //Now create other thread controllers and start them
    try {
        // first of all create controllers and threads
        CreateControllersAndThreads();

        for(Threads::tControllerMap::iterator it = m_ControllerMap.begin(); it != m_ControllerMap.end(); ++it) {
            //Himalaya Gui is already started! so dont start again
            if (it.key() != (int)HIMALAYA_GUI_THREAD) {
                StartSpecificThreadController(it.key());
            }
        }
        // start own statemachine
        StartStatemachine();

        Global::EventObject::Instance().RaiseEvent(EVENT_SYSTEM_INIT_COMPLETE);
        EventHandler::StateHandler::Instance().setIdleState();
        qDebug()<<"SYSTEM INIT COMPLETE \n\n\n\n";

        //only for debug crash issue
        QString StartMonitor = Global::SystemPaths::Instance().GetScriptsPath() + "/start_monitor.sh";
        QString EboxMonitor = Global::SystemPaths::Instance().GetScriptsPath() + "/ebox_monitor.sh";
        if(QFile::exists(StartMonitor) && QFile::exists(EboxMonitor))
        {
            EboxMonitor = QString("%1 HIM &").arg(StartMonitor);
            system(EboxMonitor.toStdString().c_str());
        }

        //SendCommand(Global::CommandShPtr_t(new HimalayaErrorHandler::CmdRaiseAlarm(true)), m_CommandChannelSchedulerMain);
    } catch(const Global::Exception &E) {
        // destroy controllers and threads
        // send error message
        Global::EventObject::Instance().RaiseEvent(E);
        // and request exit
        Global::EventObject().Instance().RaiseEvent(Global::EVENT_SHUTDOWN_ON_CHECK_FLAG4, true);
        Shutdown();
    } catch(const std::bad_alloc &) {
        // destroy controllers and threads
        // send error message
            Global::EventObject::Instance().RaiseEvent(Global::EVENT_GLOBAL_ERROR_MEMORY_ALLOCATION, FILE_LINE_LIST);
        // and request exit
            Global::EventObject().Instance().RaiseEvent(Global::EVENT_SHUTDOWN_ON_CHECK_FLAG8, true);
        Shutdown();
    } catch(...) {
        // destroy controllers and threads
        // and request exit
            Global::EventObject().Instance().RaiseEvent(Global::EVENT_SHUTDOWN_ON_CHECK_FLAG9, true);
        Shutdown();
    }
        m_ControllerCreationFlag = true;
    }
}

void HimalayaMasterThreadController::ImportFilesHandler(Global::tRefType Ref, const MsgClasses::CmdDataImportFiles &Cmd,
                                                        Threads::CommandChannel &AckCommandChannel) {
    Q_UNUSED(AckCommandChannel);
    m_ImportExportCommandRef = Ref;
    emit ImportSelectedFiles(Cmd.GetCommandData());
}
/****************************************************************************/
void HimalayaMasterThreadController::SendXML() {
    Q_ASSERT(mp_DataManager);

    if (!mp_DataManager)
        return;

    (void)SendCommand(Global::CommandShPtr_t(new NetCommands::CmdGuiInit(6000, false)), m_CommandChannelGui);

    QByteArray *p_ByteArray = new QByteArray();
    p_ByteArray->clear();
    QDataStream XmlStream(p_ByteArray, QIODevice::ReadWrite);
    XmlStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));

    //send station.xml before reagents.xml. himalaya gui will set signal slot to update station by using reagent information
    // after parse station.xml.
    p_ByteArray->clear();
    (void)XmlStream.device()->reset();
    DataManager::CDashboardDataStationList *p_StationList = mp_DataManager->GetStationList();
    if (p_StationList) {
        XmlStream << *p_StationList;
        (void)SendCommand(Global::CommandShPtr_t(new NetCommands::CmdConfigurationFile(10000, NetCommands::STATION, XmlStream)), m_CommandChannelGui);
    }

    // send reagent group list in xml ---------------------------------
    p_ByteArray->clear();
    (void)XmlStream.device()->reset();
    DataManager::CDataReagentGroupList *p_ReagentGroupList = mp_DataManager->GetReagentGroupList() ;
    if (p_ReagentGroupList) {
        XmlStream << *p_ReagentGroupList;
        (void)SendCommand(Global::CommandShPtr_t(new NetCommands::CmdConfigurationFile(10000, NetCommands::REAGENTGROUP , XmlStream)), m_CommandChannelGui);
    }

    p_ByteArray->clear();
    (void)XmlStream.device()->reset();
    DataManager::CDataReagentList *p_ReagentList = mp_DataManager->GetReagentList() ;
    if (p_ReagentList) {
        XmlStream << *p_ReagentList;
        (void)SendCommand(Global::CommandShPtr_t(new NetCommands::CmdConfigurationFile(10000, NetCommands::REAGENT, XmlStream)), m_CommandChannelGui);
    }

    p_ByteArray->clear();
    (void)XmlStream.device()->reset();

    DataManager::CReagentGroupColorList *p_ReagentGroupColorList = mp_DataManager->GetReagentGroupColorList() ;
    if (p_ReagentGroupColorList) {
        XmlStream << *p_ReagentGroupColorList;
        (void)SendCommand(Global::CommandShPtr_t(new NetCommands::CmdConfigurationFile(10000, NetCommands::REAGENTGROUPCOLOR , XmlStream)), m_CommandChannelGui);
    }

    p_ByteArray->clear();
    (void)XmlStream.device()->reset();
    DataManager::CUserSettingsInterface *p_SettingsInterface = mp_DataManager->GetUserSettingsInterface();
    if (p_SettingsInterface) {
        XmlStream << *p_SettingsInterface;
        (void)SendCommand(Global::CommandShPtr_t(new NetCommands::CmdConfigurationFile(10000, NetCommands::USER_SETTING, XmlStream)), m_CommandChannelGui);
    }

    p_ByteArray->clear();
    (void)XmlStream.device()->reset();

    DataManager::CDataProgramList *p_ProgramList =  mp_DataManager->GetProgramList();

       if (p_ProgramList) {
           XmlStream << *p_ProgramList;
           (void)SendCommand(Global::CommandShPtr_t(new NetCommands::CmdConfigurationFile(10000, NetCommands::PROGRAM , XmlStream)), m_CommandChannelGui);
       }

    if (p_SettingsInterface) {
        if (p_SettingsInterface->GetUserSettings(false) != NULL) {
        // get the current language
        QLocale::Language CurrentLanguage =
                    p_SettingsInterface->GetUserSettings(false)->GetLanguage();

        // store the langauge name de_DE
        QString LanguageName(QLocale(CurrentLanguage).name());
        // remove the DE
        LanguageName.truncate(LanguageName.lastIndexOf('_'));
        qDebug() << "Language Name ############################" << LanguageName << "#########################";

        QString LanguageFileName = "Himalaya_" + LanguageName + ".qm";
        (void) SendLanguageFileToGUI(LanguageFileName);
        }
    }

    p_ByteArray->clear();
    (void)XmlStream.device()->reset();
    //Update Supported Languages
    (void)UpdateSupportedGUILanguages();

    DataManager::CDeviceConfigurationInterface *p_DeviceConfigurationInterface = mp_DataManager->GetDeviceConfigurationInterface();
    if (p_DeviceConfigurationInterface ) {
        XmlStream << *p_DeviceConfigurationInterface;
        (void)SendCommand(Global::CommandShPtr_t(new NetCommands::CmdConfigurationFile(10000, NetCommands::DEVICE_CONFIGURATION, XmlStream)), m_CommandChannelGui);
    }

    QByteArray EventData;
    QDataStream EventDataStream(&EventData, QIODevice::ReadWrite);
    EventDataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));
    Global::tTranslations TempTranslations;
    TempTranslations = Global::UITranslator::TranslatorInstance().GetTranslations();
    EventDataStream << TempTranslations;
    (void)EventDataStream.device()->reset();
    (void)SendCommand(Global::CommandShPtr_t(new NetCommands::CmdEventStrings(10000, EventDataStream)), m_CommandChannelGui);

    delete p_ByteArray;
}


bool HimalayaMasterThreadController::IsCommandAllowed(const Global::CommandShPtr_t &Cmd) {

    return EventHandler::StateHandler::Instance().isAllowed(Cmd);
}


/****************************************************************************/
void HimalayaMasterThreadController::StartExportProcess(QString FileName) {
    // create and connect gui controller
    m_ExportTargetFileName = FileName;
    Export::ExportController *p_ExportController = new Export::ExportController(Threads::THREAD_ID_EXPORT);
    // connect the process exit slot

    CONNECTSIGNALSLOT(p_ExportController, ProcessExited(const QString &, int), this, ExportProcessExited(const QString &, int));

    // add and connect the controller
    AddAndConnectController(p_ExportController, &m_CommandChannelExport, Threads::THREAD_ID_EXPORT);
    // start the controller
    StartSpecificThreadController(Threads::THREAD_ID_EXPORT);
}

void HimalayaMasterThreadController::OnInitStateCompleted()
{

}

void HimalayaMasterThreadController::SendStateChange(QString state) {
    NetCommands::ProcessStateType stateType = NetCommands::InitState;
    if (state == "BusyState")
    {
        stateType = NetCommands::BusyState;
    } else if (state == "IdleState")
    {
       stateType = NetCommands::IdleState;
    } else if (state == "ErrorState")
    {
        stateType = NetCommands::ErrorState;
    }
    (void)SendCommand(Global::CommandShPtr_t(new NetCommands::CmdProcessState(5000,
                                                     stateType)), m_CommandChannelGui);
}

/****************************************************************************/
void HimalayaMasterThreadController::ExportProcessExited(const QString &Name, int ExitCode) {
    Q_UNUSED(Name)

    // first clear the process
    StopSpecificThreadController(Threads::THREAD_ID_EXPORT);
    // second clear the thread

    StopSpecificThreadController(Threads::THREAD_ID_IMPORTEXPORT);
    if (ExitCode == Global::EXIT_CODE_EXPORT_SUCCESS) {
        // raise the event code
        if (m_RemoteCareExportRequestInitiated) {
            Global::EventObject::Instance().RaiseEvent(Global::EVENT_GLOBAL_STRING_ID_DEBUG_MESSAGE, Global::FmtArgs() << "Send signal to RemoteCareController");
            emit RemoteCareExportFinished("");
        } else {
            Global::EventObject::Instance().RaiseEvent(Global::EVENT_EXPORT_SUCCESS);
            // send acknowledgement to GUI
            /*lint -e613 */
            SendAcknowledgeOK(m_ImportExportCommandRef, *mp_ImportExportAckChannel);
        }
    }
    else {
        quint32 EventCode = Global::EVENT_EXPORT_FAILED;
        // map the exit codes with the event code
        switch(ExitCode) {
            case Global::EXIT_CODE_EXPORT_FAILED:
                EventCode = Global::EVENT_EXPORT_FAILED;
                break;
            case Global::EXIT_CODE_EXPORT_UNABLE_TO_READ_FILE_TEMP_EXPORTCONFIGURATION:
                EventCode = Global::EVENT_EXPORT_UNABLE_TO_READ_FILE_TEMP_EXPORTCONFIGURATION;
                break;
            case Global::EXIT_CODE_EXPORT_INIT_CONTAINER_FAILED:
                EventCode = Global::EVENT_EXPORT_INIT_CONTAINER_FAILED;
                break;
            case Global::EXIT_CODE_EXPORT_VERIFICATION_CONTAINER_FAILED:
                EventCode = Global::EVENT_EXPORT_VERIFICATION_CONTAINER_FAILED;
                break;
            case Global::EXIT_CODE_EXPORT_UNABLE_ARCHIVE_FILES:
                EventCode = Global::EVENT_EXPORT_UNABLE_TO_ARCHIVE_FILES ;
                break;
            case Global::EXIT_CODE_EXPORT_SOURCE_DIRECTORY_NOT_EXISTS:
                EventCode = Global::EVENT_EXPORT_SOURCE_DIRECTORY_NOT_EXISTS;
                break;
            case Global::EXIT_CODE_EXPORT_TARGET_DIRECTORY_NOT_EXISTS:
                EventCode = Global::EVENT_EXPORT_TARGET_DIRECTORY_NOT_EXISTS;
                break;
            case Global::EXIT_CODE_EXPORT_LOG_DIRECTORY_NOT_EXISTS:
                EventCode = Global::EVENT_EXPORT_LOG_DIRECTORY_NOT_EXISTS;
                break;
            case Global::EXIT_CODE_EXPORT_TARGET_FILE_FORMAT_IS_WRONG:
                EventCode = Global::EVENT_EXPORT_TARGET_FILE_FORMAT_IS_WRONG;
                break;
            case Global::EXIT_CODE_EXPORT_INVALID_EXPORT:
                EventCode = Global::EVENT_EXPORT_INVALID_EXPORT;
                break;
            case Global::EXIT_CODE_EXPORT_CRYTOSERVICE_RUNNING:
                EventCode = Global::EVENT_EXPORT_CRYTOSERVICE_RUNNING;
                break;
            case Global::EXIT_CODE_EXPORT_CANNOT_OPEN_FILE_FOR_READ:
                EventCode = Global::EVENT_EXPORT_CANNOT_OPEN_FILE_FOR_READ;
                break;
            case Global::EXIT_CODE_EXPORT_CANNOT_OPEN_FILE_FOR_WRITE:
                EventCode = Global::EVENT_EXPORT_CANNOT_OPEN_FILE_FOR_WRITE;
                break;
            case Global::EXIT_CODE_EXPORT_ERROR_TO_READ:
                EventCode = Global::EVENT_EXPORT_ERROR_TO_READ;
                break;
            case Global::EXIT_CODE_EXPORT_ERROR_TO_WRITE:
                EventCode = Global::EVENT_EXPORT_ERROR_TO_WRITE;
                break;            
            case Global::EXIT_CODE_EXPORT_INDEX_IS_MATCHING:
                EventCode = Global::EVENT_EXPORT_INDEX_IS_MATCHING;
                break;
            case Global::EXIT_CODE_EXPORT_KEY_SIZE_LESS:
                EventCode = Global::EVENT_EXPORT_KEY_SIZE_LESS;
                break;
            case Global::EXIT_CODE_EXPORT_KEYDATA_SIZE_IS_NOT_MATCHING:
                EventCode = Global::EVENT_EXPORT_KEYDATA_SIZE_IS_NOT_MATCHING;
                break;
            case Global::EXIT_CODE_EXPORT_HMAC_NOT_INITIALIZED:
                EventCode = Global::EVENT_EXPORT_HMAC_NOT_INITIALIZED;
                break;
            case Global::EXIT_CODE_EXPORT_AES_NOT_INITIALIZED:
                EventCode = Global::EVENT_EXPORT_AES_NOT_INITIALIZED;
                break;
            case Global::EXIT_CODE_EXPORT_INTEGER_SIZE_IS_MORE:
                EventCode = Global::EVENT_EXPORT_INTEGER_SIZE_IS_MORE;
                break;
            case Global::EXIT_CODE_EXPORT_MSB_BIT_IS_NOT_SET:
                EventCode = Global::EVENT_EXPORT_MSB_BIT_IS_NOT_SET;
                break;
            case Global::EXIT_CODE_EXPORT_INVALID_FILE_MODE:
                EventCode = Global::EVENT_EXPORT_INVALID_FILE_MODE;
                break;
            case Global::EXIT_CODE_EXPORT_HMAC_COMPUTATION_STARTED:
                EventCode = Global::EVENT_EXPORT_HMAC_COMPUTATION_STARTED;
                break;
            case Global::EXIT_CODE_EXPORT_ZIP_ERROR:
                EventCode = Global::EVENT_EXPORT_ZIP_ERROR;
                break;
            case Global::EXIT_CODE_EXPORT_ZIP_COMMAND_NOT_FOUND:
                EventCode = Global::EVENT_EXPORT_ZIP_COMMAND_NOT_FOUND;
                break;
            case Global::EXIT_CODE_EXPORT_ZIP_IS_TAKING_LONGTIME:
                EventCode = Global::EVENT_EXPORT_ZIP_IS_TAKING_LONGTIME;
                break;
            case Global::EXIT_CODE_EXPORT_NO_ENOUGH_SPACE_ON_USB:
                EventCode = EVENT_EXPORT_NO_ENOUGH_SPACE_ON_USB_CARD;
                break;
        }
        // this raise event code will be informed to GUI, that Export is failed
        Global::EventObject::Instance().RaiseEvent(EventCode, true);
        // send acknowledgement to GUI
        /*lint -e613 */
        SendAcknowledgeNOK(m_ImportExportCommandRef, *mp_ImportExportAckChannel);

    }

    m_ExportProcessIsFinished = true;
    if (!m_RemoteCareExportRequestInitiated) {
        SetUserActionState(NORMAL_USER_ACTION_STATE);
    }
    m_RemoteCareExportRequestInitiated = false;
    RemoveAndDestroyObjects();
}

/****************************************************************************/
void HimalayaMasterThreadController::RemoveAndDestroyObjects() {
    // this is for safety
    if (m_ExportProcessIsFinished || m_ImportExportThreadIsRunning) {
        // remove and destroy the controller thread
        RemoveSpecificThreadController(Threads::THREAD_ID_EXPORT);
        // remove and destroy the controller thread
        RemoveSpecificThreadController(Threads::THREAD_ID_IMPORTEXPORT);

        // reset the falgs, so nothing is executing
        m_ExportProcessIsFinished = false;
        m_ImportExportThreadIsRunning = false;
    }
}



/****************************************************************************/
void HimalayaMasterThreadController::ImportExportThreadFinished(const bool IsImport, QStringList ImportTypeList,
                                                                quint32 EventCode,
                                                                bool UpdatedCurrentLanguage,
                                                                bool NewLanguageAdded) {
    Q_ASSERT(mp_DataManager);
    if (!mp_DataManager)
        return;

    Q_ASSERT(mp_ImportExportAckChannel);
    if (!mp_ImportExportAckChannel)
        return;

    bool SendAckOK = false;
    if (IsImport && ImportTypeList.count() > 0) {
        // check the type of Impor
        if ((ImportTypeList.contains("User") == 0) || (ImportTypeList.contains("Service") == 0) || (ImportTypeList.contains("Leica") == 0)) {

            // send configuration files to GUI
            /*lint -e534 */
            SendConfigurationFile(mp_DataManager->GetUserSettingsInterface(), NetCommands::USER_SETTING);
            /*lint -e534 */
            SendConfigurationFile(mp_DataManager->GetReagentList(), NetCommands::REAGENT);
            /*lint -e534 */
            SendConfigurationFile(mp_DataManager->GetReagentGroupList(), NetCommands::REAGENTGROUP);
            /*lint -e534 */
            SendConfigurationFile(mp_DataManager->GetReagentGroupColorList(), NetCommands::REAGENTGROUPCOLOR);
            /*lint -e534 */
            SendConfigurationFile(mp_DataManager->GetProgramList(), NetCommands::PROGRAM);
            /*lint -e534 */
            SendConfigurationFile(mp_DataManager->GetStationList(), NetCommands::STATION);
            // inform the event handler
            Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_SUCCESS);
            // send ack OK
            SendAckOK = true;
        }
        else if(ImportTypeList.contains("Language")) {
            // new langauge is added
            if (NewLanguageAdded) {
                if (UpdateSupportedGUILanguages()) {
                    /// \todo add a command for the language list - not implemented in the GUI
                }
            }
            // if current language file updated by the import then this needs a send command to GUI
            if (UpdatedCurrentLanguage) {
                if (mp_DataManager->GetUserSettingsInterface() != NULL) {
                    if (mp_DataManager->GetUserSettingsInterface()->GetUserSettings(false) != NULL) {
                        // get the current language
                        QLocale::Language CurrentLanguage =
                                mp_DataManager->GetUserSettingsInterface()->GetUserSettings(false)->GetLanguage();

                        // store the langauge name de_DE
                        QString LanguageName(QLocale(CurrentLanguage).name());
                        // remove the DE
                        LanguageName.truncate(LanguageName.lastIndexOf('_'));

//                        QString LanguageFileName = "Himalaya_" + LanguageName + ".qm";
//                        if (!SendLanguageFiExportleToGUI(LanguageFileName)) {
//                            /// need to inform the event handler
//                        }
                    }
                }
            }
            // inform the event handler
            Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_SUCCESS_LANGUAGE);
            // send ack OK
            SendAckOK = true;
        }        
    }

    // send ack to the channel
    if (SendAckOK) {
        // send ack is OK
        SendAcknowledgeOK(m_ImportExportCommandRef, *mp_ImportExportAckChannel);
    }
    else {        
        // send ack is NOK
        if(IsImport)
        {
            if(EventCode != 0){
                Global::EventObject::Instance().RaiseEvent(EventCode,true);
            }
            else if (ImportTypeList.count() > 0){
                Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_FAILED,true);
            }
        }
        else
        {
            if(EventCode != 0){
                Global::EventObject::Instance().RaiseEvent(EventCode,true);
            }
            else{
                Global::EventObject::Instance().RaiseEvent(Global::EVENT_EXPORT_FAILED,true);
            }
        }
//        if(IsImport && EventCode != 0){
//            Global::EventObject::Instance().RaiseEvent(EventCode,true);
//        }
//        else if(IsImport && EventCode == 0){
//            Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_FAILED,true);
//        }
//        else if(!IsImport && EventCode != 0){
//            Global::EventObject::Instance().RaiseEvent(EventCode,true);
//        }
//        else{
//            Global::EventObject::Instance().RaiseEvent(Global::EVENT_EXPORT_FAILED,true);
//        }
        SendAcknowledgeNOK(m_ImportExportCommandRef, *mp_ImportExportAckChannel);
    }

    if (!m_RemoteCareExportRequestInitiated) {
        SetUserActionState(NORMAL_USER_ACTION_STATE);
    }

    m_RemoteCareExportRequestInitiated = false;

    // clear the thread
    m_ExportProcessIsFinished = true;
    StopSpecificThreadController(Threads::THREAD_ID_IMPORTEXPORT);
    // enable the timer slot to destroy the objects after one second
    RemoveAndDestroyObjects();
}

/****************************************************************************/
bool HimalayaMasterThreadController::UpdateSupportedGUILanguages() {
    Q_ASSERT(mp_DataManager);
    if (!mp_DataManager)
        return false;

    QDir TheDir(Global::SystemPaths::Instance().GetTranslationsPath());
    QStringList FileNames = TheDir.entryList(QStringList("Himalaya_*.qm"));
    DataManager::CDeviceConfigurationInterface *p_DeviceConfigInterface = mp_DataManager->GetDeviceConfigurationInterface();
    if (p_DeviceConfigInterface) {
        DataManager::CDeviceConfiguration *p_DeviceConfiguration = p_DeviceConfigInterface->GetDeviceConfiguration();
        if (p_DeviceConfiguration) {
            QStringList LanguageList;
            for (int i = 0; i < FileNames.size(); ++i)
            {
                // get locale extracted by filename
                QString Locale;
                Locale = FileNames[i];                  // "Himalaya_de.qm"
                Locale.truncate(Locale.lastIndexOf('.'));   // "Himalaya_de"
                (void)Locale.remove(0, Locale.indexOf('_') + 1);   // "de"
                LanguageList << Global::LanguageToString(QLocale(Locale).language());
            }
            p_DeviceConfiguration->SetLanguageList(LanguageList);
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

/****************************************************************************/
bool HimalayaMasterThreadController::SendLanguageFileToGUI(QString FileName) {

    QString TranslationPath = Global::SystemPaths::Instance().GetTranslationsPath()
            + QDir::separator() + FileName;
    QFile File(TranslationPath);
    // check the existence of the file
    if (File.exists()) {
        // open the file
        if (File.open(QIODevice::ReadWrite)) {
            if (mp_DataManager) {
                const DataManager::CUserSettings *p_Settings = mp_DataManager->GetUserSettings();
                if (p_Settings) {
                    QLocale::Language CurrentLanguage =  p_Settings->GetLanguage();
                    QDataStream LangDataStream(&File);
                    LangDataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));
                    // send the command to GUI
                    (void)SendCommand(Global::CommandShPtr_t(new NetCommands::CmdLanguageFile(10000, LangDataStream,CurrentLanguage)),
                                m_CommandChannelGui);
                }
            }
            // change the UI translator default language also
            // get locale extracted by filename
            QString Locale;
            Locale = FileName;                  // "Himalaya_de.qm"
            Locale.truncate(Locale.lastIndexOf('.'));   // "Himalaya_de"
            Locale.remove(0, Locale.indexOf('_') + 1);   // "de"
            Global::UITranslator::TranslatorInstance().SetDefaultLanguage(QLocale(Locale).language());

        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
    return true;
}

/****************************************************************************/
void HimalayaMasterThreadController::ChangeUserLevelHandler(Global::tRefType Ref,
                                                            const NetCommands::CmdChangeUserLevel &Cmd,
                                                            Threads::CommandChannel &AckCommandChannel) {
    Q_ASSERT(mp_DataManager);
    if (!mp_DataManager)
        return;

    // send ack to GUI
    SendAcknowledgeOK(Ref, AckCommandChannel);
    QString DeviceName;
    quint32 LogEvent = 0;
    switch(Cmd.GetUserLevel()) {
        case Global::ADMIN:
            if (m_PasswordManager.CheckPassword("Administrator", Cmd.GetPassword())) {
                m_AuthenticatedLevel = Global::ADMIN;
                LogEvent = EVENT_DM_CHANGE_TO_SUPERVISOR;
            }
            break;
        case Global::SERVICE:            

            if (mp_DataManager->GetDeviceConfigurationInterface() != NULL) {

                DataManager::CDeviceConfiguration *p_DeviceConfiguration = mp_DataManager->GetDeviceConfigurationInterface()
                        ->GetDeviceConfiguration(false);

                if (p_DeviceConfiguration != NULL) {
                    DeviceName = p_DeviceConfiguration->GetValue("DeviceName");
                }
            }

            if (m_PasswordManager.ServiceAuthentication(Cmd.GetPassword(), DeviceName)) {
                m_AuthenticatedLevel = Global::SERVICE;
                LogEvent = EVENT_DM_CHANGE_TO_SERVICE;
            }
            break;
        case Global::OPERATOR:
            // there is no password for the operator
            m_AuthenticatedLevel = Global::OPERATOR;
            m_PasswordManager.SetFallbackPasswordFlag(false);
            LogEvent = EVENT_DM_CHANGE_TO_STANDARD_USER;
            break;
        default:
            break;
    }

    // check whether user level is operator, if it is operator then authentication failed
    if ((m_AuthenticatedLevel == Global::SERVICE || m_AuthenticatedLevel == Global::OPERATOR) && Cmd.GetUserLevel() == Global::ADMIN) {
        Global::EventObject::Instance().RaiseEvent(EVENT_PASSWORD_INCORRECT);
    }

    // send the authenticated command to GUI
    (void)SendCommand(Global::CommandShPtr_t(new NetCommands::CmdChangeUserLevelReply(5000, m_AuthenticatedLevel)), m_CommandChannelGui);
    // check the whether fallback password is validated for the successful login
    if (m_PasswordManager.GetFallbackPasswordFlag()) {
        // User entered the fallback password so ask him to change the password
        (void)SendCommand(Global::CommandShPtr_t(new NetCommands::CmdChangeAdminPasswordReply(5000, "New")), m_CommandChannelGui);
        m_PasswordManager.SetFallbackPasswordFlag(false);
    }
    if(LogEvent != 0){
        Global::EventObject::Instance().RaiseEvent(LogEvent);
    }
}

void HimalayaMasterThreadController::ResetOperationHoursHandler(Global::tRefType Ref, const MsgClasses::CmdResetOperationHours &Cmd,
                                Threads::CommandChannel &AckCommandChannel)
{
    SendAcknowledgeOK(Ref, AckCommandChannel);
    DataManager::ResetOperationHoursType_t resetOperationHoursType = Cmd.ResetOperationHoursType();
    if (resetOperationHoursType == DataManager::RESETOPERATIONHOURS_WHOLEMACHINEOPERATION)
        Global::EventObject::Instance().RaiseEvent(EVENT_SERVICE_OPERATIONTIME_RESET, true);
    else
        Global::EventObject::Instance().RaiseEvent(EVENT_SERVICE_ACTIVECARBONTIME_RESET, true);
}

/****************************************************************************/
void HimalayaMasterThreadController::ChangeAdminPasswordHandler(Global::tRefType Ref,
                                                                const NetCommands::CmdChangeAdminPassword &Cmd,
                                                                Threads::CommandChannel &AckCommandChannel) {

    QString CommandType("Failed");
    SendAcknowledgeOK(Ref, AckCommandChannel);
    if (Cmd.GetCommandType().compare("Old") == 0) {
        if (m_PasswordManager.CheckPassword("Administrator", Cmd.GetPassword())) {
            CommandType = "New";
        }
        else {
            Global::EventObject::Instance().RaiseEvent(EVENT_PASSWORD_INCORRECT);
        }
    }
    else if (Cmd.GetCommandType().compare("New") == 0) {
        // save the password value
        m_PasswordManager.SetNewPassword(Cmd.GetPassword());
        CommandType = "Confirm";
    }
    else if(Cmd.GetCommandType().compare("Confirm") == 0) {
        // compare the old pass word with new password
        if (m_PasswordManager.GetNewPassword().compare(Cmd.GetPassword()) == 0) {
            // set the new password
            m_PasswordManager.SetPassword("Administrator", m_PasswordManager.GetNewPassword());
            // success
            CommandType = "Success";
            Global::EventObject::Instance().RaiseEvent(EVENT_PASSWORD_CHANGED_SUCCESSFULLY);
            // write the changed password in the xml file
            DataManager::XmlConfigFilePasswords WritePasswordFile(GetSerialNumber());
            WritePasswordFile.WritePasswords(Global::SystemPaths::Instance().GetSettingsPath() + "/Password.xml",
                                        m_PasswordManager, GetSerialNumber());
        }
        else {
            Global::EventObject::Instance().RaiseEvent(EVENT_PASSWORD_NOT_MATCHING);
            CommandType = "New";
        }
    }

    (void)SendCommand(Global::CommandShPtr_t(new NetCommands::CmdChangeAdminPasswordReply(5000, CommandType)), m_CommandChannelGui);

}


/****************************************************************************/
void HimalayaMasterThreadController::EventCmdSystemAction(Global::tRefType Ref, const NetCommands::CmdSystemAction &Cmd,
                        Threads::CommandChannel &AckCommandChannel)
{
    Q_UNUSED(Ref);
    Q_UNUSED(AckCommandChannel);
    qDebug() << "In the event handling for CMD system Action for event " << Cmd.GetEventID();
    if(Cmd.GetAction() == Global::ACNTYPE_SHUTDOWN || Cmd.GetActionString().compare("RS_Shutdown") == 0){
        qDebug()<<"Shutting Down system";
        Global::EventObject().Instance().RaiseEvent(Global::EVENT_SHUTDOWN_ON_CHECK_FLAG7, true);
        Shutdown();
    }
}


/************************************************************************************************************************************/
void HimalayaMasterThreadController::ShutdownHandler(Global::tRefType Ref, const Global::CmdShutDown &Cmd, Threads::CommandChannel &AckCommandChannel)
{
    SendAcknowledgeOK(Ref, AckCommandChannel);
    Global::EventObject().Instance().RaiseEvent(Global::EVENT_SHUTDOWN_ON_CHECK_FLAG14, true);
    InitiateShutdown(Cmd.GetReboot());
}

void HimalayaMasterThreadController::PrepareShutdownHandler(Global::tRefType Ref,
                                                            const MsgClasses::CmdQuitAppShutdown &Cmd,
                                                            Threads::CommandChannel &AckCommandChannel)
{
    Q_UNUSED(Cmd)
    SendAcknowledgeOK(Ref, AckCommandChannel);

    m_bQuitApp = (DataManager::QUITAPPSHUTDOWNACTIONTYPE_QUITAPP == Cmd.QuitAppShutdownActionType());
    if (m_bQuitApp){
        m_BootConfigFileContent.insert("Start_Process", "Service");
    }
    else{
        m_BootConfigFileContent.insert("Start_Process", "DisplayPowerOffImage");
    }
    Global::UpdateRebootFile(m_BootConfigFileContent);
    Global::EventObject().Instance().RaiseEvent(Global::EVENT_SHUTDOWN_ON_CHECK_FLAG11, true);
    InitiateShutdown();
}

void HimalayaMasterThreadController::Reboot()
{
    m_BootConfigFileContent.insert("Main_Rebooted", "Yes");
    QString RebootCountStr = m_BootConfigFileContent.value("Reboot_Count");
    qint32  RebootCount = RebootCountStr.toInt();
    RebootCount++;
    RebootCountStr = QString::number(RebootCount);
    m_BootConfigFileContent.insert("Reboot_Count", RebootCountStr);
    Global::UpdateRebootFile(m_BootConfigFileContent);
    Global::EventObject().Instance().RaiseEvent(Global::EVENT_SHUTDOWN_ON_CHECK_FLAG10, true);
    InitiateShutdown(true);
}
/****************************************************************************/
void HimalayaMasterThreadController::ExternalProcessConnectionHandler(Global::tRefType Ref, const NetCommands::CmdExternalProcessState &Cmd, Threads::CommandChannel &AckCommandChannel)
{
    SendAcknowledgeOK(Ref, AckCommandChannel);

    if (!Cmd.m_Connected)
    {
        if (Cmd.m_ProcessType == NetCommands::HIMALAYA_GUI_PROCESS)
        {
            Global::EventObject::Instance().RaiseEvent(EVENT_PROCESS_HIMALAYA_GUI_STOPPED);
            Global::EventObject::Instance().RaiseEvent(EVENT_PROCESS_HIMALAYA_GUI_CONNECTED, Global::FmtArgs(), false);
            Global::EventObject::Instance().RaiseEvent(EVENT_GUI_AVAILABLE, Global::FmtArgs(), false);
            // emit a signal so that if logging is disabled then Main can inform to GUI
//            emit CheckLoggingEnabled();
        }
    }
    else {
        if (Cmd.m_ProcessType == NetCommands::HIMALAYA_GUI_PROCESS)
        {
            Global::EventObject::Instance().RaiseEvent(EVENT_PROCESS_HIMALAYA_GUI_STOPPED, Global::FmtArgs(), false);
            Global::EventObject::Instance().RaiseEvent(EVENT_PROCESS_HIMALAYA_GUI_CONNECTED);
            Global::EventObject::Instance().RaiseEvent(EVENT_GUI_AVAILABLE, Global::FmtArgs(), true);
            // emit a signal so that if logging is disabled then Main can inform to GUI
  //          emit CheckLoggingEnabled();
        }
    }

}


/****************************************************************************/
void HimalayaMasterThreadController::ExportDayRunLogHandler(Global::tRefType Ref, const NetCommands::CmdExportDayRunLogReply &Cmd,
                                                            Threads::CommandChannel &AckCommandChannel) {
    Q_UNUSED(Ref);
    Q_UNUSED(Cmd);
    Q_UNUSED(AckCommandChannel);

    emit DayRunLogDirectoryName("DailyRun");
}

/****************************************************************************/
void HimalayaMasterThreadController::RequestDayRunLogFileNames(QString FolderPath) {
    /*lint -e641*/
    (void)SendCommand(Global::CommandShPtr_t(new NetCommands::CmdExportDayRunLogRequest(m_AuthenticatedLevel,FolderPath)),
                                            m_CommandChannelDataLogging);

}

/****************************************************************************/
void HimalayaMasterThreadController::OnCmdSysState(Global::tRefType, const NetCommands::CmdSystemState &Cmd,
                      Threads::CommandChannel&)
{
    Q_UNUSED(Cmd)
    //EventHandler::CrisisEventHandler::Instance().currentSysState(Cmd.m_StateId);
}

void HimalayaMasterThreadController::SetAlarmHandlerTimeout(quint16 timeout)
{
    //mp_alarmHandler->setTimeout(timeout);
    Q_UNUSED(timeout);
}

void HimalayaMasterThreadController::OnFireAlarmLocalRemote(bool isLocalAlarm)
{ 
    //   (void)SendCommand(Global::CommandShPtr_t(new HimalayaErrorHandler::CmdRaiseAlarm(isLocalAlarm)), m_CommandChannelSchedulerMain);
    Q_UNUSED(isLocalAlarm);
}

/************************************************************************************************************************************/
void HimalayaMasterThreadController::SetUserActionState(const CurrentUserActionState_t UserActionState) {
    m_CurrentUserActionState = UserActionState;
//    quint32 UserActionEventID = 0;
//    bool InformGPIOManager = false;
//    bool UserActionCompleted = false;
//    DeviceCommandProcessor::CmdDrawerSetBlockingState *p_CmdBlkDrawer = new DeviceCommandProcessor::CmdDrawerSetBlockingState(DeviceControl::DEVICE_INSTANCE_ID_LOADER);
//    switch (m_CurrentUserActionState) {
//    case Colorado::NORMAL_USER_ACTION_STATE:
//        if (m_FreeDrawer) {
//            p_CmdBlkDrawer->m_BlockingState = DeviceControl::BLOCKING_STATE_FREE;
//            SendCommand(Global::CommandShPtr_t(p_CmdBlkDrawer), m_CommandChannelDeviceCommandProcessor);
//            InformGPIOManager = true;
//        }
//        UserActionCompleted = true;
//        m_FreeDrawer = false;
//        break;
//    case Colorado::EXPORT_STATE:
//        p_CmdBlkDrawer->m_BlockingState = DeviceControl::BLOCKING_STATE_BLOCKED;
//        SendCommand(Global::CommandShPtr_t(p_CmdBlkDrawer), m_CommandChannelDeviceCommandProcessor);
//        InformGPIOManager = true;
//        m_FreeDrawer = true;
//        UserActionCompleted = false;
//        UserActionEventID = EVENT_STRING_EXPORT;
//        break;
//    case Colorado::BLG_STATE:
//        p_CmdBlkDrawer->m_BlockingState = DeviceControl::BLOCKING_STATE_BLOCKED;
//        SendCommand(Global::CommandShPtr_t(p_CmdBlkDrawer), m_CommandChannelDeviceCommandProcessor);
//        InformGPIOManager = true;
//        m_FreeDrawer = true;
//        UserActionCompleted = false;
//        UserActionEventID = EVENT_STRING_BATHLAYOUT_GENERATION;
//        break;
//    case Colorado::IMPORT_STATE:
//        p_CmdBlkDrawer->m_BlockingState = DeviceControl::BLOCKING_STATE_BLOCKED;
//        SendCommand(Global::CommandShPtr_t(p_CmdBlkDrawer), m_CommandChannelDeviceCommandProcessor);
//        InformGPIOManager = true;
//        m_FreeDrawer = true;
//        UserActionCompleted = false;
//        UserActionEventID = EVENT_STRING_IMPORT;
//        break;
//    case Colorado::SW_UPDATE_STATE:
//        p_CmdBlkDrawer->m_BlockingState = DeviceControl::BLOCKING_STATE_BLOCKED;
//        SendCommand(Global::CommandShPtr_t(p_CmdBlkDrawer), m_CommandChannelDeviceCommandProcessor);
//        InformGPIOManager = true;
//        m_FreeDrawer = true;
//        UserActionCompleted = false;
//        UserActionEventID = EVENT_STRING_SW_UPDATE;
//        break;
//    case Colorado::UPDATE_LEICA_REAGENT_STATE:
//        p_CmdBlkDrawer->m_BlockingState = DeviceControl::BLOCKING_STATE_BLOCKED;
//        SendCommand(Global::CommandShPtr_t(p_CmdBlkDrawer), m_CommandChannelDeviceCommandProcessor);
//        InformGPIOManager = true;
//        m_FreeDrawer = true;
//        UserActionCompleted = false;
//        UserActionEventID = EVENT_STRING_LEICA_REAGENT_UPDATE;
//        break;
//    case Colorado::RACK_IN_DRAWER_STATE:
//        break;
//    default:
//        break;
//    }
//    if (InformGPIOManager) {
//        SendCommand(Global::CommandShPtr_t(new NetCommands::CmdUserAction(1000, UserActionEventID, UserActionCompleted)), m_CommandChannelGPIOManager);
//    }
}


void HimalayaMasterThreadController::SendFileSelectionToGUI(QStringList FileList)
{
    // send ack is OK
    SendAcknowledgeOK(m_ImportExportCommandRef, *mp_ImportExportAckChannel);
    /*lint -e534 */
    SendCommand(Global::CommandShPtr_t(new MsgClasses::CmdDataImportFiles(3000, FileList)),
                m_CommandChannelGui);

}


/************************************************************************************************************************************/
void HimalayaMasterThreadController::RemoteCareExportData(const quint8 &NoOfLogFiles) {
    m_RemoteCareExportRequest = true;
    // remote care only requests for service export
    // add all data in bytearray
    QByteArray ByteArray;
    ByteArray.append("Service_" + QString::number(NoOfLogFiles));
    // create export command
    MsgClasses::CmdDataExport *p_Command = new MsgClasses::CmdDataExport(20000, ByteArray);
    // create dummy channel because "ImportExportDataFileHandler" function uses these arguments
    Threads::CommandChannel DummyChannel(this, "Dummy", Global::EVENTSOURCE_NONE);
    ImportExportDataFileHandler(0, *p_Command, DummyChannel);
    m_RemoteCareExportRequest = false;
    // delete the created command
    delete p_Command;
}

void HimalayaMasterThreadController::SWUpdateRollbackComplete() {

    EventHandler::StateHandler::Instance().setInitStageProgress(3, true);

    NetCommands::CmdExecutionStateChanged *p_Command = new NetCommands::CmdExecutionStateChanged(5000);
    p_Command->m_WaitDialogFlag = false;
    p_Command->m_WaitDialogText = Global::INITIALIZING_TEXT;
    SendCommand(Global::CommandShPtr_t(p_Command), m_CommandChannelGui);
}

void HimalayaMasterThreadController::SWUpdateProgress(bool InProgress) {
    if (InProgress) {
        SetUserActionState(Himalaya::SW_UPDATE_STATE);
    }
    else if (GetCurrentIdleState() == Himalaya::SW_UPDATE_STATE) {
        SetUserActionState(Himalaya::NORMAL_USER_ACTION_STATE);
    }

    emit InformRCSWUpdateStatus(InProgress);
}

/****************************************************************************/
void HimalayaMasterThreadController::ShowWaitDialog(bool Display, Global::WaitDialogText_t WaitDialogText) {
    NetCommands::CmdExecutionStateChanged *p_Command = new NetCommands::CmdExecutionStateChanged(5000);
    p_Command->m_Stop = false;
    p_Command->m_WaitDialogFlag = Display;
    p_Command->m_WaitDialogText = WaitDialogText;
    SendCommand(Global::CommandShPtr_t(p_Command),m_CommandChannelGui);
}

void HimalayaMasterThreadController::SendRCCmdToGuiChannel(const Global::CommandShPtr_t &Cmd)
{
    (void)SendCommand(Cmd, m_CommandChannelGui);
}

void HimalayaMasterThreadController::OnLanguageChanged(const bool LanguangeChanged)
{
    if (!LanguangeChanged || (0 == mp_DataManager))
    {
        return;
    }

    if (mp_DataManager->GetUserSettingsInterface()->GetUserSettings(false) != NULL) {
        // get the current language
        QLocale::Language CurrentLanguage =
                mp_DataManager->GetUserSettingsInterface()->GetUserSettings(false)->GetLanguage();

        // store the langauge name de_DE
        QString LanguageName(QLocale(CurrentLanguage).name());
        // remove the DE
        LanguageName.truncate(LanguageName.lastIndexOf('_'));
        qDebug() << "Language Name ############################" << LanguageName << "#########################";

        QString LanguageFileName = "Himalaya_" + LanguageName + ".qm";
        this->SendLanguageFileToGUI(LanguageFileName);
        DataManager::CDataProgramList* pProgramList = mp_DataManager->GetProgramList();
        if(pProgramList)
        {
            pProgramList->UpdateOnLanguageChanged();
            this->SendConfigurationFile(pProgramList, NetCommands::PROGRAM);
        }

        if(mp_DataManager->GetReagentGroupList())
        {
            mp_DataManager->GetReagentGroupList()->UpdateOnLanguageChanged();
            this->SendConfigurationFile(mp_DataManager->GetReagentGroupList(),NetCommands::REAGENTGROUP);
        }

        if(mp_DataManager->GetReagentList())
        {
            mp_DataManager->GetReagentList()->UpdateOnLanguageChanged();
            this->SendConfigurationFile(mp_DataManager->GetReagentList(),NetCommands::REAGENT);
        }
    }
}

} // end namespace Himalaya
