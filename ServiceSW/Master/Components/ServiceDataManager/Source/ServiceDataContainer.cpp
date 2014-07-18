/****************************************************************************/
/*! \file ServiceDataContainer.cpp
 *
 *  \brief Implementation file for class ServiceDataContainer.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-02-25
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

#include <QDebug>
#include  "ServiceDataManager/Include/ServiceDataContainer.h"
#include "DataManager/Containers/DeviceConfiguration/Include/DeviceConfigurationVerifier.h"
#include "ServiceDataManager/Include/ModuleDataListVerifier.h"
#include "DataManager/Containers/UserSettings/Include/UserSettingsVerifier.h"

namespace DataManager {

ServiceDataContainer::ServiceDataContainer(Threads::ServiceMasterThreadController *p_MasterThreadController) :
    m_IsDeInitialised(false),
    m_IsInitialized(false),
    mp_ServiceMasterThreadController(p_MasterThreadController),
    DeviceConfigurationInterface(NULL),
    SettingsInterface(NULL),
    ModuleList(NULL),
    ServiceParameters(NULL)
{
    if (!InitContainers()) {
        qDebug() << "ServiceDataContainer::ServiceDataContainer / InitContainers failed";
    }
}


ServiceDataContainer::~ServiceDataContainer()
{
    if (!DeinitContainers()) {
        qDebug() << "ServiceDataContainer::ServiceDataContainer / DeinitContainers failed";
    }
}

bool ServiceDataContainer::InitContainers()
{
    if (m_IsInitialized == true) {
        qDebug() << "ServiceDataContainer::InitContainers() was already called";
        Q_ASSERT(false);
        return false;
    }

    SettingsInterface = new CUserSettingsInterface();
    if (!ResetDCUserSettings()) {
        qDebug() << "ServiceDataContainer::InitContainers failed, because ResetDCSettings failed!";
        return false;
    }
    DeviceConfigurationInterface = new CDeviceConfigurationInterface();
    ModuleList = new ServiceDataManager::CModuleDataList();
    ModuleListArchive = new ServiceDataManager::CInstrumentHistory();
    ServiceParameters = new CServiceParameters();
    m_IsInitialized = true;
    SettingsInterface->SetDataVerificationMode(false);
    DeviceConfigurationInterface->SetDataVerificationMode(false);
    return true;
}

bool ServiceDataContainer::DeinitContainers()
{
    delete DeviceConfigurationInterface;
    delete SettingsInterface;
    delete ServiceParameters;
    delete ModuleList;
    delete ModuleListArchive;
    m_IsDeInitialised = true;
    return true;
}

bool ServiceDataContainer::ResetDCUserSettings()
{
    if (m_IsInitialized == true) {
        qDebug() << "ServiceDataContainer::ResetDCUserSettings was already called";
        return false;
    }

    // create a verifier object for this data container, if not done before
    static IVerifierInterface *p_DataUserSettingVerifier = NULL;
    if (p_DataUserSettingVerifier == NULL) {
        p_DataUserSettingVerifier = new CUserSettingsVerifier();
    }
    // register this verifier object in the data container (=> dependency injection)
    SettingsInterface->AddVerifier(p_DataUserSettingVerifier);

    return true;
}

/****************************************************************************/
/*!
 *  \brief Reset the Adjustment data container
 *
 *  \return Success or failure
 */
/****************************************************************************/
bool ServiceDataContainer::ResetDCAdjustment()
{
    if (m_IsInitialized == true) {
        qDebug() << "ServiceDataContainer::ResetDCAdjustment was already called";
        return false;
    }
    return true;
}


}// namespace DataManager
