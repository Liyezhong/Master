/****************************************************************************/
/*! \file RMSCommandInterface.cpp
 *
 *  \brief RMSCommandInterface command implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 13.08.2012 05-10-2012
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

#include "RMS/Include/RMSCommandInterface.h"
#include "RMS/Include/RMSGlobalTypes.h"
#include "HimalayaMasterThread/Include/HimalayaMasterThreadController.h"


namespace HimalayaRMS {
CRMSCommandInterface::CRMSCommandInterface(DataManager::CDataManager *p_DataManager, Himalaya::HimalayaMasterThreadController *p_HimalayaMasterThreadController, CRMSHandler *p_RMSHandler) :
    mp_DataManager(p_DataManager),
    mp_HimalayaMasterThreadController(p_HimalayaMasterThreadController),
    mp_RMSHandler(p_RMSHandler)
{
    RegisterCommands();
    mp_ScanKitTimeout = new QTimer(this);
    mp_SendScanRequest = new QTimer(this);
    mp_ScanBottleTimeout = new QTimer(this);
}
/****************************************************************************/
/*!
 *  \brief RegisterCommands command from GUI(RMS->GUI) 
 *  \return void
 */
/****************************************************************************/
void CRMSCommandInterface::RegisterCommands()
{
    mp_HimalayaMasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdScanLeicaKit,CRMSCommandInterface>
            (&CRMSCommandInterface::ScanLeicaKit,this);

    mp_HimalayaMasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdScanLeicaBottle,HimalayaRMS::CRMSCommandInterface>
            (&CRMSCommandInterface::ScanLeicaBottle,this);

    mp_HimalayaMasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdConfirmLeicaReagent,HimalayaRMS::CRMSCommandInterface>
            (&CRMSCommandInterface::ConfirmLeicaReagent,this);


    mp_HimalayaMasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdUpdateLeicaReagent,HimalayaRMS::CRMSCommandInterface>
            (&CRMSCommandInterface::UpdateLeicaReagent,this); //! Leica Reagent


    mp_HimalayaMasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdResetStationRms,HimalayaRMS::CRMSCommandInterface>
            (&CRMSCommandInterface::ResetStationRms,this);//! Non Leica Reagent


    mp_HimalayaMasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdFillLeicaReagent,HimalayaRMS::CRMSCommandInterface>
            (&CRMSCommandInterface::FillLeicaReagent,this);

}
/****************************************************************************/
/*!
 *  \brief ScanLeicaBottle command from GUI(RMS->GUI) to scan Leica Kits
 *  \iparam Ref = Command referernce
 *  \iparam Cmd = command
 *  \iparam AckCommandChannel = Command channel to send acknowledge
 *  \return void
 */
/****************************************************************************/
void CRMSCommandInterface::ScanLeicaKit(Global::tRefType Ref, const MsgClasses::CmdScanLeicaKit &Cmd, Threads::CommandChannel &AckCommandChannel)
{
    bool IsKitScanViaBlg = Cmd.GetKitScanFlag();
    Q_UNUSED(IsKitScanViaBlg)
    InitiateandStartKitTimeout();
    mp_HimalayaMasterThreadController->SendAcknowledgeOK(Ref, AckCommandChannel);
}
/****************************************************************************/
/*!
 *  \brief ScanLeicaBottle command from GUI(RMS->GUI) to scan Leica bottles
 *  \iparam Ref = Command referernce
 *  \iparam Cmd = command
 *  \iparam AckCommandChannel = Command channel to send acknowledge
 *  \return void
 */
/****************************************************************************/
void CRMSCommandInterface::ScanLeicaBottle(Global::tRefType Ref, const MsgClasses::CmdScanLeicaBottle &Cmd, Threads::CommandChannel &AckCommandChannel)
{
    QString AutoHideStatus = Cmd.GetAutoHideStatus();
    Q_UNUSED(AutoHideStatus)
    InitiateandStartBottleTimeout();
    mp_HimalayaMasterThreadController->SendAcknowledgeOK(Ref, AckCommandChannel);
}
/****************************************************************************/
/*!
 *  \brief ConfirmLeicaReagent command from GUI(RMS->GUI) to confirm Leica reagent
 *  \iparam Ref = Command referernce
 *  \iparam Cmd = command
 *  \iparam AckCommandChannel = Command channel to send acknowledge
 *  \return void
 */
