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
#include "DiagnosticsManufacturing/Include/HeatingTestDialog.h"
#include "MainMenu/Include/MessageDlg.h"

#include "ServiceWidget/Include/DlgWaitingStatus.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"
#include <QApplication>
#include <QTimer>
#include <QObject>

#include "Diagnostics/Include/Oven/OvenHeatingTestEmpty.h"

namespace Diagnostics {

namespace Oven {

CHeatingTestEmpty::CHeatingTestEmpty(CDiagnosticMessageDlg *_dlg)
    : CTestBase()
    , dlg(_dlg)
    , waitDialog(new DiagnosticsManufacturing::CHeatingTestDialog(Service::OVEN_HEATING_EMPTY, dlg->ParentWidget()))
    , heatingTestEmptyThread(new CHeatingTestEmptyThread)
{
    waitDialog->HideAbort(false);
    waitDialog->BlgProcessProgress(true);
    connect(waitDialog, SIGNAL(AbortBlgProcess(bool)), this, SLOT(AbortDialog(bool)));
}

CHeatingTestEmpty::~CHeatingTestEmpty(void)
{
    try {
        delete waitDialog;
        delete heatingTestEmptyThread;
    } catch (...) {
        qDebug() << __FILE__ << ":" << __FUNCTION__ << __LINE__ << "delete heatingTestEmptyThread & waitDialog, catch error";
    }
}

void CHeatingTestEmpty::Clean()
{
    if (!heatingTestEmptyThread->isFinished()) {
        heatingTestEmptyThread->terminate();
        heatingTestEmptyThread->wait();
    }
    waitDialog->hide();
    emit notifyClose();
}

void CHeatingTestEmpty::AbortDialog(bool status)
{
    Q_UNUSED(status)
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SOvenPreTest");
    ServiceDeviceProcess* dev = ServiceDeviceProcess::Instance();

    qreal OvenTopTargetTemp = p_TestCase->GetParameter("OvenTopTargetTemp").toFloat();
    qreal OvenBottomTargetTemp = p_TestCase->GetParameter("OvenBottomTargetTemp").toFloat();
    dev->OvenStopHeating();
    dev->OvenStartHeating(OvenTopTargetTemp, OvenBottomTargetTemp);
    Clean();
}

void CHeatingTestEmpty::ShowWaitingDialog(QByteArray status)
{
    struct heatingTestStatus *buf = (struct heatingTestStatus *)status.constData();
    Service::ModuleTestStatus currentStatus;

    currentStatus["Duration"] = QTime().addSecs(buf->EDTime).toString("hh:mm:ss");
    currentStatus["UsedTime"] = QTime().addSecs(buf->UsedTime).toString("hh:mm:ss");
    currentStatus["CurrentTempTop"] = tr("%1").arg(buf->OvenTempTop);
    currentStatus["CurrentTempBottom1"] = tr("%1").arg(buf->OvenTempSensor1);
    currentStatus["CurrentTempBottom2"] = tr("%1").arg(buf->OvenTempSensor2);
    currentStatus["TargetTemp"] = tr("%1").arg(buf->OvenTargetTemp);

    if (!waitDialog->isVisible())
            waitDialog->Show();
     waitDialog->UpdateLabel(currentStatus);
}

void CHeatingTestEmpty::ShowMessageBoxSuccess(QString msg)
{
    QString title((tr("Paraffin Oven Heating Test (Empty)")));
    waitDialog->hide();
    dlg->ShowMessage(title, msg, RETURN_OK);
    emit notifyClose();
}

void CHeatingTestEmpty::ShowMessageBoxFail(QString msg)
{
    QString title((tr("Paraffin Oven Heating Test (Empty)")));
    waitDialog->hide();
    dlg->ShowMessage(title, msg, RETURN_ERR_FAIL);
    emit notifyClose();
}

int CHeatingTestEmpty::Run(void)
{
    qDebug() << "Oven Heating test empty Run...";

    QString title((tr("Paraffin Oven Heating Test (Empty)")));
    QString text;
    int ret;

    qreal OvenTempTop;
    qreal OvenTempSensor1;
    qreal OvenTempSensor2;

    qreal OvenTopTargetTemp;
    qreal OvenBottomTargetTemp;

    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("HeatingTestEmpty");

    qreal DiffTemp = p_TestCase->GetParameter("OvenDiffTemp").toFloat();
    ServiceDeviceProcess* dev = ServiceDeviceProcess::Instance();

    (void)dev->OvenStopHeating();

    ret = dev->OvenGetTemp(&OvenTempTop, &OvenTempSensor1, &OvenTempSensor2);

    if (ret != RETURN_OK || qAbs(OvenTempSensor1 - OvenTempSensor2) > DiffTemp) {
        text = QString(tr("Paraffin Oven Heating Test (Empty) failed.<br/> "
                          "Temperature sensors are out of specification. Please "
                          "check resistance of temperature sensors to verify, "
                          "exchange paraffin oven module and repeat this test."));
        dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
        goto out;
    }

    OvenTopTargetTemp = p_TestCase->GetParameter("OvenTopTargetTemp").toFloat();
    OvenBottomTargetTemp = p_TestCase->GetParameter("OvenBottomTargetTemp").toFloat();
    if (OvenTempSensor1 >= OvenTopTargetTemp || OvenTempSensor2 >= OvenBottomTargetTemp) {
        text = QString(tr("Please remove any paraffin bath present in "
                          "the paraffin oven. Then please leave the oven cover "
                          "opened to speed up the cooling process."));
        ret = dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::OK_ABORT);
        if (ret == CDiagnosticMessageDlg::ABORT)
            goto out;
        text = tr("Please waiting...");
        dlg->ShowWaitingDialog(title, text);
        // show a waiting dialog
         // wait a few minutes later, read temp
        int t = p_TestCase->GetParameter("t").toInt();
        int i;
        for (i = 0; i < t; i++) {
            dev->Pause(1000);
            qreal OvenTempSensor1Cur;
            qreal OvenTempSensor2Cur;
            ret = dev->OvenGetTemp(&OvenTempTop, &OvenTempSensor1Cur, &OvenTempSensor2Cur);
            if (ret != RETURN_OK)
                break;
            if (OvenTempSensor1 - OvenTempSensor1Cur >= 10 && OvenTempSensor2 - OvenTempSensor2Cur >= 10)
                break;
        }

        if (ret != RETURN_OK || i == t) {
            dlg->HideWaitingDialog();
            text = tr("Paraffin Oven Heating Test "
                      "(Empty) failed. ASB5 is damaged."
                      "Exchange it and repeat this test.");
            dlg->ShowMessage(title, text, RETURN_ERR_FAIL);
            goto out;
        }

        dlg->HideWaitingDialog();
        text = tr("Please close oven cover.");
        ret = dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::OK_ABORT);
        if (ret == CDiagnosticMessageDlg::ABORT)
            goto out;
    } else {
        text = tr("Please make sure there are no "
                  "paraffin baths present in the paraffin oven.<br/>"
                  "Verify the oven surfaces are dry and clean, "
                  "and the oven cover is closed.");
        ret = dlg->ShowConfirmMessage(title, text, CDiagnosticMessageDlg::OK_ABORT);
        if (ret == CDiagnosticMessageDlg::ABORT)
            goto out;
    }

