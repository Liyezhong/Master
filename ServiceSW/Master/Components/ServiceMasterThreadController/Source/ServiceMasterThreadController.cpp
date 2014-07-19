/****************************************************************************/
/*! \file HimalayaServiceMasterThreadController.cpp
 *
 *  \brief Implementation file for class HimalayaServiceMasterThreadController.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-02-06
 *  $Author:    $ Srivathsa HH
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

#include <ServiceMasterThreadController/Include/ServiceMasterThreadController.h>
#include <ServiceDeviceController/Include/ServiceDeviceController.h>
#include <Threads/Include/ThreadsEventCodes.h>

#include <Global/Include/Utils.h>
#include <Global/Include/SystemPaths.h>
#include <Global/Include/EventTranslator.h>
#include <Global/Include/UITranslator.h>
#include <Global/Include/GlobalExitCodes.h>

#include "ServiceDataManager/Include/ServiceDataManager.h"
#include "Core/Include/Startup.h"

#include <DataLogging/Include/DataLoggingThreadController.h>
#include <EventHandler/Include/EventHandlerThreadController.h>
#include <DeviceCommandProcessor/Include/Commands/CmdDeviceProcessingInit.h>

#include <DataManager/Helper/Include/XmlConfigFileStrings.h>
#include <DataManager/Helper/Include/DataManagerEventCodes.h>
#include <QMetaType>
#include <QSharedMemory>
#include <QDebug>

#include "ImportExport/Include/ImportExportThreadController.h"

#include "ExportController/Include/ExportController.h"

namespace Threads {
using namespace Global;

static const unsigned long THREAD_WAIT_TIME = 2000;             ///< Time to wait when stopping thread.
static const QString TimeOffsetFileName = "TimeOffset.xml";     ///< Name of file in which time offset is stored.

static const CommandExecuteFunctorAckShPtr_t    NullCommandExecuteFunctor(NULL);    ///< NULL functor for command execution.
static const CommandExecuteFunctorShPtr_t       NullCommandExecuteFunctorWithouAck(NULL); ///< Null functor command execution

/****************************************************************************/
ServiceMasterThreadController::ServiceMasterThreadController(Core::CStartup *startUp)
    : BaseThreadController(1, "BaseThread")
    , m_EventLoggerMaxFileSize(0)
    , m_DayEventLoggerMaxFileCount(0)
    , m_MaxAdjustedTimeOffset(0)
    , m_RebootCount(0)
    , m_CommandChannelDataLogging(this,
                                  "DataLogging",
                                  Global::EVENTSOURCE_DATALOGGER)
    , m_CommandChannelEventThread(this,
                                  "EventHandler",
                                  Global::EVENTSOURCE_EVENTHANDLER)
    , m_CommandChannelDeviceThread(this,
                                   "DeviceController",
                                   Global::EVENTSOURCE_NONE)
    , mp_EventThreadController(NULL)
    , mp_DeviceThreadController(NULL)
    , mp_ServiceDataManager(NULL)
    , mp_GUIStartup(startUp)
    , mp_ImportExportController(0)
    , mp_ImportExportThread(0)
    , mp_ExportController(0)
    , mp_ExportThread(0)
    , m_ImportExportThreadIsRunning(false)
    , m_ExportProcessIsFinished(false)
{
    // register the metytype for gSourceType
    qRegisterMetaType<Global::gSourceType>("Global::gSourceType");
    qRegisterMetaType<Service::ModuleTestCaseID>("Service::ModuleTestCaseID");
    CONNECTSIGNALSLOT(&m_ShutdownSharedMemTimer, timeout(), this, ExternalMemShutdownCheck());
    m_ShutdownSharedMemTimer.setInterval(500);
    m_ShutdownSharedMemTimer.setSingleShot(false);

    qDebug()<<"ServiceMasterThreadController::ServiceMasterThreadController() "<<Global::SystemPaths::Instance().GetSettingsPath();

    if (!connect(mp_GUIStartup, SIGNAL(DeviceInitRequest()), this, SLOT(sendDeviceInitCommand()))) {
        qDebug() << "CStartup: cannot connect 'deviceinitRequest' signal";
    }

    // Return Message to GUI
    if (!connect(this, SIGNAL(returnMessageToGUI(const QString)),
                 mp_GUIStartup, SLOT(ShowMessage(const QString)))) {
        qDebug() << "CStartup: cannot connect 'returnMessageToGUI' signal";
    }
    if (!connect(this, SIGNAL(ReturnErrorMessagetoMain(QString)),
                 mp_GUIStartup, SLOT(ShowErrorMessage(QString)))) {
        qDebug() << "CStartup: cannot connect 'ReturnErrorMessagetoMain' signal";
    }

    if (!connect(mp_GUIStartup,
       SIGNAL(ImportExportDataFileRequest(const QString &, const QByteArray &)),
                 this,
                 SLOT(ImportExportDataFile(const QString &, const QByteArray &))))
    {
        qDebug() << "CStartup: cannot connect 'ImportExportDataFileRequest' signal";
    }

    if (!connect(mp_GUIStartup,
       SIGNAL(ImportExportDataFileRequest(const QString &, const QByteArray &)),
                 this,
                 SLOT(ImportExportDataFile(const QString &, const QByteArray &))))
    {
        qDebug() << "CStartup: cannot connect 'ImportExportDataFileRequest' signal";
    }

    if (!connect(this,
                 SIGNAL(ImportFinish(bool)),
                 mp_GUIStartup,
                 SLOT(ImportFinished(bool))))
    {
        qDebug() << "CStartup: cannot connect 'ImportFinished' signal";
    }

    if (!connect(this,
                 SIGNAL(ExportFinish(bool)),
                 mp_GUIStartup,
                 SLOT(ExportFinished(bool))))
    {
        qDebug() << "CStartup: cannot connect 'ExportFinished' signal";
    }

    if (!connect(mp_GUIStartup, SIGNAL(AbortTest()),
                 this, SLOT(sendAbortTestCommand()))) {
        qDebug() << "CStartup: cannot connect 'AbortTest' signal";
    }

    if (!connect(mp_GUIStartup, SIGNAL(RVHeatingTest(quint8, quint8)),
                 this, SLOT(sendRVHeatingTestCommand(quint8, quint8)))) {
        qDebug() << "CStartup: cannot connect 'RVHeatingTest' signal";
    }

    if (!connect(mp_GUIStartup, SIGNAL(RVInitTest(qint32, quint8)),
                 this, SLOT(sendRotaryValveTestCommand(qint32, quint8)))) {
        qDebug() << "CStartup: cannot connect 'RVInitTest' signal";
    }

    if (!connect(mp_GUIStartup, SIGNAL(RVSelectTest(qint32, quint8)),
                 this, SLOT(sendRotaryValveTestCommand(qint32, quint8)))) {
        qDebug() << "CStartup: cannot connect 'RVSelectTest' signal";
    }

    if (!connect(mp_GUIStartup, SIGNAL(RVSealTest(qint32, quint8)),
                 this, SLOT(sendRotaryValveTestCommand(qint32, quint8)))) {
        qDebug() << "CStartup: cannot connect 'RVSealTest' signal";
    }

    if (!connect(mp_GUIStartup, SIGNAL(LevelSensorDetectingTest(qint32)),
                 this, SLOT(sendLevelSensorDetectingTestCommand(qint32)))) {
        qDebug() << "CStartup: cannot connect 'LevelSensorDetectingTest' signal";
    }

    if (!connect(mp_GUIStartup, SIGNAL(LevelSensorHeatingTest(quint8, quint8)),
                 this, SLOT(sendLevelSensorHeatingTestCommand(quint8, quint8)))) {
        qDebug() << "CStartup: cannot connect 'LevelSensorHeatingTest' signal";
    }

    if (!connect(mp_GUIStartup, SIGNAL(RetortHeatingTest(quint8, quint8)),
                 this, SLOT(sendRetortHeatingTestCommand(quint8, quint8)))) {
        qDebug() << "CStartup: cannot connect 'RetortHeatingTest' signal";
    }

    if (!connect(mp_GUIStartup, SIGNAL(TubeHeatingTest(quint8, quint8)),
                 this, SLOT(sendTubeHeatingTestCommand(quint8, quint8)))) {
        qDebug() << "CStartup: cannot connect 'TubeHeatingTest' signal";
    }

    if (!connect(mp_GUIStartup, SIGNAL(OvenHeatingTest(quint8, quint8)),
                 this, SLOT(sendOvenHeatingTestCommand(quint8, quint8)))) {
        qDebug() << "CStartup: cannot connect 'OvenHeatingTest' signal";
    }

    if (!connect(mp_GUIStartup, SIGNAL(OvenLidInitCalibrationRequest()),
                 this, SLOT(sendOvenLidInitCalibrationCommand()))) {
        qDebug() << "CStartup: cannot connect 'OvenLidInitCalibrationRequest' signal";
    }

    if (!connect(mp_GUIStartup, SIGNAL(PressureSensorCalibrationRequest()),
                 this, SLOT(sendPressureSensorCalibrationCommand()))) {
        qDebug() << "CStartup: cannot connect 'PressureSensorCalibrationRequest' signal";
    }

    if (!connect(this, SIGNAL(ReturnCalibrationInitMessagetoMain(QString,bool)),
                 mp_GUIStartup, SLOT(ShowCalibrationInitMessagetoMain(QString,bool)))) {
        qDebug() << "CStartup: cannot connect 'ReturnCalibrationInitMessagetoMain' signal";
    }

    // Manufacturing Tests
    if (!connect(mp_GUIStartup, SIGNAL(PerformManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID)), this, SLOT(sendManufacturingTestCommand(Service::ModuleTestCaseID, Service::ModuleTestCaseID)))) {
        qDebug() << "CStartup: cannot connect 'PerformManufacturingTest' signal";
    }

    // Refresh heating status to GUI
    if (!connect(this, SIGNAL(RefreshTestStatustoMain(QString, Service::ModuleTestStatus)),
                 mp_GUIStartup, SLOT(RefreshTestStatus(QString, Service::ModuleTestStatus)))) {
        qDebug() << "CStartup: cannot connect 'RefreshTestStatustoMain' signal";
    }

    if (!connect(this, SIGNAL(ReturnManufacturingMsgtoMain(bool )),
                 mp_GUIStartup, SLOT(OnReturnManufacturingMsg(bool )))) {
        qDebug() << "CStartup: cannot connect 'ReturnManufacturingMsgtoMain' signal";
    }

    // Shut down
    if (!connect(mp_GUIStartup, SIGNAL(ShutdownSystem()), this, SLOT(ShutdownSystem()))){
        qDebug() <<"CStartup: cannot connect 'ShutDownSystem' signal";
    }

    //mp_ServiceDataManager = new DataManager::CServiceDataManager(this);

   /* mp_DataManager = new DataManager::CDataManager(this);
    mp_DataManagerBase = mp_DataManager;

   mp_GUIStartup->mp_ServiceConnector->SetModuleListContainer(
                mp_ServiceDataManager->GetModuleList());*/
