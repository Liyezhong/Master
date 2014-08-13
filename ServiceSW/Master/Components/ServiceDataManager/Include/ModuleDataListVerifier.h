/****************************************************************************/
/*! \file ServiceDataManager/Include/ModuleDataListVerifier.h
 *
 *  \brief Definition for CModuleDataListVerifier class.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-01-08
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
#ifndef SERVICE_DATAMANAGER_MODULEDATALISTVERIFIER_H
#define SERVICE_DATAMANAGER_MODULEDATALISTVERIFIER_H

#include <QStringList>
#include <QDebug>

#include "ServiceDataManager/Include/ModuleDataList.h"
#include "DataManager/Containers/ContainerBase/Include/VerifierInterface.h"

namespace ServiceDataManager
{

/****************************************************************************/
/*!
 *  \brief  This class implements Module List verifier
 */
/****************************************************************************/
class CModuleDataListVerifier :public DataManager::IVerifierInterface
{
public:
    /****************************************************************************/
    /*!
     *  \brief Default Constructor
     */
    /****************************************************************************/
    CModuleDataListVerifier();

    /****************************************************************************/
    /*!
     *  \brief  Destructor
     */
    /****************************************************************************/
    virtual ~CModuleDataListVerifier();

    /****************************************************************************/
    /*!
     *  \brief Verifies the data present in the module list against constraints.
     *  \iparam p_ModuleDataList
     *  \return true - verify success , false - verify failure
     */
    /****************************************************************************/
    bool VerifyData(DataManager::CDataContainerBase* p_ModuleDataList);

    /****************************************************************************/
    /*!
     *  \brief  Gets the last errors which is done by verifier
     *
     *  \return QStringList - List of the errors occured
     */
    /****************************************************************************/
    ErrorMap_t &GetErrors();

    /****************************************************************************/
    /*!
     *  \brief  Resets the last error which is done by verifier
     */
    /****************************************************************************/
    void ResetErrors();

    /****************************************************************************/
    /*!
     *  \brief  Resets the last error which is done by verifier
     *  \return true
     */
    /****************************************************************************/
    bool IsLocalVerifier();

private:
    CModuleDataList *mp_MDL;  //!< Module List Container
    ErrorMap_t m_ErrorMap; //!< To store Error ID and any arguments associated

    /****************************************************************************/
    /*!
     *  \brief Disable copy and assignment operator.
     *
     */
    /****************************************************************************/
    Q_DISABLE_COPY(CModuleDataListVerifier)

};

}
#endif // DATAMANAGER_MODULEDATALISTVERIFIER_H
