/****************************************************************************/
/*! \file TestProcessSettings.cpp
 *
 *  \brief Unit test for Process settings Container
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-12-31
 *  $Author:    $ Stalin
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

#include <QTest>
#include <QDebug>
#include <QFile>
#include <QMap>
#include "DataManager/Containers/ProcessSettings/Include/ProcessSettings.h"
#include "DataManager/Containers/ProcessSettings/Include/ProcessSettingsVerifier.h"

namespace DataManager {

static QString FileNameWrite;
const QString XmlFileName = ":/Xml/ProcessSettings.xml";

/****************************************************************************/
/**
 * \brief Test class for Program sequence class.
 */
/****************************************************************************/
class TestProcessSettings : public QObject
{
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
     * \brief Test Adjustment object
     */
    /****************************************************************************/
    void utTestReadXml();
    void utTestLeftWriteXml();
    void utTestRightWriteXml();
    void utTestWriteXml();

};

/****************************************************************************/
void TestProcessSettings::initTestCase()
{
    FileNameWrite = QCoreApplication::applicationDirPath() + "/ProcessSettings.xml";
}

/****************************************************************************/
void TestProcessSettings::init()
{
}

/****************************************************************************/
void TestProcessSettings::cleanup()
{
}

/****************************************************************************/
void TestProcessSettings::cleanupTestCase()
{
}

/****************************************************************************/
void TestProcessSettings::utTestReadXml()
{
    CProcessSettings *p_ParameterSrc = new CProcessSettings();
    CProcessSettingsVerifier *p_Verifier = new CProcessSettingsVerifier();

    DeviceControl::Position_t PositionOld;
    DeviceControl::Position_t PositionNew;

    bool RetVal;

    // Add verifier before read
    p_ParameterSrc->AddVerifier(p_Verifier);

    if (p_ParameterSrc->Read(XmlFileName) == false)
    {
        QFAIL("Reading ProcessSettings.xml failed");
    }

    QCOMPARE(p_ParameterSrc->GetVersion(), 1);
    QCOMPARE(p_ParameterSrc->GetDataContainerType(), PARAMETER);

    RetVal = p_ParameterSrc->GetPosition("Loader", "Drawer_close", PositionOld);
    QCOMPARE(RetVal, true);

    PositionOld -= 10;
    RetVal = p_ParameterSrc->SetPosition("Loader", "Drawer_close", PositionOld);
    QCOMPARE(RetVal, true);

    RetVal = p_ParameterSrc->GetPosition("Loader", "Drawer_close", PositionNew);
    QCOMPARE(RetVal, true);
    QCOMPARE(PositionOld, PositionNew);

    QCOMPARE(p_ParameterSrc->Write(FileNameWrite), true);
}

