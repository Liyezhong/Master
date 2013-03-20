/****************************************************************************/
/*! \file DataProgramListVerifier.cpp
 *
 *  \brief Implementation file for class CDataProgramListVerifier.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2011-08-01
 *  $Author:    $ F. Toth, N.Kamath
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
#include <QStringList>

#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramListVerifier.h"
#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramList.h"
#include "HimalayaDataContainer/Containers/Programs/Include/ProgramBase.h"
#include "Global/Include/EventObject.h"
#include "HimalayaDataManager/Include/DataContainer.h"

namespace DataManager {
/****************************************************************************/
/*!
 *  \brief Default Constructor
 */
/****************************************************************************/
CDataProgramListVerifier::CDataProgramListVerifier(CDataContainer *p_DataContainer)
    :mp_DataContainer(p_DataContainer)
    ,mp_DPL(NULL)
{

}

CDataProgramListVerifier::CDataProgramListVerifier()
    :mp_DataContainer(NULL),
    mp_DPL(NULL)
{

}
/****************************************************************************/
/*!
 *  \brief Verifies the data present in the program list against constraints.
 */
/****************************************************************************/
bool CDataProgramListVerifier::VerifyData(CDataContainerBase* p_DataProgramList)
{
    // by default make the verification flag to true
    bool VerifiedData = true;

    // assign pointer to member variable
    mp_DPL = static_cast<CDataProgramList*>(p_DataProgramList);

    if (!mp_DPL) {
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_NULL_PTR, Global::GUI_MSG_BOX);
        m_ErrorsHash.insert(EVENT_DM_NULL_PTR, Global::tTranslatableStringList());
        return false;
    }
    // check constraints

    // check max number of programs (50)
    if (mp_DPL->GetNumberOfPrograms() > PROGRAMS_MAX) {
        qDebug() << "### Too many programs (max 50 allowed): " << mp_DPL->GetNumberOfPrograms();
        m_ErrorsHash.insert(EVENT_DM_PROG_COUNT_EXCEEDS_LIMIT, Global::tTranslatableStringList() << QString::number(PROGRAMS_MAX) );
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_COUNT_EXCEEDS_LIMIT,
                                                   Global::tTranslatableStringList() << QString::number(PROGRAMS_MAX));
        VerifiedData = false;
    }
    QString NextStepID = mp_DPL->GetNextFreeProgID(false).mid(1);


    if (!(NextStepID.toInt() >= NEXT_STEP_ID_MIN)) {
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_NEXT_STEPID_LESS_THAN_ONE, Global::GUI_MSG_BOX);
        m_ErrorsHash.insert(EVENT_DM_NEXT_STEPID_INVALID, Global::tTranslatableStringList());
        qDebug()<<"### ProgramId less than 1";
        VerifiedData = false;
    }

    // check content of each program
    CProgram *p_Program;
    int LeicaSplReagentCount;
    QStringList RackColorList;
    RackColorList << "white"<< "yellow" << "red" << "darkblue"
                  << "lightblue" << "black" << "grey" << "pink"
                  << "green";
    QStringList AssignedColorList;
    // empty the string
    m_LeicaReagentIDList.clear();
    for (qint32 I = 0; I < mp_DPL->GetNumberOfPrograms(); I++) {
        p_Program = mp_DPL->GetProgram(I);
        if (p_Program) {
            //Verify program ID
            if ((p_Program->GetID().at(0) != 'U') && (p_Program->GetID().at(0) != 'L')) {
                qDebug()<<"Invalid Program Id"<<p_Program->GetID();
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_INVALID_PROGRAM_ID,
                                                           Global::tTranslatableStringList() << p_Program->GetID()
                                                           << p_Program->GetLongName(),
                                                           Global::GUI_MSG_BOX);
                m_ErrorsHash.insert(EVENT_DM_INVALID_PROGRAM_ID,
                                   Global::tTranslatableStringList() << p_Program->GetID()
                                   << p_Program->GetLongName());
                VerifiedData = false;
            }
            bool Ok = false;
            int ProgramId = p_Program->GetID().mid(1).toInt(&Ok);
            if ((!Ok) && (!(ProgramId >= 1))) {
                qDebug()<<"### Invalid program Id:"<<p_Program->GetID();
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_INVALID_PROGRAM_ID,
                                                           Global::tTranslatableStringList() << p_Program->GetID()
                                                           << p_Program->GetLongName(),
                                                           Global::GUI_MSG_BOX);
                m_ErrorsHash.insert(EVENT_DM_INVALID_PROGRAM_ID,
                                   Global::tTranslatableStringList() << p_Program->GetID()
                                   << p_Program->GetLongName());
                VerifiedData = false;
            }
            //Verify short name & long name length constraints
            if ((p_Program->GetShortName().length() < SHORT_NAME_LENGTH_MIN) ||
                    (p_Program->GetShortName().length() > SHORT_NAME_LENGTH_MAX)) {
                qDebug()<<"###Short name length check failed"<<p_Program->GetShortName();
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROGSHORT_NAME_LENGTH_CHECK_FAILED,
                                                           Global::tTranslatableStringList() << p_Program->GetLongName()
                                                           << SHORT_NAME_LENGTH_MIN << SHORT_NAME_LENGTH_MAX);
                VerifiedData = false;
            }

            if ((p_Program->GetLongName().length() < LONG_NAME_LENGTH_MIN) ||
                    (p_Program->GetLongName().length() > LONG_NAME_LENGTH_MAX)) {
                qDebug()<<"### Long name length check failed"<<p_Program->GetLongName();
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROGLONG_NAME_LENGTH_CHECK_FAILED,
                                                           Global::tTranslatableStringList() << p_Program->GetLongName()
                                                           << LONG_NAME_LENGTH_MIN << LONG_NAME_LENGTH_MAX);
                VerifiedData = false;
            }

            //Verify color
            QString Color = p_Program->GetColor();
            if (!RackColorList.contains(Color,Qt::CaseSensitive)) {
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_UKNOWN_PROG_COLOUR,
                                                           Global::tTranslatableStringList() << p_Program->GetLongName()
                                                           << Color);
                qDebug("### Unknown p_Program Color");
                VerifiedData = false;
            }
            //Verify Color is unique to a program
            if (AssignedColorList.contains(Color)) {
                qDebug()<<"### Color already assigned to a program. Program Color should be unique";
                qDebug()<<"### Please assign different Color to the program. Program ID:"<<p_Program->GetID();
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_COLOUR_NOT_UNIQUE,
                                                           Global::tTranslatableStringList() << p_Program->GetColor());
                VerifiedData = false;
            }
            // White color Program can be multiple
            if (Color != "white") {
                AssignedColorList.append(p_Program->GetColor());
            }
            //check no of steps
            if (p_Program->GetNumberOfSteps() > PROGRAM_STEPS_MAX) {
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_STEP_COUNT_EXCEEDS_LIMIT,
                                                           Global::tTranslatableStringList() <<p_Program->GetLongName()
                                                           << p_Program->GetNumberOfSteps(),
                                                           Global::GUI_MSG_BOX);
                qDebug("### Program Steps should be less than 50");
                VerifiedData = false;
            }
            LeicaSplReagentCount = 0; // intialise to get the count
            // check program steps and update the verifier flags and error description
            CheckProgramStep(p_Program, LeicaSplReagentCount, VerifiedData);
            if (p_Program->GetID().at(0) == 'L') {
                // check program steps and update the verifier flags and error description
                CheckLeicaProgramSteps(p_Program, VerifiedData);
                //for leica program the count of Leica  Special Reagent must be equal to 2
                //not less or greater
                if (LeicaSplReagentCount != 2) {
                    Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_EDITED_LEICA_REAGENT_COUNT_INVALID,
                                                               Global::tTranslatableStringList() << p_Program->GetLongName()
                                                               << LeicaSplReagentCount,
                                                               Global::GUI_MSG_BOX);
                    qDebug("### Leica Special Reagent Count must be equal to 2");
                    qDebug()<<"Program Name"<<p_Program->GetLongName();
                    VerifiedData = false;
                }
            }
            else if (p_Program->GetID().at(0) == 'U') {
                // check program steps and update the verifier flags and error description
                CheckUserProgramSteps(p_Program, VerifiedData);
            }
        }
    }//End of program for loop

    return VerifiedData;
}

