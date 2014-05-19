/****************************************************************************/
/*! \file ServiceMasterThreadController.cpp
 *
 *  \brief Implementation file for class ServiceMasterThreadController.
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

#include <Core/Include/CMessageString.h>
#include <ExportController/Include/ExportController.h>
#include <ServiceMasterThreadController/Include/ServiceMasterThreadController.h>
#include <ServiceDeviceController/Include/ServiceDeviceController.h>
#include <Threads/Include/ThreadsEventCodes.h>

#include <Global/Include/Utils.h>
#include <Global/Include/SystemPaths.h>
#include <Global/Include/EventTranslator.h>
#include <Global/Include/UITranslator.h>

#include <ServiceDataManager/Include/ServiceDataManager.h>
#include "Core/Include/Startup.h"

#include <DataLogging/Include/DataLoggingThreadController.h>
#include <EventHandler/Include/EventHandlerThreadController.h>
#include <GPIOManager/Include/GPIOThreadController.h>
#include <DeviceCommandProcessor/Include/Commands/CmdDeviceProcessingInit.h>
#include <GPIOManager/Commands/Include/CmdSoftSwitchLEDControl.h>


#include <ServiceDeviceController/Include/Commands/CmdCalibrateDevice.h>


#include <DataManager/Helper/Include/XmlConfigFileStrings.h>
#include <DataManager/Helper/Include/DataManagerEventCodes.h>
//#include <DeviceControl/Include/Global/DeviceControl.h>
//#include <DataManager/Containers/Adjustment/Commands/Include/CmdGetAdjustmentDataContainer.h>

#include <DataManager/Containers/ExportConfiguration/Commands/Include/CmdDataExport.h>
#include <DataManager/Containers/ExportConfiguration/Commands/Include/CmdDataImport.h>
#include <DataManager/Containers/ExportConfiguration/Commands/Include/CmdDataImportFiles.h>

//#include <DataManager/Containers/Adjustment/Commands/Include/CmdGetAdjustmentDataContainer.h>
#include <DataManager/Helper/Include/XmlConfigFileTimeOffset.h>
#include <QMetaType>
#include <QSharedMemory>
#include <QDebug>
#include <QTimer>

namespace Threads {

static const unsigned long THREAD_WAIT_TIME = 2000;             ///< Time to wait when stopping thread.
static const QString TimeOffsetFileName = "TimeOffset.xml";     ///< Name of file in which time offset is stored.

static const CommandExecuteFunctorAckShPtr_t    NullCommandExecuteFunctor(NULL);    ///< NULL functor for command execution.
static const CommandExecuteFunctorShPtr_t       NullCommandExecuteFunctorWithouAck(NULL); ///< Null functor command execution

/****************************************************************************/
/**
 * \brief This class is the Master Thread controller for Service SW.
 * \iparam startUp = GUI start up object
 * It will handle the data logging and event handling threads and their controllers.
 * \warning This class is not thread safe!
 */
/****************************************************************************/
ServiceMasterThreadController::ServiceMasterThreadController(Core::CStartup *startUp) :
    BaseThreadController(1, "BaseThread"),
    m_EventLoggerMaxFileSize(0),
    m_DayEventLoggerMaxFileCount(0),
    m_MaxAdjustedTimeOffset(0),
    m_ExportTargetFileName(""),
    m_HeartBeatSourceDeviceHandler(120),  //! \todo Change the value -> Refer colorado project
    m_RebootCount(0),
    m_CommandChannelDataLogging(this, "DataLogging", Global::EVENTSOURCE_DATALOGGER),
    m_CommandChannelEventThread(this, "EventHandler", Global::EVENTSOURCE_EVENTHANDLER),
    m_CommandChannelDeviceThread(this,"DeviceController", Global::EVENTSOURCE_NONE),
    m_CommandChannelGPIOManager(this, "SoftSwitch", Global::EVENTSOURCE_NONE),
    m_CommandChannelExport(this, "Export", Global::EVENTSOURCE_EXPORT),
    mp_EventThreadController(NULL),
    mp_DeviceThreadController(NULL),
    mp_GPIOThreadController(NULL),
    mp_ServiceDataManager(NULL),
    mp_GUIStartup(startUp)
{
    Global::ToConsole("MasterThread: Constructor");

    // register the metytype for gSourceType
    qRegisterMetaType<Global::gSourceType>("Global::gSourceType");
    //qRegisterMetaType<Service::TemperatureData>("Service::TemperatureData");
    qRegisterMetaType<Service::ModuleNames>("Service::ModuleNames");


    CONNECTSIGNALSLOT(&m_ShutdownSharedMemTimer, timeout(), this, ExternalMemShutdownCheck());

    m_ShutdownSharedMemTimer.setInterval(500);
    m_ShutdownSharedMemTimer.setSingleShot(false);

#if defined(QT_NO_DEBUG)
    // we are in release mode so we do not start timer
#else
    // we are in debug mode so we can start timer
    m_ShutdownSharedMemTimer.start();
#endif
}

