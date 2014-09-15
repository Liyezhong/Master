/****************************************************************************/
/*! \file LidLockTest.h
 *
 *  \brief Declaration of Retort Lid Lock test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-28
 *   $Author:  $ R.Wu
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DIAGNOSTICS_RETORT_LIDLOCKTEST_H
#define DIAGNOSTICS_RETORT_LIDLOCKTEST_H

#include "Diagnostics/Include/TestBase.h"

namespace Diagnostics {

namespace Retort {

class CLidLockTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CLidLockTest(void);
    ~CLidLockTest(void);

    int Run(void);

public Q_SLOTS:
    void FirstManualClose(void);

private Q_SLOTS:
    void FirstCheckClose(void);

    void SecondPressOpen(void);

    void OpenLidLock(void);

    void SecondCheckOpen(void);

    void ThirdManualOpen(void);

    void ThirdCheckOpen(void);

    void Succeed(void);

    void Fail(void);

    void Cancel(void);
};

} // namespace Retort

} // namespace Diagnostics

#endif // DIAGNOSTICS_RETORT_LIDLOCKTEST_H
