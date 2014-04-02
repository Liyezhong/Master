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
 * \brief CServiceParameters definition
 */
/****************************************************************************/
class CServiceParameters : public CDataContainerBase {

    friend class CTestServiceDataManager;
public:
    CServiceParameters();
    ~CServiceParameters();
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
     * \brief Sets proxy user name
     * \iparam Value = Value to set
     */
    /****************************************************************************/
    void SetProxyUserName(QString Value) {
        m_ProxyUserName = Value;
    }

    /****************************************************************************/
    /**
     * \brief Gets Proxy username
     * \return Proxy username
     */
    /****************************************************************************/
    QString GetProxyUserName() {
        return m_ProxyUserName;
    }

    /****************************************************************************/
    /**
     * \brief Sets proxy password
     * \iparam Value = Value to set
     */
    /****************************************************************************/
    void SetProxyPassword(QString Value) {
        m_ProxyPassword = Value;
    }

    /****************************************************************************/
    /**
     * \brief Gets Proxy password
     * \return Proxy password
     */
    /****************************************************************************/
    QString GetProxyPassword() {
        return m_ProxyPassword;
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
     * \brief Sets proxy IP port
     * \iparam Value = Value to set
     */
    /****************************************************************************/
    void SetProxyIPPort(int Value) {
        m_ProxyIPPort = Value;
    }

    /****************************************************************************/
    /**
     * \brief Gets proxy IP port
     * \return proxy IP port
     */
    /****************************************************************************/
    int GetProxyIPPort() {
        return m_ProxyIPPort;
    }

    /****************************************************************************/
    /**
     * \brief Sets Loader RFID values
     * \iparam Value = Value to set
     */
    /****************************************************************************/
    void SetLoaderRFID(LoaderRFID_t Value) {
        mp_LoaderRfid = new LoaderRFID_t;
        mp_LoaderRfid->Rack1 = Value.Rack1;
        mp_LoaderRfid->Rack2 = Value.Rack2;
        mp_LoaderRfid->Rack3 = Value.Rack3;
        mp_LoaderRfid->Rack4 = Value.Rack4;
        mp_LoaderRfid->Rack5 = Value.Rack5;
    }

    /****************************************************************************/
    /**
     * \brief Gets Loader RFID pointer
     * \return Loader RFID pointer
     */
    /****************************************************************************/
    LoaderRFID_t* GetLoaderRFID() {
        return mp_LoaderRfid;
    }

    /****************************************************************************/
    /**
     * \brief Sets Unloader RFID pointer
     * \iparam Value = Value to set
     */
    /****************************************************************************/
    void SetUnloaderRFID(UnloaderRFID_t Value) {
        mp_UnloaderRfid = new UnloaderRFID_t;
        mp_UnloaderRfid->Rack1 = Value.Rack1;
        mp_UnloaderRfid->Rack2 = Value.Rack2;
        mp_UnloaderRfid->Rack3 = Value.Rack3;
        mp_UnloaderRfid->Rack4 = Value.Rack4;
        mp_UnloaderRfid->Rack5 = Value.Rack5;
    }

    /****************************************************************************/
    /**
     * \brief Gets Unloader RFID pointer
     * \return Unloader RFID pointer
     */
    /****************************************************************************/
    UnloaderRFID_t* GetUnloaderRFID() {
         return mp_UnloaderRfid;
    }

    /****************************************************************************/
    /**
     * \brief Sets YZ module RFID
     * \iparam Value = Value to set
     */
    /****************************************************************************/
    void SetYZModuleRFID(QString Value) {
        m_YZModuleRfid = Value;
    }

    /****************************************************************************/
    /**
     * \brief Gets YZ module RFID
     * \return YZ module RFID
     */
    /****************************************************************************/
    QString GetYZModuleRFID() {
        return m_YZModuleRfid;
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

    /****************************************************************************/
    /**
     * \brief Returns X arm left parameters.
     * \return Structure of X-Arm left parameters.
     */
    /****************************************************************************/
    XArmLeftParameters_t* GetXArmLeftParameters() {
        return mp_XArmLeftParameters;
    }

    /****************************************************************************/
    /**
     * \brief Sets XArm left parameters.
     * \iparam Value = Structure of XArm left parameters.
     */
    /****************************************************************************/
    void SetXArmLeftParamters(XArmLeftParameters_t Value) {
        mp_XArmLeftParameters = new XArmLeftParameters_t;
        mp_XArmLeftParameters->Filledx = Value.Filledx;
        mp_XArmLeftParameters->Filledy = Value.Filledy;
        mp_XArmLeftParameters->Emptyx = Value.Emptyx;
        mp_XArmLeftParameters->Emptyy = Value.Emptyy;
        mp_XArmLeftParameters->Rackx = Value.Rackx;
        mp_XArmLeftParameters->Racky = Value.Racky;
    }

    /****************************************************************************/
    /**
     * \brief Returns X arm right parameters.
     * \return Structure of X-Arm right parameters.
     */
    /****************************************************************************/
    XArmRightParameters_t* GetXArmRightParameters() {
        return mp_XArmRightParameters;
    }

    /****************************************************************************/
    /**
     * \brief Sets XArm right parameters.
     * \iparam Value = Structure of XArm right parameters.
     */
    /****************************************************************************/
    void SetXArmRightParamters(XArmRightParameters_t Value) {
        mp_XArmRightParameters = new XArmRightParameters_t;
        mp_XArmRightParameters->Filledx = Value.Filledx;
        mp_XArmRightParameters->Filledy = Value.Filledy;
        mp_XArmRightParameters->Emptyx = Value.Emptyx;
        mp_XArmRightParameters->Emptyy = Value.Emptyy;
        mp_XArmRightParameters->Rackx = Value.Rackx;
        mp_XArmRightParameters->Racky = Value.Racky;
    }

    /****************************************************************************/
    /**
     * \brief Returns Oven parameters.
     * \return Structure of Oven parameters.
     */
    /****************************************************************************/
    OvenParameters_t* GetOvenParameters() {
        return mp_OvenParameters;
    }

    /****************************************************************************/
    /**
     * \brief Sets Oven parameters.
     * \iparam Value = Structure of Oven parameters.
     */
    /****************************************************************************/
    void SetOvenParamters(OvenParameters_t Value) {
       mp_OvenParameters = new OvenParameters_t;
       mp_OvenParameters->MaxDiff = Value.MaxDiff;
       mp_OvenParameters->RoomTempLow = Value.RoomTempLow;
       mp_OvenParameters->RoomTempHigh = Value.RoomTempHigh;
       mp_OvenParameters->HTime1 = Value.HTime1;
       mp_OvenParameters->HTime2 = Value.HTime2;
       mp_OvenParameters->TempDiff = Value.TempDiff;
       mp_OvenParameters->OvenMaxLow = Value.OvenMaxLow;
       mp_OvenParameters->OvenMaxHigh = Value.OvenMaxHigh;
    }

    /****************************************************************************/
    /**
     * \brief Returns Heated cuvette parameters.
     * \return Structure of Heated cuvette parameters.
     */
    /****************************************************************************/
    HeatedCuvetteParameters_t* GetHeatedCuvetteParameters() {
        return mp_HCParameters;
    }

    /****************************************************************************/
    /**
     * \brief Sets Heated Cuvette parameters.
     * \iparam Value = Structure of Heated Cuvette parameters.
     */
    /****************************************************************************/
    void SetHeatedCuvetteParamters(HeatedCuvetteParameters_t Value) {
        mp_HCParameters = new HeatedCuvetteParameters_t;
        mp_HCParameters->MaxDiff = Value.MaxDiff;
        mp_HCParameters->RoomTempLow = Value.RoomTempLow;
        mp_HCParameters->RoomTempHigh = Value.RoomTempHigh;
        mp_HCParameters->HTime1 = Value.HTime1;
        mp_HCParameters->HTime2 = Value.HTime2;
        mp_HCParameters->TempDiff = Value.TempDiff;
        mp_HCParameters->OvenMaxLow = Value.OvenMaxLow;
        mp_HCParameters->OvenMaxHigh = Value.OvenMaxHigh;
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
     * \brief Returns Firmware file names.
     * \return Structure of Firmware file names.
     */
    /****************************************************************************/
    FirmwareFileNames_t* GetFirmwareFilenames() {
        return mp_FirmwareFilenames;
    }

    /****************************************************************************/
    /**
     * \brief Sets Firmware file names.
     * \iparam Value = Structure of Firmware file names.
     */
    /****************************************************************************/
    void SetFirmwareFilenames(FirmwareFileNames_t Value) {
        mp_FirmwareFilenames = new FirmwareFileNames_t;
        mp_FirmwareFilenames->ASB_1_Filename = Value.ASB_1_Filename;
        mp_FirmwareFilenames->ASB_2_Filename = Value.ASB_2_Filename;
        mp_FirmwareFilenames->ASB_3_Filename = Value.ASB_3_Filename;
        mp_FirmwareFilenames->ASB_4_Filename = Value.ASB_4_Filename;
        mp_FirmwareFilenames->ASB_5_Filename = Value.ASB_5_Filename;
        mp_FirmwareFilenames->ASB_a_Filename = Value.ASB_a_Filename;
        mp_FirmwareFilenames->ASB_b_Filename = Value.ASB_b_Filename;
        mp_FirmwareFilenames->ASB_12_Filename = Value.ASB_12_Filename;
        mp_FirmwareFilenames->ASB_13_Filename = Value.ASB_13_Filename;
    }

    /****************************************************************************/
    /**
     * \brief Returns Module numbers
     * \return Structure of Module numbers
     */
    /****************************************************************************/
    ModuleNumbers_t* GetModuleNumbers() {
        return mp_ModuleNumbers;
    }

    /****************************************************************************/
    /**
     * \brief Sets Module numbers.
     * \iparam Value = Structure of Module numbers
     */
    /****************************************************************************/
    void SetModuleNumbers(ModuleNumbers_t Value) {
        mp_ModuleNumbers = new ModuleNumbers_t;
        mp_ModuleNumbers->DrawerLeft = Value.DrawerLeft;
        mp_ModuleNumbers->DrawerRight = Value.DrawerRight;
        mp_ModuleNumbers->Oven = Value.Oven;
        mp_ModuleNumbers->XArmLeft = Value.XArmLeft;
        mp_ModuleNumbers->XArmRight = Value.XArmRight;
        mp_ModuleNumbers->XAxis = Value.XAxis;
        mp_ModuleNumbers->TransferStation = Value.TransferStation;
        mp_ModuleNumbers->E_Box = Value.E_Box;
        mp_ModuleNumbers->AirExhaust = Value.AirExhaust;
        mp_ModuleNumbers->WaterStation = Value.WaterStation;
        mp_ModuleNumbers->HeatedCuvettes = Value.HeatedCuvettes;
        mp_ModuleNumbers->Agitator = Value.Agitator;
    }

    /****************************************************************************/
    /**
     * \brief Returns Calibration offset parameters.
     * \return Structure of Calibration offset parameters.
     */
    /****************************************************************************/
    CalibrationOffsets_t* GetCalibrationOffsets() {
        return mp_CalibrationOffsets;
    }

    /****************************************************************************/
    /**
     * \brief Sets Calibration offset parameters.
     * \iparam Value = Structure of Calibration offset parameters.
     */
    /****************************************************************************/
    void SetCalibrationOffsets(CalibrationOffsets_t Value) {
        mp_CalibrationOffsets = new CalibrationOffsets_t;
        mp_CalibrationOffsets->OvenOpenPos = Value.OvenOpenPos;
        mp_CalibrationOffsets->OvenClosePos = Value.OvenClosePos;
        mp_CalibrationOffsets->DrawerOpenPos = Value.DrawerOpenPos;
        mp_CalibrationOffsets->DrawerClosePos = Value.DrawerClosePos;
        mp_CalibrationOffsets->StandardOffset = Value.StandardOffset;
    }

    /****************************************************************************/
    /**
     * \brief Returns Calibration parameters.
     * \return Structure of Calibration parameters.
     */
    /****************************************************************************/
    CalibrationParameters_t* GetCalibrationParameters() {
        return mp_CalibrationParameters;
    }

    /****************************************************************************/
    /**
     * \brief Sets Calibration parameters.
     * \iparam Value = Structure of Calibration parameters.
     */
    /****************************************************************************/
    void SetCalibrationParameters(CalibrationParameters_t Value) {
        mp_CalibrationParameters = new CalibrationParameters_t;
        mp_CalibrationParameters->AgitationWidth12 = Value.AgitationWidth12;
        mp_CalibrationParameters->AgitationWidth21 = Value.AgitationWidth21;
        mp_CalibrationParameters->MinDiff03 = Value.MinDiff03;
    }

    /****************************************************************************/
    /**
     * \brief Returns system log off time in mSec
     * \return Systems log off time.
     */
    /****************************************************************************/
    int GetSystemLogOffTime() {
        return m_SystemLogOffTime;
    }

    /****************************************************************************/
    /**
     * \brief Sets system log off time in mSec
     * \iparam Value = system log off time.
     */
    /****************************************************************************/
    void SetSystemLogOffTime(int Value) {
        m_SystemLogOffTime = Value;
    }

private:
    int m_Version;                                  //!< Stores the version number of the file
    QString m_ProxyUserName;                        //!< Proxy user name
    QString m_ProxyPassword;                        //!< Proxy password
    QString m_ProxyIPAddress;                       //!< Pro)xy IP address
    int m_ProxyIPPort;                              //!< Proxy IP port

    LoaderRFID_t *mp_LoaderRfid;                    //!< Drawer loader RFID structure
    UnloaderRFID_t *mp_UnloaderRfid;                //!< Drawer unloader RFID structure
    XArmLeftParameters_t *mp_XArmLeftParameters;    //!< X Arm left parameters structure
    XArmRightParameters_t *mp_XArmRightParameters;  //!< X Arm right parameters structure
    OvenParameters_t *mp_OvenParameters;            //!< Oven parameters structure
    HeatedCuvetteParameters_t *mp_HCParameters;     //!< Heated cuvette parameters structure
    FirmwareFileNames_t *mp_FirmwareFilenames;      //!< Firmware file names structure
    ModuleNumbers_t *mp_ModuleNumbers;              //!< Module numbers structure
    CalibrationOffsets_t *mp_CalibrationOffsets;    //!< Calibration offset values
    CalibrationParameters_t *mp_CalibrationParameters;    //!< Calibration Parameters

    QString m_YZModuleRfid;                         //!< YZ Module RFID structure
    QString m_TestReportFolderPath;                 //!< Test report folder path
    QString m_FirmwarefolderPath;                   //!< Firmware folder path
    int m_SystemLogOffTime;                     //!< System log off time in mSec

    bool m_DataVerificationMode;                    //!< Store the Date verification mode flag
    QString m_FileName;                             //!< Store the file name

    bool SerializeContent(QIODevice& IODevice, bool CompleteData);
    bool DeserializeContent(QIODevice& IODevice, bool CompleteData);

    bool ReadNetworkSettings(QXmlStreamReader& XmlStreamReader);
    bool ReadLoaderRFIDSettings(QXmlStreamReader& XmlStreamReader);
    bool ReadUnloaderRFIDSettings(QXmlStreamReader& XmlStreamReader);
    bool ReadYZRFIDSettings(QXmlStreamReader& XmlStreamReader);
    bool ReadXArmLeftParameters(QXmlStreamReader& XmlStreamReader);
    bool ReadXArmRightParameters(QXmlStreamReader& XmlStreamReader);
    bool ReadOvenParameters(QXmlStreamReader& XmlStreamReader);
    bool ReadCuvettesParameters(QXmlStreamReader& XmlStreamReader);
    bool ReadTestReportFolderPath(QXmlStreamReader& XmlStreamReader);
    bool ReadFirmwareFolderPath(QXmlStreamReader& XmlStreamReader);
    bool ReadFirmwareFileNames(QXmlStreamReader& XmlStreamReader);
    bool ReadModuleNumbers(QXmlStreamReader& XmlStreamReader);
    bool ReadCalibrationOffsets(QXmlStreamReader& XmlStreamReader);
    bool ReadCalibrationParameters(QXmlStreamReader& XmlStreamReader);
    bool ReadSystemLogOffTime(QXmlStreamReader& XmlStreamReader);
};


} // end of namespace DataManager

#endif // SERVICEDATACONTAINER_SERVICEPARAMETERS_H
