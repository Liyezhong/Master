/****************************************************************************/
/*! \file ACVoltageTest.cpp
 *
 *  \brief Implementation of AC voltage selection test..
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

#include "Diagnostics/Include/InitialSystem/ACVoltageTest.h"
#include "MainMenu/Include/MessageDlg.h"

#include <QDebug>
#include "Diagnostics/Include/ServiceDeviceProcess/ServiceDeviceProcess.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

#include "DiagnosticsManufacturing/Include/Select110v220vDialog.h"

namespace Diagnostics {

namespace InitialSystem {

CACVoltageTest::CACVoltageTest(QWidget *parent)
    : CTestBase(parent),
      mp_Parent(parent)
{
}

CACVoltageTest::~CACVoltageTest(void)
{
}

int CACVoltageTest::Run(void)
{
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SMainsRelay");

    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();
    qreal RVTargetTemp = p_TestCase->GetParameter("RVTargetTemp").toFloat();
    qreal RetortTargetTemp = p_TestCase->GetParameter("RetortBottomTargetTemp").toFloat();

    if (p_DevProc == NULL) {
        return RETURN_ERR_NULL_POINTER;
    }

    ShowWaitingMessage(true);

    p_DevProc->MainRelaySetOnOff(true);
    p_DevProc->RVSetTemperatureSwitchState(-1, 1);
    p_DevProc->RetortSetTemperatureSwitchState(-1, 1);
    p_DevProc->RVStartHeating(RVTargetTemp);
    p_DevProc->RetortStartHeating(0, RetortTargetTemp);

    p_DevProc->Pause(5000);

    p_DevProc->RVStopHeating();
    p_DevProc->RetortStopHeating();
    p_DevProc->MainRelaySetOnOff(false);

    ShowWaitingMessage(false);

    quint8 RVSwitchType(0);
    quint8 RetortSwitchType(0);
    p_DevProc->RVGetHeaterSwitchType(&RVSwitchType);
    p_DevProc->RetortGetHeaterSwitchType(&RetortSwitchType);

    qDebug()<<"RVSwitchType="<<RVSwitchType<<"  RetortSwitchType="<<RetortSwitchType;

    if (RetortSwitchType == 0) { // undefined
        ShowFailMessage(1);
        return RETURN_ERR_FAIL;
    }
    else if (RetortSwitchType == RVSwitchType){
        return RETURN_OK;
    }
    else {
        int SwitchType = GetCurrentSwithType();

        if (SwitchType == RVSwitchType) {
            ShowFailMessage(1);
            return RETURN_ERR_FAIL;
        }
        else {
            ShowFailMessage(2);
            return RETURN_ERR_FAIL;
        }
    }

    return RETURN_OK;

}

void CACVoltageTest::ShowWaitingMessage(bool ShowFlag)
{
    if (ShowFlag) {
        QString Title = "AC Voltage Selection Self-test";
        QString Text = Title + " is running...";
        ShowWaitingDialog(Title, Text);
    }
    else {
        HideWaitingDialog();
    }
}

void CACVoltageTest::ShowFailMessage(int Error)
{
    QString Title = "AC Voltage Selection Self-test";
    QString Text;
    if (Error == 1) {
        Text = "AC Voltage Selection Selftest Failed. <br>Please check " \
                "if mains input voltage is within [198-253V AC] or [90-132V AC]." \
                "Alternatively check resistance of Retort bottom heater, cable and function of ASB5." \
                "Resolve the issue accordingly and restart service software.<br>" \
                "Unless this error is resolved, all test functions using AC " \
                "heating are disabled. The instrument must not be used " \
                "by the user unless this error is resolved";
    }
    else if (Error == 2) {
        Text = "AC Voltage Selection Selftest Failed.<br>" \
                "Please check if mains input voltage is within [198-253V AC] or [90-132V AC]. " \
                "Alternatively check resistance of Retort bottom heater, cable and function of ASB3." \
                "Resolve the issue accordingly and restart service software.<br>" \
                "Unless this error is resolved, all test functions using AC heating are " \
                "disabled. The instrument must not be used by the user unless this error " \
                "is resolved.";
    }

    ShowMessage(Title, Text, RETURN_ERR_FAIL);
}

int CACVoltageTest::GetCurrentSwithType()
{
    DiagnosticsManufacturing::CSelect110v220vDialog* p_Dlg = new DiagnosticsManufacturing::CSelect110v220vDialog(110, mp_Parent);
    p_Dlg->UpdateLabelForService();
    (void)p_Dlg->exec();

    int Voltage = p_Dlg->GetSelectedVoltage();
    delete p_Dlg;

    if (Voltage == 110) {
        return 2;
    }
    else {
        return 1;
    }
}

} // namespace InitialSystem

} // namespace Diagnostics
