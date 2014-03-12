/****************************************************************************/
/*! \file DlgAlarm.cpp
 *
 *  \brief Implementation of Alarm test dialog.
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

#include "Diagnostics/Include/System/DlgAlarm.h"

#include <QDebug>

#include "ui_DlgAlarm.h"

namespace Diagnostics {

namespace System {

CDlgAlarm::CDlgAlarm(QWidget *parent) :
    MainMenu::CDialogFrame(parent),
    ui(new Ui::CDlgAlarm)
{
    QWidget *p_Content = GetContentFrame();
    ui->setupUi(p_Content);

    ui->sliderAlarm->setOrientation(Qt::Horizontal);
    ui->sliderAlarm->SetPosition(MainMenu::CSliderControl::PosRight);

    (void)connect(ui->sliderAlarm,
                  SIGNAL(positionChanged(MainMenu::CSliderControl::Position_t)),
                  this,
                  SLOT(EmitTestAlarm(MainMenu::CSliderControl::Position_t)) );
    (void)connect(ui->pushNext, SIGNAL(clicked()), this, SLOT(accept()) );
    (void)connect(ui->pushCancel, SIGNAL(clicked()), this, SLOT(reject()) );
}

CDlgAlarm::~CDlgAlarm()
{
    try
    {
        delete ui;
    }
    catch (...) { }
}

void CDlgAlarm::SetStatus(const QString &Status)
{
    ui->labelStatus->setText(Status);
}

void CDlgAlarm::EmitTestAlarm(MainMenu::CSliderControl::Position_t Position)
{
    bool On = (MainMenu::CSliderControl::PosLeft == Position);

    emit TestAlarm(On);
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CDlgAlarm::changeEvent(QEvent *p_Event)
{
    MainMenu::CDialogFrame::changeEvent(p_Event);

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

} // namespace System

} // namespace Diagnostics
