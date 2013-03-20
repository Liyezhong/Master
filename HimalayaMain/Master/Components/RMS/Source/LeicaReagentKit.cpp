/****************************************************************************/
/*! \file LeicaReagentKit.cpp
 *
 *  \brief LeicaReagentKit class implementation.
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

#include "RMS/Include/LeicaReagentKit.h"

namespace HimalayaRMS {


/****************************************************************************/
/*!
 *  \brief Default Constructor for Leica Reagent Bottle class.
*/
/****************************************************************************/
CLeicaReagentKit::CLeicaReagentKit() :  m_TypeID("0"), m_ProgramID("-1"),  m_ReagentBottleCount(0),
    m_BestBeforeDate("1900-1-1"), m_OpenShellLifeTime(0), m_MaxSlides(3000)
{
    m_ReagentLotNumbers.clear();
}
/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/

CLeicaReagentKit::~CLeicaReagentKit()
{
    try {
        (void)DeleteAllLotNumbers(); // to avoid lint-534
    }
    catch(...) {
        //to please PClint
    }
}
/****************************************************************************/
/*!
 *  \brief Deletes all lot number in the list
 */
/****************************************************************************/
void CLeicaReagentKit::DeleteAllLotNumbers()
{
    //remove Lotnumber from the list
    if (!m_ReagentLotNumbers.isEmpty()) {
        for (int i=0; i<m_ReagentLotNumbers.count(); i++) {
            m_ReagentLotNumbers.removeAt(i);
        }
    }
    m_ReagentLotNumbers.clear();
}
/****************************************************************************/
/*!
 *  \brief Appends the 'L' to the Program ID.
 *
 *  \iparam ProgramNumber = Program Number from RFID
 *
 *  \return
 */
/****************************************************************************/
void CLeicaReagentKit::SetProgramID(int ProgramNumber)
{
    (void)m_ProgramID.setNum(ProgramNumber);
    (void)m_ProgramID.prepend("L");
}
/****************************************************************************/
/*!
 *  \brief Checks with Lotnumber whether specific Lotnumber exists
 *
 *  \iparam Lotnumber = Lotnumber of the Reagent
 *
 *  \return True or False
 */
/****************************************************************************/
bool CLeicaReagentKit::LotNumberExists(const int Lotnumber)
{
    return m_ReagentLotNumbers.contains(Lotnumber);
}
/****************************************************************************/
/*!
 *  \brief Decrement the Reagent Bottle Count present in Kit
 *
 */
/****************************************************************************/
void CLeicaReagentKit::DecrementBottleCount()
{
    if(m_ReagentBottleCount) {
        m_ReagentBottleCount--;
    }
}
/****************************************************************************/
/*!
 *  \brief DeSerialises the Kit Information and Stores it.
 *
 *  \iparam KitTagValue = RFID Tage Value of Kit
 *
 *  \return true - success
 */
/****************************************************************************/
bool CLeicaReagentKit::DeserializeKitContent(const QByteArray *KitTagValue)
{
    // Type id has been verified store it
    unsigned int Tempbyte = 0;
    Tempbyte = (KitTagValue->at(0) & 0xFF);
    SetTypeID(Tempbyte);

    // Extract program id=>Kit Id from the tag - 2 bytes
    Tempbyte = (KitTagValue->at(1) & 0xFF);
    Tempbyte = Tempbyte << 8 | (KitTagValue->at(2) & 0xFF);
    SetProgramID(Tempbyte);

    // Extract number of bottles number = Number of reagents
    Tempbyte = (KitTagValue->at(3) & 0xFF);
    SetReagentBottleCount(Tempbyte);

    // Extract bestbefore date
    QString Year,Month,Day,BestBeforeDate;
    Tempbyte = (KitTagValue->at(4) & 0xFF);
    Year.setNum(Tempbyte);
    Tempbyte = (KitTagValue->at(5) & 0xFF);
    Month.setNum(Tempbyte);
    Tempbyte = (KitTagValue->at(6) & 0xFF);
    Day.setNum(Tempbyte);
    BestBeforeDate = Year + Month + Day;
    SetBestBeforeDate(BestBeforeDate);

    //Extract open shell life time - max time
    Tempbyte = (KitTagValue->at(7) & 0xFF);
    SetOpenShellLifetime(Tempbyte);

    //Maximum number of slides
    Tempbyte = (KitTagValue->at(8) & 0xFF);
    Tempbyte = ((Tempbyte << 8) | (KitTagValue->at(9) & 0xFF));
    SetMaxSlides(Tempbyte);

    //Initialisese the Lotnumbers
    m_ReagentLotNumbers.clear();
    //now set the lot numbers
    for (int Ic = 0; Ic < (GetReagentBottleCount() * 4); Ic+= 4) {

        Tempbyte = (KitTagValue->at(10 + Ic) & 0xFF);
        Tempbyte = Tempbyte << 8 | (KitTagValue->at(10 + Ic + 1) & 0xFF);
        Tempbyte = Tempbyte << 8 | (KitTagValue->at(10 + Ic + 2) & 0xFF);
        Tempbyte = Tempbyte << 8 | (KitTagValue->at(10 + Ic + 3) & 0xFF);

        m_ReagentLotNumbers.append(Tempbyte);
    }
    return true;
}

}  // namespace HimalayaRMS
