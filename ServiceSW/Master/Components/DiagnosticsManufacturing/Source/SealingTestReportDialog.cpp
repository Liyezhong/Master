/****************************************************************************/
/*! \file SealingTestReportDialog.cpp
 *
 *  \brief Header file for class CSealingTestReportDialog.
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
#include "DiagnosticsManufacturing/Include/SealingTestReportDialog.h"
#include "ui_SealingTestReportDialog.h"
#include <QDebug>
#include "ServiceDataManager/Include/TestCaseGuide.h"
#include "Core/Include/CMessageString.h"

namespace DiagnosticsManufacturing {

CSealingTestReportDialog::CSealingTestReportDialog(QWidget *p_Parent):
    MainMenu::CDialogFrame(p_Parent),
    mp_Ui(new Ui::CSealingTestReportDialog),
    m_Index(0),
    m_IsOkButton(false),
    m_IsAbort(false)
{
    mp_Ui->setupUi(GetContentFrame());
    InitLableGroup();
    if (mp_Ui->pushButton == NULL) {
        qDebug()<<"push null";
    }
    CONNECTSIGNALSLOT(mp_Ui->pushButton, clicked(), this, OnButtonClicked());
}

CSealingTestReportDialog::~CSealingTestReportDialog()
{
    try {
        delete mp_Ui;
        LabelGroup* labelGroup = NULL;
        foreach(labelGroup, m_LabelGroups) {
            delete labelGroup;
        }
    }
    catch (...) {}
}

void CSealingTestReportDialog::UpdateLabel(const Service::ModuleTestStatus &Status)
{
    QString Position, Pressure, Result, StatusText, FinishFlag;
    Position = Status.value("Position");
    Pressure = Status.value("Pressure");
    Result   = Status.value("Result");
    if (Status.value("CurrentPressure") == "1") {
        StatusText = Status.value("CurrentStatus");
    }
    else {
        StatusText = Status.value("Label");
    }
    FinishFlag = Status.value("Finish");

    if (!Position.isEmpty()) {
        LabelGroup* lableGroup = m_LabelGroups[m_Index];
        lableGroup->positionLabel->setText(Position);
        lableGroup->pressureLabel->setText(Pressure);
        lableGroup->resultLabel->setText(Result);
        m_Index += 1;
    }
    if (!StatusText.isEmpty()) {
        mp_Ui->statusLabel->setText(StatusText);
    }
    if (FinishFlag == "1") {
        mp_Ui->pushButton->setText(Service::CMessageString::MSG_BUTTON_OK);
        mp_Ui->pushButton->setEnabled(true);
        m_IsOkButton = true;
    }
}

void CSealingTestReportDialog::OnButtonClicked()
{
    m_Index = 0;
    if (!m_IsOkButton) {
        PerformManufacturingTest(Service::TEST_ABORT, Service::SYSTEM_SEALING_TEST);
        mp_Ui->pushButton->setEnabled(false);
    }
    else {
        emit RefreshResultToMain(true);
        (void)this->close();
    }
}

void CSealingTestReportDialog::InitLableGroup()
{
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->Position1, mp_Ui->Pressure1, mp_Ui->Result1));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->Position2, mp_Ui->Pressure2, mp_Ui->Result2));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->Position3, mp_Ui->Pressure3, mp_Ui->Result3));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->Position4, mp_Ui->Pressure4, mp_Ui->Result4));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->Position5, mp_Ui->Pressure5, mp_Ui->Result5));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->Position6, mp_Ui->Pressure6, mp_Ui->Result6));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->Position7, mp_Ui->Pressure7, mp_Ui->Result7));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->Position8, mp_Ui->Pressure8, mp_Ui->Result8));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->Position9, mp_Ui->Pressure9, mp_Ui->Result9));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->Position10, mp_Ui->Pressure10, mp_Ui->Result10));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->Position11, mp_Ui->Pressure11, mp_Ui->Result11));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->Position12, mp_Ui->Pressure12, mp_Ui->Result12));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->Position13, mp_Ui->Pressure13, mp_Ui->Result13));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->Position14, mp_Ui->Pressure14, mp_Ui->Result14));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->Position15, mp_Ui->Pressure15, mp_Ui->Result15));
    m_LabelGroups.push_back(new LabelGroup(mp_Ui->Position16, mp_Ui->Pressure16, mp_Ui->Result16));
}

void CSealingTestReportDialog::RetranslateUI()
{

}

 bool CSealingTestReportDialog::eventFilter(QObject *p_Object, QEvent *p_Event)
 {
     if (p_Event->type() == QEvent::MouseButtonPress || p_Event->type() == QEvent::MouseButtonRelease) {
         return true;
     }
     else {
         // standard event processing
         return QObject::eventFilter(p_Object, p_Event);
     }
 }

} // end namespace DiagnosticsManufacturing
