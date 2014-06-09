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
     * \param ParamName = parameter name , ParamValue = parameter value.
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
     * \param Key = result name , Value = result value.
     */
    /****************************************************************************/
    void AddResult(const QString& Key, const QString& Value);

    /****************************************************************************/
    /**
     * \brief Get result for this test case.
     */
    /****************************************************************************/
    QMap<QString, QString>& GetResult();

    /*********************Service*******************************************************/
    /**
     * \brief Set status for this test case.
     * \param Status = true or false (Pass or FAIL).
     */
    /****************************************************************************/
    void SetStatus(bool Status){
        m_Status = Status;
    }

private:
    QString m_ModuleName;                  ///< Store module name
    QString m_CaseName;                    ///< Store test case name
    bool m_Status;                         ///< Store test case Status true-PASS or false-FAIL
    QMap<QString, QString> m_Parameters;   ///< Store test case parameters
    QMap<QString, QString> m_Results;      ///< Store test Case results

    /****************************************************************************/
    /**
     * \brief Default constructor.
     */
    /****************************************************************************/
    CTestCase(QString ModuleName, QString CaseName, bool Status = false);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CTestCase();

};

} //end of namespace Datamanager

#endif // TESTCASE_H
