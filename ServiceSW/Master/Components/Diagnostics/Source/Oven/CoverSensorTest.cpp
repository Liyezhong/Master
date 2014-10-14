/****************************************************************************/
/*! \file CoverSensorTest.cpp
 *
 *  \brief Implementation of Oven cover sensor test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-10-11
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

#include "Diagnostics/Include/Oven/CoverSensorTest.h"

#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "MainMenu/Include/MessageDlg.h"

#include "ServiceWidget/Include/DlgWizardText.h"
#include "ServiceWidget/Include/DlgConfirmationStatus.h"
#include "DiagnosticsManufacturing/Include/StatusConfirmDialog.h"

namespace Diagnostics {

namespace Oven {

CCoverSensorTest::CCoverSensorTest(CDiagnosticMessageDlg *dlg)
    : CTestBase()
    , mp_dlg(dlg)
{
}

CCoverSensorTest::~CCoverSensorTest(void)
{
}

int CCoverSensorTest::CoverSensorStatusConfirmDlg(QString &title, QString &text, QString &value)
{
    // dlg->ParentWidget() == mp_MainWindow
    DiagnosticsManufacturing::CStatusConfirmDialog confirmDlg(mp_dlg->ParentWidget());
    Service::ModuleTestStatus status;
    QString key("OvenCoverSensorStatus");
    (void)status.insert(key, value);
    confirmDlg.SetDialogTitle(title);
    confirmDlg.SetText(text);
    confirmDlg.UpdateOvenLabel(status);
    return !confirmDlg.exec();
}

int CCoverSensorTest::TestCase(QString testStatus)
{
    QString title((tr("Cover Sensor Test")));
    QString text;
    int ret;

    text = tr("Please %1 the cover sensor manually.").arg(testStatus);
    ret = mp_dlg->ShowConfirmMessage(title, text,
                  testStatus == "open" ? CDiagnosticMessageDlg::NEXT_CANCEL : CDiagnosticMessageDlg::NEXT_CANCEL_DISABLE);
    if (ret == CDiagnosticMessageDlg::CANCEL)
        return RETURN_ERR_FAIL;

    ServiceDeviceProcess *dev = ServiceDeviceProcess::Instance();

    int status;
    QString statusStr;

    enum {
        __ERROR__ = -1,
        __CLOSE__,
        __OPEN__
    };

    (void)dev->OvenGetCoverSensorState(&status);
    qDebug() << "cover sensor state : " << status;

    text = tr("Do you see the cover sensor status shows '%1' ?").arg(testStatus.toUpper());

    if (status == __OPEN__)
        statusStr = tr("OPEN");
    else if (status == __CLOSE__)
        statusStr = tr("CLOSE");
    else
        statusStr = tr("ERROR");

    ret = CoverSensorStatusConfirmDlg(title, text, statusStr);
    if (ret == CDiagnosticMessageDlg::NO)
        return RETURN_ERR_FAIL;

    return RETURN_OK;
}

int CCoverSensorTest::Run(void)
{
    qDebug() << "Cover sensor test starts!";

    QString title((tr("Cover Sensor Test")));
    QString text;

    if (TestCase("open") != RETURN_OK) {
        text = tr("Cover sensor test - Failed");
        mp_dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        return RETURN_ERR_FAIL;
    }

    if (TestCase("close") != RETURN_OK) {
        text = tr("Cover sensor test - Failed");
        mp_dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        return RETURN_ERR_FAIL;
    }

    text = tr("Cover sensor test - Success");
    mp_dlg->ShowMessage(title, text, RETURN_OK);

    return RETURN_OK;
}

} // namespace Oven

} // namespace Diagnostics
