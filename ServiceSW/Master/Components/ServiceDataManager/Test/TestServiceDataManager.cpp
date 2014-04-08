    /****************************************************************************/
/*! \file TestServiceDataManager.cpp
 *
 *  \brief Implementation file for class TestServiceDataManager.
 *
 *  \b Description:
 *         Checks the CTestServiceDataManager class implementation
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-10-25
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

#include <QTest>
#include <QFile>
#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include "Global/Include/SystemPaths.h"
//#include "ServiceDataManager/Include/InstrumentHistoryArchive.h"
//#include "ServiceDataManager/Include/ModuleDataListArchive.h"
#include "ServiceDataManager/Include/ServiceParameters.h"
#include "ServiceDataManager/Include/ServiceParametersVerifier.h"

namespace DataManager {

/****************************************************************************/
/**
 * \brief Test class for TestServiceDataManager class.
 */
/****************************************************************************/
class CTestServiceDataManager : public QObject {
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
     * \brief Test write and read Instrument History Archive data container
     */
    /****************************************************************************/
    void TestInstrumentHistoryArchive();

    /****************************************************************************/
    /**
     * \brief Test write and read Module data list data container
     */
    /****************************************************************************/
    void TestModuleDataListArchive();

    /****************************************************************************/
    /**
     * \brief Test write and read service parameters data container
     */
    /****************************************************************************/
    void TestServiceParameters();

    /****************************************************************************/
    /**
     * \brief Test write and read service parameters verifier
     */
    /****************************************************************************/
    void TestServiceParametersVerifier();


}; // end class CTestServiceDataManager

/****************************************************************************/
void CTestServiceDataManager::initTestCase() {

}

/****************************************************************************/
void CTestServiceDataManager::init() {
}

/****************************************************************************/
void CTestServiceDataManager::cleanup() {
}

/****************************************************************************/
void CTestServiceDataManager::cleanupTestCase() {
}

/****************************************************************************/
void CTestServiceDataManager::TestInstrumentHistoryArchive() {

//    Global::SystemPaths::Instance().SetSettingsPath("../Settings");
//    DataManager::CInstrumentHistoryArchive *p_InstrumentHistoryArchive =
//            new DataManager::CInstrumentHistoryArchive();

//    QCOMPARE(p_InstrumentHistoryArchive->GetDataContainerType(), INSTRUMENTHISTORY);
//    p_InstrumentHistoryArchive->SetInstrumentName("ST8200");
//    QCOMPARE(p_InstrumentHistoryArchive->GetInstrumentName(), QString("ST8200"));
//    QCOMPARE(p_InstrumentHistoryArchive->GetFilename(), QString(""));

//    p_InstrumentHistoryArchive->SetDataVerificationMode(false);
//    QCOMPARE(p_InstrumentHistoryArchive->GetDataVerificationMode(), false);

//    QString FilePath = Global::SystemPaths::Instance().GetSettingsPath() + QDir::separator() + "InstrumentHistoryArchive.xml";
//    p_InstrumentHistoryArchive->SetFilename(FilePath);
//    QCOMPARE(p_InstrumentHistoryArchive->ReadFile(p_InstrumentHistoryArchive->GetFilename()), true);

//    DataManager::CInstrumentHistoryArchive *p_InstrumentHistoryCopy = new DataManager::CInstrumentHistoryArchive(*p_InstrumentHistoryArchive);
//    p_InstrumentHistoryCopy->SetDataVerificationMode(true);
//    p_InstrumentHistoryCopy->SetFilename(FilePath);

//    QFile FileOpen(p_InstrumentHistoryCopy->GetFilename());
//    p_InstrumentHistoryCopy->DeserializeContent(FileOpen, true);

//    QString WriteFilePath = Global::SystemPaths::Instance().GetSettingsPath() + QDir::separator() + "InstrumentHistoryArchiveOutput.xml";
//    p_InstrumentHistoryCopy->SetFilename(WriteFilePath);
//    QFile OutFileOpen(p_InstrumentHistoryCopy->GetFilename());
//    p_InstrumentHistoryCopy->SerializeContent(OutFileOpen, true);

//    QString ArchiveFilename = Global::SystemPaths::Instance().GetSettingsPath() + QDir::separator() + "InstrumentHistoryArchiveWrite.xml";
//    p_InstrumentHistoryCopy->WriteArchiveFile(ArchiveFilename);

//    delete p_InstrumentHistoryCopy;
//    delete p_InstrumentHistoryArchive;

}

