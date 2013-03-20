/****************************************************************************/
/*! \file SpecialVerifierGroupB.h
 *
 *  \brief Definition file for class CSpecialVerifierGroupB.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-06-15
 *  $Author:    $ Raju
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

#ifndef DATAMANAGER_SPECIALVERIFIERGROUPB_H
#define DATAMANAGER_SPECIALVERIFIERGROUPB_H

#include <QString>
#include <QXmlStreamReader>

#include "DataManager/Containers/ContainerBase/Include/VerifierInterface.h"
#include "DataManager/Containers/ContainerBase/Include/DataContainerBase.h"
#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramList.h"
#include "HimalayaDataContainer/Containers/ProgramSequence/Include/ProgramSequenceList.h"

namespace DataManager {

/****************************************************************************/
/*!
 *  \brief  This class implements Special verifier Group B
 */
/****************************************************************************/
// need to add error interface to special verifier
class CSpecialVerifierGroupB : public IVerifierInterface
{
public:

    // instead of sending all the arguments please put all class objects in a Hash table with a Key
    // so that, in feature if it adds up new arguments and function will not be changed.
    CSpecialVerifierGroupB(CDataProgramList* p_DataProgramList, CProgramSequenceList* p_ProgramSeqList);

    bool VerifyData(CDataContainerBase* p_ContainerBase);

    ErrorHash_t& GetErrors() { return m_ErrorsHash;}

    void ResetLastErrors(){m_ErrorsHash.clear();}

    bool IsLocalVerifier(){return false;}

private:
    CSpecialVerifierGroupB();
    CSpecialVerifierGroupB(const CSpecialVerifierGroupB&); ///< Not implemented
    CDataProgramList* mp_DProgramList; ///< Container for the Program list
    CProgramSequenceList* mp_PSequenceList; ///< Container for the Program Sequence list
    ErrorHash_t m_ErrorsHash;          //!< To store Error ID and any arguments associated
    bool CheckData();
};

} // namespace DataManager
#endif // DATAMANAGER_SPECIALVERIFIERGROUPB_H
