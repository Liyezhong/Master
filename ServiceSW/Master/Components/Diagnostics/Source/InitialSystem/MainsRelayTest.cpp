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
#include "Main/Include/HimalayaServiceEventCodes.h"

namespace Diagnostics {

namespace InitialSystem {

CMainsRelayTest::CMainsRelayTest(QWidget *p_Parent)
    : CTestBase(p_Parent)
{
}

CMainsRelayTest::~CMainsRelayTest(void)
{
}

int CMainsRelayTest::Run(void)
{
    QString TestName = "Mains Relay Selftest";

    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("Start %1.").arg(TestName));


    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SMainsRelay");

    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();
    qreal RVTargetTemp = p_TestCase->GetParameter("RVTargetTemp").toInt();
    qreal OvenTopTargetTemp = p_TestCase->GetParameter("OvenTopTargetTemp").toInt();
    qint32 SwitchOnCurrentHigh = p_TestCase->GetParameter("ASB3CurrentHigh").toInt();
    qint32 SwitchOnCurrentLow = p_TestCase->GetParameter("ASB3CurrentLow").toInt();
    qint32 SwitchOffCurrent = p_TestCase->GetParameter("SwitchOffCurrent").toInt();

    ShowWaitingMessage(true);


    qDebug()<<"MainRelaySetOnOff";

    (void)p_DevProc->MainRelaySetOnOff(true);
    qDebug()<<"RVStartHeating";

    (void)p_DevProc->RVStartHeating(RVTargetTemp);

    qDebug()<<"Pause 2.5 sec";

    p_DevProc->Pause(2500);

    quint16 Current(0);

    qDebug()<<"RVGetCurrent";

    (void)p_DevProc->RVGetCurrent(&Current);

    qDebug()<<"MainsRelayOn Get Current : "<<Current;

    if (Current>=SwitchOnCurrentLow && Current<=SwitchOnCurrentHigh) {
        (void)p_DevProc->MainRelaySetOnOff(false);

        p_DevProc->Pause(3000);
        (void)p_DevProc->RVGetCurrent(&Current);
        qDebug()<<"MainsRelayOff Get Current : "<<Current;
        (void)p_DevProc->RVStopHeating();

        ShowWaitingMessage(false);

        if (Current < SwitchOffCurrent) {
            Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 Successful.").arg(TestName));

            return RETURN_OK;
        }
        else {
            Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 Failed.").arg(TestName));

            ShowFailMessage(1);
            return RETURN_ERR_FAIL;
        }
    }
    else {
        (void)p_DevProc->RVStopHeating();

        (void)p_DevProc->OvenStartHeating(OvenTopTargetTemp, 0);

        p_DevProc->Pause(2500);

        (void)p_DevProc->OvenGetCurrent(&Current, NULL);

        qDebug()<<"MainsRelayOn Get slave5 Current : "<<Current;

         (void)p_DevProc->MainRelaySetOnOff(false);

        qDebug()<<"Initial System Check: Mains Relay test after oven top heating get current:---"<<Current;

        SwitchOnCurrentHigh = p_TestCase->GetParameter("ASB5CurrentHigh").toInt();
        SwitchOnCurrentLow  = p_TestCase->GetParameter("ASB5CurrentLow").toInt();

        if (Current>=SwitchOnCurrentLow && Current<=SwitchOnCurrentHigh) {

            p_DevProc->Pause(3000);
            (void)p_DevProc->OvenGetCurrent(&Current, NULL);
            qDebug()<<"MainsRelayOff Get slave5 Current : "<<Current;

            (void)p_DevProc->OvenStopHeating();

            ShowWaitingMessage(false);

            if (Current < SwitchOffCurrent) {
                Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 Successful.").arg(TestName));

                return RETURN_OK;
            }
            else {
                Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 Failed.").arg(TestName));

                ShowFailMessage(1);
                return RETURN_ERR_FAIL;
            }
        }
        else {
            Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 Failed.").arg(TestName));

            (void)p_DevProc->OvenStopHeating();

            ShowWaitingMessage(false);
            ShowFailMessage(2);
            return RETURN_ERR_FAIL;
        }
    }

    //ShowWaitingMessage(false);
    //return RETURN_OK;
}

void CMainsRelayTest::ShowWaitingMessage(bool ShowFlag)
{
    if (ShowFlag) {
        QString Title = "Mains Relay Selftest";
        QString Text = Title + QString(" is running...");
        ShowWaitingDialog(Title, Text);
    }
    else {
        HideWaitingDialog();
    }
}

void CMainsRelayTest::ShowFailMessage(int Error)
{
    QString Title = "Mains Relay Selftest";
    QString Text;
    if (Error == 1) {
        Text = "Mains Relay Selftest Failed.<br>Please exchange " \
                "Mains relay and repeat the test. If still not successful," \
                "replace ASB15. <br>" \
                "Until this error is resolved, all test functions using AC " \
                "heating are disabled. The instrument must not be used " \
                "by the user until this error is resolved";
    }
    else if (Error == 2) {
        Text = "Mains Relay Selftest Failed.<br>" \
                "Please exchange Mains relay and " \
                "repeat the test. If no success, exchange ASB3 and ASB5.<br>" \
                "Until this error is resolved, all test functions using AC heating are " \
                "disabled. The instrument must not be used by the user until this error " \
                "is resolved.";
    }

    ShowMessage(Title, Text, RETURN_ERR_FAIL);
}

} // namespace InitialSystem

} // namespace Diagnostics
