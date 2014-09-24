/****************************************************************************/
/*! \file OvenPreTest.cpp
 *
 *  \brief Implementation of Oven pre test..
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-09-15
 *   $Author:  $ Sunny.Qu
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

#include "Diagnostics/Include/InitialSystem/OvenPreTest.h"
#include "MainMenu/Include/MessageDlg.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

#include <QDebug>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"

using namespace DeviceControl;

namespace Diagnostics {

namespace InitialSystem {

COvenPreTest::COvenPreTest(QWidget *p_Parent)
    : CTestBase(p_Parent)
{
}

COvenPreTest::~COvenPreTest(void)
{
}

int COvenPreTest::Run(void)
{
    int Ret(RETURN_OK);

    qreal OvenTempTop(0);
    qreal OvenTempSensor1(0);
    qreal OvenTempSensor2(0);

    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SOvenPreTest");

    qreal DiffTemp = p_TestCase->GetParameter("OvenDiffTemp").toFloat();
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();

    ShowWaitingMessage(true);

    qDebug()<<"OvenGetTemp ---- ";

    Ret = p_DevProc->OvenGetTemp(&OvenTempTop, &OvenTempSensor1, &OvenTempSensor2);
    qDebug()<<"OvenGetTemp ---- "<<OvenTempSensor1<<"  "<<OvenTempSensor2;

    if (Ret != RETURN_OK || qAbs(OvenTempSensor1-OvenTempSensor2) > DiffTemp) {
        ShowWaitingMessage(false);
        ShowFailMessage(1);
        return Ret;
    }

    qreal TopTargetTemp    = p_TestCase->GetParameter("OvenTopTargetTemp").toFloat();
    qreal BottomTargetTemp = p_TestCase->GetParameter("OvenBottomTargetTemp").toFloat();

    Ret = p_DevProc->OvenStartHeating(TopTargetTemp, BottomTargetTemp);

    Ret = p_DevProc->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Oven", 0);
    (void)p_DevProc->OvenStopHeating();
    ShowWaitingMessage(false);

    if (Ret != RETURN_OK) {
        ShowFailMessage(2);
    }
    return Ret;
}

void COvenPreTest::StartPreHeating(qreal MeltPoint)
{
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SOvenPreTest");
    qreal MoreTargetTemp(0);

    if (MeltPoint < 64) {
        MoreTargetTemp = p_TestCase->GetParameter("PreHeatingMoreTargetTemp1").toFloat();
    }
    else {
        MoreTargetTemp = p_TestCase->GetParameter("PreHeatingMoreTargetTemp2").toFloat();
    }

    qDebug()<<"OvenStartHeating   " << MeltPoint+MoreTargetTemp;
    (void)ServiceDeviceProcess::Instance()->OvenStartHeating(MeltPoint+MoreTargetTemp, MeltPoint+MoreTargetTemp);

    p_TestCase->SetParameter("TargetTemp", QString::number(MeltPoint + MoreTargetTemp));

    qDebug()<<"OvenStartHeating";
}

void COvenPreTest::ShowWaitingMessage(bool ShowFlag)
{
    if (ShowFlag) {
        QString Title = "Pre-test Oven";
        QString Text = Title + QString(" is running ...");
        ShowWaitingDialog(Title, Text);
    }
    else {
        HideWaitingDialog();
    }
}

void COvenPreTest::ShowFailMessage(int ErrorCode)
{
    QString Title = "Pre-test Oven";
    QString Text;

    if (ErrorCode == 1) {
        Text = "Pre-test Oven failed .<br>" \
                "Detection of Oven temperature failed. Sequentially check " \
                "resistance of temperature sensors, function of ASB5 and oven heating elements." \
                "exchange ASB5 or oven accordingly and repeat this test.";
    }
    else if (ErrorCode == 2) {
        Text = "Pre-test Oven failed .<br>" \
                "Current of heating elements is out of specifications. Sequentially check " \
                "function of ASB5 and oven heating elements." \
                "Exchange ASB5 or oven accordingly and repeat this test .";
    }

    ShowMessage(Title, Text, RETURN_ERR_FAIL);
}

} // namespace InitialSystem

} // namespace Diagnostics
