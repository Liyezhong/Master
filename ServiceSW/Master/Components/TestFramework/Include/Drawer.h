/****************************************************************************/
/*! \file Drawer.h
 *
 *  \brief Drawer definition.
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

#ifndef TESTFRAMEWORK_DRAWER_H
#define TESTFRAMEWORK_DRAWER_H

#include "TestFramework/Include/TestScreen.h"
#include "TestCases/Include/Initialization.h"
#include "TestCases/Include/StepperMotor.h"
#include "TestCases/Include/IdReader.h"
#include "TestCases/Include/DigitalInput.h"
#include "TestCases/Include/DigitalOutput.h"
#include "TestCases/Include/UserInput.h"

namespace TestFramework {

/****************************************************************************/
/**
 * \brief Tests cases for the loading and unloading drawer.
 */
/****************************************************************************/
class CDrawer : public CTestScreen
{
public:
    CDrawer(QWidget *p_Parent = 0);
    virtual ~CDrawer();

private:
    TestCases::CInitialization *mp_InitTest;    //!< Assembly group registration test
    TestCases::CUserInput *mp_FrequencyTest;    //!< Input field for the resonant frequency
    TestCases::CStepperMotor *mp_ReferenceTest; //!< Stepper motor refrence run
    TestCases::CStepperMotor *mp_OpenTest;      //!< Test opening the drawer
    TestCases::CStepperMotor *mp_CloseTest;     //!< Test closing the drawer
    TestCases::CDigitalInput *mp_ButtonTest;    //!< Testing the drawer button
    TestCases::CDigitalOutput *mp_GreenLedTest; //!< Test of the green LED
    TestCases::CDigitalOutput *mp_RedLedTest;   //!< Test of the red LED
    TestCases::CIdReader *mp_RfidTest;          //!< Test of the RFID functionality
};

} // end namespace TestFramework

#endif // TESTFRAMEWORK_DRAWER_H
