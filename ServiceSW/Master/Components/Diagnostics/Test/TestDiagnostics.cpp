/****************************************************************************/
/*! \file TestDiagnostics.cpp
 *
 *  \brief Unit test for Diagnostics menu
 *
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-08-06
 *  $Author:    $ Soumya. D
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

#include <QtTest/QTest>
#include <QDebug>
#include <MainMenu/Include/MainWindow.h>
//#include <Diagnostics/Include/Agitator.h>
//#include <Diagnostics/Include/AirExhaust.h>
//#include <Diagnostics/Include/Consumables.h>
//#include <Diagnostics/Include/CoverHood.h>
//#include "Diagnostics/Include/Display.h"
//#include <Diagnostics/Include/DrawerLoader.h>
//#include <Diagnostics/Include/DrawerUnloader.h>
//#include <Diagnostics/Include/HeatedCuvettes.h>
//#include <Diagnostics/Include/Oven.h>
//#include <Diagnostics/Include/SoftSwitch.h>
//#include <Diagnostics/Include/WaterStations.h>
//#include <Diagnostics/Include/XMovement.h>
#include "Diagnostics/Include/InitialSystem/LTubePreTest.h"
#include "Diagnostics/Include/InitialSystem/MainsRelayTest.h"
#include "Diagnostics/Include/InitialSystem/OvenPreTest.h"
#include "Diagnostics/Include/InitialSystem/RetortPreTest.h"
#include "Diagnostics/Include/InitialSystem/RVPreTest.h"
#include "Diagnostics/Include/InitialSystem/ACVoltageTest.h"
#include "Diagnostics/Include/InitialSystem/InitialSystemCheck.h"
#include "Diagnostics/Include/Oven/CoverSensorTest.h"
#include "Diagnostics/Include/Oven/OvenHeatingTestEmpty.h"
#include "Diagnostics/Include/RotaryValve/MovementTest.h"
#include "Diagnostics/Include/DiagnosticMessageDlg.h"
#include <QObject>
#include <QMessageBox>
#include <QMainWindow>
#include "Global/Include/SystemPaths.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

namespace Diagnostics {

namespace InitialSystem {
class LTubePreUT : public CLTubePreTest
{
    Q_OBJECT
public:

    LTubePreUT()
        : CLTubePreTest(NULL)
    {
    }

    ~LTubePreUT()
    {
    }

    /****************************************************************************/
    /*!
     *  \brief The function for start pre heating
     */
    /****************************************************************************/
    void StartPreHeating()
    {

    }

private:
    /****************************************************************************/
    /*!
     *  \brief To show/close waiting dialog
     *  \iparam ShowFlag = true to show and flase to close the dialog
     */
    /****************************************************************************/
    void ShowWaitingMessage(bool ShowFlag=true)
    {
    }

    /****************************************************************************/
    /*!
     *  \brief To show the fail message dialog
     *  \iparam Error = the error type
     */
    /****************************************************************************/
    void ShowFailMessage(int ErrorCode)
    {
    }

};

class MainsRelayUT : public CMainsRelayTest
{
    Q_OBJECT
public:

    MainsRelayUT()
        : CMainsRelayTest(NULL)
    {
    }

    ~MainsRelayUT()
    {
    }


private:
    /****************************************************************************/
    /*!
     *  \brief To show/close waiting dialog
     *  \iparam ShowFlag = true to show and flase to close the dialog
     */
    /****************************************************************************/
    void ShowWaitingMessage(bool ShowFlag=true)
    {
    }

    /****************************************************************************/
    /*!
     *  \brief To show the fail message dialog
     *  \iparam Error = the error type
     */
    /****************************************************************************/
    void ShowFailMessage(int ErrorCode)
    {
    }

};

class OvenPreUT : public COvenPreTest
{
    Q_OBJECT
public:

    OvenPreUT()
        : COvenPreTest(NULL)
    {
    }