/****************************************************************************/
/*!
 *  \brief  Verifies the program step
 *
 *  \iparam Program - Pointer to CProgram
 *  \iparam VerifiedData = verifier flag value
 *  \iparam ErrorDescription = Description of the error
 *
 */
/****************************************************************************/
void CDataProgramListVerifier::CheckProgramStep(CProgram* p_Program, int &LeicaSplReagentCount, bool &VerifiedData)
{
    CProgramStep *p_ProgramStep = NULL;
    QStringList ReagentIDList;

    for (qint32 X = 0; X < p_Program->GetNumberOfSteps(); X++) {
        p_ProgramStep = const_cast<CProgramStep *>(p_Program->GetProgramStep(X));
        if (p_ProgramStep != NULL) {
            QString ReagentID = p_ProgramStep->GetReagentID();
            //Validate Reagent ID
            switch(ReagentID.at(0).toAscii()) {
                case 'U':
                case 'S':
                case 'L':
                    break;
                default:
                    m_ErrorsHash.insert(EVENT_DM_PROG_VERFIER_INVALID_REAGENT_ID,
                                        Global::tTranslatableStringList() << p_Program->GetLongName()
                                        << p_ProgramStep->GetReagentID());
                    Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_VERFIER_INVALID_REAGENT_ID,
                                                               Global::tTranslatableStringList() << p_Program->GetLongName()
                                                               << p_ProgramStep->GetReagentID(),
                                                               Global::GUI_MSG_BOX);
                    qDebug() << "### Reagent ID doesn't start with S,L,U: ";
                    VerifiedData = false;
                    break;
            }

            ReagentIDList << ReagentID;

            if (p_Program->GetNumberOfSteps()== 1) {
                if(p_ProgramStep->GetReagentID()== "S7" || p_ProgramStep->GetReagentID()== "S8"){
                    m_ErrorsHash.insert(EVENT_DM_TRANS_UNLOADER_STEP_ONLY, Global::tTranslatableStringList() << p_Program->GetLongName());
                    Global::EventObject::Instance().RaiseEvent(EVENT_DM_TRANS_UNLOADER_STEP_ONLY,
                                                               Global::tTranslatableStringList() <<p_Program->GetLongName());
                    qDebug() << "### CDataProgramListVerifier::NoEVENT_DM_TRANS_UNLOADER_STEP_ONLY single valid program step exists except Unloader/Transfer";
                    VerifiedData = false;
                }
            }
            // Verify that OVEN station is selected as first step for a program
            if (ReagentID == "S4") {
                if(X != 0){
                    m_ErrorsHash.insert(EVENT_DM_OVEN_NOT_FIRST_STEP,  Global::tTranslatableStringList() << p_Program->GetLongName());
                    Global::EventObject::Instance().RaiseEvent(EVENT_DM_OVEN_NOT_FIRST_STEP,  Global::tTranslatableStringList() << p_Program->GetLongName(),
                                                               Global::GUI_MSG_BOX);
                    qDebug() << "### CDataProgramListVerifier::OVEN is not selected as first step";
                    VerifiedData = false;
                }
            }

            //Make sure same reagent ID is not used in more than one step for a program
            //Not applicable for special reagents
            if ((ReagentID.at(0) != 'S') && ReagentIDList.count(ReagentID) > 1) {
                if (mp_DataContainer) {
                    m_ErrorsHash.insert(EVENT_DM_REAGENT_USED_IN_MULTIPLE_PROG_STEPS,
                                        Global::tTranslatableStringList() << p_Program->GetLongName()
                                        << mp_DataContainer->GetReagentName(ReagentID));
                    Global::EventObject::Instance().RaiseEvent(EVENT_DM_REAGENT_USED_IN_MULTIPLE_PROG_STEPS,
                                                               Global::tTranslatableStringList() << p_Program->GetLongName()
                                                               << mp_DataContainer->GetReagentName(ReagentID));
                }
                qDebug() << "### CDataProgramListVerifier::Reagent ID:"<<ReagentID<<" used in more than 1 step";
                VerifiedData = false;
            }

            bool IntOk = false;
            //Validate Step ID.
            //ignoring return value of "toInt()"
            (void)(p_ProgramStep->GetStepID().toInt(&IntOk));
            if (!IntOk) {
                m_ErrorsHash.insert(EVENT_DM_INVALID_STEP_ID,
                                    Global::tTranslatableStringList() << p_Program->GetLongName()
                                    << p_ProgramStep->GetStepID());
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_INVALID_STEP_ID,
                                                           Global::tTranslatableStringList() << p_Program->GetLongName()
                                                           << p_ProgramStep->GetStepID(),
                                                           Global::GUI_MSG_BOX);
                qDebug("### invalid StepId");
                VerifiedData = false;
            }

            //verify parallel stations count
            if ((p_ProgramStep->GetNumberOfParallelStations() < PARALLEL_STATIONS_MIN)
                    || (p_ProgramStep->GetNumberOfParallelStations() > PARALLEL_STATIONS_MAX)) {
                m_ErrorsHash.insert(EVENT_DM_INVALID_LEICA_PARALLEL_STN_CNT,
                                    Global::FmtArgs() << p_Program->GetLongName()
                                  << PARALLEL_STATIONS_MIN << PARALLEL_STATIONS_MAX);
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_INVALID_LEICA_PARALLEL_STN_CNT,
                                                           Global::FmtArgs() << p_Program->GetLongName()
                                                         << PARALLEL_STATIONS_MIN << PARALLEL_STATIONS_MAX);
                qDebug("### Parallel stations count invalid");
                VerifiedData = false;
            }
            QStringList SplitList;
            // Verify Intensity
            // if Intensity is not equal to zero then the NumberOfSteps , RangeLow, RangeHigh is considered
            // for the calculation of minduration
            if (p_Program->GetID().at(0).toAscii() == 'L' && p_ProgramStep->GetIntensity() != 0) { // If Leica Program
                if ((p_ProgramStep->GetIntensity() < INTENSITY_MIN)
                        || (p_ProgramStep->GetIntensity() > p_ProgramStep->GetNumberOfSteps())) {
                    m_ErrorsHash.insert(EVENT_DM_STEP_INTENSITY_VALUE_INVALID,
                                        Global::FmtArgs() << p_Program->GetLongName()
                                        << INTENSITY_MIN
                                        << p_ProgramStep->GetNumberOfSteps()
                                        << p_ProgramStep->GetIntensity());
                    Global::EventObject::Instance().RaiseEvent(EVENT_DM_STEP_INTENSITY_VALUE_INVALID,
                                                               Global::FmtArgs() << p_Program->GetLongName()
                                                               << INTENSITY_MIN
                                                               << p_ProgramStep->GetNumberOfSteps()
                                                               << p_ProgramStep->GetIntensity(),
                                                               Global::GUI_MSG_BOX);
                    qDebug("### Invalid Intensity value.Valid range 1-NumberOfSteps");
                    VerifiedData = false;
                }
                if ((p_ProgramStep->GetNumberOfSteps() < NUMBEROFSTEPS_MIN)
                        || (p_ProgramStep->GetNumberOfSteps() > NUMBEROFSTEPS_MAX)) {
                    m_ErrorsHash.insert(EVENT_DM_INTENSITY_STEP_COUNT_INVALID,
                                        Global::tTranslatableStringList() << p_Program->GetLongName()
                                        << p_ProgramStep->GetNumberOfSteps()
                                        << NUMBEROFSTEPS_MIN
                                        << NUMBEROFSTEPS_MAX);
                    Global::EventObject::Instance().RaiseEvent(EVENT_DM_INTENSITY_STEP_COUNT_INVALID,
                                                               Global::tTranslatableStringList() << p_Program->GetLongName()
                                                             << p_ProgramStep->GetNumberOfSteps()
                                                             << NUMBEROFSTEPS_MIN
                                                             << NUMBEROFSTEPS_MAX,
                                                               Global::GUI_MSG_BOX);
                    qDebug("### Invalid Number Of Steps Value.Valid range 2-9");
                    VerifiedData = false;
                }
                //Verify RangeLow duration
                if (p_ProgramStep->GetRangeLowInString().isEmpty()) {
                    m_ErrorsHash.insert(EVENT_DM_STEP_RANGE_LOW_VALUE_EMPTY,
                                        Global::tTranslatableStringList() << p_Program->GetLongName());
                    Global::EventObject::Instance().RaiseEvent(EVENT_DM_STEP_RANGE_LOW_VALUE_EMPTY,
                                                               Global::tTranslatableStringList() << p_Program->GetLongName(),
                                                               Global::GUI_MSG_BOX);
                    qDebug("### Empty Range Low string");
                    VerifiedData = false;
                }
                if (p_ProgramStep->GetRangeLow() == 0) {
                    m_ErrorsHash.insert(EVENT_DM_STEP_RANGE_LOW_ZERO,
                                        Global::tTranslatableStringList() << p_Program->GetLongName());
                    Global::EventObject::Instance().RaiseEvent(EVENT_DM_STEP_RANGE_LOW_ZERO,
                                                               Global::tTranslatableStringList() << p_Program->GetLongName(),
                                                               Global::GUI_MSG_BOX);
                    qDebug("### Range Low is zero");
                    VerifiedData = false;
                }
                SplitList = p_ProgramStep->GetRangeLowInString().split(" ");
                // check the duration format
                CheckDurationFormat(SplitList, VerifiedData, p_Program->GetLongName());
                //Verify RangeHigh duration
                if (p_ProgramStep->GetRangeHighInString().isEmpty()) {
                    m_ErrorsHash.insert(EVENT_DM_STEP_RANGE_HIGH_VALUE_EMPTY,
                                        Global::tTranslatableStringList() << p_Program->GetLongName());
                    Global::EventObject::Instance().RaiseEvent(EVENT_DM_STEP_RANGE_HIGH_VALUE_EMPTY,
                                                               Global::tTranslatableStringList() << p_Program->GetLongName(),
                                                               Global::GUI_MSG_BOX);
                    qDebug("### Empty Range High string");
                    VerifiedData = false;
                }
                if (p_ProgramStep->GetRangeHigh() == 0) {
                    m_ErrorsHash.insert(EVENT_DM_STEP_RANGE_HIGH_ZERO,
                                        Global::tTranslatableStringList() << p_Program->GetLongName());
                    Global::EventObject::Instance().RaiseEvent(EVENT_DM_STEP_RANGE_HIGH_ZERO,
                                                               Global::tTranslatableStringList() << p_Program->GetLongName(),
                                                               Global::GUI_MSG_BOX);
                    qDebug("### Range High is zero");
                    VerifiedData = false;
                }
                SplitList.clear();
                SplitList = p_ProgramStep->GetRangeHighInString().split(" ");
                // check the duration format
                CheckDurationFormat(SplitList, VerifiedData, p_Program->GetLongName());
                LeicaSplReagentCount++; // increament the Leica Reagent count
            } // end of verification of intensity
            else {
                //Verify min duration
                if (p_ProgramStep->GetMinDuration() == "") {
                    m_ErrorsHash.insert(EVENT_DM_STEP_MIN_DURATION_EMPTY,
                                        Global::tTranslatableStringList() << p_Program->GetLongName());
                    Global::EventObject::Instance().RaiseEvent(EVENT_DM_STEP_MIN_DURATION_EMPTY,
                                                               Global::tTranslatableStringList() << p_Program->GetLongName(),
                                                               Global::GUI_MSG_BOX);
                    qDebug("### Empty Min duration string");
                    VerifiedData = false;
                }

                SplitList = p_ProgramStep->GetMinDuration().split(" ");

                // check the duration format
                CheckDurationFormat(SplitList, VerifiedData, p_Program->GetLongName());

                //Verify that step duration is in the range 1s-1d
                if ((p_ProgramStep->GetMinDurationInSeconds() < STEP_DURATION_MIN)
                    || (p_ProgramStep->GetMinDurationInSeconds() > STEP_DURATION_MAX)) {

                    if(!(p_ProgramStep->GetReagentID()=="S7" || p_ProgramStep->GetReagentID() =="S8")){
                        qDebug("### Invalid step duration. Step duration valid range 1s-1d");
                        qDebug()<<"Minimum duration:"<<p_ProgramStep->GetMinDurationInSeconds();
                        qDebug()<<"Step Id"<<p_ProgramStep->GetStepID();
                        m_ErrorsHash.insert(EVENT_DM_STEP_MIN_DURATION_INVALID,
                                            Global::tTranslatableStringList() << p_Program->GetLongName()
                                            <<p_ProgramStep->GetMinDuration());
                        Global::EventObject::Instance().RaiseEvent(EVENT_DM_STEP_MIN_DURATION_INVALID,
                                                                   Global::tTranslatableStringList() << p_Program->GetLongName()
                                                                   <<p_ProgramStep->GetMinDuration(),
                                                                   Global::GUI_MSG_BOX);
                        VerifiedData = false;
                    }
                }
                // Verify Max duration string format
                if(!p_ProgramStep->GetMaxDurationString().endsWith('%')) {
                    qDebug("### Invalid Max duration string");
                    m_ErrorsHash.insert(EVENT_DM_STEP_MAX_DURATION_INVALID,
                                        Global::tTranslatableStringList() << p_Program->GetLongName()
                                        << p_ProgramStep->GetMaxDurationString());
                    Global::EventObject::Instance().RaiseEvent(EVENT_DM_STEP_MAX_DURATION_INVALID,
                                                               Global::tTranslatableStringList() << p_Program->GetLongName()
                                                               << p_ProgramStep->GetMaxDurationString(),
                                                               Global::GUI_MSG_BOX);
                    VerifiedData = false;
                }
                QString Helper = p_ProgramStep->GetMaxDurationString();
                Helper.chop(1);
                bool Ok = false;
                (void)Helper.toInt(&Ok);
                if (!Ok) {
                    m_ErrorsHash.insert(EVENT_DM_STEP_MAX_DURATION_INVALID,
                                        Global::tTranslatableStringList() << p_Program->GetLongName()
                                        << p_ProgramStep->GetMaxDurationString());
                    Global::EventObject::Instance().RaiseEvent(EVENT_DM_STEP_MAX_DURATION_INVALID,
                                                             Global::tTranslatableStringList() << p_Program->GetLongName()
                                                             << p_ProgramStep->GetMaxDurationString(),
                                                               Global::GUI_MSG_BOX);
                    qDebug("### Invalid Max duration string");
                    VerifiedData = false;
                }
                //No need to check constraints for Special reagents since ,
                // they can have values above 100 %
                if (!(p_ProgramStep->GetReagentID().at(0) == 'S')) {
                    if ((p_ProgramStep->GetMaxDurationInPercent() < ZERO_PERCENT)
                            || p_ProgramStep->GetMaxDurationInPercent() > HUNDRED_PERCENT) {
                        m_ErrorsHash.insert(EVENT_DM_STEP_MAX_DURATION_INVALID,
                                            Global::tTranslatableStringList() << p_Program->GetLongName()
                                            << p_ProgramStep->GetMaxDurationString());
                        Global::EventObject::Instance().RaiseEvent(EVENT_DM_STEP_MAX_DURATION_INVALID,
                                                                 Global::tTranslatableStringList() << p_Program->GetLongName()
                                                                 << p_ProgramStep->GetMaxDurationString(),
                                                                   Global::GUI_MSG_BOX);
                        qDebug()<<"### Invalid Max Duration percentage: .Valid range 0-100 %%"<< p_ProgramStep->GetMaxDurationInPercent();
                        VerifiedData = false;
                    }
                }
            }
        }
    }// End of for loop
}

