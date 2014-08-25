/****************************************************************************/
/*! \file ServiceMain.cpp
 *
 *  \brief ServiceMain implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-05-17
 *   $Author:  $ M.Scherer
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

#include "Global/Include/Utils.h"
#include "Application/Include/Application.h"
#include "Core/Include/Startup.h"
#include <Global/Include/AlarmPlayer.h>
#include <EventHandler/Include/StateHandler.h>
#include "Global/Include/SignalHandler.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"
#include "ServiceDataManager/Include/TestCaseGuide.h"

/****************************************************************************/
/*!
 *  \brief The program's main function.
 *
 *  \iparam Argc = Command line argument count
 *  \iparam p_Argv = List of command line arguments
 */
/****************************************************************************/
int main(int Argc, char *p_Argv[])
{
//    Global::ToConsole("Service Sotware execution started..");

    //************Create all the singleton objects.********************//
    //! \warning changing order of below function call an cause ugly bugs & deadlocks!!
    Global::EventObject::Instance();
    Global::AdjustedTime::Instance();
    Global::AlarmPlayer::Instance();
    EventHandler::StateHandler::Instance();
    //**************End of singleton object creation*******************//
    if(Argc>3)
    {
        Global::ToConsole("Service Sotware called with Illegal number of arguments");
        exit(1);
    }
    (void) system("lcd on");

    //lint -esym(526, qInitResources_*)
    //lint -esym(534, qInitResources_*)
    Q_INIT_RESOURCE(Application);
    Q_INIT_RESOURCE(Main);
    Q_INIT_RESOURCE(MainMenu);
    Q_INIT_RESOURCE(KeyBoard);
    Q_INIT_RESOURCE(SpellerButtons);
    Q_INIT_RESOURCE(Speller);

    Application::CApplication App(Argc, p_Argv, Application::DEVICE_HIMALAYA);
    QFont Font;
    Font.setPointSize(11);
    Font.setFamily("FreeSans"); //"FreeSans","Arial");
    App.setFont(Font);

    // set global directories.
    Global::SystemPaths::Instance().SetComponentTestPath("../Tests");
    Global::SystemPaths::Instance().SetFirmwarePath("../Firmware");
    Global::SystemPaths::Instance().SetLogfilesPath("../Logfiles");
    Global::SystemPaths::Instance().SetManualPath("../Manual");
    Global::SystemPaths::Instance().SetSettingsPath("../Settings");
    Global::SystemPaths::Instance().SetUpdatePath("../Update");
    Global::SystemPaths::Instance().SetUploadsPath("../Uploads");
    Global::SystemPaths::Instance().SetTempPath("../Temporary");
    Global::SystemPaths::Instance().SetRollbackPath("../Rollback");
    Global::SystemPaths::Instance().SetTranslationsPath("../TranslationsService");
    Global::SystemPaths::Instance().SetSoundPath("../Sounds");
    Global::SystemPaths::Instance().SetInstrumentSettingsPath("../Settings/Instrument");

    Global::SystemPaths::Instance().SetScriptsPath("../Scripts");

    Core::CStartup Startup;
    Threads::ServiceMasterThreadController TheMasterThreadController(&Startup);
    TheMasterThreadController.SetEventLoggerBaseFileName("Himalaya_Service");

    QString FileName = Global::SystemPaths::Instance().GetSettingsPath() + "/TestCaseConfig.xml";
    DataManager::CTestCaseFactory::Instance().InitData(FileName);

    FileName = Global::SystemPaths::Instance().GetSettingsPath() + "/TestCaseGuide_en.xml";
    DataManager::CTestCaseGuide::Instance().InitData(FileName);

    // catch unexpected signals
    Global::SignalHandler signalHandler;
    signalHandler.init();

    QThread thrMasterThread;

    TheMasterThreadController.moveToThread(&thrMasterThread);


    if(!QObject::connect(&thrMasterThread, SIGNAL(started()), &TheMasterThreadController, SLOT(Go()))) {
        Global::ToConsole("Connecting thrMasterThread::started() and TheMasterThreadController::Go() failed");
    }

    QObject::connect(&thrMasterThread, SIGNAL(finished()), &App, SLOT(quit()));

    thrMasterThread.start();

    QString param;
    if (Argc==2) {
        param = QString("%1").arg(p_Argv[1]);
    }


    if (Argc==1 || (Argc==2 && param == "-qws")){
        Global::ToConsole("Software in Debug Mode 111111");
        Startup.GuiInit();
    }
    else {
        Global::ToConsole("Software in Debug Mode");
        Startup.GuiInit(param);
    }

    return App.exec();
}
