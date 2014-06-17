/****************************************************************************/
/*! \file System.cpp
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

#include "Diagnostics/Include/System/BottleCheckingTest.h"
#include "Diagnostics/Include/System/FillingNDrainingTest.h"
#include "Diagnostics/Include/System/SpeakerTest.h"
#include "Diagnostics/Include/System/AlarmTest.h"
#include "Diagnostics/Include/System/UsbTest.h"
#include "Diagnostics/Include/System/MainsRelayTest.h"
#include "Diagnostics/Include/System/VoltageTest.h"
#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"


namespace Diagnostics {

CSystem::CSystem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CSystem)
{
    ui->setupUi(this);

    (void)connect(ui->testBottleChecking,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartBottleCheckingTest()) );

    (void)connect(ui->testFillingDraining,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartFillingNDrainingTest()) );

    (void)connect(ui->testOverflow,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartOverflowTest()) );

    (void)connect(ui->testSpeaker,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartSpeakerTest()) );

    (void)connect(ui->testAlarm,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartAlarmTest()) );

    (void)connect(ui->testEthernet,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartEthernetTest()) );

    (void)connect(ui->testUSB,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartUsbTest()) );

    (void)connect(ui->testMainsRelay,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartMainsRelayTest()) );

    (void)connect(ui->testVoltageSwitching,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartVoltageTest()) );
}

CSystem::~CSystem()
{
    delete ui;
}

void CSystem::StartBottleCheckingTest(void)
{
    //Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_BOTTLE_CHECKING_TEST);
    qDebug() << "System: start bottle checking test";

    System::CBottleCheckingTest test;

    test.Run();
}

void CSystem::StartFillingNDrainingTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_FILLING_DRAINING_TEST);
    qDebug() << "System: start filling and draining test";

    System::CFillingNDrainingTest test;

    test.Run();
}

void CSystem::StartOverflowTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_OVERFLOW_TEST);
    qDebug() << "System: start overflow test";
}

void CSystem::StartSpeakerTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_SPEAKER_TEST);
    qDebug() << "System: start speaker test";

    System::CSpeakerTest test;

    test.Run();
}

void CSystem::StartAlarmTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_ALARM_TEST);
    qDebug() << "System: start alarm test";

    System::CAlarmTest test;

    test.Run();
}

void CSystem::StartEthernetTest(void)
{
    //Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_ETHERNET_TEST);
    qDebug() << "System: start ethernet test";
}

void CSystem::StartUsbTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_USB_TEST);
    qDebug() << "System: start USB test";

    System::CUsbTest test;

    test.Run();
}

void CSystem::StartMainsRelayTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_MAINS_RELAY_TEST);
    qDebug() << "System: start Mains relay test";

    System::CMainsRelayTest test;

    test.Run();
}

void CSystem::StartVoltageTest(void)
{
    //Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_VOLTAGE_TEST);
    qDebug() << "System: start 110V/220V switching test";

    System::CVoltageTest test;

    test.Run();
}

} // namespace Diagnostics
