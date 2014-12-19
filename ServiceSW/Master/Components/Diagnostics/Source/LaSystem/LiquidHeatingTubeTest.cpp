/****************************************************************************/
/*! \file LiquidHeatingTubeTest.cpp
 *
 *  \brief Implementation of L&A liquid heating tube test
 *
 *   $Version: $ 0.1
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
#include "Diagnostics/Include/LaSystem/LiquidHeatingTubeTest.h"


namespace Diagnostics {

namespace LaSystem {

CLiquidHeatingTubeTest::CLiquidHeatingTubeTest(CDiagnosticMessageDlg *_dlg)
    : CTestBase()
    , dlg(_dlg)
    , timingDialog(new LaTimingDialog(dlg->ParentWidget()))
{
}

CLiquidHeatingTubeTest::~CLiquidHeatingTubeTest(void)
{
    try {
        delete timingDialog;
    } catch (...) {
    }
}


void CLiquidHeatingTubeTest::RefreshWaitingDialog(struct liquidHeatingStatus *status)
{
    Service::ModuleTestStatus refresh;

    refresh["Duration"] = QTime().addSecs(status->EDTime).toString("hh:mm:ss");
    refresh["UsedTime"] = QTime().addSecs(status->UsedTime).toString("hh:mm:ss");

    refresh["TargetTemp"] = status->TargetTemp;
    refresh["CurrentTemp"] = QString().sprintf("%.02f", status->CurrentTemp);

    timingDialog->UpdateLabel(refresh);
}

int CLiquidHeatingTubeTest::Run(void)
{
    qDebug() << "liquid heating tube test";

    QString title((tr("Liquid Heating Tube Test")));
    QString text;
    int ret, i;
    bool isAbove;
    int count;
    struct liquidHeatingStatus heatingStatus;

    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("LiquidHeatingTest");

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

    text = tr("Please look into the retort to identify if it is empty. If yes, click "
               "OK to continue. If no, look at the reagent bottles to identify from which bottle the "
               "reagent came from. Then abort this test and change to the "
               "\"Diagnostic_Retort_Drain Reagent\" function to drain the Air back to "
               "the original position. Thereafter flush the retort if necessary.");
    ret = dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::OK_ABORT);
    if (ret == CDiagnosticMessageDlg::ABORT)
        return RETURN_ABORT;

    ServiceDeviceProcess* dev = ServiceDeviceProcess::Instance();

    qreal currentTemp;
    qreal tempMaintainRangeMin = p_TestCase->GetParameter("LiquidTempMaintainRangeMin").toFloat();
    qreal tempMaintainRangeMax = p_TestCase->GetParameter("LiquidTempMaintainRangeMax").toFloat();
    qreal liquidTempAbove = p_TestCase->GetParameter("LiquidTempAbove").toFloat();
    int liquidRepeatTime = p_TestCase->GetParameter("LiquidRepeatTime").toInt();
    int liquidMaintainTime = p_TestCase->GetParameter("LiquidMaintainTime").toInt();

    (void)dev->LiquidTubeStopHeating();

    qreal liquidTargetTemp = p_TestCase->GetParameter("LiquidTargetTemp").toFloat();
    (void)dev->LiquidTubeStartHeating(liquidTargetTemp);

    //-------------------------------------------------------------------------------
    qreal liquidCurrentTemp;
    (void)dev->LiquidTubeGetTemp(&liquidCurrentTemp);

    qreal liquidCurrentTempMin = p_TestCase->GetParameter("LiquidCurrentTempMin").toFloat();
    qreal liquidCurrentTempMax = p_TestCase->GetParameter("LiquidCurrentTempMax").toFloat();
    if (liquidCurrentTemp < liquidCurrentTempMin || liquidCurrentTemp > liquidCurrentTempMax) {
        text = tr("Liquid Heating Tube Test failed.<br/>"
                  "Please check liquid heating tube, cables "
                  "and connections and ASB15 board. "
                  "Replace the defective part accordingly.");
        ret = RETURN_ERR_FAIL;
        goto __fail__;
    }


    timingDialog->SetTitle(title);

    heatingStatus.UsedTime = 0;
    heatingStatus.EDTime = liquidRepeatTime + liquidMaintainTime;
    heatingStatus.TargetTemp = tr("%1 - %2").arg(tempMaintainRangeMin).arg(tempMaintainRangeMax);
    (void)dev->LiquidTubeGetTemp(&heatingStatus.CurrentTemp);
    timingDialog->show();
    this->RefreshWaitingDialog(&heatingStatus);

    count = liquidMaintainTime;
    isAbove = false;
    ret = RETURN_ERR_FAIL;

    for (i = 0; i < (liquidRepeatTime + liquidMaintainTime) && timingDialog->isVisible(); i++) {
        QTime EndTime = QTime().currentTime().addSecs(1);

        if (dev->LiquidTubeGetTemp(&currentTemp) != RETURN_OK) {
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
            if (currentTemp >= liquidTempAbove) {
                isAbove = true;
            }
        }
        else {
            if (isAbove) {
                isAbove = false;
            }
            if (count != liquidMaintainTime) {
                count = liquidMaintainTime;
            }
        }
        if (i > liquidRepeatTime && count == liquidMaintainTime) {
            break;
        }

        heatingStatus.UsedTime++;
        heatingStatus.CurrentTemp = currentTemp;
        this->RefreshWaitingDialog(&heatingStatus);

        dev->Pause(QTime().currentTime().msecsTo(EndTime));
    }

    if (!timingDialog->isVisible()) {
        ret = RETURN_ABORT;
        goto __abort__;
    }

    timingDialog->accept();
    if (ret != RETURN_OK)
        text = tr("Liquid Heating Tube Test failed.<br/>"
                  "Please check liquid heating tube, cables "
                  "and connections and ASB15 board. "
                  "Replace the defective part accordingly.");
    else
        text = tr("Liquid Heating Tube Test successful.");

//__ok__:
__fail__:
    dlg->ShowMessage(title, text, (ErrorCode_t)ret);
__abort__:
    return ret;
}

} // namespace LaSystem

} // namespace Diagnostics
