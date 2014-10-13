/****************************************************************************/
/*! \file SelectBottleNReagentDialog.cpp
 *
 *  \brief Implementation of file for class SelectBottleNReagentDialog.
 *
 *  \b Description:
 *          This class implements a base widget for displaying the wait dialogs.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-10-08
 *   $Author:  $ Dixiong Li
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Global/Include/Utils.h"
#include "Diagnostics/Include/SelectBottleNReagentDialog.h"
#include "ui_SelectBottleNReagentDialog.h"
#include <QDebug>

namespace Diagnostics {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CSelectBottleNReagentDialog::CSelectBottleNReagentDialog(QWidget *p_Parent) :
    MainMenu::CDialogFrame(p_Parent),
    mp_Ui(new Ui::CSelectBottleNReagentDialog),
    m_BottleNumber(1),
    m_ReagentGroup(1)
{
    mp_Ui->setupUi(GetContentFrame());
    setModal(true);
    this->SetDialogTitle("System Filling && Draining Test");
    mp_ButtonGroup = new QButtonGroup();
    mp_ButtonGroup->addButton(mp_Ui->radioButtonXylene, 0);
    mp_ButtonGroup->addButton(mp_Ui->radioButtonOther, 1);

    mp_ScrollWheel = new MainMenu::CScrollWheel;

    mp_Ui->widget->Init(1);
    mp_Ui->widget->AddScrollWheel(mp_ScrollWheel, 0);

    mp_ScrollWheel->ClearItems();

    for (int i = 0; i < 13; i += 1) {
        mp_ScrollWheel->AddItem(QString::number(i+1).rightJustified(2, '0'), i);
    }

    mp_ScrollWheel->SetNonContinuous();
    mp_ScrollWheel->SetThreeDigitMode(true);
    mp_Ui->widget->SetThreeDigitMode(true);
    mp_Ui->radioButtonOther->setChecked(true);
    mp_Ui->widget->SetSubtitle("bottle number", 0);

    CONNECTSIGNALSLOTGUI(mp_ButtonGroup, buttonClicked(int), this, OnRadioBtnSelected(int));

    CONNECTSIGNALSLOTGUI(mp_Ui->okBtn, clicked(), this, OnOk());

    CONNECTSIGNALSLOTGUI(mp_Ui->abortBtn, clicked(), this, OnAbort());
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CSelectBottleNReagentDialog::~CSelectBottleNReagentDialog()
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
bool CSelectBottleNReagentDialog::eventFilter(QObject *p_Object, QEvent *p_Event)
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
void CSelectBottleNReagentDialog::OnOk()
{
    m_BottleNumber = mp_ScrollWheel->GetCurrentData().toInt() + 1;
    accept();
}

void CSelectBottleNReagentDialog::OnAbort()
{
    reject();
}

void CSelectBottleNReagentDialog::OnRadioBtnSelected(int radioBtnIndex)
{   
    m_ReagentGroup = radioBtnIndex;
}

} // end namespace Diagnostics

