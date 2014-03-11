/****************************************************************************/
/*! \file SelectingTest.h
 *
 *  \brief Declaration of Rotary valve selecting test.
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

#ifndef DIAGNOSTICS_ROTARYVALVE_SELECTINGTEST_H
#define DIAGNOSTICS_ROTARYVALVE_SELECTINGTEST_H

#include "Diagnostics/Include/TestBase.h"

namespace Diagnostics {

namespace RotaryValve {

class CSelectingTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CSelectingTest();
    ~CSelectingTest(void);

    void Run(void);

public Q_SLOTS:
    void FirstInitializing(void);

private Q_SLOTS:
    void SecondInputBottlePosition(void);

    void ThirdCheckBottle(qint32 Position);

    void ForthGetPosition(void);

    void FifthCheckBubbles(void);

    void Succeed(void);

    void Fail(void);

    void Cancel(void);

private:
    qint32 m_Position;

signals:
    void StartRVSelectTest(qint32);
};

} // namespace RotaryValve

} // namespace Diagnostics

#endif // DIAGNOSTICS_ROTARYVALVE_SELECTINGTEST_H