/*
    mp_GUIStartup->mp_ServiceConnector->SetDeviceConfigurationInterface(
                mp_ServiceDataManager->GetDeviceConfigurationInterface());

    mp_GUIStartup->mp_ServiceConnector->SetUserSettingInterface(
                mp_DataManager->GetUserSettingsInterface());*/

#if defined(QT_NO_DEBUG)
    // we are in release mode so we do not start timer
#else
    // we are in debug mode so we can start timer
    m_ShutdownSharedMemTimer.start();
#endif
}

/****************************************************************************/
ServiceMasterThreadController::~ServiceMasterThreadController() {
    try {
        mp_EventThreadController = NULL;
        m_TCCommandRoutes.clear();

        // remove and destroy the controller thread
        if (0 != mp_ExportController)
        {
            delete mp_ExportController;
            mp_ExportController = 0;
        }

        if (0 != mp_ExportThread)
        {
            delete mp_ExportThread;
            mp_ExportThread = 0;
        }
        // remove and destroy the controller thread
        if (0 != mp_ImportExportController)
        {
            delete mp_ImportExportController;
            mp_ImportExportController = 0;
        }

        if (0 != mp_ImportExportThread)
        {
            delete mp_ImportExportThread;
            mp_ImportExportThread = 0;
        }

        if (0 != mp_ServiceDataManager)
        {
            delete mp_ServiceDataManager;
            mp_ServiceDataManager = 0;
        }

    } catch(...) {
    }
}

/****************************************************************************/
void ServiceMasterThreadController::CreateAndInitializeObjects() {

    //CHECKPTR(mp_DataManagerBase);
    qDebug()<<"ServiceMasterThreadController::CreateAndInitializeObjects() Thread EventObject" << Global::EventObject::Instance().thread();
    //qDebug()<<" thread StateHandler" << EventHandler::StateHandler::Instance().thread();
    //Update serial number read from Device configuration xml
    //Serial number will be present in Log files.

    mp_DataLoggingThreadController->SetOperatingMode(m_OperatingMode);
    mp_DataLoggingThreadController->SetEventLoggerBaseFileName(m_EventLoggerBaseFileName);
    mp_DataLoggingThreadController->SetSerialNumber(m_SerialNumber);
    mp_DataLoggingThreadController->SetEventLoggerMaxFileSize(m_EventLoggerMaxFileSize);
    mp_DataLoggingThreadController->SetDayEventLoggerMaxFileCount(m_DayEventLoggerMaxFileCount);

    // read event strings. language and fallback language is English
    ReadEventTranslations(QLocale::English, QLocale::English);


    ServiceMasterThreadController::RegisterCommands();
}

/****************************************************************************/
void ServiceMasterThreadController::RegisterCommands() {
    qDebug()<<"Registering Command MasterThreadController";
    RegisterAcknowledgeForProcessing<Global::AckOKNOK, ServiceMasterThreadController>
            (&ServiceMasterThreadController::OnAckOKNOK, this);

    RegisterCommandForProcessing<DeviceCommandProcessor::CmdReturnMessage, ServiceMasterThreadController>
            (&ServiceMasterThreadController::OnReturnMessageCommand, this);
#if 0
    RegisterCommandForProcessing<DeviceCommandProcessor::CmdGetDataContainers, ServiceMasterThreadController>
            (&ServiceMasterThreadController::OnGetDataContainersCommand, this);
#endif

}
/****************************************************************************/


/****************************************************************************/
void ServiceMasterThreadController::OnAckOKNOK(Global::tRefType Ref, const Global::AckOKNOK &Ack) {
    Q_UNUSED(Ref);
    Q_UNUSED(Ack);
    qDebug()<<"ServiceMasterThreadController::OnAckOKNOK";
}
/****************************************************************************/


/****************************************************************************/
void ServiceMasterThreadController::CleanupAndDestroyObjects() {
    // nothing to do
}

/****************************************************************************/
void ServiceMasterThreadController::ConnectDataLoggingSignals(const BaseThreadController *pController) {
    CHECKPTR(mp_EventThreadController);
    //When an Event is raised, it will be received by Event hanlder.
    //EventHandler does 1) Check does logging if required. If found to be required, updates entry.
    //                  2) If event is of type warning/error/fatal error, its forwarded to errorhandler
    //Note:EventHandler is connected to DataLogger by signal slot mechanism.
    CONNECTSIGNALSLOT(pController, EmitDayEventEntry(const DataLogging::DayEventEntry &),
                      mp_EventThreadController, ProcessEvent(const DataLogging::DayEventEntry &));
}

/****************************************************************************/
void ServiceMasterThreadController::AddAndConnectController(ThreadController *pController, Threads::CommandChannel *pCommandChannel, int ControllerNumber, bool BasicThreadController) {
    // check pointers
    CHECKPTR(pController);
    CHECKPTR(pCommandChannel);

    if (!BasicThreadController) {
        // add the pair to m_Controllers to ensure it will be destroyed if connecting the signals fails
        // set thread to NULL to ensure everything works in direct calls untill threads have to be started.
        m_ControllerMap.insert(ControllerNumber, tControllerPair(pController, NULL));
        CONNECTSIGNALSLOT(this, SendGo(), pController, Go());
    }
    else {
        m_BasicControllersMap.insert(ControllerNumber, tControllerPair(pController, NULL));
        CONNECTSIGNALSLOT(this, SendGoToBasicThreads(), pController, Go());
    }

    m_channelList.insert(pCommandChannel->m_channelName.simplified().toUpper(), pCommandChannel);
    // connect some signals
    CONNECTSIGNALSLOT(this, SendStop(), pController, Stop());

    // and set controllers heartbeat timeout
    pController->SetHeartbeatTimeout(m_ControllerHeartbeatTimeout);
    // remember its ID for heartbeat checks
    //m_HeartbeatSources.insert(pController->GetHeartBeatSource());
    // insert command channel into broadcast vector
    m_BroadcastChannels.append(pCommandChannel);
    // connect comannd channel signals
    pController->ConnectToOtherCommandChannel(pCommandChannel);
}

/****************************************************************************/
void ServiceMasterThreadController::CreateControllersAndThreads() {
    // now create new objects common to all master threads
}

