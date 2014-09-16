/****************************************************************************/
/*! \file MainsRelayTest.h
 *
 *  \brief Declaration of Mains relay test.
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

#ifndef DIAGNOSTICS_INITALSYSTEM_MAINSRELAYTEST_H
#define DIAGNOSTICS_INITALSYSTEM_MAINSRELAYTEST_H

#include "Diagnostics/Include/TestBase.h"
#include "MainMenu/Include/MainWindow.h"

namespace Diagnostics {

namespace InitialSystem {

class CMainsRelayTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CMainsRelayTest(QWidget *parent=NULL);
    ~CMainsRelayTest(void);

    int Run(void);
private:
    QWidget                   *mp_Parent;
    void ShowWaitingMessage(bool ShowFlag=true);
    void ShowFailMessage(int Error);
};

} // namespace InitialSystem

} // namespace Diagnostics

#endif // DIAGNOSTICS_INITALSYSTEM_MAINSRELAYTEST_H
