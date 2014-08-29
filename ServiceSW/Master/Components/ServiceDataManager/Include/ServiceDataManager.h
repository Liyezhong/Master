/****************************************************************************/
/*! \file ServiceDataManager/Include/ServiceDataManager.h
 *
 *  \brief Definition file for class CServiceDataManager.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-22-02
 *  $Author:    $ Srivathsa HH
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems R&D Center Shanghai. All rights reserved.
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

/****************************************************************************/
/**
 * \brief CServiceDataManager definition
 */
/****************************************************************************/
class CServiceDataManager: public QObject
{
    Q_OBJECT

public:
    /****************************************************************************/
    /**
     * \brief Constructor.
     * \param p_ServiceMasterThreadController = the pointer of serivice master thread
     */
    /****************************************************************************/
    CServiceDataManager(Threads::ServiceMasterThreadController *p_ServiceMasterThreadController);

    /****************************************************************************/
    /**
     * \brief Constructor.
     * \param p_ServiceMasterThreadController = the pointer of serivice master thread
     * \param Path = unused
     */
    /****************************************************************************/
    CServiceDataManager(Threads::ServiceMasterThreadController *p_ServiceMasterThreadController, QString Path);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    virtual ~CServiceDataManager();

    /****************************************************************************/
    /*!
     *  \brief Gets the device configuration container pointer
     *  \return device configuration container pointer
     */
    /****************************************************************************/
    CDeviceConfigurationInterface* GetDeviceConfigurationInterface();

    /****************************************************************************/
    /*!
     *  \brief Gets the Module command container pointer
     *  \return Module command configuration container pointer
     */
    /****************************************************************************/
    CModuleCommandInterface* GetModuleDataListInterface();

    /****************************************************************************/
    /*!
     *  \brief Gets the Service data container pointer
     *  \return Service data configuration container pointer
     */
    /****************************************************************************/
    const ServiceDataContainer* GetDataContainer() { return mp_ServiceDataContainer; }

    /****************************************************************************/
    /*!
     *  \brief Initialization of service data container
     *  \return event ID (Success or Failure)
     */
    /****************************************************************************/
    quint32 InitDataContainer();

    /****************************************************************************/
    /*!
     *  \brief Checks data container is initialization
     *  \return Success or Failure
     */
    /****************************************************************************/
    bool IsInitialized() const { return mp_ServiceDataContainer; }

    /****************************************************************************/
    /*!
     *  \brief Gets the Module data list container pointer
     *  \return Module data list configuration container pointer
     */
    /****************************************************************************/
    ServiceDataManager::CModuleDataList *GetModuleList(void);

private:
    ServiceDataContainer* mp_ServiceDataContainer;

    CModuleCommandInterface *mp_ModuleCommandInterface;             //!< Handles commands related to Instrument History Container
    CDeviceConfigurationInterface *mp_DeviceConfigurationInterface; //!< Handles commands related to device configuration

    bool m_UpdateList;
    bool m_IsInitialized;                                  //!< Flag variable is set when container is initialized

    Threads::ServiceMasterThreadController *mp_MasterThreadController; //!< This is passed to DataContainer

    /****************************************************************************/
    /*!
     *  \brief De initialise the data containers
     *  \return Success or failure
     */
    /****************************************************************************/
    bool DeinitDataContainer();

};
}// namespace DataManager

#endif // DATAMANAGER_SERVICEDATAMANAGER_H