void TestProcessSettings::utTestLeftWriteXml()
{
    CProcessSettings *p_ParameterSrc = new CProcessSettings();
    CProcessSettingsVerifier *p_Verifier = new CProcessSettingsVerifier();

    DeviceControl::Position_t Position =11;
    DeviceControl::Position_t PositionRead;
    // DeviceControl::Position_t PositionNew;
    quint8 SpeedProfile =1;
    quint8 SpeedProfileread;
    bool RetVal;

    // Add verifier before read
    p_ParameterSrc->AddVerifier(p_Verifier);

    if (p_ParameterSrc->Read(XmlFileName) == false)
    {
        QFAIL("Reading ProcessSettings.xml failed");
    }

    QCOMPARE(p_ParameterSrc->GetVersion(), 1);
    QCOMPARE(p_ParameterSrc->GetDataContainerType(), PARAMETER);

    RetVal = p_ParameterSrc->SetSpeedProfile("Agitation", "Agitation", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("Agitation", "Agitation", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    RetVal = p_ParameterSrc->SetSpeedProfile("LeftXyz", "X_Axis_move_with_rack", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("LeftXyz", "X_Axis_move_with_rack", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    SpeedProfile=2;
    RetVal = p_ParameterSrc->SetSpeedProfile("LeftXyz", "X_Axis_move_without_rack", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("LeftXyz", "X_Axis_move_without_rack", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    SpeedProfile=3;
    RetVal = p_ParameterSrc->SetSpeedProfile("LeftXyz", "Y_Axis_attach_detach_rack", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("LeftXyz", "Y_Axis_attach_detach_rack", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    SpeedProfile=4;
    RetVal = p_ParameterSrc->SetSpeedProfile("LeftXyz", "Y_Axis_move_with_rack", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("LeftXyz", "Y_Axis_move_with_rack", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    SpeedProfile=0;
    RetVal = p_ParameterSrc->SetSpeedProfile("LeftXyz", "Y_Axis_move_without_rack", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("LeftXyz", "Y_Axis_move_without_rack", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    SpeedProfile=1;
    RetVal = p_ParameterSrc->SetSpeedProfile("LeftXyz", "Z_Axis_move_down_with_rack", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("LeftXyz", "Z_Axis_move_down_with_rack", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    SpeedProfile=2;
    RetVal = p_ParameterSrc->SetSpeedProfile("LeftXyz", "Z_Axis_move_empty", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("LeftXyz", "Z_Axis_move_empty", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    SpeedProfile=3;
    RetVal = p_ParameterSrc->SetSpeedProfile("LeftXyz", "Z_Axis_move_up_fast_with_rack", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("LeftXyz", "Z_Axis_move_up_fast_with_rack", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    SpeedProfile=4;
    RetVal = p_ParameterSrc->SetSpeedProfile("LeftXyz", "Z_Axis_move_up_slow_with_rack", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("LeftXyz", "Z_Axis_move_up_slow_with_rack", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    Position = 22;
    RetVal = p_ParameterSrc->SetPosition("LeftXyz", "Y_Axis_offset_rack_attached", Position);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetPosition("LeftXyz", "Y_Axis_offset_rack_attached", PositionRead);
    QCOMPARE(RetVal, true);
    QCOMPARE(Position, PositionRead);
    RetVal = p_ParameterSrc->ResetPositionOffset("LeftXyz", "Y_Axis_offset_rack_attached", Position);
    QCOMPARE(RetVal, true);

    Position = 21;
    RetVal = p_ParameterSrc->SetPosition("LeftXyz", "Y_Axis_offset_slide_count", Position);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetPosition("LeftXyz", "Y_Axis_offset_slide_count", PositionRead);
    QCOMPARE(RetVal, true);
    QCOMPARE(Position, PositionRead);
    RetVal = p_ParameterSrc->ResetPositionOffset("LeftXyz", "Y_Axis_offset_slide_count", Position);
    QCOMPARE(RetVal, true);

    Position = 23;
    RetVal = p_ParameterSrc->SetPosition("LeftXyz", "Z_Axis_move_empty", Position);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetPosition("LeftXyz", "Z_Axis_move_empty", PositionRead);
    QCOMPARE(RetVal, true);
    QCOMPARE(Position, PositionRead);
    RetVal = p_ParameterSrc->ResetPositionOffset("LeftXyz", "Z_Axis_move_empty", Position);
    QCOMPARE(RetVal, true);

    Position = 32;
    RetVal = p_ParameterSrc->SetPosition("LeftXyz", "Z_Axis_move_up_fast_with_rack", Position);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetPosition("LeftXyz", "Z_Axis_move_up_fast_with_rack", PositionRead);
    QCOMPARE(RetVal, true);
    QCOMPARE(Position, PositionRead);
    RetVal = p_ParameterSrc->ResetPositionOffset("LeftXyz", "Z_Axis_move_up_fast_with_rack", Position);
    QCOMPARE(RetVal, true);

    Position = 31;
    RetVal = p_ParameterSrc->SetPosition("LeftXyz", "Z_Axis_move_up_slow_with_rack", Position);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetPosition("LeftXyz", "Z_Axis_move_up_slow_with_rack", PositionRead);
    QCOMPARE(RetVal, true);
    QCOMPARE(Position, PositionRead);
    RetVal = p_ParameterSrc->ResetPositionOffset("LeftXyz", "Z_Axis_move_up_slow_with_rack", Position);
    QCOMPARE(RetVal, true);

   
    RetVal = p_ParameterSrc->ResetPositionOffset("LeftXyz", "Z_Axis_move_up_slow_with_rack", Position);
    QCOMPARE(RetVal, true);
    QCOMPARE(p_Verifier->VerifyData(p_ParameterSrc),true);
}

void TestProcessSettings::utTestRightWriteXml()
{
    CProcessSettings *p_ParameterSrc = new CProcessSettings();
    CProcessSettingsVerifier *p_Verifier = new CProcessSettingsVerifier();

    DeviceControl::Position_t Position =11;
    DeviceControl::Position_t PositionRead;
    // DeviceControl::Position_t PositionNew;
    quint8 SpeedProfile =1;
    quint8 SpeedProfileread;
    bool RetVal;

    // Add verifier before read
    p_ParameterSrc->AddVerifier(p_Verifier);

    if (p_ParameterSrc->Read(XmlFileName) == false)
    {
        QFAIL("Reading ProcessSettings.xml failed");
    }

    QCOMPARE(p_ParameterSrc->GetVersion(), 1);
    QCOMPARE(p_ParameterSrc->GetDataContainerType(), PARAMETER);

    SpeedProfile = 0;
    RetVal = p_ParameterSrc->SetSpeedProfile("RightXyz", "X_Axis_move_with_rack", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("RightXyz", "X_Axis_move_with_rack", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    SpeedProfile = 1;
    RetVal = p_ParameterSrc->SetSpeedProfile("RightXyz", "X_Axis_move_without_rack", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("RightXyz", "X_Axis_move_without_rack", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    SpeedProfile = 2;
    RetVal = p_ParameterSrc->SetSpeedProfile("RightXyz", "Y_Axis_attach_detach_rack", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("RightXyz", "Y_Axis_attach_detach_rack", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    SpeedProfile = 3;
    RetVal = p_ParameterSrc->SetSpeedProfile("RightXyz", "Y_Axis_move_with_rack", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("RightXyz", "Y_Axis_move_with_rack", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    SpeedProfile = 4;
    RetVal = p_ParameterSrc->SetSpeedProfile("RightXyz", "Y_Axis_move_without_rack", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("RightXyz", "Y_Axis_move_without_rack", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    SpeedProfile = 0;
    RetVal = p_ParameterSrc->SetSpeedProfile("RightXyz", "Z_Axis_move_down_with_rack", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("RightXyz", "Z_Axis_move_down_with_rack", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);


    SpeedProfile = 1;
    RetVal = p_ParameterSrc->SetSpeedProfile("RightXyz", "Z_Axis_move_empty", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("RightXyz", "Z_Axis_move_empty", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    SpeedProfile = 2;
    RetVal = p_ParameterSrc->SetSpeedProfile("RightXyz", "Z_Axis_move_up_fast_with_rack", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("RightXyz", "Z_Axis_move_up_fast_with_rack", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    SpeedProfile = 3;
    RetVal = p_ParameterSrc->SetSpeedProfile("RightXyz", "Z_Axis_move_up_slow_with_rack", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("RightXyz", "Z_Axis_move_up_slow_with_rack", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    Position = 7;
    RetVal = p_ParameterSrc->SetPosition("RightXyz", "Y_Axis_offset_rack_attached", Position);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetPosition("RightXyz", "Y_Axis_offset_rack_attached", PositionRead);
    QCOMPARE(RetVal, true);
    QCOMPARE(Position, PositionRead);
    RetVal = p_ParameterSrc->ResetPositionOffset("RightXyz", "Y_Axis_offset_rack_attached", Position);
    QCOMPARE(RetVal, true);

    Position = 8;
    RetVal = p_ParameterSrc->SetPosition("RightXyz", "Y_Axis_offset_slide_count", Position);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetPosition("RightXyz", "Y_Axis_offset_slide_count", PositionRead);
    QCOMPARE(RetVal, true);
    QCOMPARE(Position, PositionRead);
    RetVal = p_ParameterSrc->ResetPositionOffset("RightXyz", "Y_Axis_offset_slide_count", Position);
    QCOMPARE(RetVal, true);

    Position = 9;
    RetVal = p_ParameterSrc->SetPosition("RightXyz", "Z_Axis_move_empty", Position);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetPosition("RightXyz", "Z_Axis_move_empty", PositionRead);
    QCOMPARE(RetVal, true);
    QCOMPARE(Position, PositionRead);
    RetVal = p_ParameterSrc->ResetPositionOffset("RightXyz", "Z_Axis_move_empty", Position);
    QCOMPARE(RetVal, true);

    Position = 10;
    RetVal = p_ParameterSrc->SetPosition("RightXyz", "Z_Axis_move_up_fast_with_rack", Position);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetPosition("RightXyz", "Z_Axis_move_up_fast_with_rack", PositionRead);
    QCOMPARE(RetVal, true);
    QCOMPARE(Position, PositionRead);
    RetVal = p_ParameterSrc->ResetPositionOffset("RightXyz", "Z_Axis_move_up_fast_with_rack", Position);
    QCOMPARE(RetVal, true);

    Position = 11;
    RetVal = p_ParameterSrc->SetPosition("RightXyz", "Z_Axis_move_up_slow_with_rack", Position);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetPosition("RightXyz", "Z_Axis_move_up_slow_with_rack", PositionRead);
    QCOMPARE(RetVal, true);
    QCOMPARE(Position, PositionRead);
    RetVal = p_ParameterSrc->ResetPositionOffset("RightXyz", "Z_Axis_move_up_slow_with_rack", Position);
    QCOMPARE(RetVal, true);

   }


void TestProcessSettings::utTestWriteXml()
{
    CProcessSettings *p_ParameterSrc = new CProcessSettings();
    CProcessSettingsVerifier *p_Verifier = new CProcessSettingsVerifier();

    DeviceControl::Position_t Position =11;
    DeviceControl::Position_t PositionRead;
    // DeviceControl::Position_t PositionNew;
    quint8 SpeedProfile =1;
    quint8 SpeedProfileread;
    bool RetVal;

    // Add verifier before read
    p_ParameterSrc->AddVerifier(p_Verifier);

    if (p_ParameterSrc->Read(XmlFileName) == false)
    {
        QFAIL("Reading ProcessSettings.xml failed");
    }

    QCOMPARE(p_ParameterSrc->GetVersion(), 1);
    QCOMPARE(p_ParameterSrc->GetDataContainerType(), PARAMETER);

    SpeedProfile = 1;
    RetVal = p_ParameterSrc->SetSpeedProfile("Loader", "Drawer_close", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("Loader", "Drawer_close", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    SpeedProfile = 2;
    RetVal = p_ParameterSrc->SetSpeedProfile("Loader", "Drawer_open", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("Loader", "Drawer_open", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    Position = 12;
    RetVal = p_ParameterSrc->SetPosition("Loader", "Drawer_close", Position);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetPosition("Loader", "Drawer_close", PositionRead);
    QCOMPARE(RetVal, true);
    QCOMPARE(Position, PositionRead);
    RetVal = p_ParameterSrc->ResetPositionOffset("Loader", "Drawer_close", Position);
    QCOMPARE(RetVal, true);

    Position = 12;
    RetVal = p_ParameterSrc->SetPosition("Loader", "Drawer_open", Position);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetPosition("Loader", "Drawer_open", PositionRead);
    QCOMPARE(RetVal, true);
    QCOMPARE(Position, PositionRead);
    RetVal = p_ParameterSrc->ResetPositionOffset("Loader", "Drawer_open", Position);
    QCOMPARE(RetVal, true);

    SpeedProfile = 1;
    RetVal = p_ParameterSrc->SetSpeedProfile("Unloader", "Drawer_close", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("Unloader", "Drawer_close", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    SpeedProfile = 2;
    RetVal = p_ParameterSrc->SetSpeedProfile("Unloader", "Drawer_open", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("Unloader", "Drawer_open", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);


    Position = 15;
    RetVal = p_ParameterSrc->SetPosition("Unloader", "Drawer_close", Position);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetPosition("Unloader", "Drawer_close", PositionRead);
    QCOMPARE(RetVal, true);
    QCOMPARE(Position, PositionRead);
    RetVal = p_ParameterSrc->ResetPositionOffset("Unloader", "Drawer_close", Position);
    QCOMPARE(RetVal, true);

    Position = 16;
    RetVal = p_ParameterSrc->SetPosition("Unloader", "Drawer_open", Position);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetPosition("Unloader", "Drawer_open", PositionRead);
    QCOMPARE(RetVal, true);
    QCOMPARE(Position, PositionRead);
    RetVal = p_ParameterSrc->ResetPositionOffset("Unloader", "Drawer_open", Position);
    QCOMPARE(RetVal, true);

    SpeedProfile = 3;
    RetVal = p_ParameterSrc->SetSpeedProfile("Oven", "Cover_close", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("Oven", "Cover_close", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    SpeedProfile = 4;
    RetVal = p_ParameterSrc->SetSpeedProfile("Oven", "Cover_open", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("Oven", "Cover_open", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    Position = 17;
    RetVal = p_ParameterSrc->SetPosition("Oven", "Cover_close", Position);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetPosition("Oven", "Cover_close", PositionRead);
    QCOMPARE(RetVal, true);
    QCOMPARE(Position, PositionRead);
    RetVal = p_ParameterSrc->ResetPositionOffset("Oven", "Cover_close", Position);
    QCOMPARE(RetVal, true);

    Position = 18;
    RetVal = p_ParameterSrc->SetPosition("Oven", "Cover_open", Position);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetPosition("Oven", "Cover_open", PositionRead);
    QCOMPARE(RetVal, true);
    QCOMPARE(Position, PositionRead);
    RetVal = p_ParameterSrc->ResetPositionOffset("Oven", "Cover_open", Position);
    QCOMPARE(RetVal, true);

    SpeedProfile = 1;
    RetVal = p_ParameterSrc->SetSpeedProfile("RackTransfer", "RackTransfer", SpeedProfile);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetSpeedProfile("RackTransfer", "RackTransfer", SpeedProfileread);
    QCOMPARE(RetVal, true);
    QCOMPARE(SpeedProfile, SpeedProfileread);

    Position = 19;
    RetVal = p_ParameterSrc->SetPosition("RackTransfer", "Load_position", Position);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetPosition("RackTransfer", "Load_position", PositionRead);
    QCOMPARE(RetVal, true);
    QCOMPARE(Position, PositionRead);
    RetVal = p_ParameterSrc->ResetPositionOffset("RackTransfer", "Load_position", Position);
    QCOMPARE(RetVal, true);

    Position = 20;
    RetVal = p_ParameterSrc->SetPosition("RackTransfer", "Transfer_position", Position);
    QCOMPARE(RetVal, true);
    RetVal = p_ParameterSrc->GetPosition("RackTransfer", "Transfer_position", PositionRead);
    QCOMPARE(RetVal, true);
    QCOMPARE(Position, PositionRead);
    RetVal = p_ParameterSrc->ResetPositionOffset("RackTransfer", "Transfer_position", Position);
    QCOMPARE(RetVal, true);
    QCOMPARE(p_Verifier->VerifyData(p_ParameterSrc),true);

}

} // end namespace DataManager

QTEST_MAIN(DataManager::TestProcessSettings)

#include "TestProcessSettings.moc"
