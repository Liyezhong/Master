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
#include <QApplication>
#include "DiagnosticsManufacturing/Include/TestCaseReporter.h"
#include "Global/Include/Utils.h"
#include "Global/Include/SystemPaths.h"
#include "Global/Include/AdjustedTime.h"
#include "Core/Include/SelectTestOptions.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"
#include "ServiceDataManager/Include/TestCaseGuide.h"

namespace DiagnosticsManufacturing {

CTestCaseReporter::CTestCaseReporter(const QString ModuleName):
    m_ModuleName(ModuleName)
{
    mp_Process = new QProcess;
}

CTestCaseReporter::~CTestCaseReporter()
{
    try {
        delete mp_Process;
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
        TempPath.append("/EndTest");
    }
    else {
        TempPath.append("/StationTest");
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
    /*
    QStringList Params;
    Params<<"-c"<<"4"<<"192.168.25.32";
    mp_Process->start("ping", Params);

    QEventLoop loop;
    (void)loop.connect(mp_Process, SIGNAL(finished(int)), &loop, SLOT(quit()));
    (void)loop.exec();
    if (mp_Process->exitCode() || mp_Process->exitStatus()) {
        qDebug()<<"ping error.";
        return false;
    }
    */
    if (QFile::exists(m_TestReportFile)) {
        //QFileInfo FileInfo(m_TestReportFile);
        //QString DestFile = FileInfo.absolutePath() + "/" + FileInfo.fileName().insert(0, "copy_");
        //QCoreApplication::processEvents();
        //return QFile::copy(m_TestReportFile, DestFile);
        return true;
    }
    else {
        qDebug()<<"CTestCaseReporter:: send report file failed.";
        return false;
    }
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

void CTestCaseReporter::StopSend()
{
    if (mp_Process->isOpen()) {
        mp_Process->terminate();
    }

}

}  // end of namespace DiagnosticsManufacturing

