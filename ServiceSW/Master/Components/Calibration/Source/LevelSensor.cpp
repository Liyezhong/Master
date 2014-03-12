/****************************************************************************/
/*! \file LevelSensor.cpp
 *
 *  \brief LevelSensor implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-07-11
 *   $Author:  $ M.Scherer
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Calibration/Include/LevelSensor.h"
#include "ui_LevelSensor.h"
#include <QDebug>

namespace Calibration {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CLevelSensor::CLevelSensor(QWidget *p_Parent) : QWidget(p_Parent), mp_Ui(new Ui::CLevelSensor)
{
    mp_Ui->setupUi(this);

    m_Instructions << "Press the button to start the calibration of the liquid level sensor.\n"
                   << "Put the gripper arm into an empty cuvette and press the button below.\n"
                   << "Fill a cuvette with water up to the highest marker inside. Put the gripper arm\ninto the cuvette and press the button to finish the calibration.";

    m_Step = 0;
    mp_Ui->instructionLabel->setText(m_Instructions[m_Step]);

    if (!connect(mp_Ui->stepButton, SIGNAL(clicked()), this, SLOT(NextStep()))) {
        qDebug() << "CLevelSensor: cannot connect 'clicked' signal";
    }
    if (!connect(mp_Ui->stepButton, SIGNAL(clicked()), this, SLOT(TestForDeviceInit()))) {
        qDebug() << "CLevelSensor: cannot connect 'clicked' signal";
    }

}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CLevelSensor::~CLevelSensor()
{
    try {
        delete mp_Ui;
    }
    catch (...) {}
}

/****************************************************************************/
/*!
 *  \brief Switching between the steps of the calibration process
 */
/****************************************************************************/
void CLevelSensor::NextStep()
{
    if (m_Step == 2) {
        mp_Ui->stepButton->setText("Calibrate");
        m_Step = 0;
    }
    else if (m_Step == 1) {
        mp_Ui->stepButton->setText("Finish");
        m_Step++;
    }
    else {
        mp_Ui->stepButton->setText("Next");
        m_Step++;
    }

    mp_Ui->instructionLabel->setText(m_Instructions[m_Step]);
}

void CLevelSensor::TestForDeviceInit()
{
    qDebug() << "CLevelSensor::TestForDeviceInit() called!!!";
    emit deviceConfigure();
}

} // end namespace Calibration
