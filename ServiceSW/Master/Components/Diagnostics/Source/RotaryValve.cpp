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

void CRotaryValve::LogResult(QString &TestName, ErrorCode_t RetError)
{
    if (RetError == RETURN_OK)
        Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is successful.").arg(TestName));
    else if (RetError == RETURN_ABORT)
        Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is aborted.").arg(TestName));
    else
        Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is failed.").arg(TestName));

}

void CRotaryValve::StartMovementTest(void)
{
    QString TestName = QString("Rotary Valve %1").arg(ui->movementTest->text());
    Global::EventObject::Instance().RaiseEvent(EVENT_COMMON_ID, Global::tTranslatableStringList() << QString("%1 is requested.").arg(TestName));

    emit SetGUITabEnable(false);
    RotaryValve::CMovementTest test(mp_MessageDlg);
    ErrorCode_t ret = (ErrorCode_t)test.Run();

    if (ret == RETURN_OK) {
        emit EnableTestButton();
    }
    LogResult(TestName, ret);

    emit SetGUITabEnable(true);
}

} // namespace Diagnostics
