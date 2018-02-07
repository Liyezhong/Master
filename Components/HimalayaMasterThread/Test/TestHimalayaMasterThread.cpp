#include <QString>
#include <QtTest>
#include <Main/Include/Main.h>
#include "HimalayaMasterThread/Include/HimalayaMasterThreadController.h"
#include <DataManager/Helper/Include/XmlConfigFileTimeOffset.h>
#include <DataManager/Containers/UserSettings/Include/UserSettingsInterface.h>

#include <DeviceCommandProcessor/Include/DeviceCommandProcessorEventCodes.h>

#include <ExportController/Include/ExportController.h>
//#include <Scheduler/Include/logging.h>
#include <Global/Include/SystemPaths.h>
#include <Global/Include/Utils.h>
#include <Global/Include/GlobalDefines.h>
#include <DeviceCommandProcessor/Include/Commands/CmdDeviceProcessingCleanup.h>
#include <NetCommands/Include/CmdScheduledStations.h>
#include <NetCommands/Include/CmdProcessState.h>
#include <NetCommands/Include/CmdExecutionStateChanged.h>
#include <NetCommands/Include/CmdDayRunLogRequest.h>

#include <NetCommands/Include/CmdGenerateBathLayout.h>
#include <NetCommands/Include/CmdProblemReportReply.h>
#include <NetCommands/Include/CmdEventReport.h>
//#include <HimalayaDataContainer/Containers/Stations/Commands/Include/CmdResetStationRms.h>
#include <DataManager/Containers/UserSettings/Commands/Include/CmdRmsOnOff.h>
#include <DataManager/Containers/UserSettings/Commands/Include/CmdSetWaterStations.h>

#include <DataManager/Containers/UserSettings/Commands/Include/CmdWaterStationUpdate.h>
#include <DataManager/Containers/Stations/Include/StationBase.h>
#include <NetCommands/Include/CmdExternalProcessState.h>

#include <DeviceCommandProcessor/Include/Commands/CmdDeviceProcessingInit.h>
#include <DeviceCommandProcessor/Include/Commands/CmdDeviceProcessingCleanup.h>
#include <NetCommands/Include/CmdRequestRack.h>
#include <NetCommands/Include/CmdRequestParameterSet.h>
#include <Global/Include/Translator.h>
#include <Global/Include/UITranslator.h>
#include <QLocale>

#include "DataManager/Containers/UserSettings/Include/UserSettingsVerifier.h"
#include "DataManager/Containers/DeviceConfiguration/Include/DeviceConfigurationInterface.h"
#include <Global/Include/EventObject.h>
#include <EventHandler/Include/StateHandler.h>

#include <HimalayaEventCodes.h>
#include <HimalayaProcessExitCodes.h>
#include <EventHandler/Include/EventHandlerEventCodes.h>
#include <NetCommands/Include/CmdCriticalActionCheck.h>
#include <DataManager/Containers/ExportConfiguration/Commands/Include/CmdDataExport.h>
#include <DataManager/Containers/ExportConfiguration/Commands/Include/CmdDataImport.h>

#include "DataManager/CommandInterface/Include/UserSettingsCommandInterface.h"
#include "HimalayaDataManager/CommandInterface/Include/ProgramCommandInterface.h"
#include "NetCommands/Include/CmdUserAction.h"

#include <DataLogging/Include/DayEventLogger.h>
#include "HimalayaDataManager/CommandInterface/Include/ReagentCommandInterface.h"
#include <SWUpdateManager/Include/SWUpdateManager.h>
#include <Global/Include/GlobalDefines.h>
#include <DataManager/Helper/Include/Helper.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h> //for system()

/// \todo Norbert's debug output
#include <DeviceControl/Include/Global/dcl_log.h>

#include <QThread>
#include <QString>
#include <QDebug>

#include <stdlib.h> //For "system()"
#include "Global/Include/SignalHandler.h"
#include "Global/Include/GlobalDefines.h"
#include "Global/Include/AlarmPlayer.h"
#include <NetCommands/Include/CmdRCSoftwareUpdate.h>

#define TRACE_TO_FILE 0     // set to 1 for messages getting printed to file

QFile testlogFile;

namespace Himalaya {


class MApplication : public QCoreApplication
{
public:
    MApplication (int &Argc, char ** Argv )
        : QCoreApplication ( Argc, Argv )
    {};

    ~MApplication() {};

    Himalaya::HimalayaMasterThreadController TheMasterThreadController;