/****************************************************************************/
/**
 * \brief Destructor
 */
/****************************************************************************/
ServiceMasterThreadController::~ServiceMasterThreadController() {
    try {
        mp_EventThreadController = NULL;
        m_TCCommandRoutes.clear();
    } catch(...) {
    }
}

/****************************************************************************/
/**
 * \brief Create and initialize used objects.
 *
 * We try to read the hardware configuration file.
 */
/****************************************************************************/
void ServiceMasterThreadController::CreateAndInitializeObjects() {

    //CHECKPTR(mp_DataManagerBase);
//    qDebug()<<"ServiceMasterThreadController::CreateAndInitializeObjects() Thread EventObject" << Global::EventObject::Instance().thread();
    //qDebug()<<" thread StateHandler" << EventHandler::StateHandler::Instance().thread();
    ///Update serial number read from Device configuration xml
    //Serial number will be present in Log files.

    mp_DataLoggingThreadController->SetOperatingMode(m_OperatingMode);
    mp_DataLoggingThreadController->SetEventLoggerBaseFileName(m_EventLoggerBaseFileName);
    mp_DataLoggingThreadController->SetSerialNumber(m_SerialNumber);
    mp_DataLoggingThreadController->SetEventLoggerMaxFileSize(m_EventLoggerMaxFileSize);
    mp_DataLoggingThreadController->SetDayEventLoggerMaxFileCount(m_DayEventLoggerMaxFileCount);

    // read event strings. language and fallback language is English
    ReadEventTranslations(QLocale::English, QLocale::English);

    ServiceMasterThreadController::RegisterCommands();

    //mp_GPIOThreadController->SkipSoftSwitchAtBoot(false);
}

/****************************************************************************/
/**
 * \brief Register commands.
 *
 * Must be implemented in derived classes.
 */
/****************************************************************************/
void ServiceMasterThreadController::RegisterCommands() {

    Global::ToConsole("MasterThread: Register commands");
    qDebug()<<"ServiceMasterThreadController::RegisterCommands Registering Commands for MasterThreadController";


    RegisterAcknowledgeForProcessing<Global::AckOKNOK, ServiceMasterThreadController>
            (&ServiceMasterThreadController::OnAckOKNOK, this);
}

/****************************************************************************/

/**
 * \brief Acknowledge of type AckOKNOK received.
 *
 * \iparam   Ref     Command reference.
 * \iparam   Ack     Acknowledge.
 */
/****************************************************************************/
void ServiceMasterThreadController::OnAckOKNOK(Global::tRefType Ref, const Global::AckOKNOK &Ack) {

    Q_UNUSED(Ref);
    Q_UNUSED(Ack);
    qDebug()<<"ServiceMasterThreadController::OnAckOKNOK";
}

/****************************************************************************/
/**
 * \brief Cleanup and destroy used objects.
 */
/****************************************************************************/
void ServiceMasterThreadController::CleanupAndDestroyObjects() {
    // nothing to do
}

/****************************************************************************/
/**
 * \brief Connect data logging signals.
 *
 * We connect the data logging signals (EventObject) of a controller
 * to the event handler controller (EmitEventEntry) and to the
 * data logger controller (EmitDayOperationEntry and EmitComponentTestEntry)
 *
 * \iparam   pController     Pointer to controller.
 */
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
/**
 * \brief Add controller and thread to list of handled controllers.
 *
 * This list is used to perform common stuff to all controllers and threads:
 * connecting their Go and Stop slots, connecting them to the
 * data logging mechanism and so on.
 *
 * \iparam   pController         Pointer to controller.
 * \iparam   pCommandChannel     Pointer to command channel.
 */
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

    // connect heartbeat signals
    //CONNECTSIGNALSLOT(pController, HeartbeatSignal(const Global::gSourceType &), this, HeartbeatSlot(const Global::gSourceType &));

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
/**
 * \brief Create controllers and threads handled by us.
 *
 * We create all controllers handled by us
 * and connect them.
 *
 * \note One can create thread controllers which are platform in nature over
 *        here
 */
