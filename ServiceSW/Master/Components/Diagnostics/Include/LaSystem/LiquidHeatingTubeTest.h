/****************************************************************************/
/*! \file LiquidHeatingTubeTest.h
 *
 *  \brief Declaration of L&A liquid heating tube test.
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

#ifndef DIAGNOSTICS_LASYSTEM_LIQUIDHEATINGTUBETEST_H
#define DIAGNOSTICS_LASYSTEM_LIQUIDHEATINGTUBETEST_H


#include "Diagnostics/Include/TestBase.h"
#include "Diagnostics/Include/DiagnosticMessageDlg.h"
#include "Diagnostics/Include/LaSystem/LaTimingDialog.h"

namespace Diagnostics {

namespace LaSystem {

/****************************************************************************/
/**
 * \brief Struct for liquid heating status data.
 */
/****************************************************************************/
struct liquidHeatingStatus {
   int UsedTime;        //!< number of used time
   int EDTime;          //!< number of end time
   QString TargetTemp;  //!< string of target temperature
   qreal CurrentTemp;   //!< current temperature value
};

/****************************************************************************/
/*!
 *  \brief This is the class of the L&A system Liquid heating tube test
 */
/****************************************************************************/
class CLiquidHeatingTubeTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam _dlg = message dialog
     */
    /****************************************************************************/
    CLiquidHeatingTubeTest(CDiagnosticMessageDlg *_dlg);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CLiquidHeatingTubeTest(void);
    /****************************************************************************/
    /*!
     *  \brief To run test case.
     *  \return error code
     */
    /****************************************************************************/
    int Run(void);

private:
    /****************************************************************************/
    /*!
     *  \brief To show waiting dialog for test case.
     */
    /****************************************************************************/
    void RefreshWaitingDialog(struct liquidHeatingStatus *buf);

private:
    CDiagnosticMessageDlg *dlg; //!< For show message dialog
    LaTimingDialog *timingDialog;  //!< For show timming message dialog

};

} // namespace LaSystem

} // namespace Diagnostics

#endif // DIAGNOSTICS_LASYSTEM_LIQUIDHEATINGTUBETEST_H
