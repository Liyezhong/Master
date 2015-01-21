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
#include "Core/Include/SelectTestOptions.h"
#include "Core/Include/ManufacturingDiagnosticsHandler.h"
#include "Core/Include/CMessageString.h"
#include "DiagnosticsManufacturing/Include/OvenManufacturing.h"
#include "DiagnosticsManufacturing/Include/HeatingTestDialog.h"
#include "DiagnosticsManufacturing/Include/StatusConfirmDialog.h"
#include "ServiceDataManager/Include/TestCaseGuide.h"
#include "ServiceDataManager/Include/TestCase.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"
#include "DiagnosticsManufacturing/Include/SelectPositionDialog.h"
#include "DiagnosticsManufacturing/Include/Select110v220vDialog.h"
#include "DiagnosticsManufacturing/Include/SelectSealingDialog.h"
#include "DiagnosticsManufacturing/Include/PressureInputDialog.h"

#include "Main/Include/HimalayaServiceEventCodes.h"
#include "Global/Include/SystemPaths.h"

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
    mp_MainWindow(p_MainWindow),
    mp_WaitDialog(NULL)
{
    //m_FailStr = Service::CMessageString::MSG_DIAGNOSTICS_FAILED;
    //m_SuccessStr = Service::CMessageString::MSG_DIAGNOSTICS_SUCCESS;
    //Diagnostics Manufacturing
    mp_DiagnosticsManufGroup = new MainMenu::CMenuGroup;
    mp_DisplayManuf          = new Diagnostics::CDisplay;//(mp_MainWindow);//issue: Never add parent window here!
    mp_OvenManuf             = new DiagnosticsManufacturing::COven(mp_ServiceConnector, mp_MainWindow);
    mp_MainControlManuf      = new DiagnosticsManufacturing::CMainControl(mp_ServiceConnector, mp_MainWindow);
    mp_LaSystemManuf         = new DiagnosticsManufacturing::CLaSystem(mp_ServiceConnector, mp_MainWindow);
    mp_RotaryValveManuf      = new DiagnosticsManufacturing::CRotaryValve(mp_ServiceConnector, mp_MainWindow);
    mp_SystemManuf           = new DiagnosticsManufacturing::CSystem(mp_ServiceConnector, mp_MainWindow);
    mp_RetortManuf           = new DiagnosticsManufacturing::CRetort(mp_ServiceConnector, mp_MainWindow);
    mp_CleaningManuf         = new DiagnosticsManufacturing::CCleaning(mp_ServiceConnector, mp_MainWindow);

    CONNECTSIGNALSLOTGUI(mp_DisplayManuf, BeginModuleTest(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>), this, BeginManufacturingSWTests(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>));
    CONNECTSIGNALSLOTGUI(mp_OvenManuf, BeginModuleTest(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>), this, BeginManufacturingSWTests(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>));
    CONNECTSIGNALSLOTGUI(mp_MainControlManuf, BeginModuleTest(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>), this, BeginManufacturingSWTests(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>));
    CONNECTSIGNALSLOTGUI(mp_LaSystemManuf, BeginModuleTest(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>), this, BeginManufacturingSWTests(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>));
    CONNECTSIGNALSLOTGUI(mp_RotaryValveManuf, BeginModuleTest(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>), this, BeginManufacturingSWTests(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>));
    CONNECTSIGNALSLOTGUI(mp_SystemManuf, BeginModuleTest(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>), this, BeginManufacturingSWTests(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>));
    CONNECTSIGNALSLOTGUI(mp_RetortManuf, BeginModuleTest(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>), this, BeginManufacturingSWTests(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>));
    CONNECTSIGNALSLOTGUI(mp_CleaningManuf, BeginModuleTest(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>), this, BeginManufacturingSWTests(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>));

    CONNECTSIGNALSLOTGUI(mp_RetortManuf, LockTest(bool), this, LockTest(bool));

    /* Manufacturing SW Reset status */
    CONNECTSIGNALSLOTGUI(mp_DiagnosticsManufGroup, PanelChanged(), mp_OvenManuf, ResetTestStatus());
    CONNECTSIGNALSLOTGUI(mp_DiagnosticsManufGroup, PanelChanged(), mp_MainControlManuf, ResetTestStatus());
    CONNECTSIGNALSLOTGUI(mp_DiagnosticsManufGroup, PanelChanged(), mp_LaSystemManuf, ResetTestStatus());
    CONNECTSIGNALSLOTGUI(mp_DiagnosticsManufGroup, PanelChanged(), mp_RotaryValveManuf, ResetTestStatus());
    CONNECTSIGNALSLOTGUI(mp_DiagnosticsManufGroup, PanelChanged(), mp_SystemManuf, ResetTestStatus());
    CONNECTSIGNALSLOTGUI(mp_DiagnosticsManufGroup, PanelChanged(), mp_RetortManuf, ResetTestStatus());
    CONNECTSIGNALSLOTGUI(mp_DiagnosticsManufGroup, PanelChanged(), mp_CleaningManuf, ResetTestStatus());


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
        delete mp_MainControlManuf;
        delete mp_DisplayManuf;
        delete mp_LaSystemManuf;
        delete mp_RotaryValveManuf;
        delete mp_SystemManuf;
        delete mp_RetortManuf;
        delete mp_CleaningManuf;
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
    mp_DiagnosticsManufGroup->Clear();
    //Diagnostics
    if (Core::CSelectTestOptions::GetCurTestMode() == Core::MANUFACTURAL_ENDTEST ) {
        mp_DiagnosticsManufGroup->AddPanel(Service::CMessageString::MSG_DIAGNOSTICS_DISPLAY, mp_DisplayManuf);
        mp_DiagnosticsManufGroup->AddPanel(Service::CMessageString::MSG_DIAGNOSTICS_MC, mp_MainControlManuf);
    }
    mp_DiagnosticsManufGroup->AddPanel(Service::CMessageString::MSG_DIAGNOSTICS_RETORT, mp_RetortManuf);
    mp_DiagnosticsManufGroup->AddPanel(Service::CMessageString::MSG_DIAGNOSTICS_OVEN, mp_OvenManuf);
    mp_DiagnosticsManufGroup->AddPanel(Service::CMessageString::MSG_DIAGNOSTICS_RV, mp_RotaryValveManuf);
    if (Core::CSelectTestOptions::GetCurTestMode() == Core::MANUFACTURAL_ENDTEST ) {
        mp_DiagnosticsManufGroup->AddPanel(Service::CMessageString::MSG_DIAGNOSTICS_LA, mp_LaSystemManuf);
        mp_DiagnosticsManufGroup->AddPanel(Service::CMessageString::MSG_DIAGNOSTICS_SYSTEM, mp_SystemManuf);
        mp_DiagnosticsManufGroup->AddPanel(Service::CMessageString::MSG_DIAGNOSTICS_CLEANING, mp_CleaningManuf);
    }
    mp_MainWindow->AddMenuGroup(mp_DiagnosticsManufGroup, Service::CMessageString::MSG_DIAGNOSTICS_DIAGNOSTICS);
}