/****************************************************************************/
void ServiceMasterThreadController::CreateControllersAndThreads() {
    // now create new objects common to all master threads
}

/****************************************************************************/
/**
 * \brief Create all controllers and threads handled by us
 *
 * We create all controllers handled by us (data logging and event handler)
 * and connect them.
 */
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
    FileName.append(Global::SystemPaths::Instance().GetSettingsPath()+QDir::separator()+"EventConfigService.csv");
    mp_EventThreadController = new EventHandler::EventHandlerThreadController(m_HeartBeatSourceEventHandler, m_RebootCount, FileName);

    mp_EventThreadController->ConnectToEventObject();
    mp_DataLoggingThreadController->EnableImmediateLogging(true);


    // create system's Device handler
  //  mp_DeviceThreadController = new DeviceControl::ServiceDeviceController(m_HeartBeatSourceDeviceHandler);

    // create GPIO thread controller for Softswitch tests in Service mode
    mp_GPIOThreadController = new GPIOManager::GPIOThreadController(m_ThreadIDGPIOManager, true);

    mp_DeviceThreadController = new DeviceControl::ServiceDeviceController(m_HeartBeatSourceDeviceHandler);
    //***********************************************
    //This will connect EventHandler to DataLogger.
    //************************************************
    CONNECTSIGNALSLOT(mp_EventThreadController, LogEventEntry(const DataLogging::DayEventEntry &),
                      mp_DataLoggingThreadController, SendToDayEventLogger(const DataLogging::DayEventEntry &));
    // this will check whether logging is enabled or not and the same can inform to GUI
    CONNECTSIGNALSLOT(this, CheckLoggingEnabled(),
                      mp_DataLoggingThreadController, CheckLoggingEnabled());

    //EventHandler shall have knowledge of alarm handler to initiate alarms
    //mp_EventThreadController->SetAlarmHandler(mp_alarmHandler);

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
    try
    {
        AddAndConnectController(mp_EventThreadController, &m_CommandChannelEventThread,
                                static_cast<int>(EVENT_HANDLER_THREAD), true);

    }
    catch(...)
    {
        // m_pEventThreadController was not added properly so delete all allocated stuff yourself!
        // delete m_pEventThreadController
        delete mp_EventThreadController;
        mp_EventThreadController = NULL;
        // rethrow exception
        throw;
    }

    // connect all common signals and slots of the GUI Handler and create its thread:
    try
    {
        AddAndConnectController(mp_DeviceThreadController, &m_CommandChannelDeviceThread,
                                static_cast<int>(DEVICE_HANDLER_THREAD), true);

    }
    catch(...)
    {
        // mp_GUIThreadController was not added properly so delete all allocated stuff yourself!
        // delete m_pEventThreadController
        delete mp_EventThreadController;
        mp_EventThreadController = NULL;
        delete mp_DeviceThreadController;
        mp_DeviceThreadController = NULL;
        // rethrow exception
        throw;
    }

    try
    {
        AddAndConnectController(mp_GPIOThreadController, &m_CommandChannelGPIOManager,
                                static_cast<int>(GPIO_HANDLER_THREAD), true);
    }
    catch(...)
    {
        // mp_GPIOThreadController was not added properly so delete all allocated stuff yourself!
        // delete m_pEventThreadController
        delete mp_GPIOThreadController;
        mp_GPIOThreadController = NULL;
        // rethrow exception
        throw;
    }

}

/****************************************************************************/
/**
 * \brief Destroy all registered controllers and threads.
 * \iparam BasicThreadController
 */
/****************************************************************************/
void ServiceMasterThreadController::DestroyControllersAndThreads(const bool BasicThreadController)
{
    Q_UNUSED(BasicThreadController);
}

/****************************************************************************/
/**
 * \brief Initiate the shutdown process.
 *
 * Initiate the shutdown process. Make some project specific tasks the call
 * Shutdown to shut down software.
 * Must be implemented in derived classes.
 */
/****************************************************************************/
void ServiceMasterThreadController::InitiateShutdown(){
}

