/****************************************************************************/
/*! \file RMSHandler.cpp
 *
 *  \brief RMSHandler class implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2012-08-09 05-10-2012
 *   $Author:  $ Vikram MK, Ningu
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
#include <QString>
#include <QDate>
#include "HimalayaMasterThread/Include/HimalayaMasterThreadController.h"
#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include "RMS/Include/RMSHandler.h"
#include "../../HimalayaDataManager/Include/DataManager.h"
#include "HimalayaDataContainer/Containers/Reagents/Include/DataReagentList.h"
#include "HimalayaDataContainer/Containers/Stations/Include/DataStationList.h"
#include "HimalayaDataContainer/Containers/Stations/Include/DataStationListVerifier.h"
#include "HimalayaDataContainer/Containers/ProgramSequence/Include/ProgramSequenceVerifier.h"
#include <Global/Include/EventObject.h>
#include <Global/Include/SystemPaths.h>
#include "RMSCommands/Include/CmdUpdateRmsStatusToGui.h"



namespace HimalayaRMS {


/****************************************************************************/
/*!
 *  \brief Default Constructor for RMS Handler class.
*/
/****************************************************************************/
CRMSHandler::CRMSHandler() : mp_LeicaReagentKit(NULL), mp_LeicaReagentBottle(NULL), m_PreRfidData(NODATA_READ),
    mp_DataManager(NULL), mp_HimalayaMasterThreadController(NULL)
{

}
/****************************************************************************/
/*!
 *  \brief Parameterized Constructor for RMS Handler class.
*/
/****************************************************************************/
CRMSHandler::CRMSHandler(DataManager::CDataManager *p_DataManager,Himalaya::HimalayaMasterThreadController *p_HimalayaMasterThreadController) :
    mp_LeicaReagentKit(NULL), mp_LeicaReagentBottle(NULL), m_PreRfidData(NODATA_READ),
    mp_DataManager(NULL), mp_HimalayaMasterThreadController(NULL)
{
    CHECKPTR(p_DataManager);
    mp_DataManager = p_DataManager;
    mp_HimalayaMasterThreadController = p_HimalayaMasterThreadController;

    mp_LeicaReagentKit =  new CLeicaReagentKit();
    mp_LeicaReagentBottle = new CLeicaReagentBottle();

    mp_CheckRMSStatus = new QTimer(this);
    CONNECTSIGNALSIGNAL(this, SendStationList(), p_DataManager, \
                        StationContainerUpdate());

    InitiateandStartRMSStatus();    
}
/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/

CRMSHandler::~CRMSHandler()
{
    try {
        DeleteLeicaReagentKit();
        DeleteLeicaReagentBottle();
    }
    catch(...) {
        //to please PClint
    }
}
/****************************************************************************/
/*!
 *  \brief Delete Leica Reagent Kit
 */
/****************************************************************************/
void CRMSHandler::DeleteLeicaReagentKit()
{
    if(NULL != mp_LeicaReagentKit) {
        delete mp_LeicaReagentKit;
        mp_LeicaReagentKit = NULL;
    }
}
/****************************************************************************/
/*!
 *  \brief Delete Leica Reagent Bottle
 */
/****************************************************************************/

void CRMSHandler::DeleteLeicaReagentBottle()
{
    if(NULL != mp_LeicaReagentBottle) {
        delete mp_LeicaReagentBottle;
        mp_LeicaReagentBottle = NULL;
    }
}
/****************************************************************************/
/*!
 *  \brief Initilise the time period to check the RMS Status of Stations 
 *  \return true - success or false
 */
/****************************************************************************/
bool CRMSHandler::InitiateandStartRMSStatus()
{
    mp_CheckRMSStatus->setInterval(POLL_RMSSTATUS); // timer for 1 hour, its repetative
    CONNECTSIGNALSLOT(mp_CheckRMSStatus, timeout(), this, OnPollRMSStatusExpiry());
    mp_CheckRMSStatus->start();
    qDebug() << "the status timer has started - CRMSHandler";
    return true;
}
/****************************************************************************/
/*!
 *  \brief Calls OnPollRMSStatusExpiry() for expiry status once after GUI bootup
 *  \return void
 */
/****************************************************************************/
void CRMSHandler::RmsPollReagentExpiryStatus()
{
    OnPollRMSStatusExpiry();
}
/****************************************************************************/
/*!
 *  \brief Polls reagents for expiry status and sends expiry information to main
 *  \return void
 */