    ~OvenPreUT()
    {
    }


private:
    /****************************************************************************/
    /*!
     *  \brief To show/close waiting dialog
     *  \iparam ShowFlag = true to show and flase to close the dialog
     */
    /****************************************************************************/
    void ShowWaitingMessage(bool ShowFlag=true)
    {
    }

    /****************************************************************************/
    /*!
     *  \brief To show the fail message dialog
     *  \iparam Error = the error type
     */
    /****************************************************************************/
    void ShowFailMessage(int ErrorCode)
    {
    }

};

class RetortPreUT : public CRetortPreTest
{
    Q_OBJECT
public:

    RetortPreUT()
        : CRetortPreTest(NULL)
    {
    }

    ~RetortPreUT()
    {
    }


private:
    /****************************************************************************/
    /*!
     *  \brief To show/close waiting dialog
     *  \iparam ShowFlag = true to show and flase to close the dialog
     */
    /****************************************************************************/
    void ShowWaitingMessage(bool ShowFlag=true)
    {
    }

    /****************************************************************************/
    /*!
     *  \brief To show the fail message dialog
     *  \iparam Error = the error type
     */
    /****************************************************************************/
    void ShowFailMessage(int ErrorCode)
    {
    }

};

class RVPreUT : public CRVPreTest
{
    Q_OBJECT
public:

    RVPreUT()
        : CRVPreTest(NULL)
    {
    }

    ~RVPreUT()
    {
    }


private:
    /****************************************************************************/
    /*!
     *  \brief To show/close waiting dialog
     *  \iparam ShowFlag = true to show and flase to close the dialog
     */
    /****************************************************************************/
    void ShowWaitingMessage(bool ShowFlag=true)
    {
    }

    /****************************************************************************/
    /*!
     *  \brief To show the fail message dialog
     *  \iparam Error = the error type
     */
    /****************************************************************************/
    void ShowFailMessage(int ErrorCode)
    {
    }

};


class ACVoltageUT : public CACVoltageTest
{
    Q_OBJECT
public:

    ACVoltageUT()
        : CACVoltageTest(NULL)
    {
    }

    ~ACVoltageUT()
    {
    }


private:
    /****************************************************************************/
    /*!
     *  \brief To show/close waiting dialog
     *  \iparam ShowFlag = true to show and flase to close the dialog
     */
    /****************************************************************************/
    void ShowWaitingMessage(bool ShowFlag=true)
    {
    }

    /****************************************************************************/
    /*!
     *  \brief To show the fail message dialog
     *  \iparam Error = the error type
     */
    /****************************************************************************/
    void ShowFailMessage(int ErrorCode)
    {
    }

    int GetCurrentSwitchType()
    {
        return 1;
    }

};

}

class CDiagnosticMessageDlgMock : public CDiagnosticMessageDlg
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam parent = Parent widget
     */
    /****************************************************************************/
    CDiagnosticMessageDlgMock(QWidget *p_Parent=NULL)
        :CDiagnosticMessageDlg(p_Parent)
    {
    }

    /****************************************************************************/
    /*!
     *  \brief To popup message dialog
     *  \iparam MessageTitle = the dialog title
     *  \iparam MessageText  = the dialog text
     *  \iparam Ret = test result for set dialog type
     */
    /****************************************************************************/
    void ShowMessage(QString& MessageTitle, QString& MessageText, ErrorCode_t Ret)
    {
    }

    /****************************************************************************/
    /*!
     *  \brief To popup waiting dialog
     *  \iparam MessageTitle = the dialog title
     *  \iparam MessageText  = the dialog text
     */
    /****************************************************************************/
    void ShowWaitingDialog(QString& MessageTitle, QString& MessageText)
    {
    }

    /****************************************************************************/
    /*!
     *  \brief To hide wait dialog
     */
    /****************************************************************************/
    void HideWaitingDialog()
    {
    }

    int ShowConfirmMessage(QString& MessageTitle, QString& MessageText, int type = YES_NO)
    {
        return YES;
    }
};

