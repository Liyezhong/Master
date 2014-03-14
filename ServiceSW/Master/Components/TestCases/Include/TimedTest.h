/****************************************************************************/
/*! \file TimedTest.h
 *
 *  \brief TimedTest definition.
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

#ifndef TESTCASES_TIMEDTEST_H
#define TESTCASES_TIMEDTEST_H

#include "TestCases/Include/TestCaseBase.h"
#include <QLabel>

namespace TestCases {

/****************************************************************************/
/**
 * \brief This class displays a small clock for tests that run for a longer
 *        time.
 */
/****************************************************************************/
class CTimedTest : public CTestCaseBase
{
public:
    CTimedTest(QString Caption, bool AutoStart = true);
    virtual ~CTimedTest();

private:
    QLabel *mp_TextLabel;   //!< Displays the timer
    QLabel *mp_TimeLabel;   //!< Holding the sub caption
};

} // end namespace TestCases

#endif // TESTCASES_TIMEDTEST_H
