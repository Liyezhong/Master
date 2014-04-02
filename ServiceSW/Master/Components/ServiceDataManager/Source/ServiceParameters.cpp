/****************************************************************************/
/*! \file ServiceParameters.cpp
 *
 *  \brief Implementation file for class CServiceParameters.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-12-21
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

#include "DataManager/Helper/Include/Helper.h"
#include "ServiceDataManager/Include/ServiceParameters.h"
#include <Global/Include/SystemPaths.h>
#include <QDebug>

namespace DataManager {

/****************************************************************************/
/**
 * \brief Constructor
 */
/****************************************************************************/
CServiceParameters::CServiceParameters()  : CDataContainerBase(),
    m_Version(1),
    m_ProxyUserName("Colorado"),
    m_ProxyPassword("Colorado"),
    m_ProxyIPAddress("000.000.000.001"),
    m_ProxyIPPort(1),
    mp_LoaderRfid(),
    mp_UnloaderRfid(),
    mp_XArmLeftParameters(),
    mp_XArmRightParameters(),
    mp_OvenParameters(),
    mp_HCParameters(),
    mp_FirmwareFilenames(),
    mp_ModuleNumbers(),
    mp_CalibrationOffsets(),
    mp_CalibrationParameters(),
    m_YZModuleRfid(""),
    m_TestReportFolderPath(""),
    m_FirmwarefolderPath(""),    
    m_SystemLogOffTime(0),
    m_DataVerificationMode(true),
    m_FileName("") {

}

/****************************************************************************/
/**
 * \brief Destructor
 */
/****************************************************************************/
CServiceParameters::~CServiceParameters() {

}

/****************************************************************************/
/**
 * \brief Copy constructor
 * \iparam ServiceParameters = CServiceParameters object
 */
/****************************************************************************/
CServiceParameters::CServiceParameters(const CServiceParameters& ServiceParameters) : CDataContainerBase(),
    m_Version(1),
    m_ProxyUserName("Colorado"),
    m_ProxyPassword("Colorado"),
    m_ProxyIPAddress("000.000.000.001"),
    m_ProxyIPPort(1),
    mp_LoaderRfid(),
    mp_UnloaderRfid(),
    mp_XArmLeftParameters(),
    mp_XArmRightParameters(),
    mp_OvenParameters(),
    mp_HCParameters(),
    mp_FirmwareFilenames(),
    mp_ModuleNumbers(),
    mp_CalibrationOffsets(),
    mp_CalibrationParameters(),
    m_YZModuleRfid(""),
    m_TestReportFolderPath(""),
    m_FirmwarefolderPath(""),
    m_SystemLogOffTime(0),
    m_DataVerificationMode(true),
    m_FileName("")
{
    *this = ServiceParameters;
}

/****************************************************************************/
/**
 * \brief Reads the xml file
 * \iparam FileName = name of the file to read
 * \return True or False
 */
/****************************************************************************/
bool CServiceParameters::Read(QString FileName)
{
    bool Result = true;
//    FileName = Global::SystemPaths::Instance().GetSettingsPath() + "/ServiceParameters.xml";
//    SetFilename(FileName);
    QFile File(FileName);
    if (!QFile::exists(FileName)) {
        return false;
    }

    if (!File.open(QFile::ReadOnly | QFile::Text )) {
        qDebug() << "Open file to read failed " << FileName;
        return false;
    }

    if (!DeserializeContent(File, false)) {
        qDebug() << " CServiceParameters::Read failed for file during deserializing: " << FileName;
        return false;
    }

    m_FileName = FileName;

    File.close();

    return Result;
}

/****************************************************************************/
/*!
 *  \brief Reads the CModuleDataList Data from QIODevice
 *  \iparam IODevice = Instance of the IODevice - Buffer or File
 *  \iparam CompleteData = bool type if true writes Complete data of object
 *  \return True or False
 */
/****************************************************************************/
bool CServiceParameters::DeserializeContent(QIODevice &IODevice, bool CompleteData)
{
    bool Result = true;
    QXmlStreamReader XmlStreamReader;
    XmlStreamReader.setDevice(&IODevice);

    // look for node <ServiceParameters>
    if (!Helper::ReadNode(XmlStreamReader, "ServiceParameters")) {
        qDebug() << "DeserializeContent: abort reading. Node not found: ServiceParameters";
        return false;
    }

    // Read attribute Name
    if (!XmlStreamReader.attributes().hasAttribute("Version")) {
        qDebug() << " attribute ServiceParameters <Version> is missing => abort reading";
        return false;
    }

    SetVerion(XmlStreamReader.attributes().value("Version").toString().toInt());

    while ((!XmlStreamReader.atEnd()) || (XmlStreamReader.tokenType() == QXmlStreamReader::StartElement))
    {
        (void) XmlStreamReader.readNextStartElement();

        if (XmlStreamReader.tokenType() == QXmlStreamReader::StartElement)
        {
            if (XmlStreamReader.name() == "Network")
            {
                if (!ReadNetworkSettings(XmlStreamReader)) {
                    qDebug() << "CServiceParameters::DeserializeContent: Read Network is failed";
                    return false;
                }
            }
            else if (XmlStreamReader.name() == "LoaderRFID")
            {
                if (!ReadLoaderRFIDSettings(XmlStreamReader)) {
                    qDebug() << "CServiceParameters::DeserializeContent: Read LoaderRFID settings is failed";
                    return false;
                }
            }
            else if (XmlStreamReader.name() == "UnloaderRFID")
            {
                if (!ReadUnloaderRFIDSettings(XmlStreamReader)) {
                    qDebug() << "CServiceParameters::DeserializeContent: Read Unloader RFID is failed";
                    return false;
                }
            }
            else if (XmlStreamReader.name() == "YZModuleRFID")
            {
                if (!ReadYZRFIDSettings(XmlStreamReader)) {
                    qDebug() << "CServiceParameters::DeserializeContent: Read YZ module RFID is failed";
                    return false;
                }
            }
            else if (XmlStreamReader.name() == "XArmLeft")
            {
                if (!ReadXArmLeftParameters(XmlStreamReader)) {
                    qDebug() << "CServiceParameters::DeserializeContent: Read X arm left parameters is failed";
                    return false;
                }
            }
            else if (XmlStreamReader.name() == "XArmRight")
            {
                if (!ReadXArmRightParameters(XmlStreamReader)) {
                    qDebug() << "CServiceParameters::DeserializeContent: Read X arm right parameters is failed";
                    return false;
                }
            }
            else if (XmlStreamReader.name() == "OvenHeatingTest")
            {
                if (!ReadOvenParameters(XmlStreamReader)) {
                    qDebug() << "CServiceParameters::DeserializeContent: Read Oven parameters is failed";
                    return false;
                }
            }
            else if (XmlStreamReader.name() == "CuvettesHeatingTest")
            {
                if (!ReadCuvettesParameters(XmlStreamReader)) {
                    qDebug() << "CServiceParameters::DeserializeContent: Read Cuvettes parameters is failed";
                    return false;
                }
            }
            else if (XmlStreamReader.name() == "TestReportFolder")
            {
                if (!ReadTestReportFolderPath(XmlStreamReader)) {
                    qDebug() << "CServiceParameters::DeserializeContent: Read TestReportFolder path is failed";
                    return false;
                }
            }
            else if (XmlStreamReader.name() == "FirmwareFolder")
            {
                if (!ReadFirmwareFolderPath(XmlStreamReader)) {
                    qDebug() << "CServiceParameters::DeserializeContent: Read FirmwareFolder Path is failed";
                    return false;
                }
            }
            else if (XmlStreamReader.name() == "FirmwareFileNames")
            {
                if (!ReadFirmwareFileNames(XmlStreamReader)) {
                    qDebug() << "CServiceParameters::DeserializeContent: Read FirmwareFileNames Path is failed";
                    return false;
                }
            }
            else if (XmlStreamReader.name() == "ModuleNumber")
            {
                if (!ReadModuleNumbers(XmlStreamReader)) {
                    qDebug() << "CServiceParameters::DeserializeContent: Read Module numbers is failed";
                    return false;
                }
            }
            else if (XmlStreamReader.name() == "CalibrationOffsets")
            {
                if (!ReadCalibrationOffsets(XmlStreamReader)) {
                    qDebug() << "CServiceParameters::DeserializeContent: Read Calibration offsets is failed";
                    return false;
                }
            }
            else if (XmlStreamReader.name() == "CalibrationParameters")
            {
                if (!ReadCalibrationParameters(XmlStreamReader)) {
                    qDebug() << "CServiceParameters::DeserializeContent: Read Calibration parameters is failed";
                    return false;
                }
            }
            else if (XmlStreamReader.name() == "SystemLogOff")
            {
                if (!ReadSystemLogOffTime(XmlStreamReader)) {
                    qDebug() << "CServiceParameters::DeserializeContent: Read System log off time is failed";
                    return false;
                }
            }
        }
    }

    if(CompleteData) {

    }

    return Result;
}

