/****************************************************************************/
/*! \file Diagnostics/Source/System.cpp
 *
 *  \brief Implementation of System test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-06-03
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

#include "Diagnostics/Include/System.h"

#include <QDebug>

#include "ui_System.h"

#include "Diagnostics/Include/System/FillingNDrainingTest.h"
#include "Diagnostics/Include/System/SpeakerTest.h"
#include "Diagnostics/Include/System/AlarmTest.h"
#include "Diagnostics/Include/System/ExhaustFanTest.h"
#include "Diagnostics/Include/System/VentilationFanTest.h"
#include "Diagnostics/Include/System/SystemSealingTest.h"
#include "Diagnostics/Include/System/LiquidHoseTest.h"
#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"


namespace Diagnostics {

CSystem::CSystem(QWidget *p_Parent) :
    QWidget(p_Parent),
    mp_Ui(new Ui::CSystem)
{
    mp_Ui->setupUi(this);

    mp_MessageDlg = new CDiagnosticMessageDlg(this);

    mp_Ui->testSealing->setEnabled(false);
    mp_Ui->testLiquidHose->setEnabled(false);
    mp_Ui->testFillingDraining->setEnabled(false);

    (void)connect(mp_Ui->testSealing,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartSealingTest()) );

    (void)connect(mp_Ui->testFillingDraining,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartFillingNDrainingTest()) );

    (void)connect(mp_Ui->testLiquidHose,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartLiquidHoseTest()) );

    (void)connect(mp_Ui->testSpeaker,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartSpeakerTest()) );

    (void)connect(mp_Ui->testLocalAlarm,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartLocalAlarmTest()) );

    (void)connect(mp_Ui->testRemoteAlarm,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartRemoteAlarmTest()) );

    (void)connect(mp_Ui->testExhaustFan,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartExhaustFanTest()) );

    (void)connect(mp_Ui->testVentilationFan,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartVentilationFanTest()) );

}

CSystem::~CSystem()
{
    try {
        delete mp_Ui;
        delete mp_MessageDlg;
    }
    catch (...) {

    }
}

void CSystem::LogResult(QString &TestName, int RetError)
{
    if (RetError == RETURN_OK)
        Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is successful.").arg(TestName));
    else if (RetError == RETURN_ABORT)
        Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is aborted.").arg(TestName));
    else
        Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is failed.").arg(TestName));

}

void CSystem::OnEnableTestButton()
{
    mp_Ui->testSealing->setEnabled(true);
    mp_Ui->testLiquidHose->setEnabled(true);
    mp_Ui->testFillingDraining->setEnabled(true);
}

void CSystem::StartSealingTest(void)
{
    QString TestName = mp_Ui->testSealing->text();
    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is requested.").arg(TestName));

    emit SetGUITabEnable(false);
    System::CSystemSealingTest Test(mp_MessageDlg, this);
    int Ret = Test.Run();
    LogResult(TestName, Ret);

    emit SetGUITabEnable(true);
}

void CSystem::StartFillingNDrainingTest(void)
{
    QString TestName = "Filling&Draining Test";
    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is requested.").arg(TestName));

    emit SetGUITabEnable(false);
    System::CFillingNDrainingTest Test(mp_MessageDlg, this);
    int Ret = Test.Run();
    LogResult(TestName, Ret);

    emit SetGUITabEnable(true);
}

void CSystem::StartLiquidHoseTest(void)
{
    QString TestName = mp_Ui->testLiquidHose->text();
    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is requested.").arg(TestName));

    emit SetGUITabEnable(false);
    System::CLiquidHoseTest Test(mp_MessageDlg, this);
    int Ret = Test.Run();
    LogResult(TestName, Ret);

    emit SetGUITabEnable(true);
}

void CSystem::StartSpeakerTest(void)
{
    QString TestName = mp_Ui->testSpeaker->text();
    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is requested.").arg(TestName));

    emit SetGUITabEnable(false);
    System::CSpeakerTest Test(mp_MessageDlg, this);
    int Ret = Test.Run();
    LogResult(TestName, Ret);

    emit SetGUITabEnable(true);
}

void CSystem::StartLocalAlarmTest(void)
{
    QString TestName = mp_Ui->testLocalAlarm->text();
    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is requested.").arg(TestName));

    emit SetGUITabEnable(false);
    System::CAlarmTest Test("Local", mp_MessageDlg, this);
    int Ret = Test.Run();

    LogResult(TestName, Ret);

    emit SetGUITabEnable(true);
}

void CSystem::StartRemoteAlarmTest(void)
{
    QString TestName = mp_Ui->testRemoteAlarm->text();
    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is requested.").arg(TestName));

    emit SetGUITabEnable(false);
    System::CAlarmTest Test("Remote", mp_MessageDlg, this);
    int Ret = Test.Run();

    LogResult(TestName, Ret);

    emit SetGUITabEnable(true);
}

void CSystem::StartExhaustFanTest(void)
{
    QString TestName = mp_Ui->testExhaustFan->text();
    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is requested.").arg(TestName));


    emit SetGUITabEnable(false);
    System::CExhaustFanTest Test(mp_MessageDlg, this);

    int Ret = Test.Run();

    LogResult(TestName, Ret);

    emit SetGUITabEnable(true);
}

void CSystem::StartVentilationFanTest(void)
{
    QString TestName = mp_Ui->testVentilationFan->text();
    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is requested.").arg(TestName));

    emit SetGUITabEnable(false);
    System::CVentilationFanTest Test(mp_MessageDlg, this);
    int Ret = Test.Run();

    LogResult(TestName, Ret);

    emit SetGUITabEnable(true);
}

} // namespace Diagnostics
