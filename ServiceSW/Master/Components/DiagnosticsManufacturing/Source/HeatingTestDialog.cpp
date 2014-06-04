/****************************************************************************/
/*! \file HeatingTestDialog.cpp
 *
 *  \brief Implementation of file for class HeatingTestDialog.
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
#include "DiagnosticsManufacturing/Include/HeatingTestDialog.h"
#include "ui_HeatingTestDialog.h"
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
CHeatingTestDialog::CHeatingTestDialog(Service::ModuleTestCaseID TestCaseId, QWidget *p_Parent) :
    MainMenu::CDialogFrame(p_Parent),
    mp_Ui(new Ui::CHeatingTestDialog),
    m_TestCaseId(TestCaseId)
{
    mp_Ui->setupUi(GetContentFrame());
//    layout()->setSizeConstraint(QLayout::SetFixedSize);
    m_AbortButtonVisible = true;
    m_DialogLock = false;
    m_DialogHide = false;
    m_Timer.setSingleShot(true);
    m_IsBlgProcessStarted = false;
    setModal(true);

    QString Description = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(TestCaseId);
    this->SetDialogTitle(Description);

    mp_Ui->labelExtTemp->hide();
    mp_Ui->labelExtTempName->hide();

    CONNECTSIGNALSLOTGUI(mp_Ui->abortButton, clicked(), this, AbortWaitDialog());
    CONNECTSIGNALSLOTGUI(&m_Timer, timeout(), this, reject());
    CONNECTSIGNALSIGNALGUI(&m_Timer, timeout(), this, Timeout());    
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CHeatingTestDialog::~CHeatingTestDialog()
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
void CHeatingTestDialog::SetText(QString Text)
{
    mp_Ui->label->setText(Text);
//    mp_Ui->contentLabel->setText(QString("%1").arg(Text));
}


/****************************************************************************/
/*!
 *  \brief Update label status
 *
 *  \iparam Status = Label test status
 */
/****************************************************************************/
void CHeatingTestDialog::UpdateLabel(const Service::ModuleTestStatus &Status)
{

    mp_Ui->labelUsedTime->setText(Status.value("UsedTime"));
    mp_Ui->labelCurTempTop->setText(Status.value("CurrentTempTop"));
    mp_Ui->labelCurTempBottom1->setText(Status.value("CurrentTempBottom1"));
    mp_Ui->labelCurTempBottom2->setText(Status.value("CurrentTempBottom2"));

    if (m_TestCaseId == Service::OVEN_HEATING_WITH_WATER) {
        if (Status.value("LeftSensorTemp") != NULL) {
            QString Text = QApplication::translate("DiagnosticsManufacturing::CHeatingTestDialog",
                                                   "Left Sensor Temperature:", 0, QApplication::UnicodeUTF8);
            mp_Ui->labelCurTempTopName->setText(Text);
            mp_Ui->labelCurTempTop->setText(Status.value("LeftSensorTemp"));
        }
        if (Status.value("MiddleSensorTemp") != NULL) {
            QString Text = QApplication::translate("DiagnosticsManufacturing::CHeatingTestDialog",
                                                   "Middle Sensor Temperature:", 0, QApplication::UnicodeUTF8);
            mp_Ui->labelCurTempBottom1Name->setText(Text);
            mp_Ui->labelCurTempBottom1->setText(Status.value("MiddleSensorTemp"));
        }
        if (Status.value("RightSensorTemp") != NULL) {
            QString Text = QApplication::translate("DiagnosticsManufacturing::CHeatingTestDialog",
                                                   "Right Sensor Temperature:", 0, QApplication::UnicodeUTF8);
            mp_Ui->labelCurTempBottom2Name->setText(Text);
            mp_Ui->labelCurTempBottom2->setText(Status.value("RightSensorTemp"));
        }
    }
    if (Status.value("Duration") != NULL) {
        mp_Ui->labelDuration->setText(Status.value("Duration"));
    }
    if (Status.value("TargetTemp") != NULL) {
        mp_Ui->labelTargetTemp->setText(Status.value("TargetTemp"));
    }
}




/****************************************************************************/
/*!
 *  \brief Activates a timeout timer
 *
 *  \iparam Milliseconds = Timeout in milliseconds
 */
/****************************************************************************/
void CHeatingTestDialog::SetTimeout(qint32 Milliseconds)
{
    m_Timer.start(Milliseconds);
}

/****************************************************************************/
/*!
 *  \brief Abort wait dialog
 */
/****************************************************************************/
void CHeatingTestDialog::AbortWaitDialog()
{
    PerformManufacturingTest(Service::TEST_ABORT);

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
void CHeatingTestDialog::BlgProcessProgress(bool IsBlgProcessStarted)
{
    m_IsBlgProcessStarted = IsBlgProcessStarted;
}

/****************************************************************************/
/*!
 *  \brief Hides the abort button of the dialog
 */
/****************************************************************************/
void CHeatingTestDialog::HideAbort(bool HideFlag)
{
    if (HideFlag) {
        mp_Ui->abortButton->setText("Close");
       // mp_Ui->abortButton->hide();
    }
    else {
        mp_Ui->abortButton->setText("Abort");
       // mp_Ui->abortButton->show();
    }
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
void CHeatingTestDialog::Show()
{
    if(m_AbortButtonVisible == true) {
        CDialogFrame::show();
        this->resize(400, 268);
    }
    else {
        m_DialogLock = false;
        m_DialogHide = false;
        qApp->installEventFilter(this);
        LockDialog();
    }

    qDebug()<<"CHeatingTestDialog::show -- size("<<this->width()<<" "<<this->height()<<")";
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
void CHeatingTestDialog::done(int Result)
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
bool CHeatingTestDialog::eventFilter(QObject *p_Object, QEvent *p_Event)
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
void CHeatingTestDialog::LockDialog()
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
void CHeatingTestDialog::UnlockDialog()
{
    m_DialogLock = false;
    if (m_DialogHide == true) {
        accept();
    }
}

} // end namespace MainMenu
