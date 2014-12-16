/****************************************************************************/
/*! \file ModuleCommandInterface.h
 *
 *  \brief  Module Command Interface definition
 *
 *  $Version:   $ 0.1
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

#include "ServiceDataManager/CommandInterface/Include/CommandInterfaceBase.h"
#include "ServiceDataManager/Include/ServiceDataManager.h"
#include "ServiceDataManager/Include/ServiceDataContainer.h"
#include "DataManager/Containers/InstrumentHistory/Commands/Include/CmdModuleListUpdate.h"

namespace DataManager {

/****************************************************************************/
/**
 * \brief ModuleList container command interface definition
 */
/****************************************************************************/

class CModuleCommandInterface :public CCommandInterfaceBase
{
public:   
    CModuleCommandInterface(CServiceDataManager *p_DataManager, Threads::ServiceMasterThreadController *p_MasterThreadController,
                            ServiceDataContainer *p_DataContainer);

	/****************************************************************************/
	/**
	 * \brief rearrange module list information
     * \iparam InstrumentHistory = module list pointer
	 */
	/****************************************************************************/
    void RearrangeModuleListInfo(CModuleDataList* InstrumentHistory);

private:
    void RegisterCommands();
    void ModuleUpdateHandler(Global::tRefType, const MsgClasses::CmdModuleListUpdate &Cmd,
                             Threads::CommandChannel &AckCommandChannel);

    CModuleDataList *mp_InstrumentHistory;
    static const int m_TotalModuleCount;
    static int m_ModuleCount;

};

} // end of namespace DataManager

#endif // DATAMANAGER_MODULECOMMANDINTERFACE_H
