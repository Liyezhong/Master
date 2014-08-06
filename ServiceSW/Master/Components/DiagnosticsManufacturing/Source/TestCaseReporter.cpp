/****************************************************************************/
/*! \file TestCaseReporter.cpp
 *
 *  \brief Implementation file for class CTestCaseReporter.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-5-21
 *   $Author:  $ Dixiong.li
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#include <QList>
#include <QDateTime>
#include "DiagnosticsManufacturing/Include/TestCaseReporter.h"
#include "Global/Include/Utils.h"
#include "Global/Include/SystemPaths.h"
#include "Global/Include/AdjustedTime.h"
#include "Core/Include/SelectTestOptions.h"
#include "Core/Include/CMessageString.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"
#include "ServiceDataManager/Include/TestCaseGuide.h"

namespace DiagnosticsManufacturing {

CTestCaseReporter::CTestCaseReporter(const QString ModuleName, Core::CServiceGUIConnector *p_DataConnector,
                                     MainMenu::CMainWindow *p_Parent):
    m_ModuleName(ModuleName),
    mp_DataConnector(p_DataConnector),
    mp_IEClient(NULL),
    m_ReportDir(""),
    QObject(p_Parent)
{
    mp_MessageDlg = new MainMenu::CMessageDlg(p_Parent);
    mp_WaitDlg    = new MainMenu::CWaitDialog(p_Parent);
    mp_WaitDlg->setModal(true);
    InitIEClient();
}

CTestCaseReporter::~CTestCaseReporter()
{
    try {
        delete mp_MessageDlg;
        delete mp_WaitDlg;
        if (mp_IEClient) {
            delete mp_IEClient;
        }
    }
    catch(...) {

    }
}

bool CTestCaseReporter::GenReportFile()
{
    QDir TempDir;
    QString TempPath = Global::SystemPaths::Instance().GetTempPath();
    if (!TempDir.exists(TempPath) && !TempDir.mkdir(TempPath)) {
        qDebug()<<"CTestCaseReporter: create temp dir failed:"<<TempPath;
        return false;
    }

    if (Core::CSelectTestOptions::GetCurTestMode() == Core::MANUFACTURAL_ENDTEST) {
        TempPath.append("/endtest");
    }
    else {
        TempPath.append("/stationtest");
    }

    if (!TempDir.exists(TempPath) && !TempDir.mkdir(TempPath)) {
        qDebug()<<"CTestCaseReporter: create temp dir failed"<<TempPath;
        return false;
    }
    QDateTime DateTime = Global::AdjustedTime::Instance().GetCurrentDateTime();
    QString CurrentDateTime = DateTime.toString("yyyyMMddhhmmss");
    if (m_SerialNumber.isEmpty()) {
        qDebug()<<"CTestCaseReporter: the serial number is empty";
        return false;
    }
    QString serialNumber = m_SerialNumber;
    serialNumber.replace(QRegExp("[/.]"), "_");
    QString ReportName = m_ModuleName + "_" + serialNumber + "_" + CurrentDateTime;
    QString FileName = TempPath + "/" + ReportName + ".txt";
    QFile File(FileName);

    if (!File.open(QFile::WriteOnly | QFile::Text )) {
        qDebug() << "CTestCaseReporter:Open file to write failed " << FileName;
        return false;
    }

    QTextStream TestStream(&File);
    WriteReportFile(TestStream);

    m_TestReportFile = FileName;

    File.close();

    return true;
}

bool CTestCaseReporter::SendReportFile()
{
    //ShowWaitDialog();
    bool Result = false;
    QString Msg("");

    mp_MessageDlg->SetTitle(Service::CMessageString::MSG_TITLE_SEND_REPORT);
    mp_MessageDlg->SetButtonText(1, Service::CMessageString::MSG_BUTTON_OK);
    mp_MessageDlg->HideButtons();

    if (!mp_IEClient) {
        qDebug()<<"invalid ie client.";
        return false;
    }
    if (!mp_IEClient->PerformHostReachableTest()) {
        Msg = Service::CMessageString::MSG_SERVER_IP_CANNOT_REACHABLE;
        goto Send_Finished;
    }

    if (!mp_IEClient->PerformAccessRightsCheck(m_ReportDir)) {
        Msg = Service::CMessageString::MSG_SERVER_FOLDER_CANNOT_ACCESS;
        goto Send_Finished;
    }
    if (QFile::exists(m_TestReportFile)) {
        QString ReportPath = m_ReportDir;
        if (Core::CSelectTestOptions::GetCurTestMode() == Core::MANUFACTURAL_ENDTEST) {
            ReportPath.append("/endtest");
        }
        else {
            ReportPath.append("/stationtest");
        }
        if (mp_IEClient->SendReprotFile(m_TestReportFile, ReportPath)) {
            Result = true;
            Msg = Service::CMessageString::MSG_DIAGNOSTICS_SEND_REPORT_OK;
        }
        else {
            Msg = Service::CMessageString::MSG_SERVER_SEND_REPORT_FALIED;
        }
    }
    else {
        Msg = Service::CMessageString::MSG_DIAGNOSTICS_REPORT_FILE_NOT_EXISTS;
    }

Send_Finished:

    //mp_WaitDlg->close();
    mp_MessageDlg->SetText(Msg);
    if (Result) {
        mp_MessageDlg->SetIcon(QMessageBox::Information);
    }
    else {
        mp_MessageDlg->SetIcon(QMessageBox::Critical);
    }
    (void)mp_MessageDlg->exec();

    return Result;
}

void CTestCaseReporter::WriteReportFile(QTextStream& TextStream)
{
    QDateTime DateTime = Global::AdjustedTime::Instance().GetCurrentDateTime();
    QString CurrentDateTime = DateTime.toString("yyyy/MM/dd-hh:mm:ss");
    TextStream<<QString("Module Name: %1\n").arg(m_ModuleName);
    TextStream<<QString("Time Stamp: %1\n").arg(CurrentDateTime);
    TextStream<<QString("Serial Number: %1\n").arg(m_SerialNumber);

    QList<Service::ModuleTestCaseID>::iterator itr = m_TestCaseList.begin();
    for (; itr != m_TestCaseList.end(); ++itr) {
        QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(*itr);
        DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
        if (p_TestCase) {
            TextStream<<p_TestCase->GenReport();
        }
        else {
            qDebug()<<"CTestCaseReporter:: get test case failed the name is "<<TestCaseName;
        }
    }

}

void CTestCaseReporter::InitIEClient()
{
    if (mp_DataConnector && mp_DataConnector->GetServiceParameters()) {
        QString Username  = mp_DataConnector->GetServiceParameters()->GetUserName();
        QString IPAddress = mp_DataConnector->GetServiceParameters()->GetProxyIPAddress();
        m_ReportDir       = mp_DataConnector->GetServiceParameters()->GetTestReportFolderPath();

        mp_IEClient = new NetworkClient::IENetworkClient(IPAddress, Username, Global::SystemPaths::Instance().GetScriptsPath());
    }
}

void CTestCaseReporter::ShowWaitDialog()
{
    //QEventLoop loop;
    mp_WaitDlg->SetText(Service::CMessageString::MSG_DIAGNOSTICS_SENDING);
    mp_WaitDlg->HideAbort();
    mp_WaitDlg->show();

    //connect(mp_WaitDlg, SIGNAL(finished(int)), &m_Loop, SLOT(quit()));
    //m_Loop.exec();
}

void CTestCaseReporter::StopSend()
{

}

}  // end of namespace DiagnosticsManufacturing

