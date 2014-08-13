/****************************************************************************/
/*! \file ServiceParameters.h
 *
 *  \brief Definition file for class CServiceParameters.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-06-26
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

#ifndef SERVICEDATACONTAINER_SERVICEPARAMETERS_H
#define SERVICEDATACONTAINER_SERVICEPARAMETERS_H

#include <QString>
#include <QHash>
#include <QReadWriteLock>
#include <QDateTime>
#include <QIODevice>
#include <QList>

#include "DataManager/Containers/ContainerBase/Include/DataContainerBase.h"
#include "DataManager/Helper/Include/Types.h"

namespace DataManager {

//! Structure for drawer loader rack RFID's
typedef struct {
    QString Rack1;  //!< Rack1 RFID
    QString Rack2;  //!< Rack2 RFID
    QString Rack3;  //!< Rack3 RFID
    QString Rack4;  //!< Rack4 RFID
    QString Rack5;  //!< Rack5 RFID
} LoaderRFID_t;     //!< Structure variable

//! Structure for drawer unloader rack RFID's
typedef struct {
    QString Rack1;  //!< Rack1 RFID
    QString Rack2;  //!< Rack2 RFID
    QString Rack3;  //!< Rack3 RFID
    QString Rack4;  //!< Rack4 RFID
    QString Rack5;  //!< Rack5 RFID
} UnloaderRFID_t;   //!< Structure variable

//! Structure for X Arm Left parameters
typedef struct {
    QString Filledx;      //!< Filled x value
    QString Filledy;      //!< Filled y value
    QString Emptyx;       //!< Empty x value
    QString Emptyy;       //!< Empty y value
    QString Rackx;        //!< Rack x value
    QString Racky;        //!< Rack y value
} XArmLeftParameters_t;   //!< Structure variable

//! Structure for X Arm Right parameters
typedef struct {
    QString Filledx;      //!< Filled x value
    QString Filledy;      //!< Filled y value
    QString Emptyx;       //!< Empty x value
    QString Emptyy;       //!< Empty y value
    QString Rackx;        //!< Rack x value
    QString Racky;        //!< Rack y value
} XArmRightParameters_t;  //!< Structure variable

//! Structure for Oven heating test parameters
typedef struct {
    QString MaxDiff;      //!< Maximum difference
    QString RoomTempLow;  //!< Minimum room temperature
    QString RoomTempHigh; //!< Maximum room temperature
    QString HTime1;       //!< Time 1
    QString HTime2;       //!< Time 2
    QString TempDiff;     //!< Temperature difference
    QString OvenMaxLow;   //!< Oven minimum temperature
    QString OvenMaxHigh;  //!< Oven Maximum temperature
} OvenParameters_t;       //!< Structure variable

//! Structure for Cuvette heating test parameters
typedef struct {
    QString MaxDiff;            //!< Maximum difference
    QString RoomTempLow;        //!< Minimum room temperature
    QString RoomTempHigh;       //!< Maximum room temperature
    QString HTime1;             //!< Time 1
    QString HTime2;             //!< Time 2
    QString TempDiff;           //!< Temperature difference
    QString OvenMaxLow;         //!< HC minimum temperature
    QString OvenMaxHigh;        //!< HC Maximum temperature
} HeatedCuvetteParameters_t;    //!< Structure variable

//! Structure for Firmware file names
typedef struct {
    QString ASB_1_Filename;     //!< ASB 1 file name
    QString ASB_2_Filename;     //!< ASB 2 file name
    QString ASB_3_Filename;     //!< ASB 3 file name
    QString ASB_4_Filename;     //!< ASB 4 file name
    QString ASB_5_Filename;     //!< ASB 5 file name
    QString ASB_a_Filename;     //!< ASB a file name
    QString ASB_b_Filename;     //!< ASB b file name
    QString ASB_12_Filename;    //!< ASB 12 file name
    QString ASB_13_Filename;    //!< ASB 13 file name
} FirmwareFileNames_t;          //!< Structure variable

//! Structure for storing module numbers.
typedef struct {
    QString DrawerLeft;         //!< drawer left module number
    QString DrawerRight;        //!< drawer right module number
    QString Oven;               //!< Oven module number
    QString XArmLeft;           //!< X Arm Left module number
    QString XArmRight;          //!< X Arm Right module number
    QString XAxis;              //!< X Axis module number
    QString TransferStation;    //!< Transfer station module number
    QString E_Box;              //!< Electronic box module number
    QString AirExhaust;         //!< Air Exhaust module number
    QString WaterStation;       //!< Water Station module number
    QString HeatedCuvettes;     //!< Heated cuvettes module number
    QString Agitator;           //!< Agitator module number
} ModuleNumbers_t;              //!< Structure variable

//! Structure for storing Calibration offset values
typedef struct {
    QString OvenOpenPos;        //!< Oven open position
    QString OvenClosePos;       //!< Oven close position
    QString DrawerOpenPos;      //!< Drawer open position
    QString DrawerClosePos;     //!< Drawer close position
    QString StandardOffset;     //!< Standard offset value
} CalibrationOffsets_t;

//! Structure for storing Calibration ofset values
typedef struct {
    QString AgitationWidth12;        //!< Agitation width
    QString AgitationWidth21;        //!< Agitation width
    QString MinDiff03;               //!< Min. diff
} CalibrationParameters_t;

/****************************************************************************/
/**
 * \brief CServiceParameters definition//    FileName = Global::SystemPaths::Instance().GetSettingsPath() + "/ServiceParameters.xml";
//    SetFilename(FileName);
 */
