/****************************************************************************/
/*! \file TestCaseReporter.h
 *
 *  \brief Definition file for class CTestCaseReporter.
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
#ifndef TESTCASEREPORTER_H
#define TESTCASEREPORTER_H

#include <QString>
#include <QTextStream>
#include <QFile>
#include <QProcess>
#include "MainMenu/Include/WaitDialog.h"

namespace DiagnosticsManufacturing {

class CTestCaseReporter: public QObject
{
    Q_OBJECT
public:
    /****************************************************************************/
    /**
     * \brief Default constructor.
     */
    /****************************************************************************/
    CTestCaseReporter(const QString ModuleName, const QString SerialNumber);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CTestCaseReporter();

    /****************************************************************************/
    /**
     * \brief Generate report file.
     * \return false if generate file error.
     */
    /****************************************************************************/
    bool GenReportFile();

    /****************************************************************************/
    /**
     * \brief send report file to server.
     * \return false if send file error.
     */
    /****************************************************************************/
    bool SendReportFile();
signals:

    void StopSend();
private:

    void FillReport(QTextStream& TextStream);

    QString m_TestReportFile;               //!< Stores Test report file name.
    QString m_ModuleName;                   //!< Stores module name
    QString m_SerialNumber;                 //!< Stores serial number

    QProcess* mp_Process;
    //MainMenu::CWaitDialog *mp_WaitDlg;


};

} // end namespace DiagnosticsManufacturing

#endif // TESTCASEREPORTER_H
