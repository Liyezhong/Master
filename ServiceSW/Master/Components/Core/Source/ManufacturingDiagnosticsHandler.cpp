/****************************************************************************/
/*! \file ManufacturingDiagnosticsHandler.cpp
 *
 *  \brief ManufacturingDiagnosticsHandler class implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-05-12
 *   $Author:  $ Sunny Qu
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
#include <QApplication>
#include "Global/Include/UITranslator.h"
#include "Core/Include/ManufacturingDiagnosticsHandler.h"
#include "Core/Include/CMessageString.h"
#include "DiagnosticsManufacturing/Include/OvenManufacturing.h"
#include "DiagnosticsManufacturing/Include/HeatingTestDialog.h"

#include "Main/Include/HimalayaServiceEventCodes.h"

namespace Core {

/****************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam p_ServiceGUIConnector = GUI connector object
 *  \iparam p_MainWindow = Mainwindow object
 */
/****************************************************************************/
CManufacturingDiagnosticsHandler::CManufacturingDiagnosticsHandler(CServiceGUIConnector *p_ServiceGUIConnector,
                                                                   MainMenu::CMainWindow *p_MainWindow) :
    mp_ServiceConnector(p_ServiceGUIConnector),
    mp_MainWindow(p_MainWindow)
{

    //Diagnostics Manufacturing
    mp_DiagnosticsManufGroup = new MainMenu::CMenuGroup;
    mp_OvenManuf = new DiagnosticsManufacturing::COven(mp_ServiceConnector, mp_MainWindow);

    CONNECTSIGNALSLOTGUI(mp_OvenManuf, BeginModuleTest(Service::ModuleNames_t, QStringList), this, BeginManufacturingSWTests(Service::ModuleNames_t, QStringList));


    /* Manufacturing SW Reset status */
    CONNECTSIGNALSLOTGUI(mp_DiagnosticsManufGroup, PanelChanged(), mp_OvenManuf, ResetTestStatus());

}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CManufacturingDiagnosticsHandler::~CManufacturingDiagnosticsHandler()
{
    try {
        delete mp_OvenManuf;
        delete mp_DiagnosticsManufGroup;
    }
    catch (...) {

    }
}

/****************************************************************************/
/*!
 *  \brief Adds GUI Diagnostics Manufactruing SW Components
 */
/****************************************************************************/
void CManufacturingDiagnosticsHandler::LoadManufDiagnosticsComponents()
{
//    mp_DiagnosticsManufGroup->Clear();
    //Diagnostics
    mp_DiagnosticsManufGroup->AddPanel(QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                       "Oven", 0, QApplication::UnicodeUTF8), mp_OvenManuf);
    mp_MainWindow->AddMenuGroup(mp_DiagnosticsManufGroup, QApplication::translate
                   ("Core::CManufacturingDiagnosticsHandler", "Diagnostics", 0, QApplication::UnicodeUTF8));
}

/****************************************************************************/
/*!
 *  \brief Slot called for Module tests for manufacturing SW
 *  \iparam ModuleName = Name of the module
 */
/****************************************************************************/
void CManufacturingDiagnosticsHandler::BeginManufacturingSWTests(Service::ModuleNames_t ModuleName, const QStringList &TestCaseList)
{
    qDebug()<<"CManufacturingDiagnosticsHandler::BeginManufacturingSWTests : ModuleName="<<ModuleName;
    switch(ModuleName) {
    case Service::OVEN:
        PerformManufOvenTests(TestCaseList);
        break;
    default:
        break;
    }
}

/****************************************************************************/
/*!
 *  \brief Heated Cuvettes Test response
 *  \return true or false
 */
/****************************************************************************/
bool CManufacturingDiagnosticsHandler::GetTestResponse()
{
//    mp_ServiceConnector->ShowBusyDialog();
    QTimer timer;
    qint32 ret;
    quint32 interval = 1000 * 3 * 60 * 60; // 3 hours.
    timer.setSingleShot(true);
    timer.setInterval(interval);
    timer.start();
    CONNECTSIGNALSLOT(&timer, timeout(), &m_LoopManufacturingTest, quit());
    ret = m_LoopManufacturingTest.exec();
//    mp_ServiceConnector->HideBusyDialog();
    if (ret != 1)
    {
        return false;
    }
    return true;
}

