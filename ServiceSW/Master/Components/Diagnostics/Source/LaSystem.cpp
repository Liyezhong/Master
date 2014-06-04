/****************************************************************************/
/*! \file LaSystem.cpp
 *
 *  \brief Implementation of Liquid and air system test.
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

#include "Diagnostics/Include/LaSystem.h"

#include <QDebug>

#include "ui_LaSystem.h"

#include "Diagnostics/Include/LaSystem/HeatingBelt1Test.h"
#include "Diagnostics/Include/LaSystem/HeatingBelt2Test.h"
#include "Diagnostics/Include/LaSystem/AirSystemTest.h"
#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

namespace Diagnostics {

CLaSystem::CLaSystem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CLaSystem)
{
    ui->setupUi(this);

    (void)connect(ui->testHeatingBelt1,
                 SIGNAL(clicked()),
                 this,
                 SLOT(StartHeatingBelt1Test()) );

    (void)connect(ui->testHeatingBelt2,
                 SIGNAL(clicked()),
                 this,
                 SLOT(StartHeatingBelt2Test()) );

    (void)connect(ui->testAirSystem,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartAirSystemTest()) );
}

CLaSystem::~CLaSystem()
{
    delete ui;
}

void CLaSystem::StartHeatingBelt1Test(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_LASYSTEM_HEATING_LIQUID_TEST);
    qDebug() << "L&&A System: start heating belt 1 test";

    //LaSystem::CHeatingBelt1Test test;

    //test.Run();

    emit GuiTube1HeatingTest();
}

void CLaSystem::StartHeatingBelt2Test(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_LASYSTEM_HEATING_AIR_TEST);
    qDebug() << "L&&A System: start heating belt 2 test";

    //LaSystem::CHeatingBelt2Test test;

    //test.Run();

    emit GuiTube2HeatingTest();

}

void CLaSystem::StartAirSystemTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_LASYSTEM_AIR_SYSTEM_TEST);
    qDebug() << "L&&A System: start air system test";

    //LaSystem::CAirSystemTest test;

    //test.Run();

    //emit GuiLAHeatingTest(0, 0);

}
/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CLaSystem::changeEvent(QEvent *p_Event)
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
