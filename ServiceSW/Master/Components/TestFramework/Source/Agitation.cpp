/****************************************************************************/
/*! \file Agitation.cpp
 *
 *  \brief Agitation implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-06-08
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

#include "TestFramework/Include/Agitation.h"

namespace TestFramework {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CAgitation::CAgitation(QWidget *p_Parent) : CTestScreen(p_Parent)
{
    mp_SerialTest = new TestCases::CUserInput("Serial number");
    mp_PressureTest = new TestCases::CDigitalOutput("Apply pressure", "Did the water supply hold 6 bar?");

    mp_ReferenceTest = new TestCases::CStepperMotor("Reference run", 2, false);
    mp_Profile1Test = new TestCases::CStepperMotor("Slow speed run", 2);
    mp_Profile2Test = new TestCases::CStepperMotor("High speed run", 2);

    AddTestCase(mp_SerialTest);

    AddCaption("Water Pressure Test");
    AddTestCase(mp_PressureTest);

    AddCaption("Agitation");
    AddTestCase(mp_ReferenceTest);
    AddTestCase(mp_Profile1Test);
    AddTestCase(mp_Profile2Test);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CAgitation::~CAgitation()
{
    try {
        delete mp_Profile2Test;
        delete mp_Profile1Test;
        delete mp_ReferenceTest;
        delete mp_SerialTest;
        delete mp_PressureTest;
    }
    catch (...) {}
}

} // end namespace TestFramework
