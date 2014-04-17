/****************************************************************************/
/*! \file   ProgramSettingsVerifier.h
 *
 *  \brief  .
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
#ifndef DATAMANAGER_PROGRAMSETTINGSVERIFIER_H
#define DATAMANAGER_PROGRAMSETTINGSVERIFIER_H

#include <QString>
#include <QXmlStreamReader>

#include "DataManager/Containers/ContainerBase/Include/DataContainerBase.h"
#include "DataManager/Containers/ContainerBase/Include/VerifierInterface.h"
#include "HimalayaDataContainer/Containers/ProgramSettings/Include/ProgramSettings.h"

namespace DataManager {

#define MAX_SPEED_PROFILE   4
#define MAX_POSITION_VALUE  4500
#define MAX_OFFSET_VALUE    500

/****************************************************************************/
/*!
 *  \brief  This class implements verifiers for Parameter
 */
/****************************************************************************/
class CProgramSettingsVerifier : public IVerifierInterface
{
public:
    CProgramSettingsVerifier();

    bool VerifyData(CDataContainerBase* p_ParameterList);  // use concrete class for concrete verifier
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetSyncObject
     *
     *  \return from GetSyncObject
     */
    /****************************************************************************/
    bool GetSyncObject(QReadWriteLock* p_ReadWriteLock);

    /*! \todo complete implementation*/
    ErrorMap_t& GetErrors();

    void ResetErrors();

    bool IsLocalVerifier();

    /****************************************************************************/
    /*!
     *  \brief  Destructor
     */
    /****************************************************************************/
    virtual ~CProgramSettingsVerifier() {}

private:
    ErrorMap_t m_ErrorsHash;           //!< To store Error Key and any arguments associated
};

} // namespace DataManager

#endif // DATAMANAGER_PROGRAMSETTINGSVERIFIER_H
