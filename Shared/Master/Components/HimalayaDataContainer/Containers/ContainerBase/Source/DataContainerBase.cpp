/****************************************************************************/
/*! \file DataContainerBase.cpp
 *
 *  \brief Implementation of DataContainerBase.
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

#include "HimalayaDataContainer/Containers/ContainerBase/Include/DataContainerBase.h"

#include <QFile>
#include <QUuid>
#include <QDebug>

#include "Global/Include/GlobalDefines.h"
#include "HimalayaDataContainer/Containers/ContainerBase/Include/VerifierInterface.h"


namespace DataManager {

typedef QList<VerifierInterface *> ListOfVerifiers_t;

/****************************************************************************/
/*
 *  \brief
 *
 *  \param[in] p_Verifier pointer to the verifier.
 *
 *  \return true, if the verifier is added;
 *          false, if the verifier already exists.
 *
 *  \warning DON'T implement(override) in derived classes!
 *
 */
/****************************************************************************/
class DataContainerBase::DataContainerBaseData
{
public:
    DataContainerBaseData(void)
        : m_FileName()
        , m_ListOfErrors()
        , m_VerifierList()
    {
    }

    QString m_FileName;
    // This will hold Hash table of Error ID's from Container and it's verifiers.
    ListOfErrors_t    m_ListOfErrors;
    // Verifiers applied to the data
    ListOfVerifiers_t m_VerifierList;
};

DataContainerBase::DataContainerBase(void)
    : m_Data(new DataContainerBaseData)
{
}

DataContainerBase::~DataContainerBase(void)
{
    delete m_Data;
}

/****************************************************************************/
/*!
 *  \brief  Gets the file name.
 *
 *  \return associate file name
 *
 *  \warning DON'T implement(override) in derived classes!
 *
 */
/****************************************************************************/
const QString & DataContainerBase::GetFilename(void) const
{
    return m_Data->m_FileName;
}

/****************************************************************************/
/*!
 *  \brief  Adds a verifier to the data contaner, if the verifies in not
 *          yet in the list.
 *
 *  \param[in] p_Verifier pointer to the verifier.
 *
 *  \return true, if the verifier is added;
 *          false, if the verifier already exists.
 *
 *  \warning DON'T implement(override) in derived classes!
 *
 */
/****************************************************************************/
bool DataContainerBase::AddVerifier(VerifierInterface* p_Verifier)
{
    // check verifier pointer
    if (0 == p_Verifier)
    {
        qDebug() << "### CDataContainerBase::AddVerifier failed: "
                    "invalid Verifier.";

        return false;
    }

    // add every verifier only once
    if (-1 != m_Data->m_VerifierList.indexOf(p_Verifier))
    {
        qDebug() << "### CDataContainerBase::AddVerifier failed: "
                    "Verifier already exists.";

        return false;
    }

    m_Data->m_VerifierList.append(p_Verifier);

    return true;
}

/****************************************************************************/
/*!
 *  \brief  Gets a list of errors which occur during verification.
 *
 *  \return a list of errors with error IDs and arguements.
 *
 *  \warning DON'T implement(override) in derived classes!
 *
 */
/****************************************************************************/
ListOfErrors_t& DataContainerBase::GetErrorList(void)
{
    return  m_Data->m_ListOfErrors;
}

/****************************************************************************/
/*!
 *  \brief  Reset the error list
 *
 *  \warning DON'T implement(override) in derived classes!
 *
 */
/****************************************************************************/
void DataContainerBase::ResetLastErrors(void)
{
    for (ListOfErrors_t::iterator itr = m_Data->m_ListOfErrors.begin();
         m_Data->m_ListOfErrors.end() != itr;
         ++itr)
    {
        ErrorHash_t *p_Error = *itr;
        if (0 != p_Error)
        {
            p_Error->clear();
        }
    }

    m_Data->m_ListOfErrors.clear();
}

/****************************************************************************/
/*!
 *  \brief  Verifies this data container with all verifiers
 *
 *  \return true, if the data container is valid;
 *          false, if the verification fails.
 *
 *  \warning DON'T implement(override) in derived classes!
 *
 */
/****************************************************************************/
bool DataContainerBase::VerifyData(void)
{
    if (!this->DoLocalVerification())
    {
        qDebug() << "### CDataContainerBase::Local verifiaction failed";
        return false;
    }

    if (!this->DoGroupVerification())
    {
        qDebug() << "### CDataContainerBase::Group verifiaction failed";
        return false;
    }

    return true;
}

/****************************************************************************/
/*!
 *  \brief  Verifies this data container with local verifiers
 *
 *  \return true, if the data container is valid;
 *          false, if the verification fails.
 *
 *  \warning DON'T implement(override) in derived classes!
 *
 */
