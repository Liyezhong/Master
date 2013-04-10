/****************************************************************************/
/*! \file DashboardDataStationList.h
 *
 *  \brief Definition for DashboardDataStationList class.
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

#ifndef DATAMANAGER_DASHBOARDDATASTATIONLIST_H
#define DATAMANAGER_DASHBOARDDATASTATIONLIST_H

#include <QHash>
#include <QList>
#include <QMutableHashIterator>
#include <QReadWriteLock>
#include <QBuffer>

#include "DataManager/Helper/Include/Types.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Include/DashboardStation.h"
#include "DataManager/Containers/ContainerBase/Include/DataContainerBase.h"
#include "Global/Include/GlobalDefines.h"

namespace DataManager {

class CReagent;

typedef QHash<QString, CDashboardStation*> ListOfDashboardStation_t;      //!< List of stations

/****************************************************************************/
/*!
 *  \brief CDashboardDataStationList class Inherited from CDataContainerBase
 */
/****************************************************************************/
class CDashboardDataStationList : public CDataContainerBase
{
private:
    int m_Version;                                                    //!< version of the file read
    QString m_Filename;                                               //!< filename of the file
    bool m_DataVerificationMode;                                      //!< Verification mode flag , verify the Container
    ListOfDashboardStation_t m_DashboardStationList;                  //!< Station list
    ListOfIDs_t m_OrderedListOfDashboardStationIDs;                   //!< Ordered list of DashboardStation ID
    QReadWriteLock* mp_ReadWriteLock;                                 //!< Reade Write lock

    void CopyFromOther(const CDashboardDataStationList &Other);
    bool SerializeContent(QIODevice& IODevice, bool CompleteData);
    bool DeserializeContent(QIODevice& IODevice, bool CompleteData);
    bool ReadDashboardStations(QXmlStreamReader& XmlStreamReader, bool CompleteData);
    bool DeleteAllDashboardStations();
    void AddDashboardStationWithoutVerification(const CDashboardStation *p_DashboardStation);
    void Swap(const CDashboardDataStationList &Other);
    ListOfDashboardStation_t & GetDashboardStationList()
    {
        return m_DashboardStationList;
    }
    ListOfIDs_t & GetOrderedListOfDashboardStationIDs()
    {
        return m_OrderedListOfDashboardStationIDs;
    }
    QReadWriteLock* GetLock()
    {
        return mp_ReadWriteLock;
    }

    /****************************************************************************/
    /*!
     *  \brief Creates a Dashboard Station with the Station ID passed
     *
     *  \iparam StationID = Station ID String
     *
     *  \iparam StationType = Station Type Enum Value
     *
     *  \return CDashboardStation Class Instance
     */
    /****************************************************************************/
    CDashboardStation* CreateStation(const QString StationID)
    {
        return new CDashboardStation(StationID);
    }

    /****************************************************************************/
    /*!
     *  \brief Sets the version of The Station list File
     *
     *  \iparam Value = Version number
     *
     *  \return
     */
    /****************************************************************************/
    void SetVersion(const int Value)
    {
        m_Version = Value;
    }

public:
    CDashboardDataStationList();
    ~CDashboardDataStationList();

    CDashboardDataStationList(const CDashboardDataStationList&);
    friend QDataStream& operator <<(QDataStream& OutDataStream, const CDashboardDataStationList& StationList);
    friend QDataStream& operator >>(QDataStream& InDataStream, CDashboardDataStationList& StationList);
    CDashboardDataStationList& operator=(const CDashboardDataStationList &StationList);

    void Init();
    bool Read(QString Filename);

    DashboardStationType_t GetDashboardStationType(const QString DashboardStationID);
    DashboardStationType_t GetDashboardStationType(const unsigned int Index);
    bool IsDashboardStationExists(const QString DashboardStationID);

    /****************************************************************************/
    /*!
     *  \brief Gets the CDashboardStation Instance associated with the particular Station ID
     *
     *  \iparam StationID = Station ID String
     *
     *  \return CDashboardStation Class Instance
     */
    /****************************************************************************/
    CDashboardStation* GetDashboardStation(const QString DashboardStationID)
    {
        return m_DashboardStationList.value(DashboardStationID, NULL);
    }
    bool GetDashboardStation(const QString DashboardStationID, CDashboardStation& DashboardStation);
    bool GetDashboardStation(const unsigned int Index, CDashboardStation& DashboardStation);
    const CDashboardStation* GetDashboardStation(const unsigned int Index); // uses order index
    bool AddDashboardStation(const CDashboardStation* p_DashboardStation);

    bool UpdateDashboardStation(const CDashboardStation* p_DashboardStation);
    bool DeleteDashboardStation(const QString DashboardStationID);   // uses unique reagent ID
    bool DeleteDashboardStation(const unsigned int Index);  // uses order index

    bool UpdateStationsByReagentDelete(const QString& ReagentID);
    const QString& FindStationByReagent(const CReagent* pReagent, bool bFindNewestOne,
                                                                   Global::RMSOptions_t rmsMode) const;

    /******************** INLINE FUNCTIONS **************************************/
    /****************************************************************************/
    /*!
     *  \brief Get's the fileName of the dashboard station list
     *
     *  \return File name String
     */
    /****************************************************************************/
    QString GetFilename()
    {
        return m_Filename;
    }

    /****************************************************************************/
    /*!
     *  \brief Get's the Count of Dashboard Stations available in the list
     *
     *  \return Dashboard Stations Count
     */
    /****************************************************************************/
    int GetNumberOfDashboardStations() const
    {
        return m_DashboardStationList.count();
    }

    /****************************************************************************/
    /*!
     *  \brief Get's the version of The Dashboard Station list File
     *
     *  \return Version Number
     */
    /****************************************************************************/
    int  GetVersion() const
    {
        return m_Version;
    }

    /****************************************************************************/
    /*!
     *  \brief Get's the Verification Mode of the Station list File
     *
     *  \return true or false
     */
    /****************************************************************************/
    bool GetDataVerificationMode() const {return m_DataVerificationMode;}

    /****************************************************************************/
    /*!
     *  \brief Sets the Verification Mode of the Station list File
     *
     *  \iparam Value = bool type , either true or false
     *
     *  \return
     */
    /****************************************************************************/
    void SetDataVerificationMode(const bool Value){m_DataVerificationMode = Value;}



    /****************************************************************************/
    /*!
     *  \brief Get's the Data Container Type for  dashboard station
     *
     *  \return Datacontainer Type
     */
    /****************************************************************************/
    DataContainerType_t GetDataContainerType()
    {
        return STATIONS;
    }

};

} // namespace DataManager


#endif // DATAMANAGER_DASHBOARDDATASTATIONLIST_H
