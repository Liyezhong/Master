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

#ifndef DIAGNOSTICS_OVEN_COVERSENSORTEST_H
#define DIAGNOSTICS_OVEN_COVERSENSORTEST_H

#include "Diagnostics/Include/TestBase.h"
#include "MainMenu/Include/MainWindow.h"

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

} // namespace Oven

} // namespace Diagnostics

#endif // DIAGNOSTICS_OVEN_COVERSENSORTEST_H
