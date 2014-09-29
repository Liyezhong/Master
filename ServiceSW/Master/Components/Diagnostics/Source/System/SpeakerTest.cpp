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

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"
#include "Global/Include/SystemPaths.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

namespace Diagnostics {

namespace System {

CSpeakerTest::CSpeakerTest(CDiagnosticMessageDlg* p_MessageDlg, QWidget *p_Parent)
    : CTestBase(p_Parent),
      mp_SpeakProc(new QProcess),
      mp_MessageDlg(p_MessageDlg)
{

}

CSpeakerTest::~CSpeakerTest(void)
{
    try {
        delete mp_SpeakProc;
    }
    catch (...) {

    }
}

int CSpeakerTest::Run(void)
{
    qDebug() << "Speaker Test starts!";

    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SSystemSpeaker");

    QString LowVolume  = p_TestCase->GetParameter("LowVolume");
    QString HighVolume = p_TestCase->GetParameter("HighVolume");

    PlaySound(LowVolume);

    int FinishFlag = 1;
    int Ret = ShowConfirmDlg(1);
    if (Ret == 0) {
        ShowFinishDlg(FinishFlag);
        return Ret;
    }

    PlaySound(HighVolume);
    Ret = ShowConfirmDlg(2);

    if (Ret == 1) {
        FinishFlag = 2;
    }
    ShowFinishDlg(FinishFlag);

    return Ret;
}

void CSpeakerTest::PlaySound(QString& Volume)
{
    qDebug()<<"CSpeakerTest: start play sound...";

    QString SetVolume;
    QStringList PlayParams;

    SetVolume = "amixer set PCM " + Volume + "%";
    PlayParams<<"-r"<<Global::SystemPaths::Instance().GetSoundPath() + "/Note1.ogg";

    qDebug()<<PlayParams;
    mp_SpeakProc->start(SetVolume);
    (void)mp_SpeakProc->waitForFinished();

    mp_SpeakProc->start("ogg123", PlayParams);
}

int CSpeakerTest::ShowConfirmDlg(int StepNum)
{
    int Ret(0);
    QString Title = "Speaker test";
    QString Text;
    if (StepNum == 1) {
        Text = "Do you hear the test sound?";
    }
    else if (StepNum == 2) {
        Text = "Do you hear a louder test sound?";
    }

    Ret = mp_MessageDlg->ShowConfirmMessage(Title, Text);
    mp_SpeakProc->terminate();

    return Ret;
}

void CSpeakerTest::ShowFinishDlg(int RetNum)
{
    QString Title = "Speaker test";
    QString Text;
    ErrorCode_t Ret = RETURN_ERR_FAIL;
    if (RetNum == 1) {
        Text = "Speaker test failed.Please change the E-box.";
    }
    else if (RetNum == 2) {
        Text = "Speaker test successful.";
        Ret = RETURN_OK;
    }
    mp_MessageDlg->ShowMessage(Title, Text, Ret);
}

} // namespace System

} // namespace Diagnostics