/****************************************************************************/
void ServiceMasterThreadController::CreateBasicControllersAndThreads() {

    // now create new objects which are essential at startup. For now the
    //basic thread controllers are 1) EventHandler and 2)DataLogger 3)SoftswitchManager

    // Create the data logger controller and the data logger thread.
    // Remember pointer to data logging controller to do some automatic connecting.
    mp_DataLoggingThreadController = new DataLogging::DataLoggingThreadController(m_HeartBeatSourceDataLogging, m_EventLoggerBaseFileName);
    // configure it

    // create system's event handler
    // if an exception occures, the instance must be deleted manually!
    QStringList FileName;
    FileName.append(Global::SystemPaths::Instance().GetSettingsPath()+QDir::separator()+"EventConfigService.xml");
    mp_EventThreadController = new EventHandler::HimalayaEventHandlerThreadController(m_HeartBeatSourceEventHandler, m_RebootCount, FileName);


    mp_EventThreadController->ConnectToEventObject();
    mp_DataLoggingThreadController->EnableImmediateLogging(true);

    // create system's Device handler
    mp_DeviceThreadController = new DeviceControl::ServiceDeviceController(m_HeartBeatSourceDeviceHandler);

    //***********************************************
    //This will connect EventHandler to DataLogger.
    //************************************************
    CONNECTSIGNALSLOT(mp_EventThreadController, LogEventEntry(const DataLogging::DayEventEntry &),
                      mp_DataLoggingThreadController, SendToDayEventLogger(const DataLogging::DayEventEntry &));
    // this will check whether logging is enabled or not and the same can inform to GUI
    CONNECTSIGNALSLOT(this, CheckLoggingEnabled(),
                      mp_DataLoggingThreadController, CheckLoggingEnabled());

    // Connecting mp_DataLoggingThreadController using AddAndConnectController ensures that
    // errors from it will be send to the error logger.
    // Sending day operation and component test entries lead to an additional call since they
    // will be send to itself and dispatched to the logger instances (which results in one additional
    // function call in the same thread.
    // On the other hand, using the same mechanism for *all* controllers, ensures that
    // if (for example) day operation entries should be send to more than one logger instance,
    // this can be implemented easily.
    try {
        AddAndConnectController(mp_DataLoggingThreadController, &m_CommandChannelDataLogging,
                                static_cast<int>(DATA_LOGGING_THREAD), true);
    } catch(...) {
        // m_pDataLoggingThreadController was not added properly so delete all allocated stuff yourself!
        // delete m_pDataLoggingThreadController
        delete mp_DataLoggingThreadController;
        mp_DataLoggingThreadController = NULL;
        // donn't forget to delete m_pEventThreadController also!
        delete mp_EventThreadController;
        mp_EventThreadController = NULL;
        // rethrow exception
        throw;
    }
    // connect all common signals and slots of the Event Handler and create its thread:
    try {
        AddAndConnectController(mp_EventThreadController, &m_CommandChannelEventThread,
                                static_cast<int>(EVENT_HANDLER_THREAD), true);

    } catch(...) {
        // m_pEventThreadController was not added properly so delete all allocated stuff yourself!
        // delete m_pEventThreadController
        delete mp_EventThreadController;
        mp_EventThreadController = NULL;
        // rethrow exception
        throw;
    }

    // connect all common signals and slots of the GUI Handler and create its thread:
    try {
        AddAndConnectController(mp_DeviceThreadController, &m_CommandChannelDeviceThread,
                                static_cast<int>(DEVICE_HANDLER_THREAD), true);

    } catch(...) {
        // mp_GUIThreadController was not added properly so delete all allocated stuff yourself!
        // delete m_pEventThreadController
        delete mp_EventThreadController;
        mp_EventThreadController = NULL;
        delete mp_DeviceThreadController;
        mp_DeviceThreadController = NULL;
        // rethrow exception
        throw;
    }

}
/****************************************************************************/
void ServiceMasterThreadController::DestroyControllersAndThreads(const bool BasicThreadController) {
}
/****************************************************************************/
void ServiceMasterThreadController::InitiateShutdown(){
}
/****************************************************************************/
void ServiceMasterThreadController::InitializeGUI(){
}
/****************************************************************************/
void ServiceMasterThreadController::InitializeControllers(bool BasicThreadController) {
   CreateAndInitializeObjects();
   tControllerMap Controllers;
   BasicThreadController ? (Controllers = m_BasicControllersMap) : (Controllers = m_ControllerMap);
    for(tControllerMap::iterator it = Controllers.begin(); it != Controllers.end(); ++it) {
        // Initialize controllers
        qDebug() << "ServiceMasterThreadController::InitializeControllers";

        it.value().first->CreateAndInitializeObjects();
    }
}

/****************************************************************************/
void ServiceMasterThreadController::CleanupControllers(bool BasicThreadController) {
    tControllerMap Controllers;
   BasicThreadController ? (Controllers = m_BasicControllersMap) : (Controllers = m_ControllerMap);
    for(tControllerMap::iterator it = Controllers.end(); it != Controllers.begin();) {
        // go backwards
        --it;
        // cleanup object
        it.value().first->DoCleanupAndDestroyObjects();
    }
}

/****************************************************************************/
void ServiceMasterThreadController::AttachControllersAndStartThreads(bool BasicThreadController) {
    tControllerMap Controllers;
    BasicThreadController ? (Controllers = m_BasicControllersMap) : (Controllers = m_ControllerMap);
    for(tControllerMap::iterator it = Controllers.begin(); it != Controllers.end(); ++it) {
        // create new thread
        it.value().second = new QThread();
        // attach controller to thread
        it.value().first->moveToThread(it->second);
        // now start thread and event loop
        //   From: http://qt-project.org/wiki/Threads_Events_QObjects
        //     Note that since Qt 4.4 ... the virtual method QThread::run() ...
        //     simply calls QThread::exec();, which starts the thread’s event loop
        it.value().second->start();
    }
    // start heartbeat check timer only if m_HeartbeatCheckTimeout > 0
    m_HeartbeatCheckTimeout = 10*1000;
   if(m_HeartbeatCheckTimeout > 0) {
       m_HeartbeatCheckTimer.setInterval(m_HeartbeatCheckTimeout);
       m_HeartbeatCheckTimer.setSingleShot(false);
       m_HeartbeatCheckTimer.start();
   } else {
      // LOG_EVENT(Global::EVTTYPE_INFO, Global::LOG_ENABLED, EVENT_THREADS_INFO_NO_HEARTBEAT_CHECKING, Global::tTranslatableStringList()
        //         , Global::NO_NUMERIC_DATA, false);
   }
    // and send them the Go signal
   BasicThreadController ? (emit SendGoToBasicThreads()): (emit SendGo());
}


/****************************************************************************/
void ServiceMasterThreadController::WaitForThreads(bool BasicThreadController) {
    tControllerMap Controllers;
    BasicThreadController ? (Controllers = m_BasicControllersMap) : (Controllers = m_ControllerMap);
    // stop heartbeat check timer
    m_HeartbeatCheckTimer.stop();
    for(tControllerMap::iterator it = Controllers.begin(); it !=  Controllers.end(); ++it) {
        // and wait for threads
        if(it.value().second != NULL) {
            if(!it.value().second->wait(THREAD_WAIT_TIME)){
                // thread did not quit within time
                Global::EventObject::Instance().RaiseEvent(EVENT_THREADS_ERROR_THREAD_STOP_ARGS,
                                                           Global::FmtArgs() << it->first->GetThreadID() << QString::number(THREAD_WAIT_TIME, 10));
             }
        }
    }
}

/****************************************************************************/
void ServiceMasterThreadController::RegisterCommandExecuteFunctor(const QString &CommandName, const CommandExecuteFunctorAckShPtr_t &Functor) {
    // check if already registered
    if(m_CommandExecuteFunctors.contains(CommandName)) {
        LOGANDTHROWARGS(EVENT_THREADS_ERROR_COMMAND_FUNCTOR_ALREADY_REGISTERED, CommandName);
    }
    // everything OK
    static_cast<void>(
                // we DO NOT NEED the return value of insert
                m_CommandExecuteFunctors.insert(CommandName, Functor)
                );
}

/****************************************************************************/
void ServiceMasterThreadController::RegisterCommandExecuteFunctor(const QString &CommandName, const CommandExecuteFunctorShPtr_t &Functor) {
    // check if already registered
    if(m_CommandExecuteWithoutAckFunctors.contains(CommandName)) {
        LOGANDTHROWARGS(EVENT_THREADS_ERROR_COMMAND_FUNCTOR_ALREADY_REGISTERED, CommandName);
    }
    // everything OK
    static_cast<void>(
                // we DO NOT NEED the return value of insert
                m_CommandExecuteWithoutAckFunctors.insert(CommandName, Functor)
                );
}

/****************************************************************************/
CommandExecuteFunctorAckShPtr_t ServiceMasterThreadController::GetCommandExecuteFunctor(const QString &CommandName) const {
    CommandExecuteFunctorAckHash_t::const_iterator it = m_CommandExecuteFunctors.find(CommandName);
    if(it == m_CommandExecuteFunctors.constEnd()) {
        // functor not found
        // return NULL functor
        return NullCommandExecuteFunctor;
    }
    // return functor
    return it.value();
}