/****************************************************************************/
/*!
 *  \brief Writes the CModuleDataList Data to QIODevice
 *  \iparam IODevice = Instance of the IODevice might be Buffer or File
 *  \iparam CompleteData = bool type if true writes Complete data of object
 *  \return True or False
 */
/****************************************************************************/
bool CServiceParameters::SerializeContent(QIODevice &IODevice, bool CompleteData)
{
    QXmlStreamWriter XmlStreamWriter; ///< Xml stream writer object to write the Xml contents in a file

    XmlStreamWriter.setDevice(&IODevice);
    XmlStreamWriter.setAutoFormatting(true);

    // start the XML Document
    XmlStreamWriter.writeStartDocument();

    // write the documnet type declaration
    XmlStreamWriter.writeDTD("<!DOCTYPE ServiceParameters>");
    XmlStreamWriter.writeStartElement("ServiceParameters");

    QString StringValue; ///< to store the version number
    // write version number
    (void) StringValue.setNum(GetVersion());  //to suppress lint-534
    XmlStreamWriter.writeAttribute("Version", StringValue);

    //write network settings realted details
    XmlStreamWriter.writeStartElement("Network");
    XmlStreamWriter.writeAttribute("ProxyUserName", GetProxyUserName());
    XmlStreamWriter.writeAttribute("ProxyPassword", GetProxyPassword());
    XmlStreamWriter.writeAttribute("ProxyIPAddress", GetProxyIPAddress());
    XmlStreamWriter.writeAttribute("ProxyIPPort", QString::number(GetProxyIPPort(), 10));
    XmlStreamWriter.writeEndElement();

    // write the Drawer loader RFID details
    XmlStreamWriter.writeStartElement("LoaderRFID");
    XmlStreamWriter.writeAttribute("Rack1", mp_LoaderRfid->Rack1);
    XmlStreamWriter.writeAttribute("Rack2", mp_LoaderRfid->Rack2);
    XmlStreamWriter.writeAttribute("Rack3", mp_LoaderRfid->Rack3);
    XmlStreamWriter.writeAttribute("Rack4", mp_LoaderRfid->Rack4);
    XmlStreamWriter.writeAttribute("Rack5", mp_LoaderRfid->Rack5);
    XmlStreamWriter.writeEndElement();

    // write the Drawer unloader RFID details
    XmlStreamWriter.writeStartElement("UnloaderRFID");
    XmlStreamWriter.writeAttribute("Rack1", mp_UnloaderRfid->Rack1);
    XmlStreamWriter.writeAttribute("Rack2", mp_UnloaderRfid->Rack2);
    XmlStreamWriter.writeAttribute("Rack3", mp_UnloaderRfid->Rack3);
    XmlStreamWriter.writeAttribute("Rack4", mp_UnloaderRfid->Rack4);
    XmlStreamWriter.writeAttribute("Rack5", mp_UnloaderRfid->Rack5);
    XmlStreamWriter.writeEndElement();

    // write YZ Module RFID details
    XmlStreamWriter.writeStartElement("YZModuleRFID");
    XmlStreamWriter.writeAttribute("Rfid", m_YZModuleRfid);
    XmlStreamWriter.writeEndElement();

    // write X Arm Left parameters
    XmlStreamWriter.writeStartElement("XArmLeft");
    XmlStreamWriter.writeAttribute("Filledx", mp_XArmLeftParameters->Filledx);
    XmlStreamWriter.writeAttribute("Filledy", mp_XArmLeftParameters->Filledy);
    XmlStreamWriter.writeAttribute("Emptyx", mp_XArmLeftParameters->Emptyx);
    XmlStreamWriter.writeAttribute("Emptyy", mp_XArmLeftParameters->Emptyy);
    XmlStreamWriter.writeAttribute("Rackx", mp_XArmLeftParameters->Rackx);
    XmlStreamWriter.writeAttribute("Racky", mp_XArmLeftParameters->Racky);
    XmlStreamWriter.writeEndElement();

    // write X Arm Right parameters
    XmlStreamWriter.writeStartElement("XArmRight");
    XmlStreamWriter.writeAttribute("Filledx", mp_XArmRightParameters->Filledx);
    XmlStreamWriter.writeAttribute("Filledy", mp_XArmRightParameters->Filledy);
    XmlStreamWriter.writeAttribute("Emptyx", mp_XArmRightParameters->Emptyx);
    XmlStreamWriter.writeAttribute("Emptyy", mp_XArmRightParameters->Emptyy);
    XmlStreamWriter.writeAttribute("Rackx", mp_XArmRightParameters->Rackx);
    XmlStreamWriter.writeAttribute("Racky", mp_XArmRightParameters->Racky);
    XmlStreamWriter.writeEndElement();

    // write Oven parameters
    XmlStreamWriter.writeStartElement("OvenHeatingTest");
    XmlStreamWriter.writeAttribute("MaxDiff", mp_OvenParameters->MaxDiff);
    XmlStreamWriter.writeAttribute("RoomTempLow", mp_OvenParameters->RoomTempLow);
    XmlStreamWriter.writeAttribute("RoomTempHigh", mp_OvenParameters->RoomTempHigh);
    XmlStreamWriter.writeAttribute("HTime1", mp_OvenParameters->HTime1);
    XmlStreamWriter.writeAttribute("HTime2", mp_OvenParameters->HTime2);
    XmlStreamWriter.writeAttribute("TempDiff", mp_OvenParameters->TempDiff);
    XmlStreamWriter.writeAttribute("OvenMaxLow", mp_OvenParameters->OvenMaxLow);
    XmlStreamWriter.writeAttribute("OvenMaxHigh", mp_OvenParameters->OvenMaxHigh);
    XmlStreamWriter.writeEndElement();

    // write Cuvettes parameters
    XmlStreamWriter.writeStartElement("CuvettesHeatingTest");
    XmlStreamWriter.writeAttribute("MaxDiff", mp_HCParameters->MaxDiff);
    XmlStreamWriter.writeAttribute("RoomTempLow", mp_HCParameters->RoomTempLow);
    XmlStreamWriter.writeAttribute("RoomTempHigh", mp_HCParameters->RoomTempHigh);
    XmlStreamWriter.writeAttribute("HTime1", mp_HCParameters->HTime1);
    XmlStreamWriter.writeAttribute("HTime2", mp_HCParameters->HTime2);
    XmlStreamWriter.writeAttribute("TempDiff", mp_HCParameters->TempDiff);
    XmlStreamWriter.writeAttribute("OvenMaxLow", mp_HCParameters->OvenMaxLow);
    XmlStreamWriter.writeAttribute("OvenMaxHigh", mp_HCParameters->OvenMaxHigh);
    XmlStreamWriter.writeEndElement();

    // write Test report folder path
    XmlStreamWriter.writeStartElement("TestReportFolder");
    XmlStreamWriter.writeAttribute("TestReportFolderPath", m_TestReportFolderPath);
    XmlStreamWriter.writeEndElement();

    // write Firmware folder path
    XmlStreamWriter.writeStartElement("FirmwareFolder");
    XmlStreamWriter.writeAttribute("FirmwareFolderPath", m_FirmwarefolderPath);
    XmlStreamWriter.writeEndElement();

    XmlStreamWriter.writeStartElement("FirmwareFileNames");
    XmlStreamWriter.writeAttribute("ASB_1_Filename", mp_FirmwareFilenames->ASB_1_Filename);
    XmlStreamWriter.writeAttribute("ASB_2_Filename", mp_FirmwareFilenames->ASB_2_Filename);
    XmlStreamWriter.writeAttribute("ASB_3_Filename", mp_FirmwareFilenames->ASB_3_Filename);
    XmlStreamWriter.writeAttribute("ASB_4_Filename", mp_FirmwareFilenames->ASB_4_Filename);
    XmlStreamWriter.writeAttribute("ASB_5_Filename", mp_FirmwareFilenames->ASB_5_Filename);
    XmlStreamWriter.writeAttribute("ASB_a_Filename", mp_FirmwareFilenames->ASB_a_Filename);
    XmlStreamWriter.writeAttribute("ASB_b_Filename", mp_FirmwareFilenames->ASB_b_Filename);
    XmlStreamWriter.writeAttribute("ASB_12_Filename", mp_FirmwareFilenames->ASB_12_Filename);
    XmlStreamWriter.writeAttribute("ASB_13_Filename", mp_FirmwareFilenames->ASB_13_Filename);
    XmlStreamWriter.writeEndElement();

    XmlStreamWriter.writeStartElement("ModuleNumber");
    XmlStreamWriter.writeAttribute("DrawerLeft", mp_ModuleNumbers->DrawerLeft);
    XmlStreamWriter.writeAttribute("DrawerRight", mp_ModuleNumbers->DrawerRight);
    XmlStreamWriter.writeAttribute("Oven", mp_ModuleNumbers->Oven);
    XmlStreamWriter.writeAttribute("XArmLeft", mp_ModuleNumbers->XArmLeft);
    XmlStreamWriter.writeAttribute("XArmRight", mp_ModuleNumbers->XArmRight);
    XmlStreamWriter.writeAttribute("XAxis", mp_ModuleNumbers->XAxis);
    XmlStreamWriter.writeAttribute("RackTransfer", mp_ModuleNumbers->TransferStation);
    XmlStreamWriter.writeAttribute("E-Box", mp_ModuleNumbers->E_Box);
    XmlStreamWriter.writeAttribute("AirExhaust", mp_ModuleNumbers->AirExhaust);
    XmlStreamWriter.writeAttribute("WaterStation", mp_ModuleNumbers->WaterStation);
    XmlStreamWriter.writeAttribute("HeatedCuvettes", mp_ModuleNumbers->HeatedCuvettes);
    XmlStreamWriter.writeAttribute("Agitation", mp_ModuleNumbers->Agitator);
    XmlStreamWriter.writeEndElement();

    XmlStreamWriter.writeStartElement("CalibrationOffsets");
    XmlStreamWriter.writeAttribute("OvenOpenPos", mp_CalibrationOffsets->OvenOpenPos);
    XmlStreamWriter.writeAttribute("OvenClosePos", mp_CalibrationOffsets->OvenClosePos);
    XmlStreamWriter.writeAttribute("DrawerOpenPos", mp_CalibrationOffsets->DrawerOpenPos);
    XmlStreamWriter.writeAttribute("DrawerClosePos", mp_CalibrationOffsets->DrawerClosePos);
    XmlStreamWriter.writeAttribute("StandardOffset", mp_CalibrationOffsets->StandardOffset);
    XmlStreamWriter.writeEndElement();

    XmlStreamWriter.writeStartElement("CalibrationParameters");
    XmlStreamWriter.writeAttribute("AgitationWidth12", mp_CalibrationParameters->AgitationWidth12);
    XmlStreamWriter.writeAttribute("AgitationWidth21", mp_CalibrationParameters->AgitationWidth21);
    XmlStreamWriter.writeAttribute("MinDiff03", mp_CalibrationParameters->MinDiff03);
    XmlStreamWriter.writeEndElement();

    XmlStreamWriter.writeStartElement("SystemLogOffTime");
    XmlStreamWriter.writeAttribute("TimeInMSec", QString::number(m_SystemLogOffTime));
    XmlStreamWriter.writeEndElement();

    if(CompleteData) {

    }
    // write end element for ServiceParameters
    XmlStreamWriter.writeEndElement();

    // write end document
    XmlStreamWriter.writeEndDocument();

    return true;
}

