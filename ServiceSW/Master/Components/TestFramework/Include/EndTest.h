/****************************************************************************/
/*! \file EndTest.h
 *
 *  \brief EndTest definition.
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

#ifndef TESTFRAMEWORK_ENDTEST_H
#define TESTFRAMEWORK_ENDTEST_H

#include "TestFramework/Include/TestScreen.h"
#include "TestCases/Include/TimedTest.h"
#include "TestCases/Include/UserInput.h"

namespace TestFramework {

/****************************************************************************/
/**
 * \brief This class contains the panel displaying all end tests of the
 *        device.
 */
/****************************************************************************/
class CEndTest : public CTestScreen
{
public:
    CEndTest(QWidget *p_Parent = 0);
    virtual ~CEndTest();

private:
    TestCases::CUserInput *mp_SerialTest;   //!< Input for the serial number of the device
    TestCases::CTimedTest *mp_LongTest;     //!< Long end test
    TestCases::CTimedTest *mp_ShortTest;    //!< Short end test
};

} // end namespace TestFramework

#endif // TESTFRAMEWORK_ENDTEST_H
