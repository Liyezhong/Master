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

/****************************************************************************/
/*!
*   \brief This class implements the functionality to retort level sensor detection test
*/
/****************************************************************************/
class CLevelSensorDetectingTest : public Diagnostics::CTestBase
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
    CLevelSensorDetectingTest(CDiagnosticMessageDlg* p_MessageDlg, QWidget* p_Parent);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CLevelSensorDetectingTest(void);

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
     *  \brief To Initialize RV and show status On GUI.
     */
    /****************************************************************************/
    void TestRVInitialize();

    /****************************************************************************/
    /*!
     *  \brief To move RV position and show status on GUI.
     *  \iparam TubeFlag = flag for tube or sealing
     *  \iparam Position = The number RV position.
     */
    /****************************************************************************/
    void TestRVMovePosition(bool TubeFlag, int Position);

    /****************************************************************************/
    /*!
     *  \brief To draining from position and show status on GUI.
     *  \iparam RetCode = The code of the last step result
     *  \iparam Position = The number RV position.
     *  \return true or false
     */
    /****************************************************************************/
    bool TestDraining(int RetCode, int Positon);

    /****************************************************************************/
    /*!
     *  \brief To heating level sensor and show status on GUI.
     *  \iparam TempFlag = flag for Xylene;
     *  \return true or false
     */
    /****************************************************************************/
    bool LevelSensorHeating(bool TempFlag);

private:
    CDiagnosticMessageDlg*  mp_MessageDlg;      //!< The pointer of message dialog.
    QString                 m_MessageTitle;     //!< The message dialog title string.
};

} // namespace Retort

} // namespace Diagnostics

#endif // DIAGNOSTICS_RETORT_LEVELSENSORDETECTINGTEST_H