    ret = dev->OvenGetTemp(&OvenTempTop, &OvenTempSensor1, &OvenTempSensor2);
    if (ret != RETURN_OK)
        goto out;

    heatingTestEmptyThread->status.TempOffset = p_TestCase->GetParameter("TempOffset").toFloat();
    heatingTestEmptyThread->status.TempOffsetRange = p_TestCase->GetParameter("TempOffsetRange").toFloat();

    OvenTempSensor1 += heatingTestEmptyThread->status.TempOffset;
    OvenTempSensor2 += heatingTestEmptyThread->status.TempOffset;

    dev->OvenStartHeating(OvenTempSensor1, OvenTempSensor2);

    heatingTestEmptyThread->t1 = p_TestCase->GetParameter("t1").toInt();
    heatingTestEmptyThread->t2 = p_TestCase->GetParameter("t2").toInt();

    heatingTestEmptyThread->status.EDTime = heatingTestEmptyThread->t1 + heatingTestEmptyThread->t2;
    heatingTestEmptyThread->status.UsedTime = 0;
    heatingTestEmptyThread->status.OvenTargetTemp = OvenTempSensor1;
    heatingTestEmptyThread->status.OvenTempSensor1Target = OvenTempSensor1;
    heatingTestEmptyThread->status.OvenTempSensor2Target = OvenTempSensor2;

