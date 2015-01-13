/****************************************************************************/
/*! \file Components/Main/Source/Main.cpp
 *
 *  \brief Implementation file for main routine.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2010-07-12
 *  $Author:    $ J.Bugariu
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

#include <Main/Include/Main.h>
#include <Global/Source/GlobalDefines.cpp>
#include <Global/Include/SystemPaths.h>
#include <Global/Include/Utils.h>
#include <EventHandler/Include/StateHandler.h>
#include "Global/Include/SignalHandler.h"
#include "Global/Include/GlobalDefines.h"
#include "Global/Include/Utils.h"
/// \todo Norbert's debug output

#include <DeviceControl/Include/Global/dcl_log.h>
#include "Global/Include/AlarmPlayer.h"


//bool Global::AppSettings::SimulationEnabled = false;
//bool Global::AppSettings::ExtendedLoggingEnabled = false;

/****************************************************************************/
/**
 * \brief main
 *
 * \iparam argc
 * \iparam argv
 * \return main return
 */
/****************************************************************************/
int main(int argc, char *argv[]) {

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
    Global::SystemPaths::Instance().SetFontsPath("../Fonts");

    Threads::SingletonThreadObject SingletonObjThread;

    Global::EventObject::Instance().setParent(&SingletonObjThread);
    (void)Global::AdjustedTime::Instance();
    Global::AlarmPlayer::Instance().setParent(&SingletonObjThread);
    EventHandler::StateHandler::Instance().setParent(&SingletonObjThread);

    QThread ThreadForSingletonObjects;
    SingletonObjThread.moveToThread(&ThreadForSingletonObjects);
    (void)QObject::connect(&ThreadForSingletonObjects, SIGNAL(started()), &SingletonObjThread, SLOT(Run()));
    ThreadForSingletonObjects.start();
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
    try {

        App.TheMasterThreadController.SetOperatingMode("production");
        // base file name for logging is "Events"
        App.TheMasterThreadController.SetEventLoggerBaseFileName("HISTOCOREPRIMARIS_");
        /// \todo set max file size of event logger
        App.TheMasterThreadController.SetEventLoggerMaxFileSize(1000000);
        // set max file count for day operation logger
        App.TheMasterThreadController.SetDayEventLoggerMaxFileCount(56);
        // set max date time offset
        App.TheMasterThreadController.SetMaxAdjustedTimeOffset(24*3600);
        // read time offset from file
        if(!App.TheMasterThreadController.ReadTimeOffsetFile()) {
            // exit with error code
            return Global::RETCODE_TIME_OFFSET;
        }
        qDebug()<<"TIME \n\n" << Global::AdjustedTime::Instance().GetCurrentDateTime();
        // add code to quit Master thread if its controller requests it
        if(!QObject::connect(&App.TheMasterThreadController, SIGNAL(RequestFinish()), &App, SLOT(ProcessAnyPendingEvents()))) {
            Global::ToConsole("Connecting TheMasterThreadController::RequestFinish() and App::ProcessAnyPendingEvents() failed");
            // exit with error code
            return Global::RETCODE_CONNECT_FAILED;
        }
        // add code to quit Master thread if its controller requests it
        if(!QObject::connect(&App, SIGNAL(InitializeThreadStop()), &App.TheMasterThreadController, SLOT(Stop()))) {
            Global::ToConsole("Connecting App::InitializeThreadStop() and TheMasterThreadController::Stop() failed");
            // exit with error code
            return Global::RETCODE_CONNECT_FAILED;
        }
        // add code to close application when Master thread stops
        if(!QObject::connect(&thrMasterThread, SIGNAL(finished()), &ThreadForSingletonObjects, SLOT(quit()))) {
            Global::ToConsole("Connecting thrMasterThread::finished() and App::quit() failed");
            // exit with error code
            return Global::RETCODE_CONNECT_FAILED;
        }
        // Master thread should give its controller automatically a "Go" signal as soon it has started
        if(!QObject::connect(&thrMasterThread, SIGNAL(started()), &App.TheMasterThreadController, SLOT(Go()))) {
            Global::ToConsole("Connecting thrMasterThread::started() and TheMasterThreadController::Go() failed");
            // exit with error code
            return Global::RETCODE_CONNECT_FAILED;
        }
        (void)QObject::connect(&ThreadForSingletonObjects, SIGNAL(finished()), &App, SLOT(quit()));
        // now push controller to master thread
        App.TheMasterThreadController.moveToThread(&thrMasterThread);
        // and start the Master thread        
        thrMasterThread.start();

        // start application
        int ReturnCode = App.exec();

        // wait for Master thread
        if(!thrMasterThread.wait())
        {
            Global::ToConsole("Error waiting for thrMasterThread");
            // set error code
            ReturnCode = Global::RETCODE_MASTERTHREAD_WAIT;
        }
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
        return ReturnCode;
    }
    catch(const std::exception &E) {
        // send error message
        std::cout << "std::exception caught: \"" << E.what() << "\" in file "<< __FILE__ << " at line " << __LINE__ << std::endl;
        // exit with error code
        return Global::RETCODE_MASTERTHREAD_INIT;

    } catch(const Global::Exception &E) {
        // send error message
        std::cout << "Internal Software Error . Error Code: " << E.GetErrorCode();
        // exit with error code
        return Global::RETCODE_MASTERTHREAD_INIT;
    }
    catch(...) {
        std::cout << "Internal Software Error: Unknown Exception caught";
        // exit with error code
        return Global::RETCODE_MASTERTHREAD_INIT;
    }
}
