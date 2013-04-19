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
#include <DeviceCommandProcessor/Include/Commands/CmdDeviceProcessingCleanup.h>

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
#include <HimalayaDataManager/Commands/Include/CmdStationDataContainer.h>
#include <NetCommands/Include/CmdExternalProcessState.h>
#include <DeviceCommandProcessor/Include/Commands/CmdDeviceProcessingInit.h>
#include <DeviceCommandProcessor/Include/Commands/CmdDeviceProcessingCleanup.h>

#include <Global/Include/Commands/CmdEventUpdate.h>
#include <Global/Include/Translator.h>
#include <Global/Include/UITranslator.h>
#include <QLocale>

#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramListVerifier.h"
#include "HimalayaDataContainer/Containers/Reagents/Include/DataReagentListVerifier.h"
#include "HimalayaDataContainer/Containers/ReagentStations/Include/DataStationListVerifier.h"
#include "DataManager/Containers/UserSettings/Include/UserSettingsVerifier.h"
#include "DataManager/Containers/DeviceConfiguration/Include/DeviceConfigurationInterface.h"

#include "HimalayaDataManager/Include/DataManager.h"

#include <Global/Include/EventObject.h>
#include <EventHandler/Include/StateHandler.h>

#include <HimalayaEventCodes.h>
#include <HimalayaProcessExitCodes.h>
#include <EventHandler/Include/EventHandlerEventCodes.h>
#include <NetCommands/Include/CmdCriticalActionCheck.h>
#include <NetCommands/Include/CmdCriticalActionStatus.h>
#include <EventHandler/Include/CrisisEventHandler.h>
#include <HimalayaErrorHandler/Include/HimalayaAlarmHandler.h>
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAction.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdRetortLock.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdRetortLockStatus.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdRetortStatus.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdStationParaffinBathStatus.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdCurrentProgramStepInfor.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramEndDateTime.h"
#include "Scheduler/Commands/Include/CmdSystemState.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramStartReady.h"



