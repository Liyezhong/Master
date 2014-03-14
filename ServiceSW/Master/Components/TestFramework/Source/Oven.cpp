/****************************************************************************/
/*! \file Oven.cpp
 *
 *  \brief Oven implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-06-10
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

#include "TestFramework/Include/Oven.h"

namespace TestFramework {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
COven::COven(QWidget *p_Parent) : CTestScreen(p_Parent)
{
    mp_InitTest = new TestCases::CInitialization();

    mp_ReferenceTest = new TestCases::CStepperMotor("Reference run", 2, false);
    mp_OpenTest = new TestCases::CStepperMotor("Open cover", 2);
    mp_CloseTest = new TestCases::CStepperMotor("Close cover", 2);

    mp_DesiredTemp = new TestCases::CUserInput("Desired temperature");
    mp_TempTest = new TestCases::CTimedTest("Temperature test", false);
    mp_Sensor1 = new TestCases::CAnalogInput("Sensor 1", "Temperature:");
    mp_Sensor2 = new TestCases::CAnalogInput("Sensor 2", "Temperature:");
    mp_FanSpeed1 = new TestCases::CAnalogInput("Fan 1", "Speed:");
    mp_FanSpeed2 = new TestCases::CAnalogInput("Fan 2", "Speed:");
    mp_Current = new TestCases::CAnalogInput("Heater", "Current:");

    AddTestCase(mp_InitTest);

    AddCaption("Cover");
    AddTestCase(mp_ReferenceTest);
    AddTestCase(mp_OpenTest);
    AddTestCase(mp_CloseTest);

    AddCaption("Temperature");
    AddTestCase(mp_DesiredTemp);
    AddTestCase(mp_TempTest);
    AddTestCase(mp_Sensor1);
    AddTestCase(mp_Sensor2);
    AddTestCase(mp_FanSpeed1);
    AddTestCase(mp_FanSpeed2);
    AddTestCase(mp_Current);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
COven::~COven()
{
    try {
        // Temperature
        delete mp_Current;
        delete mp_FanSpeed2;
        delete mp_FanSpeed1;
        delete mp_Sensor2;
        delete mp_Sensor1;
        delete mp_TempTest;
        // Cover
        delete mp_DesiredTemp;
        delete mp_CloseTest;
        delete mp_OpenTest;
        delete mp_ReferenceTest;
        // Init
        delete mp_InitTest;
    }
    catch (...) {}
}

} // end namespace TestFramework
