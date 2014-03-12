/****************************************************************************/
/*! \file DataManager.cpp
 *
 *  \brief Implementation file for class CDataManager.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-02-25
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
#include <Global/Include/SystemPaths.h>
#include <DataManager/Helper/Include/DataManagerEventCodes.h>
#include "HimalayaDataContainer/Helper/Include/HimalayaDataManagerEventCodes.h"

#include "ServiceDataManager/Include/ServiceDataManager.h"

#include "ServiceDataManager/CommandInterface/Include/ModuleCommandInterface.h"

namespace DataManager {

CServiceDataManager::CServiceDataManager(Threads::ServiceMasterThreadController *p_ServiceMasterThreadController) :
    mp_ServiceDataContainer(NULL),
    mp_ModuleCommandInterface(NULL),
    mp_DeviceConfigurationInterface(NULL),
    m_IsInitialized(false),
    mp_MasterThreadController(p_ServiceMasterThreadController)
{
    quint32 ReturnCode  = InitDataContainer();
    if (ReturnCode != 0) {
        qDebug() << "CDataManager::Constructor / InitDataContainer failed";
        Global::EventObject::Instance().RaiseEvent(ReturnCode);
    }
}

CServiceDataManager::CServiceDataManager(Threads::ServiceMasterThreadController *p_ServiceMasterThreadController, QString Path) :
    mp_ServiceDataContainer(NULL),
    mp_ModuleCommandInterface(NULL),
    mp_DeviceConfigurationInterface(NULL),
    m_IsInitialized(false),
    mp_MasterThreadController(p_ServiceMasterThreadController)
{
    quint32 ReturnCode = InitDataContainer();
    if (ReturnCode != 0) {
        Global::EventObject::Instance().RaiseEvent(ReturnCode);
    }
}

CServiceDataManager::~CServiceDataManager()
{
    if (!DeinitDataContainer()) {
        qDebug() << "CDataManager::Destructor / DeinitDataContainer failed";
    }
}

quint32 CServiceDataManager::InitDataContainer()
{
    //quint32 ReturnCode = 0;
    if (m_IsInitialized == true) {
        qDebug() << "CServiceDataManager::InitDataContainer was already called";
        return EVENT_DM_INITIALIZATION_ERROR;
    }

    mp_ServiceDataContainer = new ServiceDataContainer(mp_MasterThreadController);


    QString FilenamePUserSettings = Global::SystemPaths::Instance().GetSettingsPath() + "/UserSettings.xml";
    mp_ServiceDataContainer->SettingsInterface->SetDataVerificationMode(false);
    if (!mp_ServiceDataContainer->SettingsInterface->Read(FilenamePUserSettings)) {
        return EVENT_DM_SETTINGS_XML_READ_FAILED;
    }

    QString FilenameDeviceConfiguration = Global::SystemPaths::Instance().GetSettingsPath() + "/DeviceConfiguration.xml";
    mp_ServiceDataContainer->DeviceConfigurationInterface->SetDataVerificationMode(false);
    if (!mp_ServiceDataContainer->DeviceConfigurationInterface->Read(FilenameDeviceConfiguration)) {
        return EVENT_DM_DEVICE_CONFIG_XML_READ_FAILED;
    }

    mp_ServiceDataContainer->DeviceConfigurationInterface->SetDataVerificationMode(true);

    if (!mp_ServiceDataContainer->DeviceConfigurationInterface->VerifyData()) {
        qDebug() << "CServiceDataManager::InitDataContainer failed, because mp_ServiceDataContainer->DeviceConfigurationInterface->VerifyData failed.";
        return EVENT_DM_DEVICE_CONFIG_VERIFICATION_FAILED;
    }

    mp_ServiceDataContainer->ModuleList->SetDataVerificationMode(false);
    QString FilenameModuleList = Global::SystemPaths::Instance().GetSettingsPath() + "/InstrumentHistory.xml";
    if (!mp_ServiceDataContainer->ModuleList->ReadFile(FilenameModuleList)) {
        qDebug() << "CServiceDataManager::InitDataContainer failed, \
                     because mp_ServiceDataContainer->ModuleList->Read failed with filename: "
                 <<  FilenameModuleList;
    }

    mp_ServiceDataContainer->ModuleList->SetDataVerificationMode(true);
    if (!mp_ServiceDataContainer->ModuleList->VerifyData()) {
        qDebug() << "CServiceDataManager::InitDataContainer failed,\
                because mp_ServiceDataContainer->ModuleList->VerifyData failed.";
    }

    mp_ModuleCommandInterface = new CModuleCommandInterface(this, mp_MasterThreadController, mp_ServiceDataContainer);

    qDebug() << "CServiceDataManager::InitDataContainer successful !!!";
    m_IsInitialized = true;
    return 0;
}

bool CServiceDataManager::DeinitDataContainer()
{
    delete mp_ModuleCommandInterface;
    delete mp_ServiceDataContainer;
    return true;
}

CModuleDataList *CServiceDataManager::GetModuleList(void)
{
    if (!m_IsInitialized)
    {
        qDebug() << "CDataManager::GetModuleList failed. Not initialized!";
        return 0;
    }

    return mp_ServiceDataContainer->ModuleList;
}

CDeviceConfigurationInterface *CServiceDataManager::GetDeviceConfigurationInterface()
{
    if (m_IsInitialized != true) {
        qDebug() << "CDataManager::GetDeviceConfigurationInterface failed. Not initialized!";
        return NULL;
    }
    return mp_ServiceDataContainer->DeviceConfigurationInterface;
}

CModuleCommandInterface* CServiceDataManager::GetModuleDataListInterface()
{
    return mp_ModuleCommandInterface;
}



} // namespace DataManager
