/****************************************************************************/
/*! \file SelectPositionDialog.cpp
 *
 *  \brief Implementation of file for class SelectPositionDialog.
 *
 *  \b Description:
 *          This class implements a base widget for displaying the wait dialogs.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-06-09
 *   $Author:  $ Sunny Qu
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include "DiagnosticsManufacturing/Include/SelectPositionDialog.h"
#include "ui_SelectPositionDialog.h"
#include "MainMenu/Include/MessageDlg.h"
#include "Core/Include/CMessageString.h"
#include <QDebug>
#include "ServiceDataManager/Include/TestCaseGuide.h"

namespace DiagnosticsManufacturing {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CSelectPositionDialog::CSelectPositionDialog(Service::ModuleTestCaseID Id, quint8 Position, bool TubeFlag, QWidget *p_Parent) : MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CSelectPositionDialog),
    m_Position(Position),
    m_TubeFlag(TubeFlag)
{
    mp_Ui->setupUi(GetContentFrame());
    setModal(true);
    m_pButtonGroup = new QButtonGroup();
    m_pButtonGroup->addButton(mp_Ui->radioButtonTube, 0);
    m_pButtonGroup->addButton(mp_Ui->radioButtonSealing, 1);

    mp_ScrollWheel = new MainMenu::CScrollWheel;

    mp_Ui->scrollPanelWidget->Init(1);
    mp_Ui->scrollPanelWidget->AddScrollWheel(mp_ScrollWheel, 0);

    mp_ScrollWheel->ClearItems();

    for (int i = 0; i < 16; i += 1) {
        mp_ScrollWheel->AddItem(QString::number(i+1).rightJustified(2, '0'), i);
    }
    mp_ScrollWheel->SetNonContinuous();
    mp_ScrollWheel->SetThreeDigitMode(true);
    mp_Ui->scrollPanelWidget->SetThreeDigitMode(true);
    mp_ScrollWheel->SetCurrentData(m_Position);
    mp_Ui->scrollPanelWidget->SetSubtitle(QApplication::translate("CSelectPositionDialog", "Position", 0, QApplication::UnicodeUTF8), 0);

    CONNECTSIGNALSLOTGUI(m_pButtonGroup, buttonClicked(int), this, OnRadioBtnSelected(int));

    CONNECTSIGNALSLOTGUI(mp_Ui->pushButtonOk, clicked(), this, AbortDialog());

    if (Id == Service::ROTARY_VALVE_SELECTION_FUNCTION) {
        mp_Ui->radioButtonSealing->setDisabled(true);
    }
    else if (Id == Service::ROTARY_VALVE_SEALING_FUNCTION) {
        mp_Ui->radioButtonTube->setChecked(false);
        mp_Ui->radioButtonSealing->setDisabled(false);
        mp_Ui->radioButtonSealing->setChecked(true);
    }

}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CSelectPositionDialog::~CSelectPositionDialog()
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
bool CSelectPositionDialog::eventFilter(QObject *p_Object, QEvent *p_Event)
{
    if (p_Event->type() == QEvent::MouseButtonPress || p_Event->type() == QEvent::MouseButtonRelease) {
        return true;
    }
    else {
        // standard event processing
        return QObject::eventFilter(p_Object, p_Event);
    }
    return false;
}

/****************************************************************************/
/*!
 *  \brief Abort dialog
 */
/****************************************************************************/
void CSelectPositionDialog::AbortDialog()
{
    m_Position = mp_ScrollWheel->GetCurrentData().toInt();
    accept();
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CSelectPositionDialog::RetranslateUI()
{

}

void CSelectPositionDialog::OnRadioBtnSelected(int radioBtnIndex)
{
    if (radioBtnIndex == 0) {
        m_TubeFlag = true;
    }
    else {
        m_TubeFlag = false;
    }
}

} // end namespace MainMenu
