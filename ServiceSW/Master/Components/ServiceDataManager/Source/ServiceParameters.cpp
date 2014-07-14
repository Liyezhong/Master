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
    m_ProxyIPAddress("000.000.000.001"),
    m_ServerIPAddress("000.000.000.002"),
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
    m_ProxyIPAddress("000.000.000.001"),
    m_ServerIPAddress("000.000.000.002"),
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
    XmlStreamWriter.writeAttribute("ProxyIPAddress", GetProxyIPAddress());
    XmlStreamWriter.writeAttribute("ServerIPAddress", GetServerIPAddress());
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

    if (!XmlStreamReader.attributes().hasAttribute("ProxyIPAddress")) {
        qDebug() << "CServiceParameters::ReadNetworkSettings:### attribute <ProxyIPAddress> is missing => abort reading";
        return false;
    }
    SetProxyIPAddress(XmlStreamReader.attributes().value("ProxyIPAddress").toString());

    if (!XmlStreamReader.attributes().hasAttribute("ServerIPAddress")) {
        qDebug() << "CServiceParameters::ReadNetworkSettings:### attribute <ServerIPAddress> is missing => abort reading";
        return false;
    }
    SetServerIPAddress(XmlStreamReader.attributes().value("ServerIPAddress").toString());

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
        QString ProxyIPAddress  = const_cast<CServiceParameters&>(ServiceParameters).GetProxyIPAddress();
        QString ServerIPAddress = const_cast<CServiceParameters&>(ServiceParameters).GetServerIPAddress();

        this->SetVerion(Version);
        this->SetProxyIPAddress(ProxyIPAddress);
        this->SetServerIPAddress(ServerIPAddress);
    }
    return *this;
}

} // end of namespace DataManager
