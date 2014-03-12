/****************************************************************************/
/*! \file DataManager.h
 *
 *  \brief Definition file for class CDataManager.
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

#ifndef DATAMANAGER_SERVICEDATAMANAGER_H
#define DATAMANAGER_SERVICEDATAMANAGER_H

#include <QObject>
#include <QString>
#include <QIODevice>
#include "ServiceDataManager/Include/ServiceDataContainer.h"

namespace DataManager {

class CModuleCommandInterface;

class CServiceDataManager: public QObject
{
    Q_OBJECT

public:
    CServiceDataManager(Threads::ServiceMasterThreadController *p_ServiceMasterThreadController);
    CServiceDataManager(Threads::ServiceMasterThreadController *p_ServiceMasterThreadController, QString Path);
    virtual ~CServiceDataManager();

    CDeviceConfigurationInterface* GetDeviceConfigurationInterface();
    CModuleCommandInterface* GetModuleDataListInterface();
    const ServiceDataContainer* GetDataContainer() { return mp_ServiceDataContainer; }

    quint32 InitDataContainer();
    bool IsInitialized() const { return mp_ServiceDataContainer; }

    CModuleDataList *GetModuleList(void);

private:
    ServiceDataContainer* mp_ServiceDataContainer;

    CModuleCommandInterface *mp_ModuleCommandInterface;             //!< Handles commands related to Instrument History Container
    CDeviceConfigurationInterface *mp_DeviceConfigurationInterface; //!< Handles commands related to device configuration

    bool m_UpdateList;
    bool m_IsInitialized;

    Threads::ServiceMasterThreadController *mp_MasterThreadController; //!< This is passed to DataContainer

    virtual bool DeinitDataContainer();
};
}// namespace DataManager

#endif // DATAMANAGER_SERVICEDATAMANAGER_H


