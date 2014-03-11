/****************************************************************************/
/*! \file VoltageTest.h
 *
 *  \brief Declaration of System 110V/220V switching test.
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

#ifndef DIAGNOSTICS_SYSTEM_VOLTAGETEST_H
#define DIAGNOSTICS_SYSTEM_VOLTAGETEST_H

#include "Diagnostics/Include/TestBase.h"

namespace Diagnostics {

namespace System {

class CVoltageTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CVoltageTest(void);
    ~CVoltageTest(void);

    void Run(void);

public Q_SLOTS:
    void FirstConfirmVoltage(void);

private Q_SLOTS:
    void Succeed(void);

    void Fail(void);
};

} // namespace System

} // namespace Diagnostics

#endif // DIAGNOSTICS_SYSTEM_VOLTAGETEST_H
