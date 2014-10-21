/****************************************************************************/
/*! \file RetortHeatingTestWithWater.cpp
 *
 *  \brief Implementation of Retort Heating test with water.
 *
 *   $Version: $ 0.2
 *   $Date:    $ 2014-10-21
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
#include "Diagnostics/Include/LevelSensorHeatingDialog.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Diagnostics/Include/Retort/RetortHeatingTestWithWater.h"
#include "Diagnostics/Include/Retort/RetortInputDialog.h"

namespace Diagnostics {

namespace Retort {

CHeatingTestWithWater::CHeatingTestWithWater(CDiagnosticMessageDlg *_dlg)
    : CTestBase()
    , dlg(_dlg)
    , timingDialog(new RetortTimingDialog(dlg->ParentWidget()))
{
}

CHeatingTestWithWater::~CHeatingTestWithWater(void)
{
    try {
        delete timingDialog;
    } catch (...) {
        qDebug() << __FILE__ << ":" << __FUNCTION__ << __LINE__ << "delete timingDialog, catch error";
    }
}

void CHeatingTestWithWater::ShowWaitingDialog(struct HeatingStatus *status, bool isShow)
{
    Service::ModuleTestStatus refresh;

    refresh["Duration"] = QTime().addSecs(status->EDTime).toString("hh:mm:ss");
    refresh["UsedTime"] = QTime().addSecs(status->UsedTime).toString("hh:mm:ss");

    refresh["TargetTemp"] = status->RetortTempTarget;
    refresh["CurrentTempSide"] = tr("%1").arg(status->RetortTempSide);
    refresh["CurrentTempBottom1"] = tr("%1").arg(status->RetortTempSensor1);
    refresh["CurrentTempBottom2"] = tr("%1").arg(status->RetortTempSensor2);

    timingDialog->UpdateLabel(refresh);

    if (isShow)
        timingDialog->show();
}

int CHeatingTestWithWater::Run(void)
{
    qDebug() << "Retort Heating test empty starts!";    

    QString title((tr("Retort Heating Test (with Water)")));
    QString text;
    int ret, i;
    struct HeatingStatus heatingStatus;
    RetortInputDialog inputDialog(dlg->ParentWidget());
    QString tempExternal;

    text = tr("Please perform Retort Heating Test (Empty).<br/>"
              "This will save time eventually. For this test, you will need a "
              "calibrated external thermometer.");
    ret = dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::OK_ABORT);
    if (ret == CDiagnosticMessageDlg::ABORT)
        return RETURN_OK;

    text = tr(" Please put reagent bottle with water to reagent "
              "bottle position 13. And confirm the retort is empty and "
              "dry. (If not, use the \"Diagnostic_Retort_Drain "
              "Reagent\" function first). Close retort lid and rotate lock "
              "to closed position.");
    ret = dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::OK_ABORT);
    if (ret == CDiagnosticMessageDlg::ABORT)
        return RETURN_OK;

    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("RetortHeatingTestWithWater");
    ServiceDeviceProcess* dev = ServiceDeviceProcess::Instance();

    qreal retortTempSide;
    qreal retortTempBottom1;
    qreal retortTempBottom2;

    qreal diffTemp = p_TestCase->GetParameter("RetortDiffTemp").toFloat();
    qreal retortSideTargetTemp = p_TestCase->GetParameter("RetortSideTargetTemp").toFloat();
    qreal retortBottomTargetTemp = p_TestCase->GetParameter("RetortBottomTargetTemp").toFloat();
//    int t = p_TestCase->GetParameter("t").toInt();

    ret = dev->RetortGetTemp(&retortTempSide, &retortTempBottom1, &retortTempBottom2);
    if (ret != RETURN_OK || qAbs(retortTempBottom1 - retortTempBottom2) >= diffTemp) {
        text = tr("Retort Heating Test(with Water) failed.<br/>"
                  "Detection of Retort temperature failed. Sequentially check resistance of temperature "
                  "sensor, function of ASB5 and retort heating elements. Exchange "
                  "ASB5 or retort accordingly and repeat this test.");
        dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        return RETURN_ERR_FAIL;
    }

    (void)dev->RetortStartHeating(retortSideTargetTemp + 7, retortBottomTargetTemp + 2);
    text = tr("Start heating retort...");
    dlg->ShowWaitingDialog(title, text);
    ret = dev->GetSlaveModuleReportError(DeviceControl::TEMP_CURRENT_OUT_OF_RANGE, "Retort", 0);
    dlg->HideWaitingDialog();

    if (ret != RETURN_OK) {
        (void)dev->RetortStopHeating();
        text = tr("Retort Heating Test(with Water) failed.<br/>"
                  "Current of heating elements is out of specifications. "
                  "Sequentially check function of ASB5 and retort heating elements. "
                  "Exchange ASB5 or retort accordingly and repeat this test.");
        dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        return RETURN_ERR_FAIL;
    }

    //-----
    CLevelSensorHeatingDialog HeatingDlg(dlg->ParentWidget());
    bool HeatingRet = HeatingDlg.StartHeating(false);
    if (!HeatingDlg.result())
        return RETURN_OK;
    if (!HeatingRet) {
        text = tr("Retort Level Sensor Heating Test failed.<br/>"
                  "Target temperature was not reached in time. Please "
                  "perform Level Sensor Detection Test.");
        dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        return RETURN_ERR_FAIL;
    }

    (void)dev->RVInitialize();
    (void)dev->RVMovePosition(true, 13);
    (void)dev->PumpSucking();
    (void)dev->RVMovePosition(false, 13);

    //-----

    int t1 = p_TestCase->GetParameter("t1").toInt();
    qreal tempOffset = p_TestCase->GetParameter("TempOffset").toFloat();

    (void)dev->RetortStopHeating();
    retortSideTargetTemp = p_TestCase->GetParameter("RetortSideTargetTemp1").toFloat();
    retortBottomTargetTemp = p_TestCase->GetParameter("RetortBottomTargetTemp1").toFloat();
    (void)dev->RetortStartHeating(retortSideTargetTemp, retortBottomTargetTemp);

    timingDialog->SetTitle(title);

    heatingStatus.UsedTime = 0;
    heatingStatus.EDTime = t1;
    heatingStatus.RetortTempTarget = tr("%1 ~ %2").arg(retortSideTargetTemp).arg(retortSideTargetTemp + tempOffset);
    (void)dev->RetortGetTemp(&retortTempSide, &retortTempBottom1, &retortTempBottom2);
    heatingStatus.RetortTempSide = retortTempSide;
    heatingStatus.RetortTempSensor1 = retortTempBottom1;
    heatingStatus.RetortTempSensor2 = retortTempBottom2;
    this->ShowWaitingDialog(&heatingStatus, true);

    for (i = 0; i < t1 && timingDialog->isVisible(); i++) {
        QTime EndTime = QTime().currentTime().addSecs(1);
        if ((ret = dev->RetortGetTemp(&retortTempSide,
                         &retortTempBottom1, &retortTempBottom2)) != RETURN_OK)
            break;
        int MSec = QTime().currentTime().msecsTo(EndTime);
        dev->Pause(MSec);
        heatingStatus.UsedTime++;
        heatingStatus.RetortTempSide = retortTempSide;
        heatingStatus.RetortTempSensor1 = retortTempBottom1;
        heatingStatus.RetortTempSensor2 = retortTempBottom2;
        this->ShowWaitingDialog(&heatingStatus);
    }

    if (!timingDialog->isVisible())
        return RETURN_OK;
    timingDialog->accept();
    text = tr("Retort Heating Test (with Water) failed.<br/>"
              "Sequentially check resistance of temperature "
              "sensor, function of ASB5 and retort. Exchange "
              "part accordingly and repeat this test.");
    if ((ret = dev->RetortGetTemp(&retortTempSide,
                     &retortTempBottom1, &retortTempBottom2)) != RETURN_OK)
        goto __fail__;
    if ((retortTempSide > (retortSideTargetTemp + tempOffset) || retortTempSide < retortSideTargetTemp))
        goto __fail__;
    if (retortTempBottom1 > retortBottomTargetTemp + tempOffset || retortTempBottom1 < retortBottomTargetTemp)
        goto __fail__;
    if (retortTempBottom2 > retortBottomTargetTemp + tempOffset || retortTempBottom2 < retortBottomTargetTemp)
        goto __fail__;

    inputDialog.SetTitle(title);

    for (;;) {
        inputDialog.exec();
        ret = inputDialog.getEdit(tempExternal);
        if (ret == 0)
            break;
        if (ret == 1) {
            text = tr("Edit box cannot be empty!");
            dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        }
        if (ret == 2) {
             text = tr("The input value is different!");
            dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        }
    }

    if (tempExternal.toFloat() < retortSideTargetTemp
            || tempExternal.toFloat() > retortSideTargetTemp + tempOffset) {
        text = tr("Retort Heating Test (with Water) failed.<br/>"
                  "Exchange the retort module and repeat this test.");
        ret = RETURN_ERR_FAIL;
        goto __fail__;
    } else {
        text = tr("Retort Heating Test (with Water) successfully completed.");
        ret = RETURN_OK;
        goto __ok__;
    }

__ok__:
__fail__:
    dlg->ShowMessage(title, text, (ErrorCode_t)ret);
__abort__:
    return ret;
}


} // namespace Retort

} // namespace Diagnostics