/****************************************************************************/
/*!
 *  \brief Reads Network settings from xml
 *  \iparam XmlStreamReader = Xml reader to read the XML contents
 *  \return True or False
 */
/****************************************************************************/
bool CServiceParameters::ReadNetworkSettings(QXmlStreamReader &XmlStreamReader)
{
    // read Network settings
    if (!XmlStreamReader.attributes().hasAttribute("ProxyUserName")) {
        qDebug() << "CServiceParameters::ReadNetworkSettings:### attribute <ProxyUserName> is missing => abort reading";
        return false;
    }
    SetProxyUserName(XmlStreamReader.attributes().value("ProxyUserName").toString());

    if (!XmlStreamReader.attributes().hasAttribute("ProxyPassword")) {
        qDebug() << "CServiceParameters::ReadNetworkSettings:### attribute <ProxyPassword> is missing => abort reading";
        return false;
    }
    SetProxyPassword(XmlStreamReader.attributes().value("ProxyPassword").toString());

    if (!XmlStreamReader.attributes().hasAttribute("ProxyIPAddress")) {
        qDebug() << "CServiceParameters::ReadNetworkSettings:### attribute <ProxyIPAddress> is missing => abort reading";
        return false;
    }
    SetProxyIPAddress(XmlStreamReader.attributes().value("ProxyIPAddress").toString());

    if (!XmlStreamReader.attributes().hasAttribute("ProxyIPPort")) {
        qDebug() << "CServiceParameters::ReadNetworkSettings:### attribute <ProxyIPPort> is missing => abort reading";
        return false;
    }
    SetProxyIPPort(XmlStreamReader.attributes().value("ProxyIPPort").toString().toInt());

    return true;
}

/****************************************************************************/
/*!
 *  \brief Reads Drawer loader RFID's from xml
 *  \iparam XmlStreamReader = Xml reader to read the XML contents
 *  \return True or False
 */
/****************************************************************************/
bool CServiceParameters::ReadLoaderRFIDSettings(QXmlStreamReader &XmlStreamReader)
{
    LoaderRFID_t RFID;
    // read Drawer loader RFID settings
    if (!XmlStreamReader.attributes().hasAttribute("Rack1")) {
        qDebug() << "CServiceParameters::ReadLoaderRFIDSettings:### attribute <Rack1> is missing => abort reading";
        return false;
    }
    RFID.Rack1 = XmlStreamReader.attributes().value("Rack1").toString();

    if (!XmlStreamReader.attributes().hasAttribute("Rack2")) {
        qDebug() << "CServiceParameters::ReadLoaderRFIDSettings:### attribute <Rack2> is missing => abort reading";
        return false;
    }
    RFID.Rack2 = XmlStreamReader.attributes().value("Rack2").toString();

    if (!XmlStreamReader.attributes().hasAttribute("Rack3")) {
        qDebug() << "CServiceParameters::ReadLoaderRFIDSettings:### attribute <Rack3> is missing => abort reading";
        return false;
    }
    RFID.Rack3 = XmlStreamReader.attributes().value("Rack3").toString();

    if (!XmlStreamReader.attributes().hasAttribute("Rack4")) {
        qDebug() << "CServiceParameters::ReadLoaderRFIDSettings:### attribute <Rack4> is missing => abort reading";
        return false;
    }
    RFID.Rack4 = XmlStreamReader.attributes().value("Rack4").toString();

    if (!XmlStreamReader.attributes().hasAttribute("Rack5")) {
        qDebug() << "CServiceParameters::ReadLoaderRFIDSettings:### attribute <Rack5> is missing => abort reading";
        return false;
    }
    RFID.Rack5 = XmlStreamReader.attributes().value("Rack5").toString();

    SetLoaderRFID(RFID);

    return true;
}

