/****************************************************************************/
/*! \file SealingTest.h
 *
 *  \brief Declaration of Rotary valve sealing test.
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

#ifndef DIAGNOSTICS_ROTARYVALVE_SealingTest_H
#define DIAGNOSTICS_ROTARYVALVE_SealingTest_H

#include "Diagnostics/Include/TestBase.h"

namespace Diagnostics {

namespace RotaryValve {

class CSealingTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CSealingTest();
    ~CSealingTest(void);

    int Run(void);

public Q_SLOTS:
    void FirstInitializing(void);

private Q_SLOTS:
    void SecondInputBottlePosition(void);

    void ThirdGetPosition(qint32 Position);

    void ForthSealing(void);

    void Succeed(void);

    void Fail(void);

    void Cancel(void);

private:
    qint32 m_Position;

signals:
    void StartRVSealTest(qint32);
};

} // namespace RotaryValve

} // namespace Diagnostics

#endif // DIAGNOSTICS_ROTARYVALVE_SealingTest_H
