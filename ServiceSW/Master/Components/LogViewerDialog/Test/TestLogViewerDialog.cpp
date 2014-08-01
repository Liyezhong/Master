/****************************************************************************/
/*! \file TestLogViewerDialog.cpp
 *
 *  \brief Unit test for LogViewerDialog menu
 *
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2014-04-21
 *  $Author:    $ Dixiong Li
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
#include <LogViewerDialog/Include/SystemLogViewerDlg.h>
#include <LogViewerDialog/Include/LogContentDlg.h>
#include <QObject>
//#include <QSignalSpy>

namespace LogViewer {

class CTestLogViewerDialog : public QObject
{
    Q_OBJECT
    
public:

private Q_SLOTS:
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
     * \brief Test LogViewerDialog object
     */
    /****************************************************************************/
    void utTestLogViewerDialog();
};//end Class CTestLogViewerDialog

/****************************************************************************/
void CTestLogViewerDialog::initTestCase() {
}

/****************************************************************************/
void CTestLogViewerDialog::init() {
}

/****************************************************************************/
void CTestLogViewerDialog::cleanup() {
}

/****************************************************************************/
void CTestLogViewerDialog::cleanupTestCase() {
}

/****************************************************************************/
void CTestLogViewerDialog::utTestLogViewerDialog()
{
    Global::SystemPaths::Instance().SetLogfilesPath("../../../Main/Build/Logfiles");
    Global::SystemPaths::Instance().SetSettingsPath("../../../Main/Build/Settings");

    QString RecActionPath = Global::SystemPaths::Instance().GetSettingsPath() + "/RecoveryActionText.txt";
    //QString LogFileNamePath = Global::SystemPaths::Instance().GetLogfilesPath() + "/HimalayaEvents_12345678_20140416.log";

    QVERIFY(!RecActionPath.isEmpty());
    //QVERIFY(!LogFileNamePath.isEmpty());

    QStringList HeaderLabels;
    QList<int> Columns;
    HeaderLabels.append(QApplication::translate("Core::CStartup", "Error", 0, QApplication::UnicodeUTF8));
    HeaderLabels.append(QApplication::translate("Core::CStartup", "Description", 0, QApplication::UnicodeUTF8));
    HeaderLabels.append(QApplication::translate("Core::CStartup", "Recovery Action Text", 0, QApplication::UnicodeUTF8));
    Columns.append(0);
    Columns.append(1);
    Columns.append(2);

    MainMenu::CMainWindow *p_MainWindow = new MainMenu::CMainWindow();

    CLogContentDlg *p_LogContentDlg = new LogViewer::CLogContentDlg(HeaderLabels, Columns, p_MainWindow);
    //CLogFilter *p_LogFilter = new LogViewer::CLogFilter(LogFileNamePath, Columns, true);
    CRecoveryActionFilter * p_RecoveryActionFilter = new LogViewer::CRecoveryActionFilter(RecActionPath);
    LogViewer::CSystemLogViewerDlg *p_SystemLogViewerDlg = new LogViewer::CSystemLogViewerDlg(p_MainWindow);


    //Test CLogContentDlg class member functions//
    //QVERIFY(p_LogContentDlg->InitDialog(LogFileNamePath));

    //Test CLogFilter class member functions
    //QVERIFY(p_LogFilter->InitData());
    //QStandardItemModel* p_StandardItemModel = p_LogFilter->GetItemModel();
    //QMap<QString, QString> FileInfo = p_LogFilter->GetFileInfo();
    //QVERIFY(p_StandardItemModel!=NULL);
   // QVERIFY(!FileInfo.empty());

    //Test CRecoveryActionFilter class member functions
    QString EventId = "17825801";
    p_RecoveryActionFilter->GetRecoveryActionText(EventId);


    //Test CSystemLogViewerDlg class member functions
    //QVERIFY(p_SystemLogViewerDlg->InitDialog(LogFileNamePath));
    //p_SystemLogViewerDlg->RecoveryActionDialog();
    //p_SystemLogViewerDlg->ShowRecoveryActionDetails();
    //p_SystemLogViewerDlg->CompleteLogInfo();
    //p_SystemLogViewerDlg->FilteredInfoLog();
    //p_SystemLogViewerDlg->FilteredWarningLog();
    //p_SystemLogViewerDlg->FilteredErrorLog();
    //p_SystemLogViewerDlg->FilteredUndefinedLog();

    delete p_SystemLogViewerDlg;
    p_SystemLogViewerDlg = NULL;
    delete p_RecoveryActionFilter;
    p_RecoveryActionFilter = NULL;
    //delete p_LogFilter;
    //p_LogFilter = NULL;
    delete p_LogContentDlg;
    p_LogContentDlg = NULL;

}

}// end namespace LogViewer

//QTEST_APPLESS_MAIN(CTestSystemLogViewerDlg)
QTEST_MAIN(LogViewer::CTestLogViewerDialog)

#include "TestLogViewerDialog.moc"