/****************************************************************************/
void CRMSHandler::OnPollRMSStatusExpiry()
{/*
    mp_CheckRMSStatus->stop();
    qDebug() << "the status timer has ended - CRMSHandler";

    bool StationUpdate = false;
    bool TempDataVerificationMode;
    int ReagentStatus;
    int ICount;
    const DataManager::CDataContainer *p_DataContainer = mp_DataManager->GetDataContainer();

    if (p_DataContainer) {
        DataManager::CDashboardDataStationList *p_DataStationList = p_DataContainer->StationList;
        TempDataVerificationMode = p_DataStationList->GetDataVerificationMode();
        p_DataStationList->SetDataVerificationMode(false);
        for (ICount = 0 ; ICount < p_DataStationList->GetNumberOfStations(); ICount++) {
            DataManager::CDashboardStation *p_Station = const_cast<DataManager::CDashboardStation *>(p_DataStationList->GetStation(ICount));

            // check for the station which doesn't contain -1 and is not the Special reagent contained station
            if ((p_Station->GetReagentID() != "-1") && (p_Station->GetReagentID().at(0) != 'S')) {
                ReagentStatus = CalculateReagentStatus(p_Station);
                if (ReagentStatus >= 100) {
                    // Update the data container reagent satus
                    p_Station->SetReagentStatusValue(ReagentStatus);
                    //emit SendReagentExpiryStatus(p_Station->GetProgramID(),true);
                    if (UpdateLocalStation(p_Station)) {
                        StationUpdate = true;
                    }
                    else {
                        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_UPDATE_STATION);
                    }
                }
                else {
                    if ((ReagentStatus - p_Station->GetReagentStatusValue()) >= 5) {
                        // Update the data container reagent satus only if the status has changed of 5%
                        p_Station->SetReagentStatusValue(ReagentStatus);
                        //emit SendReagentExpiryStatus(p_Station->GetProgramID(),false);
                        if (UpdateLocalStation(p_Station)) {
                            StationUpdate = true;
                        }
                        else {
                            Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_UPDATE_STATION);
                        }
                    } // 5% change check
                }
            } // non special and no reagent checks
        } // end of station list
        p_DataStationList->SetDataVerificationMode(true);
        if (!p_DataStationList->VerifyData()) {
            Global::EventObject::Instance().RaiseEvent(EVENT_RMS_STATIONLIST_VERIFICATION_FAILED);
        }
        if (!TempDataVerificationMode) {
            p_DataStationList->SetDataVerificationMode(false);
        }
        if (StationUpdate) {
            // There is change in Station list,send station list to update to Data container
            emit SendStationList();//*p_DataStationList);
        }
        mp_CheckRMSStatus->start();
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_DATA_CONTAINER_NULL_POINTER);
    }
    */
}
/****************************************************************************/
/*!
 *  \brief Calculate the Reagent Status of the Reagent 
 *  \iparam p_Station = Instance of the CDashboardStation
 *  \return Reagent Status in percentage
 */
/****************************************************************************/
int CRMSHandler::CalculateReagentStatus(DataManager::CDashboardStation *p_Station)
{
    /*int ReagentExpirySlide;
    int ReagentExpiryDate;
    double CurrentTime, ExpiryTime,MaxExpireTime,ReferenceTime,ReagentUsedTime;
    QString ReagentID;
    QDate BestBeforeDate;

    if (p_Station == NULL) {
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_DATA_CONTAINER_NULL_POINTER);
        return 0;
    }
    ReagentID = p_Station->GetReagentID();
    const DataManager::CDataContainer *p_DataContainer = mp_DataManager->GetDataContainer();
    if (p_DataContainer) {
        DataManager::CDataReagentList *p_DataReagentList = p_DataContainer->ReagentList;
        const DataManager::CReagent *p_Reagent = p_DataReagentList->GetReagent(ReagentID) ;

        if (p_Reagent == NULL) {
            Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_DATA_CONTAINER_NULL_POINTER);
            return 0;
        }
        CurrentTime = Global::AdjustedTime::Instance().GetCurrentDateTime().toTime_t();
        if (ReagentID.at(0) == 'L') {
            // if the station contains leica reagent , check best before date
            BestBeforeDate = p_Station->GetBestBeforeDate();
            QDateTime BestBeforeDateTemp(BestBeforeDate);
            if (CurrentTime > BestBeforeDateTemp.toTime_t()) {
                qDebug() << "The best before date has expired";
                Global::EventObject::Instance().RaiseEvent(EVENT_RMS_LEICA_REAGENT_BEST_BEFORE_DATE_EXPIRED);
                return 100;
            }
        }

        ReagentExpirySlide = qRound(((p_Station->GetActualSlidesForecast() * 100)/p_Reagent->GetMaxSlides()));

        ExpiryTime = p_Station->GetExpiryDate().toTime_t();
        MaxExpireTime = p_Reagent->GetMaxTimeInSeconds();
        ReferenceTime = ExpiryTime - MaxExpireTime;
        ReagentUsedTime = CurrentTime - ReferenceTime;
        ReagentExpiryDate = qRound ((ReagentUsedTime / MaxExpireTime) * 100);

        //To avoid QDate calculation before 01-Jan-1970 for unused reagents
        if (ReagentExpiryDate > 100) {
            ReagentExpiryDate = 100;
        }
        return qMax(ReagentExpirySlide, ReagentExpiryDate);
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_DATA_CONTAINER_NULL_POINTER);
    }*/
    return 0;
}
/****************************************************************************/
/*!
 *  \brief Reads the Leica Kit Data , and stores it in the Classes 
 *  \iparam RfidData = Kit data from device
 *  \return true - success or false
 */