namespace Himalaya {
const quint32 ERROR_CODE_HIMALAYA_CONSTRUCTION_FAILED = 1;
const Global::gSubComponentType SUBCOMPONENT_ERRORHANDLER   = 0x0001;   ///< Subcomponent id for error handler.

/****************************************************************************/
HimalayaMasterThreadController::HimalayaMasterThreadController() try:
    MasterThreadController(HEARTBEAT_SOURCE_MASTERTHREAD, HEARTBEAT_SOURCE_DATALOGGING, HEARTBEAT_SOURCE_EVENTHANDLER, "HimalayaShutdown"),
    /// \todo hash of master password (now master password is "Himalaya")
    m_PasswordManager("6C7F722B0C5AC1BD70BE4ECC6FC0653E"),
    m_CommandChannelGui(this, "Gui", Global::EVENTSOURCE_NONE),
    m_CommandChannelSchedulerMain(this, "SchedulerMain", Global::EVENTSOURCE_SCHEDULER_MAIN),
    m_CommandChannelEventHandler(this, "EventHandler", Global::EVENTSOURCE_EVENTHANDLER),
    m_CommandChannelExport(this, "Export", Global::EVENTSOURCE_EXPORT),
    m_CommandChannelImportExport(this, "ImportExport", Global::EVENTSOURCE_IMPORTEXPORT),
    mp_DataManager(NULL),
    mp_ImportExportAckChannel(NULL),
    m_ExportProcessIsFinished(false),
    m_ImportExportThreadIsRunning(false),
   // m_Simulation(true),
    m_Simulation(false),
    m_ProgramStartableManager(this),
    m_ControllerCreationFlag(false),
    m_AuthenticatedLevel(Global::OPERATOR)
{
}
catch (...) {
    //!< We Log to a temporary file any exception occured in the construction and  rethrow.
   Global::EventObject::Instance().LogToTemporaryFileAndThrowArgs(ERROR_CODE_HIMALAYA_CONSTRUCTION_FAILED);
}

/****************************************************************************/
HimalayaMasterThreadController::~HimalayaMasterThreadController() {
}


void HimalayaMasterThreadController::CreateAlarmHandler()
{
    mp_alarmHandler = new HimalayaErrorHandler::HimalayaAlarmHandler(5000, this);
    CONNECTSIGNALSLOT(mp_alarmHandler, sigSetTimeout(quint16), this, SetAlarmHandlerTimeout(quint16));
    CONNECTSIGNALSLOT(mp_alarmHandler, FireAlarmLocalRemote(bool), this, OnFireAlarmLocalRemote(bool));
}

/****************************************************************************/
void HimalayaMasterThreadController::CreateAndInitializeObjects() {
    try {
    //Move Event Object to Master thread, so that there is an event loop to process events
    // MTL: Moving singletons to other threads leads to an unstable system and segmentation fault
//    Global::EventObject::Instance().moveToThread(this->thread());
//    EventHandler::StateHandler::Instance().moveToThread(this->thread());

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

    //! \todo Do this when a rack is inserted
//    mp_DataManager->RefreshPossibleStationList();

    //Initialize program Startable manager
    m_ProgramStartableManager.Init();
    //Initialize objects in Master and base threads.
    MasterThreadController::CreateAndInitializeObjects();

    RegisterCommandForProcessing<NetCommands::CmdExternalProcessState, HimalayaMasterThreadController>
            (&HimalayaMasterThreadController::ExternalProcessConnectionHandler, this);

    //EventHandler Commands
    RegisterCommandForProcessing<NetCommands::CmdSystemAction, HimalayaMasterThreadController>
        (&HimalayaMasterThreadController::EventCmdSystemAction,this);
    RegisterAcknowledgeForProcessing<Global::AckOKNOK, HimalayaMasterThreadController>
        (&HimalayaMasterThreadController::OnAckOKNOK, this);
    }
    catch (...) {
        qDebug()<<"Create And Initialize Failed";
    }
    EventHandler::CrisisEventHandler::Instance().readEventStateConf("../Settings/EventStateError.csv");
}

/****************************************************************************/
void HimalayaMasterThreadController::CreateControllersAndThreads() {

    // let base class create controllers and threads
    MasterThreadController::CreateControllersAndThreads();

    CHECKPTR(mp_DataManager);
    CHECKPTR(mp_DataManager->GetDeviceConfigurationInterface());
    DataManager::CDeviceConfiguration *p_DeviceConfiguration = mp_DataManager->GetDeviceConfigurationInterface()->GetDeviceConfiguration();
    CHECKPTR(p_DeviceConfiguration);

    // create and connect scheduler main controller
    Scheduler::SchedulerMainThreadController *schedulerMainController = new Scheduler::SchedulerMainThreadController(HEARTBEAT_SOURCE_SCHEDULER_MAIN);
    AddAndConnectController(schedulerMainController, &m_CommandChannelSchedulerMain, static_cast<int>(SCHEDULER_MAIN_THREAD));
    schedulerMainController->DataManager(mp_DataManager);
    // register all commands for processing and routing
    RegisterCommands();



    // read password file
    DataManager::XmlConfigFilePasswords PwdFile(GetSerialNumber());
    PwdFile.ReadPasswords(Global::SystemPaths::Instance().GetSettingsPath() + "/Password.xml", m_PasswordManager);



    connect(&EventHandler::StateHandler::Instance(), SIGNAL(stateChanged(QString)), this, SLOT(SendStateChange(QString)));


}

/****************************************************************************/
void HimalayaMasterThreadController::OnPowerFail() {
    /// \todo implement
}

/****************************************************************************/
void HimalayaMasterThreadController::OnGoReceived() {
    CreateAndInitializeObjects();
    MasterThreadController::OnGoReceived();
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

}

/****************************************************************************/
void HimalayaMasterThreadController::InitiateShutdown() {
    // shutdown device command processor

}

/****************************************************************************/
void HimalayaMasterThreadController::SetDateTime(Global::tRefType Ref, const Global::CmdDateAndTime &Cmd,
                                                 Threads::CommandChannel &AckCommandChannel) {

    if(!MasterThreadController::SetAdjustedDateTimeOffset(Cmd.GetDateTime())) {

        // remove the offset seconds from the Adjusted time
        QString DateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs
                (-Global::AdjustedTime::Instance().GetOffsetSeconds()).toString();
        // raise the event
        Global::EventObject::Instance().RaiseEvent(EVENT_DATE_TIME_CANNOT_BE_MORE_THAN_24_HOURS,
                                                   Global::FmtArgs() << DateTime, true);

        QString TranslatedString = Global::UITranslator::TranslatorInstance().Translate
                (Global::TranslatableString(EVENT_DATE_TIME_CANNOT_BE_MORE_THAN_24_HOURS,
                                            Global::tTranslatableStringList()
                                            << DateTime));
        // send NACK
        SendAcknowledgeNOK(Ref, AckCommandChannel, TranslatedString, Global::GUIMSGTYPE_ERROR);
    }
    else {
        // send ACK
        SendAcknowledgeOK(Ref, AckCommandChannel);
    }

}

/****************************************************************************/
void HimalayaMasterThreadController::RegisterCommands() {
    //Dashboard
    RegisterCommandForRouting<MsgClasses::CmdProgramAction>(&m_CommandChannelSchedulerMain);//GUI-->master->Scheduler
    RegisterCommandForRouting<MsgClasses::CmdRetortLock>(&m_CommandChannelSchedulerMain);
    RegisterCommandForRouting<MsgClasses::CmdRetortLockStatus>(&m_CommandChannelGui);
    RegisterCommandForRouting<MsgClasses::CmdRetortStatus>(&m_CommandChannelGui);
    RegisterCommandForRouting<MsgClasses::CmdStationParaffinBathStatus>(&m_CommandChannelGui);
    RegisterCommandForRouting<MsgClasses::CmdProgramEndDateTime>(&m_CommandChannelGui);
    RegisterCommandForRouting<MsgClasses::CmdCurrentProgramStepInfor>(&m_CommandChannelGui);
    RegisterCommandForRouting<MsgClasses::CmdProgramStartReady>(&m_CommandChannelGui);

    RegisterCommandForRouting<NetCommands::CmdCriticalActionStatus>(&m_CommandChannelSoftSwitch);

    // -> Datalogging
    RegisterCommandForRouting<NetCommands::CmdDayRunLogRequest>(&m_CommandChannelDataLogging);    
    RegisterCommandForRouting<NetCommands::CmdDayRunLogRequestFile>(&m_CommandChannelDataLogging);
    RegisterCommandForRouting<NetCommands::CmdDayRunLogReply>(&m_CommandChannelGui);
    RegisterCommandForRouting<NetCommands::CmdDayRunLogReplyFile>(&m_CommandChannelGui);

    // -> device

    // so far the only registration with real function:
    RegisterCommandForProcessing<Global::CmdDateAndTime, HimalayaMasterThreadController>(&HimalayaMasterThreadController::SetDateTime, this);

    RegisterCommandForProcessing<NetCommands::CmdExportDayRunLogReply, HimalayaMasterThreadController>
            (&HimalayaMasterThreadController::ExportDayRunLogHandler, this);

    RegisterCommandForRouting<NetCommands::CmdExecutionStateChanged>(&m_CommandChannelGui);




    //Import-Export commands
    RegisterCommandForProcessing<MsgClasses::CmdDataExport, HimalayaMasterThreadController>
            (&HimalayaMasterThreadController::ImportExportDataFileHandler, this);
    RegisterCommandForProcessing<MsgClasses::CmdDataImport, HimalayaMasterThreadController>
            (&HimalayaMasterThreadController::ImportExportDataFileHandler, this);

    //User level functions
    RegisterCommandForProcessing<NetCommands::CmdChangeAdminPassword, HimalayaMasterThreadController>
            (&HimalayaMasterThreadController::ChangeAdminPasswordHandler, this);
    RegisterCommandForProcessing<NetCommands::CmdChangeUserLevel, HimalayaMasterThreadController>
            (&HimalayaMasterThreadController::ChangeUserLevelHandler, this);

    //GUI - Sent by Scheduler
    RegisterCommandForRouting<NetCommands::CmdEventReport>(&m_CommandChannelGui);


    //Update and keep the current system state
   RegisterCommandForProcessing<NetCommands::CmdSystemState, HimalayaMasterThreadController>
            (&HimalayaMasterThreadController::OnCmdSysState, this);


}

/****************************************************************************/
void HimalayaMasterThreadController::StartStatemachine() {
    // start own state machine and start to work.
    /// \todo implement

}


void HimalayaMasterThreadController::InitializeGUI() {
    HimalayaGui::HimalayaGuiController *p_GuiController = new HimalayaGui::HimalayaGuiController(HEARTBEAT_SOURCE_GUI);
    // Send XML files to GUI when its connected to Main
    CONNECTSIGNALSLOT(p_GuiController, SendConfigurationFiles(), this, SendXML());
    RegisterCommandForProcessing<NetCommands::CmdGuiInit, HimalayaMasterThreadController>
            (&HimalayaMasterThreadController::OnCmdGuiInitHandler, this);
    AddAndConnectController(p_GuiController, &m_CommandChannelGui, static_cast<int>(HIMALAYA_GUI_THREAD));
    StartSpecificThreadController(HIMALAYA_GUI_THREAD);
}

/****************************************************************************/
void HimalayaMasterThreadController::OnAckOKNOK(Global::tRefType Ref, const Global::AckOKNOK &Ack) {
    Q_UNUSED(Ref);
    Q_UNUSED(Ack);
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
            if (it.key() != HIMALAYA_GUI_THREAD) {
                StartSpecificThreadController(it.key());
            }
        }
        // log current time offset as info
        LOG_EVENT(Global::EVTTYPE_INFO, Global::LOG_ENABLED, Global::EVENT_GLOBAL_CURRENT_TIME_OFFSET,
                  QString::number(Global::AdjustedTime::Instance().GetOffsetSeconds(), 10)
                  , Global::NO_NUMERIC_DATA, false);
        // start own statemachine
        StartStatemachine();

