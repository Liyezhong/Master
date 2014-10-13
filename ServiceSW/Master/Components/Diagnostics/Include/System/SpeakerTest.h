/****************************************************************************/
/*! \file SpeakerTest.h
 *
 *  \brief Declaration of System speaker test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-09-29
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

#ifndef DIAGNOSTICS_SYSTEM_SPEAKERTEST_H
#define DIAGNOSTICS_SYSTEM_SPEAKERTEST_H

#include <QProcess>
#include "Diagnostics/Include/TestBase.h"
#include "Diagnostics/Include/DiagnosticMessageDlg.h"

namespace Diagnostics {

namespace System {

class CSpeakerTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CSpeakerTest(CDiagnosticMessageDlg* p_MessageDlg, QWidget *p_Parent=NULL);
    ~CSpeakerTest(void);

    int Run(void);

private:
    void PlaySound(QString& Volume);
    int ShowConfirmDlg(int StepNum);

    void ShowFinishDlg(int RetNum);

private:
    QProcess*               mp_SpeakProc;
    CDiagnosticMessageDlg*  mp_MessageDlg;
};

} // namespace System

} // namespace Diagnostics

#endif // DIAGNOSTICS_SYSTEM_SPEAKERTEST_H
