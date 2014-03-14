/****************************************************************************/
/*! \file PressureSensor.cpp
 *
 *  \brief Implementation of Pressure Sensor menu.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-23
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

#include "Calibration/Include/PressureSensor.h"

#include <QDebug>

#include "Calibration/Include/PressureSensorCalibrate.h"

#include "ui_PressureSensor.h"

namespace Calibration {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CPressureSensor::CPressureSensor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CPressureSensor)
{
    ui->setupUi(this);

    (void)connect(ui->startButton,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartCalibration()) );
}

CPressureSensor::~CPressureSensor()
{
    delete ui;
}

void CPressureSensor::StartCalibration(void)
{
    qDebug() << "Pressure Sensor: start calibration";

    CPressureSensorCalibrate Calibrate;

    Calibrate.Run();
}

} // namespace Calibration
