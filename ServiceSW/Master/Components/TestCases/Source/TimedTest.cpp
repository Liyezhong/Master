/****************************************************************************/
/*! \file TimedTest.cpp
 *
 *  \brief TimedTest implementation.
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

#include "TestCases/Include/TimedTest.h"

namespace TestCases {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam Caption = Title of the test case
 *  \iparam AutoStart = Indicates if the test has a start button (false)
 */
/****************************************************************************/
CTimedTest::CTimedTest(QString Caption, bool AutoStart) : CTestCaseBase(Caption, "", AutoStart)
{
    mp_TextLabel = new QLabel("Time remaining:");
    mp_TimeLabel = new QLabel("00:00");

    AddWidget(2, mp_TextLabel);
    AddWidget(3, mp_TimeLabel);
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CTimedTest::~CTimedTest()
{
    try {
        delete mp_TimeLabel;
        delete mp_TextLabel;
    }
    catch (...) {}
}

} // end namespace TestCases
