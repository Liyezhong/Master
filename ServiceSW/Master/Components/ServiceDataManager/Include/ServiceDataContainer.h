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
#include "ServiceDataManager/Include/InstrumentHistory.h"
#include "ServiceDataManager/Include/ServiceParameters.h"
#include <ServiceMasterThreadController/Include/ServiceMasterThreadController.h>

namespace DataManager {

/****************************************************************************/
/*!
 *  \brief  This class implements Service Data container
 *  Reads an write service data container.
 */
/****************************************************************************/
class ServiceDataContainer: public QObject
{
    Q_OBJECT
private:
    bool m_IsDeInitialised; //!< The flag of deinitialised.

    /****************************************************************************/
    /*!
     *  \brief Deinitialised data caontainer
     *
     *  \return Success or failure
     */
    /****************************************************************************/
    bool DeinitContainers();

    /****************************************************************************/
    /*!
     *  \brief Reset the User settings data container
     *
     *  \return Success or failure
     */
    /****************************************************************************/
    bool ResetDCUserSettings();

    /****************************************************************************/
    /*!
     *  \brief Reset the Adjustment data container
     *
     *  \return Success or failure
     */
    /****************************************************************************/
    bool ResetDCAdjustment();

protected:
    bool m_IsInitialized;    //!< The flag of initialized

    /****************************************************************************/
    /*!
     *  \brief initialized data caontainer
     *
     *  \return Success or failure
     */
    /****************************************************************************/
    bool InitContainers();

public:
    /****************************************************************************/
    /**
     * \brief Constructor.
     * \param p_ServiceMasterThreadController = the pointer of serivice master thread
     */
    /****************************************************************************/
    ServiceDataContainer(Threads::ServiceMasterThreadController *p_ServiceMasterThreadController);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~ServiceDataContainer();

    Threads::ServiceMasterThreadController *mp_ServiceMasterThreadController; //!< We use it for  broadcasting command

    // prefix mp_ left because members are intentially accessible from outside (public)
    ServiceDataManager::CModuleDataList *ModuleList;                //!< Container for Instrument History
    ServiceDataManager::CInstrumentHistory* ModuleListArchive;      //!< Container for Instrument History Archive
    CDeviceConfigurationInterface* DeviceConfigurationInterface;    //!< Container for Device configuration
    CUserSettingsInterface *SettingsInterface;                      //!< Provides interface to read the Settings info from xml
    CServiceParameters *ServiceParameters;                          //!< Container for service parameters


};

} // namespace DataManager

#endif // DATAMANAGER_SERVICEDATACONTAINER_H

