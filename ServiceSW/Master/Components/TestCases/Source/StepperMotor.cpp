/****************************************************************************/
/*! \file StepperMotor.cpp
 *
 *  \brief StepperMotor implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-05-23
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

#include "TestCases/Include/StepperMotor.h"

namespace TestCases {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam SubCaption = Subtitle of the test case
 *  \iparam LightBarriers = Number of light barriers in the test (0, 1, 2)
 *  \iparam AutoStart = Indicates if the test has a start button (false)
 */
/****************************************************************************/
CStepperMotor::CStepperMotor(QString SubCaption, quint32 LightBarriers, bool AutoStart) : CTestCaseBase("Stepper motor", SubCaption, AutoStart)
{
    QPixmap Pixmap(":/CheckBox/CheckBox-Disabled.png");

    for (qint32 i = 0; i < 2; i++) {
        mp_LightBarrierText[i] = new QLabel("Light barrier:");
        mp_LightBarrierLed[i] = new QLabel();
        mp_LightBarrierLed[i]->setPixmap(Pixmap);
    }

    if (LightBarriers == 1) {
        AddWidget(2, mp_LightBarrierText[0]);
        AddWidget(3, mp_LightBarrierLed[0]);
    }
    else if (LightBarriers == 2) {
        AddWidgetVertical(2, mp_LightBarrierText[0], mp_LightBarrierText[1]);
        AddWidgetVertical(3, mp_LightBarrierLed[0], mp_LightBarrierLed[1]);
    }
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CStepperMotor::~CStepperMotor()
{
    try {
        for (qint32 i = 0; i < 2; i++) {
            delete mp_LightBarrierLed[i];
            delete mp_LightBarrierText[i];
        }
    }
    catch (...) {}
}

} // end namespace TestCases
