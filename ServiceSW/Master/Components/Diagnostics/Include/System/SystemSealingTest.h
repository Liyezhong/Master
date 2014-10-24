/****************************************************************************/
/*! \file SystemSealingTest.h
 *
 *  \brief Declaration of System System sealing test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-10-20
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

#ifndef DIAGNOSTICS_SYSTEM_SYSTEMSEALINGTEST_H
#define DIAGNOSTICS_SYSTEM_SYSTEMSEALINGTEST_H

#include "Diagnostics/Include/TestBase.h"
#include "Diagnostics/Include/DiagnosticMessageDlg.h"

namespace Diagnostics {

namespace System {

/****************************************************************************/
/*!
*   \brief This class implements the functionality to system system sealing test
*/
/****************************************************************************/
class CSystemSealingTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_Message = The pointer of message dlg.
     *  \iparam p_Parent    = Parent widget
     */
    /****************************************************************************/
    CSystemSealingTest(CDiagnosticMessageDlg* p_MessageDlg, QWidget *p_Parent=NULL);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CSystemSealingTest(void);

    /****************************************************************************/
    /*!
     *  \brief The function for test start run
     *  \return 1 means success, other value means failed.
     */
    /****************************************************************************/
    int Run(void);

private:
    /****************************************************************************/
    /*!
     *  \brief To show confirm dialog
     *  \iparam StepNum = the number workflow step.
     *  \return reject or accept.
     */
    /****************************************************************************/
    int ShowConfirmDlg(int StepNum = 0);

    /****************************************************************************/
    /*!
     *  \brief To show finished dialog.
     *  \iparam RetNum = The number return code.
     */
    /****************************************************************************/
    void ShowFinishDlg(int RetNum);

    /****************************************************************************/
    /*!
     *  \brief To create pressure and show status to GUI.
     *  \iparam TargetPressure = the target pressure.
     *  \return true or false.
     */
    /****************************************************************************/
    bool TestCreatePressure(float TargetPressure);

    /****************************************************************************/
    /*!
     *  \brief To release pressure and show status to GUI.
     *  \iparam StopCompressorFlag = true if need to stop compressor.
     */
    /****************************************************************************/
    void TestReleasePressure(bool StopCompressorFlag=true);

    /****************************************************************************/
    /*!
     *  \brief To keep pressure and show status to GUI.
     *  \iparam TargetPressure = the target pressure.
     *  \iparam DiffPressure   = the diff pressure.
     *  \iparam KeepDuration   = keep duration time.
     *  \return true or false
     */
    /****************************************************************************/
    bool TestKeepPressure(float TargetPressure, float DiffPressure, int KeepDuration);
private:
    CDiagnosticMessageDlg*  mp_MessageDlg;  //!< The pointer of message dialog.
    QString                 m_MessageTitle; //!< The message dialog title.
};

} // namespace System

} // namespace Diagnostics

#endif // DIAGNOSTICS_SYSTEM_SYSTEMSEALINGTEST_H