/****************************************************************************/
/*!
 *  \brief Reads Drawer unloader RFID's from xml
 *  \iparam XmlStreamReader = Xml reader to read the XML contents
 *  \return True or False
 */
/****************************************************************************/
bool CServiceParameters::ReadUnloaderRFIDSettings(QXmlStreamReader &XmlStreamReader)
{
    UnloaderRFID_t RFID;
    // read Drawer loader RFID settings
    if (!XmlStreamReader.attributes().hasAttribute("Rack1")) {
        qDebug() << "CServiceParameters::ReadUnloaderRFIDSettings:### attribute <Rack1> is missing => abort reading";
        return false;
    }
    RFID.Rack1 = XmlStreamReader.attributes().value("Rack1").toString();

    if (!XmlStreamReader.attributes().hasAttribute("Rack2")) {
        qDebug() << "CServiceParameters::ReadUnloaderRFIDSettings:### attribute <Rack2> is missing => abort reading";
        return false;
    }
    RFID.Rack2 = XmlStreamReader.attributes().value("Rack2").toString();

    if (!XmlStreamReader.attributes().hasAttribute("Rack3")) {
        qDebug() << "CServiceParameters::ReadUnloaderRFIDSettings:### attribute <Rack3> is missing => abort reading";
        return false;
    }
    RFID.Rack3 = XmlStreamReader.attributes().value("Rack3").toString();

    if (!XmlStreamReader.attributes().hasAttribute("Rack4")) {
        qDebug() << "CServiceParameters::ReadUnloaderRFIDSettings:### attribute <Rack4> is missing => abort reading";
        return false;
    }
    RFID.Rack4 = XmlStreamReader.attributes().value("Rack4").toString();

    if (!XmlStreamReader.attributes().hasAttribute("Rack5")) {
        qDebug() << "CServiceParameters::ReadUnloaderRFIDSettings:### attribute <Rack5> is missing => abort reading";
        return false;
    }
    RFID.Rack5 = XmlStreamReader.attributes().value("Rack5").toString();

    SetUnloaderRFID(RFID);

    return true;
}

/****************************************************************************/
/*!
 *  \brief Reads YZ module RFID from xml
 *  \iparam XmlStreamReader = Xml reader to read the XML contents
 *  \return True or False
 */
/****************************************************************************/
bool CServiceParameters::ReadYZRFIDSettings(QXmlStreamReader &XmlStreamReader)
{
    if (!XmlStreamReader.attributes().hasAttribute("Rfid")) {
        qDebug() << "CServiceParameters::ReadYZRFIDSettings:### attribute <Rfid> is missing => abort reading";
        return false;
    }
    SetYZModuleRFID(XmlStreamReader.attributes().value("Rfid").toString());

    return true;
}

/****************************************************************************/
/*!
 *  \brief Reads X arm left parameters from xml
 *  \iparam XmlStreamReader = Xml reader to read the XML contents
 *  \return True or False
 */
/****************************************************************************/
bool CServiceParameters::ReadXArmLeftParameters(QXmlStreamReader &XmlStreamReader)
{
    XArmLeftParameters_t XArmLeftParameters;
    if (!XmlStreamReader.attributes().hasAttribute("Filledx")) {
        qDebug() << "CServiceParameters::ReadXArmLeftParameters:### attribute <Filledx> is missing => abort reading";
        return false;
    }
    XArmLeftParameters.Filledx = XmlStreamReader.attributes().value("Filledx").toString();

    if (!XmlStreamReader.attributes().hasAttribute("Filledy")) {
        qDebug() << "CServiceParameters::ReadXArmLeftParameters:### attribute <Filledy> is missing => abort reading";
        return false;
    }
    XArmLeftParameters.Filledy = XmlStreamReader.attributes().value("Filledy").toString();

    if (!XmlStreamReader.attributes().hasAttribute("Emptyx")) {
        qDebug() << "CServiceParameters::ReadXArmLeftParameters:### attribute <Emptyx> is missing => abort reading";
        return false;
    }
    XArmLeftParameters.Emptyx = XmlStreamReader.attributes().value("Emptyx").toString();

    if (!XmlStreamReader.attributes().hasAttribute("Emptyy")) {
        qDebug() << "CServiceParameters::ReadXArmLeftParameters:### attribute <Emptyy> is missing => abort reading";
        return false;
    }
    XArmLeftParameters.Emptyy = XmlStreamReader.attributes().value("Emptyy").toString();

    if (!XmlStreamReader.attributes().hasAttribute("Rackx")) {
        qDebug() << "CServiceParameters::ReadXArmLeftParameters:### attribute <Rackx> is missing => abort reading";
        return false;
    }
    XArmLeftParameters.Rackx = XmlStreamReader.attributes().value("Rackx").toString();

    if (!XmlStreamReader.attributes().hasAttribute("Racky")) {
        qDebug() << "CServiceParameters::ReadXArmLeftParameters:### attribute <Racky> is missing => abort reading";
        return false;
    }
    XArmLeftParameters.Racky = XmlStreamReader.attributes().value("Racky").toString();

    SetXArmLeftParamters(XArmLeftParameters);

    return true;
}

/****************************************************************************/
/*!
 *  \brief Reads X arm right parameters from xml
 *  \iparam XmlStreamReader = Xml reader to read the XML contents
 *  \return True or False
 */
/****************************************************************************/
bool CServiceParameters::ReadXArmRightParameters(QXmlStreamReader &XmlStreamReader)
{
    XArmRightParameters_t XArmRightParameters;
    if (!XmlStreamReader.attributes().hasAttribute("Filledx")) {
        qDebug() << "CServiceParameters::ReadXArmRightParameters:### attribute <Filledx> is missing => abort reading";
        return false;
    }
    XArmRightParameters.Filledx = XmlStreamReader.attributes().value("Filledx").toString();

    if (!XmlStreamReader.attributes().hasAttribute("Filledy")) {
        qDebug() << "CServiceParameters::ReadXArmRightParameters:### attribute <Filledy> is missing => abort reading";
        return false;
    }
    XArmRightParameters.Filledy = XmlStreamReader.attributes().value("Filledy").toString();

    if (!XmlStreamReader.attributes().hasAttribute("Emptyx")) {
        qDebug() << "CServiceParameters::ReadXArmRightParameters:### attribute <Emptyx> is missing => abort reading";
        return false;
    }
    XArmRightParameters.Emptyx = XmlStreamReader.attributes().value("Emptyx").toString();

    if (!XmlStreamReader.attributes().hasAttribute("Emptyy")) {
        qDebug() << "CServiceParameters::ReadXArmRightParameters:### attribute <Emptyy> is missing => abort reading";
        return false;
    }
    XArmRightParameters.Emptyy = XmlStreamReader.attributes().value("Emptyy").toString();

    if (!XmlStreamReader.attributes().hasAttribute("Rackx")) {
        qDebug() << "CServiceParameters::ReadXArmRightParameters:### attribute <Rackx> is missing => abort reading";
        return false;
    }
    XArmRightParameters.Rackx = XmlStreamReader.attributes().value("Rackx").toString();

    if (!XmlStreamReader.attributes().hasAttribute("Racky")) {
        qDebug() << "CServiceParameters::ReadXArmRightParameters:### attribute <Racky> is missing => abort reading";
        return false;
    }
    XArmRightParameters.Racky = XmlStreamReader.attributes().value("Racky").toString();

    SetXArmRightParamters(XArmRightParameters);

    return true;
}

