/****************************************************************************/
/*! \file WrapperFmBootLoader.cpp
 *
 *  \brief  Implementation file for class WrapperFmBootLoader
 *
 *  \version  0.1
 *  \date     2012-05-10
 *  \author   M.Scherer
 *
 *  \b Description:
 *
 *   N/A
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2012 by Leica Biosystems Nussloch GmbH. All rights reserved
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 */
/****************************************************************************/

#include "ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmBootLoader.h"

/****************************************************************************/

#define INFOBLOCK_SIGNATURE  0x91827364  //!< Signature for valid info blocks

/****************************************************************************/
/*!
 *  \brief Constructor of class WrapperFmBootLoader
 *
 *  \iparam   Name         The Name of boot loadermodule used for QtScript
 *                         identifier.
 *  \iparam   pBootLoader  Pointer to CBootLoader
 *  \iparam   pParent      Optional pointer to parent for QObject Hierarchy.
 *
 */
/****************************************************************************/
WrapperFmBootLoader::WrapperFmBootLoader(QString Name, CBootLoader *pBootLoader, QObject *pParent):
    WrapperBase(Name, pParent), m_pBootLoader(pBootLoader)
{
    Reset();

    connect(m_pBootLoader, SIGNAL(ReportUpdateFirmware(quint32, ReturnCode_t)),
            this, SLOT(OnUpdateFirmware(quint32, ReturnCode_t)));
    connect(m_pBootLoader, SIGNAL(ReportUpdateInfo(quint32, ReturnCode_t)),
            this, SLOT(OnUpdateInfo(quint32, ReturnCode_t)));
}

/****************************************************************************/
/*!
 *  \brief Script-API: Updates the firmware image
 *
 *  This method updates the firmware on a Slave using its boot loader. It
 *  reads a file from the local storage and forwards it to the boot loader
 *  component of the base module.
 *
 *  Examples:
 *  \dontinclude bootloader.js
 *  \skipline [BootLoader.UpdateFirmware]
 *  \until    [BootLoader.UpdateFirmware]
 *
 *  \iparam FirmwarePath The path to the firmware image file
 *
 *  \return true, if the setting value is success else false
 *
 */
