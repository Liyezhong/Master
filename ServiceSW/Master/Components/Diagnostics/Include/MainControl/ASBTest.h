/****************************************************************************/
/*! \file ASBTest.h
 *
 *  \brief Declaration of Main Control ASB test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-09-15
 *   $Author:  $ Sunny.Qu
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

#ifndef DIAGNOSTICS_ASBTEST_H
#define DIAGNOSTICS_ASBTEST_H

#include "Diagnostics/Include/TestBase.h"
#include "MainMenu/Include/MainWindow.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"

using namespace DeviceControl;
namespace Diagnostics {

namespace MainControl {

class CASBTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CASBTest(HimSlaveType_t SlaveType, QWidget *parent=NULL);
    ~CASBTest(void);

    int Run(void);
private:
    HimSlaveType_t            m_SlaveType;
};

} // namespace MainControl

} // namespace Diagnostics

#endif // DIAGNOSTICS_ASBTEST_H