/****************************************************************************/
/*!
 *  \brief Reads Oven parameters from xml
 *  \iparam XmlStreamReader = Xml reader to read the XML contents
 *  \return True or False
 */
/****************************************************************************/
bool CServiceParameters::ReadOvenParameters(QXmlStreamReader &XmlStreamReader)
{
    OvenParameters_t OvenParameters;
    if (!XmlStreamReader.attributes().hasAttribute("MaxDiff")) {
        qDebug() << "CServiceParameters::ReadOvenParameters:### attribute <MaxDiff> is missing => abort reading";
        return false;
    }
    OvenParameters.MaxDiff = XmlStreamReader.attributes().value("MaxDiff").toString();

    if (!XmlStreamReader.attributes().hasAttribute("RoomTempLow")) {
        qDebug() << "CServiceParameters::ReadOvenParameters:### attribute <RoomTempLow> is missing => abort reading";
        return false;
    }
    OvenParameters.RoomTempLow = XmlStreamReader.attributes().value("RoomTempLow").toString();

    if (!XmlStreamReader.attributes().hasAttribute("RoomTempHigh")) {
        qDebug() << "CServiceParameters::ReadOvenParameters:### attribute <RoomTempHigh> is missing => abort reading";
        return false;
    }
    OvenParameters.RoomTempHigh = XmlStreamReader.attributes().value("RoomTempHigh").toString();

    if (!XmlStreamReader.attributes().hasAttribute("HTime1")) {
        qDebug() << "CServiceParameters::ReadOvenParameters:### attribute <HTime1> is missing => abort reading";
        return false;
    }
    OvenParameters.HTime1 = XmlStreamReader.attributes().value("HTime1").toString();

    if (!XmlStreamReader.attributes().hasAttribute("HTime2")) {
        qDebug() << "CServiceParameters::ReadOvenParameters:### attribute <HTime2> is missing => abort reading";
        return false;
    }
    OvenParameters.HTime2 = XmlStreamReader.attributes().value("HTime2").toString();

    if (!XmlStreamReader.attributes().hasAttribute("TempDiff")) {
        qDebug() << "CServiceParameters::ReadOvenParameters:### attribute <TempDiff> is missing => abort reading";
        return false;
    }
    OvenParameters.TempDiff = XmlStreamReader.attributes().value("TempDiff").toString();

    if (!XmlStreamReader.attributes().hasAttribute("OvenMaxLow")) {
        qDebug() << "CServiceParameters::ReadOvenParameters:### attribute <OvenMaxLow> is missing => abort reading";
        return false;
    }
    OvenParameters.OvenMaxLow = XmlStreamReader.attributes().value("OvenMaxLow").toString();

    if (!XmlStreamReader.attributes().hasAttribute("OvenMaxHigh")) {
        qDebug() << "CServiceParameters::ReadOvenParameters:### attribute <OvenMaxHigh> is missing => abort reading";
        return false;
    }
    OvenParameters.OvenMaxHigh = XmlStreamReader.attributes().value("OvenMaxHigh").toString();

    SetOvenParamters(OvenParameters);

    return true;
}

/****************************************************************************/
/*!
 *  \brief Reads Cuvettes parameters from xml
 *  \iparam XmlStreamReader = Xml reader to read the XML contents
 *  \return True or False
 */
/****************************************************************************/
bool CServiceParameters::ReadCuvettesParameters(QXmlStreamReader &XmlStreamReader)
{
    HeatedCuvetteParameters_t HCParameters;
    if (!XmlStreamReader.attributes().hasAttribute("MaxDiff")) {
        qDebug() << "CServiceParameters::ReadCuvettesParameters:### attribute <MaxDiff> is missing => abort reading";
        return false;
    }
    HCParameters.MaxDiff = XmlStreamReader.attributes().value("MaxDiff").toString();

    if (!XmlStreamReader.attributes().hasAttribute("RoomTempLow")) {
        qDebug() << "CServiceParameters::ReadCuvettesParameters:### attribute <RoomTempLow> is missing => abort reading";
        return false;
    }
    HCParameters.RoomTempLow = XmlStreamReader.attributes().value("RoomTempLow").toString();

    if (!XmlStreamReader.attributes().hasAttribute("RoomTempHigh")) {
        qDebug() << "CServiceParameters::ReadCuvettesParameters:### attribute <RoomTempHigh> is missing => abort reading";
        return false;
    }
    HCParameters.RoomTempHigh = XmlStreamReader.attributes().value("RoomTempHigh").toString();

    if (!XmlStreamReader.attributes().hasAttribute("HTime1")) {
        qDebug() << "CServiceParameters::ReadCuvettesParameters:### attribute <HTime1> is missing => abort reading";
        return false;
    }
    HCParameters.HTime1 = XmlStreamReader.attributes().value("HTime1").toString();

    if (!XmlStreamReader.attributes().hasAttribute("HTime2")) {
        qDebug() << "CServiceParameters::ReadCuvettesParameters:### attribute <HTime2> is missing => abort reading";
        return false;
    }
    HCParameters.HTime2 = XmlStreamReader.attributes().value("HTime2").toString();

    if (!XmlStreamReader.attributes().hasAttribute("TempDiff")) {
        qDebug() << "CServiceParameters::ReadCuvettesParameters:### attribute <TempDiff> is missing => abort reading";
        return false;
    }
    HCParameters.TempDiff = XmlStreamReader.attributes().value("TempDiff").toString();

    if (!XmlStreamReader.attributes().hasAttribute("OvenMaxLow")) {
        qDebug() << "CServiceParameters::ReadCuvettesParameters:### attribute <OvenMaxLow> is missing => abort reading";
        return false;
    }
    HCParameters.OvenMaxLow = XmlStreamReader.attributes().value("OvenMaxLow").toString();

    if (!XmlStreamReader.attributes().hasAttribute("OvenMaxHigh")) {
        qDebug() << "CServiceParameters::ReadCuvettesParameters:### attribute <OvenMaxHigh> is missing => abort reading";
        return false;
    }
    HCParameters.OvenMaxHigh = XmlStreamReader.attributes().value("OvenMaxHigh").toString();

    SetHeatedCuvetteParamters(HCParameters);

    return true;
}

/****************************************************************************/
/*!
 *  \brief Reads Test report folder path from xml
 *  \iparam XmlStreamReader = Xml reader to read the XML contents
 *  \return True or False
 */
/****************************************************************************/
bool CServiceParameters::ReadTestReportFolderPath(QXmlStreamReader &XmlStreamReader)
{
    if (!XmlStreamReader.attributes().hasAttribute("TestReportFolderPath")) {
        qDebug() << "CServiceParameters::ReadTestReportFolderPath:### attribute <TestReportFolderPath> is missing => abort reading";
        return false;
    }
    SetTestReportFolderPath(XmlStreamReader.attributes().value("TestReportFolderPath").toString());

    return true;
}

/****************************************************************************/
/*!
 *  \brief Reads firmware folder path from xml
 *  \iparam XmlStreamReader = Xml reader to read the XML contents
 *  \return True or False
 */
/****************************************************************************/
bool CServiceParameters::ReadFirmwareFolderPath(QXmlStreamReader &XmlStreamReader)
{
    if (!XmlStreamReader.attributes().hasAttribute("FirmwareFolderPath")) {
        qDebug() << "CServiceParameters::ReadFirmwareFolderPath:### attribute <FirmwareFolderPath> is missing => abort reading";
        return false;
    }
    SetFirmwareFolderPath(XmlStreamReader.attributes().value("FirmwareFolderPath").toString());

    return true;
}

