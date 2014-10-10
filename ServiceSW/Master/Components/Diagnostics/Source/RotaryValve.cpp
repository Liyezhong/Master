/****************************************************************************/
/*! \file RotaryValve.cpp
 *
 *  \brief Implementation of RotaryValve test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-10-8
 *   $Author:  $ Arthur Li
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2014 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Diagnostics/Include/RotaryValve.h"

#include <QDebug>

#include "ui_RotaryValve.h"

#include "MainMenu/Include/MessageDlg.h"
#include "Global/Include/Utils.h"

#include "Diagnostics/Include/RotaryValve/MovementTest.h"
#include "Main/Include/HimalayaServiceEventCodes.h"
#include "Diagnostics/Include/DiagnosticMessageDlg.h"

namespace Diagnostics {

CRotaryValve::CRotaryValve(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CRotaryValve)
    , m_InitSuccess(true)
    , mp_MessageDlg(new CDiagnosticMessageDlg(this))
{
    ui->setupUi(this);
    (void)connect(ui->movementTest, SIGNAL(clicked()), this, SLOT(StartMovementTest()));
}

CRotaryValve::~CRotaryValve()
{
    delete mp_MessageDlg;
    delete ui;
}

void CRotaryValve::StartMovementTest(void)
{
//    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_INITIALIZING_TEST);
    qDebug() << "Rotary Valve: start movement test";

    RotaryValve::CMovementTest test(mp_MessageDlg);

    test.Run();
}

//void CRotaryValve::StartSelectingTest(void)
//{
//    if(!m_InitSuccess)
//    {
//        MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
//        dlg->SetTitle(tr("Warining"));
//        dlg->SetIcon(QMessageBox::Warning);
//        dlg->SetText(tr("Please run \"Initializing\" successfully first!"));
//        dlg->HideButtons();
//        dlg->SetButtonText(1, tr("Cancel"));

//        dlg->exec();

//        delete dlg;

//        return;
//    }

//    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SELECTING_TEST);
//    qDebug() << "Rotary Valve: start selecting test";

//    RotaryValve::CSelectingTest test;

//    CONNECTSIGNALSLOT(&test, StartRVSelectTest(qint32), this, OnStartRVSelectTest(qint32));

//    test.Run();
//}

//void CRotaryValve::StartSealingTest(void)
//{

//    if(!m_InitSuccess)
//    {
//        MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
//        dlg->SetTitle(tr("Warining"));
//        dlg->SetIcon(QMessageBox::Warning);
//        dlg->SetText(tr("Please run \"Initializing\" successfully first!"));
//        dlg->HideButtons();
//        dlg->SetButtonText(1, tr("Cancel"));

//        dlg->exec();

//        delete dlg;

//        return;
//    }

//    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SEALING_TEST);
//    qDebug() << "Rotary Valve: start sealing test";

//    RotaryValve::CSealingTest test;

//    test.Run();
//}

//void CRotaryValve::StartHeatingTest(void)
//{
//    //Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_HEATING_TEST);
//    qDebug() << "Rotary Valve: start heatint property test";

//    // RotaryValve::CHeatingTest test;

//    // test.Run();

//}


} // namespace Diagnostics
