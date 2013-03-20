/****************************************************************************/
/*! \file ProgramSequenceVerifier.h
 *
 *  \brief ProgramSequence defination.
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
#ifndef PROGRAMSEQUENCEVERIFIER_H
#define PROGRAMSEQUENCEVERIFIER_H

#include <QString>
#include <QXmlStreamReader>

#include "DataManager/Containers/ContainerBase/Include/DataContainerBase.h"
#include "HimalayaDataContainer/Containers/ProgramSequence/Include/ProgramSequenceList.h"
#include "DataManager/Containers/ContainerBase/Include/VerifierInterface.h"

namespace DataManager {
// constraints

const int RESULT_CODE_COUNT = 5;    //!< BLG Result code count
/****************************************************************************/
/*!
 *  \brief  This class implements verifiers for program sequence
 */
/****************************************************************************/
class CProgramSequenceListVerifier : public IVerifierInterface
{
public:
    CProgramSequenceListVerifier();

    bool VerifyData(CDataContainerBase* p_ProgramSequenceList);  // use concrete class for concrete verifier

    //lint -esym(1536,GetErrors )
    ErrorHash_t& GetErrors();

    void ResetLastErrors();

    bool IsLocalVerifier();

    /****************************************************************************/
    /*!
     *  \brief  Destructor
     */
    /****************************************************************************/
    virtual ~CProgramSequenceListVerifier() {}

private:
    CProgramSequenceList* mp_DPSL;//!< Pointer to program Sequence List
    ErrorHash_t m_ErrorsHash;          //!< To store Error ID and any arguments associated
};

} // namespace DataManager

#endif // PROGRAMSEQUENCEVERIFIER_H
