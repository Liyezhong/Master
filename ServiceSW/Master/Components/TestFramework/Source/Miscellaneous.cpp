/****************************************************************************/
/*! \file Miscellaneous.cpp
 *
 *  \brief Miscellaneous implementation.
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

#include "TestFramework/Include/Miscellaneous.h"

namespace TestFramework {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CMiscellaneous::CMiscellaneous(QWidget *p_Parent) : CTestScreen(p_Parent)
{
    mp_SerialTest = new TestCases::CUserInput("Serial number");
    mp_LightTest = new TestCases::CDigitalOutput("Light", "Is the light switched on:");
    mp_OpenCoverTest = new TestCases::CDigitalInput("Open the covering hood");
    mp_CloseCoverTest = new TestCases::CDigitalInput("Close the covering hood");
    mp_CurrentTest = new TestCases::CAnalogInput("Fan current", "Current:");
    mp_OpenExhaustTest = new TestCases::CDigitalInput("Free exhaust air unit");
    mp_CloseExhaustTest = new TestCases::CDigitalInput("Block exhaust air unit");

    AddTestCase(mp_SerialTest);
    AddCaption("Light");
    AddTestCase(mp_LightTest);
    AddCaption("Cover");
    AddTestCase(mp_OpenCoverTest);
    AddTestCase(mp_CloseCoverTest);
    AddCaption("Exhaust Air Unit");
    AddTestCase(mp_CurrentTest);
    AddTestCase(mp_OpenExhaustTest);
    AddTestCase(mp_CloseExhaustTest);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CMiscellaneous::~CMiscellaneous()
{
    try {
        delete mp_SerialTest;
        delete mp_LightTest;
        delete mp_OpenCoverTest;
        delete mp_CloseCoverTest;
        delete mp_CurrentTest;
        delete mp_OpenExhaustTest;
        delete mp_CloseExhaustTest;
    }
    catch (...) {}
}

} // end namespace TestFramework
