/****************************************************************************/
/*! \file ServiceParametersVerifier.cpp
 *
 *  \brief CServiceParametersVerifier class implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-06-26
 *   $Author:  $ Soumya D
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

#include "ServiceDataManager/Include/ServiceParametersVerifier.h"

//lint -e613
//lint -e1536

namespace DataManager
{

/****************************************************************************/
/*!
 *  \brief Default Constructor
 */
/****************************************************************************/
CServiceParametersVerifier::CServiceParametersVerifier(): mp_ServiceParameters(NULL)
{
}

/****************************************************************************/
/*!
 *  \brief Verifies the data present in the service parameters against constraints.
 *  \iparam p_ServiceParameters
 *  \return true - verify success , false - verify failure
 */
/****************************************************************************/
bool CServiceParametersVerifier::VerifyData(CDataContainerBase *p_ServiceParameters)
{
    bool VerifiedData = true;

    mp_ServiceParameters = static_cast<CServiceParameters*>(p_ServiceParameters);

    if (mp_ServiceParameters == NULL) {
        qDebug() << "Service parameters Data container is empty" << endl;
        VerifiedData = false;
    }

    int Version = mp_ServiceParameters->GetVersion();
    if (Version != 1) {
        qDebug() << "Service parameters version is invalid." << endl;
        VerifiedData = false;
    }

    QString ProxyIPAddress = mp_ServiceParameters->GetProxyIPAddress();
    if (ProxyIPAddress.isEmpty()) {
        qDebug() << "Proxy IP address is NULL." << endl;
        VerifiedData = false;
    }

    return VerifiedData;
}

/****************************************************************************/
/*!
 *  \brief  Gets the last errors which is done by verifier
 *
 *  \return QStringList - List of the errors occured
 */
/****************************************************************************/
ErrorMap_t& CServiceParametersVerifier::GetErrors()
{
    //lint -efunc(1536, GetErrors)
    //lint -esym(1536, m_ErrorMap)
    // return the last error which is occured in the verifier
    return m_ErrorMap;
}

/****************************************************************************/
/*!
 *  \brief  Resets the last error which is done by verifier
 */
/****************************************************************************/
void CServiceParametersVerifier::ResetErrors()
{
    m_ErrorMap.clear();
}

/****************************************************************************/
/*!
 *  \brief  Resets the last error which is done by verifier
 *  \return true
 */
/****************************************************************************/
bool CServiceParametersVerifier::IsLocalVerifier()
{
    return true;
}

}   // end of namespace DataManager