/****************************************************************************/
/*!
 *  \brief Slot called for Module tests for manufacturing SW
 *  \iparam ModuleName = Name of the module
 *  \iparam TestCaseList = list of test case
 */
/****************************************************************************/
void CManufacturingDiagnosticsHandler::BeginManufacturingSWTests(Service::ModuleNames_t ModuleName, const QList<Service::ModuleTestCaseID> &TestCaseList)
{
    qDebug()<<"CManufacturingDiagnosticsHandler::BeginManufacturingSWTests : ModuleName="<<ModuleName;
    switch(ModuleName) {
    case Service::DISPLAY:
        PerformManufDisplayTests(TestCaseList);
        break;
    case Service::OVEN:
        LockTest(true);
        PerformManufOvenTests(TestCaseList);
        LockTest(false);
        break;
    case Service::MAIN_CONTROL:
        LockTest(true);
        PerformManufMainControlTests(TestCaseList);
        LockTest(false);
        break;
    case Service::LA_SYSTEM:
        LockTest(true);
        PerformManufLATests(TestCaseList);
        LockTest(false);
        break;
    case Service::ROTARY_VALVE:
        LockTest(true);
        PerformManufRVTests(TestCaseList);
        LockTest(false);
        break;
    case Service::RETORT:
        LockTest(true);
        PerformManufRetortTests(TestCaseList);
        LockTest(false);
        break;
    case Service::SYSTEM:
        LockTest(true);
        PerformManufSystemTests(TestCaseList);
        LockTest(false);
        break;
    case Service::CLEANING_SYSTEM:
        LockTest(true);
        PerformManufCleaningSystem(TestCaseList);
        LockTest(false);
        break;
    case Service::FIRMWARE:
        PerformFirmwareUpdate(TestCaseList);
        break;
    case Service::OPERATION_TIME:
        PerformOperationTimeReset(TestCaseList);
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
    quint32 interval = 1000 * 5 * 60 * 60; // 5 hours.
    timer.setSingleShot(true);
    timer.setInterval(interval);
    timer.start();
    CONNECTSIGNALSLOT(&timer, timeout(), &m_LoopManufacturingTest, quit());
    ret = m_LoopManufacturingTest.exec();
//    mp_ServiceConnector->HideBusyDialog();
    return ret == 1 ? true : false;
}

bool CManufacturingDiagnosticsHandler::ShowGuide(Service::ModuleTestCaseID Id, int Index, bool haveNext)
{
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);
    QStringList Steps = DataManager::CTestCaseGuide::Instance().GetGuideSteps(TestCaseName, Index);
    QString GuideText;

    if (Steps.size() == 0) {  // no guide
        return true;
    }

    for(int i=0; i<Steps.size(); i++) {
        GuideText.append(Steps.at(i));
        if (i <= Steps.size()-1 ) {
            GuideText.append("<br>");
        }
    }

    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(mp_MainWindow);
    dlg->SetTitle(TestCaseDescription);
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(GuideText);
    if (haveNext) {
        dlg->HideCenterButton();
        dlg->SetButtonText(3, Service::CMessageString::MSG_BUTTON_NEXT);
        dlg->SetButtonText(1, Service::CMessageString::MSG_BUTTON_CANCEL);
    }
    else {
        dlg->HideButtonsOneAndTwo();
        dlg->SetButtonText(3, Service::CMessageString::MSG_BUTTON_OK);
    }
    if ((Index == 1 && Id == Service::OVEN_COVER_SENSOR) ||
            (Index == 1 && Id == Service::RETORT_LID_LOCK) ||
            (Index == 2 && Id == Service::RETORT_HEATING_WITH_WATER) ) {
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

void CManufacturingDiagnosticsHandler::ShowHeatingFailedResult(Service::ModuleTestCaseID Id)
{
    DiagnosticsManufacturing::CHeatingTestDialog *dlg = new DiagnosticsManufacturing::CHeatingTestDialog(Id, mp_MainWindow);
    QString Text = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    Service::ModuleTestStatus Status = p_TestCase->GetResult();
    if (Id == Service::RETORT_LEVEL_SENSOR_HEATING) {
        dlg->DisplayLSensorLabel();
    }
    dlg->UpdateLabel(Status);

    Text.append(" ");
    Text.append(Service::CMessageString::MSG_DIAGNOSTICS_FAILED);
    dlg->SetDialogTitle(Service::CMessageString::MSG_TITLE_ERROR);
    dlg->SetText(Text);
    dlg->HideAbort(true);
    (void)dlg->exec();
    delete dlg;
}

quint8 CManufacturingDiagnosticsHandler::GetPositionForRVTest(Service::ModuleTestCaseID Id, int Index)
{
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    quint8 Position = 0;
    bool TubeFlag = true;
    if ( p_TestCase->GetParameter("TubeFlag").toInt() == 1 ) {
        TubeFlag = true;
    }
    else {
        TubeFlag = false;
    }

    if (Id == Service::ROTARY_VALVE_SELECTION_FUNCTION) {
        Position = p_TestCase->GetParameter("Position").toInt()-1;
    }
    else if ( Id == Service::ROTARY_VALVE_SEALING_FUNCTION ) {
        if ( Index == 1 ) {
            Position = p_TestCase->GetParameter("Position1").toInt()-1;
        }
        else {
            Position = p_TestCase->GetParameter("Position2").toInt()-1;
        }
    }

    DiagnosticsManufacturing::CSelectPositionDialog *p_Dlg = new DiagnosticsManufacturing::CSelectPositionDialog(Id, Position, TubeFlag, mp_MainWindow);
    (void)p_Dlg->exec();

    Position = p_Dlg->GetPosition()+1;
    TubeFlag = p_Dlg->GetTubeFlag();

    QString PositionStr = QString("%1").arg(Position);
    p_TestCase->SetParameter("Position", PositionStr);
    if (TubeFlag == true) {
        p_TestCase->SetParameter("TubeFlag", "1");
    }
    else {
        p_TestCase->SetParameter("TubeFlag", "0");
    }

    qDebug()<<"Position = " << Position;
    delete p_Dlg;

    return Position;
}

void CManufacturingDiagnosticsHandler::SelectOptionFor110v220Switch()
{
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Service::SYSTEM_110V_220V_SWITCH);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    int DefaultVoltage = p_TestCase->GetParameter("ConnectedVoltage").toInt();

    DiagnosticsManufacturing::CSelect110v220vDialog* p_Dlg = new DiagnosticsManufacturing::CSelect110v220vDialog(DefaultVoltage, mp_MainWindow);
    (void)p_Dlg->exec();

    p_TestCase->SetParameter("ConnectedVoltage", QString::number(p_Dlg->GetSelectedVoltage()));

    qDebug()<<"Selected voltage:"<<p_Dlg->GetSelectedVoltage();
    delete p_Dlg;
}

bool CManufacturingDiagnosticsHandler::ShowConfirmDlgForRVSelecting(quint8 Position)
{
    Service::ModuleTestCaseID Id = Service::ROTARY_VALVE_SELECTION_FUNCTION;
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);
    QStringList Steps = DataManager::CTestCaseGuide::Instance().GetGuideSteps(TestCaseName, 1);
    QString GuideText;


    for(int i=0; i<Steps.size(); i++) {
        GuideText.append(Steps.at(i));
        if (i <= Steps.size()-1 ) {
            GuideText.append("\n");
        }
    }

    QString ReadyText = Service::CMessageString::MSG_DIAGNOSTICS_POSITION_READY.arg(Position);
    QString ResultConfirm = Service::CMessageString::MSG_DIAGNOSTICS_IF_TEST_PASS;
    QString Text = QString("%1 %2 %3").arg(ReadyText).arg(GuideText).arg(ResultConfirm);
    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(mp_MainWindow);
    dlg->SetTitle(TestCaseDescription);
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(Text);
    dlg->HideCenterButton();
    dlg->SetButtonText(3, Service::CMessageString::MSG_BUTTON_PASS);
    dlg->SetButtonText(1, Service::CMessageString::MSG_BUTTON_FAIL);


    int ret = dlg->exec();

    qDebug()<<"return code = "<<ret;

    delete dlg;

    if ( ret == 0 )
        return true;

    return false;
}

