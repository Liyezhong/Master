/****************************************************************************/
/*! \file CSelectSealingDialog.cpp
 *
 *  \brief Header file for class CSelectSealingDialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-06-30
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
#include "DiagnosticsManufacturing/Include/SelectSealingDialog.h"
#include "ui_SelectSealingDialog.h"
#include <QDebug>
#include "ServiceDataManager/Include/TestCaseGuide.h"

namespace DiagnosticsManufacturing {

CSelectSealingDialog::CSelectSealingDialog(QWidget *p_Parent):
    mp_Ui(new Ui::CSelectSealingDialog),
    MainMenu::CDialogFrame(p_Parent)
{
    mp_Ui->setupUi(GetContentFrame());
    setModal(true);

    mp_ButtonGroup = new QButtonGroup();
    mp_ButtonGroup->addButton(mp_Ui->radioButtonAuto, 0);
    mp_ButtonGroup->addButton(mp_Ui->radioButtonManual, 1);

    m_SelectedMode = 0;//auto test
    mp_Ui->radioButtonAuto->setChecked(true);
    mp_Ui->radioButtonManual->setChecked(false);
    this->SetDialogTitle("Sealing test");
    CONNECTSIGNALSLOTGUI(mp_ButtonGroup, buttonClicked(int), this, OnRadioBtnSelected(int));
    CONNECTSIGNALSLOTGUI(mp_Ui->okButton, clicked(), this, close());
}

CSelectSealingDialog::~CSelectSealingDialog()
{
    try {
        delete mp_Ui;
    }
    catch (...) {}
}

void CSelectSealingDialog::OnRadioBtnSelected(int radioBtnIndex)
{
    m_SelectedMode = radioBtnIndex;
}

void CSelectSealingDialog::RetranslateUI()
{

}

bool CSelectSealingDialog::eventFilter(QObject *p_Object, QEvent *p_Event)
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

} // end namespace DiagnosticsManufacturing
