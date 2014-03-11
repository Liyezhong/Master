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
#include "Diagnostics/Include/Retort/RetortHeatingTestWithLiquid.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"

namespace Diagnostics {

CRetort::CRetort(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CRetort)
{
    ui->setupUi(this);

    (void)connect(ui->testLidLock,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartLidLockTest()) );

    (void)connect(ui->testLevelSensorHeating,
                 SIGNAL(clicked()),
                 this,
                 SLOT(StartLevelSensorHeatingTest()) );

    (void)connect(ui->testLevelSensorDetecting,
                 SIGNAL(clicked()),
                 this,
                 SLOT(StartLevelSensorDetectingTest()) );

    (void)connect(ui->testHeatingEmpty,
                 SIGNAL(clicked()),
                 this,
                 SLOT(StartHeatingTestEmpty()) );

    (void)connect(ui->testHeatingWithLiquid,
                 SIGNAL(clicked()),
                 this,
                 SLOT(StartHeatingTestWithLiquid()) );
}

CRetort::~CRetort()
{
    delete ui;
}

void CRetort::StartLidLockTest(void)
{
    qDebug() << "Retort: start lid lock test";

    //Retort::CLidLockTest test;

    //test.Run();
}

void CRetort::StartLevelSensorHeatingTest(void)
{
    qDebug() << "Retort: start level sensor heating test";

    //Retort::CLevelSensorHeatingTest test;

    //test.Run();

    emit GuiLevelSensorHeatingTest();
}

void CRetort::StartLevelSensorDetectingTest(void)
{
    qDebug() << "Retort: start level sensor detecting test";

    Retort::CLevelSensorDetectingTest test;

    CONNECTSIGNALSLOT(&test, StartLevelSensorDetectTest(qint32), this, OnStartLevelSensorDetectTest(qint32));

    test.Run();

    //emit GuiLevelSensorHeatingTest(0, 2);
}

void CRetort::StartHeatingTestEmpty(void)
{
    qDebug() << "Retort: start heating test empty";

    //Retort::CHeatingTestEmpty test;

    //test.Run();

    emit GuiRetortEmptyHeatingTest();
}

void CRetort::StartHeatingTestWithLiquid(void)
{
    qDebug() << "Retort: start heating test with liquid";

    //Retort::CHeatingTestWithLiquid test;

    //test.Run();

    emit GuiRetortLiquidHeatingTest();
}

void CRetort::OnStartLevelSensorDetectTest(qint32 Position)
{
    qDebug() << "Retort: start level sensor detecting test with bottle position";

    emit GuiLevelSensorDetectingTest(Position);
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
    switch (p_Event->type())
    {
    case QEvent::LanguageChange:
    {
        ui->retranslateUi(this);
    }
        break;
    default:
        break;
    }
}

} // namespace Diagnostics
