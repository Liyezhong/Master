/****************************************************************************/
/*! \file RetortPreTest.cpp
 *
 *  \brief Implementation of Retort pre test..
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

#include "Diagnostics/Include/InitialSystem/RetortPreTest.h"
#include "MainMenu/Include/MessageDlg.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

#include <QDebug>

#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

using namespace DeviceControl;

namespace Diagnostics {

namespace InitialSystem {

CRetortPreTest::CRetortPreTest(QWidget *p_Parent)
    : CTestBase(p_Parent)
{
}

CRetortPreTest::~CRetortPreTest(void)
{
}

int CRetortPreTest::Run(void)
{
    QString TestName = "Pre-test Retort";

    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("Start %1.").arg(TestName));

    int Ret(RETURN_OK);

    qreal RetortTempSide(0);
    qreal RetortTempBottom1(0);
    qreal RetortTempBottom2(0);

    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SRetortPreTest");

    qreal DiffTemp = p_TestCase->GetParameter("RetortDiffTemp").toFloat();
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();

    ShowWaitingMessage(true);

    qDebug()<<"RetortGetTemp";

    Ret = p_DevProc->RetortGetTemp(&RetortTempSide, &RetortTempBottom1, &RetortTempBottom2);
    qDebug()<<"RetortGetTemp --- "<<RetortTempSide;


    if (Ret != RETURN_OK || qAbs(RetortTempBottom1-RetortTempBottom2) > DiffTemp) {
        Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 Failed.").arg(TestName));

        ShowWaitingMessage(false);
        ShowFailMessage(1);
        return RETURN_ERR_FAIL;
    }

    qreal RetortTargetTemp    = p_TestCase->GetParameter("RetortTargetTemp").toFloat();

    Ret = p_DevProc->RetortStartHeating(RetortTargetTemp, RetortTargetTemp);

    Ret = p_DevProc->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Retort", 0);

    (void)p_DevProc->RetortStopHeating();
    ShowWaitingMessage(false);

    if (Ret != RETURN_OK) {
        Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 Failed.").arg(TestName));

        ShowFailMessage(2);
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 Successful.").arg(TestName));
    }

    return Ret;
}

void CRetortPreTest::StartPreHeating(qreal MeltPoint)
{
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SRetortPreTest");
    qreal MoreTargetTemp = p_TestCase->GetParameter("PreHeatingMoreTargetTemp").toFloat();

    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() <<
                                               QString("Start Pre-heating Retort with target temperature %1 centigrade.").arg(MeltPoint+MoreTargetTemp));

    (void)ServiceDeviceProcess::Instance()->RetortStartHeating(MeltPoint+MoreTargetTemp+7, MeltPoint+MoreTargetTemp+2);
    p_TestCase->SetParameter("TargetTemp", QString::number(MeltPoint+MoreTargetTemp));
    qDebug()<<"Set retort Target temp :"<<p_TestCase->GetParameter("TargetTemp");
}

void CRetortPreTest::ShowWaitingMessage(bool ShowFlag)
{
    if (ShowFlag) {
        QString Title = "Pre-test Retort";
        QString Text = Title + QString(" is running ...");
        ShowWaitingDialog(Title, Text);
    }
    else {
        HideWaitingDialog();
    }
}

void CRetortPreTest::ShowFailMessage(int ErrorCode)
{
    QString Title = "Pre-test Retort";
    QString Text;

    if (ErrorCode == 1) {
        Text = "Pre-test Retort failed .<br>" \
                "Detection of Retort temperature failed. Sequentially check " \
                "resistance of temperature sensor, function of ASB5 and retort heating elements." \
                "Exchange ASB5 or retort accordingly and repeat this test.";
    }
    else if (ErrorCode == 2) {
        Text = "Pre-test Retort failed .<br>" \
                "Current of heating elements is out of specifications. Sequentially check " \
                "function of ASB5 and retort heating elements." \
                "Exchange ASB5 or retort accordingly and repeat this test.";
    }

    ShowMessage(Title, Text, RETURN_ERR_FAIL);
}

} // namespace InitialSystem

} // namespace Diagnostics
