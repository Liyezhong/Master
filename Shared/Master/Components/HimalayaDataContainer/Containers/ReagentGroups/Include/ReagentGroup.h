/****************************************************************************/
/*! \file ReagentGroup.h
 *
 *  \brief ReagentGroup defination.
 *
 *   $Date:    $ 2013-01-30
 *   $Author:  $ Swati Tiwari
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
#ifndef DATAMANAGER_REAGENTGROUP_H
#define DATAMANAGER_REAGENTGROUP_H

#include <QXmlStreamReader>

#include "DataManager/Helper/Include/Types.h"
#include "DataManager/Helper/Include/Helper.h"


namespace DataManager {
//!< Forward declration to avoid header file inclusion
class CReagentGroupList;

/****************************************************************************/
/*!
 *  \brief  This class implements Reagent Groups
 */
/****************************************************************************/
class CReagentGroup
{
    friend class CDataReagentGroupList; //!< Made friend so that serialize and deserialize methods are accessible

private:
    bool SerializeContent(QXmlStreamWriter& XmlStreamWriter, bool CompleteData);
    bool DeserializeContent(QXmlStreamReader& XmlStreamReader, bool CompleteData);

    ListOfErrors_t    m_ListOfErrors;   ///< This will hold Hash table of Error ID's.
    ErrorHash_t m_ErrorHash;    //!< Event List for GUI and for logging purpose. This member is not copied when using copy constructor/Assignment operator


protected:
    QString m_GroupID;                      //!< Reagent Group ID
    QString m_GroupName;                    //!< Reagent Name
    QString m_Color;                        //!< Reagent Color
    bool m_Locked;                          //!< locked stats for reagent , true user cannot chnage its parameters

public:
    CReagentGroup();
    CReagentGroup(const QString ProgramID);
    ~CReagentGroup();

    CReagentGroup(const CReagentGroup&ReagentGroup);
    friend QDataStream& operator << (QDataStream& OutDataStream, const CReagentGroup& ReagentGroup);
    friend QDataStream& operator >> (QDataStream& InDataStream, CReagentGroup& ReagentGroup);
    CReagentGroup& operator = (const CReagentGroup&);

    /******************** INLINE FuNCTIONS **************************************/
    /****************************************************************************/
    /*!
     *  \brief Gets the Locked State of the particular reagent
     *
     *  \return true or false
     */
    /****************************************************************************/
    bool GetLockState() const {return m_Locked;}
    /****************************************************************************/
    /*!
     *  \brief Sets the Locked State of the particular reagent
     *
     *  \iparam Value = bool type , either true or false
     *
     *  \return
     */
    /****************************************************************************/
    void SetLockState(const bool Value){m_Locked = Value;}

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
     *  \brief Gets the Name of the particular reagent
     *
     *  \return Name String
     */
    /****************************************************************************/
    QString GetReagentGroupName() const {return m_GroupName;}

    /****************************************************************************/
    /*!
     *  \brief Sets the Group ID of the particular reagent
     *
     *  \iparam Value = Desired Reagent Group ID
     *
     *  \return
     */
    /****************************************************************************/
    void SetReagentGroupName(const QString Value){m_GroupName = Value.trimmed();}

    /****************************************************************************/
    /*!
     *  \brief Gets the Group ID of the particular reagent
     *
     *  \return
     */
    /****************************************************************************/

    QString GetGroupColor() const{return m_Color;}
    /****************************************************************************/
    /*!
     *  \brief Sets the Group ID of the particular reagent
     *
     *  \iparam Value = Desired Reagent Group ID
     *
     *  \return
     */
    /****************************************************************************/
    void SetGroupColor(const QString Value){m_Color = Value.trimmed();}

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

#endif // DATAMANAGER_REAGENTGROUP_H
