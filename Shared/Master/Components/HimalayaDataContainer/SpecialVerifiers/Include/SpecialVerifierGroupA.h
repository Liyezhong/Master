/****************************************************************************/
/*! \file SpecialVerifierGroupA.h
 *
 *  \brief Definition file for class CSpecialVerifierGroupA.
 *
 *  $Version:   $ 0.2
 *  $Date:      $ 2011-08-01, 2012-05-03
 *  $Author:    $ F. Toth, Raju
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

#ifndef DATAMANAGER_SPECIALVERIFIERGROUPA_H
#define DATAMANAGER_SPECIALVERIFIERGROUPA_H

#include <QString>
#include <QXmlStreamReader>
#include "QStringList"

#include "DataManager/Containers/ContainerBase/Include/VerifierInterface.h"
#include "DataManager/Containers/ContainerBase/Include/DataContainerBase.h"
#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramList.h"
#include "HimalayaDataContainer/Containers/Reagents/Include/DataReagentList.h"
#include "HimalayaDataContainer/Containers/Stations/Include/DataStationList.h"

namespace DataManager {

/****************************************************************************/
/*!
 *  \brief  This class implements Special verifier Group A
 */
/****************************************************************************/
// need to add error interface to special verifier
class CSpecialVerifierGroupA : public IVerifierInterface
{
public:
    // instead of sending all the arguments please put all class objects in a Hash table with a Key
    // so that, in feature if it adds up new arguments and function will not be changed.
    CSpecialVerifierGroupA(CDataProgramList* p_DataProgramList, CDataReagentList* p_DataReagentList
                           , CDashboardDataStationList* p_StationList);

    bool VerifyData(CDataContainerBase* p_ContainerBase);

    ErrorHash_t& GetErrors() { return m_ErrorsHash;}


    void ResetLastErrors(){m_ErrorsHash.clear();}

    bool IsLocalVerifier(){return false;}

private:
    CSpecialVerifierGroupA();
    CSpecialVerifierGroupA(const CSpecialVerifierGroupA&); ///< Not implemented
    CDataProgramList* mp_DProgramList; ///< Container for the Program list
    CDataReagentList* mp_DReagentList; ///< Container for the Reagent list
    CDashboardDataStationList* mp_DStationList; ///< Container for the Station list
    ErrorHash_t m_ErrorsHash;          //!< To store Error ID and any arguments associated
    bool CheckData();
};

} // namespace DataManager

#endif // DATAMANAGER_SPECIALVERIFIERGROUPA_H