bool CManufacturingDiagnosticsHandler::ShowGuide(const QString &TestCaseName, int Index)
{
    QString GuideText = TestCaseName;
    qDebug() << "ShowGuide :"<<TestCaseName;

    if (TestCaseName == "OvenCoverSensor") {
        if (Index == 0) {
            GuideText = "Please open the oven cover manually.";
        }
        else if (Index == 1){
            GuideText = "Please close the oven cover manually.";
        }
    }
    else if (TestCaseName == "OvenHeatingEmpty") {
        GuideText = "Please remove the wax baths.";
    }
    else if (TestCaseName == "OvenHeatingWater") {
        GuideText = "Please fill water into 3 paraffin baths to maximum level.";
    }

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(mp_MainWindow);
    dlg->SetTitle(TestCaseName);
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(GuideText);
    dlg->HideCenterButton();
    dlg->SetButtonText(3, tr("Next"));
    dlg->SetButtonText(1, tr("Cancel"));
    if (Index == 1) {
        dlg->EnableButton(1, false);
    }

//    CONNECTSIGNALSLOT(dlg, ButtonRightClicked(), dlg, accept() );

    int ret = dlg->exec();

    qDebug()<<"return code = "<<ret;

    delete dlg;

    if ( ret == 0 )
        return true;

    return false;
}

void CManufacturingDiagnosticsHandler::ShowFailedResult(const QString &TestCaseName)
{
    DiagnosticsManufacturing::CHeatingTestDialog *dlg = new DiagnosticsManufacturing::CHeatingTestDialog(true, mp_MainWindow);
    QString Text = TestCaseName;
    Service::ModuleTestStatus Status;
    Status.insert("Duration", "0");
    Text.append(" ");
    Text.append("Test Failed !");
    dlg->SetDialogTitle("Error");
    dlg->SetText(Text);
    dlg->HideAbort(false);
    dlg->show();
}

/****************************************************************************/
/*!
 *  \brief Function called for Module tests for manufacturing SW
 */
/****************************************************************************/
void CManufacturingDiagnosticsHandler::PerformManufOvenTests(const QStringList &TestCaseList)
{
    QString TestCaseDisplayText;
    quint32 FailureId(0);
    quint32 OkId(0);
    qDebug()<<"CManufacturingDiagnosticsHandler::PerformManufOvenTests ---" << TestCaseList;
    for(int i=0; i<TestCaseList.size(); i++) {
        QString TestCaseName = TestCaseList.at(i);

        bool NextFlag = ShowGuide(TestCaseName, 0);
        if (NextFlag == false) {
            break;
        }

        if (TestCaseName == "OvenCoverSensor") {
            FailureId = EVENT_GUI_DIAGNOSTICS_OVEN_COVER_SENSOR_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_OVEN_COVER_SENSOR_TEST_SUCCESS;
            Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_OVEN_COVER_SENSOR_TEST);
            TestCaseDisplayText = "Oven Cover Sensor Test";
            emit PerformManufacturingTest(Service::OVEN_COVER_SENSOR);
        }
        else if (TestCaseName == "OvenHeatingEmpty") {
            FailureId = EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_EMPTY_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_EMPTY_TEST_SUCCESS;
            Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_EMPTY_TEST);
            TestCaseDisplayText = "Oven Heating Test (empty) ";
            emit PerformManufacturingTest(Service::OVEN_HEATING_EMPTY);

        }
        else if (TestCaseName == "OvenHeatingWater") {
            FailureId = EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_LIQUID_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_LIQUID_TEST_SUCCESS;
            Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_LIQUID_TEST);
            TestCaseDisplayText = "Oven Heating Test (with water)";
            emit PerformManufacturingTest(Service::OVEN_HEATING_WITH_WATER);
        }

        bool Result = GetTestResponse();

        if (TestCaseName == "OvenCoverSensor") {
            if (Result == true) {
                NextFlag = ShowGuide(TestCaseName, 1);
                if (NextFlag == false)
                    break;
                emit PerformManufacturingTest(Service::OVEN_COVER_SENSOR);
                Result = GetTestResponse();
            }
        }

        if (Result == false) {
            Global::EventObject::Instance().RaiseEvent(FailureId);
            QString Text = QString("%1 %2").arg(TestCaseDisplayText, "- Fail");
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, Text, true);

            if (TestCaseName != "OvenCoverSensor") {
                ShowFailedResult(TestCaseDisplayText);
            }

        }
        else {
            Global::EventObject::Instance().RaiseEvent(OkId);
            QString Text = QString("%1 %2").arg(TestCaseDisplayText, "- Success");
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_INFO, Text, true);
        }
        mp_OvenManuf->SetTestResult(TestCaseName, Result);
    }