/****************************************************************************/
bool WrapperFmBootLoader::UpdateFirmware(const QString &FirmwarePath)
{
    bool ok = HandleErrorCode(m_pBootLoader->UpdateFirmware(FirmwarePath));
    if (!ok) {
        return false;
    }
    qint32 ret = m_LoopUpdateFirmware.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief Script-API: Starts the firmware
 *
 *  This method prompts the boot loader to start the firmware image on the
 *  node. This is necessary after a firmware update or if the boot loader is
 *  in a wait state.
 *
 *  Examples:
 *  \dontinclude bootloader.js
 *  \skipline [BootLoader.BootFirmware]
 *  \until    [BootLoader.BootFirmware]
 *
 *  \return true, if the setting value is success else false
 *
 */
/****************************************************************************/
bool WrapperFmBootLoader::BootFirmware()
{
    return HandleErrorCode(m_pBootLoader->BootFirmware());
}

/****************************************************************************/
/*!
 *  \brief   slot for the firmware update
 *
 *  This slot is connected to the signal ReportUpdateFirmware.
 *
 *  \iparam InstanceID  Instance ID of the function module
 *  \iparam ReturnCode  ReturnCode of DeviceControl Layer
 *
 */
/****************************************************************************/
void WrapperFmBootLoader::OnUpdateFirmware(quint32 InstanceID, ReturnCode_t ReturnCode)
{
    Q_UNUSED(InstanceID)

    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    if (m_LoopUpdateFirmware.isRunning()) {
        m_LoopUpdateFirmware.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API: Updates the info blocks
 *
 *  This method updates the info blocks on a Slave using its boot loader. It
 *  reads info block from the script file and forwards it to the boot loader
 *  component of the base module.
 *
 *  Board info block:
 *      quint32 Signature;          Signature to detect info block
 *      quint8  NodeType;           Node type
 *      quint8  NodeClass;          Node class
 *      quint8  VersionMajor;       Major hardware version number
 *      quint8  VersionMinor;       Minor hardware version number
 *      quint8  ProductionYear;     Year of production
 *      quint8  ProductionMonth;    Month of production
 *      quint8  ProductionDay;      Day of production
 *      quint8  EndTestYear;        Year of end-test
 *      quint8  EndTestMonth;       Month of end-test
 *      quint8  EndTestDay;         Day of end-test
 *      quint8  EndTestResult;      End-test result (ok, failed, open)
 *      char    BoardName[16];      Board name string
 *      char    SerialNum[32];      Serial number string
 *      quint32 Checksum;           Checksum to verfiy info block
 *
 *  Bootloader info block:
 *      quint32 Signature;          Signature for valid info block
 *      quint8  VersionMajor;       Major hardware version number
 *      quint8  VersionMinor;       Minor hardware version number
 *      quint8  CreationYear;       Year of production
 *      quint8  CreationMonth;      Month of production
 *      quint8  CreationDay;        Day of production
 *      quint32 Checksum;           Checksum to verfiy info block
 *
 *  Board options block:
 *      One data structure for each module, may contain multiple structures.
 *      Example:
 *      quint32 Module ID
 *      quint32 Option count
 *      quint32 Option 1
 *      quint32 Option 2
 *      ...
 *
 *  Examples:
 *  \dontinclude bootloader.js
 *  \skipline [BootLoader.UpdateInfo]
 *  \until    [BootLoader.UpdateInfo]
 *
 *  \iparam Info The string array including all settings for info block
 *  \iparam UpdateType = The type of updated info block (0 = board info, 1 =
 *                       bootloader info, 2 = board options)
 *
 *  \return true, if the setting value is success else false
 *
 */
/****************************************************************************/
bool WrapperFmBootLoader::UpdateInfo(const QStringList &Info, quint8 UpdateType)
{

    bool ok;

    switch (UpdateType) {

    case 0:  //Board Info
    {        
        // Be careful with possibly different aligned bytes in different platform
        bmBoardInfoBlock_t BoardInfo;
        memset((void*)&BoardInfo, 0, sizeof(bmBoardInfoBlock_t));

        BoardInfo.Signature       = Info.at(0).toUInt(&ok, 0);
        BoardInfo.NodeType        = Info.at(1).toUInt(&ok, 0);
        BoardInfo.NodeClass       = Info.at(2).toUInt(&ok, 0);
        BoardInfo.VersionMajor    = Info.at(3).toUInt(&ok, 0);
        BoardInfo.VersionMinor    = Info.at(4).toUInt(&ok, 0);
        BoardInfo.ProductionYear  = Info.at(5).toUInt(&ok, 0);
        BoardInfo.ProductionMonth = Info.at(6).toUInt(&ok, 0);
        BoardInfo.ProductionDay   = Info.at(7).toUInt(&ok, 0);
        BoardInfo.EndTestYear     = Info.at(8).toUInt(&ok, 0);
        BoardInfo.EndTestMonth    = Info.at(9).toUInt(&ok, 0);
        BoardInfo.EndTestDay      = Info.at(10).toUInt(&ok, 0);
        BoardInfo.EndTestResult   = Info.at(11).toUInt(&ok, 0);        
        strcpy(BoardInfo.BoardName, Info.at(12).toAscii().constData());        
        strcpy(BoardInfo.SerialNum, Info.at(13).toAscii().constData());
        BoardInfo.Checksum        = Info.at(14).toUInt(&ok, 0);

        ok = HandleErrorCode( m_pBootLoader->UpdateInfo((quint8*)&BoardInfo, sizeof(bmBoardInfoBlock_t), 0) );
        if (!ok) {
            return false;
        }

    }
    break;
    case 1:  //Bootloader Info
    {
        // Be careful with possibly different aligned bytes in different platform
        bmBootInfoBlock_t BootInfo;
        memset((void*)&BootInfo, 0, sizeof(bmBootInfoBlock_t));

        BootInfo.Signature     = Info.at(0).toUInt(&ok, 0);
        BootInfo.VersionMajor  = Info.at(1).toUInt(&ok, 0);
        BootInfo.VersionMinor  = Info.at(2).toUInt(&ok, 0);
        BootInfo.CreationYear  = Info.at(3).toUInt(&ok, 0);
        BootInfo.CreationMonth = Info.at(4).toUInt(&ok, 0);
        BootInfo.CreationDay   = Info.at(5).toUInt(&ok, 0);
        BootInfo.Checksum      = Info.at(6).toUInt(&ok, 0);

        ok = HandleErrorCode( m_pBootLoader->UpdateInfo((quint8*)&BootInfo, sizeof(bmBootInfoBlock_t), 1) );
        if (!ok) {
            return false;
        }
    }
    break;
    case 2:  //Borad Options
    {
        quint8 i;
        quint8 OptionElements = Info.size();
        quint32* pBoardOptions = NULL;

        pBoardOptions = new quint32[OptionElements+3];
        if (pBoardOptions == NULL) {
            return false;
        }
        memset((void*)pBoardOptions, 0, sizeof(quint32)*(OptionElements+3));

        pBoardOptions[0] = INFOBLOCK_SIGNATURE;
        pBoardOptions[1] = OptionElements+2;
        for (i=0; i<OptionElements; i++) {
            pBoardOptions[2+i] = Info.at(i).toUInt(&ok, 0);
        }
        pBoardOptions[2+i] = 0;

        ok = HandleErrorCode( m_pBootLoader->UpdateInfo((quint8*)pBoardOptions, sizeof(quint32)*(OptionElements+3), 2) );
        delete[] pBoardOptions;

        if (!ok) {
            return false;
        }

    }
    break;
    default:
        return false;
    }

    qint32 ret = m_LoopUpdateInfo.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief   slot for setting the analog output value
 *
 *  This slot is connected to the signal, ReportOutputValueAckn.
 *
 *  \iparam InstanceID  Device instance ID
 *  \iparam ReturnCode  ReturnCode of DeviceControl Layer
 *
 */
/****************************************************************************/
void WrapperFmBootLoader::OnUpdateInfo(quint32 InstanceID, ReturnCode_t ReturnCode)
{
    Q_UNUSED(InstanceID)

    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    if (m_LoopUpdateInfo.isRunning()) {
        m_LoopUpdateInfo.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API: Reset internal states
 *
 *  The internal states of the instance are reset by calling this method.
 *  This is useful for development and debugging when the wrapper layer
 *  is waiting for a signal in vain.
 *  The implementation can be expanded to reset and reconfigure slaves.
 */
/****************************************************************************/
void WrapperFmBootLoader::Reset()
{
}
