/****************************************************************************/
/*! \file RVPreTest.cpp
 *
 *  \brief Implementation of Rotary valve pre test..
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

#include "Diagnostics/Include/InitialSystem/RVPreTest.h"
#include "MainMenu/Include/MessageDlg.h"

#include <QDebug>
#include "ServiceDataManager/Include/TestCaseFactory.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"

using namespace DeviceControl;

namespace Diagnostics {

namespace InitialSystem {

CRVPreTest::CRVPreTest(QWidget *p_Parent)
    : CTestBase(p_Parent)
{
}

CRVPreTest::~CRVPreTest(void)
{
}

int CRVPreTest::Run(void)
{
    int Ret(RETURN_OK);

    qreal RVTempSensor1(0);
    qreal RVTempSensor2(0);

    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SRVPreTest");
    qreal TargetTemp = p_TestCase->GetParameter("RVTargetTemp").toFloat();
    qreal DiffTemp = p_TestCase->GetParameter("RVDiffTemp").toFloat();
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();

    ShowWaitingMessage(true);
    qDebug()<<"RVGetTemp";

    Ret = p_DevProc->RVGetTemp(&RVTempSensor1, &RVTempSensor2);
    qDebug()<<"RVGetTemp --- "<<RVTempSensor1;


    if (Ret != RETURN_OK || qAbs(RVTempSensor1-RVTempSensor2)>DiffTemp) {
        ShowWaitingMessage(false);
        ShowFailMessage(1);
        return RETURN_ERR_FAIL;
    }


    Ret = p_DevProc->RVStartHeating(TargetTemp);

    Ret = p_DevProc->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "RV", 0);

    (void)p_DevProc->RVStopHeating();
    ShowWaitingMessage(false);

    if (Ret != RETURN_OK) {
        ShowFailMessage(2);
    }

    return Ret;
}

void CRVPreTest::StartPreHeating()
{
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SRVPreTest");
    qreal TargetTemp = p_TestCase->GetParameter("PreHeatingTargetTemp").toFloat();

    (void)ServiceDeviceProcess::Instance()->RVStartHeating(TargetTemp);
}

void CRVPreTest::ShowWaitingMessage(bool ShowFlag)
{
    if (ShowFlag) {
        QString Title = "Pre-test Rotary Valve";
        QString Text = Title + QString(" is running ...");
        ShowWaitingDialog(Title, Text);
    }
    else {
        HideWaitingDialog();
    }
}

void CRVPreTest::ShowFailMessage(int ErrorCode)
{
    QString Title = "Pre-test Rotary Valve";
    QString Text;

    if (ErrorCode == 1) {
        Text = "Pre-test Rotary Valve failed .<br>" \
                "Temperature sensors are out of specification. Please check " \
                "resistance of temperature sensors to verify ," \
                "exchange Rotary Valve module and repeat this test.";
    }
    else if (ErrorCode == 2) {
        Text = "Pre-test Rotary Valve failed .<br>" \
                "Current of heating elements is out of specifications. Sequentially check " \
                "function of ASB3 and Rotary Valve heating elements." \
                "Exchange Rotary Valve and repeat this test .";
    }

    ShowMessage(Title, Text, RETURN_ERR_FAIL);
}

} // namespace InitialSystem

} // namespace Diagnostics
