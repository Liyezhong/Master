/****************************************************************************/
/*! \file Reagent.h
 *
 *  \brief Definition file for class CReagent.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-01-17
 *   $Author:  $ Swati Tiwari
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2012 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DATAMANAGER_REAGENT_H
#define DATAMANAGER_REAGENT_H

#include <QString>
#include <QXmlStreamReader>

#include "DataManager/Helper/Include/Types.h"
#include "DataManager/Helper/Include/Helper.h"

namespace DataManager {

/****************************************************************************/
/*!
 *  \brief CReagent class containing the reagents attributes
 */
/****************************************************************************/
class CReagent
{
    friend class CDataReagentList;

private:
    bool SerializeContent(QXmlStreamWriter& XmlStreamWriter, bool CompleteData);
    bool DeserializeContent(QXmlStreamReader& XmlStreamReader, bool CompleteData);
    bool ReadUsageLimitation(QXmlStreamReader& XmlStreamReader);
    bool ReadHeatingInformation(QXmlStreamReader& XmlStreamReader);

    ListOfErrors_t    m_ListOfErrors;   ///< This will hold Hash table of Error ID's.
    ErrorHash_t m_ErrorHash;    //!< Event List for GUI and for logging purpose. This member is not copied when using copy constructor/Assignment operator

protected:
    int m_MaxTime;                          //!< Maximum  allowable time  for Reagent
    bool m_Visible;                         //!< Visible state of reagent
    QString m_Name;                         //!< Reagent Name
    int m_CassettesUntilChange;             //!< maximum Cassettes for reagent to replace
    int m_CyclesUntilChange;                //!< maximum Cycles for reagent to replace
    int m_DaysUntilChange;                  //!< maximum Days for reagent to replace
    QString m_ReagentID;                    //!< Reagent ID
    QString m_GroupID;                      //!< Reagent Group ID
//    bool m_Locked;                          //!< locked stats for reagent , true user cannot chnage its parameters
public:
    CReagent();
    CReagent(const QString ID);

    CReagent(const CReagent&);
    friend QDataStream& operator <<(QDataStream& OutDataStream, const CReagent& Reagent);
    friend QDataStream& operator >>(QDataStream& InDataStream, CReagent& Reagent);
    CReagent& operator=(const CReagent&);

    /******************** INLINE FuNCTIONS **************************************/
    /****************************************************************************/
    /*!
     *  \brief Gets the reagent Type either L,S or U
     *
     *  \return Reagent Type Leica, User or Special
     */
    /****************************************************************************/
    ReagentType_t GetReagentType() const;

//    /****************************************************************************/
//    /*!
//     *  \brief Gets the Locked State of the particular reagent
//     *
//     *  \return true or false
//     */
//    /****************************************************************************/
//    bool GetLockState() const {return m_Locked;}

//    /****************************************************************************/
//    /*!
//     *  \brief Sets the Locked State of the particular reagent
//     *
//     *  \iparam Value = bool type , either true or false
//     *
//     *  \return
//     */
//    /****************************************************************************/
//    void SetLockState(const bool Value){m_Locked = Value;}

    /****************************************************************************/
    /*!
     *  \brief Gets the Visible State for the particular reagent
     *
     *  \return true or false
     */
    /****************************************************************************/
    bool GetVisibleState() const {return m_Visible;}

    /****************************************************************************/
    /*!
     *  \brief Sets the Visible State of the particular reagent
     *
     *  \iparam Value = bool type , either true or false
     *
     *  \return
     */
    /****************************************************************************/
    void SetVisibleState(const bool Value){m_Visible = Value;}     

    /****************************************************************************/
    /*!
     *  \brief Gets the Group ID of the particular reagent
     *
     *  \return
     */
    /****************************************************************************/

    QString GetGroupID() const{return m_GroupID;}

    /****************************************************************************/
    /*!
     *  \brief Sets the Group ID of the particular reagent
     *
     *  \iparam Value = Desired Reagent Group ID
     *
     *  \return
     */
    /****************************************************************************/
    void SetGroupID(const QString Value){m_GroupID = Value.trimmed();}

