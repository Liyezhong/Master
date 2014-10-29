/****************************************************************************/
/*! \file RetortHeatingTestEmpty.cpp
 *
 *  \brief Implementation of Retort Heating test empty.
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
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Diagnostics/Include/Retort/RetortHeatingTestEmpty.h"

namespace Diagnostics {

namespace Retort {

CHeatingTestEmpty::CHeatingTestEmpty(CDiagnosticMessageDlg *_dlg)
    : CTestBase()
    , dlg(_dlg)
    , timingDialog(new RetortTimingDialog(dlg->ParentWidget()))
{
}

CHeatingTestEmpty::~CHeatingTestEmpty(void)
{
    try {
        delete timingDialog;
    } catch (...) {
    }
}

void CHeatingTestEmpty::RefreshWaitingDialog(struct HeatingStatus *status, bool isShow)
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

int CHeatingTestEmpty::Run(void)
{
    qDebug() << "Retort Heating test empty starts!";    

    QString title((tr("Retort Heating Test (Empty)")));
    QString text;
    int ret, i;
    struct HeatingStatus heatingStatus;

    text = tr("Please confirm the retort is empty and dry. "
              "(If not, use the \" Diagnostic_Retort_Drain "
              "Reagen\" function first). Close retort lid and "
              "rotate lock to closed position.");
    ret = dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::OK_ABORT);
    if (ret == CDiagnosticMessageDlg::ABORT)
        return RETURN_OK;

    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("RetortHeatingTestEmpty");
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
        text = tr("Retort Heating Test(Empty) failed.<br/>"
                  "Detection of Retort temperature failed. Sequentially check resistance of temperature "
                  "sensor, function of ASB5 and retort heating elements. Exchange "
                  "ASB5 or retort accordingly and repeat this test.");
        dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        return RETURN_ERR_FAIL;
    }

    DataManager::CTestCase* p_TestCase1 = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SRetortPreTest");
    qreal retortTargetTemp1 = p_TestCase1->GetParameter("RetortTargetTemp").toFloat();
    (void)dev->RetortStartHeating(retortTargetTemp1 + 7, retortTargetTemp1 + 2);
    text = tr("Start heating retort...");
    dlg->ShowWaitingDialog(title, text);
    ret = dev->GetSlaveModuleReportError(DeviceControl::TEMP_CURRENT_OUT_OF_RANGE, "Retort", 0);
    dlg->HideWaitingDialog();

    (void)dev->LSStopHeating();

    if (ret != RETURN_OK) {
        (void)dev->RetortStopHeating();
        text = tr(" Retort Heating Test(Empty) failed.<br/>"
                  "Current of heating elements is out of specifications. "
                  "Sequentially check function of ASB5 and retort heating elements. "
                  "Exchange ASB5 or retort accordingly and repeat this test.");
        dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        return RETURN_ERR_FAIL;
    }

    int t1 = p_TestCase->GetParameter("t1").toInt();
    int t2 = p_TestCase->GetParameter("t2").toInt();
    qreal tempOffset = p_TestCase->GetParameter("TempOffset").toFloat();

    timingDialog->SetTitle(title);
    heatingStatus.UsedTime = 0;
    heatingStatus.EDTime = t1 + t2;
    heatingStatus.RetortTempTarget = tr("%1 - %2").arg(retortSideTargetTemp).arg(retortSideTargetTemp + tempOffset);
    (void)dev->RetortGetTemp(&retortTempSide, &retortTempBottom1, &retortTempBottom2);
    heatingStatus.RetortTempSide = retortTempSide;
    heatingStatus.RetortTempSensor1 = retortTempBottom1;
    heatingStatus.RetortTempSensor2 = retortTempBottom2;
    this->RefreshWaitingDialog(&heatingStatus, true);

    int count = t2; // keep 1 mins
    for (i = 0; i < t1 + t2 && timingDialog->isVisible(); i++) {
        QTime EndTime = QTime().currentTime().addSecs(1);
        if ((ret = dev->RetortGetTemp(&retortTempSide,
                         &retortTempBottom1, &retortTempBottom2)) != RETURN_OK)
            break;
        if (retortTempSide > (retortSideTargetTemp + tempOffset)
                || retortTempBottom1 > retortBottomTargetTemp + tempOffset
                || retortTempBottom2 > retortBottomTargetTemp + tempOffset) {
            ret = RETURN_ERR_FAIL;
            break;
        }
        if (retortTempSide >= retortSideTargetTemp
                && retortTempBottom1 >= retortBottomTargetTemp
                && retortTempBottom2 >= retortBottomTargetTemp) {
            if (!--count)
                break;
        } else {
            if (i >= t1) {
                ret = RETURN_ERR_FAIL;
                break;
            }
            count = t2;
        }

        int MSec = QTime().currentTime().msecsTo(EndTime);
        dev->Pause(MSec);
        heatingStatus.UsedTime++;
        heatingStatus.RetortTempSide = retortTempSide;
        heatingStatus.RetortTempSensor1 = retortTempBottom1;
        heatingStatus.RetortTempSensor2 = retortTempBottom2;
        this->RefreshWaitingDialog(&heatingStatus);
    }

    (void)dev->RetortStopHeating();

    if (!timingDialog->isVisible())
        return RETURN_OK;
    timingDialog->accept();
    if (ret != RETURN_OK || count > 0) {
        // fail
        text = tr("Retort Heating Test (Empty) failed.<br/>"
                  "Sequentially check resistance of "
                  "temperature sensor, function of ASB5 "
                  "and retort. Exchange part accordingly "
                  "and repeat this test.");
        dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        return RETURN_ERR_FAIL;
    }

    text = tr("Retort Heating Test (Empty) successful.");
    dlg->ShowMessage(title, text, RETURN_OK);

    return RETURN_OK;
}


} // namespace Retort

} // namespace Diagnostics
