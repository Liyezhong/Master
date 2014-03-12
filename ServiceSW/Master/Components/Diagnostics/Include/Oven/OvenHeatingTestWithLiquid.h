/****************************************************************************/
/*! \file OvenHeatingTestWithLiquid.h
 *
 *  \brief Declaration of Oven heating test with liquid.
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

#ifndef DIAGNOSTICS_OVEN_OVENHEATINGTESTWITHLIQUID_H
#define DIAGNOSTICS_OVEN_OVENHEATINGTESTWITHLIQUID_H

#include "Diagnostics/Include/TestBase.h"

namespace Diagnostics {

namespace Oven {

class CHeatingTestWithLiquid : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CHeatingTestWithLiquid(void);
    ~CHeatingTestWithLiquid(void);

    void Run(void);

public Q_SLOTS:

private Q_SLOTS:

    void Succeed(void);

    void Fail(void);

    void Cancel(void);

private:
};

} // namespace Oven

} // namespace Diagnostics

#endif // DIAGNOSTICS_OVEN_OVENHEATINGTESTWITHLIQUID_H