/****************************************************************************/
bool CRMSHandler::ReadLeicaCosumableKitData(const QByteArray *RfidData)
{
//    unsigned char RfidDataByte;
    if (m_PreRfidData == NODATA_READ) {
        if(mp_LeicaReagentKit != NULL) {
            delete mp_LeicaReagentKit;
            mp_LeicaReagentKit = NULL;
        }
        mp_LeicaReagentKit = new CLeicaReagentKit();
        mp_LeicaReagentKit->DeserializeKitContent(RfidData);

        //Is Kit's best before date expired ?
        //Is Kit belongs to proper program Id ?
        //Is kit data ok ?
        if (!ValidateLeicaReagentKit()) {
            qDebug() << "validation of kit failed";
            //Send error info to main saying Kit's best before date expired
            Global::EventObject::Instance().RaiseEvent(EVENT_RMS_KIT_DATA_ERROR);
            MsgClasses::CmdUpdateRmsStatusToGui* p_Command = new MsgClasses::CmdUpdateRmsStatusToGui(1000,MsgClasses::KIT_DATA_ERROR,0,"");
            mp_HimalayaMasterThreadController->SendCommand(Global::CommandShPtr_t(p_Command),
                                                           mp_HimalayaMasterThreadController->GetCommandChannel(Himalaya::GUI));
            return false;
        }
    } //First kit and then bottle check
    else {
        qDebug() << "this is not the preffered order -- error";
        //Send error info to main saying invalid scan order
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_INVALID_SCAN_ORDER);
        return false;
    }

    return true;
}
/****************************************************************************/
/*!
 *  \brief Reads the Leica Bottle Data , and stores it in the Classes 
 *  \iparam RfidData = Bottle data from device
 *  \return true - success or false
 */
/****************************************************************************/
bool CRMSHandler::ReadLeicaCosumableBottleData(const QByteArray *RfidData)
{
    unsigned char RfidDataByte;
    RfidDataByte = (RfidData->at(0) & 0xFF);
    if (RfidDataByte == LEICACONSUMABLES_BOTTLE_TYPEID) {
        if (m_PreRfidData == NODATA_READ) {
            qDebug() << "this is not the preffered order -- error";
            //Send error info to main saying invalid scan order
            Global::EventObject::Instance().RaiseEvent(EVENT_RMS_INVALID_SCAN_ORDER);
            return false;
        }
        if (m_PreRfidData == KITDATA_READ) {
            if (CheckLeicaBottle(RfidData)) {
                if(mp_LeicaReagentBottle != NULL) {
                    delete mp_LeicaReagentBottle;
                    mp_LeicaReagentBottle = NULL;
                }
                mp_LeicaReagentBottle = new CLeicaReagentBottle();
                mp_LeicaReagentBottle->DeserializeBottleContent(RfidData);

                //Is Bottle data ok ?
                if (!ValidateLeicaReagentBottle()) {
                    qDebug() << "validation of bottle failed";
                    //Send error info to main saying Bottle lot number does not exists in the Kit
                    Global::EventObject::Instance().RaiseEvent(EVENT_RMS_BOTTLE_DATA_ERROR);
                    MsgClasses::CmdUpdateRmsStatusToGui* p_Command = new MsgClasses::CmdUpdateRmsStatusToGui(1000,MsgClasses::BOTTLE_DATA_ERROR,0,"");
                    mp_HimalayaMasterThreadController->SendCommand(Global::CommandShPtr_t(p_Command),
                                                                   mp_HimalayaMasterThreadController->GetCommandChannel(Himalaya::GUI));
                    return false;
                }
            }
            else {
                qDebug() << "This is not leica bottle of the kit -- error";
                //Send error info to main saying invalid scan order
                Global::EventObject::Instance().RaiseEvent(EVENT_RMS_INVALID_SCAN_ORDER);
                return false;
            }
        } //Kit read is ok check
        else {
            qDebug() << "this is not the preffered order -- error";
            //Send error info to main saying invalid scan order
            Global::EventObject::Instance().RaiseEvent(EVENT_RMS_INVALID_SCAN_ORDER);
            return false;
        } // end of previous scan data info
    } // end of kit or bottle check
    return true;
}
/****************************************************************************/
/*!
 *  \brief This method checks the reagent id and lot number 
 *  \iparam RfidData = Bottle data
 *  \return true - success, on correct bottle
 */
