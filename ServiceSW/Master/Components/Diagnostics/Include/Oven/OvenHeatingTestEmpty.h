/****************************************************************************/
/*! \file OvenHeatingTestEmpty.h
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

/****************************************************************************/
/**
 * \brief Struct for Oven heating heating status data.
 */
/****************************************************************************/
struct heatingTestStatus {
   int UsedTime;        //!< number of used time
   int EDTime;          //!< number end time
   qreal OvenTempTop;   //!< Value of oven top temperature
   qreal OvenTempSensor1;   //!< Value of Oven sensor1 temperature
   qreal OvenTempSensor2;   //!< Value of Oven sensor2 temperature

   qreal OvenTempTopTarget; //!< value of oven top target temperature
   qreal OvenTempSensor1Target; //!< value of oven sensor 1 target temperature
   qreal OvenTempSensor2Target; //!< value of oven sensor 2 target temperature
   qreal TempOffset;            //!< value of temperature offset
   qreal TempOffsetRangeMin;    //!< value of temperature offset range min
   qreal TempOffsetRangeMax;    //!< value of temperature offset range max
};

/****************************************************************************/
/**
 * \brief This class is class of oven heating test with empty.
 *
 */
/****************************************************************************/
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
     *  \return error code
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
     *  \iparam buf = heating test status
     *  \iparam isShow = flag of show
     */
    /****************************************************************************/
    virtual void RefreshWaitingDialog(struct heatingTestStatus *buf, bool isShow = false);

private:
    CDiagnosticMessageDlg *dlg; //!< For show message dialog
    OvenHeatingTestEmptyTimingDialog *timingDialog;  //!< For show timming message dialog
};

} // namespace Oven

} // namespace Diagnostics

#endif // DIAGNOSTICS_OVEN_HEATINGTESTEMPTY_H
