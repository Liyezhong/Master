/****************************************************************************/
/*! \file ProgramSettings.h
 *
 *  \brief Definition file for class CProgramSettings.
 *  This class is implemented to read Process Settings information from the XML.
 *  Reads all profile and position information, stores in a container (QHash)
 *  and also writes data from container to XML.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-04-02
 *  $Author:    $ L. Yang
 *
 *  \b Company:
 *
 *       Leica Biosystems SH CN
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DATAMANAGER_PRGRAMSETTINGS_H
#define DATAMANAGER_PRGRAMSETTINGS_H

#include <QHash>
#include <QMap>
#include <QIODevice>
#include <QReadWriteLock>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QString>

#include "Global/Include/Translator.h"
#include "DataManager/Helper/Include/Types.h"
#include "DataManager/Helper/Include/Helper.h"
#include "DataManager/Containers/ContainerBase/Include/DataContainerBase.h"
#include "DataManager/Containers/ContainerBase/Include/VerifierInterface.h"

namespace DataManager
{
typedef QMap<QString,QString> ParameterKeyValue_t;
typedef QMap<QString,ParameterKeyValue_t> FunctionParameter_t;
typedef QMap<QString,FunctionParameter_t> DeviceFunction_t;

/****************************************************************************/
/*!
 *  \brief  This class implements Group List
 *  Reading all Group information from XML and FunctionStoring in a Container
 */
/****************************************************************************/
class CProgramSettings: public CDataContainerBase
{
    friend class CProgramSettingsVerifier;

public:
    CProgramSettings();
    CProgramSettings(const CProgramSettings&);
    ~CProgramSettings();

    friend QDataStream& operator <<(QDataStream& OutDataStream, const CProgramSettings&  GroupList);
    friend QDataStream& operator >>(QDataStream& InDataStream, CProgramSettings& GroupList);
    CProgramSettings& operator = (const CProgramSettings&);

    void Init();

    /****************************************************************************/
    /*!
     *  \brief returns the  Data container type
     *  \return Data container type
     */
    /****************************************************************************/
    DataContainerType_t GetDataContainerType() {return PARAMETER;}

    /****************************************************************************/
    /*!
        *  \brief retrieves the status of Verification
        *  \return true-verification on , false-verification off
        */
    /****************************************************************************/
    bool GetDataVerificationMode() {return m_DataVerificationMode;}

    /****************************************************************************/
    /*!
        *  \brief sets verification mode
        *  \iparam Value = true-verification on, false - verification off
        */
    /****************************************************************************/
    void SetDataVerificationMode(const bool Value){m_DataVerificationMode = Value;}

    bool Read(QString Filename);

    /****************************************************************************/
    /*!
        *  \brief returns the version of the xml file read
        *  \return file version
        */
    /****************************************************************************/
    int GetVersion() {return m_Version;}

    /****************************************************************************/
    /*!
     *  \brief returns the filename of the xml file read
     *  \return file name
     */
    /****************************************************************************/
    QString GetFilename() {return m_Filename;}

    /****************************************************************************/
    /*!
     *  \brief get parameter's value
     *  \return parameter value
     */
    /****************************************************************************/
    QString GetParameterValue(QString DeviceKey, QString FunctionKey,QString ParameterKey, bool& ok);


private:
    int m_Version;  //!<  version of the file read
    bool m_DataVerificationMode;//!< Verification mode flag , verify the Container
    
    QString m_Filename;//!< filename of the XML file to read

    DeviceFunction_t m_Parameters;

    QReadWriteLock* mp_ReadWriteLock; //!< Lock for thread safety

    ErrorHash_t m_ErrorMap;    //!< Event List for GUI and for logging purpose. This member is not copied when using copy constructor/Assignment operator
    
    bool SerializeContent(QIODevice& IODevice, bool CompleteData);

    bool DeserializeContent(QIODevice& IODevice, bool CompleteData);

    bool ReadAllParameters(QXmlStreamReader& XmlStreamReader);
    bool ReadDeviceParameters(QXmlStreamReader& XmlStreamReader, QString DeviceKey);
    bool ReadFunctionParameters(QXmlStreamReader &XmlStreamReader, QString DeviceKey, QString FunctionKey);


    bool WriteAllParameters(QXmlStreamWriter& XmlStreamWriter);

    bool DeleteAll();

    /****************************************************************************/
    /*!
     *  \brief sets the xml version
     *  \iparam Value = Version to set
     */
    /****************************************************************************/
    void SetVersion(const int Value)    {m_Version = Value;}

    /****************************************************************************/
    /*!
     *  \brief Sets the XML file name
     *  \iparam Value = Filename to set
     */
    /****************************************************************************/
    void SetFilename(const QString Value) {m_Filename = Value;}
};


}   // namespace DataManager

#endif // DATAMANAGER_PRGRAMSETTINGS_H