bool CManufacturingDiagnosticsHandler::ShowConfirmDlgForRVSealing(quint8 Position)
{
    Q_UNUSED(Position);

    DiagnosticsManufacturing::CPressureInputDialog *p_Dlg = new DiagnosticsManufacturing::CPressureInputDialog(Service::ROTARY_VALVE_SEALING_FUNCTION, mp_MainWindow);

    (void)p_Dlg->exec();

    bool Result = p_Dlg->GetResult();

    delete p_Dlg;

    return Result;
}

bool CManufacturingDiagnosticsHandler::ShowConfirmDlgForSystemVentilationFan()
{
    QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Service::SYSTEM_VENTILATION_FAN);

    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(mp_MainWindow);
    dlg->SetTitle(TestCaseDescription);
    dlg->SetIcon(QMessageBox::Information);

    dlg->SetText(Service::CMessageString::MSG_DIAGNOSTICS_CHECK_VENRILATION_FAN);
    dlg->HideCenterButton();
    dlg->SetButtonText(3, Service::CMessageString::MSG_BUTTON_PASS);
    dlg->SetButtonText(1, Service::CMessageString::MSG_BUTTON_FAIL);

    int ret = dlg->exec();

    delete dlg;

    if ( ret == 0 )
        return true;

    return false;
}

bool CManufacturingDiagnosticsHandler::ShowConfirmDlgForSystemSealing()
{
    QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Service::SYSTEM_SEALING_TEST);
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(mp_MainWindow);
    dlg->SetTitle(TestCaseDescription);
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(Service::CMessageString::MSG_DIAGNOSTICS_ADJUST_LID_LOCK);
    dlg->HideCenterButton();
    dlg->SetButtonText(3, Service::CMessageString::MSG_BUTTON_RETEST);
    dlg->SetButtonText(1, Service::CMessageString::MSG_BUTTON_ABORT);

    int ret = dlg->exec();  
    delete dlg;

    if (ret == 0) {
        return true;
    }
    return false;
}

void CManufacturingDiagnosticsHandler::PerformManufDisplayTests(const QList<Service::ModuleTestCaseID> &TestCaseList)
{
//    quint32 EventId(0);
//    quint32 OkId(0);
//    quint32 FailureId(0);

    qDebug()<<"CManufacturingDiagnosticsHandler::PerformManufDisplayTests ---" << TestCaseList;
    for(int i = 0; i < TestCaseList.size(); i ++) {
        Service::ModuleTestCaseID id = TestCaseList.at(i);
        switch ( id ) {
        case Service::DISPLAY_BASIC_COLOR:
//            EventId = EVENT_GUI_DIAGNOSTICS_DISPLAY_BASICCOLOR_TEST;
//            OkId = EVENT_GUI_DIAGNOSTICS_DISPLAY_BASICCOLOR_TEST_FAILURE;
//            FailureId = EVENT_GUI_DIAGNOSTICS_DISPLAY_BASICCOLOR_TEST_SUCCESS;
            mp_DisplayManuf->DoBasicColorTest();
            break;
        default:
            break;
        }
    }

}


/****************************************************************************/
/*!
 *  \brief Function called for Module tests for manufacturing SW
 */
/****************************************************************************/
void CManufacturingDiagnosticsHandler::PerformManufOvenTests(const QList<Service::ModuleTestCaseID> &TestCaseList)
{
    quint32 FailureId(0);
    quint32 OkId(0);
    quint32 EventId(0);
    qDebug()<<"CManufacturingDiagnosticsHandler::PerformManufOvenTests ---" << TestCaseList;
    for(int i=0; i<TestCaseList.size(); i++) {
        Service::ModuleTestCaseID Id = TestCaseList.at(i);

        bool NextFlag = ShowGuide(Id, 0);
        if (NextFlag == false) {
            break;
        }

        switch( Id ) {
        case Service::OVEN_COVER_SENSOR:
            EventId = EVENT_GUI_DIAGNOSTICS_OVEN_COVER_SENSOR_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_OVEN_COVER_SENSOR_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_OVEN_COVER_SENSOR_TEST_SUCCESS;
            break;
        case Service::OVEN_HEATING_EMPTY:
            EventId = EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_EMPTY_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_EMPTY_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_EMPTY_TEST_SUCCESS;
            break;
        case Service::OVEN_HEATING_WITH_WATER:
            EventId = EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_LIQUID_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_LIQUID_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_LIQUID_TEST_SUCCESS;
            break;
        default:
            break;
        }

        Global::EventObject::Instance().RaiseEvent(EventId);
        emit PerformManufacturingTest(Id, Service::TEST_CASE_ID_UNUSED);

        bool Result = GetTestResponse();

        if (Id == Service::OVEN_COVER_SENSOR && Result == true) {
            NextFlag = ShowGuide(Id, 1);
            if (NextFlag == false)
                break;
            emit PerformManufacturingTest(Service::OVEN_COVER_SENSOR, Service::TEST_CASE_ID_UNUSED);
            Result = GetTestResponse();
        }

        QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
        DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
        p_TestCase->SetStatus(Result);

        if (Result == false) {
            Global::EventObject::Instance().RaiseEvent(FailureId);

            QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);

            QString Reason = p_TestCase->GetResult().value("FailReason");
            if (Reason == "Abort") {   // if user click abort, then the test routines for this modules will terminate.
                mp_OvenManuf->EnableButton(true);
                return ;
            }

            QString Text = QString("%1 - %2<br>%3").arg(TestCaseDescription, Service::CMessageString::MSG_DIAGNOSTICS_FAILED,
                                                        p_TestCase->GetResult().value("FailReason"));
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, Text, true);

            if (Id != Service::OVEN_COVER_SENSOR && p_TestCase->GetResult().value("OutOfRange")!= "1") {
                ShowHeatingFailedResult(Id);
            }

        }
        else {
            Global::EventObject::Instance().RaiseEvent(OkId);
            QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);
            QString Text = QString("%1 - %2").arg(TestCaseDescription, Service::CMessageString::MSG_DIAGNOSTICS_SUCCESS);
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_INFO, Text, true);
        }
        mp_OvenManuf->SetTestResult(Id, Result);
    }
    mp_OvenManuf->EnableButton(true);
}

