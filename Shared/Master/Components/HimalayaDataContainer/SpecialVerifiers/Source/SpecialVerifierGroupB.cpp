/****************************************************************************/
/*! \file SpecialVerifierGroupB.cpp
 *
 *  \brief Implementation file for class CSpecialVerifierGroupB.
 *
 *  $Version:   $ 0.2
 *  $Date:      $ 2011-08-01, 2012-05-03
 *  $Author:    $ F. Toth, Raju
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
#include <QFile>

#include "HimalayaDataContainer/SpecialVerifiers/Include/SpecialVerifierGroupB.h"
#include "Global/Include/EventObject.h"
#include "HimalayaDataContainer/Helper/Include/HimalayaDataManagerEventCodes.h"

namespace DataManager {

/****************************************************************************/
/*!
 *  \brief Constructor
 */
/****************************************************************************/
CSpecialVerifierGroupB::CSpecialVerifierGroupB() : mp_DProgramList(NULL), mp_PSequenceList(NULL)
{
}

/****************************************************************************/
/*!
 *  \brief  Constructor
 *
 *  \iparam p_DataProgramList = Pointer to a Program list
 *  \iparam p_PSequenceList = Pointer to a Program Sequence list
 */
/****************************************************************************/
CSpecialVerifierGroupB::CSpecialVerifierGroupB(CDataProgramList* p_DataProgramList, CProgramSequenceList* p_PSequenceList)
    : mp_DProgramList(p_DataProgramList), mp_PSequenceList(p_PSequenceList)
{
}


/****************************************************************************/
/*!
 *  \brief  Verifies the data
 *
 *  \iparam p_ContainerBase = Pointer to a Container base
 *
 *  \return Successful (true) or not (false)
 */
/****************************************************************************/
bool CSpecialVerifierGroupB::VerifyData(CDataContainerBase* p_ContainerBase)
{
    bool Result = true;

    if (p_ContainerBase != NULL) {
        // prepare member pointers according to given container type
        // switch given container to temporary one for check
        // and switch back to real data after check again
        CDataContainerBase* p_TempDataItem = NULL;

        // check the containers consists of data or not if not please log it
        // which container is failed. This is for logger
        if (mp_DProgramList == NULL) {
            Global::EventObject::Instance().RaiseEvent(EVENT_DM_GV_PROGRAM_IS_NOT_INITIALIZED);
            qDebug() << "CSpecialVerifierGroupB::VerifyData failed. Program List is NULL";
            return false;
        }
        if (mp_PSequenceList == NULL) {
            Global::EventObject::Instance().RaiseEvent(EVENT_DM_GV_PSEQUENCE_IS_NOT_INITIALIZED);
            qDebug() << "CSpecialVerifierGroupB::VerifyData failed. Program Sequence List is NULL";
            return false;
        }

        // copy all the data data to temporary variables
        if (p_ContainerBase->GetDataContainerType() == PROGRAMS) {
            p_TempDataItem = mp_DProgramList;
            mp_DProgramList = static_cast<CDataProgramList*>(p_ContainerBase);
        }
        else if (p_ContainerBase->GetDataContainerType() == PROGRAMSEQUENCE) {
            p_TempDataItem = mp_PSequenceList;
            mp_PSequenceList = static_cast<CProgramSequenceList*>(p_ContainerBase);
        }
        else {
            Global::EventObject::Instance().RaiseEvent(EVENT_DM_GV_INVALID_CONTAINER_TYPE);
            qDebug() << "CSpecialVerifierGroupB::VerifyData failed. Invalid conatiner type";
            return false;
        }

        if (Result) {
            if (!CheckData()) {
                Result = false;
            }
        }

        // revert all the temporary data to local variables
        if (p_ContainerBase->GetDataContainerType() == PROGRAMS) {
            mp_DProgramList = static_cast<CDataProgramList*>(p_TempDataItem);
        }
        else if (p_ContainerBase->GetDataContainerType() == PROGRAMSEQUENCE) {
            mp_PSequenceList = static_cast<CProgramSequenceList*>(p_TempDataItem);
        }
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_GV_BASE_CONTAINER_IS_NOT_INITIALIZED);
        qDebug() << "CSpecialVerifierGroupB::VerifyData failed. Container base is not initialised";;
        Result = false;
    }

    return Result;

}

/****************************************************************************/
/*!
 *  \brief  Checks the data with other containers (Like Reagent list,
 *          UserSettings Interface)
 *
 *  \return Successful (true) or not (false)
 */
/****************************************************************************/
bool CSpecialVerifierGroupB::CheckData()
{
    ListOfIDs_t ProgramIDs; ///< Store all program IDs

    bool Result = true;

    // check required to avoid lint warning
    if (mp_DProgramList != NULL && mp_PSequenceList != NULL) {        
        // first check the whether Programs count is equal to Program Sequence count
        if (!(mp_DProgramList->GetNumberOfPrograms() == mp_PSequenceList->GetNumberOfPrograms())) {
            Global::EventObject::Instance().RaiseEvent(EVENT_DM_GV_NO_OF_PROGRAMS_NOT_MATCHING);
            // qDebug() << "CSpecialVerifierGroupB::CheckData failed. Program list number of Programs is not same as in Program Sequence list";
            Result = false;
        }
        // store all the program IDs in a list, so it will be easy while doing testing
        // otherwise the program will be a mess        
        for (int ProgramCount = 0; ProgramCount < mp_DProgramList->GetNumberOfPrograms(); ProgramCount++) {
            // get the program class object
            CProgram* p_Program = mp_DProgramList->GetProgram(ProgramCount);
            // check the program class object status whether exists or not
            if (p_Program != NULL) {
                ProgramIDs.append(p_Program->GetID());
            }
            else {
                qDebug() << "CSpecialVerifierGroupB::CheckData failed. Program List is NULL";
                Result = false;
            }
        }

        // check the Program ID
        for (int ProgramSeqCount = 0; ProgramSeqCount < mp_PSequenceList->GetNumberOfPrograms(); ProgramSeqCount++) {
            // get the program sequence
            CProgramSequence* p_PSequence = const_cast<CProgramSequence*> (mp_PSequenceList->GetProgramSequenceStep(ProgramSeqCount));

            if (p_PSequence != NULL) {
                // check the Program ID exists in Program List
                if (!ProgramIDs.contains(p_PSequence->GetID())) {
                    Global::EventObject::Instance().RaiseEvent(EVENT_DM_GV_PROG_ID_EXIST_IN_STATION_NOT_IN_PROG_LIST,
                                                               Global::FmtArgs() << p_PSequence->GetID());
                    //qDebug() << "CSpecialVerifierGroupB::CheckData failed. Program ID does not exist in Program List but it exists in Program Sequence List - " << p_PSequence->GetID();
                    Result = false;
                    break;
                }
            }
        }
    }

    // if everything goes well then Special verifier verified all the data
    return Result;
}

}  // namespace DataManager