    heatingTestEmptyThread->start();
    connect(heatingTestEmptyThread, SIGNAL(freshWaitingDlg(QByteArray)), this, SLOT(ShowWaitingDialog(QByteArray)));
    connect(heatingTestEmptyThread, SIGNAL(finished()), this, SLOT(Clean()));
    connect(heatingTestEmptyThread, SIGNAL(ShowMessageBoxFail(QString)), this, SLOT(ShowMessageBoxFail(QString)), Qt::QueuedConnection);
    connect(heatingTestEmptyThread, SIGNAL(ShowMessageBoxSuccess(QString)), this, SLOT(ShowMessageBoxSuccess(QString)), Qt::QueuedConnection);

    return RETURN_OK;

out:
    Clean();
    return RETURN_ERR_FAIL;
}

void CHeatingTestEmptyThread::run()
{
    QString text;
    int ret;
    int i;

    qreal OvenTempTop;
    qreal OvenTempSensor1Cur;
    qreal OvenTempSensor2Cur;

    ServiceDeviceProcess* dev = ServiceDeviceProcess::Instance();

    for (i = 0; i < t1; i++) {
        dev->Pause(1000);        
        ret = dev->OvenGetTemp(&OvenTempTop, &OvenTempSensor1Cur, &OvenTempSensor2Cur);
        if (ret != RETURN_OK)
            break;
        if (OvenTempSensor1Cur >= status.OvenTempSensor1Target && OvenTempSensor2Cur >= status.OvenTempSensor1Target)
            break;
        status.OvenTempTop = OvenTempTop;
        status.OvenTempSensor1 = OvenTempSensor1Cur;
        status.OvenTempSensor2 = OvenTempSensor2Cur;
        status.UsedTime++;
        sendBuf(&status);
    }

    if (ret != RETURN_OK || i == t1) {
        text = tr("Paraffin Oven Heating Test (Empty) failed. Temperature did "
                  "not reach TM[current temperature +10℃ ] within %1 mins?. "
                  "Root cause might be damaged ASB5 or Paraffin Oven Module. "
                  "Sequentially check resistance of heaters and function of ASB5. "
                  "Exchange defective part accordingly and repeat this test.").arg(t1/60);
        emit ShowMessageBoxFail(text);
        return;
    }    

    qreal OvenTempSensor1 = OvenTempSensor1Cur;
    qreal OvenTempSensor2 = OvenTempSensor2Cur;

    for (int i = 0; i < t2; i++) {
        dev->Pause(1000);
        qreal OvenTempSensor1Cur;
        qreal OvenTempSensor2Cur;
        ret = dev->OvenGetTemp(&OvenTempTop, &OvenTempSensor1Cur, &OvenTempSensor2Cur);
        if (ret != RETURN_OK)
            break;
        if (qAbs(OvenTempSensor1Cur - OvenTempSensor1) > status.TempOffsetRange
                || qAbs(OvenTempSensor2Cur - OvenTempSensor2) > status.TempOffsetRange) {
            ret = RETURN_ERR_FAIL;
            break;
        }
        status.OvenTempTop = OvenTempTop;
        status.OvenTempSensor1 = OvenTempSensor1Cur;
        status.OvenTempSensor2 = OvenTempSensor2Cur;
        status.UsedTime++;
        sendBuf(&status);
    }

    if (ret != RETURN_OK) {
        text = tr("Paraffin Oven Heating Test (Empty) failed. "
                  "ASB5 is damaged. Exchange it and repeat this test.");
        emit ShowMessageBoxFail(text);
        return;
    }

    text = tr("Paraffin Oven Heating Test (Empty) "
              "successful. Please re-insert the paraffin "
              "baths and close the oven cover.");
    emit ShowMessageBoxSuccess(text);
}

void CHeatingTestEmptyThread::sendBuf(struct heatingTestStatus *s)
{
    QByteArray sendbuf = QByteArray::fromRawData((const char *)s, sizeof(*s));
    emit freshWaitingDlg(sendbuf);
}

} // namespace Oven

} // namespace Diagnostics