void CManufacturingDiagnosticsHandler::PerformManufMainControlTests(const QList<Service::ModuleTestCaseID> &TestCaseList)
{
    quint32 FailureId(0);
    quint32 OkId(0);
    quint32 EventId(0);
    quint8  ASBIndex = 0;
    qDebug()<<"CManufacturingDiagnosticsHandler::PerformManufMainControlTests ---" << TestCaseList;
    for(int i=0; i<TestCaseList.size(); i++) {
        Service::ModuleTestCaseID Id = TestCaseList.at(i);
        bool NextFlag = ShowGuide(Id, 0);
        if (NextFlag == false) {
            break;
        }

        switch( Id ) {
        case Service::MAINCONTROL_ASB3:
            EventId = EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB3_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB3_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB3_TEST_SUCCESS;
            ASBIndex = 3;
            break;
        case Service::MAINCONTROL_ASB5:
            EventId = EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB5_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB5_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB5_TEST_SUCCESS;
            ASBIndex = 5;
            break;
        case Service::MAINCONTROL_ASB15:
            EventId = EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB15_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB5_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB5_TEST_SUCCESS;
            ASBIndex = 15;
            break;
        default:
            break;
        }

        Global::EventObject::Instance().RaiseEvent(EventId);
        emit PerformManufacturingTest(Id);

        bool Result = GetTestResponse();
        QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
        DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
        p_TestCase->SetStatus(Result);

        if (Result == false) {
            Global::EventObject::Instance().RaiseEvent(FailureId);
        }
        else {
            Global::EventObject::Instance().RaiseEvent(OkId);
        }

        qreal Voltage      = p_TestCase->GetResult().value("Voltage").toDouble();
        qreal Current      = p_TestCase->GetResult().value("Current").toDouble();
        QString VoltageRet = p_TestCase->GetResult().value("VoltageRet");
        QString CurrentRet = p_TestCase->GetResult().value("CurrentRet");

        QString Text = Service::CMessageString::MSG_DIAGNOSTICS_ASB_OUTPUT_RESULT.arg(ASBIndex)
                .arg(VoltageRet).arg(Voltage).arg(CurrentRet).arg(Current);

        qDebug()<<"Show MessageBox " << ASBIndex;
        mp_ServiceConnector->ShowMessageDialog(Result ? Global::GUIMSGTYPE_INFO : Global::GUIMSGTYPE_ERROR, Text, true);
        qDebug()<<"End Show MessageBox "<<ASBIndex;
        mp_MainControlManuf->SetTestResult(Id, Result);
    }
    mp_MainControlManuf->EnableButton(true);
}

void CManufacturingDiagnosticsHandler::PerformManufRetortTests(const QList<Service::ModuleTestCaseID> &TestCaseList)
{
    quint32 failureId(0);
    quint32 okId(0);
    quint32 eventId(0);

    qDebug()<<"CManufacturingDiagnosticsHandler::PerformManufRetortTests ---" << TestCaseList;

    for(int i=0; i<TestCaseList.size(); i++) {
        Service::ModuleTestCaseID id = TestCaseList.at(i);
        QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(id);
        DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

        bool nextFlag = ShowGuide(id, 0);
        if (nextFlag == false) {
            break;
        }
        switch(id) {
        case Service::RETORT_LID_LOCK:
            eventId = EVENT_GUI_DIAGNOSTICS_RETORT_LIDLOCK_TEST;
            failureId = EVENT_GUI_DIAGNOSTICS_RETORT_LIDLOCK_TEST_FAILURE;
            okId = EVENT_GUI_DIAGNOSTICS_RETORT_LIDLOCK_TEST_SUCCESS;
            break;
        case Service::RETORT_HEATING_EMPTY:
            eventId = EVENT_GUI_DIAGNOSTICS_RETORT_LEVELSENSOR_HEATING_TEST;
            failureId = EVENT_GUI_DIAGNOSTICS_RETORT_LEVELSENSOR_HEATING_TEST_FAILURE;
            okId = EVENT_GUI_DIAGNOSTICS_RETORT_LEVELSENSOR_HEATING_TEST_SUCCESS;
            break;
        case Service::RETORT_LEVEL_SENSOR_HEATING:
            eventId = EVENT_GUI_DIAGNOSTICS_RETORT_LEVELSENSOR_HEATING_TEST;
            failureId = EVENT_GUI_DIAGNOSTICS_RETORT_LEVELSENSOR_HEATING_TEST_FAILURE;
            okId = EVENT_GUI_DIAGNOSTICS_RETORT_LEVELSENSOR_HEATING_TEST_SUCCESS;
            break;
        case Service::RETORT_LEVEL_SENSOR_DETECTING:
            eventId = EVENT_GUI_DIAGNOSTICS_RETORT_LEVELSENSOR_DETECT_TEST;
            failureId = EVENT_GUI_DIAGNOSTICS_RETORT_LEVELSENSOR_DETECT_TEST_FAILURE;
            okId = EVENT_GUI_DIAGNOSTICS_RETORT_LEVELSENSOR_DETECT_TEST_SUCCESS;
            p_TestCase->SetParameter("CurStep", "1");
            break;
        case Service::RETORT_HEATING_WITH_WATER:
            eventId = EVENT_GUI_DIAGNOSTICS_RETORT_HEATING_LIQUID_TEST;
            failureId = EVENT_GUI_DIAGNOSTICS_RETORT_HEATING_LIQUID_TEST_FAILURE;
            okId = EVENT_GUI_DIAGNOSTICS_RETORT_HEATING_LIQUID_TEST_SUCCESS;
            p_TestCase->SetParameter("CurStep", "1");
            break;
        default:
            break;
        }

        Global::EventObject::Instance().RaiseEvent(eventId);
        emit PerformManufacturingTest(id, Service::TEST_CASE_ID_UNUSED);

        bool result = GetTestResponse();
        if (id == Service::RETORT_LID_LOCK && result == true) {
            nextFlag = ShowGuide(id, 1);
            if (nextFlag == false)
                break;
            emit PerformManufacturingTest(Service::RETORT_LID_LOCK, Service::TEST_CASE_ID_UNUSED);
            result = GetTestResponse();
        }
        else if (result == true && id == Service::RETORT_LEVEL_SENSOR_DETECTING) {
            // user operation finished!
            p_TestCase->SetParameter("CurStep", "2");
            MainMenu::CMessageDlg messageBox(mp_MainWindow);
            messageBox.setModal(true);
            messageBox.SetTitle(Service::CMessageString::MSG_TITLE_CONFIRM_WATER_LEVEL);
            messageBox.SetButtonText(1, Service::CMessageString::MSG_BUTTON_NO);
            messageBox.SetButtonText(3, Service::CMessageString::MSG_BUTTON_YES);
            messageBox.HideCenterButton();
            messageBox.SetText(Service::CMessageString::MSG_DIAGNOSTICS_TEST_COVER_SENSOR);
            messageBox.show();
            result = messageBox.exec() == 0 ? true : false;

            emit PerformManufacturingTest(id, Service::TEST_CASE_ID_UNUSED);
            (void)GetTestResponse();
        }
        else if (id == Service::RETORT_HEATING_WITH_WATER) {
            if (result == true) {
                // popup a message to inform operator to put the external sensor into retort.
                nextFlag = ShowGuide(id, 1);
                if (nextFlag == true) {
                    p_TestCase->SetParameter("CurStep", "2");
                    emit PerformManufacturingTest(id, Service::TEST_CASE_ID_UNUSED);
                    result = GetTestResponse();

                    QString Reason = p_TestCase->GetResult().value("FailReason");
                    if (Reason != "Abort" ) {
                        p_TestCase->SetParameter("CurStep", "3");
                        emit PerformManufacturingTest(id, Service::TEST_CASE_ID_UNUSED);
                        (void)GetTestResponse();
                    }
                    (void)ShowGuide(id, 2);
                }
                else {
                    emit PerformManufacturingTest(Service::TEST_ABORT, id);
                    result = GetTestResponse();
                }
            }
        }

        p_TestCase->SetStatus(result);

        if (result == false) {
            Global::EventObject::Instance().RaiseEvent(failureId);
            QString testCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(id);
            QString reason = p_TestCase->GetResult().value("FailReason");
            if (reason == "Abort") {   // if user click abort, then the test routines for this modules will terminate.
                mp_RetortManuf->EnableButton(true);
                return ;
            }

            QString text = QString("%1 - %2<br>%3").arg(testCaseDescription, Service::CMessageString::MSG_DIAGNOSTICS_FAILED,
                                                        p_TestCase->GetResult().value("FailReason"));
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, text, true);

            if ((id == Service::RETORT_HEATING_EMPTY || id == Service::RETORT_LEVEL_SENSOR_HEATING)
                    &&p_TestCase->GetResult().value("OutOfRange")!="1") {
                ShowHeatingFailedResult(id);
            }

        }
        else {
            Global::EventObject::Instance().RaiseEvent(okId);
            QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(id);
            QString text = QString("%1 - %2").arg(TestCaseDescription, Service::CMessageString::MSG_DIAGNOSTICS_SUCCESS);
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_INFO, text, true);
        }
        mp_RetortManuf->SetTestResult(id, result);
    }
    mp_RetortManuf->EnableButton(true);
}

