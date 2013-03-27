/****************************************************************************/
/*! \file SpecialVerifierGroupA.cpp
 *
 *  \brief Implementation file for class CSpecialVerifierGroupA.
 *
 *  $Version:   $ 0.2
 *  $Date:      $ 2011-08-01, 2012-05-03
 *  $Author:    $ F. Toth, Raju
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

#include <QDebug>
#include <QFile>

#include "HimalayaDataContainer/SpecialVerifiers/Include/SpecialVerifierGroupA.h"
#include "Global/Include/EventObject.h"
#include "HimalayaDataContainer/Helper/Include/HimalayaDataManagerEventCodes.h"


namespace DataManager {

/****************************************************************************/
/*!
 *  \brief Constructor
 */
/****************************************************************************/
CSpecialVerifierGroupA::CSpecialVerifierGroupA() : mp_DProgramList(NULL), mp_DReagentList(NULL), mp_DStationList(NULL)
{
}

/****************************************************************************/
/*!
 *  \brief  Constructor
 *
 *  \iparam p_DataProgramList = Pointer to a Program list
 *  \iparam p_DataReagentList = Pointer to a Reagent list
 *  \iparam p_DataStationList = Pointer to a Station list
 */
/****************************************************************************/
CSpecialVerifierGroupA::CSpecialVerifierGroupA(CDataProgramList* p_DataProgramList, CDataReagentList* p_DataReagentList,
                                               CDashboardDataStationList* p_DataStationList)
    : mp_DProgramList(p_DataProgramList),
      mp_DReagentList(p_DataReagentList),
      mp_DStationList(p_DataStationList)
{
}


/****************************************************************************/
/*!
 *  \brief  Verifies the data
 *
 *  \iparam p_ContainerBase = Pointer to a Container base
 *
 *  \return Successful (true) or not (false)
 */
/****************************************************************************/
bool CSpecialVerifierGroupA::VerifyData(CDataContainerBase* p_ContainerBase)
{
    bool Result = true;

    if (p_ContainerBase != NULL) {
        // prepare member pointers according to given container type
        // switch given container to temporary one for check
        // and switch back to real data after check again
        CDataContainerBase* p_TempDataItem = NULL;

        // check the containers consists of data or not if not please log it
        // which container is failed. This is for logger
        if (mp_DReagentList == NULL) {
            Global::EventObject::Instance().RaiseEvent(EVENT_DM_GV_REAGENT_IS_NOT_INITIALIZED);
            qDebug() << "CSpecialVerifierGroupA::VerifyData failed. Reagent List is NULL";
            return false;
        }
        if (mp_DProgramList == NULL) {
            Global::EventObject::Instance().RaiseEvent(EVENT_DM_GV_PROGRAM_IS_NOT_INITIALIZED);
            qDebug() << "CSpecialVerifierGroupA::VerifyData failed. Program List is NULL";
            return false;
        }
        if (mp_DStationList == NULL) {
            Global::EventObject::Instance().RaiseEvent(EVENT_DM_GV_STATION_IS_NOT_INITIALIZED);
            qDebug() << "CSpecialVerifierGroupA::VerifyData failed. Station List is NULL";
            return false;
        }

        // copy all the data data to temporary variables
        if (p_ContainerBase->GetDataContainerType() == PROGRAMS) {
            p_TempDataItem = mp_DProgramList;
            mp_DProgramList = static_cast<CDataProgramList*>(p_ContainerBase);
        }
        else if (p_ContainerBase->GetDataContainerType() == REAGENTS) {
            p_TempDataItem = mp_DReagentList;
            mp_DReagentList = static_cast<CDataReagentList*>(p_ContainerBase);
        }
        else if (p_ContainerBase->GetDataContainerType() == STATIONS) {
            p_TempDataItem = mp_DStationList;
            mp_DStationList = static_cast<CDashboardDataStationList*>(p_ContainerBase);
        }
        else {
            Global::EventObject::Instance().RaiseEvent(EVENT_DM_GV_INVALID_CONTAINER_TYPE);
            qDebug() << "CSpecialVerifierGroupA::VerifyData failed. Invalid conatiner type";
            return false;
        }

        if (Result) {
            if (!CheckData()) {
                Result = false;
            }
        }

        // revert all the temporary data to local variables
        if (p_ContainerBase->GetDataContainerType() == REAGENTS) {
            mp_DReagentList = static_cast<CDataReagentList*>(p_TempDataItem);
        }
        else if (p_ContainerBase->GetDataContainerType() == PROGRAMS) {
            mp_DProgramList = static_cast<CDataProgramList*>(p_TempDataItem);
        }
        else if (p_ContainerBase->GetDataContainerType() == STATIONS) {
            mp_DStationList = static_cast<CDashboardDataStationList*>(p_TempDataItem);
        }
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_GV_BASE_CONTAINER_IS_NOT_INITIALIZED);
        qDebug() << "CSpecialVerifierGroupA::VerifyData failed. Container base is not initialised";;
        Result = false;
    }

    return Result;

}


/****************************************************************************/
/*!
 *  \brief  Checks the data with other containers (Like Program list,
 *          Station list and Reagent list)
 *
 *  \return Successful (true) or not (false)
 */
/****************************************************************************/
bool CSpecialVerifierGroupA::CheckData()
{
    bool Result = true;
    // check required to avoid lint warning
    if (mp_DReagentList && mp_DProgramList) {
        // check each program has a valid reagent ID or not
        for (qint32 Counter = 0; Counter < mp_DProgramList->GetReagentIDList().count(); Counter++) {
            // check the Program Reagent ID exists in ReagentID List
            if (!(mp_DReagentList->ReagentExists(mp_DProgramList->GetReagentIDList().at(Counter)) ||
                  (mp_DProgramList->GetReagentIDList().at(Counter) == "-1"))) {
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_GV_REAGENTID_EXIST_IN_PROG_NOT_IN_REAGENT_LIST,
                                                           Global::FmtArgs() << mp_DProgramList->GetReagentIDList().at(Counter));
//                qDebug() << "CSpecialVerifierGroupA::Reagent ID does not exist in Reagent List but it exists in Program List: "
//                         << mp_DProgramList->GetReagentIDList().at(Counter);
                Result = false;
                break;
            }
        }
/*
        for (int StationCount = 0; StationCount < mp_DStationList->GetNumberOfStations(); StationCount++) {
            CStation* p_Station = const_cast<CStation*> (mp_DStationList->GetStation(StationCount));
            if (p_Station != NULL) {
                // check the Program Reagent ID exists in ReagentID List
                if (!(mp_DReagentList->ReagentExists(p_Station->GetReagentID()) || (p_Station->GetReagentID() == "-1"))) {
                    Global::EventObject::Instance().RaiseEvent(EVENT_DM_GV_REAGENTID_EXIST_IN_STATION_NOT_IN_REAGENT_LIST,
                                                               Global::FmtArgs() << p_Station->GetReagentID());
//                    qDebug() << "CSpecialVerifierGroupA::Reagent ID does not exist in Reagent List but it exists in Station List: "
//                             << p_Station->GetReagentID();
                    Result = false;
                    break;
                }
            }
        }
*/
    }    

    // if everything goes well then Special verifier verified all the data
    return Result;
}

}  // namespace DataManager

