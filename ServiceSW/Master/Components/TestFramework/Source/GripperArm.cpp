/****************************************************************************/
/*! \file GripperArm.cpp
 *
 *  \brief GripperArm implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-05-31
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

#include "TestFramework/Include/GripperArm.h"

namespace TestFramework {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CGripperArm::CGripperArm(QWidget *p_Parent) : CTestScreen(p_Parent)
{
    // X-Axis
    mp_XAxisSerialTest = new TestCases::CUserInput("Serial number");
    mp_XAxisFrequencyTest = new TestCases::CUserInput("Resonant frequency");
    mp_XAxisReferenceTest = new TestCases::CStepperMotor("X-Axis reference", 1, false);
    mp_XAxisProfile1Test = new TestCases::CStepperMotor("X-Axis profile 1", 1);
    mp_XAxisProfile2Test = new TestCases::CStepperMotor("X-Axis profile 2", 1);
    // Y-Axis / Z-Axis
    mp_ZAxisInitTest = new TestCases::CInitialization("Z-Axis");
    mp_YAxisInitTest = new TestCases::CInitialization("Y-Axis");
    mp_ZAxisFrequencyTest = new TestCases::CUserInput("Resonant frequency", "Z-Axis");
    mp_YAxisFrequencyTest = new TestCases::CUserInput("Resonant frequency", "Y-Axis");
    mp_ZAxisReferenceTest = new TestCases::CStepperMotor("Z-Axis reference", 1, false);
    mp_ZAxisProfile1Test = new TestCases::CStepperMotor("Z-Axis profile 1", 1);
    mp_ZAxisProfile2Test = new TestCases::CStepperMotor("Z-Axis profile 2", 1);
    mp_YAxisReferenceTest = new TestCases::CStepperMotor("Y-Axis reference", 1);
    mp_YAxisProfile1Test = new TestCases::CStepperMotor("Y-Axis profile 1", 1);
    mp_YAxisProfile2Test = new TestCases::CStepperMotor("Y-Axis profile 2", 1);
    mp_ZAxisRfidTest = new TestCases::CIdReader("RFID reader");
    mp_ZAxisFillingTest = new TestCases::CAnalogInput("Level sensor", "Liquid level:");

    AddCaption("Z-Axis / Y-Axis");
    AddTestCase(mp_ZAxisInitTest);
    AddTestCase(mp_YAxisInitTest);
    AddTestCase(mp_ZAxisFrequencyTest);
    AddTestCase(mp_YAxisFrequencyTest);
    AddTestCase(mp_ZAxisReferenceTest);
    AddTestCase(mp_ZAxisProfile1Test);
    AddTestCase(mp_ZAxisProfile2Test);
    AddTestCase(mp_YAxisReferenceTest);
    AddTestCase(mp_YAxisProfile1Test);
    AddTestCase(mp_YAxisProfile2Test);
    AddTestCase(mp_ZAxisRfidTest);
    AddTestCase(mp_ZAxisFillingTest);

    AddCaption("X-Axis");
    AddTestCase(mp_XAxisSerialTest);
    AddTestCase(mp_XAxisFrequencyTest);
    AddTestCase(mp_XAxisReferenceTest);
    AddTestCase(mp_XAxisProfile1Test);
    AddTestCase(mp_XAxisProfile2Test);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CGripperArm::~CGripperArm()
{
    try {
        // Z-Axis
        delete mp_ZAxisRfidTest;
        delete mp_ZAxisFillingTest;
        delete mp_ZAxisProfile2Test;
        delete mp_ZAxisProfile1Test;
        delete mp_ZAxisReferenceTest;
        delete mp_ZAxisFrequencyTest;
        delete mp_ZAxisInitTest;
        // Y-Axis
        delete mp_YAxisProfile2Test;
        delete mp_YAxisProfile1Test;
        delete mp_YAxisReferenceTest;
        delete mp_YAxisFrequencyTest;
        delete mp_YAxisInitTest;
        // X-Axis
        delete mp_XAxisProfile2Test;
        delete mp_XAxisProfile1Test;
        delete mp_XAxisReferenceTest;
        delete mp_XAxisFrequencyTest;
        delete mp_XAxisSerialTest;
    }
    catch (...) {}
}

} // end namespace TestFramework