    virtual bool notify(QObject * Object, QEvent * Event)
    {
        try
        {
            return QCoreApplication::notify(Object, Event);
        }
        catch(std::exception& e)
        {
            qWarning() << "Exception thrown:" << e.what();
            Global::EventObject::Instance().RaiseEvent(Global::EVENT_GLOBAL_UNCAUGHT_EXCEPTION, Global::FmtArgs() << e.what());
            TheMasterThreadController.Shutdown();
            QCoreApplication::exit();
        }
        catch(Global::Exception& e)
        {
            qWarning() << "Exception thrown:" << e.what();
            Global::EventObject::Instance().RaiseEvent(Global::EVENT_GLOBAL_UNCAUGHT_EXCEPTION, Global::FmtArgs() << e.what());
            TheMasterThreadController.Shutdown();
            QCoreApplication::exit();
        }
        catch (...)
        {
            qWarning() << "Exception thrown";
            Global::EventObject::Instance().RaiseEvent(Global::EVENT_GLOBAL_ERROR_UNKNOWN_EXCEPTION);
            TheMasterThreadController.Shutdown();
            QCoreApplication::exit();
        }
        return false;
    }

    static void installMsgHandler()
    {
        (void)qInstallMsgHandler(HimalayaMessageHandler);
        testlogFile.setFileName("DebugOutput.log");
        (void)testlogFile.open(QIODevice::WriteOnly | QIODevice::Text);
    }

    static void HimalayaMessageHandler(QtMsgType type, const char *msg)
    {
      QString txt;
      QTextStream out(&testlogFile);

      switch (type) {
        case QtDebugMsg:
          txt = QString("Debug: %1").arg(msg);
          break;
        case QtWarningMsg:
          txt = QString("Warning: %1").arg(msg);
          break;
        case QtCriticalMsg:
          txt = QString("Critical: %1").arg(msg);
          break;
        case QtFatalMsg:
          txt = QString("Fatal: %1").arg(msg);
          abort();
      }
      out << txt << endl;
    }

};

class TestHimalayaMasterThread : public QObject
{
    Q_OBJECT
    
private slots:
    /****************************************************************************/
    /**
     * \brief Called before the first testfunction is executed.
     */
    /****************************************************************************/
    void initTestCase();
    /****************************************************************************/
    /**
     * \brief Called before each testfunction is executed.
     */
    /****************************************************************************/
    void init();
    /****************************************************************************/
    /**
     * \brief Called after each testfunction was executed.
     */
    /****************************************************************************/
    void cleanup();
    /****************************************************************************/
    /**
     * \brief Called after last testfunction was executed.
     */
    /****************************************************************************/
    void cleanupTestCase();