/****************************************************************************/
/**
 * \brief Create GUI controllers, which inturn starts GUI process .
 *
 * \note All the XML files are sent to GUI.
 */
/****************************************************************************/
void ServiceMasterThreadController::InitializeGUI()
{
}

/****************************************************************************/
/**
 * \brief Initialize all controllers in the order they were created.
 *
 * Calls CreateAndInitializeObjects for each.
 */
/****************************************************************************/
void ServiceMasterThreadController::InitializeControllers(bool BasicThreadController)
{
   CreateAndInitializeObjects();
   tControllerMap Controllers;
   BasicThreadController ? (Controllers = m_BasicControllersMap) : (Controllers = m_ControllerMap);
    for(tControllerMap::iterator it = Controllers.begin(); it != Controllers.end(); ++it)
    {
        // Initialize controllers
        qDebug() << "ServiceMasterThreadController::InitializeControllers";

        it.value().first->CreateAndInitializeObjects();
    }
}

/****************************************************************************/
/**
 * \brief Cleanup all controllers in the reverse order they were initialized.
 *
 * Calls CleanupAndDestroyObjects for each controller.
 */
/****************************************************************************/
void ServiceMasterThreadController::CleanupControllers(bool BasicThreadController)
{
    tControllerMap Controllers;
    BasicThreadController ? (Controllers = m_BasicControllersMap) : (Controllers = m_ControllerMap);
    for(tControllerMap::iterator it = Controllers.end(); it != Controllers.begin();)
    {
        // go backwards
        --it;
        // cleanup object
        it.value().first->DoCleanupAndDestroyObjects();
    }
}

/****************************************************************************/
/**
 * \brief Attach controllers to corresponding threads and start threads.
 */