        Global::EventObject::Instance().RaiseEvent(EVENT_SYSTEM_INIT_COMPLETE);
        EventHandler::StateHandler::Instance().setIdleState();
        qDebug()<<"SYSTEM INIT COMPLETE \n\n\n\n";

        //SendCommand(Global::CommandShPtr_t(new HimalayaErrorHandler::CmdRaiseAlarm(true)), m_CommandChannelSchedulerMain);
    } catch(const Global::Exception &E) {
        // destroy controllers and threads
        // send error message
        Global::EventObject::Instance().RaiseException(E);
        // and request exit
        Shutdown();
    } catch(const std::bad_alloc &) {
        // destroy controllers and threads
        // send error message
            Global::EventObject::Instance().RaiseEvent(Global::EVENT_GLOBAL_ERROR_MEMORY_ALLOCATION, FILE_LINE_LIST);
        // and request exit
        Shutdown();
    } catch(...) {
        // destroy controllers and threads
        // Send error message
        LOG_EVENT(Global::EVTTYPE_FATAL_ERROR, Global::LOG_ENABLED, Global::EVENT_GLOBAL_ERROR_UNKNOWN_EXCEPTION, FILE_LINE_LIST,
                  Global::NO_NUMERIC_DATA, false);
        // and request exit
        Shutdown();
    }
        m_ControllerCreationFlag = true;
    }
}

