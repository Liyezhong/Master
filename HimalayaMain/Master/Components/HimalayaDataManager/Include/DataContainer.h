/****************************************************************************/
/*! \file Himalaya/HimalayaMain/Master/Components/HimalayaDataManager/Include/DataContainer.h
 *
 *  \brief Definition file for class CDataContainer.
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

#ifndef DATAMANAGER_OBSOLETE_DATACONTAINER_H
#define DATAMANAGER_OBSOLETE_DATACONTAINER_H

#include "HimalayaDataContainer/Containers/ReagentGroups/Include/DataReagentGroupList.h"
#include "HimalayaDataContainer/Containers/ReagentGroupColor/Include/ReagentGroupColorList.h"
#include "HimalayaDataContainer/Containers/Reagents/Include/DataReagentList.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Include/DashboardDataStationList.h"
#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramList.h"
#include "HimalayaDataContainer/Containers/Reagents/Include/DataReagentList.h"
#include "DataManager/Containers/UserSettings/Include/UserSettingsInterface.h"
#include "DataManager/Include/DataContainerCollectionBase.h"
#include "DataManager/Containers/SWVersions/Include/SWVersionList.h"
#include "HimalayaDataContainer/Containers/ProgramSettings/Include/ProgramSettings.h"
//lint -sem(DataManager::CDataContainer::DeinitializeContainers, cleanup)

namespace Himalaya {
    class HimalayaMasterThreadController;
}
namespace DataManager {
class CInstrumentHistory;

//!<  List containing macro steps and normal program steps
typedef QList<CProgramStep*> ListOfExpandedSteps_t;

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CDataContainer
 */
/****************************************************************************/
class CDataContainer: public DataManager::CDataContainerCollectionBase
{
    Q_OBJECT
private:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function DeinitializeContainers
     *
     *  \return from DeinitializeContainers
     */
    /****************************************************************************/
    bool DeinitializeContainers();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ResetDCReagentGroupList
     *
     *  \return from ResetDCReagentGroupList
     */
    /****************************************************************************/
    bool ResetDCReagentGroupList();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ResetDCReagentList
     *
     *  \return from ResetDCReagentList
     */
    /****************************************************************************/
    bool ResetDCReagentList();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ResetDCReagentGroupColorList
     *
     *  \return from ResetDCReagentGroupColorList
     */
    /****************************************************************************/
    bool ResetDCReagentGroupColorList();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ResetDCProgramList
     *
     *  \return from ResetDCProgramList
     */
    /****************************************************************************/
    bool ResetDCProgramList();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ResetDCProgramSettings
     *
     *  \return from ResetDCProgramSettings
     */
    /****************************************************************************/
    bool ResetDCProgramSettings();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ResetDCStationList
     *
     *  \return from ResetDCStationList
     */
    /****************************************************************************/
    bool ResetDCStationList();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ResetDCUserSettings
     *
     *  \return from ResetDCUserSettings
     */
    /****************************************************************************/
    bool ResetDCUserSettings(void);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function AddStation
     *
     *  \param StationID = QString type parameter
     *  \param ReagentID =  QString type parameter
     *  \param Station = CDashboardStation type parameter
     *
     *  \return from AddStation
     */
    /****************************************************************************/
    void AddStation(QString StationID, QString ReagentID,CDashboardStation &Station);

public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CDataContainer
     *
     *  \param p_HimalayaMasterThreadController = Threads::MasterThreadController type parameter
     *
     *  \return from CDataContainer
     */
    /****************************************************************************/
    CDataContainer(Threads::MasterThreadController *p_HimalayaMasterThreadController);
    ~CDataContainer();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function InitializeContainers
     *
     *  \return from InitializeContainers
     */
    /****************************************************************************/
    bool InitializeContainers();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetReagentName
     *
     *  \param ReagentID = QString type parameter
     *
     *  \return from GetReagentName
     */
    /****************************************************************************/
    QString GetReagentName(QString ReagentID);

    // prefix mp_ left because members are intentially accessible from outside (public)
    CDataProgramList* ProgramList;  //!< Container for programs
    CDashboardDataStationList* StationList;  //!< Container for stations
    CDataReagentGroupList* ReagentGroupList;  //!< Container for reagent groups
    CDataReagentList* ReagentList;  //!< Container for reagents
    CSWVersionList *SWVersionList;  //!< Container for software version
    CReagentGroupColorList* ReagentGroupColorList; //!<//!< Container for reagentGroupColor
    CProgramSettings* ProgramSettings;         ///<! container for ProgramSettings
    CInstrumentHistory *InstrumentHistory;                //!< Container for Instrument History
    IVerifierInterface* SpecialVerifierGroupA; //!< Special verifier for group A
    IVerifierInterface* SpecialVerifierGroupB; //!< Special verifier for group B
    IVerifierInterface* SpecialVerifierGroupC; //!< Special verifier for group C
    IVerifierInterface* SpecialVerifierGroupD; //!< Special verifier for group D
};

} // namespace DataManager

#endif // DATAMANAGER_OBSOLETE_DATACONTAINER_H

