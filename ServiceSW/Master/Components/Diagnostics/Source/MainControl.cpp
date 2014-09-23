/****************************************************************************/
/*! \file MainContril.cpp
 *
 *  \brief Implementation of Main control test.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-09-09
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

#include "Diagnostics/Include/MainControl.h"

#include <QDebug>

#include "ui_MainControl.h"

#include "MainMenu/Include/MessageDlg.h"
#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "Diagnostics/Include/MainControl/ASBTest.h"

namespace Diagnostics {

CMainControl::CMainControl(QWidget *p_Parent) :
    QWidget(p_Parent),
    ui(new Ui::CMainControl)
{
    qDebug() << "Diagnostics::CMainControl::CMainControl";

    ui->setupUi(this);

    (void)connect(ui->testASB3,
                 SIGNAL(clicked()),
                 this,
                 SLOT(StartASB3Test()) );

    (void)connect(ui->testASB5,
                 SIGNAL(clicked()),
                 this,
                 SLOT(StartASB5Test()) );

    (void)connect(ui->testASB15,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartASB15Test()) );
}

CMainControl::~CMainControl()
{
    delete ui;
}

void CMainControl::StartASB3Test(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB3_TEST);
    qDebug() << "CMainControl: start ASB3 test";

    MainControl::CASBTest Test(Slave_3, this);

    (void)Test.Run();
}

void CMainControl::StartASB5Test(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB5_TEST);
    qDebug() << "CMainControl: start ASB5 test";

    MainControl::CASBTest Test(Slave_5, this);

    (void)Test.Run();
}

void CMainControl::StartASB15Test(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB15_TEST);
    qDebug() << "CMainControl: start ASB15 test";

    MainControl::CASBTest Test(Slave_15, this);

    (void)Test.Run();
}
/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CMainControl::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type())
    {
    case QEvent::LanguageChange:
    {
        ui->retranslateUi(this);
    }
        break;
    default:
        break;
    }
}

} // namespace Diagnostics

