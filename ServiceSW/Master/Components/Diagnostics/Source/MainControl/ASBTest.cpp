/****************************************************************************/
/*! \file ABSTest.cpp
 *
 *  \brief Implementation of Main Control ASB test..
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

#include "Diagnostics/Include/MainControl/ASBTest.h"
#include "MainMenu/Include/MessageDlg.h"

#include <QDebug>
#include "Diagnostics/Include/ServiceDeviceProcess/ServiceDeviceProcess.h"

namespace Diagnostics {

namespace MainControl {

CASBTest::CASBTest(QWidget *parent)
    : CTestBase(),
    mp_Parent(parent)
{
}

CASBTest::~CASBTest(void)
{
}

int CASBTest::Run(void)
{
    HimSlaveType_t SlaveType = Slave_3;
    quint16 Current(0);
    quint16 Voltage(0);
    ErrorCode_t Ret(RETURN_OK);
    qreal VoltageRangeLow = 24*(1-0.1);
    qreal VoltageRangeHigh = 24*(1+0.1);
    qreal CurrentRangeLow = 0;
    qreal CurrentRangeHigh = 200;
    qreal VoltageV = Voltage/1000.0;
    QString CurrentResult("OK");
    QString VoltageResult("OK");


    ServiceDeviceProcess::Instance()->MainControlGetCurrent(SlaveType, &Current);
    ServiceDeviceProcess::Instance()->MainControlGetVoltage(SlaveType, &Voltage);

    if (Current <= CurrentRangeLow || Current >= CurrentRangeHigh ) {
        CurrentResult = "failed";
         Ret = RETURN_ERR_FAIL;
    }

    if (VoltageV <= VoltageRangeLow || VoltageV >= VoltageRangeHigh) {
        Ret = RETURN_ERR_FAIL;
        VoltageResult = "failed";
    }

    QString Text = QString("ASB %1 DC output voltage is %2 (%3V/range %4~%5V), ")
            .arg(SlaveType).arg(VoltageResult).arg(VoltageV).arg(VoltageRangeLow).arg(VoltageRangeHigh);
    QString CurrentText = QString("and current is %1 (%2mA/range %3~%4mA)").
            arg(CurrentResult).arg(Current).arg(CurrentRangeLow).arg(CurrentRangeHigh);
    Text.append(CurrentText);

    QString Title = QString("ASB %1").arg(SlaveType);

    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(mp_Parent);
    dlg->SetTitle(Title);
    if (Ret == RETURN_OK) {
        dlg->SetIcon(QMessageBox::Information);
    }
    else {
        dlg->SetIcon(QMessageBox::Critical);
    }
    dlg->SetText(Text);
    dlg->HideButtons();
    dlg->SetButtonText(1, "OK");
    dlg->setModal(true);
    qDebug()<<Text;

    dlg->exec();

    delete dlg;

    return Ret;
}

} // namespace MainControl

} // namespace Diagnostics
