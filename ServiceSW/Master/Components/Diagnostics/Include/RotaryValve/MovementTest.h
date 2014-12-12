/****************************************************************************/
/*! \file MovementTest.h
 *
 *  \brief Declaration of Rotary valve movement test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-10-8
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

#ifndef DIAGNOSTICS_ROTARYVALVE_MOVEMENTTEST_H
#define DIAGNOSTICS_ROTARYVALVE_MOVEMENTTEST_H

#include "Diagnostics/Include/TestBase.h"
#include "Diagnostics/Include/DiagnosticMessageDlg.h"

namespace Diagnostics {

namespace RotaryValve {

/****************************************************************************/
/*!
 *  \brief This is the class of the Rotary valve movement test
 */
/****************************************************************************/
class CMovementTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam dlg = message widget
     */
    /****************************************************************************/
    CMovementTest(CDiagnosticMessageDlg *dlg);
    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    ~CMovementTest(void);

    /****************************************************************************/
    /*!
     *  \brief The function for test start run
     */
    /****************************************************************************/
    int Run();

private:
    CDiagnosticMessageDlg *mp_dlg;  //!< For show message dialog
};

} // namespace RotaryValve

} // namespace Diagnostics

#endif // DIAGNOSTICS_ROTARYVALVE_MOVEMENTTEST_H
