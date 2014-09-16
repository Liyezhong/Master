/****************************************************************************/
/*! \file TestCaseFactory.h
 *
 *  \brief Definition file for class CTestCaseFactory.
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

#ifndef TESTCASEFACTORY_H
#define TESTCASEFACTORY_H

#include <QString>
#include <QHash>
#include <QList>
#include <QReadWriteLock>
#include <QDateTime>
#include <QIODevice>
#include <QTime>
#include "ServiceDataManager/Include/TestCase.h"
#include "Core/Include/ServiceDefines.h"

namespace DataManager {

/****************************************************************************/
/*!
 *  \brief  This class implements TestCase Factory
 *  Reads All Test Case parameter
 */
/****************************************************************************/
class CTestCaseFactory
{
public:

    /****************************************************************************/
    /**
     * \brief Get reference to instance.
     * \return  Reference to instance.
     */
    /****************************************************************************/
    static CTestCaseFactory &Instance(){
        return m_TestCaseFactory;
    }

    /****************************************************************************/
    /**
     * \brief Get reference to instance for service.
     * \return  Reference to instance for service.
     */
    /****************************************************************************/
    static CTestCaseFactory &ServiceInstance(){
        return m_ServiceTestCaseFactory;
    }

    /****************************************************************************/
    /**
     * \brief Initialization test case parameters from config file.
     * \param FileName = config file
     * \return false if failed.
     */
    /****************************************************************************/
    bool InitData(QString FileName);

    /****************************************************************************/
    /**
     * \brief Get TestCase by test case name.
     * \param CaseName = test case name
     * \return Pointer of TestCase
     */
    /****************************************************************************/
    CTestCase* GetTestCase(const QString& CaseName);

    /****************************************************************************/
    /**
     * \brief Overload Get TestCase by test case Id.
     * \param Id = test case Id
     * \return Pointer of TestCase
     */
    /****************************************************************************/
    CTestCase* GetTestCase(Service::ModuleTestCaseID Id);

    /****************************************************************************/
    /**
     * \brief Get parameter by test case name and parameter name.
     * \iparam CaseName = test case name
     * \iparam ParamName = parameter name
     * \return Value of parameter.
     */
    /****************************************************************************/
    QString GetParameter(const QString& CaseName, const QString& ParamName);

    /****************************************************************************/
    /**
     * \brief Get Test Case list by module name.
     * \param ModuleName = module name
     * \return list of Test Case.
     */
    /****************************************************************************/
    QList<CTestCase*> GetModuleTestCase(const QString& ModuleName);

private:
    static CTestCaseFactory m_TestCaseFactory;                   ///< The one and only instance.
    static CTestCaseFactory m_ServiceTestCaseFactory;            ///< The one and only instance for service
    QHash<QString, CTestCase*> m_TestCases;                      ///< Store all test case information
    QHash<Service::ModuleTestCaseID, QString> m_TestCaseIDHash;  ///< Stores all test case ID, key=ID, value=test case name

    /****************************************************************************/
    /**
     * \brief Disable copy and assignment
     *
     */
    /****************************************************************************/
    Q_DISABLE_COPY(CTestCaseFactory)
    /****************************************************************************/

    /****************************************************************************/
    /**
     * \brief Default constructor.
     */
    /****************************************************************************/
    CTestCaseFactory();

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CTestCaseFactory();

    /****************************************************************************/
    /*!
     *  \brief Reads the CModule Data from QIODevice
     *
     *  \iparam XmlStreamReader = Xml reader to read the XML contents
     *
     *  \return True or False
     */
    /****************************************************************************/
    bool DeserializeContent(QIODevice& IODevice);

    /****************************************************************************/
    /**
     * \brief Save test case id
     * \!param TestCaseName = the name of test case
     */
    /****************************************************************************/
    void SavetoIDHash(const QString &TestCaseName);
};

} //end of namespace Datamanager

#endif // TESTCASEFACTORY_H
