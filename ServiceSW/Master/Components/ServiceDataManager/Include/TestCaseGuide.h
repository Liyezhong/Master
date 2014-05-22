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

#include "DataManager/Containers/ContainerBase/Include/DataContainerBase.h"
#include "DataManager/Helper/Include/Types.h"
namespace DataManager {

typedef QList<QString> GuideSteps;

class CTestCaseGuide : public CDataContainerBase {
public:

    /****************************************************************************/
    /**
     * \brief Constructor
     */
    /****************************************************************************/
    CTestCaseGuide();

    ~CTestCaseGuide();

    /****************************************************************************/
    /**
     * \brief Get Guide string list all of steps by test case name.
     * \param CaseName = test case name, index = index of Guide
     * \return GuideSteps.
     */
    /****************************************************************************/
    GuideSteps GetGuideSteps(const QString& CaseName, int index);

    /****************************************************************************/
    /**
     * \brief Sets Version number
     * \iparam Value = Value to set
     */
    /****************************************************************************/
    void SetVerion(int Value) {
        m_Version = Value;
    }

    /****************************************************************************/
    /**
     * \brief Gets Version number
     * \return Version number
     */
    /****************************************************************************/
    int GetVersion() {
        return m_Version;
    }

    /****************************************************************************/
    /**
     * \brief Returns the Filename
     * \return File name
     */
    /****************************************************************************/
    QString GetFilename() {
        return m_FileName;
    }

    /****************************************************************************/
    /**
     * \brief Sets data verification mode
     * \iparam Value = Value to set
     */
    /****************************************************************************/
    void SetDataVerificationMode(bool Value) {
        m_DataVerificationMode = Value;
    }

    /****************************************************************************/
    /**
     * \brief Returns Data verification mode
     * \return True or False
     */
    /****************************************************************************/
    bool GetDataVerificationMode() {
        return m_DataVerificationMode;
    }

    /****************************************************************************/
    /**
     * \brief Returns the Data container type
     * \return Data container type
     */
    /****************************************************************************/
    DataContainerType_t GetDataContainerType() { return PARAMETER; }

private:
    QHash<QString, QList<GuideSteps> > m_GuideHash; //!< Stroes all test case guide, key=test case name

    int m_Version;                          //!< Stores the version number of the file
    bool m_DataVerificationMode;            //!< Store the Date verification mode flag
    QString m_FileName;                     //!< Store the file name

    /****************************************************************************/
    /**
     * \brief Init data from config file
     * \return True or False
     */
    /****************************************************************************/
    bool InitData();

    bool SerializeContent(QIODevice& IODevice, bool CompleteData);
    bool DeserializeContent(QIODevice& IODevice, bool CompleteData);
};

} //end of namespace Datamanager

#endif // TESTCASEGUIDE_H