/****************************************************************************/
bool CRMSHandler::CheckLeicaBottle(const QByteArray *RfidData)
{
    unsigned int Tempbyte = 0;
    Tempbyte = ( RfidData->at(1) & 0xFF );
    Tempbyte = Tempbyte << 8 | (RfidData->at(2) & 0xFF);
    QString ReagentID;
    ReagentID.setNum(Tempbyte);
    ReagentID.prepend("L");

    const DataManager::CDataContainer *p_DataContainer = mp_DataManager->GetDataContainer();
    if (p_DataContainer) {
        DataManager::CDataReagentList *p_DataReagentList = p_DataContainer->ReagentList;
        // check in the reagent list
        if (p_DataReagentList->ReagentExists(ReagentID)) {
            // check for lotnumber
            Tempbyte = 0 ;
            Tempbyte = RfidData->at(3);
            Tempbyte = Tempbyte << 8 | ( RfidData->at(4) & 0xFF );
            Tempbyte = Tempbyte << 8 | ( RfidData->at(5) & 0xFF );
            Tempbyte = Tempbyte << 8 | ( RfidData->at(6) & 0xFF );
            if (mp_LeicaReagentKit->LotNumberExists(Tempbyte)) {
                return true;
            }
        }
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_DATA_CONTAINER_NULL_POINTER);
    }
    return false;
}
/****************************************************************************/
/*!
 *  \brief Resets the RMS status of the passed Station. 
 *  \iparam StationID = Station ID 
 *  \return true - success or false
 */
/****************************************************************************/

bool CRMSHandler::ResetRMSStatus(QString StationID)
{
/*    const DataManager::CDataContainer *p_DataContainer = mp_DataManager->GetDataContainer();

    if (p_DataContainer) {
        DataManager::CDashboardDataStationList *p_DataStationList = p_DataContainer->StationList;
        DataManager::CDashboardStation *p_Station;

        if (p_DataStationList->StationExists(StationID)) {
            p_Station = p_DataStationList->GetStation(StationID);
            ResetStationDates(p_Station);
            p_Station->SetReagentStatusValue(0);
            p_Station->SetActualSlidesForecast(0);
            p_Station->SetNumberOfActualSlides(0);
            if (!UpdateLocalStation(p_Station)) {
                qDebug() << "The error in update local station data container";
                Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_UPDATE_STATION);
                return false;
            }
            return true;
        }
        else {
            qDebug() << "The Station doesn't exists in the Station List";
            Global::EventObject::Instance().RaiseEvent(EVENT_RMS_NO_STATION_PRESENT);
            return false;
        }
    }
    else {
        qDebug() << "The DataManager::CDataContainer pointer is NULL";
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_DATA_CONTAINER_NULL_POINTER);
        return false;
    }*/
    return true;
}

/****************************************************************************/
/*!
 *  \brief Resets the Station Dates like expiry date. 
 *  \iparam Station = Instance of the CDashboardStation 
 *  \return true - success or false
 */
/****************************************************************************/

bool CRMSHandler::ResetStationDates(DataManager::CDashboardStation *p_Station)
{/*
    QDateTime ExpiryDate;
    const DataManager::CReagent *p_Reagent;
    QString ReagentID;
    int TempMax_Time;
    // get the reagent
    ReagentID = p_Station->GetReagentID();

    if (ReagentID == "-1") {
        qDebug() << "the error station id passed dopesn't contain reagent";
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_NO_REAGENT_PRESENT);
        return false;
    }
    const DataManager::CDataContainer *p_DataContainer = mp_DataManager->GetDataContainer();

    if (p_DataContainer) {
        DataManager::CDataReagentList *p_DataReagentList = p_DataContainer->ReagentList;
        p_Reagent  = p_DataReagentList->GetReagent(ReagentID);

        if(p_Reagent) {
            TempMax_Time = p_Reagent->GetMaxTimeInSeconds();
            ExpiryDate = Global::AdjustedTime::Instance().GetCurrentDateTime();
            qDebug() << "\nCurrent date and time"<< ExpiryDate;
            ExpiryDate = ExpiryDate.addSecs(TempMax_Time);
            p_Station->SetExpiryDate(ExpiryDate);
            qDebug() << "\nExpiry Date from Station:"<< p_Station->GetExpiryDate().toTime_t();
        }
        else {
            Global::EventObject::Instance().RaiseEvent(EVENT_RMS_REAGENT_ERROR_NULL_POINTER);
        }
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_DATA_CONTAINER_NULL_POINTER);
    }*/
    return true;
    //the best before date remains same as when initialised
}
/****************************************************************************/
/*!
 *  \brief Updates the Station Parameters to Local Data Container. 
 *  \iparam Station = Instance of the CDashboardStation 
 *  \return true - success or false
 */
/****************************************************************************/
bool CRMSHandler::UpdateLocalStation(DataManager::CDashboardStation *p_Station)
{
    /*const DataManager::CDataContainer *p_DataContainer = mp_DataManager->GetDataContainer();

    if (p_DataContainer) {
        DataManager::CDashboardDataStationList *p_DataStationList = p_DataContainer->StationList;
        p_DataStationList->SetDataVerificationMode(true);

        if(!(p_DataStationList->UpdateStation(p_Station))) {
            qDebug() << "Error during updating Station:" << p_Station->GetStationID() << "of RMS component";
            Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_UPDATE_STATION);
            return false;
        }
        return true;
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_DATA_CONTAINER_NULL_POINTER);
        return false;
    }*/
    return true;
}
/****************************************************************************/
/*!
 *  \brief Writes latest station data to Stations.xml file.
 *  \return true - success or false
 */
