/****************************************************************************/
/*! \file AlarmTest.h
 *
 *  \brief Declaration of System alarm test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-10-09
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

#ifndef DIAGNOSTICS_SYSTEM_ALARMTEST_H
#define DIAGNOSTICS_SYSTEM_ALARMTEST_H

#include "Diagnostics/Include/TestBase.h"
#include "Diagnostics/Include/DiagnosticMessageDlg.h"

namespace Diagnostics {

namespace System {

class CAlarmTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CAlarmTest(QString AlarmFlag, CDiagnosticMessageDlg* p_MessageDlg, QWidget *p_Parent=NULL);
    ~CAlarmTest(void);

    int Run(void);

private:
    int ShowConfirmDlg(int StepNum);

    void ShowFinishDlg(int RetNum);

    bool CheckAlarmStatus(int TimeOutSec, bool ConnectedFlag);
private:
    CDiagnosticMessageDlg*  mp_MessageDlg;
    QString                 m_AlarmFlag;
    QString                 m_DialogTitle;
    int                     m_LocalRemote;
};

} // namespace System

} // namespace Diagnostics

#endif // DIAGNOSTICS_SYSTEM_ALARMTEST_H
