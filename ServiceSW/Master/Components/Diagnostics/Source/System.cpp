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

void CSystem::OnEnableTestButton()
{
    mp_Ui->testSealing->setEnabled(true);
    mp_Ui->testLiquidHose->setEnabled(true);
    mp_Ui->testFillingDraining->setEnabled(true);
}

void CSystem::StartSealingTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_SEALING_TEST);
    qDebug() << "System: start sealing test";

    emit SetGUITabEnable(false);
    System::CSystemSealingTest Test(mp_MessageDlg, this);
    int Ret = Test.Run();
    if (Ret == RETURN_OK) {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_SEALING_TEST_SUCCESS);
    }
    else if (Ret == RETURN_ABORT) {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_TEST_ABORT, Global::tTranslatableStringList()<<"System sealing");
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_SEALING_TEST_FAILURE);
    }

    emit SetGUITabEnable(true);
}

void CSystem::StartFillingNDrainingTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_FILLING_DRAINING_TEST);
    qDebug() << "System: start filling and draining test";

    emit SetGUITabEnable(false);
    System::CFillingNDrainingTest Test(mp_MessageDlg, this);
    int Ret = Test.Run();
    if (Ret == RETURN_OK) {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_FILLING_DRAINING_TEST_SUCCESS);
    }
    else if (Ret == RETURN_ABORT) {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_TEST_ABORT,
                                                   Global::tTranslatableStringList()<<"System Filling&Draining");
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_FILLING_DRAINING_TEST_FAILURE);
    }

    emit SetGUITabEnable(true);
}

void CSystem::StartLiquidHoseTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_LIQUIDHOSE_TEST);
    qDebug() << "System: start liquid hose test";

    emit SetGUITabEnable(false);
    System::CLiquidHoseTest Test(mp_MessageDlg, this);
    int Ret = Test.Run();
    if (Ret == RETURN_OK) {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_LIQUIDHOSE_TEST_SUCCESS);
    }
    else if (Ret == RETURN_ABORT) {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_TEST_ABORT,
                                                   Global::tTranslatableStringList()<<"System liquid hose");
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_LIQUIDHOSE_TEST_FAILURE);
    }

    emit SetGUITabEnable(true);
}

void CSystem::StartSpeakerTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_SPEAKER_TEST);
    qDebug() << "System: start speaker test";

    emit SetGUITabEnable(false);
    System::CSpeakerTest Test(mp_MessageDlg, this);
    if (Test.Run() == RETURN_OK) {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_SPEAKER_TEST_SUCCESS);
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_SPEAKER_TEST_FAILURE);
    }

    emit SetGUITabEnable(true);
}

void CSystem::StartLocalAlarmTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_ALARM_TEST);
    qDebug() << "System: start local alarm test";

    emit SetGUITabEnable(false);
    System::CAlarmTest Test("Local", mp_MessageDlg, this);
    if (Test.Run() == RETURN_OK) {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_ALARM_TEST_SUCCESS);
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_ALARM_TEST_FAILURE);
    }

    emit SetGUITabEnable(true);
}

void CSystem::StartRemoteAlarmTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_ALARM_TEST);
    qDebug() << "System: start retmote alarm test";

    emit SetGUITabEnable(false);
    System::CAlarmTest Test("Remote", mp_MessageDlg, this);
    if (Test.Run() == RETURN_OK) {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_ALARM_TEST_SUCCESS);
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_ALARM_TEST_FAILURE);
    }

    emit SetGUITabEnable(true);
}

void CSystem::StartExhaustFanTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_EXHAUST_FAN_TEST);
    qDebug() << "System: start exhaust fan test";

    emit SetGUITabEnable(false);
    System::CExhaustFanTest Test(mp_MessageDlg, this);
    if (Test.Run() == RETURN_OK) {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_EXHAUST_FAN_TEST_SUCCESS);
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_EXHAUST_FAN_TEST_FAILURE);
    }

    emit SetGUITabEnable(true);
}

void CSystem::StartVentilationFanTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_VENTILATION_FAN_TEST);
    qDebug() << "System: start ventilation fan test";

    emit SetGUITabEnable(false);
    System::CVentilationFanTest Test(mp_MessageDlg, this);
    if (Test.Run() == RETURN_OK) {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_VENTILATION_FAN_TEST_SUCCESS);
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_VENTILATION_FAN_TEST_FAILURE);
    }

    emit SetGUITabEnable(true);
}

} // namespace Diagnostics
