/****************************************************************************/
/*! \file LevelSensorDetectingTest.h
 *
 *  \brief Declaration of Retort Level Sensor Detecting test.
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

#ifndef DIAGNOSTICS_RETORT_LEVELSENSORDETECTINGTEST_H
#define DIAGNOSTICS_RETORT_LEVELSENSORDETECTINGTEST_H

#include "Diagnostics/Include/TestBase.h"

namespace Diagnostics {

namespace Retort {

class CLevelSensorDetectingTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CLevelSensorDetectingTest();
    ~CLevelSensorDetectingTest(void);

    int Run(void);

public Q_SLOTS:
    void FirstInputBottlePosition(void);

private Q_SLOTS:
    void SecondCheckBottle(qint32 Position);

    void ThirdSelfTest();

    void SelfTestSucceed(void);

    void SelfTestFail(void);

    void ForthCheckLiquid(void);

    void OpenLidLock(void);

    void ForthConfirmResult(void);

    void Succeed(void);

    void Fail(void);

    void FifthDrain(void);

    void SixthDisconnectBottle(void);

    void Cancel(void);

private:
    qint32 m_Position;

signals:
    void StartLevelSensorDetectTest(qint32);
};

} // namespace Retort

} // namespace Diagnostics

#endif // DIAGNOSTICS_RETORT_LEVELSENSORDETECTINGTEST_H
