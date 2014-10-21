/****************************************************************************/
/*! \file HeatingBelt1Test.cpp
 *
 *  \brief Implementation of L&A System heating belt 1 test.
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
        qDebug() << __FILE__ << ":" << __FUNCTION__ << __LINE__ << "delete timingDialog, catch error";
    }
}


void CLiquidHeatingTubeTest::ShowWaitingDialog(struct liquidHeatingStatus *status)
{
    Service::ModuleTestStatus refresh;

    refresh["Duration"] = QTime().addSecs(status->EDTime).toString("hh:mm:ss");
    refresh["UsedTime"] = QTime().addSecs(status->UsedTime).toString("hh:mm:ss");

    refresh["TargetTemp"] = status->TargetTemp;
    refresh["CurrentTemp"] = tr("%1").arg(status->CurrentTemp);

    timingDialog->UpdateLabel(refresh);
}

int CLiquidHeatingTubeTest::Run(void)
{
    qDebug() << "liquid heating tube test";

    QString title((tr("Liquid Heating Tube Test")));
    QString text;
    int ret, i;
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
       dlg->ShowMessage(title, text, RETURN_OK);
       return RETURN_OK;
    }

    text = tr("Please look into the retort to identify if it is empty. If yes, click"
              "OK to continue. If no, look at the reagent bottles to identify from which bottle the "
              "reagent came from. Then abort this test and change to the "
              "\"Diagnostic_Retort_Drain Reagent\" function to drain the liquid back to"
              "the original position. Thereafter flush the retort if necessary.");
    ret = dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::OK_ABORT);
    if (ret == CDiagnosticMessageDlg::ABORT)
        return RETURN_OK;

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
            goto __fail__;
    }

    timingDialog->SetTitle(title);
    if (liquidCurrentTemp < liquidTempAbove) {
        heatingStatus.UsedTime = 0;
        heatingStatus.EDTime = liquidRepeatTime + liquidMaintainTime;
        heatingStatus.TargetTemp = tr("%1 ~ %2").arg(tempMaintainRangeMin).arg(tempMaintainRangeMax);
        (void)dev->LiquidTubeGetTemp(&heatingStatus.CurrentTemp);
        timingDialog->show();
        this->ShowWaitingDialog(&heatingStatus);

        for (i = 0; i < liquidRepeatTime && timingDialog->isVisible(); i++) {
            QTime EndTime = QTime().currentTime().addSecs(1);
            if ((ret = dev->LiquidTubeGetTemp(&currentTemp)) != RETURN_OK)
                break;
            if (currentTemp >= liquidTempAbove) {
                break;
            }
            int MSec = QTime().currentTime().msecsTo(EndTime);
            dev->Pause(MSec);
            heatingStatus.UsedTime++;
            heatingStatus.CurrentTemp = currentTemp;
            this->ShowWaitingDialog(&heatingStatus);
        }

        if (!timingDialog->isVisible())
            goto __abort__;
        if (ret != RETURN_OK || i == liquidRepeatTime) {
            // fail
            text = tr("Liquid Heating Tube Test failed.<br/>"
                      "Please check liquid heating tube, cables "
                      "and connections and ASB15 board. "
                      "Replace the defective part accordingly.");
            timingDialog->accept();
            goto __fail__;
        }
    }

    // temp > 78
    if (!timingDialog->isVisible()) {
        heatingStatus.UsedTime = 0;
        heatingStatus.EDTime = liquidMaintainTime;
        heatingStatus.TargetTemp = tr("%1 ~ %2").arg(tempMaintainRangeMin).arg(tempMaintainRangeMax);

        (void)dev->LiquidTubeGetTemp(&heatingStatus.CurrentTemp);
        timingDialog->show();
        this->ShowWaitingDialog(&heatingStatus);
    }

    for (i = 0; i < liquidMaintainTime && timingDialog->isVisible(); i++) {
        QTime EndTime = QTime().currentTime().addSecs(1);
        if ((ret = dev->LiquidTubeGetTemp(&currentTemp)) != RETURN_OK)
            break;
        if (currentTemp < liquidCurrentTempMin || currentTemp > liquidCurrentTempMax) {
            ret = RETURN_ERR_FAIL;
            break;
        }
        int MSec = QTime().currentTime().msecsTo(EndTime);
        dev->Pause(MSec);
        heatingStatus.UsedTime++;
        heatingStatus.CurrentTemp = currentTemp;
        this->ShowWaitingDialog(&heatingStatus);
    }

    if (!timingDialog->isVisible())
        goto __abort__;
    timingDialog->accept();
    if (ret != RETURN_ERR_FAIL)
        text = tr("Liquid Heating Tube Test failed.<br/>"
                  "Please check liquid heating tube, cables "
                  "and connections and ASB15 board. "
                  "Replace the defective part accordingly.");
    else
        text = tr("Liquid Heating Tube Test is successful.");

__ok__:
__fail__:
    dlg->ShowMessage(title, text, (ErrorCode_t)ret);
__abort__:
    return ret;
}

} // namespace LaSystem

} // namespace Diagnostics
