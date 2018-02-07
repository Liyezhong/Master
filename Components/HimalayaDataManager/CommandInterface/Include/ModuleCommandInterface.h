/****************************************************************************/
/*! \file ModuleCommandInterface.h
 *
 *  \brief  Module Command Interface definition
 *
 *  $Version:   $ 1.0
 *  $Date:      $ 2013-01-14
 *  $Author:    $ Soumya. D
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

#ifndef DATAMANAGER_MODULECOMMANDINTERFACE_H
#define DATAMANAGER_MODULECOMMANDINTERFACE_H

#include "DataManager/CommandInterface/Include/CommandInterfaceBase.h"
#include "HimalayaDataManager/Include/DataManager.h"
#include "DataManager/Containers/InstrumentHistory/Commands/Include/CmdModuleListUpdate.h"

//lint -sem(DataManager::CModuleCommandInterface::RegisterCommands,initializer)

namespace DataManager {

class CInstrumentHistory;
/****************************************************************************/
/**
 * \brief ModuleList container command interface definition
 */
/****************************************************************************/

class CModuleCommandInterface : public CCommandInterfaceBase
{
public:   
    explicit CModuleCommandInterface(CDataManager *p_DataManager, Threads::MasterThreadController *p_MasterThreadController,
                            CDataContainer *p_DataContainer);

    ~CModuleCommandInterface();

private:
    Q_DISABLE_COPY(CModuleCommandInterface)

    void RegisterCommands();

    /****************************************************************************/
    /**
     * \brief Function which handles CmdModuleListUpdate
     *
     * \iparam Cmd = Command
     * \iparam AckCommandChannel = Command channel to send acknowledge
     */
    /****************************************************************************/
    void ModuleUpdateHandler(Global::tRefType, const MsgClasses::CmdModuleListUpdate &Cmd,
                             Threads::CommandChannel &AckCommandChannel);

    CInstrumentHistory *mp_InstrumentHistory;  //!< Instrument History Object
    CModuleDataList *mp_DataModuleList;        //!< ModuledataList object
    QMap<QString, QString> m_SubModuleMap;  //!< Map of Submodule names with its Module Names
};

} // end of namespace DataManager

#endif // DATAMANAGER_MODULECOMMANDINTERFACE_H