/****************************************************************************/
bool CRMSHandler::WriteDataToStationsXml()
{
    const DataManager::CDataContainer *p_DataContainer = mp_DataManager->GetDataContainer();

    if (p_DataContainer) {
        DataManager::CDashboardDataStationList *p_DataStationList = p_DataContainer->StationList;
        p_DataStationList->SetDataVerificationMode(true);

        QString FilenameStationList = Global::SystemPaths::Instance().GetSettingsPath() + "/Stations.xml";
        if (!p_DataStationList->Write(FilenameStationList) ) {
            qDebug() << "Error during Writing to Station.xml";
            Global::EventObject::Instance().RaiseEvent(EVENT_RMS_WRITE_STATIONS_FAILED);
            return false;
        }
        return true;
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_DATA_CONTAINER_NULL_POINTER);
        return false;
    }
    return true;
}
/****************************************************************************/
/*!
 *  \brief Updates the Reagent Parameters to Local Data Container. 
 *  \iparam Reagent = Instance of the CReagent 
 *  \return true - success or false
 */
/****************************************************************************/

bool CRMSHandler::UpdateLocalReagent(DataManager::CReagent *p_Reagent)
{
    const DataManager::CDataContainer *p_DataContainer = mp_DataManager->GetDataContainer();
    if (p_DataContainer) {
        DataManager::CDataReagentList *p_DataReagentList = p_DataContainer->ReagentList;
        p_DataReagentList->SetDataVerificationMode(true);

        if(!(p_DataReagentList->UpdateReagent(p_Reagent))) {
            //qDebug() << "Error during updating Reagent:" << p_Reagent->GetID() << "of RMS component";
            Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_UPDATE_REAGENT);
            return false;
        }
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_DATA_CONTAINER_NULL_POINTER);
    }
    return true;
}

/****************************************************************************/
/*!
 *  \brief Resets the RMS Status of the User Reagents Stations.
 *  \return true = success or false
 */
/****************************************************************************/


/****************************************************************************/
/*!
 *  \brief Resets the RMS status of the passed Station ID. 
 *  \iparam StationID = Station ID
 *  \return true = success or false
 */
/****************************************************************************/
bool CRMSHandler::OnResetUserRMSStatus(QString &StationId)
{
    if(!ResetRMSStatus(StationId)) {
        qDebug() << "update Station data container failed in RMS";
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_UPDATE_STATION_CONTAINER);
        return false;
    }
    return true;
}
/****************************************************************************/
/*!
 *  \brief Updates the Leica reagent bottle to local DC and used station
 *  \return true = success or false
 */
/****************************************************************************/

bool CRMSHandler::ProcessLeicaReagentBottle()
{
    //Update Station id
    UpdateStationID(mp_LeicaReagentBottle->GetReagentID());

    //Update the DC and store station in the list
    if (!UpdateDataContainers()) {
        qDebug() << " Updation to data container failed";
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_UPDATE_CONTAINERS);
        return false;
    }
    else {
        // update station id to the list
        StationList.append(mp_LeicaReagentBottle->GetStationID());
        // decrement bottle count
        mp_LeicaReagentKit->DecrementBottleCount();
        return true;
    }
}

/****************************************************************************/
/*!
 *  \brief Check the Leica Reagent Bottle Count in kit
 *  \return true = success or false
 */
/****************************************************************************/

bool CRMSHandler::IsLeicaReagentBottlePresent()
{
    if (!mp_LeicaReagentKit->GetReagentBottleCount()) {        
        // no more Bottles are present in the Kit
        return false;
    }
    return true;
}
/****************************************************************************/
/*!
 *  \brief Allows to scan more than one Kit 
 *  \return void
 */
/****************************************************************************/

void CRMSHandler::ReadyToScanNextKit()
{
    m_PreRfidData = NODATA_READ;
}
/****************************************************************************/
/*!
 *  \brief Return the Leica Reagent Id
 *  \return Bottle Reagent id
 */
/****************************************************************************/

QString CRMSHandler::GetLeicaBottleReagentID()
{
    return mp_LeicaReagentBottle->GetReagentID();
}
/****************************************************************************/
/*!
 *  \brief Return the Leica Reagent Bottle Count in kit
 *  \return Bottle count
 */
/****************************************************************************/

int CRMSHandler::GetLeicaReagentBottleCount()
{
    if (mp_LeicaReagentKit->GetReagentBottleCount()) {
        return mp_LeicaReagentKit->GetReagentBottleCount();
    }
    else {
        return 0; // no more Bottles are present in the Kit
    }
}
/****************************************************************************/
/*!
 *  \brief Updates the Reagent Local Data Container with Kit ant bottle info.
 *  \iparam ReagentID = Reagent ID 
 *  \return true - success or false
 */