/****************************************************************************/
CommandExecuteFunctorShPtr_t ServiceMasterThreadController::GetCommandExecuteFunctorWithoutAck(const QString &CommandName) const {
    CommandExecuteFunctorHash_t::const_iterator it = m_CommandExecuteWithoutAckFunctors.find(CommandName);
    if(it == m_CommandExecuteWithoutAckFunctors.constEnd()) {
        // functor not found
        // return NULL functor
        return NullCommandExecuteFunctorWithouAck;
    }
    // return functor
    return it.value();
}
/****************************************************************************/
void ServiceMasterThreadController::RegisterCommandRoutingChannel(const QString &CommandName, CommandChannel *pTargetCommandChannel) {
    CHECKPTR(pTargetCommandChannel);
    // check if already registered
    if(m_TCCommandRoutes.contains(CommandName)) {
        LOGANDTHROWARGS(EVENT_THREADS_ERROR_COMMAND_FUNCTOR_ALREADY_REGISTERED, CommandName);
    }
    // everything OK
    static_cast<void>(
                // we DO NOT NEED the return value of insert mp_ServiceDataManager = new DataManager::CServiceDataManager(this);
                m_TCCommandRoutes.insert(CommandName, pTargetCommandChannel)
                );
}

/****************************************************************************/
CommandChannel *ServiceMasterThreadController::GetCommandRouteChannel(const QString &CommandName) const {
    if(!m_TCCommandRoutes.contains(CommandName)) {
        // functor not found
        return NULL;
    }
    // return command channel
    return m_TCCommandRoutes.value(CommandName);
}

CommandChannel *ServiceMasterThreadController::GetComponentRouteChannel(Global::EventSourceType component) const {
    QHashIterator<QString, Threads::CommandChannel*> i(m_channelList);
    while (i.hasNext())
    {
        i.next();
        if (i.value()->m_componentType == component)
        {
            return i.value();
        }
    }
    return NULL;
}

//lint -efunc(613, Threads::MasterThreadController::OnExecuteCommand)
// JB: Since lint does not know that Cmd can not be "NULL" after checking it with IsNull
// it will throw lots of 613 warnings when accessing Cmd, so we disable 613 for
// this method. Remember that 613 is disabled forr all pointers in this function!
/****************************************************************************/
void ServiceMasterThreadController::OnExecuteCommand(Global::tRefType Ref, const Global::CommandShPtr_t &Cmd, Threads::CommandChannel &AckCommandChannel)
{
    try {
//        qDebug() << "MasterThreadController::OnExecuteCommand" << Ref << Cmd.GetPointerToUserData()->GetName();

        if (!IsCommandAllowed(Cmd))
        {
            qDebug() << "MasterThreadController::OnExecuteCommand, command disallowed" << Cmd.GetPointerToUserData()->GetName();
            return;
        }

        // check pointer
        CHECKPTR(Cmd.GetPointerToUserData());

//        SEND_DEBUG(WHEREAMI + " " +
//                   QString("Ref = ") + QString::number(Ref, 10) +
//                   QString("Name = ") + Cmd->GetName());
        // check if this command should be routed
        CommandChannel *pChannel;
        if (Cmd->GetName().contains("CmdSystemAction"))
        {
            //NetCommands::CmdSystemAction *actionCommand = (NetCommands::CmdSystemAction*)Cmd.GetPointerToUserData();
            //pChannel = GetComponentRouteChannel(actionCommand->GetSource());
            qDebug() << "MasterThreadController::OnExecuteCommand, system commands disallowed";
            return;
        }
        else
        {
            pChannel = GetCommandRouteChannel(Cmd->GetName());
        }
        if(pChannel != NULL) {
            // yes, we must route it
            // get new ref
            Global::tRefType NewRef = GetNewCommandRef();
            // check if has timeout
            if(Cmd->GetTimeout() != Global::Command::NOTIMEOUT) {
                // it has a timeout and also an acknowledge
                tRefChannelPair Pair(Ref, &AckCommandChannel);

                // add new ref to list of routing refs
                static_cast<void> (
                            // we DO NOT NEED the return value of insert
                            m_TCAcknowledgeRoutes.insert(NewRef, Pair)
                            );
            }
            //Check if some processing must be done before sending it to the target channel
            CommandExecuteFunctorShPtr_t Functor = GetCommandExecuteFunctorWithoutAck(Cmd->GetName());
            if (Functor == NullCommandExecuteFunctorWithouAck) {
                // and send command
                DoSendCommand(NewRef, Cmd, *pChannel);
            }
            else {
                //We need to process before sending command to the target channel
                Functor.GetPointerToUserData()->Execute(Ref, Cmd.GetPointerToUserData());
                // and send command
                DoSendCommand(NewRef, Cmd, *pChannel);
            }

        } else {
            // no routing, try to execute
            // Get functor and execute
            CommandExecuteFunctorAckShPtr_t Functor = GetCommandExecuteFunctor(Cmd->GetName());
            if(Functor == NullCommandExecuteFunctor) {
                // throw exception
                qDebug() << "MasterThreadController::OnExecuteCommand, no command functor for" << Cmd->GetName();
                LOGANDTHROWARG(EVENT_THREADS_ERROR_UNSUPPORTED_COMMAND, Cmd->GetName());
            }
            // execute
            Functor.GetPointerToUserData()->Execute(Ref, Cmd.GetPointerToUserData(), AckCommandChannel);
        }
    }
    CATCHALL();
}

//lint -efunc(613, Threads::MasterThreadController::OnProcessAcknowledge)
// JB: Since lint does not know that Ack can not be "NULL" after checking it with IsNull
// it will throw lots of 613 warnings when accessing Ack, so we disable 613 for
// this method. Remember that 613 is disabled forr all pointers in this function!
/****************************************************************************/
void ServiceMasterThreadController::OnProcessAcknowledge(Global::tRefType Ref, const Global::AcknowledgeShPtr_t &Ack)
{
//    qDebug() << "MasterThreadController::OnProcessAcknowledge" << Ref << Ack.GetPointerToUserData()->GetName();

    try {
        // check pointer
        CHECKPTR(Ack.GetPointerToUserData());

//        SEND_DEBUG(WHEREAMI + " " +
//                   QString("Ref = ") + QString::number(Ref, 10) +
//                   QString("Name = ") + Ack->GetName());
        if(m_TCAcknowledgeRoutes.contains(Ref))
        {
            // it seems that we must route acknowledge
            // Remove from list of blocked references
            UnblockCommandRef(Ref);
            // remove from list of pending commands in base class
            RemoveFromPendingCommands(Ref);
            // get pointer to acknowledge channel
            tRefChannelPair Pair = m_TCAcknowledgeRoutes[Ref];
            Global::tRefType SendRef = Pair.first;
            CommandChannel *pAckChannel = Pair.second;
            // remove from lists
            static_cast<void>(
                        // we DO NOT NEED the return value of remove
                        m_TCAcknowledgeRoutes.remove(Ref)
                        );
            CHECKPTR(pAckChannel);
            // and send
            SendAcknowledge(SendRef, Ack, *pAckChannel);
        } else {
            // try to process acknoledge
            BaseThreadController::OnProcessAcknowledge(Ref, Ack);
        }
    }
    CATCHALL();
}


/****************************************************************************/
void ServiceMasterThreadController::OnProcessTimeout(Global::tRefType Ref, const QString &CmdName) {
//    SEND_DEBUG(WHEREAMI + " " +
//               QString("Ref = ") + QString::number(Ref, 10) +
//               QString("Name = ") + CmdName);
    if(m_TCAcknowledgeRoutes.contains(Ref)) {
        // send error
        Global::EventObject::Instance().RaiseEvent(EVENT_THREADS_ERROR_COMMAND_TIMEOUT, Global::FmtArgs() << CmdName << Ref, true);

//        LOG_EVENT(Global::EVTTYPE_FATAL_ERROR, Global::LOG_ENABLED, EVENT_THREADS_ERROR_COMMAND_TIMEOUT, Global::tTranslatableStringList() << CmdName << QString::number(Ref, 10)
//                  , Global::NO_NUMERIC_DATA ,false);
        // remove from list of pending commands
        RemoveFromPendingCommands(Ref);
    } else {
        // try to process timeout
        BaseThreadController::OnProcessTimeout(Ref, CmdName);
    }
}

/****************************************************************************/
void ServiceMasterThreadController::BroadcastCommand(const Global::CommandShPtr_t &Cmd) {
    // send command to all connected thread controllers.
    for(tCommandChannelVector::iterator it = m_BroadcastChannels.begin(); it != m_BroadcastChannels.end(); ++it) {
        // get new ref
        Global::tRefType Ref = GetNewCommandRef();
        // and send command to that channel
        DoSendCommand(Ref, Cmd, *(*it));
    }
}

/****************************************************************************/
Global::tRefType ServiceMasterThreadController::SendCommand(const Global::CommandShPtr_t &Cmd, CommandChannel &CmdChannel) {
    // get new command ref
    Global::tRefType Ref = GetNewCommandRef();
    // and send command to that channel
    DoSendCommand(Ref, Cmd, CmdChannel);
    // return computed Ref
    return Ref;
}

