/****************************************************************************/
/*! \file Retort.cpp
 *
 *  \brief Implementation of Retort test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-27
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

#include "Diagnostics/Include/Retort.h"

#include <QDebug>

#include "ui_Retort.h"

#include "Global/Include/Utils.h"
#include "Diagnostics/Include/Retort/LidLockTest.h"
#include "Diagnostics/Include/Retort/LevelSensorHeatingTest.h"
#include "Diagnostics/Include/Retort/LevelSensorDetectingTest.h"
#include "Diagnostics/Include/Retort/RetortHeatingTestEmpty.h"
#include "Diagnostics/Include/Retort/RetortHeatingTestWithWater.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Main/Include/HimalayaServiceEventCodes.h"


namespace Diagnostics {

CRetort::CRetort(QWidget *parent) :
    QWidget(parent),
    mp_Ui(new Ui::CRetort)
{
    mp_Ui->setupUi(this);

    mp_MessageDlg = new CDiagnosticMessageDlg(this);

    (void)connect(mp_Ui->testLidLock,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartLidLockTest()) );

    (void)connect(mp_Ui->testDrainReagent,
                 SIGNAL(clicked()),
                 this,
                 SLOT(StartDrainReagentTest()) );

    (void)connect(mp_Ui->testLevelSensorDetection,
                 SIGNAL(clicked()),
                 this,
                 SLOT(StartLevelSensorDetectionTest()) );

    (void)connect(mp_Ui->testHeatingEmpty,
                 SIGNAL(clicked()),
                 this,
                 SLOT(StartHeatingTestEmpty()) );

    (void)connect(mp_Ui->testHeatingWater,
                 SIGNAL(clicked()),
                 this,
                 SLOT(StartHeatingTestWithWater()) );
}

CRetort::~CRetort()
{
    try {
        delete mp_Ui;
        delete mp_MessageDlg;
    }
    catch (...) {

    }
}

void CRetort::StartLidLockTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_RETORT_LIDLOCK_TEST);
    qDebug() << "Retort: start lid lock test";

    Retort::CLidLockTest Test(mp_MessageDlg, this);

    (void)Test.Run();
}

void CRetort::StartDrainReagentTest(void)
{
    //Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_RETORT_LEVELSENSOR_HEATING_TEST);
    qDebug() << "Retort: start level sensor heating test";

    //Retort::CLevelSensorHeatingTest test;

    //test.Run();
}

void CRetort::StartLevelSensorDetectionTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_RETORT_LEVELSENSOR_DETECT_TEST);
    qDebug() << "Retort: start level sensor detecting test";

    Retort::CLevelSensorDetectingTest Test(mp_MessageDlg, this);

    (void)Test.Run();
}

void CRetort::StartHeatingTestEmpty(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_RETORT_HEATING_EMPTY_TEST);
    qDebug() << "Retort: start heating test empty";

    Retort::CHeatingTestEmpty test(mp_MessageDlg);
    (void)test.Run();
}

void CRetort::StartHeatingTestWithWater(void)
{
//    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_RETORT_HEATING_LIQUID_TEST);
    qDebug() << "Retort: start heating test with water";

    Retort::CHeatingTestWithWater test(mp_MessageDlg);
    (void)test.Run();
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CRetort::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
    case QEvent::LanguageChange:
        mp_Ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

} // namespace Diagnostics
