/****************************************************************************/
/*! \file RotaryValve.cpp
 *
 *  \brief Implementation of RotaryValve test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-27
 *   $Author:  $ R.Wu
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

#include "Diagnostics/Include/RotaryValve.h"

#include <QDebug>

#include "ui_RotaryValve.h"

#include "MainMenu/Include/MessageDlg.h"
#include "Global/Include/Utils.h"

#include "Diagnostics/Include/RotaryValve/InitializingTest.h"
#include "Diagnostics/Include/RotaryValve/SelectingTest.h"
#include "Diagnostics/Include/RotaryValve/SealingTest.h"
#include "Main/Include/HimalayaServiceEventCodes.h"


namespace Diagnostics {

CRotaryValve::CRotaryValve(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CRotaryValve),
    m_InitSuccess(true)
{
    ui->setupUi(this);

    (void)connect(ui->testInitializing,
                 SIGNAL(clicked()),
                 this,
                 SLOT(StartInitializingTest()) );

    (void)connect(ui->testSelecting,
                 SIGNAL(clicked()),
                 this,
                 SLOT(StartSelectingTest()) );

    (void)connect(ui->testSealing,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartSealingTest()) );
    (void)connect(ui->testHeatingProperty,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartHeatingTest()) );
}

CRotaryValve::~CRotaryValve()
{
    delete ui;
}

void CRotaryValve::StartInitializingTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_INITIALIZING_TEST);
    qDebug() << "Rotary Valve: start initializing test";

    RotaryValve::CInitializingTest test;

    CONNECTSIGNALSLOT(&test, StartRVInitTest(), this, OnStartRVInitTest());

    test.Run();
}

void CRotaryValve::StartSelectingTest(void)
{
    if(!m_InitSuccess)
    {
        MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
        dlg->SetTitle(tr("Warining"));
        dlg->SetIcon(QMessageBox::Warning);
        dlg->SetText(tr("Please run \"Initializing\" successfully first!"));
        dlg->HideButtons();
        dlg->SetButtonText(1, tr("Cancel"));

        dlg->exec();

        delete dlg;

        return;
    }

    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SELECTING_TEST);
    qDebug() << "Rotary Valve: start selecting test";

    RotaryValve::CSelectingTest test;

    CONNECTSIGNALSLOT(&test, StartRVSelectTest(qint32), this, OnStartRVSelectTest(qint32));

    test.Run();
}

void CRotaryValve::StartSealingTest(void)
{

    if(!m_InitSuccess)
    {
        MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
        dlg->SetTitle(tr("Warining"));
        dlg->SetIcon(QMessageBox::Warning);
        dlg->SetText(tr("Please run \"Initializing\" successfully first!"));
        dlg->HideButtons();
        dlg->SetButtonText(1, tr("Cancel"));

        dlg->exec();

        delete dlg;

        return;
    }

    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SEALING_TEST);
    qDebug() << "Rotary Valve: start sealing test";

    RotaryValve::CSealingTest test;

    test.Run();
}

void CRotaryValve::StartHeatingTest(void)
{
    //Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_HEATING_TEST);
    qDebug() << "Rotary Valve: start heatint property test";

    // RotaryValve::CHeatingTest test;

    // test.Run();

}


} // namespace Diagnostics
