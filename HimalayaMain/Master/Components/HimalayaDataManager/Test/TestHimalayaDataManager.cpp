#include <QString>
#include <QTest>
#include <QFile>
#include <Global/Include/Utils.h>
#include <HimalayaMasterThread/Include/HimalayaMasterThreadController.h>
#include <Threads/Include/PlatformThreadIDs.h>
#include <HimalayaGuiController/Include/HimalayaGuiController.h>
#include <Global/Include/SystemPaths.h>
#include <HimalayaDataManager/Include/DataContainer.h>
#include <HimalayaDataManager/Include/DataManager.h>
#include <stdlib.h> //For "system()"
#include "Global/Include/SignalHandler.h"
#include "Global/Include/GlobalDefines.h"
#define TRACE_TO_FILE 0     // set to 1 for messages getting printed to file

QFile logFile;

namespace DataManager {

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

class TestHimalayaDataManager : public QObject
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
     * \brief Test Himalaya Data Manager.
     */
    /****************************************************************************/
    void utTestHimalayaDataManager();
}; // end class TestHimalayaDataManager

/****************************************************************************/
void TestHimalayaDataManager::initTestCase() {

}

/****************************************************************************/
void TestHimalayaDataManager::init() {

}

/****************************************************************************/
void TestHimalayaDataManager::cleanup() {
}

/****************************************************************************/
void TestHimalayaDataManager::cleanupTestCase() {
}

void TestHimalayaDataManager::utTestHimalayaDataManager()
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
    Global::SystemPaths::Instance().SetTranslationsPath("../Translations");
    Global::SystemPaths::Instance().SetSoundPath("../Sounds");
    Global::SystemPaths::Instance().SetRemoteCarePath("../RemoteCare");

    CDataManager* pDataManager = new CDataManager(NULL);
}

} //end of namespace

QTEST_APPLESS_MAIN(DataManager::TestHimalayaDataManager)

#include "TestHimalayaDataManager.moc"