namespace Oven {

class CCoverSensorTestMock : public CCoverSensorTest
{
    Q_OBJECT

public:
    CCoverSensorTestMock(CDiagnosticMessageDlg *dlg)
        : CCoverSensorTest(dlg)
    {
    }

protected:
    virtual int CoverSensorStatusConfirmDlg(QString &title, QString &text, QString &value)
    {
        return CDiagnosticMessageDlg::YES;
    }
};

} // namespace Oven

/****************************************************************************/
/**
 * \brief Test class for Diagnostics class.
 */
/****************************************************************************/
class CTestDiagnostics : public QObject {
    Q_OBJECT
private slots:
    /****************************************************************************/
    /**
     * \brief Called before the first testfunction is executed.
     */
    /****************************************************************************/
    void initTestCase();
    /****************************************************************************/
    /**
     * \brief Called before each testfunction is executed.
     */
    /****************************************************************************/
    void init();
    /****************************************************************************/
    /**
     * \brief Called after each testfunction was executed.
     */
    /****************************************************************************/
    void cleanup();
    /****************************************************************************/
    /**
     * \brief Called after last testfunction was executed.
     */
    /****************************************************************************/
    void cleanupTestCase();

    /****************************************************************************/
    /**
     * \brief Test Diagnostics Menu object
     */
    /****************************************************************************/
    void utTestDiagnostics();

    void LTubePreTest();
    void MainsRelayTest();

    void RetortPreTest();
    void RVPreTest();
    void ACVoltageTest();
    void OvenPreTest();
    void CoverSensorTest();
    void RVMovementTest();


}; // end class CTestDiagnostics

/****************************************************************************/
void CTestDiagnostics::initTestCase() {
    Global::SystemPaths::Instance().SetSettingsPath("../../../Main/Build/Settings");
    Global::SystemPaths::Instance().SetTempPath("../../../Main/Build/Temporary");
    QString FileName = Global::SystemPaths::Instance().GetSettingsPath() + "/TestCaseConfigSVC.xml";
    DataManager::CTestCaseFactory::ServiceInstance().InitData(FileName);
}

/****************************************************************************/
void CTestDiagnostics::LTubePreTest()
{
    InitialSystem::LTubePreUT ut;
    QVERIFY(ut.Run() != 1);
}

/****************************************************************************/
void CTestDiagnostics::MainsRelayTest()
{
    InitialSystem::MainsRelayUT ut;
    QVERIFY(ut.Run() != 1);
}

/****************************************************************************/
void CTestDiagnostics::OvenPreTest()
{
    InitialSystem::OvenPreUT ut;
    QVERIFY(ut.Run() != 1);
}

/****************************************************************************/
void CTestDiagnostics::RetortPreTest()
{
    InitialSystem::RetortPreUT ut;
    QVERIFY(ut.Run() != 1);
}

/****************************************************************************/
void CTestDiagnostics::RVPreTest()
{
    InitialSystem::RVPreUT ut;
    QVERIFY(ut.Run() != 1);
}

/****************************************************************************/
void CTestDiagnostics::ACVoltageTest()
{
    InitialSystem::ACVoltageUT ut;
    QVERIFY(ut.Run() != 1);
}

/****************************************************************************/
void CTestDiagnostics::CoverSensorTest()
{
    CDiagnosticMessageDlgMock dlg;
    Oven::CCoverSensorTestMock ut(&dlg);
    QVERIFY(ut.Run() == RETURN_OK);
}

/****************************************************************************/
void CTestDiagnostics::RVMovementTest()
{
    CDiagnosticMessageDlgMock dlg;
    RotaryValve::CMovementTest ut(&dlg);
    QVERIFY(ut.Run() != RETURN_OK);
}


/****************************************************************************/
void CTestDiagnostics::init() {
}

