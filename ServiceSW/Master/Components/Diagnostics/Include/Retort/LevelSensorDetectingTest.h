/****************************************************************************/
/*! \file LevelSensorDetectingTest.h
 *
 *  \brief Declaration of Retort Level Sensor Detecting test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-10-16
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

#ifndef DIAGNOSTICS_RETORT_LEVELSENSORDETECTINGTEST_H
#define DIAGNOSTICS_RETORT_LEVELSENSORDETECTINGTEST_H

#include "Diagnostics/Include/TestBase.h"
#include "Diagnostics/Include/DiagnosticMessageDlg.h"

namespace Diagnostics {

namespace Retort {

class CLevelSensorDetectingTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CLevelSensorDetectingTest(CDiagnosticMessageDlg* p_MessageDlg, QWidget* p_Parent);
    ~CLevelSensorDetectingTest(void);

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

    void TestRVInitialize();
    void TestRVMovePosition(bool TubeFlag, int Position);

    bool TestDraining(int RetCode, int Positon);

    bool LevelSensorHeating(bool TempFlag);

private:
    CDiagnosticMessageDlg*  mp_MessageDlg;
    QString                 m_MessageTitle;
};

} // namespace Retort

} // namespace Diagnostics

#endif // DIAGNOSTICS_RETORT_LEVELSENSORDETECTINGTEST_H
