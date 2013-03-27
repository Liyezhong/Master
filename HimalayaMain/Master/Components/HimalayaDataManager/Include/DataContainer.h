/****************************************************************************/
/*! \file ObsoleteDataContainer.h
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

#include <QString>
#include <QIODevice>

#include "HimalayaDataContainer/Containers/ReagentGroups/Include/DataReagentGroupList.h"
#include "HimalayaDataContainer/Containers/ReagentGroupColor/Include/ReagentGroupColorList.h"
#include "HimalayaDataContainer/Containers/Reagents/Include/DataReagentList.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Include/DashboardDataStationList.h"
#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramList.h"
#include "HimalayaDataContainer/Containers/Reagents/Include/DataReagentList.h"
#include "DataManager/Containers/UserSettings/Include/UserSettingsInterface.h"
#include "DataManager/Containers/DeviceConfiguration/Include/DeviceConfigurationInterface.h"
#include "DataManager/Include/DataContainerCollectionBase.h"


namespace Himalaya {
    class HimalayaMasterThreadController;
}
namespace DataManager {
//!<  List containing macro steps and normal program steps
typedef QList<CProgramStep*> ListOfExpandedSteps_t;

class CDataContainer: public DataManager::CDataContainerCollectionBase
{
    Q_OBJECT
private:
    bool DeinitContainers();
    bool ResetDCReagentGroupList();
    bool ResetDCReagentList();
    bool ResetDCReagentGroupColorList();
    bool ResetDCProgramList();
    bool ResetDCStationList();

    void AddStation(QString StationID, QString ReagentID,CDashboardStation &Station);

public:
    CDataContainer(Threads::MasterThreadController *p_HimalayaMasterThreadController);
    ~CDataContainer();
    bool InitContainers();
    QString GetReagentName(QString ReagentID);

    // prefix mp_ left because members are intentially accessible from outside (public)
    CDataProgramList* ProgramList;  //!< Container for programs
    CDashboardDataStationList* StationList;  //!< Container for stations
    CDataReagentGroupList* ReagentGroupList;  //!< Container for reagent groups
    CDataReagentList* ReagentList;  //!< Container for reagents
    CReagentGroupColorList* ReagentGroupColorList; //!<//!< Container for reagentGroupColor
    IVerifierInterface* SpecialVerifierGroupA; //!< Special verifier for group A
    IVerifierInterface* SpecialVerifierGroupB; //!< Special verifier for group B
    IVerifierInterface* SpecialVerifierGroupC; //!< Special verifier for group C
    IVerifierInterface* SpecialVerifierGroupD; //!< Special verifier for group D
};

} // namespace DataManager

#endif // DATAMANAGER_OBSOLETE_DATACONTAINER_H

