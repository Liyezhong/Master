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
#include "Diagnostics/Include/System/AlarmTest.h"
#include "Diagnostics/Include/System/ExhaustFanTest.h"
#include "Diagnostics/Include/System/SpeakerTest.h"
#include "Diagnostics/Include/System/VentilationFanTest.h"
#include "Diagnostics/Include/System/FillingNDrainingTest.h"
#include "Diagnostics/Include/System/SystemSealingTest.h"
#include "Diagnostics/Include/System/LiquidHoseTest.h"
#include "Diagnostics/Include/Retort/LevelSensorDetectingTest.h"
#include "Diagnostics/Include/Retort/LidLockTest.h"
#include "Diagnostics/Include/MainControl/ASBTest.h"
#include "Diagnostics/Include/DiagnosticMessageDlg.h"
#include "Diagnostics/Include/LaSystem/AirHeatingTubeTest.h"
#include "Diagnostics/Include/LaSystem/LiquidHeatingTubeTest.h"
#include "Diagnostics/Include/Retort/RetortHeatingTestEmpty.h"
#include "Diagnostics/Include/Retort/RetortHeatingTestWithWater.h"
#include "Diagnostics/Include/ServiceDeviceProcess/ServiceDeviceProcess.h"
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
    void ShowMessage(const QString& MessageTitle, const QString& MessageText, ErrorCode_t Ret)
    {
    }

    /****************************************************************************/
    /*!
     *  \brief To popup waiting dialog
     *  \iparam MessageTitle = the dialog title
     *  \iparam MessageText  = the dialog text
     */
    /****************************************************************************/
    void ShowWaitingDialog(const QString& MessageTitle, const QString& MessageText)
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

    int ShowConfirmMessage(const QString& MessageTitle, const QString& MessageText, BUTTON_TYPE type = YES_NO)
    {
        return YES;
    }

    void ShowRVMoveFailedDlg(const QString& Title)
    {
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

class CHeatingTestEmptyMock : public CHeatingTestEmpty
{
    Q_OBJECT

public:
    CHeatingTestEmptyMock(CDiagnosticMessageDlg *dlg)
        : CHeatingTestEmpty(dlg)
    {
    }

protected:
    virtual void ShowWaitingDialog(struct heatingTestStatus *buf)
    {
    }
};

} // namespace Oven

namespace System {

class CSpeakerTestMock : public CSpeakerTest
{
    Q_OBJECT
public:
    CSpeakerTestMock(CDiagnosticMessageDlg* dlg)
        : CSpeakerTest(dlg)
    {

    }
protected:
    virtual void PlaySound(QString &Volume)
    {
    }
};

}// namespace System

namespace MainControl {

class CASBTestMock : public CASBTest
{
    Q_OBJECT
public:
    CASBTestMock(HimSlaveType_t SlaveType)
        : CASBTest(SlaveType)
    {

    }
protected:
    virtual void ShowMessage(QString &MessageTitle, QString &MessageText, ErrorCode_t Ret)
    {

    }
};
}

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

    void LTubePreTest();
    void MainsRelayTest();

    void RetortPreTest();
    void RVPreTest();
    void ACVoltageTest();
    void OvenPreTest();
    void CoverSensorTest();
    void RVMovementTest();
    void HeatingTestEmpty();

    void SystemAlarmTest();
    void SystemExhaustFanTest();
    void SystemVentilationFanTest();
    void SystemSpeakerTest();
    void SystemFillingNDrainingTest();
    void SystemSealingTest();
    void SystemLiquidHoseTest();

    void RetortLidLockTest();
    void RetortLevelSensorDetectionTest();
    void RetortHeatingTestEmpty();
    void RetortHeatingTestEmptyWithWater();

    void LAAirHeatingTubeTest();
    void LALiquidHeatingTubeTest();


    void MainControlASBTest();

    void ServiceDeviceProcessTest();


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
    //QVERIFY(ut.Run() != 1);
}

/****************************************************************************/
void CTestDiagnostics::MainsRelayTest()
{
    InitialSystem::MainsRelayUT ut;
    //QVERIFY(ut.Run() != 1);
}

/****************************************************************************/
void CTestDiagnostics::OvenPreTest()
{
    InitialSystem::OvenPreUT ut;
    //QVERIFY(ut.Run() != 1);
}

/****************************************************************************/
void CTestDiagnostics::RetortPreTest()
{
    InitialSystem::RetortPreUT ut;
    //QVERIFY(ut.Run() != 1);
}

/****************************************************************************/
void CTestDiagnostics::RVPreTest()
{
    InitialSystem::RVPreUT ut;
    //QVERIFY(ut.Run() != 1);
}

/****************************************************************************/
void CTestDiagnostics::ACVoltageTest()
{
    InitialSystem::ACVoltageUT ut;
    //QVERIFY(ut.Run() != 1);
}

