/****************************************************************************/
/*! \file ExhaustFanTest.h
 *
 *  \brief Declaration of System Exhaust Fan test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-10-08
 *   $Author:  $ Dixiong Li
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

#ifndef DIAGNOSTICS_SYSTEM_EXHAUSTFANTEST_H
#define DIAGNOSTICS_SYSTEM_EXHAUSTFANTEST_H

#include "Diagnostics/Include/TestBase.h"
#include "Diagnostics/Include/DiagnosticMessageDlg.h"

namespace Diagnostics {

namespace System {

class CExhaustFanTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CExhaustFanTest(CDiagnosticMessageDlg* p_MessageDlg, QWidget *p_Parent=NULL);
    ~CExhaustFanTest(void);

    int Run(void);

private:
    int ShowConfirmDlg(int StepNum);

    void ShowFinishDlg(int RetNum);
private:
    CDiagnosticMessageDlg*  mp_MessageDlg;
};

} // namespace System

} // namespace Diagnostics

#endif // DIAGNOSTICS_SYSTEM_EXHAUSTFANTEST_H