void CManufacturingDiagnosticsHandler::PerformManufRVTests(const QList<Service::ModuleTestCaseID> &TestCaseList)
{
    quint32 FailureId(0);
    quint32 OkId(0);
    quint32 EventId(0);
    QString Text;
    quint8 Position(0);
    quint8 SealingTestNum(0);
    qDebug()<<"CManufacturingDiagnosticsHandler::PerformManufRVTests ---" << TestCaseList;
    for(int i=0; i<TestCaseList.size(); i++) {
        Service::ModuleTestCaseID Id = TestCaseList.at(i);
        bool NextFlag = ShowGuide(Id, 0);
        if (NextFlag == false) {
            break;
        }

Sealing_Test_Twice:

        QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
        DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

        switch( Id ) {
        case Service::ROTARY_VALVE_INITIALIZING:
            EventId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_INITIALIZING_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_INITIALIZING_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_INITIALIZING_TEST_SUCCESS;
            Text = Service::CMessageString::MSG_DIAGNOSTICS_INIT_RV;
            ShowMessage(Text);
            break;
        case Service::ROTARY_VALVE_SELECTION_FUNCTION:
            EventId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SELECTING_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SELECTING_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SELECTING_TEST_SUCCESS;
            Text =Service::CMessageString::MSG_DIAGNOSTICS_RV_SELECT_TEST;

            Position = GetPositionForRVTest(Id, 1);
            ShowMessage(Text);
            break;
        case Service::ROTARY_VALVE_SEALING_FUNCTION:
            SealingTestNum++;
            EventId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SEALING_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SEALING_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SEALING_TEST_SUCCESS;
            Text = Service::CMessageString::MSG_DIAGNOSTICS_RV_SEALINT_TEST;

            Position = GetPositionForRVTest(Id, SealingTestNum);

            ShowMessage(Text);
            break;
        case Service::ROTARY_VALVE_HEATING_END:
            EventId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_HEATING_END_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_HEATING_END_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_HEATING_END_TEST_SUCCESS;
            break;
        case Service::ROTARY_VALVE_HEATING_STATION:
            EventId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_HEATING_STATION_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_HEATING_STATION_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_HEATING_STATION_TEST_SUCCESS;
            break;
        default:
            break;
        }

        Global::EventObject::Instance().RaiseEvent(EventId);
        emit PerformManufacturingTest(Id);

        bool Result = GetTestResponse();
        p_TestCase->SetStatus(Result);

        if (Id == Service::ROTARY_VALVE_INITIALIZING ||
            Id == Service::ROTARY_VALVE_SELECTION_FUNCTION ||
            Id == Service::ROTARY_VALVE_SEALING_FUNCTION) {
            HideMessage();
        }


        if (Result == false) {
            Global::EventObject::Instance().RaiseEvent(FailureId);


            QString Reason = p_TestCase->GetResult().value("FailReason");
            if (Reason == "Abort") {   // if user click abort, then the test routines for this modules will terminate.
                mp_RotaryValveManuf->EnableButton(true);
                return ;
            }

            QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);
            Text = QString("%1 - %2<br>%3").arg(TestCaseDescription, Service::CMessageString::MSG_DIAGNOSTICS_FAILED, Reason);
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, Text, true);

            if (Id == Service::ROTARY_VALVE_HEATING_STATION && p_TestCase->GetResult().value("OutOfRange") != "1") {
                ShowHeatingFailedResult(Id);
            }
            else if (Id == Service::ROTARY_VALVE_HEATING_END  ) {
                QString TestStatus = p_TestCase->GetResult().value("CurStatus");
                if (TestStatus == "IN-HEATING")
                    ShowHeatingFailedResult(Id);
            }
            mp_RotaryValveManuf->SetTestResult(Id, Result);
            mp_RotaryValveManuf->EnableButton(true);
            return ;
        }
        else {
            if ( Id == Service::ROTARY_VALVE_INITIALIZING ||
                 Id == Service::ROTARY_VALVE_HEATING_STATION ||
                 Id == Service::ROTARY_VALVE_HEATING_END ) {
                Global::EventObject::Instance().RaiseEvent(OkId);
                QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);
                Text = QString("%1 - %2").arg(TestCaseDescription).arg(Service::CMessageString::MSG_DIAGNOSTICS_SUCCESS);
                mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_INFO, Text, true);
            }
            else if ( Id == Service::ROTARY_VALVE_SELECTION_FUNCTION ) {
                Result = ShowConfirmDlgForRVSelecting(Position);
                p_TestCase->SetStatus(Result);

                if (Result == false) {
                    Global::EventObject::Instance().RaiseEvent(FailureId);
                    mp_RotaryValveManuf->SetTestResult(Id, Result);
                    mp_RotaryValveManuf->EnableButton(true);
                    return ;
                }
                else {
                    Global::EventObject::Instance().RaiseEvent(OkId);
                }
            }
            else if ( Id == Service::ROTARY_VALVE_SEALING_FUNCTION ) {
                Result = ShowConfirmDlgForRVSealing(Position);
               // qDebug()<<"ShowConfirmDlgForRVSealing result = " << Result;
                if (Result == false) {
                    Global::EventObject::Instance().RaiseEvent(FailureId);
                    p_TestCase->SetStatus(Result);

                    QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);
                    Text = QString("%1 - %2").arg(TestCaseDescription).arg(Service::CMessageString::MSG_DIAGNOSTICS_FAILED);
                    mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, Text, true);


                    mp_RotaryValveManuf->SetTestResult(Id, Result);
                    mp_RotaryValveManuf->EnableButton(true);
                    return ;
                }
                else {
                    if (SealingTestNum == 1) {
                        Global::EventObject::Instance().RaiseEvent(OkId);
                        goto Sealing_Test_Twice;
                    }
                    else {
                        Global::EventObject::Instance().RaiseEvent(OkId);
                        QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);
                        Text = QString("%1 - %2").arg(TestCaseDescription).arg(Service::CMessageString::MSG_DIAGNOSTICS_SUCCESS);
                        mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_INFO, Text, true);
                    }
                }
            }
        }
        mp_RotaryValveManuf->SetTestResult(Id, Result);
    }
    mp_RotaryValveManuf->EnableButton(true);
}

