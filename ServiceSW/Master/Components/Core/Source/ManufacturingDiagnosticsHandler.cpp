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
#include "ServiceDataManager/Include/TestCaseGuide.h"
#include "ServiceDataManager/Include/TestCase.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"
#include "DiagnosticsManufacturing/Include/SelectPositionDialog.h"
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
    mp_MainWindow(p_MainWindow)
{
    m_FailStr = QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                        "Fail", 0, QApplication::UnicodeUTF8);
    m_SuccessStr = QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                           "Success", 0, QApplication::UnicodeUTF8);
    //Diagnostics Manufacturing
    mp_DiagnosticsManufGroup = new MainMenu::CMenuGroup;
    mp_OvenManuf             = new DiagnosticsManufacturing::COven(mp_ServiceConnector, mp_MainWindow);
    mp_MainControlManuf      = new DiagnosticsManufacturing::CMainControl(mp_ServiceConnector, mp_MainWindow);
    mp_LaSystemManuf         = new DiagnosticsManufacturing::CLaSystem(mp_ServiceConnector, mp_MainWindow);
    mp_RotaryValveManuf      = new DiagnosticsManufacturing::CRotaryValve(mp_ServiceConnector, mp_MainWindow);

    CONNECTSIGNALSLOTGUI(mp_OvenManuf, BeginModuleTest(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>), this, BeginManufacturingSWTests(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>));
    CONNECTSIGNALSLOTGUI(mp_MainControlManuf, BeginModuleTest(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>), this, BeginManufacturingSWTests(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>));
    CONNECTSIGNALSLOTGUI(mp_LaSystemManuf, BeginModuleTest(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>), this, BeginManufacturingSWTests(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>));
    CONNECTSIGNALSLOTGUI(mp_RotaryValveManuf, BeginModuleTest(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>), this, BeginManufacturingSWTests(Service::ModuleNames_t, QList<Service::ModuleTestCaseID>));

    /* Manufacturing SW Reset status */
    CONNECTSIGNALSLOTGUI(mp_DiagnosticsManufGroup, PanelChanged(), mp_OvenManuf, ResetTestStatus());
    CONNECTSIGNALSLOTGUI(mp_DiagnosticsManufGroup, PanelChanged(), mp_MainControlManuf, ResetTestStatus());
    CONNECTSIGNALSLOTGUI(mp_DiagnosticsManufGroup, PanelChanged(), mp_LaSystemManuf, ResetTestStatus());
    CONNECTSIGNALSLOTGUI(mp_DiagnosticsManufGroup, PanelChanged(), mp_RotaryValveManuf, ResetTestStatus());

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
        delete mp_LaSystemManuf;
        delete mp_RotaryValveManuf;
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
    mp_DiagnosticsManufGroup->AddPanel(QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                                               "Rotary Valve", 0, QApplication::UnicodeUTF8), mp_RotaryValveManuf);
    if (Core::CSelectTestOptions::GetCurTestMode() == Core::MANUFACTURAL_ENDTEST ) {
        mp_DiagnosticsManufGroup->AddPanel(QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                       "Main Control", 0, QApplication::UnicodeUTF8), mp_MainControlManuf);
        mp_DiagnosticsManufGroup->AddPanel(QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                       "LA System", 0, QApplication::UnicodeUTF8), mp_LaSystemManuf);
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
            GuideText.append("\n");
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

void CManufacturingDiagnosticsHandler::ShowHeatingFailedResult(Service::ModuleTestCaseID Id)
{
    DiagnosticsManufacturing::CHeatingTestDialog *dlg = new DiagnosticsManufacturing::CHeatingTestDialog(Id, mp_MainWindow);
    QString Text = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase *p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    Service::ModuleTestStatus Status = p_TestCase->GetResult();
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

    qDebug()<<"Position = " << Position;
    delete p_Dlg;

    return Position;
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

    QString ReadyText = QString("Positon %1 is ready.").arg(Position);
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
        emit PerformManufacturingTest(Id);

        bool Result = GetTestResponse();

        if (Id == Service::OVEN_COVER_SENSOR && Result == true) {
            NextFlag = ShowGuide(Id, 1);
            if (NextFlag == false)
                break;
            emit PerformManufacturingTest(Service::OVEN_COVER_SENSOR);
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

void CManufacturingDiagnosticsHandler::PerformManufRVTests(const QList<Service::ModuleTestCaseID> &TestCaseList)
{
    quint32 FailureId(0);
    quint32 OkId(0);
    quint32 EventId(0);
    QString Text;
    quint8 Position(0);
    qDebug()<<"CManufacturingDiagnosticsHandler::PerformManufRVTests ---" << TestCaseList;
    for(int i=0; i<TestCaseList.size(); i++) {
        Service::ModuleTestCaseID Id = TestCaseList.at(i);
        bool NextFlag = ShowGuide(Id, 0);
        if (NextFlag == false) {
            break;
        }

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
            break;
        case Service::ROTARY_VALVE_SEALING_FUNCTION:
            EventId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SEALING_TEST;
            FailureId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SEALING_TEST_FAILURE;
            OkId = EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SEALING_TEST_SUCCESS;
            Text = QApplication::translate("Core::CManufacturingDiagnosticsHandler",
                                           "Rotary valve sealing test in progress ...", 0, QApplication::UnicodeUTF8);
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


        if ( Id == Service::ROTARY_VALVE_SELECTION_FUNCTION ||
             Id == Service::ROTARY_VALVE_SEALING_FUNCTION ) {
            Position = GetPositionForRVTest(Id, 1);

            ShowMessage(Text);
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
            qDebug()<<"Faile Reason="<<Reason;

            QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);
            Text = QString("%1 - %2").arg(TestCaseDescription).arg(m_FailStr);
            mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_ERROR, Text, true);

            mp_RotaryValveManuf->EnableButton(true);
            return ;
        }
        else {
            if ( Id == Service::ROTARY_VALVE_INITIALIZING) {
                QString TestCaseDescription = DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id);
                Text = QString("%1 - %2").arg(TestCaseDescription).arg(m_SuccessStr);
                mp_ServiceConnector->ShowMessageDialog(Global::GUIMSGTYPE_INFO, Text, true);
            }
            else if ( Id == Service::ROTARY_VALVE_SELECTION_FUNCTION ) {
                Result = ShowConfirmDlgForRVSelecting(Position);
                p_TestCase->SetStatus(Result);

                if (Result == false) {
                    mp_RotaryValveManuf->SetTestResult(Id, Result);
                    mp_RotaryValveManuf->EnableButton(true);
                    return ;
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
        mp_LaSystemManuf->SetTestResult(Id, Result);
    }
    mp_LaSystemManuf->EnableButton(true);
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
    mp_ServiceConnector->ShowBusyDialog(Message, true);
}

/****************************************************************************/
/*!
 *  \brief Hide the Pop Up Message Dialog
 */
/****************************************************************************/
void CManufacturingDiagnosticsHandler::HideMessage()
{
    mp_ServiceConnector->HideBusyDialog();
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
