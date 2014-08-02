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
#include "Global/Include/SystemPaths.h"

namespace Core {

ManufacturalTestMode_t CSelectTestOptions::m_TestMod = MANUFACTURAL_UNDEFINED_TEST;

CSelectTestOptions::CSelectTestOptions()
{
}

CSelectTestOptions::~CSelectTestOptions()
{
}

ManufacturalTestMode_t CSelectTestOptions::GetCurTestMode()
{
    if ( m_TestMod == MANUFACTURAL_UNDEFINED_TEST ) {
        QString FileName = Global::SystemPaths::Instance().GetSettingsPath() + "/TestMode.txt";

        FILE* pFile;

        if ((pFile = fopen(FileName.toStdString().c_str(), "r")) == NULL)
        {
            return MANUFACTURAL_ENDTEST;
        }

        char Buf[200];
        memset(Buf, 0, sizeof(Buf));
        if(fread(Buf, 1, 200, pFile) > 0 )
        {
            QString Content = QString::fromAscii(Buf, -1);
            m_TestMod = (ManufacturalTestMode_t) Content.toInt();

        }
        fclose(pFile);
    }

    return m_TestMod;
}

void CSelectTestOptions::SetCurTestMode(ManufacturalTestMode_t testMod)
{
    QString FileName = Global::SystemPaths::Instance().GetSettingsPath() + "/TestMode.txt";
    QString msg = QString("%1").arg(testMod);
    FILE* pFile = fopen(FileName.toStdString().c_str(), "w+");
    fprintf(pFile, "%s", msg.toStdString().c_str());
    fflush(pFile);
    fclose(pFile);
    system("sync");

    m_TestMod = testMod;
}

}   // end of namespace Core