/****************************************************************************/
void ServiceMasterThreadController::SendAcknowledge(Global::tRefType Ref, const Global::AcknowledgeShPtr_t &Ack, CommandChannel &CmdChannel) {
    // send
    DoSendAcknowledge(Ref, Ack, CmdChannel);
}

/****************************************************************************/
void ServiceMasterThreadController::SendAcknowledgeOK(Global::tRefType Ref, CommandChannel &CmdChannel) {
    // create acknowledge and send
    SendAcknowledge(Ref, Global::AcknowledgeShPtr_t(new Global::AckOKNOK(true)), CmdChannel);
}

/****************************************************************************/
void ServiceMasterThreadController::SendAcknowledgeNOK(Global::tRefType Ref, CommandChannel &CmdChannel, const QString &Text, Global::GUIMessageType Type) {
    // create acknowledge and send
    SendAcknowledge(Ref, Global::AcknowledgeShPtr_t(new Global::AckOKNOK(false, Text, Type)), CmdChannel);
}

/****************************************************************************/
void ServiceMasterThreadController::ReadEventTranslations(QLocale::Language Language, QLocale::Language FallbackLanguage) const
{
    const QString StringsFileName = Global::SystemPaths::Instance().GetTranslationsPath()
            + "/EventStrings_" + Global::LanguageToLanguageCode(Language) + ".xml";
    // cleanup translator strings. For event strings.
    Global::EventTranslator::TranslatorInstance().Reset();
    // read strings
    DataManager::XmlConfigFileStrings TranslatorDataFile;
    // Create list of used languages. Language and FallbackLanguage can be the same, since we are
    // working wit a QSet
    QSet<QLocale::Language> LanguageList;
    LanguageList << Language << FallbackLanguage;
    TranslatorDataFile.ReadStrings(StringsFileName, LanguageList);
    // check if there is still a language in LanguageList
    if(!LanguageList.isEmpty())
    {
        // Uh oh... some languages could not be read.
        // send some error messages.
        for(QSet<QLocale::Language>::const_iterator it = LanguageList.constBegin(); it != LanguageList.constEnd(); ++it)
        {
            Global::EventObject::Instance().RaiseEvent(DataManager::EVENT_DM_ERROR_LANG_NOT_FOUND,
                                                       Global::FmtArgs() << Global::LanguageToString(*it));
        }
    }
    // now configure translator with read languages.
    for(Global::tTranslations::const_iterator it = TranslatorDataFile.Data().constBegin();
        it != TranslatorDataFile.Data().constEnd();
        ++it)
    {
        // Set language data. No default no fallback.
        Global::EventTranslator::TranslatorInstance().SetLanguageData(it.key(), it.value(), false, false);
    }

    // set default language
    Global::EventTranslator::TranslatorInstance().SetDefaultLanguage(Language);
    // set fallback language
    Global::EventTranslator::TranslatorInstance().SetFallbackLanguage(FallbackLanguage);
}

/****************************************************************************/
void ServiceMasterThreadController::OnGoReceived()
{
    qDebug() << " ServiceMasterThreadController::OnGoReceived()";
    CreateBasicControllersAndThreads();
    // all objects created from now on, live in this thread so they are served by the
    // signal/slot mechanism in this event loop.

    mp_ServiceDataManager = new DataManager::CServiceDataManager(this);

    mp_GUIStartup->mp_ServiceConnector->SetModuleListContainer(
                mp_ServiceDataManager->GetDataContainer()->ModuleList,
                mp_ServiceDataManager->GetDataContainer()->ModuleListArchive);
    mp_GUIStartup->mp_ServiceConnector->SetDeviceConfigurationInterface(
                mp_ServiceDataManager->GetDataContainer()->DeviceConfigurationInterface);
    mp_GUIStartup->mp_ServiceConnector->SetServiceParametersContainer(
                mp_ServiceDataManager->GetDataContainer()->ServiceParameters);

    try {
        // Initialize controllers
        InitializeControllers(true);

        // connect heartbeat check timer
        CONNECTSIGNALSLOT(&m_HeartbeatCheckTimer, timeout(), this, HeartbeatCheck());

        // now start threads
        AttachControllersAndStartThreads(true);

        qDebug()<<"\n\n $$$$$$$$$$$$$$$$$$$$$$$$$$$$ ON GO RECEIVED $$$$$$$$$ \n\n\n\n";

        return;
    }
    CATCHALL();

    Shutdown();
}



 /****************************************************************************/
void ServiceMasterThreadController::OnStopReceived() {
    // Cleanup controllers
    CleanupControllers();
    CleanupControllers(true);

    // destroy controllers and threads
    DestroyControllersAndThreads();
    DestroyControllersAndThreads(true);
}

/****************************************************************************/
void ServiceMasterThreadController::Shutdown() {
    Global::EventObject::Instance().RaiseEvent(Global::EVENT_GLOBAL_STRING_TERMINATING, Global::tTranslatableStringList() <<"");

    // send Stop signal to all thread controllers
    emit SendStop();

    // stop all threads and wait for them
    WaitForThreads();
    WaitForThreads(true);

    // call own Stop method
    Stop();

    // send request to finish master thread
    emit RequestFinish();
}

/****************************************************************************/
void ServiceMasterThreadController::HeartbeatCheck() {
    qDebug() << "MasterThreadController::HeartbeatCheck";
    mp_DeviceThreadController->InitDevices();
    m_HeartbeatCheckTimer.stop();
}

/****************************************************************************/
void ServiceMasterThreadController::HeartbeatSlot(const Global::gSourceType &TheHeartBeatSource) {
    // remember received heartbeat logging source.
    m_ArrivedHeartbeats.insert(TheHeartBeatSource);
}

/****************************************************************************/
void ServiceMasterThreadController::StopSpecificThreadController(const int ControllerNumber, const bool BasicThreadController) {
    // Cleanup controllers
    // create the thread controller and monitor the for the command
    bool ThreadFound = false;
    tControllerPair ControllerPair;
    if (!BasicThreadController) {
        if (m_ControllerMap.contains(ControllerNumber)) {
            ControllerPair = m_ControllerMap.value(ControllerNumber);
            ThreadFound = true;
        }
    }
    else  {
        if (m_BasicControllersMap.contains(ControllerNumber)) {
            ControllerPair = m_BasicControllersMap.value(ControllerNumber);
            ThreadFound = true;
        }
    }

    if (ThreadFound) {
        ControllerPair.first->DoCleanupAndDestroyObjects();
        // block all the signals
        ControllerPair.second->blockSignals(true);
        ControllerPair.second->quit();
    }
}

/****************************************************************************/
void ServiceMasterThreadController::RemoveSpecificThreadController(const int ControllerNumber, const bool BasicThreadController) {
    bool ThreadFound = false;
    tControllerPair ControllerPair;
    if (!BasicThreadController) {
        if (m_ControllerMap.contains(ControllerNumber)) {
            ControllerPair = m_ControllerMap.value(ControllerNumber);
            m_ControllerMap.remove(ControllerNumber);
            ThreadFound = true;
        }
    }
    else  {
        if (m_BasicControllersMap.contains(ControllerNumber)) {
            ControllerPair = m_BasicControllersMap.value(ControllerNumber);
            m_BasicControllersMap.remove(ControllerNumber);
            ThreadFound = true;
        }
    }
    if (ThreadFound) {
            // destroy all the objects
            delete ControllerPair.first;
            delete ControllerPair.second;

    }
}

/****************************************************************************/
void ServiceMasterThreadController::StartSpecificThreadController(const int ControllerNumber, const bool BasicThreadController) {
    bool ThreadFound = false;
    tControllerPair ControllerPair;
    if (!BasicThreadController) {
        if (m_ControllerMap.contains(ControllerNumber)) {
            ControllerPair = m_ControllerMap.value(ControllerNumber);
            ThreadFound = true;
        }
    }
    else  {
        if (m_BasicControllersMap.contains(ControllerNumber)) {
            ControllerPair = m_BasicControllersMap.value(ControllerNumber);
            ThreadFound = true;
        }
    }
    if (ThreadFound) {
        ControllerPair.first->CreateAndInitializeObjects();
        ControllerPair.second = new QThread();
        m_ControllerMap.insert(ControllerNumber, ControllerPair);
        ControllerPair.first->moveToThread(ControllerPair.second);
        CONNECTSIGNALSLOT(ControllerPair.second, started(), ControllerPair.first, Go());
        ControllerPair.second->start();
        //BasicThreadController ? (emit SendGoToBasicThreads()): (emit SendGo());
    }
}

