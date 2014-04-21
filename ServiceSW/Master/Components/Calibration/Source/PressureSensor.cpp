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
#include "ui_PressureSensor.h"
#include "Global/Include/Utils.h"

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
    mp_Ui(new Ui::CPressureSensor)
{
    mp_Ui->setupUi(this);
    CONNECTSIGNALSLOTGUI(mp_Ui->startButton, clicked(), this, StartCalibration());
}

CPressureSensor::~CPressureSensor()
{
    try {
        delete mp_Ui;
    }
    catch (...) {
        // to please Lint
    }
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CPressureSensor::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
    case QEvent::LanguageChange:
        mp_Ui->retranslateUi(this);
        RetranslateUI();
        break;
    default:
        break;
    }
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CPressureSensor::RetranslateUI()
{
}

void CPressureSensor::StartCalibration(void)
{
    //Global::EventObject::Instance().RaiseEvent(EVENT_GUI_CALIBRATION_PRESSURE_SENSOR);
    emit CalibrationPressureSensor();
}

} // namespace Calibration
