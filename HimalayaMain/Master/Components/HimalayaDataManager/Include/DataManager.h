/****************************************************************************/
/*! \file DataManager.h
 *
 *  \brief Definition file for class CDataManager.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2011-07-20, 2013-03-17
 *  $Author:    $ F. Toth
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

#ifndef DATAMANAGER_OBSOLETE_DATAMANAGER_H
#define DATAMANAGER_OBSOLETE_DATAMANAGER_H

#include "../Include/DataContainer.h"
#include "DataManager/Include/DataManagerBase.h"

namespace DataManager {
class CProgramCommandInterface;
class CReagentCommandInterface;
class CStationCommandInterface;
class CReagentGroupCommandInterface;
class CUserSettingsCommandInterface;

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CDataManager
 */
/****************************************************************************/
class CDataManager : public CDataManagerBase
{
    Q_OBJECT
private:
    bool m_UpdateList;       ///<  Definition/Declaration of variable m_UpdateList
    CDataContainer* mp_DataContainer;       ///<  Definition/Declaration of variable mp_DataContainer
    CStationCommandInterface *mp_StationCommandInterface; //!< handles commands related to  Station container
    CReagentCommandInterface *mp_ReagentCommandInterface; //!< handles commands related to reagent container
    CReagentGroupCommandInterface *mp_ReagentGroupCommandInterface;       ///<  Definition/Declaration of variable mp_ReagentGroupCommandInterface
protected:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function DeinitializeDataContainer
     *
     *  \return from DeinitializeDataContainer
     */
    /****************************************************************************/
    bool DeinitializeDataContainer();

public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CDataManager
     *
     *  \param p_HimalayaMasterThreadController = Threads::MasterThreadController type parameter
     *
     *  \return from CDataManager
     */
    /****************************************************************************/
    CDataManager(Threads::MasterThreadController *p_HimalayaMasterThreadController);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CDataManager
     *
     *  \param p_HimalayaMasterThreadController = Threads::MasterThreadController type parameter
     *  \param Path =  QString type parameter
     *
     *  \return from CDataManager
     */
    /****************************************************************************/
    CDataManager(Threads::MasterThreadController *p_HimalayaMasterThreadController, QString Path);
    virtual ~CDataManager();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function InitializeDataContainer
     *
     *  \return from InitializeDataContainer
     */
    /****************************************************************************/
    quint32 InitializeDataContainer();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetDataContainer
     *
     *  \return from GetDataContainer
     */
    /****************************************************************************/
    const CDataContainer* GetDataContainer() { return mp_DataContainer; }

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetProgramList
     *
     *  \return from GetProgramList
     */
    /****************************************************************************/
    CDataProgramList* GetProgramList();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetProgramList
     *
     *  \param p_ProgramList = CDataProgramList type parameter
     *
     *  \return from GetProgramList
     */
    /****************************************************************************/
    bool GetProgramList(CDataProgramList *p_ProgramList);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetProgramSettings
     *
     *  \return from GetProgramSettings
     */
    /****************************************************************************/
    CProgramSettings* GetProgramSettings();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetProgramSettings
     *
     *  \param p_ProgramList = CProgramSettings type parameter
     *
     *  \return from GetProgramSettings
     */
    /****************************************************************************/
    bool GetProgramSettings(CProgramSettings *p_ProgramList);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetReagentGroupList
     *
     *  \return from GetReagentGroupList
     */
    /****************************************************************************/
    CDataReagentGroupList* GetReagentGroupList();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetReagentGroupList
     *
     *  \param p_ReagentGroupList = CDataReagentGroupList type parameter
     *
     *  \return from GetReagentGroupList
     */
    /****************************************************************************/
    bool GetReagentGroupList(CDataReagentGroupList *p_ReagentGroupList);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetReagentGroupColorList
     *
     *  \return from GetReagentGroupColorList
     */
    /****************************************************************************/
    CReagentGroupColorList* GetReagentGroupColorList();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetStationList
     *
     *  \param p_StationList = CDashboardDataStationList type parameter
     *
     *  \return from GetStationList
     */
    /****************************************************************************/
    bool GetStationList(CDashboardDataStationList *p_StationList);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetStationList
     *
     *  \return from GetStationList
     */
    /****************************************************************************/
    CDashboardDataStationList* GetStationList();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetReagentList
     *
     *  \param p_ReagentList = CDataReagentList type parameter
     *
     *  \return from GetReagentList
     */
    /****************************************************************************/
    bool GetReagentList(CDataReagentList *p_ReagentList);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetReagentList
     *
     *  \return from GetReagentList
     */
    /****************************************************************************/
    CDataReagentList* GetReagentList();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetUserSettingsInterface
     *
     *  \return from GetUserSettingsInterface
     */
    /****************************************************************************/
    CUserSettingsInterface* GetUserSettingsInterface();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetUserSettings
     *
     *  \return from GetUserSettings
     */
    /****************************************************************************/
    CHimalayaUserSettings* GetUserSettings();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetDeviceConfigurationInterface
     *
     *  \return from GetDeviceConfigurationInterface
     */
    /****************************************************************************/
    CDeviceConfigurationInterface* GetDeviceConfigurationInterface();
    CProgramCommandInterface *mp_ProgramCommandInterface; //!< Handles commands related to program container

    //function to save data during shutdown
    void SaveDataOnShutdown();

signals:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal StationUpdate
     */
    /****************************************************************************/
    void StationUpdate(CDashboardStation &Station);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal StationContainerUpdate
     */
    /****************************************************************************/
    void StationContainerUpdate();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ReagentUpdate
     */
    /****************************************************************************/
    void ReagentUpdate(CReagent &Reargent);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal ReagentContainerUpdate
     */
    /****************************************************************************/
    void ReagentContainerUpdate(CDataReagentList &);

};
}// namespace DataManager

#endif // DATAMANAGER_OBSOLETE_DATAMANAGER_H