/****************************************************************************/
/*!
 *  \brief  Verifies the program step
 *
 *  \iparam SplitList - String list for the date format
 *  \iparam VerifiedData = verifier flag value
 *  \iparam ErrorDescription = Description of the error
 *
 */
/****************************************************************************/
void CDataProgramListVerifier::CheckDurationFormat(QStringList SplitList, bool &VerifiedData, QString ProgramName)
{
    int Value = 0;
    bool Ok = true;

    foreach(QString SplitString, SplitList)
    {
        if (SplitString.endsWith("d")) {
            SplitString.chop(1);
            Value = SplitString.toInt(&Ok);
            if (!Ok) {
                m_ErrorsHash.insert(EVENT_DM_PROG_INVALID_DURATION_FORMAT,
                                    Global::tTranslatableStringList() << ProgramName);
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_INVALID_DURATION_FORMAT,
                                                           Global::tTranslatableStringList() << ProgramName,
                                                           Global::GUI_MSG_BOX);
                qDebug() << "##### Invalid Day Duration";
                VerifiedData = false;
            }
            if (Value > 1 || Value < 0) {
                m_ErrorsHash.insert(EVENT_DM_PROG_INVALID_DURATION_FORMAT,
                                    Global::tTranslatableStringList() << ProgramName);
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_INVALID_DURATION_FORMAT,
                                                           Global::tTranslatableStringList() << ProgramName,
                                                           Global::GUI_MSG_BOX);
                qDebug() << "##### Invalid Day Duration";
                VerifiedData = false;
            }

        } else if (SplitString.endsWith("h")) {
            SplitString.chop(1);
            Value = SplitString.toInt(&Ok);
            if (!Ok) {
                m_ErrorsHash.insert(EVENT_DM_PROG_INVALID_DURATION_FORMAT,
                                    Global::tTranslatableStringList() << ProgramName);
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_INVALID_DURATION_FORMAT,
                                                           Global::tTranslatableStringList() << ProgramName,
                                                           Global::GUI_MSG_BOX);
                qDebug() << "##### Invalid Hour Duration";
                VerifiedData = false;
            }
            if ( Value > 24 || Value < 0) {
                m_ErrorsHash.insert(EVENT_DM_PROG_INVALID_DURATION_FORMAT,
                                    Global::tTranslatableStringList() << ProgramName);
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_INVALID_DURATION_FORMAT,
                                                           Global::tTranslatableStringList() << ProgramName,
                                                           Global::GUI_MSG_BOX);
                qDebug() << "##### Invalid Hour Duration";
                VerifiedData = false;
            }
        } else if (SplitString.endsWith("m")) {
            SplitString.chop(1);
            Value = SplitString.toInt(&Ok);
            if (!Ok) {
                m_ErrorsHash.insert(EVENT_DM_PROG_INVALID_DURATION_FORMAT,
                                    Global::tTranslatableStringList() << ProgramName);
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_INVALID_DURATION_FORMAT,
                                                           Global::tTranslatableStringList() << ProgramName,
                                                           Global::GUI_MSG_BOX);
                qDebug() << "#####Invalid Minute Duration ";
                VerifiedData = false;
            }
            if (Value > 60 || Value < 0) {
                m_ErrorsHash.insert(EVENT_DM_PROG_INVALID_DURATION_FORMAT,
                                    Global::tTranslatableStringList() << ProgramName);
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_INVALID_DURATION_FORMAT,
                                                           Global::tTranslatableStringList() << ProgramName,
                                                           Global::GUI_MSG_BOX);
                qDebug() << "##### Invalid Minute Duration";
                VerifiedData = false;
            }
        } else if (SplitString.endsWith("s")) {
            SplitString.chop(1);
            Value = SplitString.toInt(&Ok);
            if (!Ok) {
                m_ErrorsHash.insert(EVENT_DM_PROG_INVALID_DURATION_FORMAT,
                                    Global::tTranslatableStringList() << ProgramName);
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_INVALID_DURATION_FORMAT,
                                                           Global::tTranslatableStringList() << ProgramName,
                                                           Global::GUI_MSG_BOX);
                qDebug() << "##### Invalid Seconds Duration";
                VerifiedData = false;
            }
            if (Value > 60 || Value < 0) {
                m_ErrorsHash.insert(EVENT_DM_PROG_INVALID_DURATION_FORMAT,
                                    Global::tTranslatableStringList() << ProgramName);
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_INVALID_DURATION_FORMAT,
                                                           Global::tTranslatableStringList() << ProgramName,
                                                           Global::GUI_MSG_BOX);
                qDebug() << "##### Invalid Seconds Duration";
                VerifiedData = false;
            }

        }

    }//End of foreach
}


