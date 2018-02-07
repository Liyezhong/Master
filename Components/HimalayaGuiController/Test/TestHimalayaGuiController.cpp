#include <QString>
#include <QTest>
#include <QFile>
#include <HimalayaMasterThread/Include/HimalayaMasterThreadController.h>
#include <Threads/Include/PlatformThreadIDs.h>
#include <HimalayaGuiController/Include/HimalayaGuiController.h>
#include <Global/Include/SystemPaths.h>
#include <Global/Include/Utils.h>

#include <Global/Include/Commands/AckOKNOK.h>
#include <Global/Include/Commands/CmdDateAndTime.h>
#include <NetCommands/Include/CmdConfigurationFile.h>
#include <DataManager/Containers/UserSettings/Commands/Include/CmdAlarmToneTest.h>
#include <DataManager/Containers/UserSettings/Commands/Include/CmdChangeUserSettings.h>
#include <NetCommands/Include/CmdProcessState.h>
#include <NetCommands/Include/CmdRemoteCareState.h>
#include <DataManager/Containers/UserSettings/Commands/Include/CmdWaterStationUpdate.h>
#include <NetCommands/Include/CmdDataUpdate.h>
#include <NetCommands/Include/CmdUpdateBlgStatus.h>
#include <NetCommands/Include/CmdEventStrings.h>
#include <NetCommands/Include/CmdExecutionStateChanged.h>
#include <NetCommands/Include/CmdLanguageFile.h>
#include <NetCommands/Include/CmdScheduledStations.h>
#include <NetCommands/Include/CmdExternalProcessState.h>
#include <NetCommands/Include/CmdEventReport.h>
#include <NetCommands/Include/CmdChangeAdminPassword.h>
#include <NetCommands/Include/CmdChangeAdminPasswordReply.h>
#include <NetCommands/Include/CmdChangeUserLevel.h>
#include <NetCommands/Include/CmdChangeUserLevelReply.h>
#include <NetCommands/Include/CmdStationsDefect.h>
#include <NetCommands/Include/CmdDayRunLogReply.h>
#include <NetCommands/Include/CmdDayRunLogReplyFile.h>
#include <NetCommands/Include/CmdDayRunLogRequest.h>
#include <NetCommands/Include/CmdDayRunLogRequestFile.h>
#include <NetCommands/Include/CmdAcknEventReport.h>
#include <NetCommands/Include/CmdGuiInit.h>
#include <NetCommands/Include/CmdRequestRack.h>
#include <NetCommands/Include/CmdRequestParameterSet.h>
#include <NetCommands/Include/CmdGuiEventReport.h>
#include <NetCommands/Include/CmdSWUpdate.h>
#include <NetCommands/Include/CmdRCRequestRemoteSession.h>
#include <NetCommands/Include/CmdRCSoftwareUpdate.h>
#include <NetCommands/Include/CmdStartServiceSW.h>

#include <stdlib.h> //For "system()"
#include "Global/Include/SignalHandler.h"
#include "Global/Include/GlobalDefines.h"

#define TRACE_TO_FILE 0     // set to 1 for messages getting printed to file

QFile logFile;

namespace HimalayaGui {


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
        logFile.setFileName("DebugOutput.log");
        (void)logFile.open(QIODevice::WriteOnly | QIODevice::Text);
    }

    static void HimalayaMessageHandler(QtMsgType type, const char *msg)
    {
      QString txt;
      QTextStream out(&logFile);

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

class TestHimalayaGuiController : public QObject
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
     * \brief Test Himalaya GUI controller.
     */
    /****************************************************************************/
    void utTestHimalayaGuiController();
}; // end class TestHimalayaGuiController

/****************************************************************************/
void TestHimalayaGuiController::initTestCase() {

}

/****************************************************************************/
void TestHimalayaGuiController::init() {

}

/****************************************************************************/
void TestHimalayaGuiController::cleanup() {
}

/****************************************************************************/
void TestHimalayaGuiController::cleanupTestCase() {
}

void TestHimalayaGuiController::utTestHimalayaGuiController()
{
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

    HimalayaGui::HimalayaGuiController *p_GuiController = new HimalayaGui::HimalayaGuiController(Threads::THREAD_ID_GUI);

    if (p_GuiController != NULL) {
        p_GuiController->CreateAndInitializeObjects();
        p_GuiController->OnGoReceived();
        p_GuiController->OnReadyToWork();
        p_GuiController->OnStopWorking();
        p_GuiController->OnStopReceived();
        p_GuiController->OnPowerFail(Global::POWER_FAIL_NONE);
        QString CmdName = "";
        p_GuiController->OnCmdTimeout(0, CmdName);
        p_GuiController->SendConfigurationFiles();
        p_GuiController->CleanupAndDestroyObjects();
    }
}
} //end of namespace

QTEST_APPLESS_MAIN(HimalayaGui::TestHimalayaGuiController)

#include "TestHimalayaGuiController.moc"