/****************************************************************************/
class CServiceParameters : public CDataContainerBase {

    friend class CTestServiceDataManager;
public:

    /****************************************************************************/
    /**
     * \brief Default constructor.
     */
    /****************************************************************************/
    CServiceParameters();

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CServiceParameters();

    /****************************************************************************/
    /**
     * \brief Copy constructor
     * \iparam ServiceParameters = CServiceParameters object
     */
    /****************************************************************************/
    CServiceParameters(const CServiceParameters&);              //!< Copy Constructor
    CServiceParameters& operator=(const CServiceParameters&);   //!< Assignment Operator Overloading

    /****************************************************************************/
    /**
     * \brief Sets Version number
     * \iparam Value = Value to set
     */
    /****************************************************************************/
    void SetVerion(int Value) {
        m_Version = Value;
    }

    /****************************************************************************/
    /**
     * \brief Gets Version number
     * \return Version number
     */
    /****************************************************************************/
    int GetVersion() {
        return m_Version;
    }

    /****************************************************************************/
    /**
     * \brief Sets proxy IP address
     * \iparam Value = Value to set
     */
    /****************************************************************************/
    void SetProxyIPAddress(QString Value) {
        m_ProxyIPAddress = Value;
    }

    /****************************************************************************/
    /**
     * \brief Gets proxy IP address
     * \return proxy IP address
     */
    /****************************************************************************/
    QString GetProxyIPAddress() {
        return m_ProxyIPAddress;
    }

    /****************************************************************************/
    /**
     * \brief Sets user name
     * \iparam Value = Value to user name
     */
    /****************************************************************************/
    void SetUserName(QString Value) {
        m_UserName = Value;
    }

    /****************************************************************************/
    /**
     * \brief Gets user name
     * \return user name
     */
    /****************************************************************************/
    QString GetUserName() {
        return m_UserName;
    }

    /****************************************************************************/
    /**
     * \brief Returns Test report folder path.
     * \return Test report folder path.
     */
    /****************************************************************************/
    QString GetTestReportFolderPath() {
        return m_TestReportFolderPath;
    }

    /****************************************************************************/
    /**
     * \brief Sets Test report folder path.
     * \iparam  Value = Test report folder path.
     */
    /****************************************************************************/
    void SetTestReportFolderPath(QString Value) {
        m_TestReportFolderPath = Value;
    }

