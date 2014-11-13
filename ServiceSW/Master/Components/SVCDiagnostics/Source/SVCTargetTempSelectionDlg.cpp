/****************************************************************************/
/*! \file SVCTargetTempSelectionDlg.cpp
 *
 *  \brief Implementation of target temperature selection dialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-11-10
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
#include "SVCDiagnostics/Include/SVCTargetTempSelectionDlg.h"
#include "ui_CSVCTargetTempSelectionDlg.h"

#include "Global/Include/Utils.h"

namespace SVCDiagnostics{

CSVCTargetTempSelectionDlg::CSVCTargetTempSelectionDlg(int DefaultTemp, int MinTemp, int MaxTemp, QWidget* p_Parent):
    MainMenu::CDialogFrame(p_Parent),
    mp_Ui(new Ui::CSVCTargetTempSelectionDlg),
    m_TargetTemp(0)
{
    mp_Ui->setupUi(GetContentFrame());
    setModal(true);

    //this->SetDialogTitle(tr("Target Temperature Selection"));
    mp_ScrollWheel = new MainMenu::CScrollWheel;

    mp_Ui->scrollPanelWidget->Init(1);
    mp_Ui->scrollPanelWidget->AddScrollWheel(mp_ScrollWheel, 0);

    mp_ScrollWheel->ClearItems();

    for (int i = MinTemp; i <= MaxTemp; i++) {
        mp_ScrollWheel->AddItem(QString::number(i), i);
    }

    mp_ScrollWheel->SetNonContinuous();
    mp_ScrollWheel->SetThreeDigitMode(true);
    mp_Ui->scrollPanelWidget->SetThreeDigitMode(true);
    mp_ScrollWheel->SetCurrentData(DefaultTemp);
    mp_Ui->scrollPanelWidget->SetSubtitle(tr("Target Temperature"), 0);

    CONNECTSIGNALSLOTGUI(mp_Ui->pushButtonOk, clicked(), this, OnOk());
}

CSVCTargetTempSelectionDlg::~CSVCTargetTempSelectionDlg()
{
    try {
        delete mp_Ui;
        delete mp_ScrollWheel;
    }
    catch (...) {

    }
}

bool CSVCTargetTempSelectionDlg::eventFilter(QObject *p_Object, QEvent *p_Event)
{
    if (p_Event->type() == QEvent::MouseButtonPress || p_Event->type() == QEvent::MouseButtonRelease) {
        return true;
    }
    else {
        // standard event processing
        return QObject::eventFilter(p_Object, p_Event);
    }
}

void CSVCTargetTempSelectionDlg::OnOk()
{
    m_TargetTemp = mp_ScrollWheel->GetCurrentData().toInt();
    accept();
}

}//end of namespace SVCDiagnostics
