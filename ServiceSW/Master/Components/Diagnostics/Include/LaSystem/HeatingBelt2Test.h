/****************************************************************************/
/*! \file HeatingBelt2Test.h
 *
 *  \brief Declaration of L&A System heating belt 2 test.
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

#ifndef DIAGNOSTICS_LASYSTEM_HEATINGBELT2TEST_H
#define DIAGNOSTICS_LASYSTEM_HEATINGBELT2TEST_H

#include "Diagnostics/Include/TestBase.h"

namespace Diagnostics {

namespace LaSystem {

class CHeatingBelt2Test : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CHeatingBelt2Test(void);
    ~CHeatingBelt2Test(void);

    void Run(void);

public Q_SLOTS:
    void StartHeating(void);

private Q_SLOTS:
    void Succeed(void);

    void Fail(void);
};

} // namespace LaSystem

} // namespace Diagnostics

#endif // DIAGNOSTICS_LASYSTEM_HEATINGBELT2TEST_H
