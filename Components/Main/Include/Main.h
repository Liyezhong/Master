/****************************************************************************/
/*! \file Components/Main/Include/Main.h
 *
 *  \brief Include file for main routine.
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

#ifndef MAIN_H
#define MAIN_H

#include <QObject>
#include <iostream>
#include <QThread>
#include <QString>
#include <QDebug>
#include <stdlib.h> //For "system()"

#include <Threads/Include/SingletonThreadObject.h>
#include <HimalayaMasterThread/Include/HimalayaMasterThreadController.h>
#include <DataManager/Helper/Include/XmlConfigFileTimeOffset.h>

#define TRACE_TO_FILE 0     //!< set to 1 for messages getting printed to file

static QFile logFile;  //!< log file
/****************************************************************************/
/**
 * \brief MApplication definition
 */
/****************************************************************************/
class MApplication : public QCoreApplication
{
    Q_OBJECT
public slots:
    /****************************************************************************/
    /*!
     *  \brief Calls for event processing and stops all threads
     *
     */
    /****************************************************************************/
    void ProcessAnyPendingEvents()
    {
        processEvents();
        emit InitializeThreadStop();
    }

signals:
    /****************************************************************************/
    /*!
     *  \brief Triggers the stopping of thread controllers
     *
     */
    /****************************************************************************/
    void InitializeThreadStop();
public:
    /****************************************************************************/
    /*!
     *  \brief constructor
     *
     *  \iparam Argc = command line arguments count
     *  \iparam Argv = command line arguments
     */
    /****************************************************************************/
    MApplication (int &Argc, char ** Argv )
        : QCoreApplication ( Argc, Argv )
    {}


    ~MApplication() {}

    Himalaya::HimalayaMasterThreadController TheMasterThreadController; //!< colorado master thread controller


    /****************************************************************************/
    /*!
     *  \brief notifies the objects with the event passed
     *
     *  \iparam Object = object to be notified
     *  \iparam Event = event
     *  \return true if success else false
     */
    /****************************************************************************/
    virtual bool notify(QObject * Object, QEvent * Event)
    {
        try {
            return QCoreApplication::notify(Object, Event);
        }
        CATCHALL();

        qWarning() << "Exception thrown";
        Global::EventObject::Instance().RaiseEvent(Global::EVENT_GLOBAL_UNCAUGHT_EXCEPTION);
        TheMasterThreadController.Shutdown();
        QCoreApplication::exit();
        return false;
    }

    /****************************************************************************/
    /*!
     *  \brief installs the message handler
     */
    /****************************************************************************/
    static void installMsgHandler()
    {
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        (void)qInstallMsgHandler(HimalayaMessageHandler);
#else
        (void)qInstallMessageHandler(HimalayaMessageHandler);
#endif
        logFile.setFileName("DebugOutput.log");
        (void)logFile.open(QIODevice::WriteOnly | QIODevice::Text);
    }

    /****************************************************************************/
    /*!
     *  \brief colorado message handler, logs the given message in the log file
     *
     *  \iparam type = message type, debug, waning, critical, fatal
     *  \iparam msg = message string
     *  \iparam context = message log context
     */
    /****************************************************************************/
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    static void HimalayaMessageHandler(QtMsgType type, const char *msg)
#else
    static void HimalayaMessageHandler(QtMsgType type , const QMessageLogContext & context, const QString & msg)
#endif
    {
        try {
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
        catch(const std::bad_alloc &) {
            printf("bad alloc: Not able to write message to file\n");
        }
        catch(const std::exception & E) {
            printf("exception: Not able to write message to file\n");
        }
        catch (...) {
            printf("Not able to write message to file\n");
        }
    }


};

/****************************************************************************/
/**
 * \brief Main function.
 *
 * Basicly the following is done: the main thread controller is created and
 * started in its own thread. Also the application object is started.
 * As soon as the main threads finishes, the application is also terminated.
 *
 * \iparam   argc    Argument count.
 * \iparam   argv    Argument list.
 *
 * \return   main return.
 */
/****************************************************************************/
int main(int argc, char *argv[]);
#endif // MAIN_H
