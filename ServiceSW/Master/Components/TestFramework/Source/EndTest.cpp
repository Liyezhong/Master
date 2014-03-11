/****************************************************************************/
/*! \file EndTest.cpp
 *
 *  \brief EndTest implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-06-09
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

#include "TestFramework/Include/EndTest.h"

namespace TestFramework {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CEndTest::CEndTest(QWidget *p_Parent) : CTestScreen(p_Parent)
{
    mp_SerialTest = new TestCases::CUserInput("Serial number");
    mp_LongTest = new TestCases::CTimedTest("Long end test", false);
    mp_ShortTest = new TestCases::CTimedTest("Short end test", false);

    AddTestCase(mp_SerialTest);
    AddTestCase(mp_LongTest);
    AddTestCase(mp_ShortTest);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CEndTest::~CEndTest()
{
    try {
        delete mp_ShortTest;
        delete mp_LongTest;
        delete mp_SerialTest;
    }
    catch (...) {}
}

} // end namespace TestFramework
