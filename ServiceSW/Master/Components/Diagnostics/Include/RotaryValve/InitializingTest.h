/****************************************************************************/
/*! \file InitializingTest.h
 *
 *  \brief Declaration of Rotary valve initializing test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-29
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

#ifndef DIAGNOSTICS_ROTARYVALVE_INITIALIZINGTEST_H
#define DIAGNOSTICS_ROTARYVALVE_INITIALIZINGTEST_H

#include "Diagnostics/Include/TestBase.h"

namespace Diagnostics {

namespace RotaryValve {

class CInitializingTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CInitializingTest();
    ~CInitializingTest(void);

    void Run(void);

public Q_SLOTS:
    void StartInitializing(void);

private Q_SLOTS:
    void Succeed(void);

    void Fail(void);

signals:
    void StartRVInitTest();
};

} // namespace RotaryValve

} // namespace Diagnostics

#endif // DIAGNOSTICS_ROTARYVALVE_INITIALIZINGTEST_H