void CManufacturingDiagnosticsHandler::PerformManufLATests(const QList<Service::ModuleTestCaseID> &TestCaseList)
{
    quint32 FailureId(0);
    quint32 OkId(0);
    quint32 EventId(0);
    qDebug()<<"CManufacturingDiagnosticsHandler::PerformManufLATests ---" << TestCaseList;
    for(int i=0; i<TestCaseList.size(); i++) {
        Service::ModuleTestCaseID Id = TestCaseList.at(i);
        bool NextFlag = ShowGuide(Id, 0);
        if (NextFlag == false) {
            break;
        }

        switch( Id ) {
        case Service::LA_SYSTEM_HEATING_LIQUID_TUBE:
            EventId = EVENT_GUI_DIAGNOSTICS_LASYSTEM_HEATING_LIQUID_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_LASYSTEM_HEATING_LIQUID_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_LASYSTEM_HEATING_LIQUID_TEST_SUCCESS;
            break;
        case Service::LA_SYSTEM_HEATING_AIR_TUBE:
            EventId = EVENT_GUI_DIAGNOSTICS_LASYSTEM_HEATING_AIR_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_LASYSTEM_HEATING_AIR_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_LASYSTEM_HEATING_AIR_TEST_SUCCESS;
            break;
        default:
            break;
        }

        Global::EventObject::Instance().RaiseEvent(EventId);
        emit PerformManufacturingTest(Id);

        qDebug()<<"GetTestResponse";

        bool Result = GetTestResponse();

        qDebug()<<"GetTestResponse = "<<Result;

        QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
        DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
        p_TestCase->SetStatus(Result);

        if (Result == false) {
            Global::EventObject::Instance().RaiseEvent(FailureId);

            QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);

            QString Reason = p_TestCase->GetResult().value("FailReason");
            if (Reason == "Abort") {   // if user click abort, then the test routines for this modules will terminate.
                mp_LaSystemManuf->EnableButton(true);
                return ;
            }

            QString Text = QString("%1 - %2<br>%3").arg(TestCaseDescription, Service::CMessageString::MSG_DIAGNOSTICS_FAILED,
                                                        p_TestCase->GetResult().value("FailReason"));
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, Text, true);

            if (p_TestCase->GetResult().value("OutOfRange")!="1")
                ShowHeatingFailedResult(Id);

        }
        else {
            Global::EventObject::Instance().RaiseEvent(OkId);
            QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);
            QString Text = QString("%1 - %2").arg(TestCaseDescription, Service::CMessageString::MSG_DIAGNOSTICS_SUCCESS);
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_INFO, Text, true);
        }
        mp_LaSystemManuf->SetTestResult(Id, Result);
    }
    mp_LaSystemManuf->EnableButton(true);
}

