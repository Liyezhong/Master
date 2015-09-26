/****************************************************************************/
/*! \file ReagentCommandInterface.cpp
 *
 *  \brief Reagent Command Interface  Implementation
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-07-30, 2013-02-02
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
#include "HimalayaDataManager/CommandInterface/Include/ReagentCommandInterface.h"
#include "NetCommands/Include/CmdConfigurationFile.h"
#include "DataManager/Helper/Include/DataManagerEventCodes.h"

//lint -sem(DataManager::CReagentCommandInterface::AddReagent, custodial(1))
//lint -sem(DataManager::CReagentCommandInterface::DeleteReagent, custodial(2))
//lint -sem(DataManager::CReagentCommandInterface::UpdateReagent, custodial(3))

namespace DataManager {
/****************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam p_DataManager = Datamanager object
 *  \iparam p_MasterThreadController = Master thread Cntrl
 *  \iparam p_DataContainer = DataContainer pointer
 */
/****************************************************************************/
CReagentCommandInterface::CReagentCommandInterface(CDataManager *p_DataManager, Threads::MasterThreadController *p_MasterThreadController, CDataContainer *p_DataContainer):
    CCommandInterfaceBase(p_DataManager, p_MasterThreadController, p_DataContainer)
{
    try {
    }
    catch (...) {
        qDebug()<<"Connecting signal slot failed";
    }

    RegisterCommands();
}

/****************************************************************************/
/**
 * \brief Register Commands related to Reagent Container
 */
/****************************************************************************/
void CReagentCommandInterface::RegisterCommands() {
    Q_ASSERT(mp_MasterThreadController);
    if (mp_MasterThreadController)
    {
        mp_MasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdReagentUpdate, DataManager::CReagentCommandInterface>
                (&CReagentCommandInterface::UpdateReagent, this);
        mp_MasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdReagentAdd, DataManager::CReagentCommandInterface>
                (&CReagentCommandInterface::AddReagent, this);
        mp_MasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdReagentRemove, DataManager::CReagentCommandInterface>
                (&CReagentCommandInterface::DeleteReagent, this);
    }
}

/****************************************************************************/
/**
 * \brief Function which handles CmdReagentAdd
 *  \iparam Ref = Command reference
 *  \iparam Cmd = Command
 *  \param AckCommandChannel = Command channel to send acknowledge
 */
/****************************************************************************/
void CReagentCommandInterface::AddReagent(Global::tRefType Ref, const MsgClasses::CmdReagentAdd &Cmd, Threads::CommandChannel &AckCommandChannel)
{
    Q_ASSERT(mp_MasterThreadController);
    if (!mp_MasterThreadController)
        return;

    QByteArray ReagentData(const_cast<QByteArray &>(Cmd.GetReagentData()));
    QDataStream ReagentDataStream(&ReagentData, QIODevice::ReadWrite);
    ReagentDataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));
    CReagent Reagent;
    ReagentDataStream >> Reagent;
    bool Result = true;
    Result = static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ReagentList->AddReagent(&Reagent);
    if (!Result) {
        // If error occurs , get errors and send errors to GUI
        ListOfErrors_t &ErrorList = static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ReagentList->GetErrorList();
        QString ErrorString;
        DataManager::Helper::ErrorIDToString(ErrorList, ErrorString);
        qDebug()<<"\n\n\nAdding Reagent Failed";
        mp_MasterThreadController->SendAcknowledgeNOK(Ref, AckCommandChannel, ErrorString, Global::GUIMSGTYPE_INFO);
    }
    else {
        //BroadCast Command
        (void)static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ReagentList->GetNextFreeReagentID(true);
        ReagentData.clear();
        (void)ReagentDataStream.device()->reset();
        ReagentDataStream << Reagent;
        SendAckAndUpdateGUI(Ref, AckCommandChannel, Global::CommandShPtr_t(
                                new MsgClasses::CmdReagentAdd(10000, ReagentDataStream)));
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_REAGENT_ADD_REAGENT,
                                                   Global::tTranslatableStringList() << Reagent.GetReagentName()
                                                   <<QString::number(Reagent.GetMaxCassettes())
                                                   <<QString::number(Reagent.GetMaxCycles())
                                                   <<QString::number(Reagent.GetMaxDays())
                                                   <<Reagent.GetGroupID());
        qDebug()<<"\n\n\n Adding New Reagent Success";
        /*lint -e429 */
    }
    (void)static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ReagentList->Write();

}

/****************************************************************************/
/**
 * \brief Function which handles CmdReagentRemove
 *  \iparam Ref = Command reference
 *  \iparam Cmd = Command
 *  \param AckCommandChannel = Command channel to send acknowledge
 */
