/****************************************************************************/
/*! \file Diagnostics/Source/StatusConfirmDialog.cpp
 *
 *  \brief Implementation of file for class StatusConfirmDialog.
 *
 *  \b Description:
 *          This class implements a base widget for displaying the wait dialogs.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-05-21
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
#include "Diagnostics/Include/StatusConfirmDialog.h"
#include "ui_StatusConfirmDialog.h"
#include <QDebug>

namespace Diagnostics {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CStatusConfirmDialog::CStatusConfirmDialog(QWidget *p_Parent) : MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CStatusConfirmDialog)
{
    mp_Ui->setupUi(GetContentFrame());
//    layout()->setSizeConstraint(QLayout::SetFixedSize);
    m_AbortButtonVisible = true;
    m_DialogLock = false;
    m_DialogHide = false;
    m_Timer.setSingleShot(true);
    m_IsBlgProcessStarted = false;
    setModal(true);

    mp_Ui->labelConfirm->setFixedWidth(457);
    CONNECTSIGNALSLOTGUI(mp_Ui->pushButtonNo, clicked(), this, AbortWaitDialog());
//    CONNECTSIGNALSLOTGUI(mp_Ui->pushButtonYes, clicked(), this, AbortWaitDialog());
    CONNECTSIGNALSLOTGUI(mp_Ui->pushButtonYes, clicked(), this, accept());
    CONNECTSIGNALSLOTGUI(&m_Timer, timeout(), this, reject());
    CONNECTSIGNALSIGNALGUI(&m_Timer, timeout(), this, Timeout());

//    mp_Ui->labelStatusValue->set
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CStatusConfirmDialog::~CStatusConfirmDialog()
{
    try {
        delete mp_Ui;
    }
    catch (...) {}
}

/****************************************************************************/
/*!
 *  \brief Sets the text displayed in the wait dialog
 *
 *  \iparam Text = Label text
 */
/****************************************************************************/
void CStatusConfirmDialog::SetText(const QString& Text)
{
    mp_Ui->labelConfirm->setText(Text);
    mp_Ui->labelConfirm->setAlignment(Qt::AlignHCenter);
}

/****************************************************************************/
/*!
 *  \brief Update oven label status
 *
 *  \iparam Status = Label test status
 */
/****************************************************************************/
void CStatusConfirmDialog::UpdateOvenLabel(const Service::ModuleTestStatus &Status)
{
    qDebug() << Status;

    QString CoverSensorStatus = QApplication::translate("DiagnosticsManufacturing::CStatusConfirmDialog",
                                                        "Cover sensor status: ", 0, QApplication::UnicodeUTF8) + Status.value("OvenCoverSensorStatus");
    if (Status.value("OvenCoverSensorStatus").isEmpty())
        return;
    mp_Ui->labelStatusValue->setText(CoverSensorStatus);

    if(Status.value("OvenCoverSensorStatus") != "Close" && Status.value("OvenCoverSensorStatus") != "Open"){
        mp_Ui->labelStatusValue->setStyleSheet("border-color: rgb(0, 0, 0); "
                                       "border-width:1px; border-style:solid;"
                                       "background-color: rgb(255, 0, 0);");
    }
    else if (Status.value("OvenCoverSensorStatus") == Status.value("TestStatus")) {
        mp_Ui->labelStatusValue->setStyleSheet("border-color: rgb(0, 0, 0); "
                                               "border-width:1px; border-style:solid;"
                                               "background-color: rgb(85, 255, 0);");
    }
    else {
        mp_Ui->labelStatusValue->setStyleSheet("border-color: rgb(0, 0, 0); "
                                               "border-width:1px; border-style:solid;"
                                               "background-color: rgb(255, 252, 135);");
    }
}

/****************************************************************************/
/*!
 *  \brief Update retort label status
 *
 *  \iparam Status = Label test status
 */
/****************************************************************************/
void CStatusConfirmDialog::UpdateRetortLabel(const Service::ModuleTestStatus &Status)
{
    QString CoverSensorStatus = QApplication::translate("DiagnosticsManufacturing::CStatusConfirmDialog",
                                                        "Retort lid lock status: ", 0, QApplication::UnicodeUTF8) + Status.value("LidLockerStatus");
    if (Status.value("LidLockerStatus").isEmpty())
        return;
    mp_Ui->labelStatusValue->setText(CoverSensorStatus);

    if (Status.value("LidLockerStatus") == "Open" && Status.value("StepNum")=="1" ||
            Status.value("LidLockerStatus") == "Close" && Status.value("StepNum")=="2") {
        mp_Ui->labelStatusValue->setStyleSheet("border-color: rgb(0, 0, 0); "
                                               "border-width:1px; border-style:solid;"
                                               "background-color: rgb(85, 255, 0);");
    }
    else if(Status.value("LidLockerStatus") == "Close" || Status.value("LidLockerStatus") == "Open") {
        mp_Ui->labelStatusValue->setStyleSheet("border-color: rgb(0, 0, 0); "
                                               "border-width:1px; border-style:solid;"
                                               "background-color: rgb(255, 252, 135);");
    }
    else {
        mp_Ui->labelStatusValue->setStyleSheet("border-color: rgb(0, 0, 0); "
                                       "border-width:1px; border-style:solid;"
                                       "background-color: rgb(255, 0, 0);");
    }
}

