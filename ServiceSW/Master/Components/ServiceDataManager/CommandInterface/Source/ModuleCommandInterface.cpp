
/****************************************************************************/
/*! \file ModuleCommandInterface.cpp
 *
 *  \brief Module Command Interface  Implementation
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

#include "ServiceDataManager/CommandInterface/Include/ModuleCommandInterface.h"
#include "ServiceMasterThreadController/Include/ServiceMasterThreadController.h"

namespace DataManager {

int CModuleCommandInterface::m_ModuleCount = 0;
const int CModuleCommandInterface::m_TotalModuleCount = 13;

/****************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam p_DataManager = DataManager object
 *  \iparam p_MasterThreadController = Master thread Cntrl
 *  \iparam p_DataContainer = DataContainer pointer
 */
/****************************************************************************/
CModuleCommandInterface::CModuleCommandInterface(CServiceDataManager *p_DataManager,
                         Threads::ServiceMasterThreadController *p_MasterThreadController,
                         ServiceDataContainer *p_DataContainer):
    CCommandInterfaceBase(p_DataManager, p_MasterThreadController, p_DataContainer)
{
    mp_InstrumentHistory = new CModuleDataList();
    RegisterCommands();
}

/****************************************************************************/
/**
 * \brief Register Commands related to ModuleList container
 */
/****************************************************************************/
void CModuleCommandInterface::RegisterCommands()
{
    mp_MasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdModuleListUpdate, DataManager::CModuleCommandInterface>
            (&CModuleCommandInterface::ModuleUpdateHandler, this);
}


/****************************************************************************/
/**
 * \brief Function which handles CmdModuleListUpdate
 *  \iparam Ref = Command reference
 *  \iparam Cmd = Command
 *  \AckCommandChannel = Command channel to send acknowledge
 */
/****************************************************************************/

void CModuleCommandInterface::ModuleUpdateHandler(Global::tRefType Ref, const MsgClasses::CmdModuleListUpdate &Cmd,
                                                  Threads::CommandChannel &AckCommandChannel)
{
    qDebug()<<"CModuleCommandInterface::ModuleUpdateHandler "<<m_ModuleCount;
    DataManager::CModule* Module = new CModule();   

    Module = const_cast<CModule*>(Cmd.GetModuleData());

    qDebug()<<"Module Details = "<<Module;
    qDebug()<<"GetModuleName "<<Module->GetModuleName();

    ++m_ModuleCount;
    mp_InstrumentHistory->AddModule(Module);
    if (m_TotalModuleCount == m_ModuleCount) {

        // Function to be added to rearrange the module list
        RearrangeModuleListInfo(mp_InstrumentHistory);
        //mp_InstrumentHistory->Write("FilePath");
        delete mp_InstrumentHistory;
    }
}

void CModuleCommandInterface::RearrangeModuleListInfo(CModuleDataList *InstrumentHistory)
{
    // code to be added
}

} // end of namespace DataManager