/****************************************************************************/
void CRMSCommandInterface::ConfirmLeicaReagent(Global::tRefType Ref, const MsgClasses::CmdConfirmLeicaReagent &Cmd, Threads::CommandChannel &AckCommandChannel)
{
    Q_UNUSED(Cmd)
    if(mp_RMSHandler->ProcessLeicaReagentBottle()) {
        //Send ACK to GUI
        if (mp_RMSHandler->IsLeicaReagentBottlePresent()) {
            MsgClasses::CmdUpdateRmsStatusToGui* p_Command = new MsgClasses::CmdUpdateRmsStatusToGui(1000,MsgClasses::CONFIRM_LEICA_REAGENT_STATUS_OK,mp_RMSHandler->GetLeicaReagentBottleCount(),"");
            mp_HimalayaMasterThreadController->SendCommand(Global::CommandShPtr_t(p_Command),
                                                           mp_HimalayaMasterThreadController->GetCommandChannel(Himalaya::GUI));
            mp_HimalayaMasterThreadController->SendAcknowledgeOK(Ref, AckCommandChannel);
        }
        else {
            mp_RMSHandler->ReadyToScanNextKit(); //Allows more than one kit to scan if available
            //Send Ack to GUI
            MsgClasses::CmdUpdateRmsStatusToGui* p_Command = new MsgClasses::CmdUpdateRmsStatusToGui(1000,MsgClasses::BOTTLE_SCAN_COMPLETE,0,"");
            mp_HimalayaMasterThreadController->SendCommand(Global::CommandShPtr_t(p_Command),
                                                           mp_HimalayaMasterThreadController->GetCommandChannel(Himalaya::GUI));
            mp_HimalayaMasterThreadController->SendAcknowledgeOK(Ref, AckCommandChannel);
        }
    }
    else {
        //Send Ack to GUI
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_CONFIRM_LEICA_REAGENT);
        MsgClasses::CmdUpdateRmsStatusToGui* p_Command = new MsgClasses::CmdUpdateRmsStatusToGui(1000,MsgClasses::CONFIRM_LEICA_REAGENT_FAILED,0,"");
        mp_HimalayaMasterThreadController->SendCommand(Global::CommandShPtr_t(p_Command),
                                                       mp_HimalayaMasterThreadController->GetCommandChannel(Himalaya::GUI));
        mp_HimalayaMasterThreadController->SendAcknowledgeNOK(Ref, AckCommandChannel);
    }
}

/****************************************************************************/
/*!
 *  \brief UpdateLeicaReagent command from GUI(RMS->GUI) to update reagent
 *  \iparam Ref = Command referernce
 *  \iparam Cmd = command
 *  \iparam AckCommandChannel = Command channel to send acknowledge
 *  \return void
 */
