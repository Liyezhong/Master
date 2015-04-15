/****************************************************************************/
/*! \file LidLockTest.cpp
 *
 *  \brief Implementation of Retort Lid Lock test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-10-16
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

#include "Diagnostics/Include/Retort/LidLockTest.h"

#include <QDebug>
#include "Diagnostics/Include/ServiceDeviceProcess/ServiceDeviceProcess.h"
#include "Diagnostics/Include/StatusConfirmDialog.h"

namespace Diagnostics {

namespace Retort {

CLidLockTest::CLidLockTest(CDiagnosticMessageDlg* p_MessageDlg, QWidget *p_Parent)
    : CTestBase(p_Parent),
      mp_MessageDlg(p_MessageDlg)
{
}

CLidLockTest::~CLidLockTest(void)
{
}

int CLidLockTest::Run(void)
{
    qDebug() << "Lid lock test starts!";

    int Ret = ShowConfirmDlg(1);

    if (Ret == 0) {
        return RETURN_ABORT;
    }

    qint32 LidLockState(0);
    QString StateStr;
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();
    (void)p_DevProc->RetortGetLidLockState(&LidLockState);

    if (LidLockState == 0) {
        StateStr = "Close";
    }
    else {
        StateStr = "Open";
    }

    Ret = ShowLidLockStatusDlg(1, StateStr);

    if (Ret == 1) {
        (void)ShowConfirmDlg(2);
        (void)p_DevProc->RetortGetLidLockState(&LidLockState);

        if (LidLockState == 0) {
            StateStr = "Close";
        }
        else {
            StateStr = "Open";
        }

        Ret = ShowLidLockStatusDlg(2, StateStr);
    }

    ShowFinishDlg(Ret);

    if (Ret == 0) {
        return RETURN_ERR_FAIL;
    }
    return RETURN_OK;
}

int CLidLockTest::ShowConfirmDlg(int StepNum)
{
    QString Title = tr("Lid Lock Test");
    QString Text;
    CDiagnosticMessageDlg::BUTTON_TYPE BtnType = CDiagnosticMessageDlg::NEXT_CANCEL;
    if (StepNum == 1) {
        Text = tr("Please unlock the retort lid.");
    }
    else if (StepNum == 2) {
        Text = tr("Please lock the retort lid.");
        BtnType = CDiagnosticMessageDlg::NEXT_CANCEL_DISABLE;
    }

    return mp_MessageDlg->ShowConfirmMessage(Title, Text, BtnType);
}

void CLidLockTest::ShowFinishDlg(int RetNum)
{
    QString Title = tr("Lid Lock Test");
    QString Text;
    ErrorCode_t Ret = RETURN_ERR_FAIL;

    if (RetNum == 0) {
        Text = Title + tr(" failed.");
    }
    else if (RetNum == 1) {
        Text = Title + tr(" successful.");
        Ret = RETURN_OK;
    }

    mp_MessageDlg->ShowMessage(Title, Text, Ret);
}

int CLidLockTest::ShowLidLockStatusDlg(int StepNum, QString& LidLockState)
{
    QString Title = tr("Lid Lock Test");
    QString ConfirmString = tr("Do you see the retort lid '%1'?").arg(StepNum == 1 ? "Open" : "Close");

    Diagnostics::CStatusConfirmDialog ConfirmDlg(mp_Parent);
    ConfirmDlg.SetDialogTitle(Title);
    ConfirmDlg.SetText(ConfirmString);
    Service::ModuleTestStatus Status;
    QString Key("LidLockerStatus");
    (void)Status.insert(Key, LidLockState);
    (void)Status.insert("StepNum", QString("%1").arg(StepNum));

    ConfirmDlg.UpdateRetortLabel(Status);

    return !ConfirmDlg.exec();
}

} // namespace Retort

} // namespace Diagnostics
