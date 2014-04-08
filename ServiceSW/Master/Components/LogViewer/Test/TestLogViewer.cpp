/****************************************************************************/
/*! \file TestLogViewer.cpp
 *
 *  \brief Unit test for LogViewer menu
 *
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-08-06
 *  $Author:    $ Soumya. D
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

#include <QtTest/QTest>
#include <QDebug>
#include <Core/Include/ServiceGUIConnector.h>
#include <MainMenu/Include/MainWindow.h>
#include <Global/Include/SystemPaths.h>
#include <LogViewer/Include/SystemLogViewerDlg.h>
//#include <LogViewer/Include/ServiceHelpTextDlg.h>
#include <LogViewer/Include/ServiceLogViewerDlg.h>
#include <LogViewer/Include/SoftwareUpdateLogDlg.h>
#include <QObject>

namespace LogViewer {

/****************************************************************************/
/**
 * \brief Test class for LogViewer class.
 */
/****************************************************************************/
class CTestLogViewer : public QObject {
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
     * \brief Test LogViewer Menu object
     */
    /****************************************************************************/
    void utTestLogViewer();

}; // end class CTestLogViewer

/****************************************************************************/
void CTestLogViewer::initTestCase() {
}

/****************************************************************************/
void CTestLogViewer::init() {
}

/****************************************************************************/
void CTestLogViewer::cleanup() {
}

/****************************************************************************/
void CTestLogViewer::cleanupTestCase() {
}

/****************************************************************************/
void CTestLogViewer::utTestLogViewer() {

    MainMenu::CMainWindow *p_MainWindow = new MainMenu::CMainWindow();

//    Core::CServiceGUIConnector *p_GUIConnector = new
//            Core::CServiceGUIConnector(p_MainWindow);

//    LogViewer::CSystemLogViewer *p_SystemLogViewer = new
//            LogViewer::CSystemLogViewer(p_GUIConnector, p_MainWindow);

    LogViewer::CSystemLogViewerDlg *p_SystemLogViewerDlg = new
            LogViewer::CSystemLogViewerDlg(p_MainWindow);

//    LogViewer::CServiceHelpText *p_ServiceHelpText = new
//            LogViewer::CServiceHelpText(p_MainWindow);

//    LogViewer::CServiceHelpTextDlg *p_ServiceHelpTextDlg = new
//            LogViewer::CServiceHelpTextDlg(p_MainWindow);

////    LogViewer::CServiceLogViewer *p_ServiceLogViewer = new
////            LogViewer::CServiceLogViewer(p_MainWindow);

//    LogViewer::CServiceLogViewerDlg *p_ServiceLogViewerDlg = new
//            LogViewer::CServiceLogViewerDlg(p_MainWindow);

////    LogViewer::CSoftwareUpdateLog *p_SoftwareUpdateLog = new
////            LogViewer::CSoftwareUpdateLog(p_MainWindow);

//    LogViewer::CSoftwareUpdateLogDlg *p_SoftwareUpdateLogDlg = new
//            LogViewer::CSoftwareUpdateLogDlg(p_MainWindow);

////    QString SystemLogFileName("HimalayaEvents_12345678_20130509.log");
////    p_SystemLogViewer->AddItem(SystemLogFileName);

//    QString LogfileNamePath = Global::SystemPaths::Instance().GetLogfilesPath() + "/HimalayaEvents_12345678_20130509.log";
//    p_SystemLogViewerDlg->InitDialog(LogfileNamePath);
//    QString Date("2013-02-10");
//    QString TimeStamp("12:03:45.214");
//    QString EventID("1458932");
//    QString Type("Warning");
//    QString Description("Leica/User reagents of the following Leica programs expired");
//    p_SystemLogViewerDlg->AddItem(Date, TimeStamp, EventID, Type, Description);
//    p_SystemLogViewerDlg->SetLogInformation(Date, TimeStamp, EventID, Type, Description);
//    p_SystemLogViewerDlg->CompleteLogInfo();
//    p_SystemLogViewerDlg->FilteredErrorLog();
//    p_SystemLogViewerDlg->FilteredErrorWarningLog();
//    p_SystemLogViewerDlg->FilteredInfoErrorLog();
//    p_SystemLogViewerDlg->FilteredInfoLog();
//    p_SystemLogViewerDlg->FilteredWarningLog();

////    p_ServiceHelpText->AddItem("ServiceHelpText.txt");

//    QString HelpTextFilePath = Global::SystemPaths::Instance().GetSettingsPath() + "/ServiceHelpText.txt";
//    p_ServiceHelpTextDlg->InitDialog(HelpTextFilePath);
//    p_ServiceHelpTextDlg->AddItem("123456", "Stainer GUI process has stopped", "Stainer GUI process has stopped");

////    p_ServiceLogViewer->AddItem("HimalayaService_20130429.log");

//    QString ServiceLogFilePath = Global::SystemPaths::Instance().GetLogfilesPath() + "/HimalayaService_20130429.log";
//    p_ServiceLogViewerDlg->InitDialog(ServiceLogFilePath);
//    p_ServiceLogViewerDlg->AddItem(Date, TimeStamp, Description);

////    p_SoftwareUpdateLog->AddItem("TC000411_Step6_ST8200_SW_Update_Events.log");

//    QString SoftwareUpdateLogFile = Global::SystemPaths::Instance().GetLogfilesPath() + "/TC000411_Step6_ST8200_SW_Update_Events.log";
//    p_SoftwareUpdateLogDlg->InitDialog(SoftwareUpdateLogFile);
//    QString Info("SWUpdate:Slave slave2 has equal or higher HW version compared to 2.1");
//    p_SoftwareUpdateLogDlg->AddItem(Date, TimeStamp, EventID, Info);

}

} // end namespace LogViewer

QTEST_MAIN(LogViewer::CTestLogViewer)
#include "TestLogViewer.moc"

