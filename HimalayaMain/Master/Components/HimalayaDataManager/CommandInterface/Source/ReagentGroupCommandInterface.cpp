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

#include <QDebug>

#include "Threads/Include/MasterThreadController.h"
#include "Threads/Include/CommandChannel.h"

//#include "HimalayaDataContainer/Containers/ReagentGroup/Commands/Include/CmdReagentGroupUpdateNew.h"

#include "HimalayaDataManager/Include/DataContainer.h"

namespace DataManager {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \param[in] masterThreadController reference to master thread controllor
 *  \param[in] dataContainer          reference to data container
 */
/****************************************************************************/
ReagentGroupCommandInterface::ReagentGroupCommandInterface(
        Threads::MasterThreadController &masterThreadController,
        DataContainer                   &dataContainer)
    : CommandInterfaceBase(masterThreadController, dataContainer)
{
    this->RegisterCommands();
}

/****************************************************************************/
/**
 * \brief Register Commands related to Reagent Container
 */
/****************************************************************************/
void ReagentGroupCommandInterface::RegisterCommands(void)
{
    //m_MasterThreadController->RegisterCommandForProcessing<MsgClassesNew::CmdReagentGroupUpdate, DataManager::ReagentGroupCommandInterface>
      //      (&ReagentGroupCommandInterface::UpdateReagentGroup, this);
}

/****************************************************************************/
/**
 * \brief Function which handles CmdReagentGroupUpdate
 */
/****************************************************************************/
void ReagentGroupCommandInterface::UpdateReagentGroup(
        Global::tRefType ref,
        const MsgClassesNew::CmdReagentGroupUpdate &cmd,
        Threads::CommandChannel &ackCommandChannel)
{
    /*QByteArray data = cmd.GetData();
    QDataStream stream(&data, QIODevice::ReadWrite);
    stream.setVersion(static_cast<int>(QDataStream::Qt_4_0));

    const QString &id = m_DataContainer->GetReagentGroupList()->UpdateReagentGroup(stream);
    if (id.isEmpty())
    {
        // If error occurs , get errors and send errors to GUI
        m_MasterThreadController->SendAcknowledgeNOK(ref, ackCommandChannel);
        qDebug()<<"\n\n Update Reagent Failed";
    }
    else
    {
        //BroadCast Command
        m_MasterThreadController->SendAcknowledgeOK(ref, ackCommandChannel);

        ReagentGroupPtr_t p_ReagentGroup = m_DataContainer->GetReagentGroupList()->Find(id);

        data.clear();
        stream.device()->reset();
        stream << *p_ReagentGroup;

        MsgClassesNew::CmdReagentGroupUpdate* p_Command =
                new MsgClassesNew::CmdReagentGroupUpdate(1000, stream);
        m_MasterThreadController->BroadcastCommand(
                    Global::CommandShPtr_t(p_Command));

        qDebug()<<"\n\n Update Reagent Success";
    }*/
}


}// End of namespace DataManager



