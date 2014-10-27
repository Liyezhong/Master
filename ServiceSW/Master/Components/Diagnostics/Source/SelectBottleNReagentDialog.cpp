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
CSelectBottleNReagentDialog::CSelectBottleNReagentDialog(int MaxBottleNum, QWidget *p_Parent) :
    MainMenu::CDialogFrame(p_Parent),
    mp_Ui(new Ui::CSelectBottleNReagentDialog),
    m_BottleNumber(1),
    m_Option(1)
{
    mp_Ui->setupUi(GetContentFrame());
    setModal(true);

    mp_ButtonGroup = new QButtonGroup();
    mp_ButtonGroup->addButton(mp_Ui->radioButton1, 0);
    mp_ButtonGroup->addButton(mp_Ui->radioButton2, 1);

    mp_ScrollWheel = new MainMenu::CScrollWheel;

    mp_Ui->widget->Init(1);
    mp_Ui->widget->AddScrollWheel(mp_ScrollWheel, 0);

    mp_ScrollWheel->ClearItems();

    QString ItemStr;
    for (int i = 0; i < MaxBottleNum; i += 1) {
        if (i == 13) {
            ItemStr = "P1";
        }
        else if (i == 14) {
            ItemStr = "P2";
        }
        else if (i == 15) {
            ItemStr = "P3";
        }
        else {
            ItemStr = QString::number(i+1).rightJustified(2, '0');
        }
        mp_ScrollWheel->AddItem(ItemStr, i);
    }

    mp_ScrollWheel->SetNonContinuous();
    mp_ScrollWheel->SetThreeDigitMode(true);
    mp_Ui->widget->SetThreeDigitMode(true);
    mp_Ui->radioButton2->setChecked(true);
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

void CSelectBottleNReagentDialog::SetRadioButtonVisible(bool Visible)
{
    if (Visible) {
        mp_Ui->radioButton1->show();
        mp_Ui->radioButton2->show();
    }
    else {
        mp_Ui->radioButton1->hide();
        mp_Ui->radioButton2->hide();
    }
}

void CSelectBottleNReagentDialog::SetScrollWheelVisible(bool Visible)
{
    if (Visible) {
        mp_Ui->widget->show();
    }
    else{
        mp_Ui->widget->hide();
    }
}

void CSelectBottleNReagentDialog::SetTitle(QString& Title)
{
    this->SetDialogTitle(Title);
}

void CSelectBottleNReagentDialog::SetLableText(QString &Text)
{
    mp_Ui->label->setText(Text);
}

void CSelectBottleNReagentDialog::SetRadioButtonText(QString &Button1, QString &Button2)
{
    mp_Ui->radioButton1->setText(Button1);
    mp_Ui->radioButton2->setText(Button2);
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
    m_Option = radioBtnIndex;
}

} // end namespace Diagnostics

