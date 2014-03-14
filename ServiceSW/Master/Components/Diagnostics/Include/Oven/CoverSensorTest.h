/****************************************************************************/
/*! \file CoverSensorTest.h
 *
 *  \brief Declaration of Oven cover sensor test.
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

#ifndef DIAGNOSTICS_OVEN_COVERSENSORTEST_H
#define DIAGNOSTICS_OVEN_COVERSENSORTEST_H

#include "Diagnostics/Include/TestBase.h"

namespace Diagnostics {

namespace Oven {

class CCoverSensorTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CCoverSensorTest(void);
    ~CCoverSensorTest(void);

    void Run(void);

public Q_SLOTS:
    void FirstManualOpen(void);

private Q_SLOTS:
    void FirstCheckOpen(void);

    void SecondManualClose(void);

    void SecondCheckClose(void);

    void Succeed(void);

    void Fail(void);

    void Cancel(void);
};

} // namespace Oven

} // namespace Diagnostics

#endif // DIAGNOSTICS_OVEN_COVERSENSORTEST_H