/****************************************************************************/
/*!
 *  \brief Reads firmware file names from xml
 *  \iparam XmlStreamReader = Xml reader to read the XML contents
 *  \return True or False
 */
/****************************************************************************/
bool CServiceParameters::ReadFirmwareFileNames(QXmlStreamReader &XmlStreamReader)
{
    FirmwareFileNames_t FirmwareFileNames;
    if (!XmlStreamReader.attributes().hasAttribute("ASB_1_Filename")) {
        qDebug() << "CServiceParameters::ReadFirmwareFileNames:### attribute <ASB_1_Filename> is missing => abort reading";
        return false;
    }
    FirmwareFileNames.ASB_1_Filename = XmlStreamReader.attributes().value("ASB_1_Filename").toString();

    if (!XmlStreamReader.attributes().hasAttribute("ASB_2_Filename")) {
        qDebug() << "CServiceParameters::ReadFirmwareFileNames:### attribute <ASB_2_Filename> is missing => abort reading";
        return false;
    }
    FirmwareFileNames.ASB_2_Filename = XmlStreamReader.attributes().value("ASB_2_Filename").toString();

    if (!XmlStreamReader.attributes().hasAttribute("ASB_3_Filename")) {
        qDebug() << "CServiceParameters::ReadFirmwareFileNames:### attribute <ASB_3_Filename> is missing => abort reading";
        return false;
    }
    FirmwareFileNames.ASB_3_Filename = XmlStreamReader.attributes().value("ASB_3_Filename").toString();

    if (!XmlStreamReader.attributes().hasAttribute("ASB_4_Filename")) {
        qDebug() << "CServiceParameters::ReadFirmwareFileNames:### attribute <ASB_4_Filename> is missing => abort reading";
        return false;
    }
    FirmwareFileNames.ASB_4_Filename = XmlStreamReader.attributes().value("ASB_4_Filename").toString();

    if (!XmlStreamReader.attributes().hasAttribute("ASB_5_Filename")) {
        qDebug() << "CServiceParameters::ReadFirmwareFileNames:### attribute <ASB_5_Filename> is missing => abort reading";
        return false;
    }
    FirmwareFileNames.ASB_5_Filename = XmlStreamReader.attributes().value("ASB_5_Filename").toString();

    if (!XmlStreamReader.attributes().hasAttribute("ASB_a_Filename")) {
        qDebug() << "CServiceParameters::ReadFirmwareFileNames:### attribute <ASB_a_Filename> is missing => abort reading";
        return false;
    }
    FirmwareFileNames.ASB_a_Filename = XmlStreamReader.attributes().value("ASB_a_Filename").toString();

    if (!XmlStreamReader.attributes().hasAttribute("ASB_b_Filename")) {
        qDebug() << "CServiceParameters::ReadFirmwareFileNames:### attribute <ASB_b_Filename> is missing => abort reading";
        return false;
    }
    FirmwareFileNames.ASB_b_Filename = XmlStreamReader.attributes().value("ASB_b_Filename").toString();

    if (!XmlStreamReader.attributes().hasAttribute("ASB_12_Filename")) {
        qDebug() << "CServiceParameters::ReadFirmwareFileNames:### attribute <ASB_12_Filename> is missing => abort reading";
        return false;
    }
    FirmwareFileNames.ASB_12_Filename = XmlStreamReader.attributes().value("ASB_12_Filename").toString();

    if (!XmlStreamReader.attributes().hasAttribute("ASB_13_Filename")) {
        qDebug() << "CServiceParameters::ReadFirmwareFileNames:### attribute <ASB_13_Filename> is missing => abort reading";
        return false;
    }
    FirmwareFileNames.ASB_13_Filename = XmlStreamReader.attributes().value("ASB_13_Filename").toString();

    SetFirmwareFilenames(FirmwareFileNames);

    return true;
}

/****************************************************************************/
/*!
 *  \brief Reads module numbers from xml
 *  \iparam XmlStreamReader = Xml reader to read the XML contents
 *  \return True or False
 */
/****************************************************************************/
bool CServiceParameters::ReadModuleNumbers(QXmlStreamReader &XmlStreamReader)
{
    ModuleNumbers_t ModuleNumbers;
    if (!XmlStreamReader.attributes().hasAttribute("DrawerLeft")) {
        qDebug() << "CServiceParameters::ReadModuleNumbers:### attribute <DrawerLeft> is missing => abort reading";
        return false;
    }
    ModuleNumbers.DrawerLeft = XmlStreamReader.attributes().value("DrawerLeft").toString();

    if (!XmlStreamReader.attributes().hasAttribute("DrawerRight")) {
        qDebug() << "CServiceParameters::ReadModuleNumbers:### attribute <DrawerRight> is missing => abort reading";
        return false;
    }
    ModuleNumbers.DrawerRight = XmlStreamReader.attributes().value("DrawerRight").toString();

    if (!XmlStreamReader.attributes().hasAttribute("Oven")) {
        qDebug() << "CServiceParameters::ReadModuleNumbers:### attribute <Oven> is missing => abort reading";
        return false;
    }
    ModuleNumbers.Oven = XmlStreamReader.attributes().value("Oven").toString();

    if (!XmlStreamReader.attributes().hasAttribute("XArmLeft")) {
        qDebug() << "CServiceParameters::ReadModuleNumbers:### attribute <XArmLeft> is missing => abort reading";
        return false;
    }
    ModuleNumbers.XArmLeft = XmlStreamReader.attributes().value("XArmLeft").toString();

    if (!XmlStreamReader.attributes().hasAttribute("XArmRight")) {
        qDebug() << "CServiceParameters::ReadModuleNumbers:### attribute <XArmRight> is missing => abort reading";
        return false;
    }
    ModuleNumbers.XArmRight = XmlStreamReader.attributes().value("XArmRight").toString();

    if (!XmlStreamReader.attributes().hasAttribute("XAxis")) {
        qDebug() << "CServiceParameters::ReadModuleNumbers:### attribute <XAxis> is missing => abort reading";
        return false;
    }
    ModuleNumbers.XAxis = XmlStreamReader.attributes().value("XAxis").toString();

    if (!XmlStreamReader.attributes().hasAttribute("RackTransfer")) {
        qDebug() << "CServiceParameters::ReadModuleNumbers:### attribute <RackTransfer> is missing => abort reading";
        return false;
    }
    ModuleNumbers.TransferStation = XmlStreamReader.attributes().value("RackTransfer").toString();

    if (!XmlStreamReader.attributes().hasAttribute("E-Box")) {
        qDebug() << "CServiceParameters::ReadModuleNumbers:### attribute <E-Box> is missing => abort reading";
        return false;
    }
    ModuleNumbers.E_Box = XmlStreamReader.attributes().value("E-Box").toString();

    if (!XmlStreamReader.attributes().hasAttribute("AirExhaust")) {
        qDebug() << "CServiceParameters::ReadModuleNumbers:### attribute <AirExhaust> is missing => abort reading";
        return false;
    }
    ModuleNumbers.AirExhaust = XmlStreamReader.attributes().value("AirExhaust").toString();

    if (!XmlStreamReader.attributes().hasAttribute("WaterStation")) {
        qDebug() << "CServiceParameters::ReadModuleNumbers:### attribute <WaterStation> is missing => abort reading";
        return false;
    }
    ModuleNumbers.WaterStation = XmlStreamReader.attributes().value("WaterStation").toString();

    if (!XmlStreamReader.attributes().hasAttribute("HeatedCuvettes")) {
        qDebug() << "CServiceParameters::ReadModuleNumbers:### attribute <HeatedCuvettes> is missing => abort reading";
        return false;
    }
    ModuleNumbers.HeatedCuvettes = XmlStreamReader.attributes().value("HeatedCuvettes").toString();

    if (!XmlStreamReader.attributes().hasAttribute("Agitation")) {
        qDebug() << "CServiceParameters::ReadModuleNumbers:### attribute <Agitation> is missing => abort reading";
        return false;
    }
    ModuleNumbers.Agitator = XmlStreamReader.attributes().value("Agitation").toString();

    SetModuleNumbers(ModuleNumbers);

    return true;
}

