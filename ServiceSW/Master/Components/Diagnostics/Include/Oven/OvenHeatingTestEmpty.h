/****************************************************************************/
/*! \file HeatingTestEmpty.h
 *
 *  \brief Declaration of Heating test empty.
 *
 *   $Version: $ 0.2
 *   $Date:    $ 2014-10-13
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

#ifndef DIAGNOSTICS_OVEN_HEATINGTESTEMPTY_H
#define DIAGNOSTICS_OVEN_HEATINGTESTEMPTY_H

#include "Diagnostics/Include/TestBase.h"
#include "Diagnostics/Include/DiagnosticMessageDlg.h"
#include "Diagnostics/Include/Oven/OvenHeatingTestEmptyTimingDialog.h"
#include <QThread>

namespace Diagnostics {

namespace Oven {
struct heatingTestStatus {
   int UsedTime;
   int EDTime;
   qreal OvenTempTop;
   qreal OvenTempSensor1;
   qreal OvenTempSensor2;

   qreal OvenTempTopTarget;
   qreal OvenTempSensor1Target;
   qreal OvenTempSensor2Target;
   qreal TempOffset;
   qreal TempOffsetRange;
};

class CHeatingTestEmpty : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam _dlg = message dialog
     */
    /****************************************************************************/
    CHeatingTestEmpty(CDiagnosticMessageDlg *_dlg);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CHeatingTestEmpty(void);
    /****************************************************************************/
    /*!
     *  \brief To run test case.
     */
    /****************************************************************************/
    int Run(void);

private:
    /****************************************************************************/
    /*!
     *  \brief To start preheating after test case.
     */
    /****************************************************************************/
    void StartPreHeating(void);

protected:
    /****************************************************************************/
    /*!
     *  \brief To show waiting dialog for test case.
     */
    /****************************************************************************/
    virtual void ShowWaitingDialog(struct heatingTestStatus *buf);

private:
    CDiagnosticMessageDlg *dlg; //!< For show message dialog
    OvenHeatingTestEmptyTimingDialog *timingDialog;  //!< For show timming message dialog
};

} // namespace Oven

} // namespace Diagnostics

#endif // DIAGNOSTICS_OVEN_HEATINGTESTEMPTY_H
