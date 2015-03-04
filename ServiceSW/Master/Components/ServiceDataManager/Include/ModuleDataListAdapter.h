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

/****************************************************************************/
/*!
 *  \brief  This class implements Module List
 *  Set InstrumentHistory operationTime from InstrumentHistoryRef
 */
/****************************************************************************/
class CModuleDataListAdapter
{
    /*! life cycle type */
    enum LifeCycleType {
        TIME,
        CYCLE
    };
public:
    /****************************************************************************/
    /**
     * \brief Constructor.
     * \param ModuleList = the pointer of Module list
     */
    /****************************************************************************/
    CModuleDataListAdapter(ServiceDataManager::CModuleDataList* ModuleList);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CModuleDataListAdapter();

    /****************************************************************************/
    /**
     *  \brief To run adapt.
     *  \return true or false
     */
    /****************************************************************************/
    bool Run();

private:
    /****************************************************************************/
    /**
     * \brief Init instrument ref containers.
     */
    /****************************************************************************/
    bool InitContainers();

    /****************************************************************************/
    /**
     * \brief Set oven life cycle.
     */
    /****************************************************************************/
    void SetOvenLifeCycle();

    /****************************************************************************/
    /**
     * \brief Set RV life cycle.
     */
    /****************************************************************************/
    void SetRVLifeCycle();

    /****************************************************************************/
    /**
     * \brief Set retort life cycle.
     */
    /****************************************************************************/
    void SetRetortLifeCycle();

    /****************************************************************************/
    /**
     * \brief Set L&A system life cycle.
     */
    /****************************************************************************/
    void SetLaSystemLifeCycle();

    /****************************************************************************/
    /**
     * \brief Set Main Control life cycle.
     */
    /****************************************************************************/
    void SetMCLifeCycle();

    /****************************************************************************/
    /**
     * \brief Set sub module life cycle from sub module ref.
     * \!param SubModule = sub module
     * \!param SubModuleRef = sub module ref
     * \!param Type = the type of life cycle
     */
    /****************************************************************************/
    void SetSubModuleLifeCycle(ServiceDataManager::CSubModule* SubModule, DataManager::CSubModule* SubMouleRef, LifeCycleType Type);

    /****************************************************************************/
    /**
     * \brief Set sub module life cycle.
     * \!param SubModule = sub module
     * \!param LifeCycle = life cycle value
     * \!param Type = the type of life cycle
     */
    /****************************************************************************/
    void SetSubModuleLifeCycle(ServiceDataManager::CSubModule* SubModule, qreal LifeCycle, LifeCycleType Type);

    /****************************************************************************/
    /**
     * \brief Get LA pressure cycle number.
     * \!param ParamName = name of param
     * \!param Type = the type of life cycle
     */
    /****************************************************************************/
    qreal GetLAPressureCycle(QString ParamName, LifeCycleType Type);

    ServiceDataManager::CModuleDataList* mp_ModuleList;        //!< Store module list data
    DataManager::CInstrumentHistory* mp_InstrumentHistoryRef;  //!< Store module list ref data
    DataManager::CModule* mp_OvenRef;                          //!< pointer of oven module ref
    DataManager::CModule* mp_RetortRef;                        //!< pointer of retort module ref
    DataManager::CModule* mp_LaRef;                            //!< pointer of L&A system module ref
    DataManager::CModule* mp_RVRef;                            //!< pointer of Rotary Valve module ref
    DataManager::CModule* mp_OtherRef;                         //!< pointer of Main Control module ref

};

}// namespace ServiceDataManager

#endif // MODULEDATELISTADAPTER_H
