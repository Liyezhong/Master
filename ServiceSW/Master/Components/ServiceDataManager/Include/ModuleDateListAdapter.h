/****************************************************************************/
/*! \file ServiceDataManager/Include/ModuleDataListAdapter.h
 *
 *  \brief Definition for CModuleDataListAdapter class.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-07-31
 *   $Author:  $ Dixiong Li
 *
 *  \b Company:
 *
 *      Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#ifndef MODULEDATELISTADAPTER_H
#define MODULEDATELISTADAPTER_H

#include "DataManager/Containers/InstrumentHistory/Include/InstrumentHistory.h"
#include "ServiceDataManager/Include/ModuleDataList.h"

namespace ServiceDataManager
{

class CModuleDateListAdapter
{
    enum LifeCycleType {
        TIME,
        CYCLE
    };
public:
    CModuleDateListAdapter(ServiceDataManager::CModuleDataList* ModuleList);
    ~CModuleDateListAdapter();

    bool run();

private:    
    bool InitContainers();
    void SetOvenLifeCycle();
    void SetRVLifeCycle();
    void SetRetortLifeCycle();
    void SetLaSystemLifeCycle();
    void SetMCLifeCycle();

    void SetSubModuleLifeCycle(ServiceDataManager::CSubModule* SubModule, DataManager::CSubModule* SubMouleRef, LifeCycleType Type);
    void SetSubModuleLifeCycle(ServiceDataManager::CSubModule* SubModule, int LifeCycle, LifeCycleType Type);

    ServiceDataManager::CModuleDataList* mp_ModuleList;
    DataManager::CInstrumentHistory* mp_InstrumentHistoryRef;
    DataManager::CModule* mp_OvenRef;
    DataManager::CModule* mp_RetortRef;
    DataManager::CModule* mp_LaRef;
    DataManager::CModule* mp_RVRef;
    DataManager::CModule* mp_OtherRef;

};

}// namespace ServiceDataManager

#endif // MODULEDATELISTADAPTER_H