/****************************************************************************/
void ServiceMasterThreadController::AttachControllersAndStartThreads(bool BasicThreadController)
{
    tControllerMap Controllers;
    qDebug() << "Controllers" << Controllers.count() << endl;
    BasicThreadController ? (Controllers = m_BasicControllersMap) : (Controllers = m_ControllerMap);
    for(tControllerMap::iterator it = Controllers.begin(); it != Controllers.end(); ++it)
    {

        qDebug() << it.value() << endl;
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
    qDebug() << "start heartbeat check timer only if m_HeartbeatCheckTimeout > 0" << endl;
    // start heartbeat check timer only if m_HeartbeatCheckTimeout > 0
   if(m_HeartbeatCheckTimeout > 0)
   {
       m_HeartbeatCheckTimer.setInterval(m_HeartbeatCheckTimeout);
       m_HeartbeatCheckTimer.setSingleShot(false);
       m_HeartbeatCheckTimer.start();
   }
   else
   {
//       LOG_EVENT(Global::EVTTYPE_INFO, Global::LOG_ENABLED, EVENT_THREADS_INFO_NO_HEARTBEAT_CHECKING, Global::tTranslatableStringList()
//                 , Global::NO_NUMERIC_DATA, false);
   }

   qDebug() << BasicThreadController << endl;
    // and send them the Go signal
   BasicThreadController ? (emit SendGoToBasicThreads()): (emit SendGo());
}


/****************************************************************************/
/**
 * \brief Wait for thread termination.
 *
 * We wait for a specific time amount THREAD_WAIT_TIME. If the thread does not
 * terminate we throw an exception
 */
/****************************************************************************/
void ServiceMasterThreadController::WaitForThreads(bool BasicThreadController)
{
    tControllerMap Controllers;
    BasicThreadController ? (Controllers = m_BasicControllersMap) : (Controllers = m_ControllerMap);
    // stop heartbeat check timer
    m_HeartbeatCheckTimer.stop();
    for(tControllerMap::iterator it = Controllers.begin(); it !=  Controllers.end(); ++it)
    {
        // and wait for threads
        if(it.value().second != NULL) {
            if(!it.value().second->wait(THREAD_WAIT_TIME)){
                // thread did not quit within time
                //Global::TranslatableString SourceString(it->first->GetHeartBeatSource());
                Global::EventObject::Instance().RaiseEvent(EVENT_THREADS_ERROR_THREAD_STOP_ARGS,
                                                           Global::FmtArgs() << it->first->GetThreadID() << QString::number(THREAD_WAIT_TIME, 10));
                /*
                Global::ToConsole(QString("Thread %1 did not finish within %2ms").
                                  arg(it->first->GetHeartBeatSource()).
                                  arg(THREAD_WAIT_TIME));*/
            }
        }
    }
}

/****************************************************************************/
/**
 * \brief Register a command execution functor.
 *
 * \iparam   CommandName     Name of command.
 * \iparam   Functor         Shared pointer of functor to register.
 */
/****************************************************************************/
void ServiceMasterThreadController::RegisterCommandExecuteFunctor(const QString &CommandName, const CommandExecuteFunctorAckShPtr_t &Functor)
{
    // check if already registered
    if(m_CommandExecuteFunctors.contains(CommandName))
    {
        LOGANDTHROWARGS(EVENT_THREADS_ERROR_COMMAND_FUNCTOR_ALREADY_REGISTERED, CommandName);
    }
    // everything OK
    static_cast<void>(
                // we DO NOT NEED the return value of insert
                m_CommandExecuteFunctors.insert(CommandName, Functor)
                );
}

/****************************************************************************/
/**
 * \brief Register a command execution functor without Ack.
 *
 * \iparam   CommandName     Name of command.
 * \iparam   Functor         Shared pointer of functor to register.
 */
/****************************************************************************/
void ServiceMasterThreadController::RegisterCommandExecuteFunctor(const QString &CommandName, const CommandExecuteFunctorShPtr_t &Functor)
{
    // check if already registered
    if(m_CommandExecuteWithoutAckFunctors.contains(CommandName))
    {
        LOGANDTHROWARGS(EVENT_THREADS_ERROR_COMMAND_FUNCTOR_ALREADY_REGISTERED, CommandName);
    }
    // everything OK
    static_cast<void>(
                // we DO NOT NEED the return value of insert
                m_CommandExecuteWithoutAckFunctors.insert(CommandName, Functor)
                );
}

/****************************************************************************/
/**
 * \brief Get command execute functor by name.
 *
 * Get command execute functor by name. If functor is not found
 * NullCommandExecuteFunctor will be returned.
 *
 * \iparam   CommandName     Name of command.
 * \return                      The functor or NullCommandExecuteFunctor.
 */
/****************************************************************************/
CommandExecuteFunctorAckShPtr_t ServiceMasterThreadController::GetCommandExecuteFunctor(const QString &CommandName) const
{
    CommandExecuteFunctorAckHash_t::const_iterator it = m_CommandExecuteFunctors.find(CommandName);
    if(it == m_CommandExecuteFunctors.constEnd())
    {
        // functor not found
        // return NULL functor
        return NullCommandExecuteFunctor;
    }
    // return functor
    return it.value();
}

/****************************************************************************/
/**
 * \brief Get command execute functor by name.
 *
 * Get command execute functor by name. If functor is not found
 * NullCommandExecuteFunctor will be returned.
 *
 * \iparam   CommandName     Name of command.
 * \return                      The functor or NullCommandExecuteFunctor.
 */
/****************************************************************************/
CommandExecuteFunctorShPtr_t ServiceMasterThreadController::GetCommandExecuteFunctorWithoutAck(const QString &CommandName) const
{
    CommandExecuteFunctorHash_t::const_iterator it = m_CommandExecuteWithoutAckFunctors.find(CommandName);
    if(it == m_CommandExecuteWithoutAckFunctors.constEnd())
    {
        // functor not found
        // return NULL functor
        return NullCommandExecuteFunctorWithouAck;
    }
    // return functor
    return it.value();
}

/****************************************************************************/
/**
 * \brief Register a command route functor.
 *
 * \iparam   CommandName             Name of command.
 * \iparam   pTargetCommandChannel   The target command channel.
 */
/****************************************************************************/
void ServiceMasterThreadController::RegisterCommandRoutingChannel(const QString &CommandName, CommandChannel *pTargetCommandChannel)
{
    CHECKPTR(pTargetCommandChannel);
    // check if already registered
    if(m_TCCommandRoutes.contains(CommandName))
    {
        LOGANDTHROWARGS(EVENT_THREADS_ERROR_COMMAND_FUNCTOR_ALREADY_REGISTERED, CommandName);
    }
    // everything OK
    static_cast<void>(
                // we DO NOT NEED the return value of insert
                m_TCCommandRoutes.insert(CommandName, pTargetCommandChannel)
                );
}

/****************************************************************************/
/**
 * \brief Get command channel for routing by command name.
 *
 * Returns NULL if functor not found.
 * \iparam   CommandName     Name of command.
 * \return                      The command channels.
 */
/****************************************************************************/
CommandChannel *ServiceMasterThreadController::GetCommandRouteChannel(const QString &CommandName) const
{

    if(!m_TCCommandRoutes.contains(CommandName))
    {
        // functor not found
        return NULL;
    }
    // return command channel
    return m_TCCommandRoutes.value(CommandName);
}

/****************************************************************************/
/**
 * \brief Get command channel for routing by component type.
 *
 * Returns NULL if functor not found.
 * \iparam   component     Type of component.
 * \return                      The command channels.
 */
/****************************************************************************/
CommandChannel *ServiceMasterThreadController::GetComponentRouteChannel(Global::EventSourceType component) const
{
    QHashIterator<QString, Threads::CommandChannel*> i(m_channelList);
    while (i.hasNext())
    {
        (void) i.next();
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
/**
 * \brief Thread command processing method.
 *
 * Is called when an command has to be processed.
 * \warning This method should be called only from within CommandChannel::CommandChannelRx
 * \warning Do not let exceptions escape this method!
 *
 * \iparam       Ref                 The command reference.
 * \iparam       Cmd                 The command.
 * \param[in, out]  AckCommandChannel   The command channel for acknowledges.
 */
/****************************************************************************/
void ServiceMasterThreadController::OnExecuteCommand(Global::tRefType Ref, const Global::CommandShPtr_t &Cmd, Threads::CommandChannel &AckCommandChannel)
{
    try
    {
        qDebug() << "MasterThreadController::OnExecuteCommand" << Ref << Cmd.GetPointerToUserData()->GetName();

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
            if(Cmd->GetTimeout() != Global::Command::NOTIMEOUT)
            {
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
            if (Functor == NullCommandExecuteFunctorWithouAck)
            {
                // and send command
                DoSendCommand(NewRef, Cmd, *pChannel);
            }
            else
            {
                //We need to process before sending command to the target channel
                Functor.GetPointerToUserData()->Execute(Ref, Cmd.GetPointerToUserData());
                // and send command
                DoSendCommand(NewRef, Cmd, *pChannel);
            }

        }
        else
        {
            // no routing, try to execute
            // Get functor and execute
            CommandExecuteFunctorAckShPtr_t Functor = GetCommandExecuteFunctor(Cmd->GetName());
            if(Functor == NullCommandExecuteFunctor)
            {
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
/**
 * \brief An acknowledge for an send command was received.
 *
 * Is called when an command acknowledge has to be processed.
 * \warning This method should be called only from within CommandChannel::CommandChannelTxAck
 * \warning Do not let exceptions escape this method!
 *
 * \iparam       Ref         The command reference.
 * \iparam       Ack         The received acknowledge.
 */
/****************************************************************************/
void ServiceMasterThreadController::OnProcessAcknowledge(Global::tRefType Ref, const Global::AcknowledgeShPtr_t &Ack)
{
//    qDebug() << "MasterThreadController::OnProcessAcknowledge" << Ref << Ack.GetPointerToUserData()->GetName();

    try
    {
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
        }
        else
        {
            // try to process acknoledge
            BaseThreadController::OnProcessAcknowledge(Ref, Ack);
        }
    }
    CATCHALL();
}

/****************************************************************************/
/**
 * \brief An timeout for an send command was received.
 *
 * Is called when an command timeout has to be processed.
 *
 * \iparam       Ref         The command reference.
 * \iparam       CmdName     Name of command.
 */
/****************************************************************************/
void ServiceMasterThreadController::OnProcessTimeout(Global::tRefType Ref, const QString &CmdName)
{
//    SEND_DEBUG(WHEREAMI + " " +
//               QString("Ref = ") + QString::number(Ref, 10) +
//               QString("Name = ") + CmdName);
    if(m_TCAcknowledgeRoutes.contains(Ref))
    {
        // send error
        Global::EventObject::Instance().RaiseEvent(EVENT_THREADS_ERROR_COMMAND_TIMEOUT, Global::FmtArgs() << CmdName << Ref, true);

//        LOG_EVENT(Global::EVTTYPE_FATAL_ERROR, Global::LOG_ENABLED, EVENT_THREADS_ERROR_COMMAND_TIMEOUT, Global::tTranslatableStringList() << CmdName << QString::number(Ref, 10)
//                  , Global::NO_NUMERIC_DATA ,false);
        // remove from list of pending commands
        RemoveFromPendingCommands(Ref);
    }
    else
    {
        // try to process timeout
        BaseThreadController::OnProcessTimeout(Ref, CmdName);
    }
}

/****************************************************************************/
/**
 * \brief Broadcast a command to all controllers.
 *
 * \iparam       Cmd         The command.
 */
/****************************************************************************/
void ServiceMasterThreadController::BroadcastCommand(const Global::CommandShPtr_t &Cmd)
{
    // send command to all connected thread controllers.
    for(tCommandChannelVector::iterator it = m_BroadcastChannels.begin(); it != m_BroadcastChannels.end(); ++it)
    {
        // get new ref
        Global::tRefType Ref = GetNewCommandRef();
        // and send command to that channel
        DoSendCommand(Ref, Cmd, *(*it));
    }
}

/****************************************************************************/
/**
 * \brief Send a command over a specific command channel.
 *
 *
 * \iparam   Cmd         The command.
 * \iparam   CmdChannel  The command channel for the command
 * \return                  The command reference.
 */
/****************************************************************************/
Global::tRefType ServiceMasterThreadController::SendCommand(const Global::CommandShPtr_t &Cmd, CommandChannel &CmdChannel)
{
    // get new command ref
    Global::tRefType Ref = GetNewCommandRef();
    // and send command to that channel
    DoSendCommand(Ref, Cmd, CmdChannel);
    // return computed Ref
    return Ref;
}

/****************************************************************************/
/**
 * \brief Send an acknowledge over a specific command channel.
 *
 * \iparam   Ref         The acknowledge reference.
 * \iparam   Ack         The acknowledge.
 * \iparam   CmdChannel  The command channel for the command
 */
/****************************************************************************/
void ServiceMasterThreadController::SendAcknowledge(Global::tRefType Ref, const Global::AcknowledgeShPtr_t &Ack, CommandChannel &CmdChannel)
{

    // send
    DoSendAcknowledge(Ref, Ack, CmdChannel);
}

/****************************************************************************/
/**
 * \brief Send a positive acknowledge over its command channel.
 *
 * Create a positive acknwoledge of type Global::AckOKNOK and send it.
 *
 * \iparam   Ref         Acknowledge reference.
 * \iparam   CmdChannel  The command channel for the command
 */
/****************************************************************************/
void ServiceMasterThreadController::SendAcknowledgeOK(Global::tRefType Ref, CommandChannel &CmdChannel)
{
    // create acknowledge and send
    SendAcknowledge(Ref, Global::AcknowledgeShPtr_t(new Global::AckOKNOK(true)), CmdChannel);
}

/****************************************************************************/
/**
 * \brief Send a negative acknowledge over its command channel.
 *
 * Create a negative acknwoledge of type Global::AckOKNOK and send it.
 *
 * \iparam   Ref         Acknowledge reference.
 * \iparam   CmdChannel  The command channel for the command
 * \iparam   Text        Text of message.
 * \iparam   Type        Type of message.
 */
/****************************************************************************/
void ServiceMasterThreadController::SendAcknowledgeNOK(Global::tRefType Ref, CommandChannel &CmdChannel, const QString &Text, Global::GUIMessageType Type)
{
    // create acknowledge and send
    SendAcknowledge(Ref, Global::AcknowledgeShPtr_t(new Global::AckOKNOK(false, Text, Type)), CmdChannel);
}

/****************************************************************************/
/**
 * \brief Read set of needed event string translations.
 *
 * These consist of the language resulting from current operation mode and the
 * fallback language.
 *
 * \iparam   Language            The desired language.
 * \iparam   FallbackLanguage    The desired fallback language.
 */
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

    mp_ServiceDataManager = new DataManager::CServiceDataManager(this);

    mp_ServiceDataContainer = const_cast<DataManager::ServiceDataContainer *>(mp_ServiceDataManager->GetDataContainer());

//    mp_GUIStartup->mp_ServiceConnector->SetModuleListContainer(mp_ServiceDataContainer->ModuleList, mp_ServiceDataContainer->ModuleListArchive);
//    mp_GUIStartup->mp_ServiceConnector->SetServiceParametersContainer(mp_ServiceDataContainer->ServiceParameters);
    mp_GUIStartup->mp_ServiceConnector->SetDeviceConfigurationInterface(mp_ServiceDataContainer->DeviceConfigurationInterface);

    CreateBasicControllersAndThreads();
    // all objects created from now on, live in this thread so they are served by the
    // signal/slot mechanism in this event loop.
    try
    {
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
void ServiceMasterThreadController::OnStopReceived()
{
    // Cleanup controllers
    CleanupControllers();
    CleanupControllers(true);

    // destroy controllers and threads
    DestroyControllersAndThreads();
    DestroyControllersAndThreads(true);
}

/****************************************************************************/
void ServiceMasterThreadController::Shutdown()
{
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
void ServiceMasterThreadController::HeartbeatCheck()
{
//    qDebug() << "MasterThreadController::HeartbeatCheck";

}

/****************************************************************************/
void ServiceMasterThreadController::HeartbeatSlot(const Global::gSourceType &TheHeartBeatSource)
{
    // remember received heartbeat logging source.
    m_ArrivedHeartbeats.insert(TheHeartBeatSource);
}

/****************************************************************************/
void ServiceMasterThreadController::StopSpecificThreadController(const int ControllerNumber, const bool BasicThreadController)
{

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
        (void) ControllerPair.second->blockSignals(true);
        ControllerPair.second->quit();
    }
}

/****************************************************************************/
void ServiceMasterThreadController::RemoveSpecificThreadController(const int ControllerNumber, const bool BasicThreadController)
{
    bool ThreadFound = false;
    tControllerPair ControllerPair;
    if (!BasicThreadController)
    {
        if (m_ControllerMap.contains(ControllerNumber))
        {
            ControllerPair = m_ControllerMap.value(ControllerNumber);
            (void) m_ControllerMap.remove(ControllerNumber);
            ThreadFound = true;
        }
    }
    else
    {
        if (m_BasicControllersMap.contains(ControllerNumber))
        {
            ControllerPair = m_BasicControllersMap.value(ControllerNumber);
            (void) m_BasicControllersMap.remove(ControllerNumber);
            ThreadFound = true;
        }
    }
    if (ThreadFound)
    {
            // destroy all the objects
            delete ControllerPair.first;
            delete ControllerPair.second;
    }
}

/****************************************************************************/
void ServiceMasterThreadController::StartSpecificThreadController(const int ControllerNumber, const bool BasicThreadController) {
    bool ThreadFound = false;
    tControllerPair ControllerPair;
    if (!BasicThreadController)
    {
        if (m_ControllerMap.contains(ControllerNumber))
        {
            ControllerPair = m_ControllerMap.value(ControllerNumber);
            ThreadFound = true;
        }
    }
    else
    {
        if (m_BasicControllersMap.contains(ControllerNumber))
        {
            ControllerPair = m_BasicControllersMap.value(ControllerNumber);
            ThreadFound = true;
        }
    }
    if (ThreadFound)
    {
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
void ServiceMasterThreadController::ExternalMemShutdownCheck()
{
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
        if(ShutdownSharedMem.attach())
        {
            // attach OK
            if(ShutdownSharedMem.lock())
            {
                // read data
                unsigned char *pbData = reinterpret_cast<unsigned char *>(ShutdownSharedMem.data());
                if(pbData != NULL)
                {
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
    if(oTerminate)
    {
        InitiateShutdown();
    }
#endif
}


/****************************************************************************/
void ServiceMasterThreadController::OnPowerFail(const Global::PowerFailStages PowerFailStage)
{
    Q_UNUSED(PowerFailStage);
}


/****************************************************************************/
/**
 * \brief Send a data changed command.
 *
 * The command will be broadcasted to all connected thread controllers.
 *
 * \iparam   Cmd         The command.
 */
/****************************************************************************/
void ServiceMasterThreadController::DoSendDataChanged(const Global::CommandShPtr_t &Cmd)
{
    // process command to ensure local OnDataChanged methods are called
    const Global::CmdDataChanged *pCommand = dynamic_cast<Global::CmdDataChanged *>(Cmd.GetPointerToUserData());
    CHECKPTR(pCommand);
    DispatchDataChangedCommand(*pCommand);
    // broadcast command to all
    BroadcastCommand(Cmd);
}

/****************************************************************************/
/**
 * \brief Start statemachine.
 *
 * Start own statemachine and start to work.
 * Must be implemented in derived classes.
 */
/****************************************************************************/
void ServiceMasterThreadController::StartStatemachine()
{
    // Implementation of StartStateMachine
}

} // end namespace Threads
