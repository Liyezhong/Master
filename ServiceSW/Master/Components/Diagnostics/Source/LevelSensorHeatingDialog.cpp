/****************************************************************************/
/*! \file LevelSensorHeatingDialog.cpp
 *
 *  \brief Implementation of level sensor heating dialog.
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

#include "Diagnostics/Include/LevelSensorHeatingDialog.h"
#include <QDebug>
#include "ui_LevelSensorHeatingDialog.h"
#include "Diagnostics/Include/ServiceDeviceProcess/ServiceDeviceProcess.h"

#include "Global/Include/Utils.h"

namespace Diagnostics {

CLevelSensorHeatingDialog::CLevelSensorHeatingDialog(QWidget *p_Parent)
    :MainMenu::CDialogFrame(p_Parent),
     mp_Ui(new Ui::CLevelSensorHeatingDialog),
     m_Abort(false)
{
    mp_Ui->setupUi(this);
    setModal(true);
    //resize(400, 300);

    CONNECTSIGNALSLOTGUI(mp_Ui->abortBtn, clicked(), this, AbortDialog());
}

CLevelSensorHeatingDialog::~CLevelSensorHeatingDialog()
{
    try {
        delete mp_Ui;
    }
    catch (...) {}
}

void CLevelSensorHeatingDialog::SetTitle(QString& Title)
{
    this->SetDialogTitle(Title);
}

bool CLevelSensorHeatingDialog::StartHeating(bool XyleneFlag)
{
    this->show();

    quint16 TargetTemp(0);
    qint16 ExchangePIDTemp(0);
    qreal  CurrentTemp(0);
    int WaitSeconds = 120;
    ServiceDeviceProcess* p_DevProc = ServiceDeviceProcess::Instance();
    if (XyleneFlag) {
        TargetTemp = 115;
        ExchangePIDTemp = 110;
    }
    else {
        TargetTemp = 95;
        ExchangePIDTemp = 90;
    }

    (void)p_DevProc->LSStartHeating(true, !XyleneFlag);
    while (WaitSeconds) {
        if (m_Abort) {
            (void)p_DevProc->LSStopHeating();
            return false;
        }
        (void)p_DevProc->LSGetTemp(&CurrentTemp);
        if (CurrentTemp >= ExchangePIDTemp) {
            break;
        }
        p_DevProc->Pause(1000);
        WaitSeconds--;
        UpdateUI(120-WaitSeconds, 120, TargetTemp, CurrentTemp);
    }

    (void)p_DevProc->LSStopHeating();

    (void)p_DevProc->LSStartHeating(false, !XyleneFlag);
    p_DevProc->Pause(1000);
    while (WaitSeconds) {
        if (m_Abort) {
            (void)p_DevProc->LSStopHeating();
            return false;
        }
        (void)p_DevProc->LSGetTemp(&CurrentTemp);
        if (CurrentTemp > ExchangePIDTemp) {
            break;
        }
        p_DevProc->Pause(1000);
        WaitSeconds--;
        UpdateUI(120-WaitSeconds, 120, TargetTemp, CurrentTemp);
    }

    accept();

    if (WaitSeconds == 0 && CurrentTemp <= ExchangePIDTemp) {
        (void)p_DevProc->LSStopHeating();
        return false;
    }

    return true;
}

void CLevelSensorHeatingDialog::UpdateUI(int UsedTime, int EDTime, qreal TargetTemp, qreal CurrentTemp)
{
    QString UsedTimeStr     = QTime().addSecs(UsedTime).toString("hh:mm:ss");
    QString DurationTimeStr = QTime().addSecs(EDTime).toString("hh:mm:ss");
    QString TargetTempStr   = QString::number(TargetTemp);
    QString CurrentTempStr  = QString::number(CurrentTemp);

    mp_Ui->usedTimeValue->setText(UsedTimeStr);
    mp_Ui->durationValue->setText(DurationTimeStr);
    mp_Ui->targetTempValue->setText(TargetTempStr);
    mp_Ui->currentTempValue->setText(CurrentTempStr);
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
bool CLevelSensorHeatingDialog::eventFilter(QObject *p_Object, QEvent *p_Event)
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
void CLevelSensorHeatingDialog::AbortDialog()
{
    //ServiceDeviceProcess::Instance()->LSStopHeating();
    m_Abort = true;
    reject();
}

} // namespace Diagnostics




