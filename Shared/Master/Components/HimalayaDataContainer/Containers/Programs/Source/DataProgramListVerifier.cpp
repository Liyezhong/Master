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
    // empty the string
    m_LeicaReagentIDList.clear();
    for (qint32 I = 0; I < mp_DPL->GetNumberOfPrograms(); I++) {
        p_Program = mp_DPL->GetProgram(I);
        if (p_Program) {
            //Verify program ID
            if ((p_Program->GetID().at(0) != 'U') && (p_Program->GetID().at(0) != 'L') && (p_Program->GetID().at(0) != 'C')) {
                qDebug()<<"Invalid Program Id"<<p_Program->GetID();
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_INVALID_PROGRAM_ID,
                                                           Global::tTranslatableStringList() << p_Program->GetID()
                                                           << p_Program->GetName(),
                                                           Global::GUI_MSG_BOX);
                m_ErrorsHash.insert(EVENT_DM_INVALID_PROGRAM_ID,
                                   Global::tTranslatableStringList() << p_Program->GetID()
                                   << p_Program->GetName());
                VerifiedData = false;
            }
            bool Ok = false;
            int ProgramId = p_Program->GetID().mid(1).toInt(&Ok);
            if ((!Ok) && (!(ProgramId >= 1))) {
                qDebug()<<"### Invalid program Id:"<<p_Program->GetID();
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_INVALID_PROGRAM_ID,
                                                           Global::tTranslatableStringList() << p_Program->GetID()
                                                           << p_Program->GetName(),
                                                           Global::GUI_MSG_BOX);
                m_ErrorsHash.insert(EVENT_DM_INVALID_PROGRAM_ID,
                                   Global::tTranslatableStringList() << p_Program->GetID()
                                   << p_Program->GetName());
                VerifiedData = false;
            }

            //Verify short name & long name constraints
            if ((p_Program->GetName().length() < NAME_LENGTH_MIN) ||
                    (p_Program->GetName().length() > NAME_LENGTH_MAX) ||
                    p_Program->GetName().contains("LEICA",Qt::CaseInsensitive)) {
                qDebug()<<"###Short name length check failed"<<p_Program->GetName();
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROGSHORT_NAME_LENGTH_CHECK_FAILED,
                                                           Global::tTranslatableStringList() << p_Program->GetName()
                                                           << NAME_LENGTH_MIN << NAME_LENGTH_MAX);
                VerifiedData = false;
            }


            //check no of steps
            if (p_Program->GetNumberOfSteps() > PROGRAM_STEPS_MAX) {
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_STEP_COUNT_EXCEEDS_LIMIT,
                                                           Global::tTranslatableStringList() <<p_Program->GetName()
                                                           << p_Program->GetNumberOfSteps(),
                                                           Global::GUI_MSG_BOX);
                qDebug("### Program Steps should be less than 50");
                VerifiedData = false;
            }

            CheckProgramStep(p_Program, VerifiedData);
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
void CDataProgramListVerifier::CheckProgramStep(CProgram* p_Program, bool &VerifiedData)
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

            QStringList SplitList;
            SplitList = p_ProgramStep->GetDuration().split(" ");

            // check the duration format
            CheckDurationFormat(SplitList, VerifiedData, p_Program->GetName());

        } // End of if
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

