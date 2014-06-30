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
#include "DiagnosticsManufacturing/Include/PressureInputDialog.h"

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
    mp_MainWindow(p_MainWindow),
    mp_WaitDialog(NULL)
{
    m_FailStr = QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                        "Fail", 0, QApplication::UnicodeUTF8);
    m_SuccessStr = QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                           "Success", 0, QApplication::UnicodeUTF8);
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
//    mp_DiagnosticsManufGroup->Clear();
    //Diagnostics
    if (Core::CSelectTestOptions::GetCurTestMode() == Core::MANUFACTURAL_ENDTEST ) {
        mp_DiagnosticsManufGroup->AddPanel(QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                       "Display", 0, QApplication::UnicodeUTF8), mp_DisplayManuf);
    }

    mp_DiagnosticsManufGroup->AddPanel(QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                       "Oven", 0, QApplication::UnicodeUTF8), mp_OvenManuf);
    mp_DiagnosticsManufGroup->AddPanel(QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                                               "Rotary Valve", 0, QApplication::UnicodeUTF8), mp_RotaryValveManuf);
    mp_DiagnosticsManufGroup->AddPanel(QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                       "Retort", 0, QApplication::UnicodeUTF8), mp_RetortManuf);
    if (Core::CSelectTestOptions::GetCurTestMode() == Core::MANUFACTURAL_ENDTEST ) {
        mp_DiagnosticsManufGroup->AddPanel(QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                       "Main Control", 0, QApplication::UnicodeUTF8), mp_MainControlManuf);
        mp_DiagnosticsManufGroup->AddPanel(QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                       "LA System", 0, QApplication::UnicodeUTF8), mp_LaSystemManuf);
        mp_DiagnosticsManufGroup->AddPanel(QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                       "System", 0, QApplication::UnicodeUTF8), mp_SystemManuf);
        mp_DiagnosticsManufGroup->AddPanel(QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                       "Cleaning", 0, QApplication::UnicodeUTF8), mp_CleaningManuf);
    }
    mp_MainWindow->AddMenuGroup(mp_DiagnosticsManufGroup, QApplication::translate
                   ("Core::CManufacturingDiagnosticsHandler", "Diagnostics", 0, QApplication::UnicodeUTF8));
}

