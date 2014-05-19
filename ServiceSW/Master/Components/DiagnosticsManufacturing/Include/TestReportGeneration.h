/****************************************************************************/
/*! \file TestReportGeneration.h
 *
 *  \brief CTestReportGeneration class definition for Manufacturing SW
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

#ifndef DIAGNOSTICSMANUFACTURING_TESTREPORTGENERATION_H
#define DIAGNOSTICSMANUFACTURING_TESTREPORTGENERATION_H

#include <QObject>
#include <QHash>

namespace DiagnosticsManufacturing {

/***********************************************************************************/
/**
 * \brief This class provides functions to generate test report and send to server.
 */
/***********************************************************************************/
class CTestReportGeneration : public QObject
{

public:    
    explicit CTestReportGeneration();
    explicit  CTestReportGeneration(QString ModuleNumber, QString SerialNumber, QString TestResult);
    ~CTestReportGeneration();

    bool CreateTestReportFile(QList<QString> ModuleInfo, QHash<QString, QString> ModuleTestReport);
    bool WriteTestReport(QIODevice& IODevice, QList<QString>ModuleInfo, QHash<QString, QString> ModuleTestReport);

private:
    QString m_TestReportFile;               //!< Stores Test report file name.
    QString m_ModuleNumber;                 //!< Stores module number
    QString m_SerialNumber;                 //!< Stores serial number
    QString m_TestResult;                   //!< Stores test result
};

} // end of namespace DiagnosticsManufacturing

#endif // DIAGNOSTICSMANUFACTURING_TESTREPORTGENERATION_H