#if 0
    QString Title;
    QString GBox;
    QString Instr;

    qDebug()<<"CManufacturingDiagnosticsHandler::PerformManufOvenTests -- emit ASB3_0_FWUPDATE";

    emit PerformManufacturingTest(Service::ASB3_0_FWUPDATE);
    if(!GetTestResponse())
    {
        mp_OvenManuf->SetTestResult(Service::OVEN_FIRMWARE_UPDATE, false);
        Title = ( QApplication::translate("Core::CStartup", "Firmware Update Failed", 0, QApplication::UnicodeUTF8));
        GBox = Service::CMessageString::MSG_ASB3_0_FWUPDATE_FAILURE;
        Instr = ( QApplication::translate("Core::CStartup", "Firmware update failed ! Are you sure you want to continue with the tests.\nPress Next button to continue and Abort button to abort.",
                                          0, QApplication::UnicodeUTF8));
        mp_ServiceConnector->SetPopUpDialogBackNext(Title, GBox, Instr);
        MainMenu::ButtonPress_t Response1 = mp_ServiceConnector->GetPopUpResponse();
        mp_ServiceConnector->ClosePopUp();
        if (MainMenu::BTN_NEXT_PRESSED != Response1)
        {
            return;
        }
    }
    else
    {
        mp_OvenManuf->SetTestResult(Service::OVEN_FIRMWARE_UPDATE, true);
    }

    emit PerformManufacturingTest(Service::ASB12_0_FWUPDATE);
    if(!GetTestResponse())
    {
        Title = ( QApplication::translate("Core::CStartup", "Firmware Update Failed", 0, QApplication::UnicodeUTF8));
        GBox = Service::CMessageString::MSG_ASB12_0_FWUPDATE_FAILURE;
        Instr = ( QApplication::translate("Core::CStartup", "Firmware update failed ! Are you sure you want to continue with the tests.\nPress Next button to continue and Abort button to abort.",
                                          0, QApplication::UnicodeUTF8));
        mp_ServiceConnector->SetPopUpDialogBackNext(Title, GBox, Instr);
        MainMenu::ButtonPress_t Response1 = mp_ServiceConnector->GetPopUpResponse();
        mp_ServiceConnector->ClosePopUp();
        if (MainMenu::BTN_NEXT_PRESSED != Response1)
        {
            return;
        }
    }

    (void) OnOvenHeatingTest();

    emit PerformManufacturingTest(Service::OVEN_PHOTOSENSOR_TEST);
    if(!GetTestResponse())
    {
        mp_OvenManuf->SetTestResult(Service::OVEN_PHOTOSENSOR_TEST, false);
    }
    else
    {
        mp_OvenManuf->SetTestResult(Service::OVEN_PHOTOSENSOR_TEST, true);
    }

    QString Title_ =  QApplication::translate("Core::CManufacturingDiagnosticsHandler", "Slide Count",
                                              0, QApplication::UnicodeUTF8);
    QString GBox_ =  QApplication::translate("Core::CManufacturingDiagnosticsHandler", "Slide Count",
                                             0, QApplication::UnicodeUTF8);
    QString Instr_ =  QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                      "Place the golden standard rack with 30 slides in R29 station. Do you confirm the rack is in place? ",
                                              0, QApplication::UnicodeUTF8);
    mp_ServiceConnector->SetPopUpDialogYesNo(Title_, GBox_, Instr_);
    MainMenu::ButtonPress_t Response_1 = mp_ServiceConnector->GetPopUpResponse();
    mp_ServiceConnector->ClosePopUp();
    if ((MainMenu::BTN_ABORT_PRESSED == Response_1) ||
            (MainMenu::BTN_CANCEL_PRESSED == Response_1) ||
            (MainMenu::BTN_NO_PRESSED == Response_1)) {
        ShowMessage( QApplication::translate("Core::CManufacturingDiagnosticsHandler", "Manufacturing Test Aborted!",
                                             0, QApplication::UnicodeUTF8));
        return;
    }
    emit PerformManufacturingTest(Service::OVEN_SLIDECOUNT_TEST);
    if(!GetTestResponse())
    {
        mp_OvenManuf->SetTestResult(Service::OVEN_SLIDECOUNT_TEST, false);
    }
    else
    {
        mp_OvenManuf->SetTestResult(Service::OVEN_SLIDECOUNT_TEST, true);
    }

    emit PerformManufacturingTestCANID(Service::OVEN_CANID);
    if(!GetTestResponse())
    {
        mp_OvenManuf->SetTestResult(Service::OVEN_CANID, false);
    }
    else
    {
        mp_OvenManuf->SetTestResult(Service::OVEN_CANID, true);
    }
#endif
}


/****************************************************************************/
/*!
 *  \brief Returns message for manufacturing tests
 *  \iparam Result = true or false
 */
/****************************************************************************/
void CManufacturingDiagnosticsHandler::OnReturnManufacturingMsg(bool Result)
{
    qDebug()<<"CManufacturingDiagnosticsHandler::OnReturnManufacturingMsg Result="<<Result;
    qint32 ret(-1);
    if(Result) {
        ret = 1;
    }
    if (m_LoopManufacturingTest.isRunning()) {
        m_LoopManufacturingTest.exit(ret);
    } else {
        qDebug()<<"NOTICE: Unexpected action acknowledgement";
    }
}

/****************************************************************************/
/*!
 *  \brief Throws open a Pop Up Message with Custom Messages
 *  \iparam Message = Dialog title
 */
/****************************************************************************/
void CManufacturingDiagnosticsHandler::ShowMessage(const QString &Message)
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
void CManufacturingDiagnosticsHandler::ShowErrorMessage(const QString &Message)
{
    mp_ServiceConnector->HideBusyDialog();
    mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, Message);
}

} // end of namespace Core
