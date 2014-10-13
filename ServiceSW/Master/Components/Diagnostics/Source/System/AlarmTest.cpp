/****************************************************************************/
/*! \file AlarmTest.cpp
 *
 *  \brief Implementation of System alarm test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-10-09
 *   $Author:  $ Dixiong Li
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

#include "Diagnostics/Include/System/AlarmTest.h"

#include <QDebug>
#include "ServiceDataManager/Include/TestCaseFactory.h"

namespace Diagnostics {

namespace System {

CAlarmTest::CAlarmTest(QString AlarmFlag, CDiagnosticMessageDlg* p_MessageDlg, QWidget *p_Parent)
    : CTestBase(p_Parent),
      mp_MessageDlg(p_MessageDlg),
      m_AlarmFlag(AlarmFlag)
{
    m_DialogTitle = m_AlarmFlag + " Alarm Test";
    if (m_AlarmFlag == "Local") {
        m_LocalRemote = 1;
    }
    else {
        m_LocalRemote = 2;
    }
}

CAlarmTest::~CAlarmTest(void)
{
}

int CAlarmTest::Run(void)
{
    qDebug() << "Alarm Test starts!";

    int Ret(0);
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase("SSystemAlarm");
    int DetectTimeOut = p_TestCase->GetParameter("DetectTimeOut").toInt();

    QString WaitText = QString("Please remove any external ") + m_AlarmFlag.toLower()
            + QString(" alarm connection within the next %1 secs").arg(DetectTimeOut);
    mp_MessageDlg->ShowWaitingDialog(m_DialogTitle, WaitText);
    bool CheckStatus = CheckAlarmStatus(DetectTimeOut, false);
    mp_MessageDlg->HideWaitingDialog();
    if (!CheckStatus) {
        ShowFinishDlg(1);
        return Ret;
    }

    WaitText = QString("Please create a shortcut between pin4 and pin5 on the ") + m_AlarmFlag.toLower()
            + QString(" alarm connector for at least 2 sec within the next %1 secs").arg(DetectTimeOut);
    mp_MessageDlg->ShowWaitingDialog(m_DialogTitle, WaitText);
    CheckStatus = CheckAlarmStatus(DetectTimeOut, true);
    mp_MessageDlg->HideWaitingDialog();

    if (!CheckStatus) {
        ShowFinishDlg(1);
        return Ret;
    }

    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();

    p_DevProc->AlarmSetOnOff(m_LocalRemote, true);
    Ret = ShowConfirmDlg(1);

    if (Ret == 0) {
        ShowFinishDlg(1);
        return Ret;
    }

    p_DevProc->AlarmSetOnOff(m_LocalRemote, false);
    Ret = ShowConfirmDlg(2);

    if (Ret == 0) {
        ShowFinishDlg(1);
    }
    else {
        ShowFinishDlg(2);
    }

    return Ret;
}

bool CAlarmTest::CheckAlarmStatus(int TimeOutSec, bool ConnectedFlag)
{
    qint32  RetState;
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();
    qint64 StartTime = QDateTime::currentMSecsSinceEpoch();
    qint64 NowTime   = QDateTime::currentMSecsSinceEpoch();

    while (NowTime - StartTime < TimeOutSec*1000) {
        p_DevProc->AlarmGetState(m_LocalRemote, &RetState);
        qDebug()<<"RetState:"<<RetState;
        if (RetState == (ConnectedFlag ? 0 : 1)) {
            return true;
        }
        p_DevProc->Pause(1000);
        NowTime = QDateTime::currentMSecsSinceEpoch();
    }

    return false;
}

int CAlarmTest::ShowConfirmDlg(int StepNum)
{
    QString Text = "Please Measure and confirm the following resistances:<br>";

    if (StepNum == 1) {
        Text += "Between pin1 and pin2: Less than 10 Ohm<br>"\
                "Between pin1 and pin3: More than 100 kOhm";
    }
    else if (StepNum == 2) {
        Text += "Between pin1 and pin2: More than 100 kOhm<br>"\
                "Between pin1 and pin3: Less than 10 Ohm";
    }

    return mp_MessageDlg->ShowConfirmMessage(m_DialogTitle, Text);
}

void CAlarmTest::ShowFinishDlg(int RetNum)
{
    QString Text;

    ErrorCode_t Ret = RETURN_ERR_FAIL;

    if (RetNum == 1) {
        Text = m_DialogTitle + " failed.<br>Please check " + m_AlarmFlag.toLower() + " alarm connector, cable and ASB15.";
    }
    else if (RetNum == 2) {
        Text = m_DialogTitle + " successful.<br>Please re-connect a formerly connected external alarm system.";
        Ret = RETURN_OK;
    }

    mp_MessageDlg->ShowMessage(m_DialogTitle, Text, Ret);
}

} // namespace System

} // namespace Diagnostics
