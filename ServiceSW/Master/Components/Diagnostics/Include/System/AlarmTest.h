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

/****************************************************************************/
/*!
*   \brief This class implements the functionality to system alarm test
*/
/****************************************************************************/
class CAlarmTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam AlarmFlag = The flag of Local and Remote
     *  \iparam p_MessageDlg = The pointer of message dlg.
     *  \iparam p_Parent    = Parent widget
     */
    /****************************************************************************/
    CAlarmTest(QString AlarmFlag, CDiagnosticMessageDlg* p_MessageDlg, QWidget *p_Parent=NULL);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CAlarmTest(void);

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
    int ShowConfirmDlg(int StepNum);

    /****************************************************************************/
    /*!
     *  \brief To show finished dialog.
     *  \iparam RetNum = The number return code.
     */
    /****************************************************************************/
    void ShowFinishDlg(int RetNum);

    /****************************************************************************/
    /*!
     *  \brief To check the alarm connect status.
     *  \iparam TimeOutSec = the second of time out.
     *  \iparam ConnectedFlag = true - connected, false - disconnected.
     *  \return true or false
     */
    /****************************************************************************/
    bool CheckAlarmStatus(int TimeOutSec, bool ConnectedFlag);
private:
    CDiagnosticMessageDlg*  mp_MessageDlg;  //!< The pointer of message dialog.
    QString                 m_AlarmFlag;    //!< "Remote" or "Local"
    QString                 m_DialogTitle;  //!< The string of message dialog title.
    int                     m_LocalRemote;  //!< 1-local, 2-remote.
};

} // namespace System

} // namespace Diagnostics

#endif // DIAGNOSTICS_SYSTEM_ALARMTEST_H
