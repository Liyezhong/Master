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
#include "DataManager/Helper/Include/DataManagerEventCodes.h"


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
    Q_ASSERT(mp_MasterThreadController);
    if (!mp_MasterThreadController)
        return;

    mp_MasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdStationChangeReagent, CStationCommandInterface>
            (&CStationCommandInterface::ChangeReagentInStation, this);

    mp_MasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdStationResetData, CStationCommandInterface>
            (&CStationCommandInterface::ResetStationData, this);

    mp_MasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdStationSetAsFull, CStationCommandInterface>
            (&CStationCommandInterface::SetStationAsFull, this);

    mp_MasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdStationSetAsEmpty, CStationCommandInterface>
            (&CStationCommandInterface::SetStationAsEmpty, this);

    mp_MasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdUpdateStationReagentStatus, CStationCommandInterface>
            (&CStationCommandInterface::UpdateStationReagentStatus, this);

}

void CStationCommandInterface::ChangeReagentInStation(Global::tRefType Ref,
                             const MsgClasses::CmdStationChangeReagent & cmd,
                             Threads::CommandChannel& AckCommandChannel)
{
    Q_ASSERT(mp_MasterThreadController);
    if (!mp_MasterThreadController)
        return;

    bool Result = true;
    CDashboardStation* pDashboardStation = static_cast<CDataContainer*>(mp_DataContainer)->StationList->GetDashboardStation(cmd.StationID());
    if (pDashboardStation)
    {
        pDashboardStation->SetDashboardReagentID(cmd.ReagentID());
        pDashboardStation->ResetData();
        pDashboardStation->SetDashboardReagentStatus("Empty");
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
        SendAckAndUpdateGUI(Ref, AckCommandChannel, Global::CommandShPtr_t(
                                new MsgClasses::CmdStationChangeReagent(10000, cmd.StationID(), cmd.ReagentID())));
        QString reagentName;
        QString reagentId = cmd.ReagentID();
        if ("" == reagentId)
        {
            reagentName = "None";
        }
        else
        {
            reagentName = static_cast<CDataContainer*>(mp_DataContainer)->ReagentList->GetReagent(cmd.ReagentID())->GetReagentName();
        }
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_STATION_CHANGE_STATION,
                                                   Global::tTranslatableStringList() << cmd.StationID()
                                                   << reagentName);

    }

    (void)static_cast<CDataContainer*>(mp_DataContainer)->StationList->Write();
}

 void CStationCommandInterface::ResetStationData(Global::tRefType Ref,
                       const MsgClasses::CmdStationResetData & cmd,
                       Threads::CommandChannel & AckCommandChannel)
 {
     Q_ASSERT(mp_MasterThreadController);
     if (!mp_MasterThreadController)
         return;

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
         SendAckAndUpdateGUI(Ref, AckCommandChannel, Global::CommandShPtr_t(
                                 new MsgClasses::CmdStationResetData(10000, cmd.StationID())));
         Global::EventObject::Instance().RaiseEvent(EVENT_DM_STATION_RESET_STATION,
                                                    Global::tTranslatableStringList() << cmd.StationID());
     }
     (void)static_cast<CDataContainer*>(mp_DataContainer)->StationList->Write();
 }

 void CStationCommandInterface::SetStationAsFull(Global::tRefType Ref,
                       const MsgClasses::CmdStationSetAsFull& cmd,
                       Threads::CommandChannel & AckCommandChannel)
 {
     Q_ASSERT(mp_MasterThreadController);
     if (!mp_MasterThreadController)
         return;

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
         SendAckAndUpdateGUI(Ref, AckCommandChannel, Global::CommandShPtr_t(
                                 new MsgClasses::CmdStationSetAsFull(10000, cmd.StationID())));
         Global::EventObject::Instance().RaiseEvent(EVENT_DM_STATION_SET_STATION_FULL,
                                                    Global::tTranslatableStringList() << cmd.StationID());
     }
     (void)static_cast<CDataContainer*>(mp_DataContainer)->StationList->Write();
 }

 void CStationCommandInterface::SetStationAsEmpty(Global::tRefType Ref,
                        const MsgClasses::CmdStationSetAsEmpty& Cmd,
                        Threads::CommandChannel& AckCommandChannel)
 {
     Q_ASSERT(mp_MasterThreadController);
     if (!mp_MasterThreadController)
         return;

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
         SendAckAndUpdateGUI(Ref, AckCommandChannel, Global::CommandShPtr_t(
                                 new MsgClasses::CmdStationSetAsEmpty(10000, Cmd.StationID())));
         Global::EventObject::Instance().RaiseEvent(EVENT_DM_STATION_SET_STATION_EMPTY,
                                                    Global::tTranslatableStringList() << Cmd.StationID());

     }
     (void)static_cast<CDataContainer*>(mp_DataContainer)->StationList->Write();
 }

//Based on the RMS mode to send CmdUpdateStationReagentStatus
 void CStationCommandInterface::UpdateStationReagentStatus(Global::tRefType Ref,
                        const MsgClasses::CmdUpdateStationReagentStatus &Cmd,
                        Threads::CommandChannel& AckCommandChannel)
 {
     Q_ASSERT(mp_MasterThreadController);
     if (!mp_MasterThreadController)
         return;

     const QStringList& Ids = Cmd.StationIDs();
     for (int i = 0; i < Ids.count(); i++)
     {
         CDashboardStation* pDashboardStation = static_cast<CDataContainer*>(mp_DataContainer)->StationList->GetDashboardStation(Ids[i]);
         if (pDashboardStation)
         {
             if(Cmd.CassetteCount() > 0)
             {

                 pDashboardStation->SetDashboardReagentActualCassettes(
                             pDashboardStation->GetDashboardReagentActualCassettes() + Cmd.CassetteCount());

             }
             else
             {
                 pDashboardStation->SetDashboardReagentActualCycles(
                             pDashboardStation->GetDashboardReagentActualCycles() + 1);
             }

         }
         else
         {
             qDebug() << "UpdateStationReagentStatus Failed.";
         }
     }

     if (mp_MasterThreadController) {
         mp_MasterThreadController->SendAcknowledgeOK(Ref, AckCommandChannel);
         //for update the data in UI
         (void)mp_MasterThreadController->SendCommand(Global::CommandShPtr_t(
                                                          new MsgClasses::CmdUpdateStationReagentStatus(10000, Cmd.StationIDs(), Cmd.CassetteCount())),
                                                      mp_MasterThreadController->GetCommandChannel(0)); //avoid lint 534
     }

    /*lint -e534 */
     static_cast<CDataContainer*>(mp_DataContainer)->StationList->Write();
 }
}