/****************************************************************************/
void CTestDiagnostics::cleanup() {
}

/****************************************************************************/
void CTestDiagnostics::cleanupTestCase() {
}


//int CTestDiagnostics::eventLoop() {

//    QTimer timer;
//    qint32 ret;
//    quint32 interval = 1000 * 3 * 60 * 60; // for GUI test
//    timer.setSingleShot(true);
//    timer.setInterval(interval);
//    timer.start();
//    CONNECTSIGNALSLOT(&timer, timeout(), &m_LoopTest, quit());
//    ret = m_LoopTest.exec();
//}

/****************************************************************************/
void CTestDiagnostics::utTestDiagnostics() {
//    MainMenu::CMainWindow *p_MainWindow = new MainMenu::CMainWindow();

//    Diagnostics::CAgitator *p_Agitator = new Diagnostics::CAgitator(p_MainWindow);

//    Diagnostics::CAirExhaust *p_AirExhaust = new Diagnostics::CAirExhaust(p_MainWindow);

//    Diagnostics::CConsumables *p_Consumables = new Diagnostics::CConsumables(p_MainWindow);

//    Diagnostics::CCoverHood *p_Coverhood = new Diagnostics::CCoverHood(p_MainWindow);

//    Diagnostics::CDisplay *p_Display = new Diagnostics::CDisplay(p_MainWindow);

//    Diagnostics::CDrawerLoader *p_DrawerLoader = new Diagnostics::CDrawerLoader(p_MainWindow);

//    Diagnostics::CDrawerUnloader *p_DrawerUnloader = new Diagnostics::CDrawerUnloader(p_MainWindow);

//    Diagnostics::CHeatedCuvettes *p_HeatedCuvettes = new Diagnostics::CHeatedCuvettes(p_MainWindow);

//    Diagnostics::COven *p_Oven = new Diagnostics::COven(p_MainWindow);

//    Diagnostics::CSoftSwitch *p_Softswitch = new Diagnostics::CSoftSwitch(p_MainWindow);

//    Diagnostics::CWaterStation *p_WaterStation = new Diagnostics::CWaterStation(p_MainWindow);

//    Diagnostics::CXMovement *p_XAxis = new Diagnostics::CXMovement(p_MainWindow);

//    p_Agitator->ReferenceRun();
//    p_Agitator->TestAgitatorSpeed();

//    p_AirExhaust->AirFlowStatusDialog(QString("Air flow status dialog"), false);

//    p_Consumables->DisplayCANID();
//    p_Consumables->DisplayFirmwareInfo();
//    p_Consumables->KitInformation();
//    p_Consumables->BottleInformation();

//    p_Coverhood->DisplayStatus();
//    p_Coverhood->HoodStatusDialog("Cover Hood Status", true);

//     p_Display->DoBasicColorTest();
//     eventLoop();

//    p_DrawerLoader->AddItem("1", "Drawer loader reference run");
//    p_DrawerLoader->TestRun();

//    p_DrawerUnloader->AddItem("1", "Teach drawer");
//    p_DrawerUnloader->TestRun();

//    p_HeatedCuvettes->AddItem("2", "Maximum temperature test");
//    p_HeatedCuvettes->TestRun();

//    p_Oven->AddItem("2", "Photo Sensor test");
//    p_Oven->TestRun();

//    p_Softswitch->SoftSwitchTest();
//    p_Softswitch->LEDTest();

//    p_WaterStation->AddItem("1", "Water valves test");
//    p_WaterStation->WaterValvesTest();
//    p_WaterStation->DrainSensorTest();

//    p_XAxis->MoveYZArmLeft();
//    p_XAxis->MoveYZArmRight();
//    p_XAxis->XLeftReferenceRun();
//    p_XAxis->XRightReferenceRun();

}

} // end namespace Diagnostics

QTEST_MAIN(Diagnostics::CTestDiagnostics)
#include "TestDiagnostics.moc"

