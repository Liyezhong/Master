/****************************************************************************/
/*! \file CoverSensorTest.h
 *
 *  \brief Declaration of Oven cover sensor test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-28
 *   $Author:  $ R.Wu
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

class CCoverSensorTest : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    CCoverSensorTest(CDiagnosticMessageDlg *dlg);
    ~CCoverSensorTest(void);

    int Run(void);
    int TestCase(QString testStatus);
protected:
    virtual int CoverSensorStatusConfirmDlg(QString &title, QString &text, QString &value);

private:
    CDiagnosticMessageDlg *mp_dlg;
};

} // namespace Oven

} // namespace Diagnostics

#endif // DIAGNOSTICS_OVEN_COVERSENSORTEST_H
