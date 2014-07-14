/****************************************************************************/
/*! \file ServiceDataContainer.h
 *
 *  \brief Definition file for class CServiceDataContainer.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-22-02
 *  $Author:    $ Srivathsa HH
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

#ifndef DATAMANAGER_SERVICEDATACONTAINER_H
#define DATAMANAGER_SERVICEDATACONTAINER_H

#include <QString>
#include <QIODevice>

#include "DataManager/Containers/DeviceConfiguration/Include/DeviceConfigurationInterface.h"
#include "DataManager/Containers/UserSettings/Include/UserSettingsInterface.h"
#include "ServiceDataManager/Include/ModuleDataList.h"
#include "ServiceDataManager/Include/ServiceParameters.h"
#include <ServiceMasterThreadController/Include/ServiceMasterThreadController.h>

namespace DataManager {

class ServiceDataContainer: public QObject
{
    Q_OBJECT
private:
    bool m_IsDeInitialised;

    bool DeinitContainers();
    bool ResetDCUserSettings();
    bool ResetDCAdjustment();

protected:
    bool m_IsInitialized;

    bool InitContainers();

public:
    ServiceDataContainer(Threads::ServiceMasterThreadController *p_ServiceMasterThreadController);
    ~ServiceDataContainer();

    Threads::ServiceMasterThreadController *mp_ServiceMasterThreadController; //!< We use it for  broadcasting command

    // prefix mp_ left because members are intentially accessible from outside (public)
    ServiceDataManager::CModuleDataList *ModuleList;                //!< Container for Instrument History
    CDeviceConfigurationInterface* DeviceConfigurationInterface;    //!< Container for Device configuration
    CUserSettingsInterface *SettingsInterface;                      //!< Provides interface to read the Settings info from xml
    CServiceParameters *ServiceParameters;                          //!< Container for service parameters


};

} // namespace DataManager

#endif // DATAMANAGER_SERVICEDATACONTAINER_H

