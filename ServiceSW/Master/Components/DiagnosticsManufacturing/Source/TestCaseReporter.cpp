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
#include "ServiceDataManager/Include/TestCaseFactory.h"

namespace DiagnosticsManufacturing {

CTestCaseReporter::CTestCaseReporter(const QString ModuleName, const QString SerialNumber):
    m_ModuleName(ModuleName),
    m_SerialNumber(SerialNumber)
{
    mp_Process = new QProcess;

    CONNECTSIGNALSLOT(this, StopSend(), mp_Process, terminate());
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
    QString TempFilePath = Global::SystemPaths::Instance().GetTempPath();
    QDateTime DateTime = Global::AdjustedTime::Instance().GetCurrentDateTime();
    QString CurrentDateTime = DateTime.toString("yyyyMMddhhmmss");
    QString ReportName = m_SerialNumber + "_" + m_ModuleName + "_" + CurrentDateTime;
    QString FileName = TempFilePath + "/" + ReportName + ".txt";
    QFile File(FileName);

    if (!File.open(QFile::WriteOnly | QFile::Text )) {
        qDebug() << "CTestCaseReporter:Open file to write failed " << FileName;
        return false;
    }

    QTextStream TestStream(&File);
    FillReport(TestStream);

    m_TestReportFile = FileName;

    File.close();

    return true;
}

bool CTestCaseReporter::SendReportFile()
{
    QStringList Params;

    Params<<"-c"<<"1"<<"192.168.25.33";
    mp_Process->start("ping", Params);
    if (mp_Process->waitForFinished(2000) && mp_Process->exitCode()) {
        qDebug()<<"ping error.";
    }
    else {
        qDebug()<<"The exit code is "<<mp_Process->exitCode();//0 ok, 1 fail
        return false;
    }
    return true;

    /*
    if (QFile::exists(m_TestReportFile)) {
        QFileInfo FileInfo(m_TestReportFile);
        QString DestFile = FileInfo.absolutePath() + "/" + FileInfo.fileName().insert(0, "copy_");
        return QFile::copy(m_TestReportFile, DestFile);
    }
    else {
        qDebug()<<"CTestCaseReporter:: send report file failed.";
        return false;
    }
    */
}

void CTestCaseReporter::FillReport(QTextStream& TextStream)
{
    QDateTime DateTime = Global::AdjustedTime::Instance().GetCurrentDateTime();
    QString CurrentDateTime = DateTime.toString("yyyy/MM/dd-hh:mm:ss");
    TextStream<<QString("Module Name: %1\n").arg(m_ModuleName);
    TextStream<<QString("Time Stamp: %1\n").arg(CurrentDateTime);
    TextStream<<QString("Serial Number: %1\n").arg(m_SerialNumber);

    QList<DataManager::CTestCase*> TestCases = DataManager::CTestCaseFactory::Instance().GetModuleTestCase(m_ModuleName);
    QList<DataManager::CTestCase*>::iterator itr = TestCases.begin();

    for (; itr != TestCases.end(); ++itr) {
        TextStream<<(*itr)->GenReport();
    }
}

}  // end of namespace DiagnosticsManufacturing

