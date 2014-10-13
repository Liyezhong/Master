/****************************************************************************/
/*! \file Oven.cpp
 *
 *  \brief Implementation of Oven test.
 *
 *   $Version: $ 1.0
 *   $Date:    $ 2014-10-10
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
#include <QDebug>
#include "Diagnostics/Include/Oven/OvenHeatingTestEmpty.h"
#include "Diagnostics/Include/Oven/OvenHeatingTestWithLiquid.h"
#include "Diagnostics/Include/Oven/CoverSensorTest.h"
#include "Global/Include/Utils.h"
#include "Main/Include/HimalayaServiceEventCodes.h"
#include "Diagnostics/Include/Oven.h"
#include "ui_Oven.h"

namespace Diagnostics {

COven::COven(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::COven),
    dlg(new CDiagnosticMessageDlg(this))
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
    delete dlg;
    delete ui;
}

void COven::StartHeatingTestEmpty(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_EMPTY_TEST);
    qDebug() << "Oven: start heating test empty";

    Oven::CHeatingTestEmpty test(dlg);
    (void)test.Run();
}

void COven::StartCoverSensorTest(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_OVEN_COVER_SENSOR_TEST);
    qDebug() << "Oven: start cover sensor test";

    Oven::CCoverSensorTest test(dlg);
    (void)test.Run();
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
    switch (p_Event->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

} // namespace Diagnostics
