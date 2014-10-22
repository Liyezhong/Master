/****************************************************************************/
/*! \file Select110v220vDialog.cpp
 *
 *  \brief Header file for class CSelect110v220vDialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-06-18
 *   $Author:  $ Dixiong Li
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/


#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include "DiagnosticsManufacturing/Include/Select110v220vDialog.h"
#include "ui_Select110v220vDialog.h"
#include <QDebug>
#include "ServiceDataManager/Include/TestCaseGuide.h"

namespace DiagnosticsManufacturing {

CSelect110v220vDialog::CSelect110v220vDialog(int DefaultVoltage, QWidget *p_Parent):
    mp_Ui(new Ui::CSelect110v220vDialog),
    m_SelectedVoltage(DefaultVoltage),
    MainMenu::CDialogFrame(p_Parent)
{
    mp_Ui->setupUi(GetContentFrame());
    setModal(true);

    mp_ButtonGroup = new QButtonGroup();
    mp_ButtonGroup->addButton(mp_Ui->radioButton110, 0);
    mp_ButtonGroup->addButton(mp_Ui->radioButton220, 1);

    mp_Ui->radioButton110->setChecked(m_SelectedVoltage == 110);
    mp_Ui->radioButton220->setChecked(m_SelectedVoltage == 220);
    this->SetDialogTitle(QApplication::translate("DiagnosticsManufacturing::CSelect110v220vDialog",
                                                 "110V/220V switch test", 0, QApplication::UnicodeUTF8));
    CONNECTSIGNALSLOTGUI(mp_ButtonGroup, buttonClicked(int), this, OnRadioBtnSelected(int));
    CONNECTSIGNALSLOTGUI(mp_Ui->okButton, clicked(), this, close());
}

CSelect110v220vDialog::~CSelect110v220vDialog()
{
    try {
        delete mp_Ui;
    }
    catch (...) {}
}

void CSelect110v220vDialog::OnRadioBtnSelected(int radioBtnIndex)
{
    if (radioBtnIndex == 0) {
        m_SelectedVoltage = 110;
    }
    else {
        m_SelectedVoltage = 220;
    }
}

void CSelect110v220vDialog::RetranslateUI()
{

}

bool CSelect110v220vDialog::eventFilter(QObject *p_Object, QEvent *p_Event)
{
    if (p_Event->type() == QEvent::MouseButtonPress || p_Event->type() == QEvent::MouseButtonRelease) {
        return true;
    }
    else {
        // standard event processing
        return QObject::eventFilter(p_Object, p_Event);
    }
    //return false; //PC-lint
}

void CSelect110v220vDialog::UpdateLabelForService()
{
    mp_Ui->radioButton110->setText("[90-132V AC]");
    mp_Ui->radioButton220->setText("[198-253V AC]");
    this->SetDialogTitle("AC Voltage Selection Self-test");
}

} // end namespace DiagnosticsManufacturing
