/****************************************************************************/
/*! \file DataManager.cpp
 *
 *  \brief Implementation file for class CDataManager.
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

#include <Global/Include/SystemPaths.h>
#include "../Include/DataManager.h"
#include "HimalayaDataManager/CommandInterface/Include/ModuleCommandInterface.h"
#include "HimalayaDataManager/CommandInterface/Include/ProgramCommandInterface.h"
#include "HimalayaDataManager/CommandInterface/Include/ReagentCommandInterface.h"
#include "HimalayaDataManager/CommandInterface/Include/StationCommandInterface.h"
#include "HimalayaDataManager/CommandInterface/Include/ReagentGroupCommandInterface.h"
#include "DataManager/CommandInterface/Include/UserSettingsCommandInterface.h"
#include "HimalayaDataManager/Include/DataManagerDefinitions.h"
#include "DataManager/Helper/Include/DataManagerEventCodes.h"
#include "DataManager/Containers/InstrumentHistory/Include/InstrumentHistory.h"
#include "DeviceControl/Include/DeviceProcessing/DeviceLifeCycleRecord.h"

namespace DataManager {

CDataManager::CDataManager(Threads::MasterThreadController *p_HimalayaMasterThreadController)
    :CDataManagerBase(p_HimalayaMasterThreadController),
    m_UpdateList(false),
    mp_DataContainer(NULL),
    mp_StationCommandInterface(NULL),
    mp_ReagentCommandInterface(NULL),
    mp_ReagentGroupCommandInterface(NULL),
    mp_ModuleCommandInterface(NULL),
    mp_ProgramCommandInterface(NULL)
{
    quint32 ReturnCode  = InitializeDataContainer();
    if (ReturnCode != INIT_OK) {
        qDebug() << "CDataManager::Constructor / InitializeDataContainer failed";
        Global::EventObject::Instance().RaiseEvent(ReturnCode);
    }
}

CDataManager::CDataManager(Threads::MasterThreadController *p_HimalayaMasterThreadController, QString Path) :CDataManagerBase(p_HimalayaMasterThreadController),
    mp_DataContainer(NULL),
    mp_StationCommandInterface(NULL),
    mp_ReagentCommandInterface(NULL),
    mp_ReagentGroupCommandInterface(NULL),
    mp_ModuleCommandInterface(NULL),
    mp_ProgramCommandInterface(NULL)
{
    Q_UNUSED(Path)
    quint32 ReturnCode = InitializeDataContainer();
    if (ReturnCode != INIT_OK) {
        Global::EventObject::Instance().RaiseEvent(ReturnCode);
    }
}

CDataManager::~CDataManager()
{
    try {
        (void)(DeinitializeDataContainer());
    }
    catch(...){
    }
}

quint32 CDataManager::InitializeDataContainer()
{
    quint32 ReturnCode = INIT_OK;
    //Create DataContainer - Inturn creates individual containers
    mp_DataContainer = new CDataContainer(mp_MasterThreadController);
    mp_DataContainerCollectionBase = mp_DataContainer;

    //Update activeCarbonFilter LifeTime according to DeviceLifeCycleRecord.xml
    DeviceControl::DeviceLifeCycleRecord deviceLifeCycleRecord;
    deviceLifeCycleRecord.ReadRecord();

    int activeCarbonFilterLifeTime = -1;
    uint carbonFilter_FirstRecord_Flag = 0;
    
    DeviceControl::ModuleLifeCycleRecord* pModuleLifeCycleRecord = deviceLifeCycleRecord.m_ModuleLifeCycleMap.value("LA");
    if (pModuleLifeCycleRecord)
    {
        DeviceControl::PartLifeCycleRecord* pPartLifeCycleRecord = pModuleLifeCycleRecord->m_PartLifeCycleMap.value("AL_pressure_ctrl");
        if (pPartLifeCycleRecord)
        {
            activeCarbonFilterLifeTime = pPartLifeCycleRecord->m_ParamMap.value("ActiveCarbonFilter_LifeTime").toInt();
            carbonFilter_FirstRecord_Flag = pPartLifeCycleRecord->m_ParamMap.value("CarbonFilter_FirstRecord_Flag").toUInt();
        }
    }

    ReturnCode = CDataManagerBase::InitDataContainer();
    if ( ReturnCode != INIT_OK) {
        return ReturnCode;
    }

    if ((0 == activeCarbonFilterLifeTime) && (1 == carbonFilter_FirstRecord_Flag))
    {
        CHimalayaUserSettings tempSettings(*mp_DataContainer->SettingsInterface->GetUserSettings());
        QString strDate = Global::AdjustedTime::Instance().GetCurrentDateTime().toString();
        tempSettings.SetActiveCarbonLastResetDate(strDate);
        (void)mp_DataContainer->SettingsInterface->UpdateUserSettings(&tempSettings);
        mp_DataContainer->SettingsInterface->Write();
    }

    mp_DataContainer->ReagentGroupList->SetDataVerificationMode(false);
    QString FilenameReagentGroupList = Global::SystemPaths::Instance().GetSettingsPath() + "/" + REAGENT_GROUPS_XML;
    if (!mp_DataContainer->ReagentGroupList->Read(FilenameReagentGroupList)) {
        qDebug() << "CDataManager::InitializeDataContainer failed, because mp_DataContainer->ReagentGroupList->Read failed with filename: " << FilenameReagentGroupList;
        return EVENT_DM_PROGRAM_XML_READ_FAILED;
    }

    mp_DataContainer->ReagentGroupColorList->SetDataVerificationMode(false);
    QString FilenameReagentGroupColorList = Global::SystemPaths::Instance().GetSettingsPath() + "/" + REAGENT_GROUPS_COLOR_XML;
    if (!mp_DataContainer->ReagentGroupColorList->Read(FilenameReagentGroupColorList)) {
        qDebug() << "CDataManager::InitializeDataContainer failed, because mp_DataContainer->ReagentGroupColorList->Read failed with filename: " << FilenameReagentGroupColorList;
        return EVENT_DM_PROGRAM_XML_READ_FAILED;
    }

    mp_DataContainer->ReagentList->SetDataVerificationMode(false);
    QString FilenameReagentList = Global::SystemPaths::Instance().GetSettingsPath() + "/" + REAGENTS_XML;
    if (!mp_DataContainer->ReagentList->Read(FilenameReagentList)) {
        qDebug() << "CDataManager::InitializeDataContainer failed, because mp_DataContainer->ReagentList->Read failed with filename: " << FilenameReagentList;
        return EVENT_DM_REAGENT_XML_READ_FAILED;
    }

    mp_DataContainer->ProgramList->SetDataVerificationMode(false);
    QString FilenameProgramList = Global::SystemPaths::Instance().GetSettingsPath() + "/" + PROGRAMS_XML;
    if (!mp_DataContainer->ProgramList->Read(FilenameProgramList)) {
        qDebug() << "CDataManager::InitializeDataContainer failed, because mp_DataContainer->ProgramList->Read failed with filename: " << FilenameProgramList;
        return EVENT_DM_PROGRAM_XML_READ_FAILED;
    }

    mp_DataContainer->ProgramSettings->SetDataVerificationMode(false);
    QString FilenameProgramSettings = Global::SystemPaths::Instance().GetSettingsPath() + "/" + PROGRAM_SETTINGS_XML;
    if (!mp_DataContainer->ProgramSettings->Read(FilenameProgramSettings)) {
         qDebug() << "CDataManager::InitializeDataContainer failed, because mp_DataContainer->ProgramSettings->Read failed with filename: " << FilenameProgramSettings;
         return EVENT_DM_PROGRAM_XML_READ_FAILED; // TBD need to be changed
    }

    mp_DataContainer->StationList->SetDataVerificationMode(false);

    QString FilenameStationList = Global::SystemPaths::Instance().GetSettingsPath() + "/" + STATIONS_XML;
    if (!mp_DataContainer->StationList->Read(FilenameStationList)) {
        qDebug() << "CDataManager::InitializeDataContainer failed, \
                because mp_DataContainer->StationtList->Read failed with filename: "
                << FilenameStationList;
        return EVENT_DM_STATIONS_XML_READ_FAILED;
    }

    mp_DataContainer->InstrumentHistory->SetDataVerificationMode(false);
    QString FilenameModuleList = Global::SystemPaths::Instance().GetSettingsPath() + "/" + INSTRUMENT_HISTORY_XML;
    if (!mp_DataContainer->InstrumentHistory->ReadFile(FilenameModuleList)) {
        return EVENT_DM_MODULE_CONF_READ_FAILED;
    }

    mp_DataContainer->SWVersionList->SetDataVerificationMode(false);
    (void)mp_DataContainer->SWVersionList->Read( Global::SystemPaths::Instance().GetSettingsPath() + "/" + "SW_Version.xml");

    // when all containers are loaded, activate verification mode and verify there initial content
    mp_DataContainer->ProgramList->SetDataVerificationMode(true);
    mp_DataContainer->ReagentList->SetDataVerificationMode(true);
    mp_DataContainer->StationList->SetDataVerificationMode(true);

    if (!mp_DataContainer->ProgramList->VerifyData()) {
        qDebug() << "CDataManager::InitializeDataContainer failed, because mp_DataContainer->ProgramList->VerifyData failed.";
        return EVENT_DM_PROGRAM_VERIFICATION_FAILED;
    }
    if (!mp_DataContainer->ReagentList->VerifyData()) {
        qDebug() << "CDataManager::InitializeDataContainer failed, because mp_DataContainer->ReagentList->VerifyData failed.";
        return EVENT_DM_REAGENT_VERIFICATION_FAILED;
    }
    if (!mp_DataContainer->StationList->VerifyData()) {
        qDebug() << "CDataManager::InitializeDataContainer failed,\
                because mp_DataContainer->StationList->VerifyData failed.";
        return EVENT_DM_STATION_VERIFICATION_FAILED;
    }

    // do special verification
    if (!mp_DataContainer->StationList->VerifyData(true)) {
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_GV_FAILED);
    }


    if (!mp_DataContainer->ProgramList->VerifyData(true)) {
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_GV_FAILED);
    }

    if (!mp_DataContainer->SettingsInterface->VerifyData(true)) {
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_GV_FAILED);
    }

    mp_ModuleCommandInterface = new CModuleCommandInterface(this, mp_MasterThreadController, mp_DataContainer);
    mp_ProgramCommandInterface = new CProgramCommandInterface(this, mp_MasterThreadController, mp_DataContainer);
    mp_StationCommandInterface = new CStationCommandInterface(this, mp_MasterThreadController, mp_DataContainer);
    mp_ReagentCommandInterface = new CReagentCommandInterface(this, mp_MasterThreadController, mp_DataContainer);
    mp_ReagentGroupCommandInterface = new CReagentGroupCommandInterface(this, mp_MasterThreadController, mp_DataContainer);
    m_IsInitialized = true;
    return INIT_OK;
}

bool CDataManager::DeinitializeDataContainer()
{
    delete mp_ProgramCommandInterface;
    delete mp_StationCommandInterface;
    delete mp_ReagentCommandInterface;
    delete mp_ReagentGroupCommandInterface;
    delete mp_DataContainer;
    delete mp_ModuleCommandInterface;
    return true;
}

//get copy of complete station list
CDataReagentList* CDataManager::GetReagentList()
{
    if (m_IsInitialized != true) {
        qDebug() << "CDataManager::GetStationList failed. Not initialized!";
        return NULL;
    }

    return mp_DataContainer->ReagentList;
}

CSWVersionList* CDataManager::GetSWVersion()
{
    if (m_IsInitialized != true) {
        qDebug() << "CDataManager::GetSWVersion failed. Not initialized!";
        return NULL;
    }

    return mp_DataContainer->SWVersionList;
}

//********************************************************************
// some function to access the station list
//********************************************************************

//get copy of complete station list
CDashboardDataStationList* CDataManager::GetStationList()
{
    if (m_IsInitialized != true) {
        qDebug() << "CDataManager::GetStationList failed. Not initialized!";
        return NULL;
    }

    return mp_DataContainer->StationList;
}

CDataReagentGroupList* CDataManager::GetReagentGroupList() {
    if (m_IsInitialized != true) {
        qDebug() << "CDataManager::GetReagentGroupList failed. Not initialized!";
        return NULL;
    }
    return mp_DataContainer->ReagentGroupList;
}

CReagentGroupColorList* CDataManager::GetReagentGroupColorList()
{
    if (m_IsInitialized != true) {
        qDebug() << "CDataManager::GetReagentGroupColorList failed. Not initialized!";
        return NULL;
    }
    return mp_DataContainer->ReagentGroupColorList;
}

bool CDataManager::GetReagentGroupList(CDataReagentGroupList *p_ReagentGroupList)
{
    if (m_IsInitialized != true) {
        qDebug() << "CDataManager::GetReagentList failed. Not initialized!";
        return false;
    }
    if (p_ReagentGroupList) {
        *p_ReagentGroupList = *(mp_DataContainer->ReagentGroupList);
        return true;
    }
    return false;
}

CDataProgramList* CDataManager::GetProgramList() {
    if (m_IsInitialized != true) {
        qDebug() << "CDataManager::GetReagentList failed. Not initialized!";
        return NULL;
    }
    return mp_DataContainer->ProgramList;
}


bool CDataManager::GetProgramList(CDataProgramList *p_ProgramList)
{
    if (m_IsInitialized != true) {
        qDebug() << "CDataManager::GetReagentList failed. Not initialized!";
        return false;
    }
    if (p_ProgramList) {
        *p_ProgramList = *(mp_DataContainer->ProgramList);
        return true;
    }
    return false;
}

CProgramSettings* CDataManager::GetProgramSettings() {
    if (m_IsInitialized != true) {
        qDebug() << "CDataManager::GetProgramSettings failed. Not initialized!";
        return NULL;
    }
    return mp_DataContainer->ProgramSettings;
}


bool CDataManager::GetProgramSettings(CProgramSettings *p_ProgramSettings)
{
    if (m_IsInitialized != true) {
        qDebug() << "CDataManager::GetProgramSettings failed. Not initialized!";
        return false;
    }
    if (p_ProgramSettings) {
        *p_ProgramSettings = *(mp_DataContainer->ProgramSettings);
        return true;
    }
    return false;
}


bool CDataManager::GetReagentList(CDataReagentList *p_ReagentList)
{
    if (m_IsInitialized != true) {
        qDebug() << "CDataManager::GetReagentList failed. Not initialized!";
        return false;
    }
    if (p_ReagentList) {
        *p_ReagentList = *(mp_DataContainer->ReagentList);
        return true;
    }
    return false;
}

bool CDataManager::GetStationList(CDashboardDataStationList *p_StationList)
{
    if (m_IsInitialized != true) {
        qDebug() << "CDataManager::GetStationList failed. Not initialized!";
        return false;
    }
    if (p_StationList) {
        *p_StationList = *(mp_DataContainer->StationList);
        return true;
    }
    return false;
}


CUserSettingsInterface *CDataManager::GetUserSettingsInterface()
{
    if (m_IsInitialized != true) {
        qDebug() << "CDataManager::GetUserSettingsInterface failed. Not initialized!";
        return NULL;
    }
    return mp_DataContainer->SettingsInterface;
}

CHimalayaUserSettings* CDataManager::GetUserSettings()
{
    if (m_IsInitialized != true) {
        qDebug() << "CDataManager::GetUserSettingsInterface failed. Not initialized!";
        return NULL;
    }
    return mp_DataContainer->SettingsInterface->GetUserSettings();
}

CDeviceConfigurationInterface *CDataManager::GetDeviceConfigurationInterface()
{
    if (m_IsInitialized != true) {
        qDebug() << "CDataManager::GetDeviceConfigurationInterface failed. Not initialized!";
        return NULL;
    }
    return mp_DataContainer->DeviceConfigurationInterface;
}
/****************************************************************************/
/*!
 *  \brief saves all the files modification before shutting down
 */
/****************************************************************************/
void CDataManager::SaveDataOnShutdown()
{
    if(!m_IsInitialized) {
        return;
    }

}
} // namespace DataManager
