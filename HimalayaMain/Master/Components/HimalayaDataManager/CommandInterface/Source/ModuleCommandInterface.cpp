/****************************************************************************/
/*! \file ModuleCommandInterface.cpp
 *
 *  \brief Module Command Interface  Implementation
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

#include "HimalayaDataManager/CommandInterface/Include/ModuleCommandInterface.h"
#include "DataManager/Containers/InstrumentHistory/Include/InstrumentHistory.h"
#include "Scheduler/Commands/Include/CmdSavedServiceInfor.h"

namespace DataManager {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_DataManager = DataManager object
 *  \iparam p_MasterThreadController = Master thread Cntrl
 *  \iparam p_DataContainer = DataContainer pointer
 */
/****************************************************************************/
CModuleCommandInterface::CModuleCommandInterface(CDataManager *p_DataManager,
                                                 Threads::MasterThreadController *p_MasterThreadController,
                                                 CDataContainer *p_DataContainer):
    CCommandInterfaceBase(p_DataManager, p_MasterThreadController, p_DataContainer),
    mp_InstrumentHistory(NULL),
    mp_DataModuleList(NULL)
{
    mp_InstrumentHistory = p_DataContainer->InstrumentHistory;

    //Review:Srivathsa: check for return value before assigning. throw exception or raise
    //                  event incase od NULL.
    mp_DataModuleList = p_DataContainer->InstrumentHistory->GetModuleList(0);
    RegisterCommands();

    int ModuleCount(0);
    int SubModuleCount(0);

    try
    {
        for(ModuleCount = 0; ModuleCount < mp_DataModuleList->GetNumberofModules(); ModuleCount ++)
        {
            CModule* p_Module = mp_DataModuleList->GetModule(ModuleCount);
            for(SubModuleCount = 0; SubModuleCount < p_Module->GetNumberofSubModules();SubModuleCount++)
            {
                CSubModule* p_SubModule = p_Module->GetSubModuleInfo(SubModuleCount);
                m_SubModuleMap.insert(p_SubModule->GetSubModuleName(), p_Module->GetModuleName());
            }
        }

    }
    CATCHALL();
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CModuleCommandInterface::~CModuleCommandInterface()
{
    mp_InstrumentHistory = NULL;
}

/****************************************************************************/
/**
 * \brief Register Commands related to ModuleList container
 */
/****************************************************************************/
void CModuleCommandInterface::RegisterCommands()
{
    if (mp_MasterThreadController) {
        mp_MasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdModuleListUpdate,
                DataManager::CModuleCommandInterface>(&CModuleCommandInterface::ModuleUpdateHandler, this);
    }

}


/****************************************************************************/
/**
 *  \brief Function which handles CmdModuleListUpdate
 *  \iparam Ref = Command reference
 *  \iparam Cmd = Command
 *  \iparam AckCommandChannel = Command channel to send acknowledge
 */
/****************************************************************************/
void CModuleCommandInterface::ModuleUpdateHandler(Global::tRefType Ref, const MsgClasses::CmdModuleListUpdate &Cmd,
                                                  Threads::CommandChannel &AckCommandChannel)
{
    //Review:Srivathsa: Check for null before access (mp_DataModuleList)
    try
    {
        QString ModuleName("");
        mp_MasterThreadController->SendAcknowledgeOK(Ref, AckCommandChannel);

        if(Cmd.m_Error)
        {
            Global::EventObject::Instance().RaiseEvent(EVENT_SERVICE_INFO_NOT_FOUND, Global::tTranslatableStringList() << Cmd.DeviceType());
            return;
        }

        CModule* Module = const_cast<CModule*>(Cmd.GetModuleData());
        CHECKPTR(Module);

        for (qint32 Counter = 0; Counter < Module->GetNumberofSubModules(); Counter++)
        {
            const DataManager::CSubModule *p_SubModule = Module->GetSubModuleInfo(Counter);
            //ModuleName = m_SubModuleMap.value(p_SubModule->GetSubModuleName());
            ModuleName = Module->GetModuleName();
            CModule *p_Module = mp_DataModuleList->GetModule(ModuleName);
            if(NULL == p_Module)
            {
                continue;
            }
            if(!p_Module->UpdateSubModule(p_SubModule))
            {
                Global::EventObject::Instance().RaiseEvent(Global::EVENT_GLOBAL_STRING_ID_DEBUG_MESSAGE,
                                                           Global::FmtArgs() << QString("CModuleCommandInterface::ModuleUpdateHandler, Error Updating SubModuleName = %1 , NoOfParameters = %2").arg(p_SubModule->GetSubModuleName())
                                                           .arg((int)p_SubModule->GetNumberOfParameters()));
                continue;
            }

        }
        (void)mp_InstrumentHistory->UpdateModuleList(mp_DataModuleList);
        if(!mp_InstrumentHistory->Write())
        {
            Global::EventObject::Instance().RaiseEvent(Global::EVENT_GLOBAL_STRING_ID_DEBUG_MESSAGE,
                                                       Global::FmtArgs() << QString("CModuleCommandInterface::ModuleUpdateHandler, InstrumentHistory write Failed !!!"));
        }
        else
        {
            if(mp_MasterThreadController)
            {
                mp_MasterThreadController->BroadcastCommand(Global::CommandShPtr_t(new MsgClasses::CmdSavedServiceInfor(5000, Cmd.DeviceType())));
            }

        }
    }
    CATCHALL();
}

} // end of namespace DataManager
