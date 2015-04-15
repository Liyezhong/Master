/****************************************************************************/
/*! \file CoverSensorTest.h
 *
 *  \brief Declaration of Oven cover sensor test.
 *
 *   $Version: $ 0.2
 *   $Date:    $ 2014-10-10
 *   $Author:  $ Arthur LI
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

#ifndef DIAGNOSTICS_OVEN_COVERSENSORTEST_H
#define DIAGNOSTICS_OVEN_COVERSENSORTEST_H

#include "Diagnostics/Include/TestBase.h"
#include "Diagnostics/Include/DiagnosticMessageDlg.h"

namespace Diagnostics {

namespace Oven {

/****************************************************************************/
/**
 * \brief This class is a helper to test cover sensor.
 *
 */
/****************************************************************************/
class CCoverSensorTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam _dlg = message widget
     */
    /****************************************************************************/
    CCoverSensorTest(CDiagnosticMessageDlg *_dlg);
    /****************************************************************************/
    /**
     * \brief Deconstructor.
     */
    /****************************************************************************/
    ~CCoverSensorTest(void);

    /****************************************************************************/
    /*!
     *  \brief The function for test start run
     *  \return Error code
     */
    /****************************************************************************/
    int Run(void);

private:
    enum TestCaseRet {
        __FAIL__,
        __CANCEL__,
        __OK__
    };

    /****************************************************************************/
    /*!
     *  \brief To test a case
     */
    /****************************************************************************/
    TestCaseRet TestCase(QString testStatus);

protected:

    /****************************************************************************/
    /*!
     *  \brief To show confirm dialog
     *  \iparam title = dialog title
     *  \iparam text =  test status
     *  \iparam value = cover sensor status
     *  \return clicked code.
     */
    /****************************************************************************/
    virtual int CoverSensorStatusConfirmDlg(QString &title, QString &testStatus, QString &value);

private:
    CDiagnosticMessageDlg *dlg; //!< For show message dialog

};

} // namespace Oven

} // namespace Diagnostics

#endif // DIAGNOSTICS_OVEN_COVERSENSORTEST_H
