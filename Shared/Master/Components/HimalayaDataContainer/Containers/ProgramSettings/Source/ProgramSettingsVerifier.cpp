/****************************************************************************/
/*! \file   ProgramSettingsVerifier.cpp
 *
 *  \brief  Implementation of ProgramSettingsVerifier
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-04-03
 *   $Author:  $ L. Yang
 *
 *  \b Company:
 *
 *       Leica Biosystems SH. CN.
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

#include "HimalayaDataContainer/Containers/ProgramSettings/Include/ProgramSettingsVerifier.h"

namespace DataManager {

/****************************************************************************/
/*!
 *  \brief Constructor
 */
/****************************************************************************/
CProgramSettingsVerifier::CProgramSettingsVerifier()
{
}

/****************************************************************************/
/*!
 *  \brief Function to verify the data present in xml

 *  \iparam p_ParameterList
 *
 *  \return Changed something (true) or not (false)
 */
/****************************************************************************/
bool CProgramSettingsVerifier::VerifyData(CDataContainerBase *p_ParameterList)
{
    // by default make the verification flag to true
    bool VerifiedData = true;

    CProgramSettings* p_ProcSettings = static_cast<CProgramSettings*>(p_ParameterList);;

    if (!p_ProcSettings) {
//        Global::EventObject::Instance().RaiseEvent(EVENT_DM_NULL_PTR);
        return false;
    }

    // check constraints

    return VerifiedData;
}

/****************************************************************************/
/*!
 *  \brief  Gets the last errors which is done by verifier
 *
 *  \return QStringList - List of the errors occured
 */
/****************************************************************************/
ErrorMap_t &CProgramSettingsVerifier::GetErrors()
{
    // return the last error which is occured in the verifier
    //lint -esym(429, m_ErrorsHash)
    //lint -esym(1536, m_ErrorsHash)
    return m_ErrorsHash;
}

/****************************************************************************/
/*!
 *  \brief  Resets the last error which is done by verifier
 */
/****************************************************************************/
void CProgramSettingsVerifier::ResetErrors()
{
    m_ErrorsHash.clear();
}

/****************************************************************************/
/*!
 *  \brief  Resets the last error which is done by verifier
 *  \return bool
 */
/****************************************************************************/
bool CProgramSettingsVerifier::IsLocalVerifier()
{
    return true;
}


}  // namespace DataManager