/****************************************************************************/
/*!
 *  \brief Reads Calibration offsets from xml
 *  \iparam XmlStreamReader = Xml reader to read the XML contents
 *  \return True or False
 */
/****************************************************************************/
bool CServiceParameters::ReadCalibrationOffsets(QXmlStreamReader &XmlStreamReader)
{
    CalibrationOffsets_t CalibrationOffsets;
    if (!XmlStreamReader.attributes().hasAttribute("OvenOpenPos")) {
        qDebug() << "CServiceParameters::ReadCalibrationOffsets:### attribute <OvenOpenPos> is missing => abort reading";
        return false;
    }
    CalibrationOffsets.OvenOpenPos = XmlStreamReader.attributes().value("OvenOpenPos").toString();

    if (!XmlStreamReader.attributes().hasAttribute("OvenClosePos")) {
        qDebug() << "CServiceParameters::ReadCalibrationOffsets:### attribute <OvenClosePos> is missing => abort reading";
        return false;
    }
    CalibrationOffsets.OvenClosePos = XmlStreamReader.attributes().value("OvenClosePos").toString();

    if (!XmlStreamReader.attributes().hasAttribute("DrawerOpenPos")) {
        qDebug() << "CServiceParameters::ReadCalibrationOffsets:### attribute <DrawerOpenPos> is missing => abort reading";
        return false;
    }
    CalibrationOffsets.DrawerOpenPos = XmlStreamReader.attributes().value("DrawerOpenPos").toString();

    if (!XmlStreamReader.attributes().hasAttribute("DrawerClosePos")) {
        qDebug() << "CServiceParameters::ReadCalibrationOffsets:### attribute <DrawerClosePos> is missing => abort reading";
        return false;
    }
    CalibrationOffsets.DrawerClosePos = XmlStreamReader.attributes().value("DrawerClosePos").toString();

    if (!XmlStreamReader.attributes().hasAttribute("StandardOffset")) {
        qDebug() << "CServiceParameters::ReadCalibrationOffsets:### attribute <StandardOffset> is missing => abort reading";
        return false;
    }
    CalibrationOffsets.StandardOffset = XmlStreamReader.attributes().value("StandardOffset").toString();

    SetCalibrationOffsets(CalibrationOffsets);

    return true;
}

/****************************************************************************/
/*!
 *  \brief Reads Calibration parameters from xml
 *  \iparam XmlStreamReader = Xml reader to read the XML contents
 *  \return True or False
 */
/****************************************************************************/
bool CServiceParameters::ReadCalibrationParameters(QXmlStreamReader &XmlStreamReader)
{
    CalibrationParameters_t CalibrationParameters;
    if (!XmlStreamReader.attributes().hasAttribute("AgitationWidth12")) {
        qDebug() << "CServiceParameters::ReadCalibrationParameters:### attribute <AgitationWidth12> is missing => abort reading";
        return false;
    }
    CalibrationParameters.AgitationWidth12 = XmlStreamReader.attributes().value("AgitationWidth12").toString();

    if (!XmlStreamReader.attributes().hasAttribute("AgitationWidth21")) {
        qDebug() << "CServiceParameters::ReadCalibrationParameters:### attribute <AgitationWidth21> is missing => abort reading";
        return false;
    }
    CalibrationParameters.AgitationWidth21 = XmlStreamReader.attributes().value("AgitationWidth21").toString();

    if (!XmlStreamReader.attributes().hasAttribute("MinDiff03")) {
        qDebug() << "CServiceParameters::ReadCalibrationParameters:### attribute <MinDiff03> is missing => abort reading";
        return false;
    }
    CalibrationParameters.MinDiff03 = XmlStreamReader.attributes().value("MinDiff03").toString();

    SetCalibrationParameters(CalibrationParameters);

    return true;
}

/****************************************************************************/
/*!
 *  \brief Reads firmware folder path from xml
 *  \iparam XmlStreamReader = Xml reader to read the XML contents
 *  \return True or False
 */
/****************************************************************************/
bool CServiceParameters::ReadSystemLogOffTime(QXmlStreamReader &XmlStreamReader)
{
    if (!XmlStreamReader.attributes().hasAttribute("TimeInMSec")) {
        qDebug() << "CServiceParameters::ReadSystemLogOffTime:### attribute <TimeInMSec> is missing => abort reading";
        return false;
    }
    SetSystemLogOffTime(XmlStreamReader.attributes().value("TimeInMSec").toString().toInt());

    return true;
}

/****************************************************************************/
/*!
 *  \brief Assignment Operator which copies from rhs to lhs.
 *  \iparam ServiceParameters = CServiceParameters class object
 *  \return CServiceParameters Class Object
 */
