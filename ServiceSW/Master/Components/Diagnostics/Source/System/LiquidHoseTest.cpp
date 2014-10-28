/****************************************************************************/
/*! \file LiquidHoseTest.cpp
 *
 *  \brief Implementation of System Liquid hose test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-10-20
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

#include "Diagnostics/Include/System/LiquidHoseTest.h"
#include "Diagnostics/Include/System/LiquidHoseTestDlg.h"
#include "Diagnostics/Include/SelectBottleNReagentDialog.h"

#include <QDebug>
namespace Diagnostics {

namespace System {

CLiquidHoseTest::CLiquidHoseTest(CDiagnosticMessageDlg* p_MessageDlg, QWidget *p_Parent)
    : CTestBase(p_Parent),
      mp_MessageDlg(p_MessageDlg),
      m_MessageTitle("Liquid Hose Test")
{
}

CLiquidHoseTest::~CLiquidHoseTest(void)
{
}

int CLiquidHoseTest::Run(void)
{
    qDebug() << "Liquid Hose Test starts!";

    int Ret(0);
    if (ShowConfirmDlg(1) == 0) {
        return Ret;
    }

    QString Text = "Please select an option and press OK to continue.";
    CSelectBottleNReagentDialog* p_SelectDlg = new CSelectBottleNReagentDialog(16, mp_Parent);
    p_SelectDlg->SetScrollWheelVisible(false);
    p_SelectDlg->SetTitle(m_MessageTitle);
    p_SelectDlg->SetLableText(Text);
    QString ButtonText1 = "Particular single bottle test";
    QString ButtonText2 = "Full test including all liquid hoses";
    p_SelectDlg->SetRadioButtonText(ButtonText1, ButtonText2);

    if (p_SelectDlg->exec() == 0) {
        delete p_SelectDlg;
        return Ret;
    }

    int Option = p_SelectDlg->GetOption();

    CLiquidHoseTestDlg* p_TestDlg = new CLiquidHoseTestDlg(mp_Parent);

    if (Option == 0) { //Particular single bottle test
        delete p_SelectDlg;
        p_SelectDlg = NULL;
        p_SelectDlg = new CSelectBottleNReagentDialog(16, mp_Parent);
        p_SelectDlg->SetRadioButtonVisible(false);
        p_SelectDlg->SetTitle(m_MessageTitle);
        p_SelectDlg->SetLableText(Text);

        if (p_SelectDlg->exec() == 0) {
            delete p_SelectDlg;
            return Ret;
        }

        int BottleNum = p_SelectDlg->GetBottleNumber();
        p_TestDlg->AddBottleNumber(BottleNum);
    }
    else {
        for (int i = 1; i < 17; ++i){
            p_TestDlg->AddBottleNumber(i);
        }
    }

    p_TestDlg->RunTest();

    delete p_SelectDlg;
    delete p_TestDlg;

    return Ret;
}

int CLiquidHoseTest::ShowConfirmDlg(int StepNum)
{
    QString Text;
    switch (StepNum) {
    case 1:
        Text = "Be aware that the result of this test is only valid if the System_System Sealing Test "
                "was successfully performed before!<br>"
                "Please confirm the retort is empty and dry.(If not, use the 'Diagnostic_Retort_Drain "
                "Reagent' function first). Please check sealing surfaces on retort and lid. Close retort lid "
                "and rotate lock to closed position.<br>"
                "Please ensure the liquid levels in all bottles and paraffin baths are between the min/max mark.";
        break;
    }
    return mp_MessageDlg->ShowConfirmMessage(m_MessageTitle, Text, CDiagnosticMessageDlg::OK_ABORT);
}

} // namespace System

} // namespace Diagnostics


