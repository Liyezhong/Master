/****************************************************************************/
/*! \file LeicaReagentBottle.cpp
 *
 *  \brief LeicaReagentBottle class implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2012-08-09
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

#include <QDebug>
#include <QString>

#include "RMS/Include/LeicaReagentBottle.h"

namespace HimalayaRMS {


/****************************************************************************/
/*!
 *  \brief Default Constructor for Leica Reagent Bottle class.
*/
/****************************************************************************/
CLeicaReagentBottle::CLeicaReagentBottle() :  m_TypeID("0"), m_ReagentID("-1"), m_StationID("-1"),
    m_LotNumber(0), m_MaxSlidesKit(3000)
{  
}
/****************************************************************************/
/*!
 *  \brief Copy Constructor
 *
 *  \iparam LeicaReagentBottle = Instance of the LeicaReagentBottle class
 *
 *  \return
 */
/****************************************************************************/
CLeicaReagentBottle::CLeicaReagentBottle(const CLeicaReagentBottle& LeicaReagentBottle)
{
    CLeicaReagentBottle* p_TempReagentBottle = const_cast<CLeicaReagentBottle*>(&LeicaReagentBottle);

    *this = *p_TempReagentBottle;
}
/****************************************************************************/
/*!
 *  \brief Assignment Operator which copies from rhs to lhs.
 *
 *  \iparam SourceLeicaReagentBottle = CLeicaReagentBottle class object
 *
 *  \return CLeicaReagentBottle Class Object
 */
/****************************************************************************/
CLeicaReagentBottle& CLeicaReagentBottle::operator=(const CLeicaReagentBottle& SourceLeicaReagentBottle)
{
    // make sure not same object
    if (this != &SourceLeicaReagentBottle)
    {
        this->m_TypeID = SourceLeicaReagentBottle.m_TypeID;
        this->m_ReagentID = SourceLeicaReagentBottle.m_ReagentID;
        this->m_StationID = SourceLeicaReagentBottle.m_StationID;
        this->m_LotNumber = SourceLeicaReagentBottle.m_LotNumber;
        this->m_MaxSlidesKit = SourceLeicaReagentBottle.m_MaxSlidesKit;
    }
    return *this;
}
/****************************************************************************/
/*!
 *  \brief Appends the 'L' to the Reagent ID.
 *
 *  \iparam ReagentNumber = Reagent Number from RFID
 *
 *  \return
 */
/****************************************************************************/
void CLeicaReagentBottle::SetReagentID(int ReagentNumber)
{
    (void)m_ReagentID.setNum(ReagentNumber);
    (void)m_ReagentID.prepend("L");
}
/****************************************************************************/
/*!
 *  \brief DeSerialises the bottle Information and Stores it.
 *
 *  \iparam KitTagValue = RFID Tage Value of bottle
 *
 *  \return true - success
 */
/****************************************************************************/
bool CLeicaReagentBottle::DeserializeBottleContent(const QByteArray *KitTagValue)
{
    //Type id has been verified store it
    unsigned int Tempbyte = 0;
    Tempbyte = (KitTagValue->at(0) & 0xFF);
    SetTypeID(Tempbyte);

    // Extract reagent id from the tag - 2 bytes
    Tempbyte = (KitTagValue->at(1) & 0xFF);
    Tempbyte = Tempbyte << 8 | (KitTagValue->at(2) & 0xFF);
    SetReagentID(Tempbyte);

    // Extract lot number
    Tempbyte = (KitTagValue->at(3) & 0xFF);
    Tempbyte = Tempbyte << 8 | (KitTagValue->at(4) & 0xFF);
    Tempbyte = Tempbyte << 8 | (KitTagValue->at(5) & 0xFF);
    Tempbyte = Tempbyte << 8 | (KitTagValue->at(6) & 0xFF);
    SetLotNumber(Tempbyte);

    // Extract max slides kit
    Tempbyte = (KitTagValue->at(7) & 0xFF);
    Tempbyte = Tempbyte << 8 | (KitTagValue->at(8) & 0xFF);
    SetMaxSlidesKit(Tempbyte);

    return true;
}

}  // namespace HimalayaRMS
