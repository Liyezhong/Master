/****************************************************************************/
/*! \file ObsoleteDataContainer.cpp
 *
 *  \brief Implementation file for class CDataContainer.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2011-07-20
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
#include  "../Include/DataContainer.h"

#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramListVerifier.h"

#include "HimalayaDataContainer/Containers/Reagents/Include/DataReagentListVerifier.h"

#include "HimalayaDataContainer/Containers/ReagentStations/Include/DataStationListVerifier.h"


#include "DataManager/Containers/DeviceConfiguration/Include/DeviceConfigurationVerifier.h"

//#include "DataManager/Containers/Adjustment/Include/AdjustmentVerifier.h"

#include "DataManager/Containers/Racks/Include/RackList.h"
#include "DataManager/Containers/Racks/Include/RackListVerifier.h"
#include "DataManager/Containers/UserSettings/Include/UserSettingsVerifier.h"
#include "HimalayaDataContainer/SpecialVerifiers/Include/SpecialVerifierGroupA.h"
#include "HimalayaDataContainer/SpecialVerifiers/Include/SpecialVerifierGroupC.h"
#include "HimalayaDataContainer/SpecialVerifiers/Include/SpecialVerifierGroupD.h"
#include "HimalayaDataContainer/Containers/UserSettings/Include/HimalayaSettingsVerifier.h"


namespace DataManager {

CDataContainer::CDataContainer(Threads::MasterThreadController *p_MasterThreadController) :
    DataManager::CDataContainerCollectionBase(p_MasterThreadController),
    ProgramList(NULL),
    StationList(NULL),
    ReagentGroupList(NULL),
    ReagentList(NULL),
    ReagentGroupColorList(NULL),
    ProgramSettings(NULL),
    SpecialVerifierGroupA(NULL),
    SpecialVerifierGroupB(NULL),
    SpecialVerifierGroupC(NULL),
    SpecialVerifierGroupD(NULL)
{
    if (!InitializeContainers()) {
        qDebug() << "CDataContainer::Constructor / InitContainers failed";
    }
}


CDataContainer::~CDataContainer()
{
    try {
        (void)DeinitializeContainers();
    }
    catch(...) {
    }
}

bool CDataContainer::InitializeContainers()
{
    if (!CDataContainerCollectionBase::InitContainers()) {
        return false;
    }

    if (!ResetDCUserSettings()) {
        qDebug() << "CDataContainer::InitializeContainers failed, because ResetDCSettings failed!";
        return false;
    }

    ReagentGroupList = new CDataReagentGroupList();
    if (!ResetDCReagentGroupList()) {
        qDebug() << "CDataContainer::InitContainers failed, because ResetDCReagentGroupList failed.";
        return false;
    }

    ReagentGroupColorList = new CReagentGroupColorList();
    if (!ResetDCReagentGroupColorList()) {
        qDebug() << "CDataContainer::InitContainers failed, because ResetDCReagentGroupColorList failed.";
        return false;
    }

    ReagentList = new CDataReagentList();
    if (!ResetDCReagentList()) {
        qDebug() << "CDataContainer::InitContainers failed, because ResetDCReagentList failed.";
        return false;
    }

    StationList = new CDashboardDataStationList();
    if (!ResetDCStationList()) {
        qDebug() << "CDataContainer::InitContainers failed, because ResetDCStationList failed!";
        return false;
    }

    ProgramList = new CDataProgramList();
    if (!ResetDCProgramList()) {
        qDebug() << "CDataContainer::InitContainers failed, because ResetDCProgramList failed.";
        return false;
    }

    ProgramSettings = new CProgramSettings();
    if (!ResetDCProgramSettings()) {
        qDebug() << "CDataContainer::InitContainers failed, because ResetDCProgramSettings failed.";
        return false;
    }



      // create special verifier for reagents and programs
//    IVerifierInterface *p_SpecialVerifier = new CSpecialVerifierGroupA(ProgramList, ReagentList, StationList);
//    // register this verifier object in the data containers (=> dependency injection)
//    if (!ProgramList->AddVerifier(p_SpecialVerifier)) {
//        qDebug() << "CDataContainer::InitContainers failed, because ProgramList->AddVerifier failed.";
//        return false;
//    }
//    if (!ReagentList->AddVerifier(p_SpecialVerifier)) {
//        qDebug() << "CDataContainer::InitContainers failed, because ReagentList->AddVerifier failed.";
//        return false;
//    }

    // create special verfiers
    if (SpecialVerifierGroupA == NULL) {
        SpecialVerifierGroupA = new CSpecialVerifierGroupA(ProgramList, ReagentList, StationList, ReagentGroupList);
    }


    if (SpecialVerifierGroupC == NULL) {
        SpecialVerifierGroupC = new CSpecialVerifierGroupC(ReagentList, SettingsInterface);
    }

    /*if (SpecialVerifierGroupD == NULL) {
        SpecialVerifierGroupD = new CSpecialVerifierGroupD(StationList, StationMatrix);
    }
*/

    (void)ProgramList->AddVerifier(SpecialVerifierGroupA);
    // add required special verifiers for the station list
    (void)StationList->AddVerifier(SpecialVerifierGroupA);
    (void)StationList->AddVerifier(SpecialVerifierGroupD);

    (void)SettingsInterface->AddVerifier(SpecialVerifierGroupC);


    m_IsInitialized = true;
    return true;
}