/****************************************************************************/
void CTestServiceDataManager::TestModuleDataListArchive() {
//    DataManager::CModuleDataListArchive *p_ModuleDataListArchive =
//            new DataManager::CModuleDataListArchive();

//    p_ModuleDataListArchive->SetModuleTimeStamp("2013-09-18 06:11:06.607");
//    QCOMPARE(p_ModuleDataListArchive->GetModuleTimeStamp(), QString("2013-09-18 06:11:06.607"));

//    DataManager::CModuleDataListArchive *p_ModuleDataList =
//            new DataManager::CModuleDataListArchive("2013-09-18 06:11:06.607");

//    DataManager::CModuleDataListArchive *p_ModuleDataListCopy =
//            new DataManager::CModuleDataListArchive(*p_ModuleDataList);


//    CModule *p_Module1 = new CModule();

//    p_Module1->SetModuleName(QString(tr("Drawer Left")));
//    p_Module1->SetModuleDescription(QString(tr("Loader")));
//    p_Module1->SetSerialNumber(QString(tr("SN_LOADER_00001")));
//    p_Module1->SetOperatingHours(QString(tr("Hrs")));
//    p_Module1->SetCalibrationDate(QString(tr("DATE2")));
//    p_Module1->SetCalibrationResult(QString(tr("PASS")));
//    p_Module1->SetTestDate(QString(tr("DATE2")));
//    p_Module1->SetTestResult(QString(tr("PASS")));

//    CSubModule *p_SubModule1 = new CSubModule();
//    p_SubModule1->SetSubModuleName("asb4_0");
//    p_SubModule1->SetSubModuleType("Board");
//    p_SubModule1->SetSubModuleDescription("DisplayName");
//    p_Module1->AddSubModuleInfo(p_SubModule1);

//    CSubModule *p_SubModule2 = new CSubModule();
//    p_SubModule2->SetSubModuleName("asb5_0");
//    p_SubModule2->SetSubModuleType("Board");
//    p_SubModule2->SetSubModuleDescription("DisplayName");
//    p_Module1->AddSubModuleInfo(p_SubModule2);

//    p_ModuleDataListCopy->AddModule(p_Module1);
//    CModule Module;
//    QCOMPARE(p_ModuleDataListCopy->GetModule("Drawer Left", Module), true);

//    CModule *p_TempModule = new CModule();
//    QByteArray *p_TempByteArray = new QByteArray();
//    QDataStream DataStream(p_TempByteArray, QIODevice::ReadWrite);
//    DataStream.setVersion(QDataStream::Qt_4_0);
//    p_TempByteArray->clear();
//    DataStream << *p_Module1;
//    DataStream.device()->reset();
//    DataStream >> *p_TempModule;
//    QCOMPARE(p_TempModule->GetNumberofSubModules(), p_Module1->GetNumberofSubModules());

//    delete p_TempModule;
//    delete p_SubModule1;
//    delete p_SubModule2;
//    delete p_Module1;
//    delete p_ModuleDataListCopy;
//    delete p_ModuleDataList;
//    delete p_ModuleDataListArchive;

}