/****************************************************************************/
/*!
 *  \brief  Verifies the program steps of User programs
 *
 *  \iparam Program - Pointer to CProgram
 *  \iparam VerifiedData = verifier flag value
 *
 */
/****************************************************************************/
void CDataProgramListVerifier::CheckUserProgramSteps(CProgram* p_Program, bool &VerifiedData)
{
    if (p_Program != NULL) {
        for (qint32 ReagentIDCount = 0; ReagentIDCount < p_Program->GetReagentIDList().count(); ReagentIDCount++) {
            //Validate Reagent ID
            switch(p_Program->GetReagentIDList().value(ReagentIDCount).at(0).toAscii()) {
                // all other cases are covered in the CheckProgramStep function
                case 'L':

                    m_ErrorsHash.insert(EVENT_DM_USER_PROGRAM_CONTAINS_LEICA_REAGENT,
                                        Global::tTranslatableStringList() << p_Program->GetID()
                                        << p_Program->GetReagentIDList().value(ReagentIDCount));
                    Global::EventObject::Instance().RaiseEvent(EVENT_DM_USER_PROGRAM_CONTAINS_LEICA_REAGENT,
                                                               Global::tTranslatableStringList() << p_Program->GetID()
                                                               << p_Program->GetReagentIDList().value(ReagentIDCount));
                    qDebug() << "##### User program" << p_Program->GetID() << "is having the Leica reagent"
                             << p_Program->GetReagentIDList().value(ReagentIDCount);
                    VerifiedData = false;
                    break;
            }
        }// End of for loop
    }
}


