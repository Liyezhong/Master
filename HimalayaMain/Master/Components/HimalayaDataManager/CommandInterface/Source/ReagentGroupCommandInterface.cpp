/****************************************************************************/
/*! \file ReagentCommandInterface.cpp
 *
 *  \brief Reagent Command Interface  Implementation
 *
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-02-02
 *  $Author:    $ R. Wu
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 */
/****************************************************************************/
#include "HimalayaDataManager/CommandInterface/Include/ReagentGroupCommandInterface.h"
#include "Threads/Include/MasterThreadController.h"
#include "Threads/Include/CommandChannel.h"
#include "HimalayaDataManager/Include/DataContainer.h"
#include "HimalayaDataContainer/Containers/ReagentGroups/Commands/Include/CmdReagentGroupUpdate.h"


namespace DataManager {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \param[in] p_MasterThreadController reference to master thread controllor
 *  \param[in] p_DataContainer          reference to data container
 *  \param p_DataManager
 */
/****************************************************************************/

CReagentGroupCommandInterface::CReagentGroupCommandInterface(CDataManager *p_DataManager,
                                                           Threads::MasterThreadController *p_MasterThreadController,
                                                           CDataContainer *p_DataContainer):
                        CCommandInterfaceBase(p_DataManager, p_MasterThreadController, p_DataContainer)
{
    this->RegisterCommands();
}

/****************************************************************************/
/**
 * \brief Register Commands related to Reagent Container
 */
/****************************************************************************/
void CReagentGroupCommandInterface::RegisterCommands(void)
{
    Q_ASSERT(mp_MasterThreadController);
    if (!mp_MasterThreadController)
        return;

    mp_MasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdReagentGroupUpdate, DataManager::CReagentGroupCommandInterface>
            (&CReagentGroupCommandInterface::UpdateReagentGroup, this);
}

/****************************************************************************/
/**
 * \brief Function which handles CmdReagentGroupUpdate
 */
/****************************************************************************/
void CReagentGroupCommandInterface::UpdateReagentGroup(
        Global::tRefType ref,
        const MsgClasses::CmdReagentGroupUpdate &cmd,
        Threads::CommandChannel &ackCommandChannel)
{
    Q_ASSERT(mp_MasterThreadController);
    if (!mp_MasterThreadController)
        return;

    bool Result = true;
    CReagentGroup* pReagentGroup = static_cast<CDataContainer*>(mp_DataContainer)->ReagentGroupList->GetReagentGroup(cmd.GroupId());
    if (pReagentGroup)
        pReagentGroup->SetGroupColor(cmd.GroupColor());
    else
        Result = false;
    if (!Result)
    {
        // If error occurs , get errors and send errors to GUI
        mp_MasterThreadController->SendAcknowledgeNOK(ref, ackCommandChannel);
        qDebug()<<"\n\n Update Reagent Failed";
    }
    else
    {
        //BroadCast Command
        mp_MasterThreadController->SendAcknowledgeOK(ref, ackCommandChannel);
        mp_MasterThreadController->BroadcastCommand(
                    Global::CommandShPtr_t(new MsgClasses::CmdReagentGroupUpdate(1000, cmd.GroupId(), cmd.GroupColor())));

        qDebug()<<"\n\n Update Reagent Success";
    }
    (void)static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ReagentGroupList->Write();
}


}// End of namespace DataManager



