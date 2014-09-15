/****************************************************************************/
/*! \file FillingNDrainingTest.h
 *
 *  \brief Declaration of System filling and draining test.
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

#ifndef DIAGNOSTICS_SYSTEM_FILLINGNDRAININGTEST_H
#define DIAGNOSTICS_SYSTEM_FILLINGNDRAININGTEST_H

#include "Diagnostics/Include/TestBase.h"

namespace Diagnostics {

namespace System {

class CFillingNDrainingTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CFillingNDrainingTest();
    ~CFillingNDrainingTest(void);

    int Run(void);

public Q_SLOTS:
    void FirstInputBottlePosition(void);

private Q_SLOTS:
    void SecondCheckBottle(int Position);

    void ThirdFill(void);

    void ForthPressToDrain(void);

    void FifthDrain(void);

    void Succeed(void);

    void Fail(void);

    void Cancel(void);

private:
    int m_Position;
};

} // namespace System

} // namespace Diagnostics

#endif // DIAGNOSTICS_SYSTEM_FILLINGNDRAININGTEST_H
