/****************************************************************************/
/*! \file DashboardStation.h
 *
 *  \brief Definition for DashboardStation class.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 16-Jan-2013
 *   $Author:  $ Santhosha Kumar Raju
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2013 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#ifndef DATAMANAGER_DASHBOARDSTATION_H
#define DATAMANAGER_DASHBOARDSTATION_H

#include "DataManager/Helper/Include/Types.h"
#include "DataManager/Helper/Include/Helper.h"
#include "Global/Include/GlobalDefines.h"


namespace DataManager {

// forward declaration
class CDashboardDataStationList;
/****************************************************************************/
/*!
 *  \brief CDashboardStation class containing the station attributes
 */
/****************************************************************************/
class CDashboardStation
{
    friend class CDashboardDataStationList;
private:
    QString                     m_DashboardStationID;
    QString                     m_DashboardStationName;
    bool                        m_ParaffinBath;
    QString                     m_ReagentID;
    QDate                       m_ReagentExchangeDate;
    int                         m_ReagentActualCassettes;
    int                         m_ReagentActualCycles;
    QString                     m_ReagentStatus;
    DashboardStationStatus_t    m_DashboardStationStatus;
    DashboardStationType_t      m_DashboardStationType;
    bool SerializeContent(QXmlStreamWriter& XmlStreamWriter, bool CompleteData);
    bool DeserializeContent(QXmlStreamReader& XmlStreamReader, bool CompleteData);

public:
    CDashboardStation();
    CDashboardStation(const QString ID);
    CDashboardStation(const CDashboardStation& ExistingStation);
    void CopyFromOther(const CDashboardStation& Station);
    friend QDataStream& operator <<(QDataStream& OutDataStream, const CDashboardStation& Station);
    friend QDataStream& operator >>(QDataStream& InDataStream, CDashboardStation& Station);
    CDashboardStation& operator=(const CDashboardStation&);
    void ResetData(void);

//    /******************** INLINE FuNCTIONS **************************************/
    /****************************************************************************/
    /*!
     *  \brief Get's the DashboardStation Type
     *
     *  \return DashboardStation Type Value
     */
    /****************************************************************************/
    DashboardStationType_t GetDashboardStationType()
    {
        return m_DashboardStationType;
    }

    /****************************************************************************/
    /*!
     *  \brief Get's the Station ID
     *
     *  \return Station Id String
     */
    /****************************************************************************/
    QString  GetDashboardStationID()
    {
        return m_DashboardStationID;
    }

    /****************************************************************************/
    /*!
     *  \brief Set's the Station ID
     *
     *  \iparam Value = station ID string
     *
     *  \return
     */
    /****************************************************************************/
    void SetDashboardStationID(const QString Value)
    {
        m_DashboardStationID = Value.trimmed();
    }

    /****************************************************************************/
    /*!
     *  \brief Get's the Station Name
     *
     *  \return Station Name String
     */
    /****************************************************************************/
    QString  GetDashboardStationName()
    {
        return m_DashboardStationName;
    }

    /****************************************************************************/
    /*!
     *  \brief Set's the Station Name
     *
     *  \iparam Value = station Name string
     *
     *  \return
     */
    /****************************************************************************/
    void SetDashboardStationName(const QString Value)
    {
        m_DashboardStationName = Value.trimmed();
    }


    /****************************************************************************/
    /*!
     *  \brief Set's the Station ID
     *
     *  \iparam Value = station ID string
     *
     *  \return
     */
    /****************************************************************************/
    void SetDashboardParaffinBath(const bool Value)
    {
        m_ParaffinBath = Value;
    }

    /****************************************************************************/
    /*!
     *  \brief Get's the Station Name
     *
     *  \return Station Name String
     */
    /****************************************************************************/
    bool  GetDashboardParaffinBath()
    {
        return m_ParaffinBath;
    }

    /****************************************************************************/
    /*!
     *  \brief Get's the Station Name
     *
     *  \return Station Name String
     */
    /****************************************************************************/
    QString  GetDashboardReagentID()
    {
        return m_ReagentID;
    }

    /****************************************************************************/
    /*!
     *  \brief Set's the Station Name
     *
     *  \iparam Value = station Name string
     *
     *  \return
     */
    /****************************************************************************/
    void SetDashboardReagentID(const QString Value)
    {
        m_ReagentID = Value.trimmed();
    }

    /****************************************************************************/
    /*!
     *  \brief Get's the Station Name
     *
     *  \return Station Name String
     */
    /****************************************************************************/
    QString  GetDashboardReagentStatus()
    {
        return m_ReagentStatus;
    }

    /****************************************************************************/
    /*!
     *  \brief Set's the Station Name
     *
     *  \iparam Value = station Name string
     *
     *  \return
     */
    /****************************************************************************/
    void SetDashboardReagentStatus(const QString Value)
    {
        m_ReagentStatus = Value.trimmed();
    }


    /****************************************************************************/
    /*!
     *  \brief Get's the Station Name
     *
     *  \return Station Name String
     */
    /****************************************************************************/
    QDate  GetDashboardReagentExchangeDate()
    {
        return m_ReagentExchangeDate;
    }

    /****************************************************************************/
    /*!
     *  \brief Set's the Station Name
     *
     *  \iparam Value = station Name string
     *
     *  \return
     */
    /****************************************************************************/
    void SetDashboardReagentExchangeDate(const QDate Value)
    {
        m_ReagentExchangeDate = Value;
    }

    /****************************************************************************/
    /*!
     *  \brief Get's the Station Name
     *
     *  \return Station Name String
     */
    /****************************************************************************/
    qint32  GetDashboardReagentActualCassettes()
    {
        return m_ReagentActualCassettes;
    }

    /****************************************************************************/
    /*!
     *  \brief Set's the Station Name
     *
     *  \iparam Value = station Name string
     *
     *  \return
     */
    /****************************************************************************/
    void SetDashboardReagentActualCassettes(const qint32 Value)
    {
        m_ReagentActualCassettes = Value;
    }

    /****************************************************************************/
    /*!
     *  \brief Get's the Station Name
     *
     *  \return Station Name String
     */
    /****************************************************************************/
    qint32  GetDashboardReagentActualCycles()
    {
        return m_ReagentActualCycles;
    }

    /****************************************************************************/
    /*!
     *  \brief Set's the Station Name
     *
     *  \iparam Value = station Name string
     *
     *  \return
     */
    /****************************************************************************/
    void SetDashboardReagentActualCycles(const qint32 Value)
    {
        m_ReagentActualCycles = Value;
    }

    /****************************************************************************/
    /*!
     *  \brief Gets the Station Status Value ( ex: Empty, Full, Partially Filled, Not Used )
     *
     *  \return Station Status
     */
    /****************************************************************************/
    DashboardStationStatus_t GetDashboardStationStatus()
    {
        return m_DashboardStationStatus;
    }

    /****************************************************************************/
    /*!
     *  \brief Sets the Station Status Value
     *
     *  \iparam Value = Station Status Value ( ex: Empty, Full, Partially Filled, Not Used )
     *
     *  \return
     */
    /****************************************************************************/
    void SetDashboardStationStatus(DashboardStationStatus_t eValue)
    {
        m_DashboardStationStatus = eValue;
    }


};

} // end of namespace

#endif // CDASHBOARDSTATION_H
