/****************************************************************************/
/*! \file StationCommandInterface.cpp
 *
 *  \brief Station Command Interface  Implementation
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-07-30
 *  $Author:    $ N.Kamath
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
#include "HimalayaDataManager/CommandInterface/Include/StationCommandInterface.h"
#include "HimalayaMasterThread/Include/HimalayaMasterThreadController.h"


namespace DataManager {
/****************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam p_DataManager = Datamanager object
 *  \iparam p_MasterThreadController = Master thread Cntrl
 *  \iparam p_DataContainer = DataContainer pointer
 */
/****************************************************************************/
CStationCommandInterface::CStationCommandInterface(CDataManager *p_DataManager, Threads::MasterThreadController *p_MasterThreadController, CDataContainer *p_DataContainer):
    CCommandInterfaceBase(p_DataManager, p_MasterThreadController, p_DataContainer)
{
    RegisterCommands();
}

/****************************************************************************/
/**
 * \brief Register Commands related to Station Container
 */
/****************************************************************************/
void CStationCommandInterface::RegisterCommands()
{
    mp_MasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdStationChangeReagent, CStationCommandInterface>
            (&CStationCommandInterface::ChangeReagentInStation, this);

    mp_MasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdStationResetData, CStationCommandInterface>
            (&CStationCommandInterface::ResetStationData, this);

    mp_MasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdStationSetAsFull, CStationCommandInterface>
            (&CStationCommandInterface::SetStationAsFull, this);

    mp_MasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdStationSetAsEmpty, CStationCommandInterface>
            (&CStationCommandInterface::SetStationAsEmpty, this);
}

