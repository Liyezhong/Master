/****************************************************************************/
/*! \file TestCaseGuide.h
 *
 *  \brief Definition file for class CTestCaseGuide.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-5-19
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
#ifndef TESTCASEGUIDE_H
#define TESTCASEGUIDE_H

#include "Core/Include/ServiceDefines.h"
#include <QString>
#include <QIODevice>
#include <QList>
#include <QHash>

namespace DataManager {

typedef QList<QString> GuideSteps; //!< list of guide steps

/****************************************************************************/
/*!
 *  \brief  This class implements TestCaseGuide
 *  Reads Test Case guides
 */
/****************************************************************************/
class CTestCaseGuide
{
public:

    /****************************************************************************/
    /**
     * \brief Get reference to instance.
     * \return  Reference to instance.
     */
    /****************************************************************************/
    static CTestCaseGuide &Instance(){
        return m_TestCaseGuide;
    }

    /****************************************************************************/
    /**
     * \brief Initialization test case guide from config file.
     * \param FileName = config file
     * \return false if failed.
     */
    /****************************************************************************/
    bool InitData(QString FileName);

    /****************************************************************************/
    /**
     * \brief To clear all test guide.
     *
     */
    /****************************************************************************/
    void Clear();

    /****************************************************************************/
    /**
     * \brief Get Guide string list all of steps by test case name.
     * \iparam CaseName = test case name
     * \iparam Index = index of Guide
     * \return GuideSteps.
     */
    /****************************************************************************/
    GuideSteps GetGuideSteps(const QString& CaseName, int Index);

    /****************************************************************************/
    /**
     * \brief Get test case name by test case id.
     * \param Id = test case id
     * \return Test case name.
     */
    /****************************************************************************/
    QString GetTestCaseName(Service::ModuleTestCaseID Id);

    /****************************************************************************/
    /**
     * \brief Get test case id by test case name.
     * \param TestCaseName = test case name
     * \return Test case id.
     */
    /****************************************************************************/
    Service::ModuleTestCaseID GetTestCaseId(const QString &TestCaseName);

    /****************************************************************************/
    /**
     * \brief Get test case discription by test case id.
     * \param Id = test case id
     * \return Test case display text.
     */
    /****************************************************************************/
    QString GetTestCaseDescription(Service::ModuleTestCaseID Id);

private:
    static CTestCaseGuide m_TestCaseGuide;   ///< The one and only instance.
    QHash<QString, QList<GuideSteps> > m_GuideHash; //!< Stroes all test case guide, key=test case name
    QHash<QString, QString> m_TestCaseDescriptionHash; //!< Stores all test case display text, key=test case name
    QHash<Service::ModuleTestCaseID, QString> m_TestCaseIDHash;  //!< Stores all test case ID, key=ID, value=test case name

    /****************************************************************************/
    /**
     * \brief Constructor
     */
    /****************************************************************************/
    CTestCaseGuide();

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CTestCaseGuide();

    /****************************************************************************/
    /**
     * \brief Disable copy and assignment
     *
     */
    /****************************************************************************/
    Q_DISABLE_COPY(CTestCaseGuide)
    /****************************************************************************/

    /****************************************************************************/
    /*!
     *  \brief Reads the CModule Data from QIODevice
     *
     *  \iparam XmlStreamReader = Xml reader to read the XML contents
     *  \iparam CompleteData = bool type if true writes Complete data of object
     *
     *  \return True or False
     */
    /****************************************************************************/
    bool DeserializeContent(QIODevice& IODevice, bool CompleteData);


    /****************************************************************************/
    /**
     * \brief Save test case id
     * \!param TestCaseName = the name of test case
     */
    /****************************************************************************/
    void SavetoIDHash(const QString &TestCaseName);
};

} //end of namespace Datamanager

#endif // TESTCASEGUIDE_H
