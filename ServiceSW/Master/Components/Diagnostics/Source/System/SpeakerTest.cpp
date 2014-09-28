/****************************************************************************/
/*! \file SpeakerTest.cpp
 *
 *  \brief Implementation of System speaker test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-28
 *   $Author:  $ R.Wu
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

#include "Diagnostics/Include/System/SpeakerTest.h"

#include <QDebug>
#include <QProcess>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"
#include "Global/Include/SystemPaths.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

namespace Diagnostics {

namespace System {

CSpeakerTest::CSpeakerTest(QWidget *p_Parent)
    : CTestBase(p_Parent)
{
}

CSpeakerTest::~CSpeakerTest(void)
{
}

int CSpeakerTest::Run(void)
{
    qDebug() << "Speaker Test starts!";

    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase("SSystemSpeaker");

    QString LowVolume  = p_TestCase->GetParameter("LowVolume");
    QString HighVolume = p_TestCase->GetParameter("HighVolume");

    QProcess  SpeakProc;

    QString SetVolume;
    QStringList PlayParams;

    SetVolume = "amixer set PCM " + LowVolume + "%";
    PlayParams<<"-r"<<Global::SystemPaths::Instance().GetSoundPath() + "/Note6.ogg";


    SpeakProc.start(SetVolume);
    (void)SpeakProc.waitForFinished();

    SpeakProc.start("ogg123", PlayParams);
}

} // namespace System

} // namespace Diagnostics
