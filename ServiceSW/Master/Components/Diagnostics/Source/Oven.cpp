/****************************************************************************/
/*! \file Oven.cpp
 *
 *  \brief Implementation of Oven test.
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

#include "Diagnostics/Include/Oven.h"

#include <QDebug>

#include "ui_Oven.h"

#include "Diagnostics/Include/Oven/OvenHeatingTestEmpty.h"
#include "Diagnostics/Include/Oven/OvenHeatingTestWithLiquid.h"
#include "Diagnostics/Include/Oven/CoverSensorTest.h"
#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"



namespace Diagnostics {

COven::COven(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::COven)
{
    ui->setupUi(this);

    (void)connect(ui->testHeatingEmpty,
                 SIGNAL(clicked()),
                 this,
                 SLOT(StartHeatingTestEmpty()) );

    (void)connect(ui->testCoverSensor,
                  SIGNAL(clicked()),
                  this,
                  SLOT(StartCoverSensorTest()) );
}

COven::~COven()
{
    delete ui;
}

void COven::StartHeatingTestEmpty(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_EMPTY_TEST);
    qDebug() << "Oven: start heating test empty";

    //Oven::CHeatingTestEmpty test;

    //test.Run();

    emit GuiOvenEmptyHeatingTest();
}

void COven::StartHeatingTestWithLiquid(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_LIQUID_TEST);
    qDebug() << "Oven: start heating test with liquid";

    //Oven::CHeatingTestWithLiquid test;

    //test.Run();

}

void COven::StartCoverSensorTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_OVEN_COVER_SENSOR_TEST);
    qDebug() << "Oven: start cover sensor test";

    //Oven::CCoverSensorTest test;

    //test.Run();
}
/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void COven::changeEvent(QEvent *p_Event)
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
