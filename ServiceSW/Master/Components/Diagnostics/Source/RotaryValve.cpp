/****************************************************************************/
/*! \file Diagnostics/Source/RotaryValve.cpp
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

CRotaryValve::CRotaryValve(QWidget *p_Parent)
    : QWidget(p_Parent)
    , ui(new Ui::CRotaryValve)
{
    ui->setupUi(this);
    mp_MessageDlg = new CDiagnosticMessageDlg(this);
    (void)connect(ui->movementTest, SIGNAL(clicked()), this, SLOT(StartMovementTest()));
}

CRotaryValve::~CRotaryValve()
{
    try {
        delete mp_MessageDlg;
        delete ui;
    }
    catch (...) {

    }
}

void CRotaryValve::StartMovementTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_MOVEMENT_TEST);
    qDebug() << "Rotary Valve: start movement test";

    RotaryValve::CMovementTest test(mp_MessageDlg);

    if (test.Run() == RETURN_OK) {
        emit EnableTestButton();
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_MOVEMENT_TEST_SUCCESS);
    }
    else {
        Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_MOVEMENT_TEST_FAILURE);
    }
}

} // namespace Diagnostics
