/****************************************************************************/
/*! \file Core/Source/CalibrationHandler.cpp
 *
 *  \brief CalibrationHandler class implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-11-14
 *   $Author:  $ Srivathsa HH, Soumya. D
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

#include "Core/Include/CalibrationHandler.h"
#include <QApplication>
//#include "Core/Include/CMessageString.h"

namespace Core {

/****************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam p_ServiceGUIConnector = GUI connector object
 *  \iparam p_MainWindow = Mainwindow object
 */
/****************************************************************************/
CCalibrationHanlder::CCalibrationHanlder(CServiceGUIConnector *p_ServiceGUIConnector,
                                         MainMenu::CMainWindow *p_MainWindow) :
    mp_ServiceConnector(p_ServiceGUIConnector),
    mp_MainWindow(p_MainWindow)
{
    mp_CalibrationGroup = new MainMenu::CMenuGroup;
    mp_PressureSensor = new Calibration::CPressureSensor();
    CONNECTSIGNALSLOT(mp_PressureSensor, CalibrationPressureSensor(), this, OnPressureSensorCalibration());
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CCalibrationHanlder::~CCalibrationHanlder()
{
    try {
        // Calibration
        delete mp_PressureSensor;
        delete mp_CalibrationGroup;
    }
    catch(...) {

    }
}

/****************************************************************************/
/*!
 *  \brief PressureSensorCalibration
 *
 */
/****************************************************************************/
void CCalibrationHanlder::OnPressureSensorCalibration()
{
    //mp_ServiceConnector->ShowBusyDialog();
    /*emit OvenLidInitCalibrationRequest();
    QString Title( QApplication::translate("Core::CCalibrationHanlder", "Oven LID/COVER Calibration",
                                           0, QApplication::UnicodeUTF8));
    QString GBox( QApplication::translate("Core::CCalibrationHanlder", "Oven LID/COVER",
                                          0, QApplication::UnicodeUTF8));
    QString Instr( QApplication::translate("Core::CCalibrationHanlder",
            "Set the Oven Lid in its final rear closing positon.\nPress Next button to calibrate the Oven Lid/Cover.",
                                           0, QApplication::UnicodeUTF8));
    if(!PerformCalibration(Title, GBox, Instr))
    {
        return;
    }*/
    emit PressureSensorCalibrationRequest();
}

/****************************************************************************/
/*!
 *  \brief PerformCalibration
 *  \iparam Title = Pop Up dialog Title
 *  \iparam GBox = Pop Up dialog G Box
 *  \iparam Instr = Pop Up dialog Instruction
 *  \return True or falses
 */
/****************************************************************************/
bool CCalibrationHanlder::PerformCalibration(QString Title, QString GBox, QString Instr)
{
    QTimer timer;
    timer.setSingleShot(true);
    timer.setInterval(30000);
    timer.start();
    // quit via timer is the same as exit(0)
    CONNECTSIGNALSLOT(&timer, timeout(), &m_LoopCalibrationStart, quit());

    qint32 ret = m_LoopCalibrationStart.exec();
    if (ret == 0) {
        ShowErrorMessage( QApplication::translate("Core::CCalibrationHanlder", "Timeout expired: ",
                                                  0, QApplication::UnicodeUTF8));
        return false;
    }
    if(ret != 1)
    {
        //ShowErrorMessage(Service::CMessageString::MSG_CALIBRATION_FAILURE);
        return false;
    }
    mp_ServiceConnector->HideBusyDialog();

    /*mp_ServiceConnector->SetPopUpDialogBackNext(Title, GBox, Instr);
    MainMenu::ButtonPress_t Response1 = mp_ServiceConnector->GetPopUpResponse();
    if (MainMenu::BTN_NEXT_PRESSED != Response1) {
        mp_ServiceConnector->ClosePopUp();
        ShowMessage( QApplication::translate("Core::CCalibrationHanlder", "Calibration Aborted!",
                                             0, QApplication::UnicodeUTF8));
        return false;
    }
    */
    //mp_ServiceConnector->ClosePopUp();
    //mp_ServiceConnector->ShowBusyDialog();
    return true;
}

/****************************************************************************/
/*!
 *  \brief Throws open a Pop Up Message with Custom Messages
 *  \iparam Message = Dialog title
 */
/****************************************************************************/
void CCalibrationHanlder::ShowMessage(const QString &Message)
{
    mp_ServiceConnector->HideBusyDialog();
    mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_INFO, Message);
}

/****************************************************************************/
/*!
 *  \brief Throws open a Pop Up Message with Custom Messages
 *  \iparam Message = Dialog title
 */
/****************************************************************************/
void CCalibrationHanlder::ShowErrorMessage(const QString &Message)
{
    mp_ServiceConnector->HideBusyDialog();
    mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, Message);
}

/****************************************************************************/
/*!
 *  \brief Throws open a Pop Up Message with Custom Messages
 *  \iparam Message = Dialog title
 *  \iparam OkStatus = Status flag
 */
/****************************************************************************/
void CCalibrationHanlder::ShowCalibrationInitMessagetoMain(const QString &Message, bool OkStatus)
{
    Q_UNUSED(Message);
    qint32 ret = -1;
    if (OkStatus) {
        ret = 1;
    }
    if (m_LoopCalibrationStart.isRunning()) {
        m_LoopCalibrationStart.exit(ret);
    } else {
        qDebug()<<"NOTICE: Unexpected action acknowledgement for Calibration";
    }
}

/****************************************************************************/
/*!
 *  \brief Slot called for enabling Calibration Group
 *  \iparam Status = true or false
 */
/****************************************************************************/
void CCalibrationHanlder::EnableCalibrationGroup(bool Status)
{
}

} // end of namespace Core
