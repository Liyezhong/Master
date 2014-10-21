/****************************************************************************/
/*! \file AirHeatingTubeTest.h
 *
 *  \brief Declaration of L&A air heating tube test.
 *
 *   $Version: $ 0.2
 *   $Date:    $ 2014-10-20
 *   $Author:  $ Arthur Li
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2014 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DIAGNOSTICS_LASYSTEM_AIRHEATINGTUBETEST_H
#define DIAGNOSTICS_LASYSTEM_AIRHEATINGTUBETEST_H

#include "Diagnostics/Include/TestBase.h"
#include "Diagnostics/Include/DiagnosticMessageDlg.h"
#include "Diagnostics/Include/LaSystem/LaTimingDialog.h"

namespace Diagnostics {

namespace LaSystem {

struct airHeatingStatus {
   int UsedTime;
   int EDTime;
   QString TargetTemp;
   qreal CurrentTemp;
};

class CAirHeatingTubeTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam _dlg = message dialog
     */
    /****************************************************************************/
    CAirHeatingTubeTest(CDiagnosticMessageDlg *_dlg);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CAirHeatingTubeTest(void);
    /****************************************************************************/
    /*!
     *  \brief To run test case.
     */
    /****************************************************************************/
    int Run(void);

private:
    /****************************************************************************/
    /*!
     *  \brief To show waiting dialog for test case.
     */
    /****************************************************************************/
    void ShowWaitingDialog(struct airHeatingStatus *buf);

private:
    CDiagnosticMessageDlg *dlg; //!< For show message dialog
    LaTimingDialog *timingDialog;  //!< For show timming message dialog

};
} // namespace LaSystem

} // namespace Diagnostics

#endif // DIAGNOSTICS_LASYSTEM_AIRHEATINGTUBETEST_H

