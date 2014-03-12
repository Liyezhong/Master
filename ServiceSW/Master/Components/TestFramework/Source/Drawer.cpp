/****************************************************************************/
/*! \file Drawer.cpp
 *
 *  \brief Drawer implementation.
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

#include "TestFramework/Include/Drawer.h"

namespace TestFramework {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CDrawer::CDrawer(QWidget *p_Parent) : CTestScreen(p_Parent)
{
    mp_InitTest = new TestCases::CInitialization();
    mp_FrequencyTest = new TestCases::CUserInput("Resonant frequency");
    mp_ReferenceTest = new TestCases::CStepperMotor("Reference run", 2, false);
    mp_OpenTest = new TestCases::CStepperMotor("Open drawer", 2);
    mp_CloseTest = new TestCases::CStepperMotor("Close drawer", 2);
    mp_RfidTest = new TestCases::CIdReader("RFID reader");
    mp_ButtonTest = new TestCases::CDigitalInput("Press the drawer button");
    mp_GreenLedTest = new TestCases::CDigitalOutput("Green LED", "Does the LED shine green:");
    mp_RedLedTest = new TestCases::CDigitalOutput("Red LED", "Does the LED shine red:");

    AddTestCase(mp_InitTest);
    AddTestCase(mp_FrequencyTest);
    AddTestCase(mp_ReferenceTest);
    AddTestCase(mp_OpenTest);
    AddTestCase(mp_CloseTest);
    AddTestCase(mp_RfidTest);
    AddTestCase(mp_ButtonTest);
    AddTestCase(mp_GreenLedTest);
    AddTestCase(mp_RedLedTest);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CDrawer::~CDrawer()
{
    try {
        delete mp_RfidTest;
        delete mp_RedLedTest;
        delete mp_GreenLedTest;
        delete mp_ButtonTest;
        delete mp_CloseTest;
        delete mp_OpenTest;
        delete mp_ReferenceTest;
        delete mp_FrequencyTest;
        delete mp_InitTest;
    }
    catch (...) {}
}

} // end namespace TestFramework
