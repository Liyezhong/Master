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

#include "HimalayaDataContainer/Containers/ProgramSequence/Include/ProgramSequenceVerifier.h"

#include "HimalayaDataContainer/Containers/Reagents/Include/DataReagentListVerifier.h"

#include "HimalayaDataContainer/Containers/Stations/Include/DataStationListVerifier.h"


#include "DataManager/Containers/DeviceConfiguration/Include/DeviceConfigurationVerifier.h"

#include "DataManager/Containers/Adjustment/Include/AdjustmentVerifier.h"

#include "DataManager/Containers/Racks/Include/RackList.h"
#include "DataManager/Containers/Racks/Include/RackListVerifier.h"
#include "DataManager/Containers/UserSettings/Include/UserSettingsVerifier.h"
#include "HimalayaDataContainer/SpecialVerifiers/Include/SpecialVerifierGroupA.h"
#include "HimalayaDataContainer/SpecialVerifiers/Include/SpecialVerifierGroupB.h"
#include "HimalayaDataContainer/SpecialVerifiers/Include/SpecialVerifierGroupC.h"
#include "HimalayaDataContainer/SpecialVerifiers/Include/SpecialVerifierGroupD.h"


namespace DataManager {

CDataContainer::CDataContainer(Threads::MasterThreadController *p_MasterThreadController) :
    DataManager::CDataContainerCollectionBase(p_MasterThreadController),
    ReagentGroupList(NULL),
    ProgramList(NULL),
    ReagentList(NULL),
    StationList(NULL),
    ProgramSequenceList(NULL),
    SpecialVerifierGroupA(NULL),
    SpecialVerifierGroupB(NULL),
    SpecialVerifierGroupC(NULL),
    SpecialVerifierGroupD(NULL)
{
    if (!InitContainers()) {
        qDebug() << "CDataContainer::Constructor / InitContainers failed";
    }
}


CDataContainer::~CDataContainer()
{
    if (!DeinitContainers()) {
        qDebug() << "CDataContainer::Destructor / DeinitContainers failed";
    }
}

bool CDataContainer::InitContainers()
{
    if (!CDataContainerCollectionBase::InitContainers()) {
        return false;
    }

    ProgramList = new CDataProgramList();
    if (!ResetDCProgramList()) {
        qDebug() << "CDataContainer::InitContainers failed, because ResetDCProgramList failed.";
        return false;
    }

    ReagentGroupList = new CDataReagentGroupList();
    if (!ResetDCReagentGroupList()) {
        qDebug() << "CDataContainer::InitContainers failed, because ResetDCProgramList failed.";
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

    ProgramSequenceList = new CProgramSequenceList();
    if (!ResetDCProgramSequenceList()) {
        qDebug() << "CDataContainer::InitContainers failed, because ResetProgramSequenceList failed.";
        return false;
    }

    RackList = new CRackList();
    if (!ResetDCRackList()) {
        qDebug() << "CDataContainer::InitContainers failed, because ResetDCRackList failed.";
        return false;
    }



    AdjustmentList = new CAdjustment();
    if (!ResetDCAdjustment()) {
        qDebug() << "CDataContainer::InitContainers failed, because ResetDCAdjustment failed.";
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
        SpecialVerifierGroupA = new CSpecialVerifierGroupA(ProgramList, ReagentList, StationList);
    }

    if (SpecialVerifierGroupB == NULL) {
        SpecialVerifierGroupB = new CSpecialVerifierGroupB(ProgramList, ProgramSequenceList);
    }

    if (SpecialVerifierGroupC == NULL) {
        SpecialVerifierGroupC = new CSpecialVerifierGroupC(ReagentList, SettingsInterface);
    }

    /*if (SpecialVerifierGroupD == NULL) {
        SpecialVerifierGroupD = new CSpecialVerifierGroupD(StationList, StationMatrix);
    }
*/

    ProgramList->AddVerifier(SpecialVerifierGroupA);
    // add required special verifiers for the station list
    StationList->AddVerifier(SpecialVerifierGroupA);
    StationList->AddVerifier(SpecialVerifierGroupD);

    ProgramSequenceList->AddVerifier(SpecialVerifierGroupB);
    SettingsInterface->AddVerifier(SpecialVerifierGroupC);


    m_IsInitialized = true;
    return true;
}

bool CDataContainer::DeinitContainers()
{
    if (CDataContainerCollectionBase::DeinitContainers()) {

    }
    delete RackList;
    delete ProgramSequenceList;
    delete StationList;
    delete ProgramList;
    delete ReagentList;
    delete ReagentGroupList;
    delete SpecialVerifierGroupA;
    delete SpecialVerifierGroupB;
    delete SpecialVerifierGroupC;
    delete SpecialVerifierGroupD;

    return true;
}

bool CDataContainer::ResetDCProgramList()
{
    if (m_IsInitialized == true) {
        qDebug() << "CDataContainer::ResetDCProgramList was already called";
        return false;
    }

    // init program list
    ProgramList->Init();


    // create a verifier object for this data container, if not done before
    static IVerifierInterface *p_DataProgramListVerifier = NULL;
    if (p_DataProgramListVerifier == NULL) {
        p_DataProgramListVerifier = new CDataProgramListVerifier(this);
    }
    // register this verifier object in the data container (=> dependency injection)
    ProgramList->AddVerifier(p_DataProgramListVerifier);

    return true;
}

bool CDataContainer::ResetDCProgramSequenceList()
{
    if (m_IsInitialized == true) {
        qDebug() << "CDataContainer::ResetDCProgramList was already called";
        return false;
    }

    // init program list
    ProgramSequenceList->Init();


    // create a verifier object for this data container, if not done before
    static IVerifierInterface *p_ProgramSequenceListVerifier = NULL;
    if (p_ProgramSequenceListVerifier == NULL) {
        p_ProgramSequenceListVerifier = new CProgramSequenceListVerifier();
    }
    // register this verifier object in the data container (=> dependency injection)
    ProgramSequenceList->AddVerifier(p_ProgramSequenceListVerifier);

    return true;
}

bool CDataContainer::ResetDCReagentGroupList()
{
    if (m_IsInitialized == true) {
        qDebug() << "CDataContainer::ResetDCReagentGroupList was already called";
        return false;
    }

    // init reagent list
    ReagentGroupList->Init();
    return true;
}

bool CDataContainer::ResetDCReagentList()
{
    if (m_IsInitialized == true) {
        qDebug() << "CDataContainer::ResetDCReagentList was already called";
        return false;
    }

    // init reagent list
    ReagentList->Init();

    // create a verifier object for this data container, if not done before
    static IVerifierInterface *p_DataReagentListVerifier = NULL;
    if (p_DataReagentListVerifier == NULL) {
        p_DataReagentListVerifier = new CDataReagentListVerifier();
    }
    // register this verifier object in the data container (=> dependency injection)
    ReagentList->AddVerifier(p_DataReagentListVerifier);

    return true;
}

bool CDataContainer::ResetDCRackList()
{
    if (m_IsInitialized) {
        qDebug() << "CDataContainer::ResetDCReagentList was already called";
        return false;
    }

    // create a verifier object for this data container, if not done before
    static IVerifierInterface *p_RackListVerifier = NULL;
    if (p_RackListVerifier == NULL) {
        p_RackListVerifier = new CRackListVerifier();
    }
    // register this verifier object in the data container (=> dependency injection)
    RackList->AddVerifier(p_RackListVerifier);

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




bool CDataContainer::ResetDCStationGrid()
{
    if (m_IsInitialized == true) {
        qDebug() << "CDataContainer::ResetDCStationGrid was already called";
        return false;
    }



    return true;
}

bool CDataContainer::ResetDCAdjustment()
{
    if (m_IsInitialized == true) {
        qDebug() << "CDataContainer::ResetDCAdjustment was already called";
        return false;
    }

    // create a verifier object for this data container, if not done before
    static IVerifierInterface *p_AdjustmentVerifier = NULL;
    if (p_AdjustmentVerifier == NULL) {
        p_AdjustmentVerifier = new CAdjustmentVerifier();
    }
    // register this verifier object in the data container (=> dependency injection)
    AdjustmentList->AddVerifier(p_AdjustmentVerifier);

    return true;
}

bool CDataContainer::RefreshProgramStepStationlist()
{
    bool Result = true;

    //Get Program ID's of Programs which are startable and used flag is set
    //from program sequence list
    QStringList StartableProgramList;
    StartableProgramList = ProgramSequenceList->GetStartableProgramIDList();
    CProgram* p_Program = NULL;
    CProgramStep* p_ProgStep = NULL;
    //For all the steps in the above programs generate possible station list
    for (qint32 I = 0; I < StartableProgramList.count(); I++) {
        p_Program = const_cast<CProgram*>(ProgramList->GetProgram(StartableProgramList.at(I)));
        if (p_Program) {
            p_ProgStep = p_Program->GetProgramStepExpanded(0);
            if (p_ProgStep->GetReagentID() != SLIDE_ID_SPECIAL_REAGENT) {
                //Will add SID step to expanded step list
                (void)p_Program->CreateSIDStationStep(p_Program->GetNextFreeStepID(true));
            }
            for (qint32 J = 0; J < p_Program->GetNumberOfStepsInExpandedList(); J++) {
                p_ProgStep = p_Program->GetProgramStepExpanded(J);
                if (p_ProgStep) {
                    QString ReagentID = p_ProgStep->GetReagentID();
                    QString ProgramID = p_Program->GetID();
                    QString StepID = p_ProgStep->GetStepID();
                    QStringList PossibleStations;
                    if (ReagentID == SLIDE_ID_SPECIAL_REAGENT) {
                        ProgramID = "-1";
                        StepID = "-1";
                    }
                    //PossibleStations = StationList->GetPossibleStations(ReagentID, ProgramID, StepID);
                    //Todo Abe
                    p_ProgStep->SetStationIDList(PossibleStations);
                }
                else {
                    return false;
                }
            }//End of prog step for loop
        }
        else {
            Result = false;
        }
        Result = AddStepsToExpandedStepList(p_Program);
    }// end of program for loop

    return Result;
}



bool CDataContainer::AddStepsToExpandedStepList(CProgram *p_Program)
{
    /*if (!p_Program)
        return false;

    ListOfExpandedSteps_t* p_ExpandedStepList = const_cast<ListOfExpandedSteps_t *>(p_Program->GetExpandedStepList());

    if (p_ExpandedStepList->count() == 0) {
        return false;
    }
    for (int I = 0; I < p_Program->GetNumberOfStepsInExpandedList(); I++) {
        CProgramStep* p_CurrentProgStep = NULL;
        CProgramStep* p_NextProgStep = NULL;
        p_CurrentProgStep = p_Program->GetProgramStepExpanded(I);
        if (p_CurrentProgStep) {
            qDebug()<<"\nCurrent PRogram"<<p_Program->GetID();
            qDebug()<<"\n\n p_CurrentProgStep StationID List"<< p_CurrentProgStep->GetStationIDList();

            /* I am not worrying about Index crossing the maximum , since GetProgramStep checks validity of index */
           /* p_NextProgStep = const_cast<CProgramStep *>(p_Program->GetProgramStepExpanded(I + 1));
            if (p_NextProgStep) {
                bool OK = true;
                //if the current step(i.e Next Prog Step) and previous step(i.e Current Prog Step) is in different zone
                //insert an dry station step inbetween
                if (!CompareSteps(*p_CurrentProgStep, *p_NextProgStep, OK)) {
                    if (OK) {
                        QString ID = p_Program->GetID();
                        QStringList PossibleStations = StationList->GetPossibleStations(DRY_STATION_TYPE, "-1", "-1");
                        CProgramStep *p_ExpandedStep = p_Program->CreateDryStationStep(p_Program->GetNextFreeStepID(true), PossibleStations);
                        p_ExpandedStepList->insert(I + 1, p_ExpandedStep);
                        //this makes sure index is pointing to the correct program step for comparision
                        I++;
                    }
                    else {
                        return false;
                    }
                }
                //else no need to add dry station steps
            }
        }
    }*///End of for loop
    return true;
}

bool CDataContainer::CompareSteps(CProgramStep& CurrentProgramStep, CProgramStep& NextProgramStep, bool& OK)
{
   /* QStringList StationIDList = CurrentProgramStep.GetStationIDList();
    StationZoneType_t CurrentZone = INVALID_ZONE;
    StationZoneType_t TemporaryCurrentZone = INVALID_ZONE;
    StationZoneType_t NextStepZone = INVALID_ZONE;
    //To make sure that all the stations in the list are in the same zone.
    //Else the stations xml generated by BLG has a bug
    for (qint32 I = 0; I < StationIDList.count(); I++) {
        CurrentZone = StationMatrix->GetStationZone(StationIDList.at(I));
        if ((I != 0) && CurrentZone != TemporaryCurrentZone) {
            qDebug("### FATAL ERROR Possible Stations for program step in different zone ");
            //! \todo Inform event Handler
            OK = false;
            return false;
        }
        TemporaryCurrentZone = CurrentZone;
    }
    StationIDList.clear();

    StationIDList = NextProgramStep.GetStationIDList();
    //To make sure that all the stations in the list are in the same zone.
    //Else the stations xml generated by BLG has a bug
    for (qint32 I = 0; I < StationIDList.count(); I++) {
        NextStepZone = StationMatrix->GetStationZone(StationIDList.at(I));
        if ((I != 0) && NextStepZone != TemporaryCurrentZone) {
            qDebug("### FATAL ERROR Possible Stations for program step in different zone ");
            OK = false;
            return false;
        }
        TemporaryCurrentZone = NextStepZone;
    }


    //if zones are different , need to insert dry station step
    if ((CurrentZone == LEFT_ZONE && NextStepZone == RIGHT_ZONE) ||
        (CurrentZone == RIGHT_ZONE && NextStepZone == LEFT_ZONE))  {
        return false;
    }
    else {
        return true;
    }*/
    return false;
}

QString CDataContainer::GetReagentName(QString ReagentID)
{
    if (ReagentList) {
        const CReagent *p_Reagent = ReagentList->GetReagent(ReagentID);
        if (p_Reagent) {
            return p_Reagent->GetReagentName();
        }
    }
}

}// namespace DataManager
