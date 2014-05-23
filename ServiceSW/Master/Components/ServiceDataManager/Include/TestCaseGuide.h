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

#include <QString>
#include <QIODevice>
#include <QList>
#include <QHash>

namespace DataManager {

typedef QList<QString> GuideSteps;

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
     * \brief Get Guide string list all of steps by test case name.
     * \param CaseName = test case name, index = index of Guide
     * \return GuideSteps.
     */
    /****************************************************************************/
    GuideSteps GetGuideSteps(const QString& CaseName, int index);

private:
    static CTestCaseGuide m_TestCaseGuide;   ///< The one and only instance.
    QHash<QString, QList<GuideSteps> > m_GuideHash; //!< Stroes all test case guide, key=test case name

    /****************************************************************************/
    /**
     * \brief Constructor
     */
    /****************************************************************************/
    CTestCaseGuide();

    ~CTestCaseGuide();

    /****************************************************************************/
    /**
     * \brief Disable copy and assignment
     *
     */
    /****************************************************************************/
    Q_DISABLE_COPY(CTestCaseGuide)
    /****************************************************************************/

    bool DeserializeContent(QIODevice& IODevice, bool CompleteData);
};

} //end of namespace Datamanager

#endif // TESTCASEGUIDE_H