/****************************************************************************/
void ServiceMasterThreadController::ExternalMemShutdownCheck() {
#if defined(QT_NO_DEBUG)
    // we are in release mode so we exit
    return;
#else
    // we are in debug mode
    bool oTerminate = false;
    {
        // create shared memory object
        QSharedMemory ShutdownSharedMem(this);
        ShutdownSharedMem.setKey(m_ShutdownSharedMemName);
        // try to attach
        if(ShutdownSharedMem.attach()) {
            // attach OK
            if(ShutdownSharedMem.lock()) {
                // read data
                unsigned char *pbData = reinterpret_cast<unsigned char *>(ShutdownSharedMem.data());
                if(pbData != NULL) {
                    oTerminate = (*pbData != 0);
                    // reset
                    *pbData = 0;
                }
                static_cast<void> (
                            // we DO NOT NEED the return value of next
                            ShutdownSharedMem.unlock()
                            );
            }
        }
    }
    // check if termination requested.
    if(oTerminate) {
        InitiateShutdown();
    }
#endif
}

/****************************************************************************/
void ServiceMasterThreadController::sendDeviceInitCommand() {

    qDebug()<<"ServiceMasterThreadController::sendDeviceInitCommand called!!!";

    if(NULL == mp_ServiceDataManager)
    {
        qDebug()<<"mp_ServiceDataManager is NULL";
    }

    m_ExpectedDCRef = SendCommand(Global::CommandShPtr_t(new DeviceCommandProcessor::CmdDeviceProcessingInit()),
                                  m_CommandChannelDeviceThread);

}

/****************************************************************************/
/**
 * \brief Send a data changed command.
 *
 * The command will be broadcasted to all connected thread controllers.
 *
 * \param[in]   Cmd         The command.
 */
/****************************************************************************/
void ServiceMasterThreadController::DoSendDataChanged(const Global::CommandShPtr_t &Cmd) {
    // process command to ensure local OnDataChanged methods are called
    const Global::CmdDataChanged *pCommand = dynamic_cast<Global::CmdDataChanged *>(Cmd.GetPointerToUserData());
    CHECKPTR(pCommand);
    DispatchDataChangedCommand(*pCommand);
    // broadcast command to all
    BroadcastCommand(Cmd);
}

/****************************************************************************/
void ServiceMasterThreadController::OnPowerFail(const Global::PowerFailStages PowerFailStage)
{
    Q_UNUSED(PowerFailStage);
}
/****************************************************************************/

/****************************************************************************/
/**
 * \brief Start statemachine.
 *
 * Start own statemachine and start to work.
 * Must be implemented in derived classes.
 */
/****************************************************************************/
void ServiceMasterThreadController::StartStatemachine(){
    // Implementation of StartStateMachine
}

void ServiceMasterThreadController::ImportExportDataFile(
        const QString &CommandName, const QByteArray &CommandData)
{
    qDebug() << "ServiceMasterThreadController::ImportExportDataFile, command: "
             << CommandName
             << " data: "
             << CommandData;

    if (!m_ImportExportThreadIsRunning)
    {
        // create and connect scheduler controller
        /*mp_ImportExportController =
                new ImportExport::ImportExportThreadController(
                    HEARTBEAT_SOURCE_IMPORTEXPORT,
                    *mp_DataManager,
                    CommandName,
                    CommandData);
*/
        // connect the siganl slot mechanism to create the process.
        CONNECTSIGNALSLOT(mp_ImportExportController,
                          StartExportProcess(),
                          this,
                          StartExportProcess());

        CONNECTSIGNALSLOT(mp_ImportExportController,
                        ThreadFinished(const bool, const QString &, bool, bool),
                          this,
           ImportExportThreadFinished(const bool, const QString &, bool, bool));

        CONNECTSIGNALSLOT(this,
                          DayRunLogDirectoryName(const QString &),
                          mp_ImportExportController,
                          SetDayRunLogFilesDirectoryName(const QString &));

        // start import/export thread
        mp_ImportExportController->CreateAndInitializeObjects();

        mp_ImportExportThread = new QThread();
        mp_ImportExportController->moveToThread(mp_ImportExportThread);

        CONNECTSIGNALSLOT(mp_ImportExportThread,
                          started(),
                          mp_ImportExportController,
                          Go());

        mp_ImportExportThread->start();

        m_ImportExportThreadIsRunning = true;
    }
    else
    {
    }
}

void ServiceMasterThreadController::StartExportProcess(void)
{
    mp_ExportController = new Export::ExportController(THREAD_ID_EXPORT);

    CONNECTSIGNALSLOT(mp_ExportController,
                      ProcessExited(const QString &, int),
                      this,
                      ExportProcessExited(const QString &, int));

    mp_ExportThread = new QThread();
    mp_ExportController->moveToThread(mp_ExportThread);

    CONNECTSIGNALSLOT(mp_ExportThread,
                      started(),
                      mp_ExportController,
                      Go());

    mp_ExportThread->start();
}

/****************************************************************************/
void ServiceMasterThreadController::ExportProcessExited(const QString &Name, int ExitCode)
{
    // first clear the export thread
    mp_ExportController->CleanupAndDestroyObjects();
    // block all the signals
    mp_ExportThread->blockSignals(true);
    mp_ExportThread->quit();

    // second clear the import export thread
    mp_ImportExportController->CleanupAndDestroyObjects();
    // block all the signals
    mp_ImportExportThread->blockSignals(true);
    mp_ImportExportThread->quit();

    bool ExportFailed = true;
    if (ExitCode == Global::EXIT_CODE_EXPORT_SUCCESS)
    {
        // raise the event code
        Global::EventObject::Instance().RaiseEvent(EVENT_EXPORT_SUCCESS);

        ExportFailed = false;
    }
    else
    {
        quint32 EventCode = EVENT_EXPORT_FAILED;

        // map the exit codes with the event code
        switch(ExitCode)
        {
        case Global::EXIT_CODE_EXPORT_FAILED:
            EventCode = EVENT_EXPORT_FAILED;
            break;
        case Global::EXIT_CODE_EXPORT_UNABLE_TO_READ_FILE_TEMP_EXPORTCONFIGURATION:
            EventCode = EVENT_EXPORT_UNABLE_TO_READ_FILE_TEMP_EXPORTCONFIGURATION;
            break;
        case Global::EXIT_CODE_EXPORT_INIT_CONTAINER_FAILED:
            EventCode = EVENT_EXPORT_INIT_CONTAINER_FAILED;
            break;
        case Global::EXIT_CODE_EXPORT_VERIFICATION_CONTAINER_FAILED:
            EventCode = EVENT_EXPORT_VERIFICATION_CONTAINER_FAILED;
            break;
        case Global::EXIT_CODE_EXPORT_UNABLE_ARCHIVE_FILES:
            EventCode = EVENT_EXPORT_UNABLE_TO_ARCHIVE_FILES ;
            break;
        case Global::EXIT_CODE_EXPORT_SOURCE_DIRECTORY_NOT_EXISTS:
            EventCode = EVENT_EXPORT_SOURCE_DIRECTORY_NOT_EXISTS;
            break;
        case Global::EXIT_CODE_EXPORT_TARGET_DIRECTORY_NOT_EXISTS:
            EventCode = EVENT_EXPORT_TARGET_DIRECTORY_NOT_EXISTS;
            break;
        case Global::EXIT_CODE_EXPORT_LOG_DIRECTORY_NOT_EXISTS:
            EventCode = EVENT_EXPORT_LOG_DIRECTORY_NOT_EXISTS;
            break;
        case Global::EXIT_CODE_EXPORT_TARGET_FILE_FORMAT_IS_WRONG:
            EventCode = EVENT_EXPORT_TARGET_FILE_FORMAT_IS_WRONG;
            break;
        case Global::EXIT_CODE_EXPORT_INVALID_EXPORT:
            EventCode = EVENT_EXPORT_INVALID_EXPORT;
            break;
        case Global::EXIT_CODE_EXPORT_CRYTOSERVICE_RUNNING:
            EventCode = EVENT_EXPORT_CRYTOSERVICE_RUNNING;
            break;
        case Global::EXIT_CODE_EXPORT_CANNOT_OPEN_FILE_FOR_READ:
            EventCode = EVENT_EXPORT_CANNOT_OPEN_FILE_FOR_READ;
            break;
        case Global::EXIT_CODE_EXPORT_CANNOT_OPEN_FILE_FOR_WRITE:
            EventCode = EVENT_EXPORT_CANNOT_OPEN_FILE_FOR_WRITE;
            break;
        case Global::EXIT_CODE_EXPORT_ERROR_TO_READ:
            EventCode = EVENT_EXPORT_ERROR_TO_READ;
            break;
        case Global::EXIT_CODE_EXPORT_ERROR_TO_WRITE:
            EventCode = EVENT_EXPORT_ERROR_TO_WRITE;
            break;
        case Global::EXIT_CODE_EXPORT_INDEX_IS_MATCHING:
            EventCode = EVENT_EXPORT_INDEX_IS_MATCHING;
            break;
        case Global::EXIT_CODE_EXPORT_KEY_SIZE_LESS:
            EventCode = EVENT_EXPORT_KEY_SIZE_LESS;
            break;
        case Global::EXIT_CODE_EXPORT_KEYDATA_SIZE_IS_NOT_MATCHING:
            EventCode = EVENT_EXPORT_KEYDATA_SIZE_IS_NOT_MATCHING;
            break;
        case Global::EXIT_CODE_EXPORT_HMAC_NOT_INITIALIZED:
            EventCode = EVENT_EXPORT_HMAC_NOT_INITIALIZED;
            break;
        case Global::EXIT_CODE_EXPORT_AES_NOT_INITIALIZED:
            EventCode = EVENT_EXPORT_AES_NOT_INITIALIZED;
            break;
        case Global::EXIT_CODE_EXPORT_INTEGER_SIZE_IS_MORE:
            EventCode = EVENT_EXPORT_INTEGER_SIZE_IS_MORE;
            break;
        case Global::EXIT_CODE_EXPORT_MSB_BIT_IS_NOT_SET:
            EventCode = EVENT_EXPORT_MSB_BIT_IS_NOT_SET;
            break;
        case Global::EXIT_CODE_EXPORT_INVALID_FILE_MODE:
            EventCode = EVENT_EXPORT_INVALID_FILE_MODE;
            break;
        case Global::EXIT_CODE_EXPORT_HMAC_COMPUTATION_STARTED:
            EventCode = EVENT_EXPORT_HMAC_COMPUTATION_STARTED;
            break;
        case Global::EXIT_CODE_EXPORT_ZIP_ERROR:
            EventCode = EVENT_EXPORT_ZIP_ERROR;
            break;
        case Global::EXIT_CODE_EXPORT_ZIP_COMMAND_NOT_FOUND:
            EventCode = EVENT_EXPORT_ZIP_COMMAND_NOT_FOUND;
            break;
        case Global::EXIT_CODE_EXPORT_ZIP_IS_TAKING_LONGTIME:
            EventCode = EVENT_EXPORT_ZIP_IS_TAKING_LONGTIME;
            break;
        }

        // this raise event code will be informed to GUI, that Export is failed
        Global::EventObject::Instance().RaiseEvent(EVENT_EXPORT_FAILED);

        if (EventCode != EVENT_EXPORT_FAILED)
        {
            // raise the event code
            Global::EventObject::Instance().RaiseEvent(EventCode);
        }
    }

    emit ExportFinish(ExportFailed);

    m_ExportProcessIsFinished = true;

    // enable the timer slot to destroy the objects after one second
    QTimer::singleShot(1000, this, SLOT(RemoveAndDestroyObjects()));
}