/****************************************************************************/
bool CRMSHandler::UpdateReagentDC(QString ReagentID)
{/*
    QString TempMem;
    if (ReagentID == "-1") {
        qDebug() << "The Reagent is not valid";
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_REAGENT_ID_NOT_VALID);
        return false;
    }
    const DataManager::CDataContainer *p_DataContainer = mp_DataManager->GetDataContainer();

    if (p_DataContainer) {
        DataManager::CDataReagentList *p_DataReagentList = p_DataContainer->ReagentList;
        DataManager::CReagent *p_Reagent = const_cast<DataManager::CReagent *>(p_DataReagentList->GetReagent(ReagentID));
        CHECKPTR(p_Reagent);

        // max slides
        p_Reagent->SetMaxSlides(mp_LeicaReagentKit->GetMaxSlides());

        // max time in the format "4d"
        TempMem.clear();
        TempMem.setNum(mp_LeicaReagentKit->GetOpenShellLifetime());
        TempMem.append("d");
        p_Reagent->SetMaxTime(TempMem);

        if (!UpdateLocalReagent(p_Reagent)) {
            Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_UPDATE_REAGENT);
            return false;
        }
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_DATA_CONTAINER_NULL_POINTER);
    }*/
    return true;
}
/****************************************************************************/
/*!
 *  \brief Updates the Station Local Data Container with Kit ant bottle info. 
 *  \iparam StationID = Station ID 
 *  \return true - success or false
 */
/****************************************************************************/
bool CRMSHandler::UpdateStationDC(QString StationID)
{/*
    QDate BestBeforeDateTemp;
    QString TempMem;

    if (StationID == "-1") {
        qDebug() << "The Station id is not valid";
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_STATION_ID_NOT_VALID);
        return false;
    }
    const DataManager::CDataContainer *p_DataContainer = mp_DataManager->GetDataContainer();

    if (p_DataContainer) {
        DataManager::CDashboardDataStationList *p_DataStationList = p_DataContainer->StationList;
        DataManager::CDashboardStation* p_Station = p_DataStationList->GetStation(StationID);
        CHECKPTR(p_Station);

        // update best before date and lotnumber
        TempMem = mp_LeicaReagentKit->GetBestBeforeDate();
        BestBeforeDateTemp = ConvertBestBeforeDate(TempMem);
        p_Station->SetBestBeforeDate(BestBeforeDateTemp);

        // update lotnumber
        p_Station->SetLotNumber(TempMem.setNum(mp_LeicaReagentBottle->GetLotNumber()));

        // update expiry date, actual slides , reagent status
        if(!ResetRMSStatusDC(StationID,p_Station)) {
            qDebug() << "update Station data container failed in RMS";
            Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_UPDATE_STATION_CONTAINER);
            return false;
        }
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_DATA_CONTAINER_NULL_POINTER);
    }*/
    return true;
}
/****************************************************************************/
/*!
 *  \brief Resets the given station with latest data.
 *  \iparam StationID = Station ID
 *  \iparam p_Station = Station pointer
 *  \return true - success or false
 */
/****************************************************************************/
bool CRMSHandler::ResetRMSStatusDC(QString StationID,DataManager::CDashboardStation *p_Station)
{
   /* const DataManager::CDataContainer *p_DataContainer = mp_DataManager->GetDataContainer();

    if (p_DataContainer) {
        DataManager::CDashboardDataStationList *p_DataStationList = p_DataContainer->StationList;

        if (p_DataStationList->StationExists(StationID)) {
            ResetStationDates(p_Station);
            p_Station->SetReagentStatusValue(0);
            p_Station->SetActualSlidesForecast(0);
            p_Station->SetNumberOfActualSlides(0);
            if (!UpdateLocalStation(p_Station)) {
                qDebug() << "The error in update local station data container";
                Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_UPDATE_STATION);
                return false;
            }
            return true;
        }
        else {
            qDebug() << "The Station doesn't exists in the Station List";
            Global::EventObject::Instance().RaiseEvent(EVENT_RMS_NO_STATION_PRESENT);
            return false;
        }
    }
    else {
        qDebug() << "The DataManager::CDataContainer pointer is NULL";
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_DATA_CONTAINER_NULL_POINTER);
        return false;
    }*/
    return true;
}
/****************************************************************************/
/*!
 *  \brief Updates the local datacontainers from kit and bottle information. 
 *  \return true - success or false
 */
/****************************************************************************/
bool CRMSHandler::UpdateDataContainers()
{
    QString TempMem;
    TempMem = mp_LeicaReagentBottle->GetReagentID();
    if(!UpdateReagentDC(TempMem)) {
        qDebug() << " the reagent dc failed to update - RMS";
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_UPDATE_REAGENT_CONTAINER);
        return false;
    }
    TempMem = mp_LeicaReagentBottle->GetStationID();
    if(!UpdateStationDC(TempMem)) {
        qDebug() << " the station dc failed to update - RMS";
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_UPDATE_STATION_CONTAINER);
        return false;
    }
    return true;
}

