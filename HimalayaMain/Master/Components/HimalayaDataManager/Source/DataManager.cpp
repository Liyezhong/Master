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

#include <QDebug>
#include <Global/Include/SystemPaths.h>
#include "../Include/DataManager.h"
#include "HimalayaDataManager/CommandInterface/Include/ProgramCommandInterface.h"
#include "HimalayaDataManager/CommandInterface/Include/ReagentCommandInterface.h"
#include "HimalayaDataManager/CommandInterface/Include/StationCommandInterface.h"
#include "HimalayaDataManager/CommandInterface/Include/ReagentGroupCommandInterface.h"
#include "DataManager/CommandInterface/Include/UserSettingsCommandInterface.h"
#include "HimalayaDataManager/Include/DataManagerDefinitions.h"

namespace DataManager {

CDataManager::CDataManager(Threads::MasterThreadController *p_HimalayaMasterThreadController) :CDataManagerBase(p_HimalayaMasterThreadController),
    mp_DataContainer(NULL),
    mp_ReagentCommandInterface(NULL),
    mp_StationCommandInterface(NULL),
    mp_ProgramCommandInterface(NULL),
    mp_ReagentGroupCommandInterface(NULL)
{
    quint32 ReturnCode  = InitDataContainer();
    if (ReturnCode != INIT_OK) {
        qDebug() << "CDataManager::Constructor / InitDataContainer failed";
        Global::EventObject::Instance().RaiseEvent(ReturnCode);
    }
}

CDataManager::CDataManager(Threads::MasterThreadController *p_HimalayaMasterThreadController, QString Path) :CDataManagerBase(p_HimalayaMasterThreadController),
    mp_DataContainer(NULL),
    mp_ProgramCommandInterface(NULL),
    mp_ReagentCommandInterface(NULL),
    mp_StationCommandInterface(NULL),
    mp_ReagentGroupCommandInterface(NULL)
{
    quint32 ReturnCode = InitDataContainer();
    if (ReturnCode != INIT_OK) {
        Global::EventObject::Instance().RaiseEvent(ReturnCode);
    }
}

CDataManager::~CDataManager()
{
    if (!DeinitDataContainer()) {
        qDebug() << "CDataManager::Destructor / DeinitDataContainer failed";
    }
}

quint32 CDataManager::InitDataContainer()
{
    quint32 ReturnCode = INIT_OK;
    //Create DataContainer - Inturn creates individual containers
    mp_DataContainer = new CDataContainer(mp_MasterThreadController);
    mp_DataContainerCollection = mp_DataContainer;
    ReturnCode = CDataManagerBase::InitDataContainer();
    if ( ReturnCode != INIT_OK) {
        return ReturnCode;
    }

    mp_DataContainer->ReagentGroupList->SetDataVerificationMode(false);
    QString FilenameReagentGroupList = Global::SystemPaths::Instance().GetSettingsPath() + "/" + REAGENT_GROUPS_XML;
    if (!mp_DataContainer->ReagentGroupList->Read(FilenameReagentGroupList)) {
        qDebug() << "CDataManager::InitDataContainer failed, because mp_DataContainer->ReagentGroupList->Read failed with filename: " << FilenameReagentGroupList;
        return EVENT_DM_PROGRAM_XML_READ_FAILED;
    }

    mp_DataContainer->ReagentGroupColorList->SetDataVerificationMode(false);
    QString FilenameReagentGroupColorList = Global::SystemPaths::Instance().GetSettingsPath() + "/" + REAGENT_GROUPS_COLOR_XML;
    if (!mp_DataContainer->ReagentGroupColorList->Read(FilenameReagentGroupColorList)) {
        qDebug() << "CDataManager::InitDataContainer failed, because mp_DataContainer->ReagentGroupColorList->Read failed with filename: " << FilenameReagentGroupColorList;
        return EVENT_DM_PROGRAM_XML_READ_FAILED;
    }

    mp_DataContainer->ReagentList->SetDataVerificationMode(false);
    QString FilenameReagentList = Global::SystemPaths::Instance().GetSettingsPath() + "/" + REAGENTS_XML;
    if (!mp_DataContainer->ReagentList->Read(FilenameReagentList)) {
        qDebug() << "CDataManager::InitDataContainer failed, because mp_DataContainer->ReagentList->Read failed with filename: " << FilenameReagentList;
        return EVENT_DM_REAGENT_XML_READ_FAILED;
    }

    mp_DataContainer->ProgramList->SetDataVerificationMode(false);
    QString FilenameProgramList = Global::SystemPaths::Instance().GetSettingsPath() + "/" + PROGRAMS_XML;
    if (!mp_DataContainer->ProgramList->Read(FilenameProgramList)) {
        qDebug() << "CDataManager::InitDataContainer failed, because mp_DataContainer->ProgramList->Read failed with filename: " << FilenameProgramList;
        return EVENT_DM_PROGRAM_XML_READ_FAILED;
    }



    mp_DataContainer->StationList->SetDataVerificationMode(false);

    QString FilenameStationList = Global::SystemPaths::Instance().GetSettingsPath() + "/" + STATIONS_XML;
    if (!mp_DataContainer->StationList->Read(FilenameStationList)) {
        qDebug() << "CDataManager::InitDataContainer failed, \
                because mp_DataContainer->StationtList->Read failed with filename: "
                << FilenameStationList;
        return EVENT_DM_STATIONS_XML_READ_FAILED;
    }

    // when all containers are loaded, activate verification mode and verify there initial content
    mp_DataContainer->ProgramList->SetDataVerificationMode(true);
    mp_DataContainer->ReagentList->SetDataVerificationMode(true);
    mp_DataContainer->StationList->SetDataVerificationMode(true);

    if (!mp_DataContainer->ProgramList->VerifyData()) {
        qDebug() << "CDataManager::InitDataContainer failed, because mp_DataContainer->ProgramList->VerifyData failed.";
        return EVENT_DM_PROGRAM_VERIFICATION_FAILED;
    }
    if (!mp_DataContainer->ReagentList->VerifyData()) {
        qDebug() << "CDataManager::InitDataContainer failed, because mp_DataContainer->ReagentList->VerifyData failed.";
        return EVENT_DM_REAGENT_VERIFICATION_FAILED;
    }
    if (!mp_DataContainer->StationList->VerifyData()) {
        qDebug() << "CDataManager::InitDataContainer failed,\
                because mp_DataContainer->StationList->VerifyData failed.";
        return EVENT_DM_STATION_VERIFICATION_FAILED;
    }

    // do special verification
    if (!mp_DataContainer->StationList->VerifyData(true)) {
        Global::EventObject::Instance().RaiseEvent(EVENT_HIMALAYA_DM_GV_FAILED);
    }


    if (!mp_DataContainer->ProgramList->VerifyData(true)) {
        Global::EventObject::Instance().RaiseEvent(EVENT_HIMALAYA_DM_GV_FAILED);
    }

    if (!mp_DataContainer->SettingsInterface->VerifyData(true)) {
        Global::EventObject::Instance().RaiseEvent(EVENT_HIMALAYA_DM_GV_FAILED);
    }

    mp_ProgramCommandInterface = new CProgramCommandInterface(this, mp_MasterThreadController, mp_DataContainer);
    mp_StationCommandInterface = new CStationCommandInterface(this, mp_MasterThreadController, mp_DataContainer);
    mp_ReagentCommandInterface = new CReagentCommandInterface(this, mp_MasterThreadController, mp_DataContainer);
    mp_ReagentGroupCommandInterface = new CReagentGroupCommandInterface(this, mp_MasterThreadController, mp_DataContainer);
    m_IsInitialized = true;
    return INIT_OK;
}

bool CDataManager::DeinitDataContainer()
{
    delete mp_ProgramCommandInterface;
    delete mp_StationCommandInterface;
    delete mp_ReagentCommandInterface;
    delete mp_ReagentGroupCommandInterface;
    delete mp_DataContainer;
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

CDeviceConfigurationInterface *CDataManager::GetDeviceConfigurationInterface()
{
    if (m_IsInitialized != true) {
        qDebug() << "CDataManager::GetDeviceConfigurationInterface failed. Not initialized!";
        return NULL;
    }
    return mp_DataContainer->DeviceConfigurationInterface;
}

} // namespace DataManager
