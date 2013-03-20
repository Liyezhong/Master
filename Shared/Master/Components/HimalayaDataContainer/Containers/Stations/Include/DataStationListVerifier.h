/****************************************************************************/
/*! \file DataStationListVerifier.h
 *
 *  \brief Definition for DataStationListVerfier class.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2012-05-07
 *   $Author:  $ Vikram MK
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
#ifndef DATAMANAGER_DATASTATIONLISTVERIFIER_H
#define DATAMANAGER_DATASTATIONLISTVERIFIER_H

#include <QString>
#include <QXmlStreamReader>

#include "DataManager/Containers/ContainerBase/Include/DataContainerBase.h"
#include "HimalayaDataContainer/Containers/Stations/Include/DataStationList.h"
#include "DataManager/Containers/ContainerBase/Include/VerifierInterface.h"

namespace DataManager {

// These constants are used to check in local verifiers
const int  MAX_SIZEOF_STATIONID = 3;            //!< maximum size of the Station ID

/****************************************************************************/
/*!
 *  \brief CDataStationListVerifier class Inherited from IVerifierInterface
 */
/****************************************************************************/
class CDataStationListVerifier : public IVerifierInterface

{
public:
    CDataStationListVerifier();

    bool VerifyData(CDataContainerBase* p_DataStationList);  // use concrete class for concrete verifier
    //lint -esym(1536,GetErrors )
    ErrorHash_t &GetErrors();

    void ResetLastErrors();

    bool IsLocalVerifier();

    /****************************************************************************/
    /*!
     *  \brief  Destructor
     */
    /****************************************************************************/
    virtual ~CDataStationListVerifier() {}
private:
    CDashboardDataStationList* mp_DSL;                //!< Station Data Container
    ErrorHash_t m_ErrorsHash;          //!< To store Error ID and any arguments associated

};

} // namespace DataManager

#endif // DATAMANAGER_DATASTATIONLISTVERIFIER_H