/****************************************************************************/
/*!
 *  \brief Validates the Leica Reagent kit after Deserialisation. 
 *  \return true - success or false
 */
/****************************************************************************/
bool CRMSHandler::ValidateLeicaReagentKit()
{
    //Type ID check is already verified
    //check for Program ID
    QString TempMem;
    QStringList ProgramIdList;
    TempMem = mp_LeicaReagentKit->GetProgramID();

    const DataManager::CDataContainer *p_DataContainer = mp_DataManager->GetDataContainer();

    if (p_DataContainer) {
        DataManager::CProgramSequenceList *p_DataProgramSequenceList = p_DataContainer->ProgramSequenceList;
        ProgramIdList = p_DataProgramSequenceList->GetProgramIDList();

        // check the presence of said program id
        if (!ProgramIdList.contains(TempMem)) {
            qDebug() << "the program id doesn't exists or It it is not startable and";
            qDebug() << "in  the list- kit is not valid";
            //Send error info to main saying Kit does not belong to proper program Id
            Global::EventObject::Instance().RaiseEvent(EVENT_RMS_INVALID_KIT_PROG_ID);
            MsgClasses::CmdUpdateRmsStatusToGui* p_Command = new MsgClasses::CmdUpdateRmsStatusToGui(1000,MsgClasses::INVALID_KIT_PROGRAM_ID,0,"");
            mp_HimalayaMasterThreadController->SendCommand(Global::CommandShPtr_t(p_Command),
                                                           mp_HimalayaMasterThreadController->GetCommandChannel(Himalaya::GUI));
            return false;
        }
        //Check for best before date
        QDate TempDate;
        TempMem = mp_LeicaReagentKit->GetBestBeforeDate();
        TempDate = ConvertBestBeforeDate(TempMem);
        QDateTime BestBeforeDateTemp(TempDate);

        if (Global::AdjustedTime::Instance().GetCurrentDateTime().toTime_t() > BestBeforeDateTemp.toTime_t()) {
            qDebug() << "The best before Date has Expired";
            //Send error info to main saying Kit's best before date expired
            Global::EventObject::Instance().RaiseEvent(EVENT_RMS_KIT_BEST_BEFORE_EXPIRED);
            MsgClasses::CmdUpdateRmsStatusToGui* p_Command = new MsgClasses::CmdUpdateRmsStatusToGui(1000,MsgClasses::KIT_BEST_BEFORE_DATE_EXPIRED,0,"");
            mp_HimalayaMasterThreadController->SendCommand(Global::CommandShPtr_t(p_Command),
                                                           mp_HimalayaMasterThreadController->GetCommandChannel(Himalaya::GUI));
            return false;
        }
        m_PreRfidData = KITDATA_READ;
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_DATA_CONTAINER_NULL_POINTER);
    }
    return true;
}

/****************************************************************************/
/*!
 *  \brief Validates the Leica Reagent bottle after Deserialisation. 
 *  \return true - success or false
 */
/****************************************************************************/
bool CRMSHandler::ValidateLeicaReagentBottle()
{
    //Type ID check is already verified
    //check for Reagent ID
    /*QString ReagentID;
    int ICount;
    bool ReagentPresent = false;
    ReagentID = mp_LeicaReagentBottle->GetReagentID();
    DataManager::CDataContainer const *p_DataContainer = mp_DataManager->GetDataContainer();

    if (p_DataContainer) {
        DataManager::CDashboardDataStationList *p_DataStationList = p_DataContainer->StationList;

        for (ICount = 0 ; ICount < p_DataStationList->GetNumberOfStations(); ICount++) {
            DataManager::CDashboardStation *p_Station = const_cast<DataManager::CDashboardStation *>(p_DataStationList->GetStation(ICount));

            //QString ReagentId = p_Station->GetReagentID();
            // Check for the existance of reagent
            if (p_Station->GetReagentID() == ReagentID) {
                ReagentPresent = true;
                break;
            }
        }
        if(!ReagentPresent) {
            qDebug() << "the reagent id doesn't exists in  the list- bottle is not valid";
            Global::EventObject::Instance().RaiseEvent(EVENT_RMS_REAGENT_ID_NOT_PRESENT);
            return false;
        }
        // check for lotnumber
        unsigned int Lotnumber;
        Lotnumber = mp_LeicaReagentBottle->GetLotNumber();
        if (!mp_LeicaReagentKit->LotNumberExists(Lotnumber)) {
            qDebug() << "The Lotnumber doesn't exists in the Kit";
            Global::EventObject::Instance().RaiseEvent(EVENT_RMS_LOT_NUMBER_NOT_PRESENT_IN_KIT);
            return false;
        }
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_DATA_CONTAINER_NULL_POINTER);
    }*/
    return true;
}