/****************************************************************************/
void HimalayaMasterThreadController::SendXML() {
    SendCommand(Global::CommandShPtr_t(new NetCommands::CmdGuiInit(2000, false)), m_CommandChannelGui);

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
        SendCommand(Global::CommandShPtr_t(new NetCommands::CmdConfigurationFile(5000, NetCommands::STATION, XmlStream)), m_CommandChannelGui);
    }

    // send reagent group list in xml ---------------------------------
    p_ByteArray->clear();
    (void)XmlStream.device()->reset();
    DataManager::CDataReagentGroupList *p_ReagentGroupList = mp_DataManager->GetReagentGroupList() ;
    if (p_ReagentGroupList) {
        XmlStream << *p_ReagentGroupList;
        SendCommand(Global::CommandShPtr_t(new NetCommands::CmdConfigurationFile(5000, NetCommands::REAGENTGROUP , XmlStream)), m_CommandChannelGui);
    }

    p_ByteArray->clear();
    (void)XmlStream.device()->reset();
    DataManager::CDataReagentList *p_ReagentList = mp_DataManager->GetReagentList() ;
    if (p_ReagentList) {
        XmlStream << *p_ReagentList;
        SendCommand(Global::CommandShPtr_t(new NetCommands::CmdConfigurationFile(5000, NetCommands::REAGENT, XmlStream)), m_CommandChannelGui);
    }

    p_ByteArray->clear();
    (void)XmlStream.device()->reset();

    DataManager::CReagentGroupColorList *p_ReagentGroupColorList = mp_DataManager->GetReagentGroupColorList() ;
    if (p_ReagentGroupColorList) {
        XmlStream << *p_ReagentGroupColorList;
        SendCommand(Global::CommandShPtr_t(new NetCommands::CmdConfigurationFile(5000, NetCommands::REAGENTGROUPCOLOR , XmlStream)), m_CommandChannelGui);
    }

    p_ByteArray->clear();
    (void)XmlStream.device()->reset();

    DataManager::CDataProgramList *p_ProgramList =  mp_DataManager->GetProgramList();

    if (p_ProgramList) {
        XmlStream << *p_ProgramList;
        SendCommand(Global::CommandShPtr_t(new NetCommands::CmdConfigurationFile(5000, NetCommands::PROGRAM , XmlStream)), m_CommandChannelGui);
    }

    p_ByteArray->clear();
    (void)XmlStream.device()->reset();
    DataManager::CUserSettingsInterface *p_SettingsInterface = mp_DataManager->GetUserSettingsInterface();
    if (p_SettingsInterface) {
        XmlStream << *p_SettingsInterface;
        SendCommand(Global::CommandShPtr_t(new NetCommands::CmdConfigurationFile(5000, NetCommands::USER_SETTING, XmlStream)), m_CommandChannelGui);
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
        SendCommand(Global::CommandShPtr_t(new NetCommands::CmdConfigurationFile(5000, NetCommands::DEVICE_CONFIGURATION, XmlStream)), m_CommandChannelGui);
    }

    QByteArray EventData;
    QDataStream EventDataStream(&EventData, QIODevice::ReadWrite);
    EventDataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));
    Global::tTranslations TempTranslations;
    TempTranslations = Global::UITranslator::TranslatorInstance().GetTranslations();
    EventDataStream << TempTranslations;
    EventDataStream.device()->reset();
    SendCommand(Global::CommandShPtr_t(new NetCommands::CmdEventStrings(5000, EventDataStream)), m_CommandChannelGui);

    delete p_ByteArray;
}


