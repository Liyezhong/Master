/****************************************************************************/
/*! \file OvenHeatingTestEmpty.cpp
 *
 *  \brief Implementation of Heating test empty.
 *
 *   $Version: $ 1.0
 *   $Date:    $ 2014-10-13
 *   $Author:  $ Arthur Li
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


#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"
#include "MainMenu/Include/MessageDlg.h"

#include "ServiceWidget/Include/DlgWaitingStatus.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"
#include <QApplication>
#include <QTimer>
#include <QObject>

#include "Diagnostics/Include/InitialSystem/OvenPreTest.h"
#include "Diagnostics/Include/Oven/OvenHeatingTestEmpty.h"

namespace Diagnostics {

namespace Oven {

CHeatingTestEmpty::CHeatingTestEmpty(CDiagnosticMessageDlg *_dlg)
    : CTestBase()
    , dlg(_dlg)
    , timingDialog(new OvenHeatingTestEmptyTimingDialog(dlg->ParentWidget()))
{

}

CHeatingTestEmpty::~CHeatingTestEmpty(void)
{
    try {
        delete timingDialog;
    } catch (...) {
        qDebug() << __FILE__ << ":" << __FUNCTION__ << __LINE__ << "delete heatingTestEmptyThread & waitDialog, catch error";
    }
}

void CHeatingTestEmpty::StartPreHeating()
{
    qreal meltPoint = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SGlobal")->GetParameter("PMeltingPoint").toFloat();

    InitialSystem::COvenPreTest OvenPreTest;
    OvenPreTest.StartPreHeating(meltPoint);
}

void CHeatingTestEmpty::ShowWaitingDialog(struct heatingTestStatus *buf)
{
    Service::ModuleTestStatus currentStatus;

    currentStatus["Duration"] = QTime().addSecs(buf->EDTime).toString("hh:mm:ss");
    currentStatus["UsedTime"] = QTime().addSecs(buf->UsedTime).toString("hh:mm:ss");

    currentStatus["CurrentTempTop"] = tr("%1").arg(buf->OvenTempTop);
    currentStatus["TargetTempTop"] = tr("%1").arg(buf->OvenTempTopTarget);

    currentStatus["CurrentTempBottom1"] = tr("%1").arg(buf->OvenTempSensor1);
    currentStatus["CurrentTempBottom2"] = tr("%1").arg(buf->OvenTempSensor2);

    currentStatus["TargetTempBottom"] = tr("%1").arg(buf->OvenTempSensor1Target);

    timingDialog->UpdateLabel(currentStatus);
}

int CHeatingTestEmpty::Run(void)
{
    qDebug() << "Oven Heating test empty Run...";

    QString title((tr("Paraffin Oven Heating Test (Empty)")));
    QString text;
    int ret;
    int i;
    int t1, t2;

    qreal OvenTempTop;
    qreal OvenTempSensor1;
    qreal OvenTempSensor2;

    qreal OvenTempTopCur;
    qreal OvenTempSensor1Cur;
    qreal OvenTempSensor2Cur;

    qreal OvenTopTargetTemp;
    qreal OvenBottomTargetTemp;

    struct heatingTestStatus status;

    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("HeatingTestEmpty");

    qreal DiffTemp = p_TestCase->GetParameter("OvenDiffTemp").toFloat();
    ServiceDeviceProcess* dev = ServiceDeviceProcess::Instance();
    timingDialog->SetTitle(tr("Paraffin Oven Heating Test (Empty)"));

    (void)dev->OvenStopHeating();
    ret = dev->OvenGetTemp(&OvenTempTop, &OvenTempSensor1, &OvenTempSensor2);

    if (ret != RETURN_OK || qAbs(OvenTempSensor1 - OvenTempSensor2) >= DiffTemp
            || qAbs(OvenTempTop - OvenTempSensor1) >= DiffTemp
            || qAbs(OvenTempTop - OvenTempSensor2) >= DiffTemp) {
        text = QString(tr("Paraffin Oven Heating Test (Empty) failed.<br/> "
                          "Temperature sensors are out of specification. Please "
                          "check resistance of temperature sensors to verify, "
                          "exchange paraffin oven module and repeat this test."));
        goto _fail_;
    }

    OvenTopTargetTemp = p_TestCase->GetParameter("OvenTopTargetTemp").toFloat();
    OvenBottomTargetTemp = p_TestCase->GetParameter("OvenBottomTargetTemp").toFloat();
    if (OvenTempTop >= OvenTopTargetTemp || OvenTempSensor1 >= OvenBottomTargetTemp
            || OvenTempSensor2 >= OvenBottomTargetTemp) {
        text = QString(tr("Please remove any paraffin bath present in "
                          "the paraffin oven. Then please leave the oven cover "
                          "opened to speed up the cooling process."));
        ret = dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::OK_ABORT);
        if (ret == CDiagnosticMessageDlg::ABORT)
            goto _abort_;

        qreal TempOffset = p_TestCase->GetParameter("TempOffset").toFloat();
        int t = p_TestCase->GetParameter("t").toInt();
        status.UsedTime = 0;
        status.OvenTempTopTarget = OvenTempTop - TempOffset;
        status.OvenTempSensor1Target = OvenTempSensor1 - TempOffset;
        status.OvenTempSensor2Target = OvenTempSensor2 - TempOffset;
        status.OvenTempSensor1 = OvenTempSensor1;
        status.OvenTempSensor2 = OvenTempSensor2;
        status.OvenTempTop = OvenTempTop;
        status.EDTime = t;

        timingDialog->show();
        for (i = 0; i < t && timingDialog->isVisible(); i++) {
            dev->Pause(1000);
            ret = dev->OvenGetTemp(&OvenTempTopCur, &OvenTempSensor1Cur, &OvenTempSensor2Cur);
            if (ret != RETURN_OK)
                break;
            status.OvenTempSensor1 = OvenTempSensor1Cur;
            status.OvenTempSensor2 = OvenTempSensor2Cur;
            status.OvenTempTop = OvenTempTopCur;
            status.UsedTime++;
            ShowWaitingDialog(&status);
            if (OvenTempTop - OvenTempTopCur >= TempOffset
                    && OvenTempSensor1 - OvenTempSensor1Cur >= TempOffset
                    && OvenTempSensor2 - OvenTempSensor2Cur >= TempOffset)
                break;
        }

        if (!timingDialog->isVisible())
           goto _abort_;

        timingDialog->accept();
        if (ret != RETURN_OK || i == t) {
            text = tr("Paraffin Oven Heating Test "
                      "(Empty) failed. ASB5 is damaged."
                      "Exchange it and repeat this test.");
            goto _fail_;
        }

        text = tr("Please close oven cover.");
        ret = dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::OK_ABORT);
        if (ret == CDiagnosticMessageDlg::ABORT)
            goto _abort_;
    } else {
        text = tr("Please make sure there are no "
                  "paraffin baths present in the paraffin oven.<br/>"
                  "Verify the oven surfaces are dry and clean, "
                  "and the oven cover is closed.");
        ret = dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::OK_ABORT);
        if (ret == CDiagnosticMessageDlg::ABORT)
            goto _abort_;
    }

    ret = dev->OvenGetTemp(&OvenTempTop, &OvenTempSensor1, &OvenTempSensor2);
    if (ret != RETURN_OK) {
        text = tr("Oven Get Temp error");
        goto _fail_;
    }

    status.TempOffset = p_TestCase->GetParameter("TempOffset").toFloat();
    status.TempOffsetRange = p_TestCase->GetParameter("TempOffsetRange").toFloat();

    OvenTempSensor1 += status.TempOffset;
    OvenTempSensor2 += status.TempOffset;
    OvenTempTop     += status.TempOffset;

    (void)dev->OvenStartHeating(OvenTempTop, OvenTempSensor1);

    t1 = p_TestCase->GetParameter("t1").toInt();
    t2 = p_TestCase->GetParameter("t2").toInt();

    status.EDTime = t1 + t2;
    status.UsedTime = 0;
    status.OvenTempTopTarget = OvenTempTop;
    status.OvenTempTop = OvenTempTop;
    status.OvenTempSensor1Target = OvenTempSensor1;
    status.OvenTempSensor2Target = OvenTempSensor2;
    status.OvenTempSensor1 = OvenTempSensor1;
    status.OvenTempSensor2 = OvenTempSensor2;

    timingDialog->show();

    for (i = 0; i < t1 && timingDialog->isVisible(); i++) {
        dev->Pause(1000);
        ret = dev->OvenGetTemp(&OvenTempTopCur, &OvenTempSensor1Cur, &OvenTempSensor2Cur);
        if (ret != RETURN_OK)
            break;
        if (OvenTempTopCur >= status.OvenTempTopTarget
                && OvenTempSensor1Cur >= status.OvenTempSensor1Target
                && OvenTempSensor2Cur >= status.OvenTempSensor1Target)
            break;
        status.OvenTempTop = OvenTempTopCur;
        status.OvenTempSensor1 = OvenTempSensor1Cur;
        status.OvenTempSensor2 = OvenTempSensor2Cur;
        status.UsedTime++;
        ShowWaitingDialog(&status);
    }

    if (i != t1 && !timingDialog->isVisible())
        goto _abort_;

    if (ret != RETURN_OK || i == t1) {
        text = tr("Paraffin Oven Heating Test (Empty) failed. Temperature did "
                  "not reach TM[current temperature +10\260C ] within %1 mins?. "
                  "Root cause might be damaged ASB5 or Paraffin Oven Module. "
                  "Sequentially check resistance of heaters and function of ASB5. "
                  "Exchange defective part accordingly and repeat this test.").arg(t1/60);
        goto _fail_;
    }

    OvenTempTop = OvenTempTopCur;
    OvenTempSensor1 = OvenTempSensor1Cur;
    OvenTempSensor2 = OvenTempSensor2Cur;

    for (i = 0; i < t2 && timingDialog->isVisible(); i++) {
        dev->Pause(1000);
        qreal OvenTempSensor1Cur;
        qreal OvenTempSensor2Cur;
        ret = dev->OvenGetTemp(&OvenTempTopCur, &OvenTempSensor1Cur, &OvenTempSensor2Cur);
        if (ret != RETURN_OK)
            break;
        if (qAbs(OvenTempTopCur - OvenTempTop) > status.TempOffsetRange
                || qAbs(OvenTempSensor1Cur - OvenTempSensor1) > status.TempOffsetRange
                || qAbs(OvenTempSensor2Cur - OvenTempSensor2) > status.TempOffsetRange) {
            ret = RETURN_ERR_FAIL;
            break;
        }
        status.OvenTempTop = OvenTempTop;
        status.OvenTempSensor1 = OvenTempSensor1Cur;
        status.OvenTempSensor2 = OvenTempSensor2Cur;
        status.UsedTime++;
        ShowWaitingDialog(&status);
    }

    if (i != t2 && !timingDialog->isVisible())
        goto _abort_;

    timingDialog->accept();

    if (ret != RETURN_OK) {
        text = tr("Paraffin Oven Heating Test (Empty) failed. "
                  "ASB5 is damaged. Exchange it and repeat this test.");
        goto _fail_;
    }

    text = tr("Paraffin Oven Heating Test (Empty) "
              "successful. Please re-insert the paraffin "
              "baths and close the oven cover.");

//_ok_:
_fail_:
    dlg->ShowMessage(title, text, (ErrorCode_t)ret);

_abort_:
    dev->OvenStopHeating();
    StartPreHeating();
    return ret;
}

} // namespace Oven

} // namespace Diagnostics