/****************************************************************************/
void CRMSCommandInterface::UpdateLeicaReagent(Global::tRefType Ref, const MsgClasses::CmdUpdateLeicaReagent &Cmd, Threads::CommandChannel &AckCommandChannel)
{
    Q_UNUSED(Ref)
    Q_UNUSED(Cmd)
         if (mp_RMSHandler->WriteDataToStationsXml()) {

        MsgClasses::CmdUpdateRmsStatusToGui* p_Command = new MsgClasses::CmdUpdateRmsStatusToGui(1000,MsgClasses::UPDATE_LEICA_REAGENT_STATUS_OK,0,"");
        mp_HimalayaMasterThreadController->SendCommand(Global::CommandShPtr_t(p_Command),
                                                       mp_HimalayaMasterThreadController->GetCommandChannel(Himalaya::GUI));
        // Send updated container data to GUI
        mp_RMSHandler->SendLatestStationListToGui();
        mp_HimalayaMasterThreadController->SendAcknowledgeOK(Ref, AckCommandChannel);
        }
         else {
             Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_UPDATE_LEICA_REAGENT);
             MsgClasses::CmdUpdateRmsStatusToGui* p_Command = new MsgClasses::CmdUpdateRmsStatusToGui(1000,MsgClasses::UPDATE_LEICA_REAGENT_FAILED,0,"");
        mp_HimalayaMasterThreadController->SendCommand(Global::CommandShPtr_t(p_Command),
                                                       mp_HimalayaMasterThreadController->GetCommandChannel(Himalaya::GUI));
        mp_HimalayaMasterThreadController->SendAcknowledgeNOK(Ref, AckCommandChannel);
    }

}
/****************************************************************************/
/*!
 *  \brief FillLeicaReagent command from GUI(RMS->GUI) about filled reagent info
 *  \iparam Ref = Command referernce
 *  \iparam Cmd = command
 *  \iparam AckCommandChannel = Command channel to send acknowledge
 *  \return
 */
/****************************************************************************/
void CRMSCommandInterface::FillLeicaReagent(Global::tRefType Ref, const MsgClasses::CmdFillLeicaReagent &Cmd, Threads::CommandChannel &AckCommandChannel)
{
    QString FillStationId = Cmd.GetStationId();
    //Update reagent sequence
    mp_HimalayaMasterThreadController->SendAcknowledgeOK(Ref, AckCommandChannel);
}
/****************************************************************************/
/*!
 *  \brief ResetStationRms command from GUI(RMS->GUI) to reset station RMS
 *  \iparam Ref = Command referernce
 *  \iparam Cmd = command
 *  \iparam AckCommandChannel = Command channel to send acknowledge
 *  \return void
 */
/****************************************************************************/
void CRMSCommandInterface::ResetStationRms(Global::tRefType Ref, const MsgClasses::CmdResetStationRms &Cmd, Threads::CommandChannel &AckCommandChannel)
{
    QString ResetStationId = Cmd.GetStationId();
    if (mp_RMSHandler->OnResetUserRMSStatus(ResetStationId)) {
        if (mp_RMSHandler->WriteDataToStationsXml()) {
            MsgClasses::CmdUpdateRmsStatusToGui* p_Command = new MsgClasses::CmdUpdateRmsStatusToGui(1000,MsgClasses::RESET_USER_RMS_OK,0,"");
            mp_HimalayaMasterThreadController->SendCommand(Global::CommandShPtr_t(p_Command),
                                                           mp_HimalayaMasterThreadController->GetCommandChannel(Himalaya::GUI));
            // Send updated container data to GUI
            mp_RMSHandler->SendLatestStationListToGui();
            mp_HimalayaMasterThreadController->SendAcknowledgeOK(Ref, AckCommandChannel);
        }
        else {
            Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_RESET_STATION_RMS);
            MsgClasses::CmdUpdateRmsStatusToGui* p_Command = new MsgClasses::CmdUpdateRmsStatusToGui(1000,MsgClasses::RESET_USER_RMS_FAILED,0,"");
            mp_HimalayaMasterThreadController->SendCommand(Global::CommandShPtr_t(p_Command),
                                                           mp_HimalayaMasterThreadController->GetCommandChannel(Himalaya::GUI));
            mp_HimalayaMasterThreadController->SendAcknowledgeNOK(Ref, AckCommandChannel);
        }
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_RMS_ERROR_RESET_STATION_RMS);
        MsgClasses::CmdUpdateRmsStatusToGui* p_Command = new MsgClasses::CmdUpdateRmsStatusToGui(1000,MsgClasses::RESET_USER_RMS_FAILED,0,"");
        mp_HimalayaMasterThreadController->SendCommand(Global::CommandShPtr_t(p_Command),
                                                       mp_HimalayaMasterThreadController->GetCommandChannel(Himalaya::GUI));
        mp_HimalayaMasterThreadController->SendAcknowledgeNOK(Ref, AckCommandChannel);
        qDebug() << "ResetStationRMSFailed";
    }
}

