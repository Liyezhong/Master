/****************************************************************************/
/*! \file DataStationListVerifier.cpp
 *
 *  \brief DataStationListVerifier class implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2012-05-07
 *   $Author:  $ Vikram MK
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

#include "HimalayaDataContainer/Containers/ReagentStations/Include/DataStationListVerifier.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Include/DashboardDataStationList.h"


namespace DataManager {

/****************************************************************************/
/*!
 *  \brief Constructor
 */
/****************************************************************************/
CDataStationListVerifier::CDataStationListVerifier() : mp_DSL(NULL)
{
}

/****************************************************************************/
/*!
 *  \brief Verifies the attribute values for the Station Class
 *
 *  \iparam p_DataStationList = Pointer to DataStationList class
 *
 *  \return True or False
 */
/****************************************************************************/
bool CDataStationListVerifier::VerifyData(CDataContainerBase* p_DataStationList)
{
    Q_UNUSED(p_DataStationList)
    /*
    mp_DSL = static_cast<CDashboardDataStationList*>(p_DataStationList);

    for(int i=0;i<mp_DSL->GetNumberOfStations();i++) {
        CDashboardStation Station;
        if (mp_DSL->GetStation(i, Station)) {
            QString StationID = Station.GetStationID();
            if (StationID.size() > MAX_SIZEOF_STATIONID) {
                qDebug() << "### Station ID more than 3 characters: " << StationID;
                return false;
            }
            if (mp_DSL->GetStationType(StationID) == INVALID_TYPE) {
                qDebug() << "### Station ID is Invalid: " << StationID;
                return false;
            }
            if ((StationID != SLIDEID_STATION_ID) && (StationID != TRANSFER_STATION_ID)) {
                if (!StationID[1].isDigit() && !StationID[2].isDigit()) {
                    qDebug() << "### Station ID doesn't have numbers: " << StationID;
                    return false;
                }
            }
            // if the station contains special reagent or if their is no reagent the below checks not required
            if ((Station.GetReagentID().at(0) != SPECIAL_REAGENT_START_CHARACTER) &&
                    (Station.GetReagentID() != "-1")) {
                if (!Station.GetExpiryDate().isValid()) {
                    qDebug() << "### Station's Expiry Date not valid: " << Station.GetExpiryDate().toString();
                    return false;
                }
                // if its Leica Reagent check best before date validity
                if (Station.GetReagentID().at(0) == LEICA_REAGENT_START_CHARACTER) {
                    if (!Station.GetBestBeforeDate().isValid()) {
                        qDebug() << "### Station's Best Before Date not valid: " << Station.GetExpiryDate().toString();
                        return false;
                    }
                }
            } // end of special reagent check
        }
    } // end of for loop of each stations
    */
    return true;
}

/****************************************************************************/
/*!
 *  \brief  Gets the last errors which is done by verifier
 *
 *  \return QStringList - List of the errors occured
 */
/****************************************************************************/
ErrorMap_t& CDataStationListVerifier::GetErrors()
{
    // return the last error which is occured in the verifier
    return m_ErrorsHash;
}

/****************************************************************************/
/*!
 *  \brief  Resets the last error which is done by verifier
 */
/****************************************************************************/
void CDataStationListVerifier::ResetErrors()
{
    m_ErrorsHash.clear();
}

/****************************************************************************/
/*!
 *  \brief  Resets the last error which is done by verifier
 *  \return bool
 */
/****************************************************************************/
bool CDataStationListVerifier::IsLocalVerifier()
{
    return true;
}

}  // namespace DataManager
