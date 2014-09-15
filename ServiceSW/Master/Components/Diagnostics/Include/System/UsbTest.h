/****************************************************************************/
/*! \file UsbTest.h
 *
 *  \brief Declaration of USB test.
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

#ifndef DIAGNOSTICS_SYSTEM_USBTEST_H
#define DIAGNOSTICS_SYSTEM_USBTEST_H

#include "Diagnostics/Include/TestBase.h"


namespace Diagnostics {

namespace System {

class CUsbTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CUsbTest();
    ~CUsbTest(void);

    int Run(void);

public Q_SLOTS:
    void FirstConnectUSB(void);

private Q_SLOTS:
    void FirstCheckStatus(void);

    void SecondDisconnectUSB(void);

    void SecondCheckStatus(void);

    void Succeed(void);

    void Fail(void);

    void Cancel(void);

private:
    int m_Position;
};

} // namespace System

} // namespace Diagnostics

#endif // DIAGNOSTICS_SYSTEM_USBTEST_H
