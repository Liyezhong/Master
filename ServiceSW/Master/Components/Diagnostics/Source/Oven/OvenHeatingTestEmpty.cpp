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
 *  (C) Copyright 2014 by LBS R&D Center Shanghai. All rights reserved.
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
    }
}

void CHeatingTestEmpty::StartPreHeating()
{
    qreal meltPoint = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SGlobal")->GetParameter("PMeltingPoint").toFloat();

    InitialSystem::COvenPreTest OvenPreTest;
    OvenPreTest.StartPreHeating(meltPoint);
}

void CHeatingTestEmpty::RefreshWaitingDialog(struct heatingTestStatus *buf, bool isShow)
{
    Service::ModuleTestStatus currentStatus;

    currentStatus["Duration"] = QTime().addSecs(buf->EDTime).toString("hh:mm:ss");
    currentStatus["UsedTime"] = QTime().addSecs(buf->UsedTime).toString("hh:mm:ss");
    if (buf->TempOffsetRangeMax == 0)
        currentStatus["TargetTempTopRange"] = tr("%1").arg(buf->OvenTempTopTarget);
    else
        currentStatus["TargetTempTopRange"] = tr("%1-%2").arg(buf->OvenTempTopTarget + buf->TempOffsetRangeMin)
                                                       .arg(buf->OvenTempTopTarget + buf->TempOffsetRangeMax);
    currentStatus["CurrentTempTop"] = tr("%1").arg(buf->OvenTempTop);
    currentStatus["CurrentTempBottom1"] = tr("%1").arg(buf->OvenTempSensor1);
    currentStatus["CurrentTempBottom2"] = tr("%1").arg(buf->OvenTempSensor2);

    timingDialog->UpdateLabel(currentStatus);

    if (isShow)
        timingDialog->show();
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
//    qreal OvenBottomTargetTemp;
    qreal max;
    int count;
    qreal targetTempRangeMin;
    qreal targetTempRangeMax;
    struct heatingTestStatus status;
    bool isAbove = false;

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
        ret = RETURN_ERR_FAIL;
        goto _fail_;
    }

    OvenTopTargetTemp = p_TestCase->GetParameter("OvenTopTargetTemp").toFloat();
