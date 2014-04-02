/****************************************************************************/
/*! \file ServiceParametersVerifier.cpp
 *
 *  \brief CServiceParametersVerifier class implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-06-26
 *   $Author:  $ Soumya D
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

#include "ServiceDataManager/Include/ServiceParametersVerifier.h"

//lint -e613
//lint -e1536

namespace DataManager
{

/****************************************************************************/
/*!
 *  \brief Default Constructor
 */
/****************************************************************************/
CServiceParametersVerifier::CServiceParametersVerifier(): mp_ServiceParameters(NULL)
{
}

/****************************************************************************/
/*!
 *  \brief Verifies the data present in the service parameters against constraints.
 *  \iparam p_ServiceParameters
 *  \return true - verify success , false - verify failure
 */
/****************************************************************************/
bool CServiceParametersVerifier::VerifyData(CDataContainerBase *p_ServiceParameters)
{
    bool VerifiedData = true;

    mp_ServiceParameters = static_cast<CServiceParameters*>(p_ServiceParameters);

    if (mp_ServiceParameters == NULL) {
        qDebug() << "Service parameters Data container is empty" << endl;
        VerifiedData = false;
    }

    int Version = mp_ServiceParameters->GetVersion();
    if (Version != 1) {
        qDebug() << "Service parameters version is invalid." << endl;
        VerifiedData = false;
    }

    QString ProxyUserName = mp_ServiceParameters->GetProxyUserName();
    if (ProxyUserName.isEmpty()) {
        qDebug() << "Proxy user name is NULL." << endl;
        VerifiedData = false;
    }

    QString ProxyPassword = mp_ServiceParameters->GetProxyPassword();
    if (ProxyPassword.isEmpty()) {
        qDebug() << "Proxy password is NULL." << endl;
        VerifiedData = false;
    }

    int ProxyPort = mp_ServiceParameters->GetProxyIPPort();
    if (ProxyPort == (int) INVALID_TYPE) {
        qDebug() << "Proxy port data is NULL." << endl;
        VerifiedData = false;
    }

    QString ProxyIPAddress = mp_ServiceParameters->GetProxyIPAddress();
    if (ProxyIPAddress.isEmpty()) {
        qDebug() << "Proxy IP address is NULL." << endl;
        VerifiedData = false;
    }

    LoaderRFID_t *LoaderRFID = mp_ServiceParameters->GetLoaderRFID();
    if (LoaderRFID->Rack1.isEmpty() || LoaderRFID->Rack2.isEmpty() || LoaderRFID->Rack3.isEmpty() ||
            LoaderRFID->Rack4.isEmpty() || LoaderRFID->Rack5.isEmpty()) {
        qDebug() << "Loader RFID values are NULL." << endl;
        VerifiedData = false;
    }

    UnloaderRFID_t *UnLoaderRFID = mp_ServiceParameters->GetUnloaderRFID();
    if (UnLoaderRFID->Rack1.isEmpty() || UnLoaderRFID->Rack2.isEmpty() || UnLoaderRFID->Rack3.isEmpty() ||
            UnLoaderRFID->Rack4.isEmpty() || UnLoaderRFID->Rack5.isEmpty()) {
        qDebug() << "UnLoader RFID values are NULL." << endl;
        VerifiedData = false;
    }

    QString YZRfid = mp_ServiceParameters->GetYZModuleRFID();
    if (YZRfid.isEmpty()) {
        qDebug() << "YZ module RFID is NULL." << endl;
        VerifiedData = false;
    }   

    XArmLeftParameters_t *XArmLeftParameters = mp_ServiceParameters->GetXArmLeftParameters();
    if (XArmLeftParameters->Filledx.isEmpty() || XArmLeftParameters->Filledy.isEmpty() || XArmLeftParameters->Emptyx.isEmpty() ||
            XArmLeftParameters->Emptyy.isEmpty() || XArmLeftParameters->Rackx.isEmpty() || XArmLeftParameters->Racky.isEmpty()) {
        qDebug() << "X Arm left parameter values are NULL." << endl;
        VerifiedData = false;
    }

    XArmRightParameters_t *XArmRightParameters = mp_ServiceParameters->GetXArmRightParameters();
    if (XArmRightParameters->Filledx.isEmpty() || XArmRightParameters->Filledy.isEmpty() || XArmRightParameters->Emptyx.isEmpty() ||
            XArmRightParameters->Emptyy.isEmpty() || XArmRightParameters->Rackx.isEmpty() || XArmRightParameters->Racky.isEmpty()) {
        qDebug() << "X Arm right parameter values are NULL." << endl;
        VerifiedData = false;
    }

    OvenParameters_t *OvenParameters = mp_ServiceParameters->GetOvenParameters();
    if (OvenParameters->MaxDiff.isEmpty() || OvenParameters->RoomTempLow.isEmpty() || OvenParameters->RoomTempHigh.isEmpty() ||
            OvenParameters->HTime1.isEmpty() || OvenParameters->HTime2.isEmpty() || OvenParameters->TempDiff.isEmpty()
            || OvenParameters->OvenMaxLow.isEmpty() || OvenParameters->OvenMaxHigh.isEmpty()) {
        qDebug() << "Oven parameter values are NULL." << endl;
        VerifiedData = false;
    }

    HeatedCuvetteParameters_t *HCParameters = mp_ServiceParameters->GetHeatedCuvetteParameters();
    if (HCParameters->MaxDiff.isEmpty() || HCParameters->RoomTempLow.isEmpty() || HCParameters->RoomTempHigh.isEmpty() ||
            HCParameters->HTime1.isEmpty() || HCParameters->HTime2.isEmpty() || HCParameters->TempDiff.isEmpty()
            || HCParameters->OvenMaxLow.isEmpty() || HCParameters->OvenMaxHigh.isEmpty()) {
        qDebug() << "Heated cuvette parameter values are NULL." << endl;
        VerifiedData = false;
    }

    ModuleNumbers_t *ModuleNumbers = mp_ServiceParameters->GetModuleNumbers();
    if (ModuleNumbers->DrawerLeft.isEmpty() || ModuleNumbers->DrawerRight.isEmpty() || ModuleNumbers->XArmLeft.isEmpty() ||
            ModuleNumbers->XArmRight.isEmpty() || ModuleNumbers->XAxis.isEmpty() || ModuleNumbers->Oven.isEmpty() ||
            ModuleNumbers->Agitator.isEmpty() || ModuleNumbers->AirExhaust.isEmpty() || ModuleNumbers->E_Box.isEmpty() ||
            ModuleNumbers->HeatedCuvettes.isEmpty() || ModuleNumbers->TransferStation.isEmpty() ||
            ModuleNumbers->WaterStation.isEmpty()) {
        qDebug() << "Module number values are NULL." << endl;
        VerifiedData = false;
    }    

    QString TestReportFolderPath = mp_ServiceParameters->GetTestReportFolderPath();
    if (TestReportFolderPath.isEmpty()) {
        qDebug() << "TestReportFolder Path is NULL." << endl;
        VerifiedData = false;
    }

    QString FirmwareFolderPath = mp_ServiceParameters->GetFirmwareFolderPath();
    if (FirmwareFolderPath.isEmpty()) {
        qDebug() << "FirmwareFolder Path is NULL." << endl;
        VerifiedData = false;
    }

    FirmwareFileNames_t *FirmwareFileNames = mp_ServiceParameters->GetFirmwareFilenames();
    if (FirmwareFileNames->ASB_1_Filename.isEmpty() || FirmwareFileNames->ASB_2_Filename.isEmpty() || FirmwareFileNames->ASB_3_Filename.isEmpty() ||
            FirmwareFileNames->ASB_4_Filename.isEmpty() || FirmwareFileNames->ASB_5_Filename.isEmpty() || FirmwareFileNames->ASB_a_Filename.isEmpty()
            || FirmwareFileNames->ASB_b_Filename.isEmpty() || FirmwareFileNames->ASB_12_Filename.isEmpty() || FirmwareFileNames->ASB_13_Filename.isEmpty()) {
        qDebug() << "Firmware file names are NULL." << endl;
        VerifiedData = false;
    }

    CalibrationOffsets_t *CalibrationOffsets = mp_ServiceParameters->GetCalibrationOffsets();
    if (CalibrationOffsets->OvenOpenPos.isEmpty() || CalibrationOffsets->OvenClosePos.isEmpty() ||
            CalibrationOffsets->DrawerOpenPos.isEmpty() || CalibrationOffsets->DrawerClosePos.isEmpty() ||
            CalibrationOffsets->StandardOffset.isEmpty()) {
        qDebug() << "Calibration Offset values are NULL." << endl;
        VerifiedData = false;
    }

    CalibrationParameters_t *CalibrationParameters = mp_ServiceParameters->GetCalibrationParameters();
    if (CalibrationParameters->AgitationWidth12.isEmpty() || CalibrationParameters->AgitationWidth21.isEmpty() ||
            CalibrationParameters->MinDiff03.isEmpty()) {
        qDebug() << "Calibration parameter values are NULL." << endl;
        VerifiedData = false;
    }

    int SystemLogOffTime = mp_ServiceParameters->GetSystemLogOffTime();
    if (!SystemLogOffTime) {
        qDebug() << "SystemLogOffTime value is empty." << endl;
        VerifiedData = false;
    }

    return VerifiedData;
}

/****************************************************************************/
/*!
 *  \brief  Gets the last errors which is done by verifier
 *
 *  \return QStringList - List of the errors occured
 */
/****************************************************************************/
ErrorMap_t& CServiceParametersVerifier::GetErrors()
{
    //lint -efunc(1536, GetErrors)
    //lint -esym(1536, m_ErrorMap)
    // return the last error which is occured in the verifier
    return m_ErrorMap;
}

/****************************************************************************/
/*!
 *  \brief  Resets the last error which is done by verifier
 */
/****************************************************************************/
void CServiceParametersVerifier::ResetErrors()
{
    m_ErrorMap.clear();
}

/****************************************************************************/
/*!
 *  \brief  Resets the last error which is done by verifier
 *  \return true
 */
/****************************************************************************/
bool CServiceParametersVerifier::IsLocalVerifier()
{
    return true;
}

}   // end of namespace DataManager