/****************************************************************************/
CServiceParameters& CServiceParameters::operator=(const CServiceParameters& ServiceParameters)
{
    if (this != &ServiceParameters) {

        int Version = const_cast<CServiceParameters&>(ServiceParameters).GetVersion();
        QString ProxyUserName = const_cast<CServiceParameters&>(ServiceParameters).GetProxyUserName();
        QString ProxyPassword = const_cast<CServiceParameters&>(ServiceParameters).GetProxyPassword();
        QString ProxyIPAddress = const_cast<CServiceParameters&>(ServiceParameters).GetProxyIPAddress();
        int ProxyIPPort = const_cast<CServiceParameters&>(ServiceParameters).GetProxyIPPort();
        QString YZModuleRfid = const_cast<CServiceParameters&>(ServiceParameters).GetYZModuleRFID();
        QString FileName = const_cast<CServiceParameters&>(ServiceParameters).GetFilename();
        bool VerificationMode = const_cast<CServiceParameters&>(ServiceParameters).GetDataVerificationMode();        
        QString TestReportFolderPath = const_cast<CServiceParameters&>(ServiceParameters).GetTestReportFolderPath();
        QString FirmwareFolderPath = const_cast<CServiceParameters&>(ServiceParameters).GetFirmwareFolderPath();
        int SystemLogOffTime = const_cast<CServiceParameters&>(ServiceParameters).GetSystemLogOffTime();

        LoaderRFID_t Rfid;
        Rfid.Rack1 = ServiceParameters.mp_LoaderRfid->Rack1;
        Rfid.Rack2 = ServiceParameters.mp_LoaderRfid->Rack2;
        Rfid.Rack3 = ServiceParameters.mp_LoaderRfid->Rack3;
        Rfid.Rack4 = ServiceParameters.mp_LoaderRfid->Rack4;
        Rfid.Rack5 = ServiceParameters.mp_LoaderRfid->Rack5;

        UnloaderRFID_t URfid;
        URfid.Rack1 = ServiceParameters.mp_UnloaderRfid->Rack1;
        URfid.Rack2 = ServiceParameters.mp_UnloaderRfid->Rack2;
        URfid.Rack3 = ServiceParameters.mp_UnloaderRfid->Rack3;
        URfid.Rack4 = ServiceParameters.mp_UnloaderRfid->Rack4;
        URfid.Rack5 = ServiceParameters.mp_UnloaderRfid->Rack5;

        XArmLeftParameters_t XArmLeftParameters;
        XArmLeftParameters.Filledx = ServiceParameters.mp_XArmLeftParameters->Filledx;
        XArmLeftParameters.Filledy = ServiceParameters.mp_XArmLeftParameters->Filledy;
        XArmLeftParameters.Emptyx = ServiceParameters.mp_XArmLeftParameters->Emptyx;
        XArmLeftParameters.Emptyy  = ServiceParameters.mp_XArmLeftParameters->Emptyy;
        XArmLeftParameters.Rackx = ServiceParameters.mp_XArmLeftParameters->Rackx;
        XArmLeftParameters.Racky = ServiceParameters.mp_XArmLeftParameters->Racky;

        XArmRightParameters_t XArmRightParameters;
        XArmRightParameters.Filledx = ServiceParameters.mp_XArmRightParameters->Filledx;
        XArmRightParameters.Filledy = ServiceParameters.mp_XArmRightParameters->Filledy;
        XArmRightParameters.Emptyx = ServiceParameters.mp_XArmRightParameters->Emptyx;
        XArmRightParameters.Emptyy  = ServiceParameters.mp_XArmRightParameters->Emptyy;
        XArmRightParameters.Rackx = ServiceParameters.mp_XArmRightParameters->Rackx;
        XArmRightParameters.Racky = ServiceParameters.mp_XArmRightParameters->Racky;

        OvenParameters_t OvenParamaters;
        OvenParamaters.MaxDiff = ServiceParameters.mp_OvenParameters->MaxDiff;
        OvenParamaters.RoomTempLow = ServiceParameters.mp_OvenParameters->RoomTempLow;
        OvenParamaters.RoomTempHigh = ServiceParameters.mp_OvenParameters->RoomTempHigh;
        OvenParamaters.HTime1 = ServiceParameters.mp_OvenParameters->HTime1;
        OvenParamaters.HTime2 = ServiceParameters.mp_OvenParameters->HTime2;
        OvenParamaters.OvenMaxLow = ServiceParameters.mp_OvenParameters->OvenMaxLow;
        OvenParamaters.OvenMaxHigh = ServiceParameters.mp_OvenParameters->OvenMaxHigh;
        OvenParamaters.TempDiff = ServiceParameters.mp_OvenParameters->TempDiff;

        HeatedCuvetteParameters_t HCParamaters;
        HCParamaters.MaxDiff = ServiceParameters.mp_HCParameters->MaxDiff;
        HCParamaters.RoomTempLow = ServiceParameters.mp_HCParameters->RoomTempLow;
        HCParamaters.RoomTempHigh = ServiceParameters.mp_HCParameters->RoomTempHigh;
        HCParamaters.HTime1 = ServiceParameters.mp_HCParameters->HTime1;
        HCParamaters.HTime2 = ServiceParameters.mp_HCParameters->HTime2;
        HCParamaters.OvenMaxLow = ServiceParameters.mp_HCParameters->OvenMaxLow;
        HCParamaters.OvenMaxHigh = ServiceParameters.mp_HCParameters->OvenMaxHigh;
        HCParamaters.TempDiff = ServiceParameters.mp_HCParameters->TempDiff;

        FirmwareFileNames_t FirmwareFileNames;
        FirmwareFileNames.ASB_1_Filename = ServiceParameters.mp_FirmwareFilenames->ASB_1_Filename;
        FirmwareFileNames.ASB_2_Filename = ServiceParameters.mp_FirmwareFilenames->ASB_2_Filename;
        FirmwareFileNames.ASB_3_Filename = ServiceParameters.mp_FirmwareFilenames->ASB_3_Filename;
        FirmwareFileNames.ASB_4_Filename = ServiceParameters.mp_FirmwareFilenames->ASB_4_Filename;
        FirmwareFileNames.ASB_5_Filename = ServiceParameters.mp_FirmwareFilenames->ASB_5_Filename;
        FirmwareFileNames.ASB_a_Filename = ServiceParameters.mp_FirmwareFilenames->ASB_a_Filename;
        FirmwareFileNames.ASB_b_Filename = ServiceParameters.mp_FirmwareFilenames->ASB_b_Filename;
        FirmwareFileNames.ASB_12_Filename = ServiceParameters.mp_FirmwareFilenames->ASB_12_Filename;
        FirmwareFileNames.ASB_13_Filename = ServiceParameters.mp_FirmwareFilenames->ASB_13_Filename;

        ModuleNumbers_t ModuleNumbers;
        ModuleNumbers.DrawerLeft = ServiceParameters.mp_ModuleNumbers->DrawerLeft;
        ModuleNumbers.DrawerRight = ServiceParameters.mp_ModuleNumbers->DrawerRight;
        ModuleNumbers.Oven = ServiceParameters.mp_ModuleNumbers->Oven;
        ModuleNumbers.XArmLeft = ServiceParameters.mp_ModuleNumbers->XArmLeft;
        ModuleNumbers.XArmRight = ServiceParameters.mp_ModuleNumbers->XArmRight;
        ModuleNumbers.XAxis = ServiceParameters.mp_ModuleNumbers->XAxis;
        ModuleNumbers.TransferStation = ServiceParameters.mp_ModuleNumbers->TransferStation;
        ModuleNumbers.E_Box = ServiceParameters.mp_ModuleNumbers->E_Box;
        ModuleNumbers.AirExhaust = ServiceParameters.mp_ModuleNumbers->AirExhaust;
        ModuleNumbers.WaterStation = ServiceParameters.mp_ModuleNumbers->WaterStation;
        ModuleNumbers.HeatedCuvettes = ServiceParameters.mp_ModuleNumbers->HeatedCuvettes;
        ModuleNumbers.Agitator = ServiceParameters.mp_ModuleNumbers->Agitator;

        CalibrationOffsets_t CalibrationOffsets;
        CalibrationOffsets.OvenOpenPos = ServiceParameters.mp_CalibrationOffsets->OvenOpenPos;
        CalibrationOffsets.OvenClosePos = ServiceParameters.mp_CalibrationOffsets->OvenClosePos;
        CalibrationOffsets.DrawerOpenPos = ServiceParameters.mp_CalibrationOffsets->DrawerOpenPos;
        CalibrationOffsets.DrawerClosePos = ServiceParameters.mp_CalibrationOffsets->DrawerClosePos;
        CalibrationOffsets.StandardOffset = ServiceParameters.mp_CalibrationOffsets->StandardOffset;

        CalibrationParameters_t CalibrationParameters;
        CalibrationParameters.AgitationWidth12 = ServiceParameters.mp_CalibrationParameters->AgitationWidth12;
        CalibrationParameters.AgitationWidth21 = ServiceParameters.mp_CalibrationParameters->AgitationWidth21;
        CalibrationParameters.MinDiff03 = ServiceParameters.mp_CalibrationParameters->MinDiff03;

        this->SetVerion(Version);
        this->SetProxyUserName(ProxyUserName);
        this->SetProxyPassword(ProxyPassword);
        this->SetProxyIPAddress(ProxyIPAddress);
        this->SetProxyIPPort(ProxyIPPort);
        this->SetYZModuleRFID(YZModuleRfid);
        this->SetFilename(FileName);
        this->SetDataVerificationMode(VerificationMode);
        this->SetLoaderRFID(Rfid);
        this->SetUnloaderRFID(URfid);
        this->SetXArmLeftParamters(XArmLeftParameters);
        this->SetXArmRightParamters(XArmRightParameters);
        this->SetOvenParamters(OvenParamaters);
        this->SetHeatedCuvetteParamters(HCParamaters);
        this->SetTestReportFolderPath(TestReportFolderPath);
        this->SetFirmwareFolderPath(FirmwareFolderPath);
        this->SetFirmwareFilenames(FirmwareFileNames);
        this->SetModuleNumbers(ModuleNumbers);
        this->SetCalibrationOffsets(CalibrationOffsets);
        this->SetCalibrationParameters(CalibrationParameters);
        this->SetSystemLogOffTime(SystemLogOffTime);

    }
    return *this;
}

} // end of namespace DataManager