bool HimalayaMasterThreadController::IsCommandAllowed(const Global::CommandShPtr_t &Cmd) {

    return EventHandler::StateHandler::Instance().isAllowed(Cmd);
}


void HimalayaMasterThreadController::SendContainersTo(Threads::CommandChannel &rCommandChannel) {

    SendCommand(Global::CommandShPtr_t(new DataManager::CmdStationDataContainer(*mp_DataManager->GetStationList(),
                                                                                *mp_DataManager->GetReagentList())), rCommandChannel);
}


/****************************************************************************/
void HimalayaMasterThreadController::StartExportProcess() {
    // create and connect gui controller
    Export::ExportController *p_ExportController = new Export::ExportController(HEARTBEAT_SOURCE_EXPORT);
    // connect the process exit slot

    CONNECTSIGNALSLOT(p_ExportController, ProcessExited(const QString &, int), this, ExportProcessExited(const QString &, int));

    // add and connect the controller
    AddAndConnectController(p_ExportController, &m_CommandChannelExport, static_cast<int>(EXPORT_CONTROLLER_THREAD));
    // start the controller
    StartSpecificThreadController(static_cast<int>(EXPORT_CONTROLLER_THREAD));
}

void HimalayaMasterThreadController::SendStateChange(QString state) {
    if (state == "BusyState")
    {
        // inform gui
        SendCommand(Global::CommandShPtr_t(new NetCommands::CmdProcessState(3000, true)), m_CommandChannelGui);
    }

    if (state == "IdleState")
    {
        // inform gui
        SendCommand(Global::CommandShPtr_t(new NetCommands::CmdProcessState(3000, false)), m_CommandChannelGui);
    }

    if (state == "ErrorState")
    {

    }

    if (state == "NormalState")
    {

    }

}