/****************************************************************************/
void CTestDiagnostics::CoverSensorTest()
{
    CDiagnosticMessageDlgMock dlg;
    Oven::CCoverSensorTestMock ut(&dlg);
    ////QVERIFY(ut.Run() == RETURN_OK);
}

/****************************************************************************/
void CTestDiagnostics::RVMovementTest()
{
    CDiagnosticMessageDlgMock dlg;
    RotaryValve::CMovementTest ut(&dlg);
    //QVERIFY(ut.Run() != RETURN_OK);
}

/****************************************************************************/
void CTestDiagnostics::HeatingTestEmpty()
{
    CDiagnosticMessageDlgMock dlg;
    Oven::CHeatingTestEmpty ut(&dlg);
    //QVERIFY(ut.Run() != RETURN_OK);
}


/****************************************************************************/
void CTestDiagnostics::SystemAlarmTest()
{
    CDiagnosticMessageDlgMock dlg;
    System::CAlarmTest utRemote("Remote", &dlg);
    System::CAlarmTest utLocal("Local", &dlg);
//    QVERIFY(utRemote.Run() != RETURN_OK);
//    QVERIFY(utLocal.Run() != RETURN_OK);
}

/****************************************************************************/
void CTestDiagnostics::SystemExhaustFanTest()
{
    CDiagnosticMessageDlgMock dlg;
    System::CExhaustFanTest ut(&dlg);
//    QVERIFY(ut.Run() == RETURN_OK);
}

/****************************************************************************/
void CTestDiagnostics::SystemVentilationFanTest()
{
    CDiagnosticMessageDlgMock dlg;
    System::CVentilationFanTest ut(&dlg);
//    QVERIFY(ut.Run() == RETURN_OK);
}

/****************************************************************************/
void CTestDiagnostics::SystemSpeakerTest()
{
    CDiagnosticMessageDlgMock dlg;
    System::CSpeakerTestMock ut(&dlg);
//    QVERIFY(ut.Run() == RETURN_OK);
}

/****************************************************************************/
void CTestDiagnostics::SystemFillingNDrainingTest()
{
    CDiagnosticMessageDlgMock dlg;
    System::CFillingNDrainingTest ut(&dlg);
//    QVERIFY(ut.Run() != RETURN_OK);
}

/****************************************************************************/
void CTestDiagnostics::SystemSealingTest()
{
    CDiagnosticMessageDlgMock dlg;
    System::CSystemSealingTest ut(&dlg);

//    QVERIFY(ut.Run() != RETURN_OK);
}

/****************************************************************************/
void CTestDiagnostics::SystemLiquidHoseTest()
{
    CDiagnosticMessageDlgMock dlg;
    System::CLiquidHoseTest ut(&dlg);
    //QVERIFY(ut.Run() == RETURN_OK);
}

/****************************************************************************/
void CTestDiagnostics::RetortLevelSensorDetectionTest()
{
    CDiagnosticMessageDlgMock dlg;
    Retort::CLevelSensorDetectingTest ut(&dlg);
    //QVERIFY(ut.Run() == RETURN_OK);
}

/****************************************************************************/
void CTestDiagnostics::RetortLidLockTest()
{
    CDiagnosticMessageDlgMock dlg;
    Retort::CLidLockTest ut(&dlg);
//    //QVERIFY(ut.Run() == RETURN_OK);
}

void CTestDiagnostics::RetortHeatingTestEmpty()
{
    CDiagnosticMessageDlgMock dlg;
    Retort::CHeatingTestEmpty ut(&dlg);
//    QVERIFY(ut.Run() != RETURN_OK);
}

/****************************************************************************/
void CTestDiagnostics::MainControlASBTest()
{
    MainControl::CASBTestMock ASB3Test(Slave_3);
    MainControl::CASBTestMock ASB5Test(Slave_5);
    MainControl::CASBTestMock ASB15Test(Slave_15);
//    ASB3Test.Run();
//    ASB5Test.Run();
//    ASB15Test.Run();
}

void CTestDiagnostics::RetortHeatingTestEmptyWithWater()
{
    CDiagnosticMessageDlgMock dlg;
    Retort::CHeatingTestWithWater ut(&dlg);
//    QVERIFY(ut.Run() != RETURN_OK);
}


void CTestDiagnostics::LALiquidHeatingTubeTest()
{
    CDiagnosticMessageDlgMock dlg;
    LaSystem::CLiquidHeatingTubeTest ut(&dlg);
    //QVERIFY(ut.Run() == RETURN_OK);
}

void CTestDiagnostics::LAAirHeatingTubeTest()
{
    CDiagnosticMessageDlgMock dlg;
    LaSystem::CAirHeatingTubeTest ut(&dlg);
    //QVERIFY(ut.Run() == RETURN_OK);
}