/****************************************************************************/
void CReagentCommandInterface::DeleteReagent(Global::tRefType Ref, const MsgClasses::CmdReagentRemove &Cmd, Threads::CommandChannel &AckCommandChannel)
{
    Q_ASSERT(mp_MasterThreadController);
    if (!mp_MasterThreadController)
        return;

    bool Result = true;
    bool ResultUpdateStation = true;
    //Also the Station should be updated
    QString ReagentName  = static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ReagentList->GetReagent(Cmd.GetReagentID())->GetReagentName();
    ResultUpdateStation = static_cast<DataManager::CDataContainer*>(mp_DataContainer)->StationList->UpdateStationsByReagentDelete(Cmd.GetReagentID());
    Result = static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ReagentList->DeleteReagent(Cmd.GetReagentID());
    if (!Result || !ResultUpdateStation) {
        // If error occurs , get errors and send errors to GUI
        ListOfErrors_t &ErrorList = static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ReagentList->GetErrorList();
        QString ErrorString;
        DataManager::Helper::ErrorIDToString(ErrorList, ErrorString);
        mp_MasterThreadController->SendAcknowledgeNOK(Ref, AckCommandChannel, ErrorString, Global::GUIMSGTYPE_INFO);
             qDebug()<<"\n\n Delete Reagent Failed";
    }
    else {
        SendAckAndUpdateGUI(Ref, AckCommandChannel, Global::CommandShPtr_t(
                                new MsgClasses::CmdReagentRemove(10000, Cmd.GetReagentID())));
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_REAGENT_DEL_REAGENT,
                                                   Global::tTranslatableStringList() << ReagentName);
        qDebug()<<"\n\n Delete Reagent Success";
    }
    (void)static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ReagentList->Write();
    (void)static_cast<DataManager::CDataContainer*>(mp_DataContainer)->StationList->Write();
}

/****************************************************************************/
/**
 * \brief Function which handles CmdReagentUpdate
 *  \iparam Ref = Command reference
 *  \iparam Cmd = Command
 *  \param AckCommandChannel = Command channel to send acknowledge
 */
/****************************************************************************/
void CReagentCommandInterface::UpdateReagent(Global::tRefType Ref, const MsgClasses::CmdReagentUpdate &Cmd, Threads::CommandChannel &AckCommandChannel)
{
    Q_ASSERT(mp_MasterThreadController);
    if (!mp_MasterThreadController)
        return;

    QByteArray ReagentData(const_cast<QByteArray &>(Cmd.GetReagentData()));
    QDataStream ReagentDataStream(&ReagentData, QIODevice::ReadWrite);
    ReagentDataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));
    CReagent Reagent;
    ReagentDataStream >> Reagent;
    bool Result = true;
    Result = static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ReagentList->UpdateReagent(&Reagent);
    if (!Result) {
        // If error occurs , get errors and send errors to GUI
        ListOfErrors_t &ErrorList = static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ReagentList->GetErrorList();
        QString ErrorString;
        DataManager::Helper::ErrorIDToString(ErrorList, ErrorString);
        mp_MasterThreadController->SendAcknowledgeNOK(Ref, AckCommandChannel, ErrorString, Global::GUIMSGTYPE_INFO);
    }
    else {
        ReagentData.clear();
        (void)ReagentDataStream.device()->reset();
        ReagentDataStream << Reagent;
        SendAckAndUpdateGUI(Ref, AckCommandChannel, Global::CommandShPtr_t(
                                new MsgClasses::CmdReagentUpdate(10000, ReagentDataStream)));
        if(Reagent.IsLeicaReagent())
        {
            Global::EventObject::Instance().RaiseEvent(EVENT_DM_REAGENT_UPDATE_REAGENT,
                                                       Global::tTranslatableStringList() << Global::TranslatableString(Reagent.GetReagentNameID().toUInt())
                                                       <<QString::number(Reagent.GetMaxCassettes())
                                                       <<QString::number(Reagent.GetMaxCycles())
                                                       <<QString::number(Reagent.GetMaxDays())
                                                       <<Reagent.GetGroupID());
        }
        else
        {
            Global::EventObject::Instance().RaiseEvent(EVENT_DM_REAGENT_UPDATE_REAGENT,
                                                       Global::tTranslatableStringList() << Reagent.GetReagentName()
                                                       <<QString::number(Reagent.GetMaxCassettes())
                                                       <<QString::number(Reagent.GetMaxCycles())
                                                       <<QString::number(Reagent.GetMaxDays())
                                                       <<Reagent.GetGroupID());
        }
        qDebug()<<"\n\n Update Reagent Success";
    }
    (void)static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ReagentList->Write();
}

}// End of namespace DataManager