/****************************************************************************/
void HimalayaMasterThreadController::ExportProcessExited(const QString &Name, int ExitCode) {
    // first clear the process
    StopSpecificThreadController(static_cast<int>(EXPORT_CONTROLLER_THREAD));
    // second clear the thread
    StopSpecificThreadController(static_cast<int>(IMPORT_EXPORT_THREAD));
    if (ExitCode == Global::EXIT_CODE_EXPORT_SUCCESS) {
        // raise the event code
        Global::EventObject::Instance().RaiseEvent(Global::EVENT_EXPORT_SUCCESS);
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
        }


        // this raise event code will be informed to GUI, that Export is failed
        Global::EventObject::Instance().RaiseEvent(Global::EVENT_EXPORT_FAILED);

        if (EventCode != Global::EVENT_EXPORT_FAILED) {
            // raise the event code
            Global::EventObject::Instance().RaiseEvent(EventCode);
        }

    }

    // send acknowledgement to GUI
    SendAcknowledgeOK(m_ImportExportCommandRef, *mp_ImportExportAckChannel);

    m_ExportProcessIsFinished = true;
    // enable the timer slot to destroy the objects after one second
    QTimer::singleShot(1000, this, SLOT(RemoveAndDestroyObjects()));

}

/****************************************************************************/
void HimalayaMasterThreadController::RemoveAndDestroyObjects() {
    // this is for safety
    if (m_ExportProcessIsFinished || m_ImportExportThreadIsRunning) {
        // remove and destroy the controller thread
        RemoveSpecificThreadController(static_cast<int>(EXPORT_CONTROLLER_THREAD));
        // remove and destroy the controller thread
        RemoveSpecificThreadController(static_cast<int>(IMPORT_EXPORT_THREAD));

        // reset the falgs, so nothing is executing
        m_ExportProcessIsFinished = false;
        m_ImportExportThreadIsRunning = false;

    }
}



/****************************************************************************/
void HimalayaMasterThreadController::ImportExportThreadFinished(const bool IsImport, const QString &TypeOfImport,
                                                                bool UpdatedCurrentLanguage,
                                                                bool NewLanguageAdded) {
    bool SendAckOK = false;
    if (IsImport) {
        // check the type of Impor
        if ((TypeOfImport.compare("User") == 0) || (TypeOfImport.compare("Service") == 0) || (TypeOfImport.compare("Leica") == 0)) {

            // send configuration files to GUI
            SendConfigurationFile(mp_DataManager->GetUserSettingsInterface(), NetCommands::USER_SETTING, m_CommandChannelGui);
            SendConfigurationFile(mp_DataManager->GetReagentList(), NetCommands::REAGENT, m_CommandChannelGui);
            SendConfigurationFile(mp_DataManager->GetReagentGroupList(), NetCommands::REAGENTGROUP, m_CommandChannelGui);
            SendConfigurationFile(mp_DataManager->GetReagentGroupColorList(), NetCommands::REAGENTGROUPCOLOR, m_CommandChannelGui);
            SendConfigurationFile(mp_DataManager->GetProgramList(), NetCommands::PROGRAM, m_CommandChannelGui);
            SendConfigurationFile(mp_DataManager->GetStationList(), NetCommands::STATION, m_CommandChannelGui);
            // inform the event handler
            Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_SUCCESS);
            // send ack OK
            SendAckOK = true;
        }
        else if(TypeOfImport == "Language") {
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

                        QString LanguageFileName = "Himalaya_" + LanguageName + ".qm";
                        if (!SendLanguageFileToGUI(LanguageFileName)) {
                            /// need to inform the event handler
                        }
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
        SendAcknowledgeNOK(m_ImportExportCommandRef, *mp_ImportExportAckChannel);
    }
    // clear the thread
    StopSpecificThreadController(static_cast<int>(IMPORT_EXPORT_THREAD));
    // enable the timer slot to destroy the objects after one second
    QTimer::singleShot(1000, this, SLOT(RemoveAndDestroyObjects()));
}

