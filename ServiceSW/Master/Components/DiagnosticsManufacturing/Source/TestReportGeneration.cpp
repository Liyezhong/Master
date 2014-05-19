/****************************************************************************/
/*! \file TestReportGeneration.cpp
 *
 *  \brief CTestReportGeneration class implementation file
 *
 *  \b Description:
 *          This class implements functions to generation module test reports.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-09-26
 *   $Author:  $ Soumya. D
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

#include "DiagnosticsManufacturing/Include/TestReportGeneration.h"
#include "Global/Include/SystemPaths.h"
#include "Global/Include/AdjustedTime.h"
#include <QXmlStreamWriter>

namespace DiagnosticsManufacturing {

/****************************************************************************/
/*!
 *  \brief Constructor
 */
/****************************************************************************/
CTestReportGeneration::CTestReportGeneration()
    : m_TestReportFile("")
    , m_ModuleNumber("")
    , m_SerialNumber("")
    , m_TestResult("")
{

}

/****************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam ModuleNumber = Module number
 *  \iparam SerialNumber = Serial number of the module
 *  \iparam TestResult = Test result
 */
/****************************************************************************/
CTestReportGeneration::CTestReportGeneration(QString ModuleNumber, QString SerialNumber, QString TestResult)
    : m_TestReportFile("")
    , m_ModuleNumber(ModuleNumber)
    , m_SerialNumber(SerialNumber)
    , m_TestResult(TestResult)
{

}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CTestReportGeneration::~CTestReportGeneration()
{

}

/****************************************************************************/
/*!
 *  \brief Creates a test report xml file
 *  \iparam ModuleInfo = Module Information
 *  \iparam ModuleTestReport = Module Test report information
 *  \return true or false
 */
/****************************************************************************/
bool CTestReportGeneration::CreateTestReportFile(QList<QString> ModuleInfo, QHash<QString, QString> ModuleTestReport)
{
    bool Result = false;
    QString TempFilePath = Global::SystemPaths::Instance().GetLogfilesPath();
    QDateTime DateTime = Global::AdjustedTime::Instance().GetCurrentDateTime();
    QString CurrentDateTime = DateTime.toString("yyyyMMddhhmmss");

    QString TestReportName = m_ModuleNumber + "_" + m_SerialNumber + "_" + CurrentDateTime + "_" + m_TestResult;
    m_TestReportFile =  TempFilePath + "/" + TestReportName + ".xml";

    QFile File(m_TestReportFile);
    if (!File.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "open file failed in Write: " << endl;
        return false;
    }

    Result = WriteTestReport(File, ModuleInfo, ModuleTestReport);

    File.close();

    return Result;
}

/****************************************************************************/
/*!
 *  \brief Creates a test report xml file
 *  \iparam IODevice = Instance of the IODevice might be Buffer or File
 *  \iparam ModuleInfo = Module information list
 *  \iparam ModuleTestReport = Module Test report information
 *  \return true or false
 */
/****************************************************************************/
bool CTestReportGeneration::WriteTestReport(QIODevice &IODevice, QList<QString>ModuleInfo,
                                            QHash<QString, QString> ModuleTestReport)
{
    QXmlStreamWriter XmlStreamWriter;
    XmlStreamWriter.setDevice(&IODevice);

    XmlStreamWriter.setAutoFormatting(true);
    XmlStreamWriter.writeStartDocument();

    XmlStreamWriter.writeStartElement("ModuleTestReport");

    QString ModuleName = ModuleTestReport.value("ModuleName", NULL);
    XmlStreamWriter.writeAttribute("Name", ModuleName);

    QString SerialNumber = ModuleTestReport.value("SerialNumber", NULL);
    XmlStreamWriter.writeAttribute("SerialNumber", SerialNumber);

    QString TestResult = ModuleTestReport.value("TestResult", NULL);
    XmlStreamWriter.writeAttribute("TestResult", TestResult);

    for (int i = 3; i < ModuleInfo.count(); i++) {
        QString Key = ModuleInfo.at(i);
        QString Value = ModuleTestReport.value(Key, NULL);

        XmlStreamWriter.writeStartElement("Test");
        XmlStreamWriter.writeAttribute("Name", Key);
        XmlStreamWriter.writeCharacters(Value);
        XmlStreamWriter.writeEndElement();
    }
    XmlStreamWriter.writeEndElement();
    XmlStreamWriter.writeEndDocument(); // End of Document

    return true;
}
}   // end of namespace DiagnosticsManufacturing