void CStationCommandInterface::ChangeReagentInStation(Global::tRefType Ref,
                             const MsgClasses::CmdStationChangeReagent & cmd,
                             Threads::CommandChannel& AckCommandChannel)
{
    bool Result = true;
    CDashboardStation* pDashboardStation = static_cast<CDataContainer*>(mp_DataContainer)->StationList->GetDashboardStation(cmd.StationID());
    if (pDashboardStation)
    {
        pDashboardStation->SetDashboardReagentID(cmd.ReagentID());
        pDashboardStation->ResetData();
    }
    else
      Result = false;
    if (!Result) {
        // If error occurs , get errors and send errors to GUI
        ListOfErrors_t &ErrorList = static_cast<CDataContainer*>(mp_DataContainer)->StationList->GetErrorList();
        QString ErrorString;
        Helper::ErrorIDToString(ErrorList, ErrorString);
        mp_MasterThreadController->SendAcknowledgeNOK(Ref, AckCommandChannel, ErrorString, Global::GUIMSGTYPE_INFO);
    }
    else {
        //BroadCast Command
        mp_MasterThreadController->SendAcknowledgeOK(Ref, AckCommandChannel);
        MsgClasses::CmdStationChangeReagent* p_Command = new MsgClasses::CmdStationChangeReagent(1000, cmd.StationID(), cmd.ReagentID());
        mp_MasterThreadController->BroadcastCommand(Global::CommandShPtr_t(p_Command));
        qDebug()<<"\n\n Change reagent in station Success";
    }
    static_cast<CDataContainer*>(mp_DataContainer)->StationList->Write();
}

 void CStationCommandInterface::ResetStationData(Global::tRefType Ref,
                       const MsgClasses::CmdStationResetData & cmd,
                       Threads::CommandChannel & AckCommandChannel)
 {
     bool Result = true;
     CDashboardStation* pDashboardStation = static_cast<CDataContainer*>(mp_DataContainer)->StationList->GetDashboardStation(cmd.StationID());
     if (pDashboardStation)
         pDashboardStation->ResetData();
     else
       Result = false;
     if (!Result) {
         // If error occurs , get errors and send errors to GUI
         ListOfErrors_t &ErrorList = static_cast<CDataContainer*>(mp_DataContainer)->StationList->GetErrorList();
         QString ErrorString;
         Helper::ErrorIDToString(ErrorList, ErrorString);
         mp_MasterThreadController->SendAcknowledgeNOK(Ref, AckCommandChannel, ErrorString, Global::GUIMSGTYPE_INFO);
     }
     else {
         //BroadCast Command
         mp_MasterThreadController->SendAcknowledgeOK(Ref, AckCommandChannel);
         MsgClasses::CmdStationResetData* p_Command = new MsgClasses::CmdStationResetData(1000, cmd.StationID());
         mp_MasterThreadController->BroadcastCommand(Global::CommandShPtr_t(p_Command));
         qDebug()<<"\n\n Reset station data Success";
     }
     static_cast<CDataContainer*>(mp_DataContainer)->StationList->Write();
 }

 void CStationCommandInterface::SetStationAsFull(Global::tRefType Ref,
                       const MsgClasses::CmdStationSetAsFull& cmd,
                       Threads::CommandChannel & AckCommandChannel)
 {
     bool Result = true;
     CDashboardStation* pDashboardStation = static_cast<CDataContainer*>(mp_DataContainer)->StationList->GetDashboardStation(cmd.StationID());
     if (pDashboardStation)
     {
        pDashboardStation->SetDashboardReagentStatus("Full");
        pDashboardStation->ResetData();
     }
     else
       Result = false;
     if (!Result) {
         // If error occurs , get errors and send errors to GUI
         ListOfErrors_t &ErrorList = static_cast<CDataContainer*>(mp_DataContainer)->StationList->GetErrorList();
         QString ErrorString;
         Helper::ErrorIDToString(ErrorList, ErrorString);
         mp_MasterThreadController->SendAcknowledgeNOK(Ref, AckCommandChannel, ErrorString, Global::GUIMSGTYPE_INFO);
     }
     else {
         //BroadCast Command
         mp_MasterThreadController->SendAcknowledgeOK(Ref, AckCommandChannel);
         MsgClasses::CmdStationResetData* p_Command = new MsgClasses::CmdStationResetData(1000, cmd.StationID());
         mp_MasterThreadController->BroadcastCommand(Global::CommandShPtr_t(p_Command));
         qDebug()<<"\n\n SetStationAsFull Success";
     }
     static_cast<CDataContainer*>(mp_DataContainer)->StationList->Write();
 }

 void CStationCommandInterface::SetStationAsEmpty(Global::tRefType Ref,
                        const MsgClasses::CmdStationSetAsEmpty& Cmd,
                        Threads::CommandChannel& AckCommandChannel)
 {
     bool Result = true;
     CDashboardStation* pDashboardStation = static_cast<CDataContainer*>(mp_DataContainer)->StationList->GetDashboardStation(Cmd.StationID());
     if (pDashboardStation)
         pDashboardStation->SetDashboardReagentStatus("Empty");
     else
       Result = false;
     if (!Result) {
         // If error occurs , get errors and send errors to GUI
         ListOfErrors_t &ErrorList = static_cast<CDataContainer*>(mp_DataContainer)->StationList->GetErrorList();
         QString ErrorString;
         Helper::ErrorIDToString(ErrorList, ErrorString);
         mp_MasterThreadController->SendAcknowledgeNOK(Ref, AckCommandChannel, ErrorString, Global::GUIMSGTYPE_INFO);
     }
     else {
         //BroadCast Command
         mp_MasterThreadController->SendAcknowledgeOK(Ref, AckCommandChannel);
         MsgClasses::CmdStationResetData* p_Command = new MsgClasses::CmdStationResetData(1000, Cmd.StationID());
         mp_MasterThreadController->BroadcastCommand(Global::CommandShPtr_t(p_Command));
         qDebug()<<"\n\n SetStationAsFull Success";
     }
     static_cast<CDataContainer*>(mp_DataContainer)->StationList->Write();
 }


}