void ServiceMasterThreadController::ImportExportThreadFinished(
        const bool IsImport,
        const QString &TypeOfImport,
        bool UpdatedCurrentLanguage,
        bool NewLanguageAdded)
{
    if (IsImport)
    {
        bool ImportFail = true;

        // check the type of Import
        if (TypeOfImport.compare("Service") == 0)
        {
            // inform the event handler
            Global::EventObject::Instance().RaiseEvent(EVENT_IMPORT_SUCCESS);

            ImportFail = false;
        }
        else if(TypeOfImport == "Language")
        {
            // inform the event handler
            Global::EventObject::Instance().RaiseEvent(
                        EVENT_IMPORT_SUCCESS_LANGUAGE);

            ImportFail = false;
        }

        emit ImportFinish(ImportFail);
    }
    else
    {
        emit ExportFinish(true);
    }

    // clear the thread
    mp_ImportExportController->CleanupAndDestroyObjects();
    // block all the signals
    mp_ImportExportThread->blockSignals(true);
    mp_ImportExportThread->quit();

    // enable the timer slot to destroy the objects after one second
    QTimer::singleShot(1000, this, SLOT(RemoveAndDestroyObjects()));
}

void ServiceMasterThreadController::RemoveAndDestroyObjects(void)
{
    // this is for safety
    if (m_ExportProcessIsFinished || m_ImportExportThreadIsRunning)
    {
        // remove and destroy the controller thread
        if (0 != mp_ExportController)
        {
            delete mp_ExportController;
            mp_ExportController = 0;
        }

        if (0 != mp_ExportThread)
        {
            delete mp_ExportThread;
            mp_ExportThread = 0;
        }
        // remove and destroy the controller thread
        if (0 != mp_ImportExportController)
        {
            delete mp_ImportExportController;
            mp_ImportExportController = 0;
        }

        if (0 != mp_ImportExportThread)
        {
            delete mp_ImportExportThread;
            mp_ImportExportThread = 0;
        }

        // reset the falgs, so nothing is executing
        m_ExportProcessIsFinished = false;
        m_ImportExportThreadIsRunning = false;

    }
}

#if 0
/****************************************************************************/
void ServiceMasterThreadController::OnGetDataContainersCommand(Global::tRefType Ref, const DeviceCommandProcessor::CmdGetDataContainers &Cmd, Threads::CommandChannel &AckCommandChannel) {

    qDebug()<<"ServiceMasterThreadController::OnGetDataContainersCommand";

    if(NULL == mp_ServiceDataManager)
    {
        qDebug()<<"ServiceMasterThreadController::OnGetDataContainersCommand ERROR: mp_ServiceDataManager is NULL";
        return;
    }

    DataManager::CProcessSettings *ProcessSettings = mp_ServiceDataManager->GetProcessSettings();
    if(NULL == ProcessSettings)
    {
        qDebug()<<"ServiceMasterThreadController::OnGetDataContainersCommand ERROR: ProcessSettings is NULL";
        return;
    }
    SendCommand(Global::CommandShPtr_t(new DataManager::CmdGetProcessSettingsDataContainer(*ProcessSettings)), m_CommandChannelDeviceThread);
}
#endif


/****************************************************************************/
void ServiceMasterThreadController::OnReturnMessageCommand(Global::tRefType Ref, const DeviceCommandProcessor::CmdReturnMessage &Cmd, Threads::CommandChannel &AckCommandChannel)
{
//    qDebug()<<" ServiceMasterThreadController::OnReturnMessageCommand CmdType="<<Cmd.m_MessageType;

    SendAcknowledgeOK(Ref, AckCommandChannel);
    switch(Cmd.m_MessageType)
    {
    case Service::GUIMSGTYPE_ERROR:
        emit ReturnErrorMessagetoMain(Cmd.m_ReturnMessage);
        break;
    case Service::GUIMSGTYPE_INFO:
        emit returnMessageToGUI(Cmd.m_ReturnMessage);
        break;
    case Service::GUIMSGTYPE_CANID:
        //emit ReturnCANIDMessagetoMain(Cmd.m_ReturnMessage, Cmd.m_ReadCanID, Cmd.m_ExpectedCanID);
        break;
    case Service::GUIMSGTYPE_FIRMWAREINFO:
        //emit ReturnFirmwareMessagetoMain(Cmd.m_ReturnMessage, Cmd.m_FirwmwareInfo);
        break;
    case Service::GUIMSGTYPE_INITCALIBRATION:
        emit ReturnCalibrationInitMessagetoMain(Cmd.m_ReturnMessage, Cmd.m_CalibStatus);
        break;
    case Service::GUIMSGTYPE_HEATINGSTATUS:
        emit RefreshTestStatustoMain(Cmd.m_ReturnMessage, Cmd.m_Status);
        break;
    case Service::GUIMSGTYPE_MANUFMSGMODE:
        emit ReturnManufacturingMsgtoMain(Cmd.m_ModuleTestResult);
        break;
    default:
        emit returnMessageToGUI(Cmd.m_ReturnMessage);
        break;
    }
}


/****************************************************************************/
void ServiceMasterThreadController::sendAbortTestCommand()
{
    qDebug()<<"ServiceMasterThreadController::sendAbortTestCommand";
    SendCommand(Global::CommandShPtr_t(new DeviceCommandProcessor::CmdAbortTest(DEVICE_INSTANCE_ID_DEVPROC)),
                m_CommandChannelDeviceThread);
}

/****************************************************************************/
void ServiceMasterThreadController::sendRVHeatingTestCommand(quint8 HeaterIndex, quint8 CmdType)
{
    qDebug()<<"ServiceMasterThreadController::sendRVHeatingTestCommand";
    SendCommand(Global::CommandShPtr_t(new DeviceCommandProcessor::CmdHeatingTest(DEVICE_INSTANCE_ID_ROTARY_VALVE, HeaterIndex, CmdType)),
                m_CommandChannelDeviceThread);
}

/****************************************************************************/
void ServiceMasterThreadController::sendRotaryValveTestCommand(qint32 Position, quint8 CmdType)
{
    qDebug()<<"ServiceMasterThreadController::sendRVInitTestCommand";
    SendCommand(Global::CommandShPtr_t(new DeviceCommandProcessor::CmdRotaryValveTest(DEVICE_INSTANCE_ID_ROTARY_VALVE, Position, CmdType)),
                m_CommandChannelDeviceThread);
}

