/****************************************************************************/
/*! \file LidLockTest.h
 *
 *  \brief Declaration of Retort Lid Lock test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-28
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

#ifndef DIAGNOSTICS_RETORT_LIDLOCKTEST_H
#define DIAGNOSTICS_RETORT_LIDLOCKTEST_H

#include "Diagnostics/Include/TestBase.h"
#include "Diagnostics/Include/DiagnosticMessageDlg.h"

namespace Diagnostics {

namespace Retort {

class CLidLockTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CLidLockTest(CDiagnosticMessageDlg* p_MessageDlg, QWidget *p_Parent=NULL);
    ~CLidLockTest(void);

    int Run(void);

private:
    /****************************************************************************/
    /*!
     *  \brief To show confirm dialog
     *  \iparam StepNum = the number workflow step.
     *  \return reject or accept.
     */
    /****************************************************************************/
    int ShowConfirmDlg(int StepNum);

    /****************************************************************************/
    /*!
     *  \brief To show finished dialog.
     *  \iparam RetNum = The number return code.
     */
    /****************************************************************************/
    void ShowFinishDlg(int RetNum);

    int ShowLidLockStatusDlg(int StepNum, QString& LidLockState);

private:
    CDiagnosticMessageDlg*  mp_MessageDlg;  //!< The pointer of message dialog.
};

} // namespace Retort

} // namespace Diagnostics

#endif // DIAGNOSTICS_RETORT_LIDLOCKTEST_H
