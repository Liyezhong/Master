
/****************************************************************************/
/*! \file DataReagentListVerifier.cpp
 *
 *  \brief DataReagentListVerifier Implementation.
 *
 *   $Version: $ 0.2
 *   $Date:    $ 2012-04-28, 2013-01-17
 *   $Author:  $ Vikram MK, Swati Tiwari
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2012 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include <QDebug>
#include <QFile>

#include "HimalayaDataContainer/Containers/Reagents/Include/DataReagentListVerifier.h"
#include "HimalayaDataContainer/Containers/Reagents/Include/DataReagentList.h"
#include "HimalayaDataContainer/Containers/Reagents/Include/Reagent.h"

namespace DataManager {

CDataReagentListVerifier::CDataReagentListVerifier() : mp_DRL(NULL)
{

}

/****************************************************************************/
/*!
 *  \brief Verifies the Constraints of Particular Reagent list
 *
 *  \iparam p_DataReagentList = Instance of the CDateReagentList class
 *
 *  \return true or false
 */
/****************************************************************************/
bool CDataReagentListVerifier::VerifyData(CDataContainerBase* p_DataReagentList)
{
    /** todo **/
    return true;

    // assign pointer to member variable
    mp_DRL = static_cast<CDataReagentList*>(p_DataReagentList);
    // check constraints
    // check max number of reagents (100)
    if (mp_DRL->GetNumberOfReagents() > MAX_REAGENTS) {
        qDebug() << "### Too many reagents (max 100 allowed): " << mp_DRL->GetNumberOfReagents();
        m_ErrorsHash.insert(EVENT_DM_REAGENT_EXCEEDS_LIMIT, Global::tTranslatableStringList() << "");
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_REAGENT_EXCEEDS_LIMIT);
        return false;
    }

    // by default make the verification flag to true
    bool VerifiedData = true;

    // check content of each reagent
    for (int i=0;i< mp_DRL->GetNumberOfReagents();i++) {
        CReagent Reagent;
        if (mp_DRL->GetReagent(i, Reagent)) {
            QString ReagentID;
            ReagentID = Reagent.GetReagentID();
            if (mp_DRL->GetReagentType(ReagentID) == UNDEFINED_REAGENT_TYPE) {
                qDebug() << "### Reagent ID doesn't start with L,U: " << ReagentID;
                m_ErrorsHash.insert(EVENT_DM_REAGENT_INVALID_PREFIX, Global::tTranslatableStringList() << Reagent.GetReagentName());
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_REAGENT_INVALID_PREFIX, Global::tTranslatableStringList() << Reagent.GetReagentName(), true);
                VerifiedData = false;
            }
            QString TempString;
            TempString = Reagent.GetReagentName();
            if ((TempString.size() > MAX_CHARACTERS_NAME) || (TempString.size() < 0)) {
                qDebug() << "### Reagent:" << ReagentID <<"Name is too long (max 32 Characters) " << Reagent.GetReagentName();
                m_ErrorsHash.insert(EVENT_DM_REAGENT_NAME_LENGTH_CHECK_FAILED, Global::tTranslatableStringList() << Reagent.GetReagentName());
                Global::EventObject::Instance().RaiseEvent(EVENT_DM_REAGENT_NAME_LENGTH_CHECK_FAILED, Global::tTranslatableStringList() << Reagent.GetReagentName(), true);
                VerifiedData = false;
            }
/*            if (Reagent.IsLeicaReagent() || Reagent.IsUserReagent()) {
                if ((Reagent.GetMaxCassettes() > MAX_CASSETTES_TOREPLACE_REAGENT) ||
                        (Reagent.GetMaxCassettes() < 0)) {
                    qDebug() << "### Reagent:" << ReagentID <<"Max Cassettes Reached Replace Reagent" << Reagent.GetMaxCassettes();
                    m_ErrorsHash.insert(EVENT_DM_REAGENT_MAXSLIDES_EXCEEDS_LIMIT, Global::tTranslatableStringList() << "");
                    Global::EventObject::Instance().RaiseEvent(EVENT_DM_REAGENT_MAXSLIDES_EXCEEDS_LIMIT, Global::tTranslatableStringList() << Reagent.GetReagentName(), true);
                    VerifiedData = false;
                }

            }*/ // end of Leica and User comparison
        }
    } // end of for loop
    return VerifiedData;
}


/****************************************************************************/
/*!
 *  \brief  Gets the last errors which is done by verifier
 *
 *  \return QStringList - List of the errors occured
 */
/****************************************************************************/
ErrorHash_t& CDataReagentListVerifier::GetErrors()
{
    // return the last error which is occured in the verifier
    return m_ErrorsHash;
}

/****************************************************************************/
/*!
 *  \brief  Resets the last error which is done by verifier
 */
/****************************************************************************/
void CDataReagentListVerifier::ResetLastErrors()
{
    m_ErrorsHash.clear();
}

/****************************************************************************/
/*!
 *  \brief  Resets the last error which is done by verifier
 */
/****************************************************************************/
bool CDataReagentListVerifier::IsLocalVerifier()
{
    return true;
}

}  // namespace DataManager

