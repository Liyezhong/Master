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
#include "ServiceDataManager/Include/TestCaseFactory.h"

namespace Diagnostics {

namespace MainControl {

CASBTest::CASBTest(HimSlaveType_t SlaveType, QWidget *parent)
    : CTestBase(),
    mp_Parent(parent),
    m_SlaveType(SlaveType)
{
}

CASBTest::~CASBTest(void)
{
}

int CASBTest::Run(void)
{
    QString TestCaseName;
    HimSlaveType_t SlaveType = m_SlaveType;

    if (SlaveType == Slave_3) {
        TestCaseName = QString("SMCASB3");
    }
    else if (SlaveType == Slave_5) {
        TestCaseName = QString("SMCASB5");
    }
    else if (SlaveType == Slave_15) {
        TestCaseName = QString("SMCASB15");
    }

    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::ServiceInstance().GetTestCase(TestCaseName);

    if (!p_TestCase) {
        qDebug()<<"Diagnostics::CASBTest get test case config failed.";
        return RETURN_ERR_FAIL;
    }

    quint16 Current(0);
    quint16 Voltage(0);
    ErrorCode_t Ret(RETURN_OK);

    qreal VoltageRange = p_TestCase->GetParameter("Voltage").toDouble();
    qreal VoltageTolerance = p_TestCase->GetParameter("VolTolerance").toDouble();
    qreal VoltageRangeLow  = VoltageRange*(1-VoltageTolerance);
    qreal VoltageRangeHigh = VoltageRange*(1+VoltageTolerance);
    qreal CurrentRangeLow = p_TestCase->GetParameter("CurrentLow").toInt();
    qreal CurrentRangeHigh = p_TestCase->GetParameter("CurrentHigh").toInt();

    qDebug()<<"Diagnostics:ASB test current low:"<<CurrentRangeLow;
    qDebug()<<"Diagnostics:ASB test current high:"<<CurrentRangeHigh;
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

    QString Text = QString("ASB%1 DC output voltage is %2 (%3V/range %4~%5V), ")
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