/****************************************************************************/
/*!
 *  \brief SendReadLeicaConsumablesRequest sends command for Kit/Bottle RFID data
 *  \return void
 */
/****************************************************************************/
void CRMSCommandInterface::SendReadLeicaConsumablesRequest()
{
}
/****************************************************************************/
/*!
 *  \brief ReadLeicaConsumables Read Kit/Bottle RFID information from Device layer 
 *  \iparam tRefType = Return type
 *  \iparam RfidData = Kit/Bottle data
 *  \return void
 */
/****************************************************************************/
void CRMSCommandInterface::ReadLeicaConsumables(Global::tRefType, const QByteArray *RfidData)
{
    //Check the Type ID to differentiate bottle or Kit
    if (RfidData->at(0) == LEICACONSUMABLES_KIT_TYPEID) {
        if (mp_ScanKitTimeout->isActive()) {
            mp_ScanKitTimeout->stop();
        }
        StopScanRequestCount(); //Data arrived: Stop sending request
        if(mp_RMSHandler->ReadLeicaCosumableKitData(RfidData)) {
            //Send Ack to GUI
            MsgClasses::CmdUpdateRmsStatusToGui* p_Command = new MsgClasses::CmdUpdateRmsStatusToGui(1000,MsgClasses::KIT_SCAN_STATUS_OK,mp_RMSHandler->GetLeicaReagentBottleCount(),"");
            mp_HimalayaMasterThreadController->SendCommand(Global::CommandShPtr_t(p_Command),
                                                           mp_HimalayaMasterThreadController->GetCommandChannel(Himalaya::GUI));
        }
    }
    else if (RfidData->at(0) == LEICACONSUMABLES_BOTTLE_TYPEID)
    {
        if (mp_ScanBottleTimeout->isActive()) {
            mp_ScanBottleTimeout->stop();
        }
        StopScanRequestCount(); //Data arrived: Stop sending request
        if(mp_RMSHandler->ReadLeicaCosumableBottleData(RfidData)) {
            //Send Ack to GUI
            MsgClasses::CmdUpdateRmsStatusToGui* p_Command = new MsgClasses::CmdUpdateRmsStatusToGui(1000,MsgClasses::BOTTLE_SCAN_STATUS_OK,mp_RMSHandler->GetLeicaReagentBottleCount(),mp_RMSHandler->GetLeicaBottleReagentID());
            mp_HimalayaMasterThreadController->SendCommand(Global::CommandShPtr_t(p_Command),
                                                           mp_HimalayaMasterThreadController->GetCommandChannel(Himalaya::GUI));
        }
    }
    else {
         Global::EventObject::Instance().RaiseEvent(EVENT_RMS_INVALID_LEICA_CONSUMABLE);
    }
}
/****************************************************************************/
/*!
 *  \brief InitiateandStartKitTimeout for Scaning the leica Reagent Kit 
 *  \return true - success
 */
/****************************************************************************/
bool CRMSCommandInterface::InitiateandStartKitTimeout()
{
    mp_ScanKitTimeout->setInterval(MAX_KITSCAN_TIME); // timer for 3 minutes
    mp_ScanKitTimeout->setSingleShot(true);   // Single shot

    CONNECTSIGNALSLOT(mp_ScanKitTimeout, timeout(), this, OnScanKitTimeoutExpiry());
    mp_ScanKitTimeout->start();

    //Send request for Kit data as soon as command received for bottle scan
    SendReadLeicaConsumablesRequest();
    InitiateandStartSendRequest();
    return true;
}

/****************************************************************************/
/*!
 *  \brief InitiateandStartBottleTimeout for Scaning the leica Reagent bottle 
 *  \return true - success
 */
/****************************************************************************/
bool CRMSCommandInterface::InitiateandStartBottleTimeout()
{
    mp_ScanBottleTimeout->setInterval(MAX_BOTTLESCAN_TIME); // timer for 1 minutes ,
    mp_ScanBottleTimeout->setSingleShot(true); // Single shot

    CONNECTSIGNALSLOT(mp_ScanBottleTimeout, timeout(), this, OnScanBottleTimeoutExpiry());
    mp_ScanBottleTimeout->start();
    mp_SendScanRequest->start(); //Start consumables repeatative request
    return true;
}

