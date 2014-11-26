/****************************************************************************/
/*! \file DrainReagentTest.h
 *
 *  \brief Declaration of Retort drain reagent test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-11-25
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
#ifndef DRAINREAGENTTEST_H
#define DRAINREAGENTTEST_H

#include "Diagnostics/Include/TestBase.h"
#include "Diagnostics/Include/DiagnosticMessageDlg.h"

namespace Diagnostics {

namespace Retort{

class CDrainReagentTest : public CTestBase
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
    CDrainReagentTest(CDiagnosticMessageDlg* p_MessageDlg, QWidget* p_Parent);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CDrainReagentTest();

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

    bool SelectPosition(int StepNum, bool& TubeFlag, int& Position);

    bool BuildVacuum();
    bool CheckRVTemp(bool IsParaffin);
    bool CheckLTubeTemp();
    bool CheckOvenTemp();

private:
    CDiagnosticMessageDlg*  mp_MessageDlg;      //!< The pointer of message dialog.
    QString                 m_MessageTitle;     //!< The message dialog title string.
};

}// namespace Retort

}// namespace Diagnostics


#endif // DRAINREAGENTTEST_H
