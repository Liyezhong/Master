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

//#include "Diagnostics/Include/LaSystem/HeatingBelt1Test.h"
//#include "Diagnostics/Include/LaSystem/HeatingBelt2Test.h"
//#include "Diagnostics/Include/LaSystem/AirSystemTest.h"
#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

namespace Diagnostics {

CMainControl::CMainControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CMainControl)
{
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

    //LaSystem::CHeatingBelt1Test test;

    //test.Run();

    emit GuiASB3Test();
}

void CMainControl::StartASB5Test(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB5_TEST);
    qDebug() << "CMainControl: start ASB5 test";

    //LaSystem::CHeatingBelt2Test test;

    //test.Run();

    emit GuiASB5Test();

}

void CMainControl::StartASB15Test(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB15_TEST);
    qDebug() << "CMainControl: start ASB15 test";

    //LaSystem::CAirSystemTest test;

    //test.Run();

    emit GuiASB15Test();

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

