/****************************************************************************/
/*! \file LevelSensorHeatingTest.h
 *
 *  \brief Declaration of Retort Level Sensor Heating test.
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

#ifndef DIAGNOSTICS_RETORT_LEVELSENSORHEATINGTEST_H
#define DIAGNOSTICS_RETORT_LEVELSENSORHEATINGTEST_H

#include "Diagnostics/Include/TestBase.h"

namespace Diagnostics {

namespace Retort {

class CLevelSensorHeatingTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CLevelSensorHeatingTest();
    ~CLevelSensorHeatingTest(void);

    int Run(void);

public Q_SLOTS:
    void StartHeating(void);

private Q_SLOTS:
    void Succeed(void);

    void Fail(void);
};

} // namespace Retort

} // namespace Diagnostics

#endif // DIAGNOSTICS_RETORT_LEVELSENSORHEATINGTEST_H
