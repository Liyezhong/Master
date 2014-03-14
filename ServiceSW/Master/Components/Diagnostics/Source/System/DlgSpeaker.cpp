/****************************************************************************/
/*! \file DlgSpeaker.cpp
 *
 *  \brief Implementation of Speaker test dialog.
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

#include "Diagnostics/Include/System/DlgSpeaker.h"

#include <QDebug>

#include "ui_DlgSpeaker.h"


namespace Diagnostics {

namespace System {

CDlgSpeaker::CDlgSpeaker(QWidget *parent) :
    MainMenu::CDialogFrame(parent),
    ui(new Ui::CDlgSpeaker)
{
    QWidget *p_Content = GetContentFrame();
    ui->setupUi(p_Content);

    ui->scrollPanelSound->Init(1);
    ui->scrollPanelSound->SetTitle(tr("Sound"));

    ui->scrollPanelVolume->Init(1);
    ui->scrollPanelVolume->SetTitle(tr("Volume"));

    mp_ScrollSound = new MainMenu::CScrollWheel;
    for (int i = 1; i < 7; i++)
    {
        mp_ScrollSound->AddItem(QString::number(i), i);
    }
    mp_ScrollSound->SetNonContinuous();
    ui->scrollPanelSound->AddScrollWheel(mp_ScrollSound, 0);

    mp_ScrollVolume = new MainMenu::CScrollWheel;
    for (int i = 0; i < 10; ++i)
    {
        mp_ScrollVolume->AddItem(QString::number(i), i);
    }
    mp_ScrollVolume->SetNonContinuous();
    ui->scrollPanelVolume->AddScrollWheel(mp_ScrollVolume, 0);

    (void)connect(ui->pushTest,
                  SIGNAL(clicked()),
                  this,
                  SLOT(EmitTestSpeaker()) );
    (void)connect(ui->pushNext, SIGNAL(clicked()), this, SLOT(accept()) );
    (void)connect(ui->pushCancel, SIGNAL(clicked()), this, SLOT(reject()) );
}

CDlgSpeaker::~CDlgSpeaker(void)
{
    try
    {
        delete mp_ScrollVolume;
        delete mp_ScrollSound;

        delete ui;
    }
    catch (...) { }
}

void CDlgSpeaker::EmitTestSpeaker(void)
{
    int Sound = mp_ScrollSound->GetCurrentData().toInt();
    int Volume = mp_ScrollVolume->GetCurrentData().toInt();

    emit TestSpeaker(Sound, Volume);
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CDlgSpeaker::changeEvent(QEvent *p_Event)
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

} //namespace System

} // namespace Diagnostics
