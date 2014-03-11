/****************************************************************************/
/*! \file HeatingBelt1Test.h
 *
 *  \brief Declaration of L&A System heating belt 1 test.
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

#ifndef DIAGNOSTICS_LASYSTEM_HEATINGBELT1TEST_H
#define DIAGNOSTICS_LASYSTEM_HEATINGBELT1TEST_H

#include "Diagnostics/Include/TestBase.h"

namespace Diagnostics {

namespace LaSystem {

class CHeatingBelt1Test : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CHeatingBelt1Test(void);
    ~CHeatingBelt1Test(void);

    void Run(void);

public Q_SLOTS:
    void FirstCheckBottle(void);

private Q_SLOTS:
    void SecondHeating(void);

    void Succeed(void);

    void Fail(void);

    void Cancel(void);
};

} // namespace LaSystem

} // namespace Diagnostics

#endif // DIAGNOSTICS_LASYSTEM_HEATINGBELT1TEST_H
