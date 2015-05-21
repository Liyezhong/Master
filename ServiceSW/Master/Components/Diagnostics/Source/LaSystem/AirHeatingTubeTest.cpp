/****************************************************************************/
/*! \file AirHeatingTubeTest.cpp
 *
 *  \brief Implementation of L&A lir heating tube test.
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
#include <QDebug>
#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"
#include "Diagnostics/Include/LaSystem/AirHeatingTubeTest.h"

namespace Diagnostics {

namespace LaSystem {

CAirHeatingTubeTest::CAirHeatingTubeTest(CDiagnosticMessageDlg *_dlg)
    : CTestBase()
    , dlg(_dlg)
    , timingDialog(new LaTimingDialog(dlg->ParentWidget()))
{
}

CAirHeatingTubeTest::~CAirHeatingTubeTest(void)
{
    try {
        delete timingDialog;
    } catch (...) {
    }
}


void CAirHeatingTubeTest::RefreshWaitingDialog(struct airHeatingStatus *status)
{
    Service::ModuleTestStatus refresh;

    refresh["Duration"] = QTime().addSecs(status->EDTime).toString("hh:mm:ss");
    refresh["UsedTime"] = QTime().addSecs(status->UsedTime).toString("hh:mm:ss");

    refresh["TargetTemp"] = status->TargetTemp;
    refresh["CurrentTemp"] = tr("%1").arg(status->CurrentTemp);

    timingDialog->UpdateLabel(refresh);
}

int CAirHeatingTubeTest::Run(void)
{
    qDebug() << "air heating tube test";

    QString title((tr("Air Heating Tube Test")));
    QString text;
    int ret, i;
    bool isAbove;
    int count;
    struct airHeatingStatus heatingStatus;

    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("AirHeatingTest");

    qreal roomTempMin = p_TestCase->GetParameter("RoomTempMin").toFloat();
    qreal roomTempMax = p_TestCase->GetParameter("RoomTempMax").toFloat();

    text = tr("Please check room temperature.<br/>"
             "Is it within %1\260C-%2\260C ?").arg(roomTempMin).arg(roomTempMax);
    ret = dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::YES_NO);
    if (ret == CDiagnosticMessageDlg::NO) {
       text = tr("Test has to be aborted. Please inform the "
                 "user that the Instrument is operated out of the "
                 "operating temperature range of  %1\260C-%2\260C.").arg(roomTempMin).arg(roomTempMax);
       dlg->ShowMessage(title, text, RETURN_ABORT);
       return RETURN_ABORT;
    }

    ServiceDeviceProcess* dev = ServiceDeviceProcess::Instance();

    qreal currentTemp;
    qreal tempMaintainRangeMin = p_TestCase->GetParameter("AirTempMaintainRangeMin").toFloat();
    qreal tempMaintainRangeMax = p_TestCase->GetParameter("AirTempMaintainRangeMax").toFloat();
    qreal AirTempAbove = p_TestCase->GetParameter("AirTempAbove").toFloat();
    int AirRepeatTime = p_TestCase->GetParameter("AirRepeatTime").toInt();
    int AirMaintainTime = p_TestCase->GetParameter("AirMaintainTime").toInt();

    qreal AirTargetTemp = p_TestCase->GetParameter("AirTargetTemp").toFloat();
    (void)dev->AirTubeStartHeating(AirTargetTemp);

    //-------------------------------------------------------------------------------
    qreal AirCurrentTemp;
    (void)dev->AirTubeGetTemp(&AirCurrentTemp);

    qreal AirCurrentTempMin = p_TestCase->GetParameter("AirCurrentTempMin").toFloat();
    qreal AirCurrentTempMax = p_TestCase->GetParameter("AirCurrentTempMax").toFloat();
    if (AirCurrentTemp < AirCurrentTempMin || AirCurrentTemp > AirCurrentTempMax) {
        text = tr("Air Heating Tube Test failed.<br/>"
                  "Please check Air heating tube, cables "
                  "and connections and ASB15 board. "
                  "Replace the defective part accordingly.");
        ret = RETURN_ERR_FAIL;
        goto __fail__;
    }

    timingDialog->SetTitle(title);

    heatingStatus.UsedTime = 0;
    heatingStatus.EDTime = AirRepeatTime + AirMaintainTime;
    heatingStatus.TargetTemp = tr("%1 - %2").arg(tempMaintainRangeMin).arg(tempMaintainRangeMax);
    (void)dev->AirTubeGetTemp(&heatingStatus.CurrentTemp);
    timingDialog->show();
    this->RefreshWaitingDialog(&heatingStatus);

    count = AirMaintainTime;
    isAbove = false;
    ret = RETURN_ERR_FAIL;
    for (i = 0; i < (AirRepeatTime + AirMaintainTime) && timingDialog->isVisible(); i++) {
        QTime EndTime = QTime().currentTime().addSecs(1);

        if (dev->AirTubeGetTemp(&currentTemp) != RETURN_OK) {
            break;
        }

        if (currentTemp >= tempMaintainRangeMin && currentTemp <= tempMaintainRangeMax) {
            if (isAbove) {
                count--;
                if (count <= 0) {
                    ret = RETURN_OK;
                    break;
                }
            }
            if (currentTemp >= AirTempAbove) {
                isAbove = true;
            }
        }
        else {
            if (isAbove) {
                isAbove = false;
            }
            if (count != AirMaintainTime) {
                count = AirMaintainTime;
            }
        }
        if (i > AirRepeatTime && count == AirMaintainTime) {
            break;
        }

        heatingStatus.UsedTime++;
        heatingStatus.CurrentTemp = currentTemp;
        this->RefreshWaitingDialog(&heatingStatus);
        int MSec = QTime().currentTime().msecsTo(EndTime);
        if (MSec <= 0) {
           MSec = 1000;
        }
        dev->Pause(MSec);

    }

    if (!timingDialog->isVisible()) {
        ret = RETURN_ABORT;
        goto __abort__;
    }
    timingDialog->accept();
    if (ret != RETURN_OK)
        text = tr("Air Heating Tube Test failed.<br/>"
                  "Please check Air heating tube, cables "
                  "and connections and ASB15 board. "
                  "Replace the defective part accordingly.");
    else
        text = tr("Air Heating Tube Test successful.");

//__ok__:
__fail__:
    dlg->ShowMessage(title, text, (ErrorCode_t)ret);

__abort__:
    (void)dev->AirTubeStopHeating();
    return ret;
}

} // namespace LaSystem

} // namespace Diagnostics