/****************************************************************************/
bool HimalayaMasterThreadController::UpdateSupportedGUILanguages() {

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
            Locale.remove(0, Locale.indexOf('_') + 1);   // "de"
            LanguageList << QLocale::languageToString(QLocale(Locale).language());
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
            QDataStream LangDataStream(&File);
            LangDataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));
            // send the command to GUI
            SendCommand(Global::CommandShPtr_t(new NetCommands::CmdLanguageFile(5000, LangDataStream)),
                        m_CommandChannelGui);
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
    // send ack to GUI
    SendAcknowledgeOK(Ref, AckCommandChannel);
    QString DeviceName;
    bool bPassed = false;
    switch(Cmd.GetUserLevel()) {
        case Global::ADMIN:
            if (m_PasswordManager.CheckPassword("Administrator", Cmd.GetPassword())) {
                m_AuthenticatedLevel = Global::ADMIN;
                bPassed = true;
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
                bPassed = true;
            }
            break;
        case Global::OPERATOR:
            // there is no password for the operator
            m_AuthenticatedLevel = Global::OPERATOR;
            bPassed = true;
            break;        
    }

    // check whether user level is operator, if it is operator then authentication failed
    if (m_AuthenticatedLevel == Global::OPERATOR && Cmd.GetUserLevel() == Global::ADMIN) {
        Global::EventObject::Instance().RaiseEvent(EVENT_PASSWORD_INCORRECT);
    }

    if (bPassed)
        SendCommand(Global::CommandShPtr_t(new NetCommands::CmdChangeUserLevel(Cmd)),m_CommandChannelEventThread);

    // send the authenticated command to GUI
    SendCommand(Global::CommandShPtr_t(new NetCommands::CmdChangeUserLevelReply(5000, m_AuthenticatedLevel)), m_CommandChannelGui);
    // check the whether fallback password is validated for the successful login
    if (m_PasswordManager.GetFallbackPasswordFlag()) {
        // User entered the fallback password so ask him to change the password
        SendCommand(Global::CommandShPtr_t(new NetCommands::CmdChangeAdminPasswordReply(5000, "New")), m_CommandChannelGui);
    }
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

    SendCommand(Global::CommandShPtr_t(new NetCommands::CmdChangeAdminPasswordReply(5000, CommandType)), m_CommandChannelGui);

}


/****************************************************************************/
void HimalayaMasterThreadController::EventCmdSystemAction(Global::tRefType Ref, const NetCommands::CmdSystemAction &Cmd,
                        Threads::CommandChannel &AckCommandChannel)
{
    Q_UNUSED(Ref);
    Q_UNUSED(AckCommandChannel);

    qDebug() << "In the event handling for CMD system Action for event " << Cmd.GetEventID();

                switch (Cmd.GetAction()) {
                    case Global::ACNTYPE_SHUTDOWN:
                        qDebug()<<"Shutting Down system";
                        Shutdown();
                        break;
                    default:
                        break;
                }
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
            emit CheckLoggingEnabled();
        }
    }
    else {
        if (Cmd.m_ProcessType == NetCommands::HIMALAYA_GUI_PROCESS)
        {
            Global::EventObject::Instance().RaiseEvent(EVENT_PROCESS_HIMALAYA_GUI_STOPPED, Global::FmtArgs(), false);
            Global::EventObject::Instance().RaiseEvent(EVENT_PROCESS_HIMALAYA_GUI_CONNECTED);
            Global::EventObject::Instance().RaiseEvent(EVENT_GUI_AVAILABLE, Global::FmtArgs(), true);
            // emit a signal so that if logging is disabled then Main can inform to GUI
            emit CheckLoggingEnabled();
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
void HimalayaMasterThreadController::RequestDayRunLogFileNames() {
    SendCommand(Global::CommandShPtr_t(new NetCommands::CmdExportDayRunLogRequest(m_AuthenticatedLevel)),
                                            m_CommandChannelDataLogging);

}

/****************************************************************************/
void HimalayaMasterThreadController::OnCmdSysState(Global::tRefType, const NetCommands::CmdSystemState &Cmd,
                      Threads::CommandChannel&)
{
    EventHandler::CrisisEventHandler::Instance().currentSysState(Cmd.m_StateId);
}

void HimalayaMasterThreadController::SetAlarmHandlerTimeout(quint16 timeout)
{
    mp_alarmHandler->setTimeout(timeout);
}

void HimalayaMasterThreadController::OnFireAlarmLocalRemote(bool isLocalAlarm)
{
    SendCommand(Global::CommandShPtr_t(new HimalayaErrorHandler::CmdRaiseAlarm(isLocalAlarm)), m_CommandChannelSchedulerMain);
}

} // end namespace Himalaya