void CTestDiagnostics::ServiceDeviceProcessTest()
{
    Diagnostics::ServiceDeviceProcess* p_Dev = Diagnostics::ServiceDeviceProcess::Instance();
    qreal OutReal;
    int OutInt;
    quint8 OutUint8;
    quint16 OutUint16;
    qint32 OutInt32;
    bool OutBool;
    float OutFloat;

    QVERIFY(p_Dev->OvenStartHeating(20, 30) == 0);
    QVERIFY(p_Dev->OvenStopHeating() == 0);
    QVERIFY(p_Dev->OvenGetCoverSensorState(&OutInt32) == 0);
    QVERIFY(p_Dev->OvenGetCurrent(&OutUint16, &OutUint16) == 0);
    QVERIFY(p_Dev->OvenGetSwitchType(&OutInt) == 0);
    QVERIFY(p_Dev->OvenGetTemp(&OutReal, &OutReal, &OutReal) == 0);
    QVERIFY(p_Dev->OvenTempControlIsOn(&OutBool) == 0);

//    QVERIFY(p_Dev->RetortStartHeating(20, 30) == 0);
//    QVERIFY(p_Dev->RetortStopHeating() == 0);
//    QVERIFY(p_Dev->RetortGetCurrent(&OutUint16, &OutUint16) == 0);
//    QVERIFY(p_Dev->RetortGetHeaterSwitchType(&OutUint8) == 0);
//    QVERIFY(p_Dev->RetortGetLidLockState(&OutInt32) == 0);
//    QVERIFY(p_Dev->RetortGetTemp(&OutReal, &OutReal, &OutReal) == 0);
//    QVERIFY(p_Dev->RetortSetTemperatureSwitchState(1, 1) == 0);
//    QVERIFY(p_Dev->RetortTempControlIsOn(&OutBool) == 0);

//    QVERIFY(p_Dev->AirTubeGetCurrent(&OutUint16) == 0);
//    QVERIFY(p_Dev->AirTubeGetTemp(&OutReal) == 0);
//    QVERIFY(p_Dev->AirTubeStartHeating(30) == 0);
//    //QVERIFY(p_Dev->AirTubeStopHeating() == 0);
//    QVERIFY(p_Dev->AirTubeTempControlIsOn(&OutBool) == 0);

//    //QVERIFY(p_Dev->AlarmGetState(1, &OutInt32) == 0);
//    //QVERIFY(p_Dev->AlarmSetOnOff(1, false) == 0);

//    QVERIFY(p_Dev->LiquidTubeGetCurrent(&OutUint16) == 0);
//    QVERIFY(p_Dev->LiquidTubeGetTemp(&OutReal) == 0);
//    QVERIFY(p_Dev->LiquidTubeStartHeating(30) == 0);
////    QVERIFY(p_Dev->LiquidTubeStopHeating() == 0);
//    QVERIFY(p_Dev->LiquidTubeTempControlIsOn(&OutBool) == 0);

//    QVERIFY(p_Dev->RVGetCurrent(&OutUint16) == 0);
//    QVERIFY(p_Dev->RVGetHeaterSwitchType(&OutUint8) == 0);
//    QVERIFY(p_Dev->RVGetPosition(&OutBool, &OutInt32) == 0);
//    QVERIFY(p_Dev->RVGetTemp(&OutReal, &OutReal) == 0);
//    QVERIFY(p_Dev->RVInitialize() == 0);
//    QVERIFY(p_Dev->RVMovePosition(false, 6) == 0);
//    QVERIFY(p_Dev->RVSetTemperatureSwitchState(1, 0) == 0);
//    QVERIFY(p_Dev->RVStartHeating(80) == 0);
//    QVERIFY(p_Dev->RVStopHeating() == 0);
//    QVERIFY(p_Dev->RVTempControlIsOn(&OutBool) == 0);

//    QVERIFY(p_Dev->PumpBuildPressure(30) == 0);
//    QVERIFY(p_Dev->PumpDraining() == 0);
//    QVERIFY(p_Dev->PumpGetPressure(&OutFloat) == 0);
//    //QVERIFY(p_Dev->PumpReadPressureDrift(&OutFloat) != RETURN_OK);
//    QVERIFY(p_Dev->PumpReleasePressure() == 0);
//    QVERIFY(p_Dev->PumpSetFan(1) == 0);
//    QVERIFY(p_Dev->PumpSetPressure(0, 30) == 0);
//    QVERIFY(p_Dev->PumpSetValve(0, 0) == 0);
//    QVERIFY(p_Dev->PumpStopCompressor() == 0);
//    QVERIFY(p_Dev->PumpSucking() == 0);
    //QVERIFY(p_Dev->PumpWritePressureDrift(3.6) != RETURN_OK);

    QVERIFY(p_Dev->PumpGetFan() == false);
    QVERIFY(p_Dev->PumpGetStatus() == false);
//    p_Dev->PumpGetValve(0, OutUint8);
//    QVERIFY(OutUint8 == false);
//    p_Dev->PumpGetValve(1, OutUint8);
//    QVERIFY(OutUint8 == false);
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

} // end namespace Diagnostics

QTEST_MAIN(Diagnostics::CTestDiagnostics)
#include "TestDiagnostics.moc"