    /****************************************************************************/
    /*!
     *  \brief Gets the maximum no. of cycles for the particular reagent
     *
     *  \return
     */
    /****************************************************************************/

    int GetMaxCycles() {return m_CyclesUntilChange;}

    /****************************************************************************/
    /*!
     *  \brief Sets the maximum no. of cycles for of the particular reagent
     *
     *  \iparam Value = maximum no. of cycles
     *
     *  \return
     */
    /****************************************************************************/
    void SetMaxCycles(const int Value){m_CyclesUntilChange = Value;}

    /****************************************************************************/
    /*!
     *  \brief Gets the maximum no. of Cassettes for the particular reagent
     *
     *  \return
     */
    /****************************************************************************/

    int GetMaxCassettes() {return m_CassettesUntilChange;}

    /****************************************************************************/
    /*!
     *  \brief Sets the maximum no. of Cassettes for of the particular reagent
     *
     *  \iparam Value = maximum no. of Cassettes
     *
     *  \return
     */
    /****************************************************************************/
    void SetMaxCassettes(const int Value)
    {
        m_CassettesUntilChange = Value;
    }

    /****************************************************************************/
    /*!
     *  \brief Get's the Reagent ID
     *
     *  \return Reagent Id String
     */
    /****************************************************************************/
    QString  GetReagentID() const {return m_ReagentID;}

    /****************************************************************************/
    /*!
     *  \brief Sets the Reagent ID to the value passed
     *
     *  \iparam Value = Reagent ID string
     *
     *  \return
     */
    /****************************************************************************/
    void SetReagentID(const QString Value){m_ReagentID = Value.trimmed();}

    /****************************************************************************/
    /*!
     *  \brief Gets the Name of the particular reagent
     *
     *  \return Name String
     */
    /****************************************************************************/
    QString GetReagentName() const {return m_Name;}

    /****************************************************************************/
    /*!
     *  \brief Sets the Name for the particular reagent
     *
     *  \iparam Value = Name
     *
     *  \return
     */
    /****************************************************************************/
    void SetReagentName(const QString Value){m_Name = Value.trimmed();}

    /****************************************************************************/
    /*!
     *  \brief Gets Maximum Time in Days of the particular reagent
     *
     *  \return Time strings like "4d"
     */
    /****************************************************************************/
    int GetMaxDays() const {return m_DaysUntilChange;}      // returns

    /****************************************************************************/
    /*!
     *  \brief Sets the Max Time in terms of days for the particular reagent
     *
     *  \iparam Value = time strings like "4d "
     *
     *  \return
     */
    /****************************************************************************/
    void SetMaxDays(const int Value) { m_DaysUntilChange = Value;}

//    /****************************************************************************/
//    /*!
//     *  \brief Gets Maximum Time in Days of the particular reagent
//     *
//     *  \return Time strings like "4d"
//     */
//    /****************************************************************************/
//    QString GetMaxTime() const {return Helper::ConvertSecondsToTimeString(m_DaysUntilChange);}      // returns

//    /****************************************************************************/
//    /*!
//     *  \brief Sets the Max Time in terms of days for the particular reagent
//     *
//     *  \iparam Value = time strings like "4d "
//     *
//     *  \return
//     */
//    /****************************************************************************/
//    void SetMaxTime(const QString Value) {m_DaysUntilChange = Helper::ConvertTimeStringToSeconds(Value.trimmed());}  // for

    /****************************************************************************/
    /*!
     *  \brief Checks if the particular reagent is Leica Reagent or not
     *
     *  \return true or false
     */
    /****************************************************************************/
    bool IsLeicaReagent() const {return (GetReagentType() == LEICA_REAGENT);}

    /****************************************************************************/
    /*!
     *  \brief Checks if the particular reagent is User Reagent or not
     *
     *  \return true or false
     */
    /****************************************************************************/
    bool IsUserReagent() const {return (GetReagentType() == USER_REAGENT);}    

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

} // namespace DataManager

#endif // DATAMANAGER_REAGENT_H