/****************************************************************************/
/*!
 *  \brief  Verifies the program step for the Leica program
 *
 *  \iparam Program - Pointer to CProgram
 *  \iparam VerifiedData = verifier flag value
 *
 */
/****************************************************************************/
void CDataProgramListVerifier::CheckLeicaProgramSteps(CProgram* p_Program, bool &VerifiedData)
{
    QStringList LeicaReagentIDList(m_LeicaReagentIDList);
    if (p_Program != NULL) {
        // check Leica reagent exists in the reagent ID list
        if (LeicaReagentIDList.count() > 0) {
            for (qint32 ReagentIDCount = 0; ReagentIDCount < p_Program->GetReagentIDList().count(); ReagentIDCount++) {
                //Validate Reagent ID
                switch(p_Program->GetReagentIDList().value(ReagentIDCount).at(0).toAscii()) {
                // all other cases are covered in the CheckProgramStep function
                case 'L':

                    if (LeicaReagentIDList.contains(p_Program->GetReagentIDList().value(ReagentIDCount))) {
                        m_ErrorsHash.insert(EVENT_DM_LEICA_PROGRAMS_CONTAIN_SAME_LEICA_REAGENT,
                                            Global::tTranslatableStringList()
                                            << p_Program->GetReagentIDList().value(ReagentIDCount)
                                            << p_Program->GetID());
                        Global::EventObject::Instance().RaiseEvent(EVENT_DM_LEICA_PROGRAMS_CONTAIN_SAME_LEICA_REAGENT,
                                                                    Global::tTranslatableStringList()
                                                                    << p_Program->GetReagentIDList().value(ReagentIDCount)
                                                                    << p_Program->GetID());
                        qDebug() << "### Same Leica Reagent ID exists in two programs ";
                        VerifiedData = false;
                    }
                    else {
                        // if the reagent ID does not exist then add to the list
                        m_LeicaReagentIDList << p_Program->GetReagentIDList().value(ReagentIDCount);
                    }

                    break;
                }
            }// End of for loop
        }
        else {
            m_LeicaReagentIDList << p_Program->GetReagentIDList();
        }
    }
}

/****************************************************************************/
/*!
 *  \brief  Gets the last errors which is done by verifier
 *
 *  \return QStringList - List of the errors occured
 */
/****************************************************************************/
ErrorHash_t& CDataProgramListVerifier::GetErrors()
{
    // return the last error which is occured in the verifier
    return m_ErrorsHash;
}

/****************************************************************************/
/*!
 *  \brief  Resets the last error which is done by verifier
 */
/****************************************************************************/
void CDataProgramListVerifier::ResetLastErrors()
{
    m_ErrorsHash.clear();
}

/****************************************************************************/
/*!
 *  \brief  Resets the last error which is done by verifier
 */
/****************************************************************************/
bool CDataProgramListVerifier::IsLocalVerifier()
{
    return true;
}

}  // namespace DataManager

