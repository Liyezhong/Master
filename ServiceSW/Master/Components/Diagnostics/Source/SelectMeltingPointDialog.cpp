/****************************************************************************/
/*! \file SelectMeltingPointDialog.cpp
 *
 *  \brief Implementation of select melting point.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-09-16
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

#include "Diagnostics/Include/SelectMeltingPointDialog.h"
#include <QDebug>
#include "ui_SelectMeltingPointDialog.h"

#include "Global/Include/Utils.h"

namespace Diagnostics {

CSelectMeltingPointDialog::CSelectMeltingPointDialog(int MeltingPoint, QWidget *parent)
    :MainMenu::CDialogFrame(parent),
     mp_Ui(new Ui::CSelectMeltingPointDialog),
     m_MeltingPoint(MeltingPoint)
{
    mp_Ui->setupUi(GetContentFrame());
    setModal(true);

    this->SetDialogTitle(tr("Initial System Check"));

    mp_ScrollWheel = new MainMenu::CScrollWheel;

    mp_Ui->scrollPanelWidget->Init(1);
    mp_Ui->scrollPanelWidget->AddScrollWheel(mp_ScrollWheel, 0);

    mp_ScrollWheel->ClearItems();

    for (int i = 50; i <= 70; i++) {
        mp_ScrollWheel->AddItem(QString::number(i).rightJustified(2, '0'), i);
    }

    mp_ScrollWheel->SetNonContinuous();
    mp_ScrollWheel->SetThreeDigitMode(true);
    mp_Ui->scrollPanelWidget->SetThreeDigitMode(true);
    mp_ScrollWheel->SetCurrentData(m_MeltingPoint);
    mp_Ui->scrollPanelWidget->SetSubtitle(tr("Melting Point"), 0);

    CONNECTSIGNALSLOTGUI(mp_Ui->pushButtonOk, clicked(), this, AbortDialog());
}

CSelectMeltingPointDialog::~CSelectMeltingPointDialog()
{
    try {
        delete mp_ScrollWheel;
        delete mp_Ui;
    }
    catch (...) {}
}

/****************************************************************************/
/*!
 *  \brief Filters all mouse events
 *
 *      There can be a small period of time in which the wait dialog is not
 *      displayed. This is required so that the wait dialog does not have to
 *      be displayed everytime network communication takes place.
 *
 *  \iparam p_Object = Object that is watched
 *  \iparam p_Event = Current event
 *
 *  \return True if an event should be filtered
 */
/****************************************************************************/
bool CSelectMeltingPointDialog::eventFilter(QObject *p_Object, QEvent *p_Event)
{
    if (p_Event->type() == QEvent::MouseButtonPress || p_Event->type() == QEvent::MouseButtonRelease) {
        return true;
    }
    else {
        // standard event processing
        return QObject::eventFilter(p_Object, p_Event);
    }
}

/****************************************************************************/
/*!
 *  \brief Abort dialog
 */
/****************************************************************************/
void CSelectMeltingPointDialog::AbortDialog()
{
    m_MeltingPoint = mp_ScrollWheel->GetCurrentData().toInt();
    accept();
}

} // namespace Diagnostics