/****************************************************************************/
/*!
 *  \brief InitiateandStartSendRequest to send request to scan the kit or bottle 
 *  \return true - success
 */
/****************************************************************************/
bool CRMSCommandInterface::InitiateandStartSendRequest()
{
    mp_SendScanRequest->setInterval(MAX_SENDREQUEST_TIME); // timer for 1 second, its repetative
    CONNECTSIGNALSLOT(mp_SendScanRequest, timeout(), this, OnSendScanRequest());
    mp_SendScanRequest->start();
    return true;
}
/****************************************************************************/
/*!
 *  \brief OnScanKitTimeoutExpiry handles the error scenario when
 *  \brief Kit is not scanned in time 
 *  \return void
 */
/****************************************************************************/
void CRMSCommandInterface::OnScanKitTimeoutExpiry()
{
    mp_ScanKitTimeout->stop();
    StopScanRequestCount();
    //Send error info to main saying Kit info did not arrive within 3min
    Global::EventObject::Instance().RaiseEvent(EVENT_RMS_KIT_SCAN_TIMEOUT);
    MsgClasses::CmdUpdateRmsStatusToGui* p_Command = new MsgClasses::CmdUpdateRmsStatusToGui(1000,MsgClasses::KIT_SCAN_TIMEOUT,0,"");
    mp_HimalayaMasterThreadController->SendCommand(Global::CommandShPtr_t(p_Command),
                                                   mp_HimalayaMasterThreadController->GetCommandChannel(Himalaya::GUI));

}
/****************************************************************************/
/*!
 *  \brief OnScanBottleTimeoutExpiry handles the error scenario when
 *  \brief Bottle is not scanned in time 
 *  \return void
 */
/****************************************************************************/
void CRMSCommandInterface::OnScanBottleTimeoutExpiry()
{
    mp_ScanBottleTimeout->stop();
    StopScanRequestCount();
    //Send error info to main saying Bottle info did not arrive within 1min
    Global::EventObject::Instance().RaiseEvent(EVENT_RMS_BOTTLE_SCAN_TIMEOUT);
    MsgClasses::CmdUpdateRmsStatusToGui* p_Command = new MsgClasses::CmdUpdateRmsStatusToGui(1000,MsgClasses::BOTTLE_SCAN_TIMEOUT,0,"");
    mp_HimalayaMasterThreadController->SendCommand(Global::CommandShPtr_t(p_Command),
                                                   mp_HimalayaMasterThreadController->GetCommandChannel(Himalaya::GUI));

}
/****************************************************************************/
/*!
 *  \brief OnSendScanRequest sends the request command to scan the rfid 
 *  \return void
 */
/****************************************************************************/
void CRMSCommandInterface::OnSendScanRequest()
{
    mp_SendScanRequest->stop();
    //Send request for RFID data
    SendReadLeicaConsumablesRequest();
    mp_SendScanRequest->start();
}
/****************************************************************************/
/*!
 *  \brief StopScanRequestCount checks the rfid scan request timer running and stops 
 *  \return void
 */
/****************************************************************************/
void CRMSCommandInterface::StopScanRequestCount()
{
    if (mp_SendScanRequest->isActive()) {
        mp_SendScanRequest->stop();
    }
}
/****************************************************************************/
/*!
 *  \brief AbortLeicaScanProcess aborts Leica consumables scan process
 *  \return void
 */
/****************************************************************************/
void CRMSCommandInterface::AbortLeicaScanProcess()
{
    StopScanRequestCount(); //Abort sending request
    if (mp_ScanKitTimeout->isActive()) {
        mp_ScanKitTimeout->stop();
    }
    if (mp_ScanBottleTimeout->isActive()) {
        mp_ScanBottleTimeout->stop();
    }

}
/****************************************************************************/
}// End of namespace HimalayaRMS