/****************************************************************************/
/*!
 *  \brief Slot called for Module tests for manufacturing SW
 *  \iparam ModuleName = Name of the module
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
        PerformManufOvenTests(TestCaseList);
        break;
    case Service::MAIN_CONTROL:
        PerformManufMainControlTests(TestCaseList);
        break;
    case Service::LA_SYSTEM:
        PerformManufLATests(TestCaseList);
        break;
    case Service::ROTARY_VALVE:
        PerformManufRVTests(TestCaseList);
        break;
    case Service::RETORT:
        PerformManufRetortTests(TestCaseList);
        break;
    case Service::SYSTEM:
        PerformManufSystemTests(TestCaseList);
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
    return ret == 1 ? true : false;
}

bool CManufacturingDiagnosticsHandler::ShowGuide(Service::ModuleTestCaseID Id, int Index)
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
    dlg->HideCenterButton();
    dlg->SetButtonText(3, tr("Next"));
    dlg->SetButtonText(1, tr("Cancel"));
    if (Index == 1 && Id == Service::OVEN_COVER_SENSOR ||
            Index == 2 && Id == Service::RETORT_HEATING_WITH_WATER ) {
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
    if (Service::RETORT_LEVEL_SENSOR_HEATING) {
        dlg->DisplayLSensorLabel();
    }
    dlg->UpdateLabel(Status);

    Text.append(" ");
    Text.append("Failed !");
    dlg->SetDialogTitle("Error");
    dlg->SetText(Text);
    dlg->HideAbort(true);
    dlg->exec();
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
    p_Dlg->exec();

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

    QString ReadyText = QString("Positon %1# is ready.").arg(Position);
    QString ResultConfirm = "Is the test pass ?";
    QString Text = QString("%1 %2 %3").arg(ReadyText).arg(GuideText).arg(ResultConfirm);
    // display success message
    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(mp_MainWindow);
    dlg->SetTitle(TestCaseDescription);
    dlg->SetIcon(QMessageBox::Information);
    dlg->SetText(Text);
    dlg->HideCenterButton();
    dlg->SetButtonText(3, tr("Pass"));
    dlg->SetButtonText(1, tr("Fail"));


    int ret = dlg->exec();

    qDebug()<<"return code = "<<ret;

    delete dlg;

    if ( ret == 0 )
        return true;

    return false;
}

bool CManufacturingDiagnosticsHandler::ShowConfirmDlgForRVSealing(quint8 Position)
{
    DiagnosticsManufacturing::CPressureInputDialog *p_Dlg = new DiagnosticsManufacturing::CPressureInputDialog(Service::ROTARY_VALVE_SEALING_FUNCTION, mp_MainWindow);

    p_Dlg->exec();

    bool Result = p_Dlg->GetResult();

    delete p_Dlg;

    return Result;
}

bool CManufacturingDiagnosticsHandler::ShowConfirmDlgForSystemFan(Service::ModuleTestCaseID Id)
{
    QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);
    QString Text;

    MainMenu::CMessageDlg *dlg = new MainMenu::CMessageDlg(mp_MainWindow);
    dlg->SetTitle(TestCaseDescription);
    dlg->SetIcon(QMessageBox::Information);

    if (Id == Service::SYSTEM_VENTILATION_FAN) {
        Text = "Please check if the ventilation fan is runing";
    }
    else if (Id == Service::SYSTEM_EXHAUST_FAN) {
        Text = "Please open the retort lid, and check if the exhaust fan is runing.";
    }

    dlg->SetText(Text);
    dlg->HideCenterButton();
    dlg->SetButtonText(3, tr("Pass"));
    dlg->SetButtonText(1, tr("Fail"));

    int ret = dlg->exec();

    delete dlg;

    if ( ret == 0 )
        return true;

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

            QString Text = QString("%1 - %2\n%3").arg(TestCaseDescription, m_FailStr, p_TestCase->GetResult().value("FailReason"));
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, Text, true);

            if (Id != Service::OVEN_COVER_SENSOR) {
                ShowHeatingFailedResult(Id);
            }

        }
        else {
            Global::EventObject::Instance().RaiseEvent(OkId);
            QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);
            QString Text = QString("%1 - %2").arg(TestCaseDescription, m_SuccessStr);
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
        }

        Global::EventObject::Instance().RaiseEvent(EventId);
        emit PerformManufacturingTest(Id);

        bool Result = GetTestResponse();
        QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
        DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);
        p_TestCase->SetStatus(Result);

        if (Result == false) {
            Global::EventObject::Instance().RaiseEvent(FailureId);

            qreal Voltage = p_TestCase->GetResult().value("Voltage").toDouble();
            qreal Current = p_TestCase->GetResult().value("Current").toDouble();
            QString Text = QString("ASB %1 DC output voltage is failed (%2 V), \nand current is failed (%3 mA).").arg(ASBIndex)
                    .arg(Voltage).arg(Current);

            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, Text, true);
        }
        else {
            Global::EventObject::Instance().RaiseEvent(OkId);
            qreal Voltage = p_TestCase->GetResult().value("Voltage").toDouble();
            qreal Current = p_TestCase->GetResult().value("Current").toDouble();
            QString Text = QString("ASB %1 DC output voltage is Ok (%2 V), \nand current is Ok (%3 mA).").arg(ASBIndex)
                    .arg(Voltage).arg(Current);

            qDebug()<<"Show MessageBox " << ASBIndex;
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_INFO, Text, true);
            qDebug()<<"End Show MessageBox "<<ASBIndex;
        }
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
        else if (id == Service::RETORT_LEVEL_SENSOR_DETECTING) {
            // user operation finished!
            p_TestCase->SetParameter("CurStep", "2");

            MainMenu::CMessageDlg messageBox;
            messageBox.setModal(true);
            messageBox.SetTitle(tr("Confirm water level"));
            messageBox.SetButtonText(1, "No");
            messageBox.SetButtonText(3, "Yes");
            messageBox.HideCenterButton();
            messageBox.SetText("Open the retort lid, do you see water cover the level sensor?");
            messageBox.show();
            result = messageBox.exec() == 0 ? true : false;
            emit PerformManufacturingTest(id, Service::TEST_CASE_ID_UNUSED);
            (void)GetTestResponse();
        }
        else if (id == Service::RETORT_HEATING_WITH_WATER) {
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
                    GetTestResponse();
                }
                ShowGuide(id, 2);
            }
            else {
                emit PerformManufacturingTest(Service::TEST_ABORT, id);
                result = GetTestResponse();
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

            QString text = QString("%1 - %2\n%3").arg(testCaseDescription, m_FailStr, p_TestCase->GetResult().value("FailReason"));
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, text, true);

            if (id != Service::RETORT_LID_LOCK && id != Service::RETORT_LEVEL_SENSOR_DETECTING) {
                ShowHeatingFailedResult(id);
            }
        }
        else {
            Global::EventObject::Instance().RaiseEvent(okId);
            QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(id);
            QString text = QString("%1 - %2").arg(TestCaseDescription, m_SuccessStr);
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
            Text = QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                           "Initializing rotary valve in progress...", 0, QApplication::UnicodeUTF8);
            ShowMessage(Text);
            break;
        case Service::ROTARY_VALVE_SELECTION_FUNCTION:
            EventId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SELECTING_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SELECTING_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SELECTING_TEST_SUCCESS;
            Text = QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                           "Rotary valve selecting test in progress ...", 0, QApplication::UnicodeUTF8);

            Position = GetPositionForRVTest(Id, 1);
            ShowMessage(Text);
            break;
        case Service::ROTARY_VALVE_SEALING_FUNCTION:
            SealingTestNum++;
            EventId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SEALING_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SEALING_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SEALING_TEST_SUCCESS;
            Text = QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                           "Rotary valve sealing test in progress ...", 0, QApplication::UnicodeUTF8);

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
            Text = QString("%1 - %2\n%3").arg(TestCaseDescription).arg(m_FailStr).arg(Reason);
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, Text, true);

            if (Id == Service::ROTARY_VALVE_HEATING_END ||
                    Id == Service::ROTARY_VALVE_HEATING_STATION ) {
                if (Reason != "NOT-IN-HEATING")
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
                QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);
                Text = QString("%1 - %2").arg(TestCaseDescription).arg(m_SuccessStr);
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
                    Text = QString("%1 - %2").arg(TestCaseDescription).arg(m_FailStr);
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
                        Text = QString("%1 - %2").arg(TestCaseDescription).arg(m_SuccessStr);
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

            QString Text = QString("%1 - %2\n%3").arg(TestCaseDescription, m_FailStr, p_TestCase->GetResult().value("FailReason"));
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, Text, true);


            ShowHeatingFailedResult(Id);

        }
        else {
            Global::EventObject::Instance().RaiseEvent(OkId);
            QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);
            QString Text = QString("%1 - %2").arg(TestCaseDescription, m_SuccessStr);
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
    qDebug()<<"CManufacturingDiagnosticsHandler::PerformManufSystemTests ---" << TestCaseList;
    for(int i=0; i<TestCaseList.size(); i++) {
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

            TestCaseDescription = p_TestCase->GetParameter("ConnectedVoltage") + "V test";
            StrResult = "(Current Voltage:"+p_TestCase->GetResult().value("CurrentVoltage")+"V)";

            break;
        case Service::SYSTEM_EXHAUST_FAN:
            EventId   = EVENT_GUI_DIAGNOSTICS_SYSTEM_EXHAUST_FUN_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_SYSTEM_EXHAUST_FUN_TEST_FAILURE;
            OkId      = EVENT_GUI_DIAGNOSTICS_SYSTEM_EXHAUST_FUN_TEST_SUCCESS;

            NextFlag = ShowGuide(Id, 0);
            if (!NextFlag) {
                break;
            }

            Result    = ShowConfirmDlgForSystemFan(Id);
            for (int j = 1; j <= 4 && Result && NextFlag; ++j) {
                NextFlag = ShowGuide(Id, j);
                if (!NextFlag) {
                    break;
                }
                emit PerformManufacturingTest(Id);
                Result = GetTestResponse();
            }
            break;
        case Service::SYSTEM_VENTILATION_FAN:
            EventId   = EVENT_GUI_DIAGNOSTICS_SYSTEM_VENTILATION_FUN_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_SYSTEM_VENTILATION_FUN_TEST_FAILURE;
            OkId      = EVENT_GUI_DIAGNOSTICS_SYSTEM_VENTILATION_FUN_TEST_SUCCESS;
            Result    = ShowConfirmDlgForSystemFan(Id);
            break;
        case Service::SYSTEM_OVERFLOW:
            EventId   = EVENT_GUI_DIAGNOSTICS_SYSTEM_OVERFLOW_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_SYSTEM_OVERFLOW_TEST_FAILURE;
            OkId      = EVENT_GUI_DIAGNOSTICS_SYSTEM_OVERFLOW_TEST_SUCCESS;
            RotatingMsg = QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                                "RV is rotating to tube #1", 0, QApplication::UnicodeUTF8);
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
            for (int j = 0; j < 2 && Result && NextFlag; ++j) {
                NextFlag = ShowGuide(Id, j);
                if (!NextFlag) {
                    break;
                }
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
            break;
        case Service::SYSTEM_REMOTE_LOCAL_ALARM:
            EventId   = EVENT_GUI_DIAGNOSTICS_SYSTEM_ALARM_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_SYSTEM_ALARM_TEST_FAILURE;
            OkId      = EVENT_GUI_DIAGNOSTICS_SYSTEM_ALARM_TEST_SUCCESS;

            for (int j = 0; j < 4 && Result && NextFlag; ++j) {
                NextFlag = ShowGuide(Id, j);
                if (!NextFlag) {
                    break;
                }
                emit PerformManufacturingTest(Id);
                Result = GetTestResponse();
            }
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

            QString Text = QString("%1 - %2\n%3").arg(TestCaseDescription, m_FailStr, p_TestCase->GetResult().value("FailReason"));
            Text.append(StrResult);
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, Text, true);

        }
        else {
            Global::EventObject::Instance().RaiseEvent(OkId);
            QString Text = QString("%1 - %2").arg(TestCaseDescription, m_SuccessStr);
            Text.append(StrResult);
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_INFO, Text, true);
        }
        mp_SystemManuf->SetTestResult(Id, Result);
    }
    mp_SystemManuf->EnableButton(true);
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
        mp_WaitDialog->close();
        delete mp_WaitDialog;
        mp_WaitDialog = NULL;
    }
    mp_WaitDialog = new MainMenu::CMessageDlg(mp_MainWindow);
    mp_WaitDialog->SetDialogTitle(tr("Please Wait"));
    mp_WaitDialog->SetText(Message);
    mp_WaitDialog->HideAllButtons();
    mp_WaitDialog->setMinimumWidth(200);
    mp_WaitDialog->setMinimumHeight(150);

    mp_WaitDialog->show();
    mp_WaitDialog->setModal(false);
}

/****************************************************************************/
/*!
 *  \brief Hide the Pop Up Message Dialog
 */
/****************************************************************************/
void CManufacturingDiagnosticsHandler::HideMessage()
{
    if (mp_WaitDialog != NULL) {
        mp_WaitDialog->close();
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

} // end of namespace Core
