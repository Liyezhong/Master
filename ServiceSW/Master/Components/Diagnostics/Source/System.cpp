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
    mp_Ui(new Ui::CSystem)
{
    mp_Ui->setupUi(this);

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
    delete mp_Ui;
}

void CSystem::StartSealingTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_SEALING_TEST);

    qDebug() << "System: start sealing test";

}

void CSystem::StartFillingNDrainingTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_FILLING_DRAINING_TEST);
    qDebug() << "System: start filling and draining test";


}

void CSystem::StartLiquidHoseTest(void)
{
    //Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_OVERFLOW_TEST);
    qDebug() << "System: start liquid hose test";
}

void CSystem::StartSpeakerTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_SPEAKER_TEST);
    qDebug() << "System: start speaker test";

}

void CSystem::StartLocalAlarmTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_ALARM_TEST);
    qDebug() << "System: start local alarm test";

}

void CSystem::StartRemoteAlarmTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_ALARM_TEST);
    qDebug() << "System: start retmote alarm test";
}

void CSystem::StartExhaustFanTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_EXHAUST_FUN_TEST);
    qDebug() << "System: start exhaust fan test";


}

void CSystem::StartVentilationFanTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_SYSTEM_VENTILATION_FUN_TEST;
    qDebug() << "System: start ventilation fan test";


}

} // namespace Diagnostics
