/****************************************************************************/
/*! \file AirHeatingTubeTest.h
 *
 *  \brief Declaration of L&A air heating tube test.
 *
 *   $Version: $ 0.2
 *   $Date:    $ 2014-10-20
 *   $Author:  $ Arthur Li
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2014 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DIAGNOSTICS_LASYSTEM_AIRHEATINGTUBETEST_H
#define DIAGNOSTICS_LASYSTEM_AIRHEATINGTUBETEST_H

#include "Diagnostics/Include/TestBase.h"

namespace Diagnostics {

namespace LaSystem {

class CAirHeatingTubeTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CAirHeatingTubeTest(void);
    ~CAirHeatingTubeTest(void);

    int Run(void);

public Q_SLOTS:
    void FirstBuildPressure(void);

private Q_SLOTS:
    void SecondReleasePressure(void);
	
	void ThirdBuildVaccuum(void);
	
	void ForthReleaseVaccuum(void);

    void Succeed(void);

    void Fail(void);
};

} // namespace LaSystem

} // namespace Diagnostics

#endif // DIAGNOSTICS_LASYSTEM_AIRHEATINGTUBETEST_H