/****************************************************************************/
bool DataContainerBase::DoLocalVerification(void)
{
    for (ListOfVerifiers_t::iterator itr = m_Data->m_VerifierList.begin();
         m_Data->m_VerifierList.end() != itr;
         ++itr)
    {
        VerifierInterface *p_Verifier = *itr;
        // this should be unnecessary
        if (0 != p_Verifier)
        {
            if (p_Verifier->IsLocalVerifier())
            {
                p_Verifier->ResetLastErrors();

                if (!p_Verifier->VerifyData(this))
                {
                    // append the string list with verifier error list
                    m_Data->m_ListOfErrors.append(&(p_Verifier->GetErrors()));
                    return false;
                }
            }
        }
    }

    return true;
}

/****************************************************************************/
/*!
 *  \brief  Verifies this data container with group verifiers
 *
 *  \return true, if the data container is valid;
 *          false, if the verification fails.
 *
 *  \warning DON'T implement(override) in derived classes!
 *
 */
/****************************************************************************/
bool DataContainerBase::DoGroupVerification(void)
{
    bool result = true;

    for (ListOfVerifiers_t::iterator itr = m_Data->m_VerifierList.begin();
         m_Data->m_VerifierList.end() != itr;
         ++itr)
    {
        VerifierInterface *p_Verifier = *itr;
        if (0 != p_Verifier)
        {
            if (!p_Verifier->IsLocalVerifier())
            {
                // clear the error before verifying the data
                p_Verifier->ResetLastErrors();
                // verify the data
                if (!p_Verifier->VerifyData(this))
                {
                    result = false;
                    // append the string list with verifier error list
                    m_Data->m_ListOfErrors.append(&(p_Verifier->GetErrors()));
                }
            }
        }
    }

    return result;
}

/****************************************************************************/
/*!
 *  \brief Read the data into Container from a file
 *
 *  \param[in] fileNamem Name of the input file
 *
 *  \return true, if succeed to read the file and verify the data;
 *          false, if the following error happens
 *                 1, the file does not exist
 *                 2, fail to read data
 *                 3,
 *
 *  \warning DON'T implement(override) in derived classes!
 */
/****************************************************************************/
bool DataContainerBase::Read(QString fileName)
{
    if (!QFile::exists(fileName))
    {
        qDebug() << "### CDataContainerBase::Read: "
                    "cannot find file "
                 << fileName;
        return false;
    }

    // open the file
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "### CDataContainerBase::Read: "
                    "fail to open file "
                 << fileName;
        return false;
    }

    if (!DeserializeContent(file))
    {
        qDebug() << "### CDataContainerBase::Read: "
                    "fail to read content from file "
                 << fileName;

        file.close();
        return false;
    }

    file.close();

    // save file name on success
    m_Data->m_FileName = fileName;

    return true;
}

/****************************************************************************/
/*!
 *  \brief Writes the data from Container to the file
 *
 *  \param[in] fileName Name of the Output File
 *
 *  \return true, succeed to write into the output file;
 *          false, fail to open the output file, or fail to write to the file
 *
 *  \warning DON'T implement(override) in derived classes!
 */
/****************************************************************************/
bool DataContainerBase::Write(QString fileName)
{
    // if file already exists, delete it
    if (QFile::exists(fileName))
    {
        (void)QFile::remove(fileName);
    }

    // create the file
    QFile file(fileName);
    (void)file.open(QFile::WriteOnly | QFile::Text);

    if (!SerializeContent(file))
    {
        qDebug() << "### CDataContainerBase::Write: fail to write to file: "
                 << fileName;
        file.close();

        return false;
    }

    file.close();

    return true;
}

/****************************************************************************/
/*!
 *  \brief Writes the data from Container to the associate file.
 *         the data is originally read from the file.
 *
 *  \return true, succeed to write into the file;
 *          false, if one of the following error happens:
 *                 1, no associate file;
 *                 2, fail to open the temporary output file;
 *                 3, fail to write content to the temporary file;
 *                 4, fail to rename the temporary file.
 *
 *  \warning DON'T implement(override) in derived classes!
 */
/****************************************************************************/
bool DataContainerBase::Write(void)
{
    QString fileName = GetFilename();

    if (fileName.isEmpty())
    {
        qDebug() << "### CDataContainerBase::Write: "
                    "no file is read";

        return false;
    }

    // create an temporary file
    QString directory = fileName.left(fileName.lastIndexOf('/') + 1);
    QString tempFileName = directory + QUuid::createUuid().toString();
    tempFileName += QString(".xml");

    QFile file(tempFileName);
    (void)file.open(QFile::WriteOnly | QFile::Text);

    if (!SerializeContent(file))
    {
        qDebug() << "### CDataContainerBase::Write: "
                    "fail to write to file "
                 << tempFileName;
        file.close();

        return false;
    }

    file.close();

    // if Old file already exists, first remove it
    if (QFile::exists(fileName))
    {
        (void)QFile::remove(fileName);
    }

    (void)QFile::rename(tempFileName, fileName);

    return true;
}

} // end of namespace datamanager

