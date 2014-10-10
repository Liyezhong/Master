/****************************************************************************/
/*! \file CoverSensorTest.cpp
 *
 *  \brief Implementation of Oven cover sensor test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-28
 *   $Author:  $ R.Wu
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

#include "Diagnostics/Include/Oven/CoverSensorTest.h"

#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "MainMenu/Include/MessageDlg.h"

#include "ServiceWidget/Include/DlgWizardText.h"
#include "ServiceWidget/Include/DlgConfirmationStatus.h"

namespace Diagnostics {

namespace Oven {

CCoverSensorTest::CCoverSensorTest(CDiagnosticMessageDlg *dlg)
    : CTestBase()
    , dlg(dlg)
{
}

CCoverSensorTest::~CCoverSensorTest(void)
{
}

int CCoverSensorTest::Run(void)
{
    qDebug() << "Cover sensor test starts!";

    QString title((tr(" Cover Sensor Test")));
    QString text;
    int ret;

    text = tr("Please open the cover sensor manually.");
    ret = dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::NEXT_CANCEL);
    if (ret == CDiagnosticMessageDlg::CANCEL)
        return RETURN_ERR_FAIL;

    ServiceDeviceProcess *dev = ServiceDeviceProcess::Instance();
    int status;
    enum {
        __ERROR__ = -1,
        __CLOSE__ = 0,
        __OPEN__  = 1
    };
    dev->OvenGetCoverSensorState(&status);
    qDebug() << "status : " << status;

    if (status == __OPEN__)
        text = tr("<p>Do you see the cover sensor status shows 'OPEN' ?</p>"
                  "<p>	------------------------------------------------</p>"
                  "<p>	| Cover ensor status : <strong><span style=\"color:#E53333;\">OPEN</span></strong>&nbsp; &nbsp;|<strong></strong></p>"
                  "<p>	------------------------------------------------</p>"
                );
    else if (status == __CLOSE__)
        text = tr("<p>Do you see the cover sensor status shows 'OPEN' ?</p>"
                  "<p>	------------------------------------------------</p>"
                  "<p>	| Cover ensor status : <strong><span style=\"color:#E53333;\">CLOSE</span></strong>&nbsp; &nbsp;|<strong></strong></p>"
                  "<p>	------------------------------------------------</p>"
                );
    else
        text = tr("<p>Do you see the cover sensor status shows 'OPEN' ?</p>"
                  "<p>	------------------------------------------------</p>"
                  "<p>	| Cover ensor status : <strong><span style=\"color:#E53333;\">ERROR</span></strong>&nbsp; &nbsp;|<strong></strong></p>"
                  "<p>	------------------------------------------------</p>"
                );
    ret = dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::YES_NO);
    if (ret == CDiagnosticMessageDlg::NO) {
        text = tr("Cover sensor test - Failed");
        dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        return RETURN_ERR_FAIL;
    }

    text = tr("Please close the cover sensor manually.");
    ret = dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::NEXT_CANCEL_DISABLE);

    dev->OvenGetCoverSensorState(&status);
    qDebug() << "status : " << status;

    if (status == __OPEN__)
        text = tr("<p>Do you see the cover sensor status shows 'CLOSE' ?</p>"
                  "<p>	------------------------------------------------</p>"
                  "<p>	| Cover ensor status : <strong><span style=\"color:#E53333;\">OPEN</span></strong>&nbsp; &nbsp;|<strong></strong></p>"
                  "<p>	------------------------------------------------</p>"
                );
    else if (status == __CLOSE__)
        text = tr("<p>Do you see the cover sensor status shows 'CLOSE' ?</p>"
                  "<p>	------------------------------------------------</p>"
                  "<p>	| Cover ensor status : <strong><span style=\"color:#E53333;\">CLOSE</span></strong>&nbsp; &nbsp;|<strong></strong></p>"
                  "<p>	------------------------------------------------</p>"
                );
    else
        text = tr("<p>Do you see the cover sensor status shows 'CLOSE' ?</p>"
                  "<p>	------------------------------------------------</p>"
                  "<p>	| Cover ensor status : <strong><span style=\"color:#E53333;\">ERROR</span></strong>&nbsp; &nbsp;|<strong></strong></p>"
                  "<p>	------------------------------------------------</p>"
                );
    ret = dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::YES_NO);
    if (ret == CDiagnosticMessageDlg::NO) {
        text = tr("Cover sensor test - Failed");
        dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        return RETURN_ERR_FAIL;
    }

    text = tr("Cover sensor test - Success");
    dlg->ShowMessage(title, text, RETURN_OK);

    return RETURN_OK;
}

} // namespace Oven

} // namespace Diagnostics