/****************************************************************************/
void CTestServiceDataManager::TestServiceParameters() {

    Global::SystemPaths::Instance().SetSettingsPath("../Settings");
    DataManager::CServiceParameters *p_ServiceParameters =
            new DataManager::CServiceParameters();

    p_ServiceParameters->SetProxyUserName("himalaya");
    QCOMPARE(p_ServiceParameters->GetProxyUserName(), QString("himalaya"));

    p_ServiceParameters->SetProxyPassword("himalaya");
    QCOMPARE(p_ServiceParameters->GetProxyPassword(), QString("himalaya"));

    p_ServiceParameters->SetProxyIPAddress("123.234.123.234");
    QCOMPARE(p_ServiceParameters->GetProxyIPAddress(), QString("123.234.123.234"));

    p_ServiceParameters->SetProxyIPPort(1234);
    QCOMPARE(p_ServiceParameters->GetProxyIPPort(), 1234);

    p_ServiceParameters->SetVerion(1);
    QCOMPARE(p_ServiceParameters->GetVersion(), 1);

    LoaderRFID_t LoaderValue;
    LoaderValue.Rack1 = QString("0xAB12345CADFE1234");
    LoaderValue.Rack2 = QString("0xAB12345CADFECEAD");
    LoaderValue.Rack3 = QString("0xAB12345CADFE9634");
    LoaderValue.Rack4 = QString("0xAB12345CADFE1324");
    LoaderValue.Rack5 = QString("0xAB12345CADFEABCD");

    p_ServiceParameters->SetLoaderRFID(LoaderValue);
    QCOMPARE(p_ServiceParameters->GetLoaderRFID()->Rack1, QString("0xAB12345CADFE1234"));
    QCOMPARE(p_ServiceParameters->GetLoaderRFID()->Rack2, QString("0xAB12345CADFECEAD"));
    QCOMPARE(p_ServiceParameters->GetLoaderRFID()->Rack3, QString("0xAB12345CADFE9634"));
    QCOMPARE(p_ServiceParameters->GetLoaderRFID()->Rack4, QString("0xAB12345CADFE1324"));
    QCOMPARE(p_ServiceParameters->GetLoaderRFID()->Rack5, QString("0xAB12345CADFEABCD"));

    UnloaderRFID_t UnloaderValue;
    UnloaderValue.Rack1 = QString("0xAB12345CADFE1234");
    UnloaderValue.Rack2 = QString("0xAB12345CADFECEAD");
    UnloaderValue.Rack3 = QString("0xAB12345CADFE9634");
    UnloaderValue.Rack4 = QString("0xAB12345CADFE1324");
    UnloaderValue.Rack5 = QString("0xAB12345CADFEABCD");

    p_ServiceParameters->SetUnloaderRFID(UnloaderValue);
    QCOMPARE(p_ServiceParameters->GetUnloaderRFID()->Rack1, QString("0xAB12345CADFE1234"));
    QCOMPARE(p_ServiceParameters->GetUnloaderRFID()->Rack2, QString("0xAB12345CADFECEAD"));
    QCOMPARE(p_ServiceParameters->GetUnloaderRFID()->Rack3, QString("0xAB12345CADFE9634"));
    QCOMPARE(p_ServiceParameters->GetUnloaderRFID()->Rack4, QString("0xAB12345CADFE1324"));
    QCOMPARE(p_ServiceParameters->GetUnloaderRFID()->Rack5, QString("0xAB12345CADFEABCD"));

    p_ServiceParameters->SetYZModuleRFID("0xAB12345CADFEQPWE");
    QCOMPARE(p_ServiceParameters->GetYZModuleRFID(), QString("0xAB12345CADFEQPWE"));


    XArmLeftParameters_t XArmLeftParameters;
    XArmLeftParameters.Filledx = QString("5");
    XArmLeftParameters.Filledy = QString("2");
    XArmLeftParameters.Emptyx = QString("4");
    XArmLeftParameters.Emptyy = QString("5");
    XArmLeftParameters.Rackx = QString("8");
    XArmLeftParameters.Racky = QString("6");

    p_ServiceParameters->SetXArmLeftParamters(XArmLeftParameters);
    QCOMPARE(p_ServiceParameters->GetXArmLeftParameters()->Filledx, QString("5"));
    QCOMPARE(p_ServiceParameters->GetXArmLeftParameters()->Filledy, QString("2"));
    QCOMPARE(p_ServiceParameters->GetXArmLeftParameters()->Emptyx, QString("4"));
    QCOMPARE(p_ServiceParameters->GetXArmLeftParameters()->Emptyy, QString("5"));
    QCOMPARE(p_ServiceParameters->GetXArmLeftParameters()->Rackx, QString("8"));
    QCOMPARE(p_ServiceParameters->GetXArmLeftParameters()->Racky, QString("6"));

    XArmRightParameters_t XArmRightParameters;
    XArmRightParameters.Filledx = QString("5");
    XArmRightParameters.Filledy = QString("2");
    XArmRightParameters.Emptyx = QString("4");
    XArmRightParameters.Emptyy = QString("5");
    XArmRightParameters.Rackx = QString("8");
    XArmRightParameters.Racky = QString("6");

    p_ServiceParameters->SetXArmRightParamters(XArmRightParameters);
    QCOMPARE(p_ServiceParameters->GetXArmRightParameters()->Filledx, QString("5"));
    QCOMPARE(p_ServiceParameters->GetXArmRightParameters()->Filledy, QString("2"));
    QCOMPARE(p_ServiceParameters->GetXArmRightParameters()->Emptyx, QString("4"));
    QCOMPARE(p_ServiceParameters->GetXArmRightParameters()->Emptyy, QString("5"));
    QCOMPARE(p_ServiceParameters->GetXArmRightParameters()->Rackx, QString("8"));
    QCOMPARE(p_ServiceParameters->GetXArmRightParameters()->Racky, QString("6"));

    OvenParameters_t OvenParameters;
    OvenParameters.MaxDiff = QString("3");
    OvenParameters.RoomTempLow = QString("18");
    OvenParameters.RoomTempHigh = QString("30");
    OvenParameters.HTime1 = QString("1");
    OvenParameters.HTime2 = QString("2");
    OvenParameters.TempDiff = QString("15");
    OvenParameters.OvenMaxLow = QString("40");
    OvenParameters.OvenMaxHigh = QString("70");

    p_ServiceParameters->SetOvenParamters(OvenParameters);
    QCOMPARE(p_ServiceParameters->GetOvenParameters()->MaxDiff, QString("3"));
    QCOMPARE(p_ServiceParameters->GetOvenParameters()->RoomTempLow, QString("18"));
    QCOMPARE(p_ServiceParameters->GetOvenParameters()->RoomTempHigh, QString("30"));
    QCOMPARE(p_ServiceParameters->GetOvenParameters()->HTime1, QString("1"));
    QCOMPARE(p_ServiceParameters->GetOvenParameters()->HTime2, QString("2"));
    QCOMPARE(p_ServiceParameters->GetOvenParameters()->TempDiff, QString("15"));
    QCOMPARE(p_ServiceParameters->GetOvenParameters()->OvenMaxLow, QString("40"));
    QCOMPARE(p_ServiceParameters->GetOvenParameters()->OvenMaxHigh, QString("70"));

    HeatedCuvetteParameters_t HCParameters;
    HCParameters.MaxDiff = QString("3");
    HCParameters.RoomTempLow = QString("18");
    HCParameters.RoomTempHigh = QString("30");
    HCParameters.HTime1 = QString("1");
    HCParameters.HTime2 = QString("2");
    HCParameters.TempDiff = QString("15");
    HCParameters.OvenMaxLow = QString("40");
    HCParameters.OvenMaxHigh = QString("70");

    p_ServiceParameters->SetHeatedCuvetteParamters(HCParameters);
    QCOMPARE(p_ServiceParameters->GetHeatedCuvetteParameters()->MaxDiff, QString("3"));
    QCOMPARE(p_ServiceParameters->GetHeatedCuvetteParameters()->RoomTempLow, QString("18"));
    QCOMPARE(p_ServiceParameters->GetHeatedCuvetteParameters()->RoomTempHigh, QString("30"));
    QCOMPARE(p_ServiceParameters->GetHeatedCuvetteParameters()->HTime1, QString("1"));
    QCOMPARE(p_ServiceParameters->GetHeatedCuvetteParameters()->HTime2, QString("2"));
    QCOMPARE(p_ServiceParameters->GetHeatedCuvetteParameters()->TempDiff, QString("15"));
    QCOMPARE(p_ServiceParameters->GetHeatedCuvetteParameters()->OvenMaxLow, QString("40"));
    QCOMPARE(p_ServiceParameters->GetHeatedCuvetteParameters()->OvenMaxHigh, QString("70"));

    p_ServiceParameters->SetTestReportFolderPath("");
    QCOMPARE(p_ServiceParameters->GetTestReportFolderPath(), QString(""));

    p_ServiceParameters->SetFirmwareFolderPath(QString(""));
    QCOMPARE(p_ServiceParameters->GetFirmwareFolderPath(), QString(""));

    FirmwareFileNames_t FirmwareFileNames;
    FirmwareFileNames.ASB_1_Filename = QString("ASB1.bin");
    FirmwareFileNames.ASB_2_Filename = QString("ASB2.bin");
    FirmwareFileNames.ASB_3_Filename = QString("ASB3.bin");
    FirmwareFileNames.ASB_4_Filename = QString("ASB4.bin");
    FirmwareFileNames.ASB_5_Filename = QString("ASB5.bin");
    FirmwareFileNames.ASB_a_Filename = QString("ASBa.bin");
    FirmwareFileNames.ASB_b_Filename = QString("ASBb.bin");
    FirmwareFileNames.ASB_12_Filename = QString("ASB12.bin");
    FirmwareFileNames.ASB_13_Filename = QString("ASB13.bin");

    p_ServiceParameters->SetFirmwareFilenames(FirmwareFileNames);
    QCOMPARE(p_ServiceParameters->GetFirmwareFilenames()->ASB_1_Filename, QString("ASB1.bin"));
    QCOMPARE(p_ServiceParameters->GetFirmwareFilenames()->ASB_2_Filename, QString("ASB2.bin"));
    QCOMPARE(p_ServiceParameters->GetFirmwareFilenames()->ASB_3_Filename, QString("ASB3.bin"));
    QCOMPARE(p_ServiceParameters->GetFirmwareFilenames()->ASB_4_Filename, QString("ASB4.bin"));
    QCOMPARE(p_ServiceParameters->GetFirmwareFilenames()->ASB_5_Filename, QString("ASB5.bin"));
    QCOMPARE(p_ServiceParameters->GetFirmwareFilenames()->ASB_a_Filename, QString("ASBa.bin"));
    QCOMPARE(p_ServiceParameters->GetFirmwareFilenames()->ASB_b_Filename, QString("ASBb.bin"));
    QCOMPARE(p_ServiceParameters->GetFirmwareFilenames()->ASB_12_Filename, QString("ASB12.bin"));
    QCOMPARE(p_ServiceParameters->GetFirmwareFilenames()->ASB_13_Filename, QString("ASB13.bin"));

    ModuleNumbers_t ModuleNumbers;
    ModuleNumbers.DrawerLeft = QString("001");
    ModuleNumbers.DrawerRight = QString("002");
    ModuleNumbers.Oven = QString("003");
    ModuleNumbers.XArmLeft = QString("004");
    ModuleNumbers.XArmRight = QString("005");
    ModuleNumbers.XAxis = QString("006");
    ModuleNumbers.HeatedCuvettes = QString("007");
    ModuleNumbers.Agitator = QString("008");
    ModuleNumbers.AirExhaust = QString("009");
    ModuleNumbers.E_Box = QString("0010");
    ModuleNumbers.WaterStation = QString("0011");
    ModuleNumbers.TransferStation = QString("0012");

    p_ServiceParameters->SetModuleNumbers(ModuleNumbers);
    QCOMPARE(p_ServiceParameters->GetModuleNumbers()->DrawerLeft, QString("001"));
    QCOMPARE(p_ServiceParameters->GetModuleNumbers()->DrawerRight, QString("002"));
    QCOMPARE(p_ServiceParameters->GetModuleNumbers()->Oven, QString("003"));
    QCOMPARE(p_ServiceParameters->GetModuleNumbers()->XArmLeft, QString("004"));
    QCOMPARE(p_ServiceParameters->GetModuleNumbers()->XArmRight, QString("005"));
    QCOMPARE(p_ServiceParameters->GetModuleNumbers()->XAxis, QString("006"));
    QCOMPARE(p_ServiceParameters->GetModuleNumbers()->HeatedCuvettes, QString("007"));
    QCOMPARE(p_ServiceParameters->GetModuleNumbers()->Agitator, QString("008"));
    QCOMPARE(p_ServiceParameters->GetModuleNumbers()->AirExhaust, QString("009"));
    QCOMPARE(p_ServiceParameters->GetModuleNumbers()->E_Box, QString("0010"));
    QCOMPARE(p_ServiceParameters->GetModuleNumbers()->WaterStation, QString("0011"));
    QCOMPARE(p_ServiceParameters->GetModuleNumbers()->TransferStation, QString("0012"));

    CalibrationOffsets_t CalibrationOffsets;
    CalibrationOffsets.OvenOpenPos = QString("0");
    CalibrationOffsets.OvenClosePos = QString("0");
    CalibrationOffsets.DrawerOpenPos = QString("0");
    CalibrationOffsets.DrawerClosePos = QString("0");
    CalibrationOffsets.StandardOffset = QString("0");

    p_ServiceParameters->SetCalibrationOffsets(CalibrationOffsets);
    QCOMPARE(p_ServiceParameters->GetCalibrationOffsets()->OvenOpenPos, QString("0"));
    QCOMPARE(p_ServiceParameters->GetCalibrationOffsets()->OvenClosePos, QString("0"));
    QCOMPARE(p_ServiceParameters->GetCalibrationOffsets()->DrawerOpenPos, QString("0"));
    QCOMPARE(p_ServiceParameters->GetCalibrationOffsets()->DrawerClosePos, QString("0"));
    QCOMPARE(p_ServiceParameters->GetCalibrationOffsets()->StandardOffset, QString("0"));

    CalibrationParameters_t CalibrationParameters;
    CalibrationParameters.AgitationWidth12 = QString("0");
    CalibrationParameters.AgitationWidth21 = QString("0");
    CalibrationParameters.MinDiff03 = QString("0");

    p_ServiceParameters->SetCalibrationParameters(CalibrationParameters);
    QCOMPARE(p_ServiceParameters->GetCalibrationParameters()->AgitationWidth12, QString("0"));
    QCOMPARE(p_ServiceParameters->GetCalibrationParameters()->AgitationWidth21, QString("0"));
    QCOMPARE(p_ServiceParameters->GetCalibrationParameters()->MinDiff03, QString("0"));

    p_ServiceParameters->SetSystemLogOffTime(1000);
    QCOMPARE(p_ServiceParameters->GetSystemLogOffTime(), 1000);

    p_ServiceParameters->SetFilename("");
    QCOMPARE(p_ServiceParameters->GetFilename(), QString(""));

    p_ServiceParameters->SetDataVerificationMode(false);
    QCOMPARE(p_ServiceParameters->GetDataVerificationMode(), false);

    QCOMPARE(p_ServiceParameters->GetDataContainerType(), INSTRUMENTHISTORY);

    QString FileName = Global::SystemPaths::Instance().GetSettingsPath() + "/ServiceParameters.xml";
    QCOMPARE(p_ServiceParameters->Read(FileName), true);

    DataManager::CServiceParameters *p_ServiceParametersCopy = new DataManager::CServiceParameters(*p_ServiceParameters);
    p_ServiceParametersCopy->SetDataVerificationMode(true);
    p_ServiceParametersCopy->SetFilename(FileName);

    QFile FileOpen(p_ServiceParametersCopy->GetFilename());
    p_ServiceParametersCopy->DeserializeContent(FileOpen, true);

    QString WriteFilePath = Global::SystemPaths::Instance().GetSettingsPath() + QDir::separator() + "ServiceParamatersOutput.xml";
    p_ServiceParametersCopy->SetFilename(WriteFilePath);
    QFile OutFileOpen(p_ServiceParametersCopy->GetFilename());
    p_ServiceParametersCopy->SerializeContent(OutFileOpen, true);

    DataManager::CServiceParameters *p_ServiceParameters1 = new DataManager::CServiceParameters();
    *p_ServiceParameters1 = *p_ServiceParametersCopy;
    delete p_ServiceParametersCopy;

    QCOMPARE(p_ServiceParameters1->GetVersion(), 1);

    delete p_ServiceParameters1;
    delete p_ServiceParameters;
}