    /****************************************************************************/
    /**
     * \brief Returns Firmware folder path.
     * \return Firmware folder path.
     */
    /****************************************************************************/
    QString GetFirmwareFolderPath() {
        return m_FirmwarefolderPath;
    }

    /****************************************************************************/
    /**
     * \brief Sets Firmware folder path.
     * \iparam Value = Firmware folder path.
     */
    /****************************************************************************/
    void SetFirmwareFolderPath(QString Value) {
        m_FirmwarefolderPath = Value;
    }

    /****************************************************************************/
    /**
     * \brief Sets file name
     * \iparam FileName = File Name to set
     */
    /****************************************************************************/
    void SetFilename(QString FileName) {
        m_FileName = FileName;
    }

    /****************************************************************************/
    /**
     * \brief Returns the Filename
     * \return File name
     */
    /****************************************************************************/
    QString GetFilename() {
        return m_FileName;
    }

    /****************************************************************************/
    /**
     * \brief Sets data verification mode
     * \iparam Value = Value to set
     */
    /****************************************************************************/
    void SetDataVerificationMode(bool Value) {
        m_DataVerificationMode = Value;
    }

    /****************************************************************************/
    /**
     * \brief Returns Data verification mode
     * \return True or False
     */
    /****************************************************************************/
    bool GetDataVerificationMode() {
        return m_DataVerificationMode;
    }

    /****************************************************************************/
    /**
     * \brief Returns the Data container type
     * \return Data container type
     */
    /****************************************************************************/
    DataContainerType_t GetDataContainerType() { return INSTRUMENTHISTORY; }

    /****************************************************************************/
    /**
     * \brief Reads the xml file
     * \iparam FileName = name of the file to read
     * \return True or False
     */
    /****************************************************************************/
    bool Read(QString FileName);

private:
    int m_Version;                                  //!< Stores the version number of the file
    QString m_ProxyIPAddress;                       //!< Proxy IP address
    QString m_UserName;                             //!< User name
    QString m_TestReportFolderPath;                 //!< Test report folder path
    QString m_FirmwarefolderPath;                   //!< Firmware folder path

    bool m_DataVerificationMode;                    //!< Store the Date verification mode flag
    QString m_FileName;                             //!< Store the file name

    /****************************************************************************/
    /*!
     *  \brief Writes the CModuleDataList Data to QIODevice
     *  \iparam IODevice = Instance of the IODevice might be Buffer or File
     *  \iparam CompleteData = bool type if true writes Complete data of object
     *  \return True or False
     */
    /****************************************************************************/
    bool SerializeContent(QIODevice& IODevice, bool CompleteData);

    /****************************************************************************/
    /*!
     *  \brief Reads the CModuleDataList Data from QIODevice
     *  \iparam IODevice = Instance of the IODevice - Buffer or File
     *  \iparam CompleteData = bool type if true writes Complete data of object
     *  \return True or False
     */
    /****************************************************************************/
    bool DeserializeContent(QIODevice& IODevice, bool CompleteData);

    /****************************************************************************/
    /*!
     *  \brief Reads Network settings from xml
     *  \iparam XmlStreamReader = Xml reader to read the XML contents
     *  \return True or False
     */
    /****************************************************************************/
    bool ReadNetworkSettings(QXmlStreamReader& XmlStreamReader);

    /****************************************************************************/
    /*!
     *  \brief Reads Test report folder path from xml
     *  \iparam XmlStreamReader = Xml reader to read the XML contents
     *  \return True or False
     */
    /****************************************************************************/
    bool ReadTestReportFolderPath(QXmlStreamReader& XmlStreamReader);

    /****************************************************************************/
    /*!
     *  \brief Reads firmware folder path from xml
     *  \iparam XmlStreamReader = Xml reader to read the XML contents
     *  \return True or False
     */
    /****************************************************************************/
    bool ReadFirmwareFolderPath(QXmlStreamReader& XmlStreamReader);
};


} // end of namespace DataManager

#endif // SERVICEDATACONTAINER_SERVICEPARAMETERS_H
