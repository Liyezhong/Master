/****************************************************************************/
/*! \file DataContainerBase.h
 *
 *  \brief Definition file for class DataContainerBase.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-01-07
 *  $Author:    $ R. Wu
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

#ifndef DATAMANAGER_DATACONTAINER_BASE_H
#define DATAMANAGER_DATACONTAINER_BASE_H

#include "Global/Include/GlobalDefines.h"
#include "HimalayaDataContainer/Helper/Include/Types.h"

namespace DataManager {

class VerifierInterface;

/****************************************************************************/
/**
 * \brief    Base class for all the containers
 */
/****************************************************************************/
class DataContainerBase
{
public:
    DataContainerBase(void);
    virtual ~DataContainerBase(void);

    /****************************************************************************/
    /*!
     *  \brief  Gets the data container type.
     */
    /****************************************************************************/
    virtual DataContainerType_t GetDataContainerType(void) const = 0;

    /****************************************************************************/
    /*
     *  \brief  Gets the file name.
     *
     *  \warning DON'T implement(override) in derived classes!
     *
     */
    /****************************************************************************/
    const QString &GetFilename(void) const;

    /****************************************************************************/
    /*
     *  \brief  Adds a verifier to the data contaner
     *
     *  \warning DON'T implement(override) in derived classes!
     *
     */
    /****************************************************************************/
    bool AddVerifier(VerifierInterface* p_Verifier);

    /****************************************************************************/
    /*
     *  \brief  Gets a list of errors which occur during verification.
     *
     *  \warning DON'T implement(override) in derived classes!
     *
     */
    /****************************************************************************/
    ListOfErrors_t& GetErrorList(void);

    /****************************************************************************/
    /*
     *  \brief  Reset the error list
     *
     *  \warning DON'T implement(override) in derived classes!
     *
     */
    /****************************************************************************/
    void ResetLastErrors(void);

    /****************************************************************************/
    /*
     *  \brief  Verifies this data container with all verifiers
     *
     *  \warning DON'T implement(override) in derived classes!
     *
     */
    /****************************************************************************/
    bool VerifyData(void);

    /****************************************************************************/
    /*
     *  \brief  Verifies this data container with local verifiers
     *
     *  \warning DON'T implement(override) in derived classes!
     *
     */
    /****************************************************************************/
    bool DoLocalVerification(void);

    /****************************************************************************/
    /*
     *  \brief  Verifies this data container with group verifiers
     *
     *  \warning DON'T implement(override) in derived classes!
     *
     */
    /****************************************************************************/
    bool DoGroupVerification(void);

    /****************************************************************************/
    /*
     *  \brief Read the data into Container from a file
     *
     *  \warning DON'T implement(override) in derived classes!
     */
    /****************************************************************************/
    bool Read(QString fileName);

    /****************************************************************************/
    /*
     *  \brief Writes the data from Container to the file
     *
     *  \warning DON'T implement(override) in derived classes!
     */
    /****************************************************************************/
    bool Write(QString fileName);

    /****************************************************************************/
    /*
     *  \brief Writes the data from Container to the associate file.
     *         the data is originally read from the file.
     *
     *  \warning DON'T implement(override) in derived classes!
     */
    /****************************************************************************/
    bool Write(void);

protected:
    /****************************************************************************/
    /*!
     *  \brief Serializes the xml content
     *
     *  \param[in] ioDevice Instance of the IODevice might be Buffer or File
     *  \param[in] completeData bool type if true writes Complete data of object
     *
     *  \return true, if succeeds; false, if fail
     */
    /****************************************************************************/
    virtual bool SerializeContent(QIODevice& ioDevice) const = 0;

    /****************************************************************************/
    /*!
     *  \brief  Deserializes the xml content
     *
     *  \param[in] ioDevice Instance of the IODevice might be Buffer or File
     *  \param[in] completeData bool type if true writes Complete data of object
     *
     *  \return true, if succeeds; false, if fail
     */
    /****************************************************************************/
    virtual bool DeserializeContent(QIODevice& ioDevice) = 0;

private:
    class DataContainerBaseData;
    DataContainerBaseData *m_Data;
};

} // namespace DataManager

#endif // DATAMANAGER_DATACONTAINER_BASE_H
