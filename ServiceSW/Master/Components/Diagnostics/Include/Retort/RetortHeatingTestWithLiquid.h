/****************************************************************************/
/*! \file HeatingTestWithLiquid.h
 *
 *  \brief Declaration of Retort heating test with liquid.
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

#ifndef DIAGNOSTICS_RETORT_HEATINGTESTWITHLIQUID_H
#define DIAGNOSTICS_RETORT_HEATINGTESTWITHLIQUID_H

#include "Diagnostics/Include/TestBase.h"

namespace Diagnostics {

namespace Retort {

class CHeatingTestWithLiquid : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CHeatingTestWithLiquid();
    ~CHeatingTestWithLiquid(void);

    void Run(void);

public Q_SLOTS:
    void FirstCheckBottle(int Position = 1);

private Q_SLOTS:
    void SecondStartHeating(void);

    void Succeed(void);

    void Fail(void);

    void ThirdDrain(void);

    void ForthDisconnectBottle(void);

    void Cancel(void);

private:
    int m_Position;
};

} // namespace Retort

} // namespace Diagnostics

#endif // DIAGNOSTICS_RETORT_HEATINGTESTWITHLIQUID_H
