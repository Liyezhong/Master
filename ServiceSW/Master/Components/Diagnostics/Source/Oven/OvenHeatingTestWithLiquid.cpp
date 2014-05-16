/****************************************************************************/
/*! \file OvenHeatingTestWithLiquid.cpp
 *
 *  \brief Implementation of Oven heating test with liquid.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-29
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

#include "Diagnostics/Include/Oven/OvenHeatingTestWithLiquid.h"

#include <QDebug>

#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "MainMenu/Include/MessageDlg.h"
#include "MainMenu/Include/WaitDialog.h"


namespace Diagnostics {

namespace Oven {

CHeatingTestWithLiquid::CHeatingTestWithLiquid(void)
    : CTestBase()
{
}

CHeatingTestWithLiquid::~CHeatingTestWithLiquid(void)
{
}

void CHeatingTestWithLiquid::Run(void)
{
    qDebug() << "Oven heating test with liquid starts!";
}

void CHeatingTestWithLiquid::Succeed(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_LIQUID_TEST_SUCCESS);
    qDebug() << "Oven heating test with liquid succeeded!";

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Oven Heating Test - with liquid"));
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(tr("Oven heating test with liquid SUCCEEDED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CHeatingTestWithLiquid::Fail(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_LIQUID_TEST_FAILURE);
    qDebug() << "Oven heating test with liquid failed!";

    // display failure message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg;
    dlg->SetTitle(tr("Oven Heating Test - with liquid"));
    dlg->SetIcon(QMessageBox::Critical);
    dlg->SetText(tr("Oven heating test with liquid FAILED!"));
    dlg->HideButtons();
    dlg->SetButtonText(1, tr("OK"));

    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    dlg->exec();

    delete dlg;

    /// \todo: log here **************************************/
}

void CHeatingTestWithLiquid::Cancel(void)
{
    qDebug() << "Oven heating test with liquid canceled!";
}

} // namespace Oven

} // namespace Diagnostics