void CManufacturingDiagnosticsHandler::PerformManufSystemTests(const QList<Service::ModuleTestCaseID> &TestCaseList)
{
    quint32 FailureId(0);
    quint32 OkId(0);
    quint32 EventId(0);
    QString StrResult;
    QString RotatingMsg;
    DiagnosticsManufacturing::CSelectSealingDialog* p_Dlg = new DiagnosticsManufacturing::CSelectSealingDialog(mp_MainWindow);
    qDebug()<<"CManufacturingDiagnosticsHandler::PerformManufSystemTests ---" << TestCaseList;
    for(int i=0; i<TestCaseList.size(); i++) {
        StrResult.clear();
        Service::ModuleTestCaseID Id = TestCaseList.at(i);
        QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
        DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
        QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);

        bool Result   = true;
        bool NextFlag = true;

        switch(Id) {
        case Service::SYSTEM_110V_220V_SWITCH:
            EventId   = EVENT_GUI_DIAGNOSTICS_SYSTEM_110V220V_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_SYSTEM_110V220V_TEST_FAILURE;
            OkId      = EVENT_GUI_DIAGNOSTICS_SYSTEM_110V220V_TEST_SUCCESS;
            NextFlag = ShowGuide(Id, 0);
            if (!NextFlag) {
                break;
            }
            SelectOptionFor110v220Switch();
            emit PerformManufacturingTest(Id);
            Result = GetTestResponse();

            TestCaseDescription = Service::CMessageString::MSG_DIAGNOSTICS_VOLTAGE_TEST.arg(p_TestCase->GetParameter("ConnectedVoltage"));
            StrResult = Service::CMessageString::MSG_DIAGNOSTICS_CURRENT_VOLTAGE.arg(
                        p_TestCase->GetResult().value("CurrentVoltage3"), p_TestCase->GetResult().value("CurrentVoltage5"));

            break;
        case Service::SYSTEM_EXHAUST_FAN:
            EventId   = EVENT_GUI_DIAGNOSTICS_SYSTEM_EXHAUST_FUN_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_SYSTEM_EXHAUST_FUN_TEST_FAILURE;
            OkId      = EVENT_GUI_DIAGNOSTICS_SYSTEM_EXHAUST_FUN_TEST_SUCCESS;

            NextFlag = ShowGuide(Id, 0);
            if (!NextFlag) {
                break;
            }
            emit PerformManufacturingTest(Id);
            Result = GetTestResponse();
            break;
        case Service::SYSTEM_VENTILATION_FAN:
            EventId   = EVENT_GUI_DIAGNOSTICS_SYSTEM_VENTILATION_FUN_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_SYSTEM_VENTILATION_FUN_TEST_FAILURE;
            OkId      = EVENT_GUI_DIAGNOSTICS_SYSTEM_VENTILATION_FUN_TEST_SUCCESS;
            NextFlag = ShowGuide(Id, 0);
            if (!NextFlag) {
                break;
            }
            Result    = ShowConfirmDlgForSystemVentilationFan();
            break;
        case Service::SYSTEM_OVERFLOW:
            EventId   = EVENT_GUI_DIAGNOSTICS_SYSTEM_OVERFLOW_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_SYSTEM_OVERFLOW_TEST_FAILURE;
            OkId      = EVENT_GUI_DIAGNOSTICS_SYSTEM_OVERFLOW_TEST_SUCCESS;
            RotatingMsg = Service::CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_TUBE.arg("1");
            NextFlag = ShowGuide(Id, 0);
            if (!NextFlag) {
                break;
            }
            ShowMessage(RotatingMsg);
            emit PerformManufacturingTest(Id);
            Result = GetTestResponse();
            HideMessage();
            break;
        case Service::SYSTEM_SPEARKER:
            EventId   = EVENT_GUI_DIAGNOSTICS_SYSTEM_SPEAKER_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_SYSTEM_SPEAKER_TEST_FAILURE;
            OkId      = EVENT_GUI_DIAGNOSTICS_SYSTEM_SPEAKER_TEST_SUCCESS;

            p_TestCase->SetParameter("VolumeFlag", "0");
            NextFlag = ShowGuide(Id, 0);
            if (!NextFlag) {
                break;
            }
            for (int j = 0; j < 2 && Result; ++j) {

                emit PerformManufacturingTest(Id);
                Result = GetTestResponse();
            }
            break;
        case Service::SYSTEM_MAINS_RELAY:
            EventId   = EVENT_GUI_DIAGNOSTICS_SYSTEM_MAINS_RELAY_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_SYSTEM_MAINS_RELAY_TEST_FAILURE;
            OkId      = EVENT_GUI_DIAGNOSTICS_SYSTEM_MAINS_RELAY_TEST_SUCCESS;


            for (int j = 0; j < 2 && Result && NextFlag; ++j) {
                NextFlag = ShowGuide(Id, j);
                if (!NextFlag) {
                    break;
                }
                emit PerformManufacturingTest(Id);
                Result = GetTestResponse();
            }
            break;
        case Service::SYSTEM_SEALING_TEST:
            EventId   = EVENT_GUI_DIAGNOSTICS_SYSTEM_SEALING_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_SYSTEM_SEALING_TEST_FAILURE;
            OkId      = EVENT_GUI_DIAGNOSTICS_SYSTEM_SEALING_TEST_SUCCESS;

            NextFlag = ShowGuide(Id, 0);
            if (!NextFlag) {
                break;
            }

            // the first sealing test
            p_TestCase->SetParameter("CurStep", "1");
            p_TestCase->SetParameter("RVMove", "1");
            while(1) {
                emit PerformManufacturingTest(Id);
                Result = GetTestResponse();
                if (!Result) {
                    if (ShowConfirmDlgForSystemSealing()) {
                        continue;
                    }
                    else {
                        NextFlag = false;
                        break;
                    }
                }
                break;
            }
            if (Result == false) {
                break;
            }

            // the second sealing test
            p_TestCase->SetParameter("CurStep", "2");
            while(1) {
                emit PerformManufacturingTest(Id);
                Result = GetTestResponse();
                if (!Result) {
                    if (ShowConfirmDlgForSystemSealing()) {
                        continue;
                    }
                    else {
                        NextFlag = false;
                        break;
                    }
                }
                break;
            }
            if (Result == false) {
                break;
            }

            p_TestCase->SetParameter("CurStep", "3");
            // select test mode
            (void)p_Dlg->exec();
            if (p_Dlg->GetSelectedMode() == 0) {
                p_TestCase->SetParameter("TestMode", "0");
            }
            else {
                DiagnosticsManufacturing::CSelectPositionDialog *p_SelectDlg = new DiagnosticsManufacturing::CSelectPositionDialog(Id, 1, true, mp_MainWindow);
                (void)p_SelectDlg->exec();

                p_TestCase->SetParameter("TestMode", "1");
                p_TestCase->SetParameter("Position", QString::number(p_SelectDlg->GetPosition() + 1));
                delete p_SelectDlg;
            }

            emit PerformManufacturingTest(Id);
            Result = GetTestResponse();

            break;
        case Service::SYSTEM_REMOTE_LOCAL_ALARM:
            EventId   = EVENT_GUI_DIAGNOSTICS_SYSTEM_ALARM_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_SYSTEM_ALARM_TEST_FAILURE;
            OkId      = EVENT_GUI_DIAGNOSTICS_SYSTEM_ALARM_TEST_SUCCESS;

            p_TestCase->SetParameter("ConnectFlag", "1");
            p_TestCase->SetParameter("AlarmFlag", "1");
            for (int j = 0; j < 4 && Result && NextFlag; ++j) {
                NextFlag = ShowGuide(Id, j);
                if (!NextFlag) {
                    break;
                }
                emit PerformManufacturingTest(Id);
                Result = GetTestResponse();
            }
            break;
        default:
            break;
        }

        if (!NextFlag) {
            break;
        }

        Global::EventObject::Instance().RaiseEvent(EventId);

        p_TestCase->SetStatus(Result);

        if (!Result) {
            Global::EventObject::Instance().RaiseEvent(FailureId);

            QString Reason = p_TestCase->GetResult().value("FailReason");
            if (Reason == "Abort") {   // if user click abort, then the test routines for this modules will terminate.
                mp_SystemManuf->EnableButton(true);
                return ;
            }

            QString Text = QString("%1 - %2<br>%3").arg(TestCaseDescription, Service::CMessageString::MSG_DIAGNOSTICS_FAILED,
                                                        p_TestCase->GetResult().value("FailReason"));
            Text.append(StrResult);
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, Text, true);

        }
        else {
            Global::EventObject::Instance().RaiseEvent(OkId);
            QString Text = QString("%1 - %2").arg(TestCaseDescription, Service::CMessageString::MSG_DIAGNOSTICS_SUCCESS);
            Text.append(StrResult);
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_INFO, Text, true);
        }
        if (Id == Service::SYSTEM_SEALING_TEST) {
            (void)GetTestResponse();
        }

        mp_SystemManuf->SetTestResult(Id, Result);
    }

    delete p_Dlg;
    mp_SystemManuf->EnableButton(true);
}