//    OvenBottomTargetTemp = p_TestCase->GetParameter("OvenBottomTargetTemp").toFloat();

    max = OvenTempTop > OvenTempSensor1 ? OvenTempTop : OvenTempSensor1;
    max = max > OvenTempSensor2 ? max : OvenTempSensor2;

    if (max >= OvenTopTargetTemp) {
        text = QString(tr("Please remove any paraffin bath present in "
                          "the paraffin oven. Then please leave the oven cover "
                          "opened to speed up the cooling process."));
        ret = dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::OK_ABORT);
        if (ret == CDiagnosticMessageDlg::ABORT) {
            ret = RETURN_ABORT;
            goto _abort_;
        }

        qreal TempOffset = p_TestCase->GetParameter("TempOffset").toFloat();
        int t = p_TestCase->GetParameter("t").toInt();
        status.UsedTime = 0;
        status.OvenTempTopTarget = max - TempOffset;
        status.OvenTempSensor1Target = max - TempOffset;
        status.OvenTempSensor2Target = max - TempOffset;
        status.OvenTempSensor1 = OvenTempSensor1;
        status.OvenTempSensor2 = OvenTempSensor2;
        status.OvenTempTop = OvenTempTop;
        status.EDTime = t + 60;
        status.TempOffsetRangeMax = 0;
        status.TempOffsetRangeMin = 0;

        RefreshWaitingDialog(&status, true);
        count = 60;
        for (i = 0; i < t + 60 && timingDialog->isVisible(); i++) {
            QTime EndTime = QTime().currentTime().addSecs(1);
            ret = dev->OvenGetTemp(&OvenTempTopCur, &OvenTempSensor1Cur, &OvenTempSensor2Cur);
            if (ret != RETURN_OK)
                break;

            if (max - OvenTempTopCur >= TempOffset
                    && max - OvenTempSensor1Cur >= TempOffset
                    && max - OvenTempSensor2Cur >= TempOffset) {
                if (!count--)
                    break;
            } else {
                if (i >= t) {
                    ret = RETURN_ERR_FAIL;
                    break;
                }
                count = 60;
            }

            status.OvenTempSensor1 = OvenTempSensor1Cur;
            status.OvenTempSensor2 = OvenTempSensor2Cur;
            status.OvenTempTop = OvenTempTopCur;
            status.UsedTime++;
            RefreshWaitingDialog(&status);
            int MSec = QTime().currentTime().msecsTo(EndTime);
            if (MSec <= 0) {
                MSec = 1000;
            }
            dev->Pause(MSec);
        }

        if (!timingDialog->isVisible()) {
           ret = RETURN_ABORT;
           goto _abort_;
        }

        timingDialog->accept();
        if (ret != RETURN_OK || count > 0) {
            text = tr("Paraffin Oven Heating Test "
                      "(Empty) failed. ASB5 is damaged."
                      "Exchange it and repeat this test.");
            goto _fail_;
        }

        text = tr("Please close oven cover.");
        ret = dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::OK_ABORT);
        if (ret == CDiagnosticMessageDlg::ABORT) {
            ret = RETURN_ABORT;
            goto _abort_;
        }
    } else {
        text = tr("Please make sure there are no "
                  "paraffin baths present in the paraffin oven.<br/>"
                  "Verify the oven surfaces are dry and clean, "
                  "and the oven cover is closed.");
        ret = dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::OK_ABORT);
        if (ret == CDiagnosticMessageDlg::ABORT) {
            ret = RETURN_ABORT;
            goto _abort_;
        }
    }

    ret = dev->OvenGetTemp(&OvenTempTop, &OvenTempSensor1, &OvenTempSensor2);
    if (ret != RETURN_OK) {
        text = tr("Oven Get Temp error");
        goto _fail_;
    }

    max = OvenTempTop > OvenTempSensor1 ? OvenTempTop : OvenTempSensor1;
    max = max > OvenTempSensor2 ? max : OvenTempSensor2;

    status.TempOffset = p_TestCase->GetParameter("TempOffset").toFloat();
    status.TempOffsetRangeMin = p_TestCase->GetParameter("TempOffsetRangeMin").toFloat();
    status.TempOffsetRangeMax = p_TestCase->GetParameter("TempOffsetRangeMax").toFloat();

    (void)dev->OvenStartHeating(max + status.TempOffset, max + status.TempOffset);

    t1 = p_TestCase->GetParameter("t1").toInt();
    t2 = p_TestCase->GetParameter("t2").toInt();

    status.EDTime = t1 + t2;
    status.UsedTime = 0;
    status.OvenTempTopTarget = max + status.TempOffset;
    status.OvenTempSensor1Target = max + status.TempOffset;
    status.OvenTempSensor2Target = max + status.TempOffset;
    status.OvenTempTop = OvenTempTop;
    status.OvenTempSensor1 = OvenTempSensor1;
    status.OvenTempSensor2 = OvenTempSensor2;

    RefreshWaitingDialog(&status, true);
    count = t2;
    targetTempRangeMin = status.OvenTempTopTarget + status.TempOffsetRangeMin;
    targetTempRangeMax = status.OvenTempTopTarget + status.TempOffsetRangeMax;

    ret = RETURN_ERR_FAIL;
    for (i = 0; i < t1 + t2 && timingDialog->isVisible(); i++) {
        QTime EndTime = QTime().currentTime().addSecs(1);

        if (dev->OvenGetTemp(&OvenTempTopCur, &OvenTempSensor1Cur, &OvenTempSensor2Cur) != RETURN_OK) {
            break;
        }

        if (OvenTempTopCur >= targetTempRangeMin &&
                OvenTempSensor1Cur >= targetTempRangeMin &&
                OvenTempSensor2Cur >= targetTempRangeMin )
        {
            if (OvenTempTopCur <= targetTempRangeMax &&
                   OvenTempSensor1Cur <= targetTempRangeMax &&
                   OvenTempSensor2Cur <= targetTempRangeMax ) {
                if (isAbove) {
                    count--;
                }
                if (count <= 0) {
                    ret = RETURN_OK;
                    break;
                }
            }
            else {
                count = t2;
            }

            if (!isAbove) {
                isAbove = true;
            }
        }
        else {
            isAbove = false;
            count = t2;
        }

        if (i > t1 && count == t2) {
            break;
        }

        status.OvenTempTop = OvenTempTopCur;
        status.OvenTempSensor1 = OvenTempSensor1Cur;
        status.OvenTempSensor2 = OvenTempSensor2Cur;
        status.UsedTime++;
        RefreshWaitingDialog(&status);

        int MSec = QTime().currentTime().msecsTo(EndTime);
        if (MSec <= 0) {
            MSec = 1000;
        }
        dev->Pause(MSec);
    }

    if (!timingDialog->isVisible()) {
        ret = RETURN_ABORT;
        goto _abort_;
    }

    timingDialog->accept();

    if (ret != RETURN_OK && isAbove==false && i<=t1+1) {
        text = tr("Paraffin Oven Heating Test (Empty) failed. Temperature did "
                  "not reach %1\260C within %2 mins. "
                  "Root cause might be damaged ASB5 or Paraffin Oven Module. "
                  "Sequentially check resistance of heaters and function of ASB5. "
                  "Exchange defective part accordingly and repeat this test.").arg(targetTempRangeMin).arg(t1/60);
        goto _fail_;
    }
    if (count > 0) {
        text = tr("Paraffin Oven Heating Test (Empty) failed. <br/>"
                  "ASB5 is damaged. Exchange it and repeat this test.");
        ret = RETURN_ERR_FAIL;
        goto _fail_;
    }

    text = tr("Paraffin Oven Heating Test (Empty) "
              "successful. Please re-insert the paraffin "
              "baths and close the oven cover.");

//_ok_:
_fail_:
    dlg->ShowMessage(title, text, (ErrorCode_t)ret);

_abort_:
    (void)dev->OvenStopHeating();
    StartPreHeating();
    return ret;
}

} // namespace Oven

} // namespace Diagnostics
