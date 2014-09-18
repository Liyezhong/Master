/****************************************************************************/
/*! \file MainsRelayTest.cpp
 *
 *  \brief Implementation of Mains relay test..
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

#include "Diagnostics/Include/InitialSystem/MainsRelayTest.h"
#include "MainMenu/Include/MessageDlg.h"

#include <QDebug>
#include "Diagnostics/Include/ServiceDeviceProcess/ServiceDeviceProcess.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

namespace Diagnostics {

namespace InitialSystem {

CMainsRelayTest::CMainsRelayTest(QWidget *parent)
    : CTestBase(parent)
{
}

CMainsRelayTest::~CMainsRelayTest(void)
{
}

int CMainsRelayTest::Run(void)
{
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SMainsRelay");

    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();
    qreal RVTargetTemp = p_TestCase->GetParameter("RVTargetTemp").toInt();
    qreal OvenTopTargetTemp = p_TestCase->GetParameter("OvenTopTargetTemp").toInt();
    qint32 SwitchOnCurrentHigh = p_TestCase->GetParameter("SwitchOnCurrentHigh").toInt();
    qint32 SwitchOnCurrentLow = p_TestCase->GetParameter("SwitchOnCurrentLow").toInt();
    qint32 SwitchOffCurrent = p_TestCase->GetParameter("SwitchOffCurrent").toInt();

    ShowWaitingMessage(true);

    p_DevProc->RVStartHeating(RVTargetTemp);
    p_DevProc->MainRelaySetOnOff(true);

    p_DevProc->Pause(2500);

    quint16 Current(0);

    p_DevProc->RVGetCurrent(&Current);

    qDebug()<<"MainsRelayOn Get Current : "<<Current;

    p_DevProc->RVStopHeating();

    if (Current>=SwitchOnCurrentLow && Current<=SwitchOnCurrentHigh) {
        p_DevProc->MainRelaySetOnOff(false);

        p_DevProc->Pause(1000);
        p_DevProc->RVGetCurrent(&Current);
        qDebug()<<"MainsRelayOff Get Current : "<<Current;

        ShowWaitingMessage(false);

        if (Current < SwitchOffCurrent) {
            return RETURN_OK;
        }
        else {
            ShowFailMessage(1);
            return RETURN_ERR_FAIL;
        }
    }
    else {

        p_DevProc->OvenStartHeating(OvenTopTargetTemp, 0);

        p_DevProc->Pause(2500);

        p_DevProc->OvenGetCurrent(&Current, NULL);

        qDebug()<<"MainsRelayOn Get slave5 Current : "<<Current;

        p_DevProc->OvenStopHeating();
        p_DevProc->MainRelaySetOnOff(false);

        qDebug()<<"Initial System Check: Mains Relay test after oven top heating get current:---"<<Current;

        if (Current>=SwitchOnCurrentLow && Current<=SwitchOnCurrentHigh) {

            p_DevProc->Pause(1000);
            p_DevProc->OvenGetCurrent(&Current, NULL);
            qDebug()<<"MainsRelayOff Get slave5 Current : "<<Current;


            ShowWaitingMessage(false);

            if (Current < SwitchOffCurrent) {
                return RETURN_OK;
            }
            else {
                ShowFailMessage(2);
                return RETURN_ERR_FAIL;
            }
        }
        else {
            ShowWaitingMessage(false);
            ShowFailMessage(1);
            return RETURN_ERR_FAIL;
        }
    }

    ShowWaitingMessage(false);
    return RETURN_OK;
}

void CMainsRelayTest::ShowWaitingMessage(bool ShowFlag)
{
    if (ShowFlag) {
        QString Title = "Mains Relay Self-test";
        QString Text = Title + QString(" is running...");
        ShowWaitingDialog(Title, Text);
    }
    else {
        HideWaitingDialog();
    }
}

void CMainsRelayTest::ShowFailMessage(int Error)
{
    QString Title = "Mains Relay Self-test";
    QString Text;
    if (Error == 1) {
        Text = "Mains Relay Selftest Failed. <br>Please exchange " \
                "Mains relay and repeat the test. If still not successful," \
                "replace ASB15. <br>" \
                "Unless this error is resolved, all test functions using AC " \
                "heating are disabled. The instrument must not be used " \
                "by the user unless this error is resolved";
    }
    else if (Error == 2) {
        Text = "Mains Relay Selftest Failed.<br>" \
                "Please exchange Mains relay and " \
                "repeat the test. If no success, exchange ASB3 and ASB5.<br>" \
                "Unless this error is resolved, all test functions using AC heating are " \
                "disabled. The instrument must not be used by the user unless this error " \
                "is resolved.";
    }

    ShowMessage(Title, Text, RETURN_ERR_FAIL);
}

} // namespace InitialSystem

} // namespace Diagnostics