/****************************************************************************/
void CTestServiceDataManager::TestServiceParametersVerifier() {

    DataManager::CServiceParameters *p_ServiceParameters = new DataManager::CServiceParameters();
    DataManager::CServiceParametersVerifier *p_ServiceParametersVerifier = new
            DataManager::CServiceParametersVerifier();

    p_ServiceParameters->SetVerion(1);

    if(!p_ServiceParameters->AddVerifier(p_ServiceParametersVerifier)) {
        QFAIL("Compiler never should come to this statement");
    }
    p_ServiceParameters->SetDataVerificationMode(false);
    p_ServiceParameters->SetProxyUserName("Himalaya");
    p_ServiceParameters->SetProxyPassword("Himalaya");
    p_ServiceParameters->SetProxyIPAddress("123.234.123.234");
    p_ServiceParameters->SetProxyIPPort(1234);

    LoaderRFID_t LoaderValue;
    LoaderValue.Rack1 = QString("0xAB12345CADFE1234");
    LoaderValue.Rack2 = QString("0xAB12345CADFECEAD");
    LoaderValue.Rack3 = QString("0xAB12345CADFE9634");
    LoaderValue.Rack4 = QString("0xAB12345CADFE1324");
    LoaderValue.Rack5 = QString("0xAB12345CADFEABCD");
    p_ServiceParameters->SetLoaderRFID(LoaderValue);

    UnloaderRFID_t UnloaderValue;
    UnloaderValue.Rack1 = QString("0xAB12345CADFE1234");
    UnloaderValue.Rack2 = QString("0xAB12345CADFECEAD");
    UnloaderValue.Rack3 = QString("0xAB12345CADFE9634");
    UnloaderValue.Rack4 = QString("0xAB12345CADFE1324");
    UnloaderValue.Rack5 = QString("0xAB12345CADFEABCD");
    p_ServiceParameters->SetUnloaderRFID(UnloaderValue);

    p_ServiceParameters->SetYZModuleRFID("0xAB12345CADFEQPWE");

    XArmLeftParameters_t XArmLeftParameters;
    XArmLeftParameters.Filledx = QString("5");
    XArmLeftParameters.Filledy = QString("2");
    XArmLeftParameters.Emptyx = QString("4");
    XArmLeftParameters.Emptyy = QString("5");
    XArmLeftParameters.Rackx = QString("8");
    XArmLeftParameters.Racky = QString("6");
    p_ServiceParameters->SetXArmLeftParamters(XArmLeftParameters);

    XArmRightParameters_t XArmRightParameters;
    XArmRightParameters.Filledx = QString("5");
    XArmRightParameters.Filledy = QString("2");
    XArmRightParameters.Emptyx = QString("4");
    XArmRightParameters.Emptyy = QString("5");
    XArmRightParameters.Rackx = QString("8");
    XArmRightParameters.Racky = QString("6");
    p_ServiceParameters->SetXArmRightParamters(XArmRightParameters);

    OvenParameters_t OvenParameters;
    OvenParameters.MaxDiff = QString("3");
    OvenParameters.RoomTempLow = QString("18");
    OvenParameters.RoomTempHigh = QString("30");
    OvenParameters.HTime1 = QString("1");
    OvenParameters.HTime2 = QString("2");
    OvenParameters.TempDiff = QString("15");
    OvenParameters.OvenMaxLow = QString("40");
    OvenParameters.OvenMaxHigh = QString("70");
    p_ServiceParameters->SetOvenParamters(OvenParameters);

    HeatedCuvetteParameters_t HCParameters;
    HCParameters.MaxDiff = QString("3");
    HCParameters.RoomTempLow = QString("18");
    HCParameters.RoomTempHigh = QString("30");
    HCParameters.HTime1 = QString("1");
    HCParameters.HTime2 = QString("2");
    HCParameters.TempDiff = QString("15");
    HCParameters.OvenMaxLow = QString("40");
    HCParameters.OvenMaxHigh = QString("70");
    p_ServiceParameters->SetHeatedCuvetteParamters(HCParameters);

    p_ServiceParameters->SetTestReportFolderPath("/home/himalaya/Temporary");

    p_ServiceParameters->SetFirmwareFolderPath("/home/himalaya/Firmware");

    FirmwareFileNames_t FirmwareFileNames;
    FirmwareFileNames.ASB_1_Filename = QString("ASB1.bin");
    FirmwareFileNames.ASB_2_Filename = QString("ASB2.bin");
    FirmwareFileNames.ASB_3_Filename = QString("ASB3.bin");
    FirmwareFileNames.ASB_4_Filename = QString("ASB4.bin");
    FirmwareFileNames.ASB_5_Filename = QString("ASB5.bin");
    FirmwareFileNames.ASB_a_Filename = QString("ASBa.bin");
    FirmwareFileNames.ASB_b_Filename = QString("ASBb.bin");
    FirmwareFileNames.ASB_12_Filename = QString("ASB12.bin");
    FirmwareFileNames.ASB_13_Filename = QString("ASB13.bin");
    p_ServiceParameters->SetFirmwareFilenames(FirmwareFileNames);

    ModuleNumbers_t ModuleNumbers;
    ModuleNumbers.DrawerLeft = QString("001");
    ModuleNumbers.DrawerRight = QString("002");
    ModuleNumbers.Oven = QString("003");
    ModuleNumbers.XArmLeft = QString("004");
    ModuleNumbers.XArmRight = QString("005");
    ModuleNumbers.XAxis = QString("006");
    ModuleNumbers.HeatedCuvettes = QString("007");
    ModuleNumbers.Agitator = QString("008");
    ModuleNumbers.AirExhaust = QString("009");
    ModuleNumbers.E_Box = QString("0010");
    ModuleNumbers.WaterStation = QString("0011");
    ModuleNumbers.TransferStation = QString("0012");
    p_ServiceParameters->SetModuleNumbers(ModuleNumbers);

    CalibrationOffsets_t CalibrationOffsets;
    CalibrationOffsets.OvenOpenPos = QString("0");
    CalibrationOffsets.OvenClosePos = QString("0");
    CalibrationOffsets.DrawerOpenPos = QString("0");
    CalibrationOffsets.DrawerClosePos = QString("0");
    CalibrationOffsets.StandardOffset = QString("0");
    p_ServiceParameters->SetCalibrationOffsets(CalibrationOffsets);

    CalibrationParameters_t CalibrationParameters;
    CalibrationParameters.AgitationWidth12 = QString("0");
    CalibrationParameters.AgitationWidth21 = QString("0");
    CalibrationParameters.MinDiff03 = QString("0");
    p_ServiceParameters->SetCalibrationParameters(CalibrationParameters);

    p_ServiceParameters->SetSystemLogOffTime(1000);
    QCOMPARE(p_ServiceParameters->GetSystemLogOffTime(), 1000);

    QCOMPARE(p_ServiceParameters->VerifyData(), true);

    delete p_ServiceParameters;
    delete p_ServiceParametersVerifier;
}

} // end namespace ImportExport

QTEST_MAIN(DataManager::CTestServiceDataManager)

#include "TestServiceDataManager.moc"
