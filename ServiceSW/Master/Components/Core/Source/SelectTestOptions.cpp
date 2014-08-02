/****************************************************************************/
/*! \file SelectTestOptions.cpp
 *
 *  \brief
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-05-12
 *   $Author:  $ Abe Yang
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Core/Include/SelectTestOptions.h"

namespace Core {

ManufacturalTestMode_t CSelectTestOptions::m_TestMod = MANUFACTURAL_ENDTEST;

CSelectTestOptions::CSelectTestOptions()
{
}

CSelectTestOptions::~CSelectTestOptions()
{
}

ManufacturalTestMode_t CSelectTestOptions::GetCurTestMode()
{
    return m_TestMod;
}

void CSelectTestOptions::SetCurTestMode(ManufacturalTestMode_t testMod)
{
    m_TestMod = testMod;
}

}   // end of namespace Core