/****************************************************************************/
void ServiceMasterThreadController::sendLevelSensorDetectingTestCommand(qint32 Position)
{
    qDebug()<<"ServiceMasterThreadController::sendLevelSensorDetectingTestCommand";
    SendCommand(Global::CommandShPtr_t(new DeviceCommandProcessor::CmdLSensorDetectingTest(DEVICE_INSTANCE_ID_RETORT, Position)),
                m_CommandChannelDeviceThread);
}


/****************************************************************************/
void ServiceMasterThreadController::sendLevelSensorHeatingTestCommand(quint8 HeaterIndex, quint8 CmdType)
{
    qDebug()<<"ServiceMasterThreadController::sendLevelSensorHeatingTestCommand";
    SendCommand(Global::CommandShPtr_t(new DeviceCommandProcessor::CmdHeatingTest(DEVICE_INSTANCE_ID_AIR_LIQUID, HeaterIndex, CmdType)),
                m_CommandChannelDeviceThread);
}

/****************************************************************************/
void ServiceMasterThreadController::sendRetortHeatingTestCommand(quint8 HeaterIndex, quint8 CmdType)
{
    qDebug()<<"ServiceMasterThreadController::sendRetortHeatingTestCommand";
    SendCommand(Global::CommandShPtr_t(new DeviceCommandProcessor::CmdHeatingTest(DEVICE_INSTANCE_ID_RETORT, HeaterIndex, CmdType)),
                m_CommandChannelDeviceThread);
}

/****************************************************************************/
void ServiceMasterThreadController::sendTubeHeatingTestCommand(quint8 HeaterIndex, quint8 CmdType)
{
    qDebug()<<"ServiceMasterThreadController::sendTubeHeatingTestCommand";
    SendCommand(Global::CommandShPtr_t(new DeviceCommandProcessor::CmdHeatingTest(DEVICE_INSTANCE_ID_AIR_LIQUID, HeaterIndex, CmdType)),
                m_CommandChannelDeviceThread);
}

/****************************************************************************/
void ServiceMasterThreadController::sendOvenHeatingTestCommand(quint8 HeaterIndex, quint8 CmdType)
{
    qDebug()<<"ServiceMasterThreadController::sendOvenHeatingTestCommand";
    SendCommand(Global::CommandShPtr_t(new DeviceCommandProcessor::CmdHeatingTest(DEVICE_INSTANCE_ID_OVEN, HeaterIndex, CmdType)),
                m_CommandChannelDeviceThread);
}

/****************************************************************************/
void ServiceMasterThreadController::sendOvenLidInitCalibrationCommand()
{
    qDebug()<<"ServiceMasterThreadController::sendOvenLidInitCalibrationCommand";
    (void) SendCommand(Global::CommandShPtr_t(new DeviceCommandProcessor::CmdCalibrateDevice(Service::DEVICE_OVENLID_INIT)), m_CommandChannelDeviceThread);
}

/****************************************************************************/
void ServiceMasterThreadController::sendPressureSensorCalibrationCommand()
{
    qDebug()<<"ServiceMasterThreadController::sendPressureSensorCalibrationCommand";
    (void) SendCommand(Global::CommandShPtr_t(new DeviceCommandProcessor::CmdCalibrateDevice(Service::DEVICE_PRESSURE_SENSOR)), m_CommandChannelDeviceThread);
}


/****************************************************************************/
void ServiceMasterThreadController::sendManufacturingTestCommand(Service::ModuleTestCaseID Test, Service::ModuleTestCaseID AbortTestCaseId)
{
    qDebug()<<"ServiceMasterThreadController::sendManufacturingTestCommand -- modulename="<<Test;
    (void) SendCommand(Global::CommandShPtr_t(new DeviceCommandProcessor::CmdModuleManufacturingTest(Test, AbortTestCaseId)), m_CommandChannelDeviceThread);

}

void ServiceMasterThreadController::ShutdownSystem()
{
    qDebug()<<"ServiceMasterThreadController::ShutdownSystem ----------------- ";

    sendManufacturingTestCommand(Service::SYSTEM_SHUTDOWN);

    QTimer timer;
    timer.setSingleShot(true);
    timer.setInterval(4000);
    timer.start();
    QEventLoop loop;
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    loop.exec();

    QString RebootPath =  Global::SystemPaths::Instance().GetSettingsPath() + "/BootConfig.txt";
    QFile BootConfigFile(RebootPath);

    m_BootConfigFileContent.clear();
    if (BootConfigFile.exists()) {
        ReadBootConfigFile(&BootConfigFile);
    }

    m_BootConfigFileContent.insert("Start_Process", "DisplayPowerOffImage");

    UpdateRebootFile(m_BootConfigFileContent);

#if 1  // disabled for test
    const QString MD5sumGenerator = QString("%1%2").arg(Global::SystemPaths::Instance().GetScriptsPath()).
                                    arg(QString("/EBox-Utils.sh update_md5sum_for_settings"));
    (void)system(MD5sumGenerator.toStdString().c_str());


    std::cout <<"\n\n Shutdown Start time " << Global::AdjustedTime::Instance().GetCurrentTime().toString().toStdString();
    Global::EventObject::Instance().RaiseEvent(Global::EVENT_GLOBAL_STRING_TERMINATING, Global::tTranslatableStringList() <<"");
    //write buffered data to disk-> refer man pages for sync
    (void)system("sync &");
    (void)system("lcd off");
#endif

    qDebug()<<"emit SendStop ----";

    // send Stop signal to all thread controllers
    emit SendStop();

    qDebug()<<"WaitforThreads.........";
    // stop all threads and wait for the
    WaitForThreads();

    qDebug()<<"CleanupControllers..........";
    // Cleanup controllers
    CleanupControllers();

    qDebug()<<"DestroyControllerAndThreads ..........";
    // destroy controllers and threads
    DestroyControllersAndThreads();

    // call own Stop method
    //    Stop();
    for (qint32 I = 0; I < m_BroadcastChannels.count(); I++ ) {
        Threads::CommandChannel *p_Channel = m_BroadcastChannels.at(I);
        p_Channel->setParent(NULL);
    }

    this->thread()->quit();

//    qDebug() <<"Shutdown End time " << Global::AdjustedTime::Instance().GetCurrentTime().toString();

}

void ServiceMasterThreadController::ReadBootConfigFile(QFile *p_BootConfigFile) {
    if (p_BootConfigFile) {
        if(!p_BootConfigFile->open(QIODevice::ReadWrite | QIODevice::Text)) {
            //!< todo raise event.
            qDebug()<<"Reboot file open failed";
        }
        QString Line;
        QTextStream BootConfigFileStream(p_BootConfigFile);
        do {
            Line = BootConfigFileStream.readLine().simplified();
            QString RebootCount("0");
            if (Line.contains("Main_Rebooted", Qt::CaseInsensitive)) {
                QStringList LineFields = Line.split(":", QString::SkipEmptyParts);

                m_BootConfigFileContent.insert("Main_Rebooted", LineFields[1]);
            }
            else if (Line.contains("Reboot_Count", Qt::CaseInsensitive)) {
                QStringList LineFields = Line.split(":", QString::SkipEmptyParts);
                if (LineFields.count() == 2) {
                    RebootCount = LineFields[1];
                    m_RebootCount = RebootCount.toUInt();
                    m_BootConfigFileContent.insert("Reboot_Count", QString::number(m_RebootCount));
                }

            }
            else if (Line.contains("Software_Update_Status", Qt::CaseInsensitive)) {
                QStringList LineFields = Line.split(":", QString::SkipEmptyParts);


                m_BootConfigFileContent.insert("Software_Update_Status", "NA");

            }
            else if (Line.contains("PowerFailed", Qt::CaseInsensitive)) {
                QStringList LineFields = Line.split(":", QString::SkipEmptyParts);

                m_BootConfigFileContent.insert("PowerFailed", LineFields[1]);
            }
        } while (!Line.isNull());

        p_BootConfigFile->close();
    }
}

void ServiceMasterThreadController::UpdateRebootFile(const QMap<QString, QString> RebootFileContent)
{
    const QString RebootPath = Global::SystemPaths::Instance().GetSettingsPath() + "/BootConfig.txt";
    QFile RebootFile(RebootPath);
    if(!RebootFile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)) {
        //!< todo raise event.
        qDebug()<<"Reboot file open failed";
    }
    QTextStream RebootFileStream(&RebootFile);
    RebootFileStream.setFieldAlignment(QTextStream::AlignLeft);
    QMapIterator<QString, QString> RebootfileItr(RebootFileContent);
    while (RebootfileItr.hasNext()) {
        RebootfileItr.next();
        QString Key = RebootfileItr.key();
        QString Value = RebootFileContent.value(Key);
        RebootFileStream << Key << ":" << Value << "\n" << left;
    }
    RebootFile.flush();
    fsync(RebootFile.handle());
    RebootFile.close();
}

} // end namespace Threads
