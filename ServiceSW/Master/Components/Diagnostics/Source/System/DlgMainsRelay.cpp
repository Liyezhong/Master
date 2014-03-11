/****************************************************************************/
/*! \file DlgMainsRelay.cpp
 *
 *  \brief Implementation of Mains relay test dialog.
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

#include "Diagnostics/Include/System/DlgMainsRelay.h"

#include "ui_DlgMainsRelay.h"

namespace Diagnostics {

namespace System {


CDlgMainsRelay::CDlgMainsRelay(QWidget *parent) :
    MainMenu::CDialogFrame(parent),
    ui(new Ui::CDlgMainsRelay)
{
    QWidget *p_Content = GetContentFrame();
    ui->setupUi(p_Content);

    ui->sliderAcHeating->setOrientation(Qt::Horizontal);
    ui->sliderAcHeating->SetPosition(MainMenu::CSliderControl::PosRight);

    ui->sliderRelay->setOrientation(Qt::Horizontal);
    ui->sliderRelay->SetPosition(MainMenu::CSliderControl::PosRight);

    (void)connect(ui->sliderAcHeating,
                  SIGNAL(positionChanged(MainMenu::CSliderControl::Position_t)),
                  this,
                  SLOT(EmitTestAcHeating(MainMenu::CSliderControl::Position_t)) );
    (void)connect(ui->sliderRelay,
                  SIGNAL(positionChanged(MainMenu::CSliderControl::Position_t)),
                  this,
                  SLOT(EmitTestRelay(MainMenu::CSliderControl::Position_t)) );
    (void)connect(ui->pushNext, SIGNAL(clicked()), this, SLOT(accept()) );
    (void)connect(ui->pushCancel, SIGNAL(clicked()), this, SLOT(reject()) );
}

CDlgMainsRelay::~CDlgMainsRelay()
{
    try
    {
        delete ui;
    }
    catch (...) { }
}

void CDlgMainsRelay::SetStatus(const QString &Status)
{
    ui->labelStatus->setText(Status);
}

void CDlgMainsRelay::EmitTestAcHeating(
        MainMenu::CSliderControl::Position_t Position)
{
    bool On = (MainMenu::CSliderControl::PosLeft == Position);

    emit TestAcHeating(On);
}

void CDlgMainsRelay::EmitTestRelay(
        MainMenu::CSliderControl::Position_t Position)
{
    bool On = (MainMenu::CSliderControl::PosLeft == Position);

    emit TestRelay(On);
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CDlgMainsRelay::changeEvent(QEvent *p_Event)
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