bool CDataContainer::DeinitializeContainers()
{
    if (CDataContainerCollectionBase::DeinitContainers()) {

    }
    delete StationList;
    delete ProgramList;
    delete ReagentList;
    delete ReagentGroupList;
    delete ReagentGroupColorList;
    delete SpecialVerifierGroupA;
    delete SpecialVerifierGroupB;
    delete SpecialVerifierGroupC;
    delete SpecialVerifierGroupD;
    delete ProgramSettings;

    return true;
}

bool CDataContainer::ResetDCProgramList()
{
    if (m_IsInitialized == true) {
        qDebug() << "CDataContainer::ResetDCProgramList was already called";
        return false;
    }

    // init program list
    if(ProgramList){
        ProgramList->Init();
    }



    // create a verifier object for this data container, if not done before
    static IVerifierInterface *p_DataProgramListVerifier = NULL;
    if (p_DataProgramListVerifier == NULL) {
        p_DataProgramListVerifier = new CDataProgramListVerifier(this);
    }
    // register this verifier object in the data container (=> dependency injection)
    if(ProgramList){
        (void)ProgramList->AddVerifier(p_DataProgramListVerifier);
    }

    return true;
}

bool CDataContainer::ResetDCProgramSettings()
{
    if (m_IsInitialized == true) {
        qDebug() << "CDataContainer::ResetDCProgramSettings was already called";
        return false;
    }

    // init program list
    if(ProgramSettings){
        ProgramSettings->Init();
    }


    // create a verifier object for this data container, if not done before
//    static IVerifierInterface *p_DataProgramListVerifier = NULL;
//    if (p_DataProgramListVerifier == NULL) {
//        p_DataProgramListVerifier = new CDataProgramListVerifier(this);
//    }
    // register this verifier object in the data container (=> dependency injection)
//    ProgramList->AddVerifier(p_DataProgramListVerifier);

    return true;
}

bool CDataContainer::ResetDCReagentGroupList()
{
    if (m_IsInitialized == true) {
        qDebug() << "CDataContainer::ResetDCReagentGroupList was already called";
        return false;
    }

    // init reagent list
    if(ReagentGroupList){
        ReagentGroupList->Init();
    }
    return true;
}

bool CDataContainer::ResetDCReagentGroupColorList()
{
    if (m_IsInitialized == true) {
        qDebug() << "CDataContainer::ResetDCReagentGroupColorList was already called";
        return false;
    }

    // init reagent list
    if(ReagentGroupColorList){
        ReagentGroupColorList->Init();
    }
    return true;
}

bool CDataContainer::ResetDCReagentList()
{
    if (m_IsInitialized == true) {
        qDebug() << "CDataContainer::ResetDCReagentList was already called";
        return false;
    }

    // init reagent list
    if(ReagentList){
        ReagentList->Init();

        // create a verifier object for this data container, if not done before
        static IVerifierInterface *p_DataReagentListVerifier = NULL;
        if (p_DataReagentListVerifier == NULL) {
            p_DataReagentListVerifier = new CDataReagentListVerifier();
        }
        // register this verifier object in the data container (=> dependency injection)
        (void)ReagentList->AddVerifier(p_DataReagentListVerifier);
    }

    return true;
}

bool CDataContainer::ResetDCStationList()
{
    /*
    if (m_IsInitialized == true) {
        qDebug() << "CDataContainer::ResetDCStationList was already called";
        return false;
    }

    StationList->Init();

    // create a verifier object for this data container, if not done before
    static IVerifierInterface *p_DataStationListVerifier = NULL;
    if (p_DataStationListVerifier == NULL) {
        p_DataStationListVerifier = new CDataStationListVerifier();
    }
    // register this verifier object in the data container (=> dependency injection)
    StationList->AddVerifier(p_DataStationListVerifier);
*/
      return true;
}

bool CDataContainer::ResetDCUserSettings(void)
{
    if (m_IsInitialized == true) {
        qDebug() << "CDataContainer::ResetDCUserSettings was already called";
        return false;
    }

    // create a verifier object for this data container, if not done before
    static IVerifierInterface *p_DataUserSettingVerifier = NULL;
    if (p_DataUserSettingVerifier == NULL) {
        p_DataUserSettingVerifier = new CHimalayaSettingsVerifier();
    }

    // register this verifier object in the data container (=> dependency injection)
    if(SettingsInterface && p_DataUserSettingVerifier){
        if(!SettingsInterface->AddVerifier(p_DataUserSettingVerifier)) {
            qDebug() << "ResetDCUserSettings failed due to \
                        SettingsInterface->AddVerifier(p_DataUserSettingVerifier)";
            return false;
        }
    }

    return true;
}


QString CDataContainer::GetReagentName(QString ReagentID)
{
    if (ReagentList) {
        const CReagent *p_Reagent = ReagentList->GetReagent(ReagentID);
        if (p_Reagent) {
            return p_Reagent->GetReagentName();
        }
    }

    return NULL;
}

}// namespace DataManager