    /****************************************************************************/
    /**
     * \brief Test Himalaya Sepia controller.
     */
    /****************************************************************************/
    void utTestHimalayaMasterThread();
}; // end class TestHimalayaSepiaController

/****************************************************************************/
void TestHimalayaMasterThread::initTestCase() {

}

/****************************************************************************/
void TestHimalayaMasterThread::init() {
}

/****************************************************************************/
void TestHimalayaMasterThread::cleanup() {
}

/****************************************************************************/
void TestHimalayaMasterThread::cleanupTestCase() {
}

void TestHimalayaMasterThread::utTestHimalayaMasterThread()
{
    char **argv = NULL;
    int argc = 0;
    Global::EventObject::Instance();
    Global::AdjustedTime::Instance();
    Global::AlarmPlayer::Instance();
    EventHandler::StateHandler::Instance();
    MApplication App(argc, argv);

    // set global directories.
    Global::SystemPaths::Instance().SetComponentTestPath("../Tests");
    Global::SystemPaths::Instance().SetFirmwarePath("../Firmware");
    Global::SystemPaths::Instance().SetLogfilesPath("../Logfiles");
    Global::SystemPaths::Instance().SetManualPath("../Manual");
    Global::SystemPaths::Instance().SetSettingsPath("../Settings");
    Global::SystemPaths::Instance().SetInstrumentSettingsPath("../Settings/Instrument");
    Global::SystemPaths::Instance().SetUpdatePath("../Update");
    Global::SystemPaths::Instance().SetUploadsPath("../Uploads");
    Global::SystemPaths::Instance().SetTempPath("../Temporary");
    Global::SystemPaths::Instance().SetRollbackPath("../Rollback");
    Global::SystemPaths::Instance().SetTranslationsPath("../Translations");
    Global::SystemPaths::Instance().SetSoundPath("../Sounds");
    Global::SystemPaths::Instance().SetRemoteCarePath("../RemoteCare");
    Global::SystemPaths::Instance().SetScriptsPath("../Scripts");

    Threads::SingletonThreadObject SingletonObjThread;

    Global::EventObject::Instance().setParent(&SingletonObjThread);
    (void)Global::AdjustedTime::Instance();
    Global::AlarmPlayer::Instance().setParent(&SingletonObjThread);
    EventHandler::StateHandler::Instance().setParent(&SingletonObjThread);

    QThread ThreadForSingletonObjects;
    SingletonObjThread.moveToThread(&ThreadForSingletonObjects);
    (void)QObject::connect(&ThreadForSingletonObjects, SIGNAL(started()), &SingletonObjThread, SLOT(Run()));
    ThreadForSingletonObjects.start();
    QTimer TimeFinish;
    (void)QObject::connect(&TimeFinish, SIGNAL(timeout()), &ThreadForSingletonObjects, SIGNAL(finished()));
    TimeFinish.setInterval(10000);
    TimeFinish.start();
    //=========================================================
    // WARNING: uncomment following lines to switch on
    //          the DeviceControl layer debug logging.
//    FILE* pFile = fopen ( "colorado_app.log", "w" );
//    Output2FILE::Stream() = pFile;
//    FILE_LOG_L(laFCT, llINFO) << "DEBUG FCT";
//    FILE_LOG_L(laDEV, llINFO) << "DEBUG DEV";
//    FILE_LOG_L(laDEVPROC, llINFO) << "DEBUG DEVPROC";
    //=========================================================

    // catch unexpected signals
    Global::SignalHandler signalHandler;
    signalHandler.init();

    for (int i=1; i< argc; i++) {
        if ((QString(argv[i]) == "-rolledback")) {
            Global::AppSettings::SettingsRolledBack = true;
        }
    }

    //#ifdef QT_DEBUG
////    SchedulerSimulation::MainWindow::getInstance().show();
//#endif



    // create master thread
    QThread thrMasterThread;
    // create controller for master thread
    App.TheMasterThreadController.SetOperatingMode("production");
    // base file name for logging is "Events"
    App.TheMasterThreadController.SetEventLoggerBaseFileName("PRIMARIS_");
    /// \todo set max file size of event logger
    App.TheMasterThreadController.SetEventLoggerMaxFileSize(1000000);
    // set max file count for day operation logger
    App.TheMasterThreadController.SetDayEventLoggerMaxFileCount(56);
    // set max date time offset
    App.TheMasterThreadController.SetMaxAdjustedTimeOffset(24*3600);
    // read time offset from file
    App.TheMasterThreadController.ReadTimeOffsetFile();
    // add code to close application when Master thread stops
    QObject::connect(&thrMasterThread, SIGNAL(finished()), &ThreadForSingletonObjects, SLOT(quit()));
    // Master thread should give its controller automatically a "Go" signal as soon it has started
    QObject::connect(&thrMasterThread, SIGNAL(started()), &App.TheMasterThreadController, SLOT(Go()));

    (void)QObject::connect(&ThreadForSingletonObjects, SIGNAL(finished()), &App, SLOT(quit()));
    // now push controller to master thread
    App.TheMasterThreadController.moveToThread(&thrMasterThread);
    // and start the Master thread
    thrMasterThread.start();

    // start application
    App.exec();

    // wait for Master thread
//    thrMasterThread.wait();
    // cleanup controller for master thread.
    App.TheMasterThreadController.CleanupAndDestroyObjects();
    //write buffered data to disk-> refer man pages for sync
    (void)system("sync &");
    std::cout << "\n\n\n Sync in Main" << Global::AdjustedTime::Instance().GetCurrentTime().toString().toStdString();
    // and exit
    //Remove parents for singleton
    Global::EventObject::Instance().setParent(NULL);
    Global::AlarmPlayer::Instance().setParent(NULL);
    EventHandler::StateHandler::Instance().setParent(NULL);
    App.TheMasterThreadController.CreateAndInitializeObjects();
    App.TheMasterThreadController.CreateControllersAndThreads();
    App.TheMasterThreadController.OnLanguageChanged(true);
    App.TheMasterThreadController.OnPowerFail(Global::POWER_FAIL_NONE);
    App.TheMasterThreadController.SendRCCmdToGuiChannel(Global::CommandShPtr_t(new RemoteCare::CmdRCSoftwareUpdate(15000, RemoteCare::SWUpdate_Available)));
    App.TheMasterThreadController.ShowWaitDialog(false, Global::DEFAULT_TEXT);
    App.TheMasterThreadController.SWUpdateProgress(false);
    App.TheMasterThreadController.SWUpdateRollbackComplete();
    
    App.TheMasterThreadController.RequestDayRunLogFileNames("\\");
    Global::tRefType ref;
    Threads::CommandChannel cmdChannel(&(App.TheMasterThreadController), "GUI", Global::EVENTSOURCE_NONE);
    NetCommands::CmdExportDayRunLogReply* cmdExportDayRunLogReply = new NetCommands::CmdExportDayRunLogReply();
    App.TheMasterThreadController.ExportDayRunLogHandler(ref, *cmdExportDayRunLogReply,
                                                         cmdChannel);

    delete cmdExportDayRunLogReply;

    NetCommands::CmdSystemAction* cmdSystemAction = new NetCommands::CmdSystemAction();
    App.TheMasterThreadController.EventCmdSystemAction(ref, *cmdSystemAction, cmdChannel);
    delete cmdSystemAction;

    NetCommands::CmdChangeAdminPassword* cmdChangeAdminPassword = new NetCommands::CmdChangeAdminPassword();
    App.TheMasterThreadController.ChangeAdminPasswordHandler(ref, *cmdChangeAdminPassword, cmdChannel);
    delete cmdChangeAdminPassword;

    MsgClasses::CmdResetOperationHours* cmdResetOperationHours = new MsgClasses::CmdResetOperationHours(5000, DataManager::RESETOPERATIONHOURS_WHOLEMACHINEOPERATION);
    App.TheMasterThreadController.ResetOperationHoursHandler(ref, *cmdResetOperationHours, cmdChannel);
    delete cmdResetOperationHours;

    NetCommands::CmdChangeUserLevel* cmdChangeUserLevel = new NetCommands::CmdChangeUserLevel();
    App.TheMasterThreadController.ChangeUserLevelHandler(ref, *cmdChangeUserLevel, cmdChannel);

    App.TheMasterThreadController.SendLanguageFileToGUI("\\");

    App.TheMasterThreadController.UpdateSupportedGUILanguages();

    QStringList importTypeList;
    App.TheMasterThreadController.ImportExportThreadFinished(true, importTypeList, 1305045);
    App.TheMasterThreadController.SendXML();

    MsgClasses::CmdDataImportFiles cmdDataImportFiles;
    App.TheMasterThreadController.ImportFilesHandler(ref, cmdDataImportFiles, cmdChannel);

    NetCommands::CmdGuiInit cmdGuiInit;
    App.TheMasterThreadController.OnCmdGuiInitHandler(ref, cmdGuiInit, cmdChannel);

    App.TheMasterThreadController.SendStateChange("IdleState");

    App.quit();
#if 0
    char **argv = NULL;
    int argc = 0;
    // catch unexpected signals
    Global::SignalHandler signalHandler;
    signalHandler.init();


    MApplication App(argc, argv);
//    QApplication App(argc, argv);

    if (Global::AppSettings::TraceDebugMessagesEnabled) {
        MApplication::installMsgHandler();
    }


    Global::AppSettings::SimulationEnabled = true;

    Global::AppSettings::ExtendedLoggingEnabled = true;

    Global::AppSettings::TraceDebugMessagesEnabled = true;



    // set global directories.
    Global::SystemPaths::Instance().SetComponentTestPath("../Tests");
    Global::SystemPaths::Instance().SetFirmwarePath("../Firmware");
    Global::SystemPaths::Instance().SetLogfilesPath("../Logfiles");
    Global::SystemPaths::Instance().SetManualPath("../Manual");
    Global::SystemPaths::Instance().SetSettingsPath("../Settings");
    Global::SystemPaths::Instance().SetInstrumentSettingsPath("../Settings/Instrument");
    Global::SystemPaths::Instance().SetUpdatePath("../Update");
    Global::SystemPaths::Instance().SetUploadsPath("../Uploads");
    Global::SystemPaths::Instance().SetTempPath("../Temporary");
    Global::SystemPaths::Instance().SetRollbackPath("../Rollback");
    Global::SystemPaths::Instance().SetTranslationsPath("../Translations");
    Global::SystemPaths::Instance().SetSoundPath("../Sounds");
    Global::SystemPaths::Instance().SetRemoteCarePath("../RemoteCare");

    system("sudo chmod 777 ../");
//    // create master thread
//    QThread thrMasterThread;

    App.TheMasterThreadController.SetOperatingMode("production");
    // base file name for logging is "Events"
    App.TheMasterThreadController.SetEventLoggerBaseFileName("Leica_ST_");
    /// \todo set max file size of event logger
    App.TheMasterThreadController.SetEventLoggerMaxFileSize(1000000);
    // set max file count for day operation logger - Maximum 30 working days of logging is enabled
    App.TheMasterThreadController.SetDayEventLoggerMaxFileCount(30);
    // set max date time offset
    App.TheMasterThreadController.SetMaxAdjustedTimeOffset(24*3600);
    // read time offset from file
    App.TheMasterThreadController.ReadTimeOffsetFile();

    Global::tRefType Ref = 0;
    Global::AckOKNOK Ack;
    QString CmdName = "";
    QStringList ImportTypeList;
    ImportTypeList.clear();

    Threads::CommandChannel AckCommandChannel(&App.TheMasterThreadController, "Dummy", Global::EVENTSOURCE_NONE);
    NetCommands::CmdExternalProcessState ExternalProcessCmd;


    NetCommands::CmdSystemAction SystemActionCmd;


    // now push controller to master thread
    //App.TheMasterThreadController.moveToThread(App.thread());

    // and start the Master thread
    App.TheMasterThreadController.CreateAndInitializeObjects();
//    App.TheMasterThreadController.ShutdownOnPowerFail();

    App.TheMasterThreadController.OnAckOKNOK(Ref, Ack);
    App.TheMasterThreadController.OnProcessTimeout(Ref, CmdName);
    App.TheMasterThreadController.SendDCLContainerTo(AckCommandChannel);
    App.TheMasterThreadController.OnInitStateCompleted();
    App.TheMasterThreadController.SendStateChange("IdleState");
    App.TheMasterThreadController.RemoveAndDestroyObjects();

    App.TheMasterThreadController.EventCmdSystemAction(Ref, SystemActionCmd, AckCommandChannel);
    App.TheMasterThreadController.ExternalProcessConnectionHandler(Ref, ExternalProcessCmd, AckCommandChannel);
    App.TheMasterThreadController.RequestDayRunLogFileNames("../DayRunLog/");
    App.TheMasterThreadController.SWUpdateProgress(false);
    App.TheMasterThreadController.OnInitStateCompleted();
    App.TheMasterThreadController.SendStateChange("IdleState");
    App.TheMasterThreadController.EventCmdSystemAction(Ref, SystemActionCmd, AckCommandChannel);
    App.TheMasterThreadController.ExternalProcessConnectionHandler(Ref, ExternalProcessCmd, AckCommandChannel);
    App.TheMasterThreadController.SWUpdateRollbackComplete();
    App.TheMasterThreadController.InitializeGUI();

//    App.TheMasterThreadController.RemoteCareExportData(1);
//    Threads::CommandChannel DummyChannel(&(App.TheMasterThreadController), "Dummy", Global::EVENTSOURCE_NONE);
//    App.TheMasterThreadController.ImportExportDataFileHandler<MsgClasses::CmdDataExport>(Ref,
//                                  MsgClasses::CmdDataExport(),DummyChannel);
//    App.TheMasterThreadController.ExportProcessExited(CmdName, Global::EXIT_CODE_EXPORT_SUCCESS);
//    App.TheMasterThreadController.ImportExportDataFileHandler<MsgClasses::CmdDataImport>(Ref,
//                                  MsgClasses::CmdDataImport(),DummyChannel);
//    App.TheMasterThreadController.ImportExportThreadFinished(true, ImportTypeList,
//                                                             Global::EXIT_CODE_EXPORT_SUCCESS, false, false);

//    App.TheMasterThreadController.OnPowerFail(Global::POWER_FAIL_NONE);
   // App.TheMasterThreadController.ShutdownHandler(Ref,ShutdownCmd, AckCommandChannel);
    // cleanup controller for master thread.
    //App.TheMasterThreadController.CleanupAndDestroyObjects();
#endif

}
} //end of namespace
QTEST_MAIN(Himalaya::TestHimalayaMasterThread)

#include "TestHimalayaMasterThread.moc"
