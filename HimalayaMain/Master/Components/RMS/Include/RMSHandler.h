/****************************************************************************/
/*! \file RMS/Include/RMSHandler.h
 *
 *  \brief Definition file for class CRMSHandler.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-08-08 05-10-2012
 *  $Author:    $ Vikram MK, Ningu
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
#ifndef RMS_HANDLER_H
#define RMS_HANDLER_H

#include <QTimer>
#include "../../HimalayaDataManager/Include/DataManager.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Include/DashboardDataStationList.h"
#include "HimalayaDataContainer/Containers/Reagents/Include/DataReagentList.h"
#include "RMS/Include/LeicaReagentKit.h"
#include "RMS/Include/RMSGlobalTypes.h"
#include <NetCommands/Include/CmdConfigurationFile.h>


namespace HimalayaRMS {

const int POLL_RMSSTATUS = (1000*60*60);   //!< Timer of 1 hour which polls the stations Status

/****************************************************************************/
/**
 * \brief This is a Reagent Management System Handler. It handles RMS.
 * \warning This class is not threadsafe!
 */
/****************************************************************************/
class CRMSHandler: public QObject
{
    Q_OBJECT

private:

    QTimer *mp_CheckRMSStatus;                          //!< Timer of 1 hour
    CLeicaReagentKit  *mp_LeicaReagentKit;              //!< Leica Reagent Kit information
    CLeicaReagentBottle  *mp_LeicaReagentBottle;        //!< Leica Reagent Bottle information
    RfidDataType_t  m_PreRfidData;                      //!< Leica consumables scan identifier
    //ScanStatus_t    m_ScanStatus;                       //!< Leica consumables scan status
    QStringList StationList;                            //!< Stores already filled Stations in the kit
    DataManager::CDataManager     *mp_DataManager;      //!< DataManager instance of the Master
    Himalaya::HimalayaMasterThreadController *mp_HimalayaMasterThreadController; //!< Will be used to send container data to GUI

    bool ResetStationDates(DataManager::CDashboardStation *p_Station);
    QDate ConvertBestBeforeDate(QString BestBeforeDate);
    void UpdateStationID(QString LeicaReagentID);
    bool UpdateDataContainers();
    bool UpdateStationDC(QString StationID);
    bool UpdateReagentDC(QString ReagentID);    


public:
    CRMSHandler();
    CRMSHandler(DataManager::CDataManager *p_DataManager,Himalaya::HimalayaMasterThreadController *p_HimalayaMasterThreadController);
    ~CRMSHandler();
    void RmsPollReagentExpiryStatus();
    void DeleteLeicaReagentKit();
    void DeleteLeicaReagentBottle();

    bool InitiateandStartRMSStatus();
    bool ReadLeicaCosumablesData(const QByteArray *RfidData);
    bool ReadLeicaCosumableKitData(const QByteArray *RfidData);
    bool ReadLeicaCosumableBottleData(const QByteArray *RfidData);
    bool CheckLeicaBottle(const QByteArray *RfidData);

    bool ResetRMSStatus(QString StationID);
    bool OnConfirmBathLayout();
    bool WriteDataToStationsXml();
    bool OnResetUserRMSStatus(QString &StationId);  
    int CalculateReagentStatus(DataManager::CDashboardStation *p_Station);

    bool UpdateLocalStation(DataManager::CDashboardStation *p_Station);
    bool UpdateLocalReagent(DataManager::CReagent *p_Reagent);
    bool ResetRMSStatusDC(QString StationID, DataManager::CDashboardStation *p_Station);

    void SendLatestContainerDataToGui();
    void SendLatestStationListToGui();
    bool ProcessLeicaReagentBottle();
    bool IsLeicaReagentBottlePresent();
    void ReadyToScanNextKit();
    int  GetLeicaReagentBottleCount();
    QString GetLeicaBottleReagentID();

    bool ValidateLeicaReagentBottle();
    bool ValidateLeicaReagentKit();

signals:
    /****************************************************************************/
    /*!
     *  \brief This signal is emitted to update station list
     *  \return void
     */
    /****************************************************************************/
    void SendStationList();
    /****************************************************************************/
    /*!
     *  \brief This signal is emitted to send reagent expiry status
     *  \return void
     */
    /****************************************************************************/
    void SendReagentExpiryStatus(QString &ProgramID, bool ExpiryStatus);

private slots:   
    void OnPollRMSStatusExpiry();
};
/****************************************************************************/
}// namespace HimalayaRMS
#endif // RMS_HANDLER_H
