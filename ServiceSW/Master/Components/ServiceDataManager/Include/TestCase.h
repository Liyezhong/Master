/****************************************************************************/
/*! \file TestCase.h
 *
 *  \brief Definition file for class CTestCase.
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

#ifndef TESTCASE_H
#define TESTCASE_H

#include <QString>
#include <QMap>

namespace DataManager {

class CTestCaseFactory;

/****************************************************************************/
/*!
 *  \brief  This class implements TestCase
 *  Reads Test Case parameter, Write Test case results
 */
/****************************************************************************/
class CTestCase
{
    friend class CTestCaseFactory;
public:

    /****************************************************************************/
    /**
     * \brief Get Parameter by parameter name.
     * \param ParamName = parameter name
     * \return value of parameter.
     */
    /****************************************************************************/
    QString GetParameter(const QString& ParamName);

    /****************************************************************************/
    /**
     * \brief Add parameter key-value for this test case.
     * \iparam ParamName = parameter name
     * \iparam ParamValue = parameter value.
     */
    /****************************************************************************/
    void SetParameter(const QString& ParamName, const QString& ParamValue);
    /****************************************************************************/
    /**
     * \brief Getnerate text for report.
     * \return Text of case information.
     */
    /****************************************************************************/
    QString GenReport();

    /****************************************************************************/
    /**
     * \brief Add result key-value for this test case.
     * \iparam Key   = result name
     * \iparam Value = result value.
     */
    /****************************************************************************/
    void AddResult(const QString& Key, const QString& Value);

    /****************************************************************************/
    /**
     * \brief Get result for this test case.
     * \return result of map
     */
    /****************************************************************************/
    QMap<QString, QString>& GetResult();

    /****************************************************************************/
    /**
     * \brief Reset result for this test case.
     */
    /****************************************************************************/
    void ResetResult();

    /*********************Service*******************************************************/
    /**
     * \brief Set status for this test case.
     * \param Status = true or false (Pass or FAIL).
     */
    /****************************************************************************/
    void SetStatus(bool Status){
        if (Status) {
            m_Status = 1;
        }
        else {
            m_Status = 0;
        }
    }

private:
    QString m_ModuleName;                  ///< Store module name
    QString m_CaseName;                    ///< Store test case name
    int m_Status;                          ///< Store test case Status 1-PASS or false-FAIL
    QMap<QString, QString> m_Parameters;   ///< Store test case parameters
    QMap<QString, QString> m_Results;      ///< Store test Case results

    /****************************************************************************/
    /**
     * \brief Default constructor.
     */
    /****************************************************************************/
    CTestCase(QString ModuleName, QString CaseName);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CTestCase();

};

} //end of namespace Datamanager

#endif // TESTCASE_H