void CStatusConfirmDialog::HideLabel()
{
//    mp_Ui->labelStatus->hide();
    mp_Ui->labelStatusValue->hide();
}

void CStatusConfirmDialog::UpdateLabel(const QString& Status, const QString& Value)
{
//    mp_Ui->labelStatus->setText(Status);
    Q_UNUSED(Status);
    mp_Ui->labelStatusValue->setText(Value);
}

/****************************************************************************/
/*!
 *  \brief Activates a timeout timer
 *
 *  \iparam Milliseconds = Timeout in milliseconds
 */
/****************************************************************************/
void CStatusConfirmDialog::SetTimeout(qint32 Milliseconds)
{
    m_Timer.start(Milliseconds);
}

/****************************************************************************/
/*!
 *  \brief Abort wait dialog
 */
/****************************************************************************/
void CStatusConfirmDialog::AbortWaitDialog()
{
    if (m_IsBlgProcessStarted) {
        m_IsBlgProcessStarted = false;
        //Send command to main to abort blg process and park the robot arms
        emit AbortBlgProcess(true);
        reject();
    }
    else {
        m_IsBlgProcessStarted = false;
        reject();
    }
}

/****************************************************************************/
/*!
 *  \brief Abort Bathlayout generating process
 */
/****************************************************************************/
void CStatusConfirmDialog::BlgProcessProgress(bool IsBlgProcessStarted)
{
    m_IsBlgProcessStarted = IsBlgProcessStarted;
}

/****************************************************************************/
/*!
 *  \brief Hides the abort button of the dialog
 *  \iparam HideFlag = hide flag
 */
/****************************************************************************/
void CStatusConfirmDialog::HideAbort(bool HideFlag)
{
    Q_UNUSED(HideFlag);
    //m_AbortButtonVisible = false;
}

/****************************************************************************/
/*!
 *  \brief Overrides the show function of QDialog
 *
 *      If the abort button of this message box is enabled, this method will
 *      immediately show the dialog. If this is not the case, the dialog will
 *      be shown after a time of 500 ms.
 */
/****************************************************************************/
void CStatusConfirmDialog::Show()
{
    if(m_AbortButtonVisible == true) {
        CDialogFrame::show();
    }
    else {
        m_DialogLock = false;
        m_DialogHide = false;
        qApp->installEventFilter(this);
        LockDialog();
    }

    qDebug()<<"CStatusConfirmDialog::show -- size("<<this->width()<<" "<<this->height()<<")";
}

/****************************************************************************/
/*!
 *  \brief Overrides the done function of QDialog
 *
 *      Only If the message box is not locked, the dialog will be closed
 *      immediately.
 *
 *  \iparam Result = Dialog result code
 */
/****************************************************************************/
void CStatusConfirmDialog::done(int Result)
{
    m_DialogHide = true;
    m_Timer.stop();
    if (m_DialogLock == false) {
        CDialogFrame::done(Result);
    }
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
bool CStatusConfirmDialog::eventFilter(QObject *p_Object, QEvent *p_Event)
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
 *  \brief Blocks the dialog for closing
 *
 *      This function helps to assure that the dialog is displayed for at
 *      least 500 ms. This needed avoid the flickering of the dialog, when
 *      it is displayed only for a short period of time.
 */
/****************************************************************************/
void CStatusConfirmDialog::LockDialog()
{
    qApp->removeEventFilter(this);
    if (m_DialogHide == false) {
        CDialogFrame::show();
        for (int Count =0;Count < 5 ;Count++) {
          QCoreApplication::processEvents();
        }
        m_DialogLock = true;
        QTimer::singleShot(500, this, SLOT(UnlockDialog()));
    }
}

/****************************************************************************/
/*!
 *  \brief Unblocks the dialog for closing
 */
/****************************************************************************/
void CStatusConfirmDialog::UnlockDialog()
{
    m_DialogLock = false;
    if (m_DialogHide == true) {
        accept();
    }
}

} // end namespace MainMenu
