/****************************************************************************/
/*! \file ProgramSequence.h
 *
 *  \brief ProgramSequence defination.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-12-26
 *   $Author:  $ N.Kamath
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
#ifndef PROGRAMSEQUENCE_H
#define PROGRAMSEQUENCE_H

#include <QXmlStreamReader>

#include "DataManager/Helper/Include/Types.h"
#include "DataManager/Helper/Include/Helper.h"


namespace DataManager {
//!< Forward declration to avoid header file inclusion
class CProgramSequenceList;

/****************************************************************************/
/*!
 *  \brief  This class implements program sequence
 */
/****************************************************************************/
class CProgramSequence
{
    friend class CProgramSequenceList; //!< Made friend so that serialize and deserialize methods are accessible

private:
    bool SerializeContent(QXmlStreamWriter& XmlStreamWriter, bool CompleteData);
    bool DeserializeContent(QXmlStreamReader& XmlStreamReader, bool CompleteData);

    ListOfErrors_t    m_ListOfErrors;   ///< This will hold Hash table of Error ID's.
    ErrorHash_t m_ErrorHash;    //!< Event List for GUI and for logging purpose. This member is not copied when using copy constructor/Assignment operator


protected:
    QString m_ProgramID;            //!< Program Id
    bool m_Used;                    //!< whether program is used or not
    quint32 m_Counter;              //!< No of times the program is used
    bool m_StartCapable;            //!< Program capable to start or not
    QString m_StartCapableResult;            //!< Result string of BathLayout generator and Main
    bool m_StartCapableDynamic;     //!< Program capable to start or not when in Processing state

public:
    CProgramSequence();
    CProgramSequence(const QString ProgramID);
    CProgramSequence(const QString ProgramID, bool Used, quint32 Counter, bool StartCapable, QString m_StartCapableResult);
    ~CProgramSequence();

    CProgramSequence(const CProgramSequence&);
    friend QDataStream& operator << (QDataStream& OutDataStream, const CProgramSequence& ProgramSequence);
    friend QDataStream& operator >> (QDataStream& InDataStream, CProgramSequence& ProgramSequence);
    CProgramSequence& operator = (const CProgramSequence&);

    /****************************************************************************/
    /*!
     *  \brief  Retrieve the program id
     *
     *  \return ProgramID
     */
    /****************************************************************************/
    QString GetID() const {return m_ProgramID;}
    /****************************************************************************/
    /*!
     *  \brief Sets the program ID
     *
     *  \iparam Value = Program ID
     */
    /****************************************************************************/
    void SetID(const QString Value) { m_ProgramID = Value; }

    /****************************************************************************/
    /*!
     *  \brief Retrieve if the program is already used.
     *  \return true - program used. false - program used
     */
    /****************************************************************************/
    bool GetUsedStatus() const {return m_Used;}
    /****************************************************************************/
    /*!
     *  \brief Sets program used status
     *
     *  \iparam UsedStatus = true - used , false - unused
     */
    /****************************************************************************/
    void SetUsedStatus(bool UsedStatus) { m_Used = UsedStatus; }

    /****************************************************************************/
    /*!
     *  \brief Retrieve the number of times the program has been used
     *  \return Used Count
     */
    /****************************************************************************/
    quint32 GetCounterValue() const {return m_Counter;}
    /****************************************************************************/
    /*!
     *  \brief Sets the count of number of times the program is used
     *
     *  \iparam CounterValue = Number of times the program is used
     */
    /****************************************************************************/
    void SetCounterValue(quint32 CounterValue) { m_Counter = CounterValue; }

    /****************************************************************************/
    /*!
     *  \brief Retrieve the start capable status of the program
     *  \return true - program ready to start, false  - not ready
     */
    /****************************************************************************/
    bool GetStartCapableStatus() const {return m_StartCapable; }

    /****************************************************************************/
    /*!
     *  \brief Sets whether program is start capable
     *
     *  \iparam StartCapableStatus = true -startable ,false - not ready to start
     */
    /****************************************************************************/
    void SetStartCapableStatus(bool StartCapableStatus) {m_StartCapable = StartCapableStatus; }

    /****************************************************************************/
    /*!
     *  \brief Retrieve the Bathlayout generator result
     *  \return BLG result
     */
    /****************************************************************************/
    QString GetStartCapableResult() const {return m_StartCapableResult;}
    /****************************************************************************/
    /*!
     *  \brief Sets Bathlayout generator result
     *
     *  \iparam BLGResult = comma separated result code. e.g. "1,2" , "0"
     */
    /****************************************************************************/
    void SetStartCapableResult(QString StartCapableResult){m_StartCapableResult = StartCapableResult;}

    /****************************************************************************/
    /*!
     *  \brief Retrieve the start capable status of the program
     *  \return true - program ready to start, false  - not ready
     */
    /****************************************************************************/
    bool GetStartCapableDynamicStatus() const {return m_StartCapableDynamic; }

    /****************************************************************************/
    /*!
     *  \brief Sets whether program is start capable
     *
     *  \iparam StartCapableStatus = true -startable ,false - not ready to start
     */
    /****************************************************************************/
    void SetStartCapableDynamicStatus(bool StartCapableStatus) {m_StartCapableDynamic = StartCapableStatus; }

    /****************************************************************************/
    /*!
     *  \brief  Appends the error hash in the error list
     *
     *  \iparam p_ErrorHash = pointer to error hash
     *
     */
    /****************************************************************************/
    /*! \warning DON'T implement(override) in derived classes! */
    void SetErrorList(ErrorHash_t *p_ErrorHash)
    {
        m_ListOfErrors.append(p_ErrorHash);
    }

    /****************************************************************************/
    /*!
     *  \brief      Get the last error
     *  \warning    Error List is deleted/cleared after retrieval.
     *  \return     List of errors
     */
    /****************************************************************************/
    ListOfErrors_t& GetErrorList()
    {
        return m_ListOfErrors;
    }

    /****************************************************************************/
    /*!
     *  \brief  Reset the error list
     */
    /****************************************************************************/
    void ResetLastErrors();

};

}//End of namespace DataManager

#endif // PROGRAMSEQUENCE_H