void CManufacturingDiagnosticsHandler::PerformManufCleaningSystem(const QList<Service::ModuleTestCaseID> &TestCaseList)
{
    quint32 FailureId(0);
    quint32 OkId(0);
    quint32 EventId(0);
    qDebug()<<"CManufacturingDiagnosticsHandler::PerformManufCleaningSystem ---" << TestCaseList;

    for(int i=0; i<TestCaseList.size(); i++) {
        Service::ModuleTestCaseID Id = TestCaseList.at(i);
        QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
        DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
        QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);

        bool Result = false;
        bool NextFlag = ShowGuide(Id, 0);
        if (!NextFlag) {
            break;
        }

        switch(Id) {
        case Service::CLEANING_SYSTEM_TEST:
            EventId   = EVENT_GUI_DIAGNOSTICS_CLEANING_SYSTEM_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_CLEANING_SYSTEM_TEST_FAILURE;
            OkId      = EVENT_GUI_DIAGNOSTICS_CLEANING_SYSTEM_TEST_SUCCESS;

            p_TestCase->SetParameter("TestStep", "1");
            ShowMessage(Service::CMessageString::MSG_DIAGNOSTICS_CLEANING_BEGIN);

            emit PerformManufacturingTest(Id);
            Result = GetTestResponse();

            if (!Result) {
                break;
            }

            p_TestCase->SetParameter("TestStep", "2");

            NextFlag = ShowGuide(Id, 1) && ShowGuide(Id, 2);
            if (!NextFlag) {
                break;
            }

            emit PerformManufacturingTest(Id);
            Result = GetTestResponse();
            HideMessage();
        default:
            break;
        }

        if (!NextFlag) {
            break;
        }

        Global::EventObject::Instance().RaiseEvent(EventId);

        p_TestCase->SetStatus(Result);

        if (!Result) {
            Global::EventObject::Instance().RaiseEvent(FailureId);

            QString Reason = p_TestCase->GetResult().value("FailReason");
            if (Reason == "Abort") {   // if user click abort, then the test routines for this modules will terminate.
                mp_CleaningManuf->EnableButton(true);
                return ;
            }

            QString Text = QString("%1 - %2<br>%3").arg(TestCaseDescription, Service::CMessageString::MSG_DIAGNOSTICS_FAILED,
                                                        p_TestCase->GetResult().value("FailReason"));
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, Text, true);

        }
        else {
            Global::EventObject::Instance().RaiseEvent(OkId);
            QString Text = QString("%1 - %2").arg(TestCaseDescription, Service::CMessageString::MSG_DIAGNOSTICS_SUCCESS);
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_INFO, Text, true);
            (void)ShowGuide(Id, 3, false);
        }
        mp_CleaningManuf->SetTestResult(Id, Result);
    }
    mp_CleaningManuf->EnableButton(true);
}

void CManufacturingDiagnosticsHandler::PerformFirmwareUpdate(const QList<Service::ModuleTestCaseID> &TestCaseList)
{
    Q_UNUSED(TestCaseList);

    Service::ModuleTestCaseID Id = Service::FIRMWARE_UPDATE;

    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);


    QString Msg = Service::CMessageString::MSG_DIAGNOSTICS_UPDATE_FIRMWARE_SLAVE.arg(p_TestCase->GetParameter("SlaveType"));

    ShowMessage(Msg);
    emit PerformManufacturingTest(Id);

    bool Result = GetTestResponse();
    HideMessage();

    emit PerformManufacturingTest(Service::FIRMWARE_GET_SLAVE_INFO);
    Result = GetTestResponse();

    qDebug()<<"Update Firmware result = "<<Result;

}

void CManufacturingDiagnosticsHandler::PerformOperationTimeReset(const QList<Service::ModuleTestCaseID> &TestCaseList)
{
    Q_UNUSED(TestCaseList);

    Service::ModuleTestCaseID Id = Service::RESET_OPERATION_TIME;

    emit PerformManufacturingTest(Id);

    bool Result = GetTestResponse();

    qDebug()<<"Update Firmware result = "<<Result;
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
        qDebug()<<"m_LoopManufacturingTest exit = "<<ret;
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
    if (mp_WaitDialog != NULL) {
        (void)mp_WaitDialog->close();
        delete mp_WaitDialog;
        mp_WaitDialog = NULL;
    }
    mp_WaitDialog = new MainMenu::CMessageDlg(mp_MainWindow);
    mp_WaitDialog->setMinimumWidth(300);
    mp_WaitDialog->setMinimumHeight(150);
    mp_WaitDialog->SetDialogTitle(Service::CMessageString::MSG_TITLE_WAIT);
    mp_WaitDialog->SetText(Message);
    mp_WaitDialog->HideAllButtons();

    mp_WaitDialog->setModal(true);
    mp_WaitDialog->show();
}

/****************************************************************************/
/*!
 *  \brief Throws open a Pop Up Message with current pressure
 *  \iparam Message = Current pressure value
 */
/****************************************************************************/
void CManufacturingDiagnosticsHandler::ShowCurrentPressure(const QString &Message)
{
    if (mp_WaitDialog == NULL) {
        mp_WaitDialog = new MainMenu::CMessageDlg(mp_MainWindow);
        mp_WaitDialog->setMinimumWidth(300);
        mp_WaitDialog->setMinimumHeight(150);
        mp_WaitDialog->SetDialogTitle(Service::CMessageString::MSG_TITLE_WAIT);
        mp_WaitDialog->SetText(Message);
        mp_WaitDialog->HideAllButtons();

        mp_WaitDialog->setModal(true);
        mp_WaitDialog->show();
    }
    else {
        mp_WaitDialog->SetText(Message);
    }
}

/****************************************************************************/
/*!
 *  \brief Hide the Pop Up Message Dialog
 */
/****************************************************************************/
void CManufacturingDiagnosticsHandler::HideMessage()
{
    if (mp_WaitDialog != NULL) {
        (void)mp_WaitDialog->close();
        delete mp_WaitDialog;
        mp_WaitDialog = NULL;
    }
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

void CManufacturingDiagnosticsHandler::LockTest(bool LockFlag)
{
    bool enabled = !LockFlag;
    mp_MainWindow->SetTabEnabled(0, enabled);
    mp_MainWindow->SetTabEnabled(1, enabled);
    mp_MainWindow->SetTabEnabled(3, enabled);
    mp_MainWindow->SetTabEnabled(4, enabled);

    mp_DiagnosticsManufGroup->setEnabled(enabled);
}

} // end of namespace Core