/****************************************************************************/
/*!
 *  \brief Converts Best Before Date String to Qdate Format.
 *  \iparam BestBeforeDate = Reagent best before date
 *  \return QDate
 */
/****************************************************************************/
QDate CRMSHandler::ConvertBestBeforeDate(QString BestBeforeDate)
{
    unsigned int year;
    unsigned int month;
    unsigned int day;

    // best before date is "yymmdd" need to convert to "20yy-mm-dd" or QDate format
    year = BestBeforeDate.left(2).toInt();
    year += CENTURY_21;
    month = BestBeforeDate.mid(2,2).toInt();
    day = BestBeforeDate.right(2).toInt();
    QDate BestBeforeDateTemp(year, month, day);
    return BestBeforeDateTemp;
}

/****************************************************************************/
/*!
 *  \brief Updates the StationID to the Leica Bottle
 *  \iparam LeicaReagentID = Leica reagent ID
 *  \return void
 */
/****************************************************************************/
void CRMSHandler::UpdateStationID(QString LeicaReagentID)
{
    /*QStringList Stations;
    QString TempStation;
    const DataManager::CDataContainer *p_DataContainer = mp_DataManager->GetDataContainer();

    if (p_DataContainer) {
        DataManager::CDashboardDataStationList *p_DataStationList = p_DataContainer->StationList;
        Stations = p_DataStationList->GetLeicaReagentStations(LeicaReagentID);

        // traverse in the stringlist and compare already filled station.
        // if it is a parallel station , we get more then one stations and need to be checked
        // which is already filled.
        for (int ic=0; ic < Stations.size(); ic++)
        {
            TempStation.clear();
            TempStation = Stations.at(ic);
            // check this station contains in already filled list
            if (!StationList.contains(TempStation)) {
                // update the station to leica reagent bottle attribute
                mp_LeicaReagentBottle->SetStationID(TempStation);
                break;
            }
        }
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_DATA_CONTAINER_NULL_POINTER);
    }*/
}
/****************************************************************************/
/*!
 *  \brief Sends updated stations list and Program sequence list to GUI 
 *  \return void
 */
/****************************************************************************/
void CRMSHandler::SendLatestContainerDataToGui()
{
    const DataManager::CDataContainer *p_DataContainer = mp_DataManager->GetDataContainer();

    if (p_DataContainer) {
        QByteArray ByteArray;
        ByteArray.clear();
        QDataStream XmlStream(&ByteArray, QIODevice::ReadWrite);
        XmlStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));

        ByteArray.clear();
        (void)XmlStream.device()->reset();
        DataManager::CProgramSequenceList *p_ProgramSequenceList = NULL;
        p_ProgramSequenceList =  p_DataContainer->ProgramSequenceList;
        if (p_ProgramSequenceList) {
            XmlStream << *p_ProgramSequenceList;
            mp_HimalayaMasterThreadController->SendCommand(Global::CommandShPtr_t(new NetCommands::CmdConfigurationFile(5000, NetCommands::PROGRAM_SEQUENCE , XmlStream)), mp_HimalayaMasterThreadController->GetCommandChannel(Himalaya::GUI));
        }
        ByteArray.clear();
        (void)XmlStream.device()->reset();
        DataManager::CDashboardDataStationList *p_StationList = NULL;
        p_StationList =  p_DataContainer->StationList;
        if (p_StationList) {
            XmlStream << *p_StationList;
            mp_HimalayaMasterThreadController->SendCommand(Global::CommandShPtr_t(new NetCommands::CmdConfigurationFile(5000, NetCommands::STATION , XmlStream)), mp_HimalayaMasterThreadController->GetCommandChannel(Himalaya::GUI));
        }
        }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_DATA_CONTAINER_NULL_POINTER);
    }
}
/****************************************************************************/
/*!
 *  \brief Sends updated stations list to GUI
 *  \return void
 */
/****************************************************************************/
void CRMSHandler::SendLatestStationListToGui()
{
    const DataManager::CDataContainer *p_DataContainer = mp_DataManager->GetDataContainer();

    if (p_DataContainer) {
        QByteArray ByteArray;
        ByteArray.clear();
        QDataStream XmlStream(&ByteArray, QIODevice::ReadWrite);
        XmlStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));

        ByteArray.clear();
        (void)XmlStream.device()->reset();
        DataManager::CDashboardDataStationList *p_StationList = NULL;
        p_StationList =  p_DataContainer->StationList;
        if (p_StationList) {
            XmlStream << *p_StationList;
            mp_HimalayaMasterThreadController->SendCommand(Global::CommandShPtr_t(new NetCommands::CmdConfigurationFile(5000, NetCommands::STATION , XmlStream)), mp_HimalayaMasterThreadController->GetCommandChannel(Himalaya::GUI));
        }
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_DATA_CONTAINER_NULL_POINTER);
    }

}
/****************************************************************************/
}  // namespace HimalayaRMS
