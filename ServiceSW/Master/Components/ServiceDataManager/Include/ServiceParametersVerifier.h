/****************************************************************************/
/*! \file ServiceParametersVerifier.h
 *
 *  \brief Definition for CServiceParametersVerifier class.
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
#ifndef DATAMANAGER_SERVICEPARAMETERSVERIFIER_H
#define DATAMANAGER_SERVICEPARAMETERSVERIFIER_H

#include <QStringList>
#include <QDebug>

#include "ServiceDataManager/Include/ServiceParameters.h"
#include "DataManager/Containers/ContainerBase/Include/VerifierInterface.h"

namespace DataManager
{

/****************************************************************************/
/*!
 *  \brief  This class implements Service parameters verifier
 */
/****************************************************************************/
class CServiceParametersVerifier :public IVerifierInterface
{
public:
    CServiceParametersVerifier();
    /****************************************************************************/
    /*!
     *  \brief  Destructor
     */
    /****************************************************************************/
    virtual ~CServiceParametersVerifier() {}

    bool VerifyData(CDataContainerBase* p_ServiceParameters);
    ErrorMap_t &GetErrors();

    void ResetErrors();

    bool IsLocalVerifier();

private:
    CServiceParameters *mp_ServiceParameters;  //!< Service parameters Container
    ErrorMap_t m_ErrorMap;                     //!< To store Error ID and any arguments associated

};

}
#endif // DATAMANAGER_SERVICEPARAMETERSVERIFIER_H
