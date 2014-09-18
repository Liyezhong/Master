/****************************************************************************/
/*! \file LTubePreTest.h
 *
 *  \brief Declaration of Liquid tube pre test.
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

#ifndef DIAGNOSTICS_INITALSYSTEM_LTUBEPRETEST_H
#define DIAGNOSTICS_INITALSYSTEM_LTUBEPRETEST_H

#include "Diagnostics/Include/TestBase.h"
#include "MainMenu/Include/MainWindow.h"

namespace Diagnostics {

namespace InitialSystem {

class CLTubePreTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CLTubePreTest(QWidget *parent=NULL);
    ~CLTubePreTest(void);

    int Run(void);

    void StartPreHeating();

private:
    void ShowWaitingMessage(bool ShowFlag=true);
    void ShowFailMessage(int ErrorCode);
};

} // namespace InitialSystem

} // namespace Diagnostics

#endif // DIAGNOSTICS_INITALSYSTEM_LTUBEPRETEST_H
