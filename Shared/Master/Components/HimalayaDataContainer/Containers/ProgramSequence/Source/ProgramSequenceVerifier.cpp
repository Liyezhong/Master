/****************************************************************************/
/*! \file ProgramSequenceVerifier.cpp
 *
 *  \brief ProgramSequence Verifier implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-12-26
 *   $Author:  $ N.Kamath
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

#include "Global/Include/EventObject.h"
#include "HimalayaDataContainer/Containers/ProgramSequence/Include/ProgramSequenceVerifier.h"
#include "HimalayaDataContainer/Containers/ProgramSequence/Include/ProgramSequenceList.h"
#include "HimalayaDataContainer/Containers/ProgramSequence/Include/ProgramSequence.h"
#include "HimalayaDataContainer/Helper/Include/HimalayaDataManagerEventCodes.h"

namespace DataManager {
const int PROGRAMS_MAX = 50;
/****************************************************************************/
/*!
 *  \brief Constructor
 */
/****************************************************************************/

CProgramSequenceListVerifier::CProgramSequenceListVerifier() : mp_DPSL(NULL)
{
}

//bool CDataProgramListVerifier::VerifyData(CDataContainerBase* p_DataProgramList)

/****************************************************************************/
/*!
 *  \brief Function to verify the data present in xml

 *  \iparam p_ProgramSequenceList
 *
 *  \return Changed something (true) or not (false)
 */
/****************************************************************************/
bool CProgramSequenceListVerifier::VerifyData(CDataContainerBase *p_ProgramSequenceList)
{
    // assign pointer to member variable
    mp_DPSL = static_cast<CProgramSequenceList*>(p_ProgramSequenceList);

    // check max number of ProgramSequence (50)

    if(mp_DPSL->GetNumberOfPrograms() > PROGRAMS_MAX) {
        qDebug() << "### Too many program sequence (max 50 allowed): " << mp_DPSL->GetNumberOfPrograms();
        m_ErrorsHash.insert(EVENT_DM_PROGSEQ_MAX_COUNT_REACHED, Global::tTranslatableStringList() << "");
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROGSEQ_MAX_COUNT_REACHED);
        return false;
    }

    bool bResult= true;

    //TODO check constraints
    //Verify program ID
    for (qint32 I = 0; I < mp_DPSL->GetNumberOfPrograms(); I++) {
        CProgramSequence ProgSequence;
        if (mp_DPSL->GetProgramSequenceStep(I, ProgSequence)) {
            if ((ProgSequence.GetID().at(0) != 'U') &&(ProgSequence.GetID().at(0) != 'L')) {
                qDebug()<<"Invalid Program Id"<<ProgSequence.GetID();
                m_ErrorsHash.insert(EVENT_DM_PROGSEQ_INVALID_PREFIX, Global::tTranslatableStringList() << ProgSequence.GetID());
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROGSEQ_INVALID_PREFIX, Global::tTranslatableStringList() << ProgSequence.GetID(), true);
                bResult = false;
            }
            bool Ok = false;
            int ProgramId = ProgSequence.GetID().mid(1).toInt(&Ok);
            qDebug()<<"#####Program Id"<<ProgramId << "OK"<<Ok;
            if ((!Ok) && (!(ProgramId >= 1))) {
                qDebug("### Invalid program Id.Program ID should be greater than 0");
                m_ErrorsHash.insert(EVENT_DM_PROGSEQ_INVALID_PREFIX, Global::tTranslatableStringList() << ProgSequence.GetID());
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROGSEQ_INVALID_PREFIX, Global::tTranslatableStringList() << ProgSequence.GetID(), true);
                bResult = false;
            }

            QStringList BLGResult = ProgSequence.GetStartCapableResult().split(",");
            if (BLGResult.count() > (RESULT_CODE_COUNT - 1)) {
                qDebug("### Can't have more than 5 Error codes");
                m_ErrorsHash.insert(EVENT_DM_PROGSEQ_INVALID_BLGRESULT, Global::tTranslatableStringList() << "");
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROGSEQ_INVALID_BLGRESULT, Global::tTranslatableStringList() << "", true);
                bResult = false;
            }

            if ((BLGResult.contains("0")) && (BLGResult.count() > 1)) {
                qDebug()<<"### BLG Result can be either success or failure, not both. BLG result string:"<< BLGResult;
                m_ErrorsHash.insert(EVENT_DM_PROGSEQ_INVALID_BLGRESULT, Global::tTranslatableStringList() << "");
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROGSEQ_INVALID_BLGRESULT, Global::tTranslatableStringList() << "", true);
                bResult = false;
            }

            QStringList BLGResultCode;
            for (qint32 ResultCode = -1; ResultCode < 9; ResultCode++) {
                BLGResultCode << QString::number(ResultCode);
            }
            foreach(QString SplitString, BLGResult) {
                if (!(BLGResultCode.contains(SplitString))) {
                    qDebug()<<"### Invalid BLG result code"<< SplitString;
                    m_ErrorsHash.insert(EVENT_DM_PROGSEQ_INVALID_BLGRESULT_CODE, Global::tTranslatableStringList() << SplitString);
                    Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROGSEQ_INVALID_BLGRESULT_CODE, Global::tTranslatableStringList() << SplitString, true);
                    bResult = false;
                }
            }
        }
    }
    return bResult;
}

/****************************************************************************/
/*!
 *  \brief  Gets the last errors which is done by verifier
 *
 *  \return QStringList - List of the errors occured
 */
/****************************************************************************/
ErrorHash_t& CProgramSequenceListVerifier::GetErrors()
{
    // return the last error which is occured in the verifier
    return m_ErrorsHash;
}

/****************************************************************************/
/*!
 *  \brief  Resets the last error which is done by verifier
 */
/****************************************************************************/
void CProgramSequenceListVerifier::ResetLastErrors()
{
    m_ErrorsHash.clear();
}

/****************************************************************************/
/*!
 *  \brief  Resets the last error which is done by verifier
 */
/****************************************************************************/
bool CProgramSequenceListVerifier::IsLocalVerifier()
{
    return true;
}

}  // namespace DataManager


