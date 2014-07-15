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
    mp_MainWindow(p_MainWindow),
    m_Result(0)
{
    mp_CalibrationGroup = new MainMenu::CMenuGroup;
    mp_PressureSensor = new Calibration::CPressureSensor();
    mp_Touchscreen = new Calibration::CTouchscreen(mp_MainWindow);
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
    MainMenu::CMessageDlg *MsgDlg = new MainMenu::CMessageDlg(mp_MainWindow);
    MsgDlg->HideAllButtons();
    MsgDlg->SetTitle(QApplication::translate("Core::CCalibrationHanlder",
                                             "Pressure sensor calibration",
                                             0, QApplication::UnicodeUTF8));
    MsgDlg->setModal(false);

    mp_PressureSensor->SetButtonStatus(false);

    for(int i=0; i<3; i++) {
        QString Msg;
        if (i==0) {
            Msg = QApplication::translate("Core::CCalibrationHanlder",
                                              "Executing the first calibration, please wait...",
                                              0, QApplication::UnicodeUTF8);
        }
        else if (i==1) {
            Msg = QApplication::translate("Core::CCalibrationHanlder",
                                              "Executing the second calibration, please wait...",
                                              0, QApplication::UnicodeUTF8);
        }
        else if (i==2) {
            Msg = QApplication::translate("Core::CCalibrationHanlder",
                                              "Executing the third calibration, please wait...",
                                              0, QApplication::UnicodeUTF8);
        }
        MsgDlg->SetText(Msg);
        MsgDlg->Show();

        emit PerformManufacturingTest(Service::PRESSURE_CALIBRATION);

        GetCalibrationResponse();

        MsgDlg->hide();

        if (m_Result == 0) {
            QString Msg = QApplication::translate("Core::CCalibrationHanlder",
                                                  "Pressure sensor calibration success.",
                                                  0, QApplication::UnicodeUTF8);
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_INFO, Msg, true);

            break;
        }
        else if (m_Result == 2) {
            if (i==3) {
                break;
            }
            QString Msg = QApplication::translate("Core::CCalibrationHanlder",
                                                  "Please open the retort lid, and then click 'OK' to retry.",
                                                  0, QApplication::UnicodeUTF8);
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_INFO, Msg, true);
        }
    }

    if (m_Result != 0) {
        QString Msg = QApplication::translate("Core::CCalibrationHanlder",
                                              "Pressure sensor calibration failed.",
                                              0, QApplication::UnicodeUTF8);
        mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, Msg, true);
    }
    mp_PressureSensor->SetButtonStatus(true);
    delete MsgDlg;
}

/****************************************************************************/
/*!
 *  \brief GetCalibrationResponse
 *
 */
/****************************************************************************/
bool CCalibrationHanlder::GetCalibrationResponse()
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

    return true;
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

        m_Result = -1;
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

void CCalibrationHanlder::RefreshCalibrationMessagetoMain(const Service::ModuleTestStatus &Status)
{
    qDebug()<<"RefreshCalibrationMessagetoMain ---------"<<Status;
    m_Result = Status.value("Result").toInt();

    if (m_LoopCalibrationStart.isRunning()) {
        m_LoopCalibrationStart.exit(1);
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

/****************************************************************************/
/*!
 *  \brief Adds GUI Calibration Components
 *
 */
/****************************************************************************/
void CCalibrationHanlder::LoadCalibrationGUIComponenets()
{
    mp_CalibrationGroup->AddPanel(QApplication::translate("Core::CCalibrationHanlder", "Pressure Sensor",
                                                          0, QApplication::UnicodeUTF8), mp_PressureSensor);
    mp_CalibrationGroup->AddPanel(QApplication::translate("Core::CCalibrationHanlder", "Touchscreen",
                                                          0, QApplication::UnicodeUTF8), mp_Touchscreen);
    mp_MainWindow->AddMenuGroup(mp_CalibrationGroup, QApplication::translate("Core::CCalibrationHanlder", "Calibration"
                                                                                          , 0, QApplication::UnicodeUTF8));
}


} // end of namespace Core
