/****************************************************************************/
/*! \file OvenPreTest.h
 *
 *  \brief Declaration of Oven pre test.
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

#ifndef DIAGNOSTICS_INITIALSYSTEM_OVENPRETEST_H
#define DIAGNOSTICS_INITIALSYSTEM_OVENPRETEST_H

#include "Diagnostics/Include/TestBase.h"
#include "MainMenu/Include/MainWindow.h"

namespace Diagnostics {

namespace InitialSystem {

class COvenPreTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    COvenPreTest(QWidget *parent=NULL);
    ~COvenPreTest(void);

    int Run(void);
private:
    QWidget                   *mp_Parent;
};

} // namespace InitialSystem

} // namespace Diagnostics

#endif // DIAGNOSTICS_INITIALSYSTEM_OVENPRETEST_H
