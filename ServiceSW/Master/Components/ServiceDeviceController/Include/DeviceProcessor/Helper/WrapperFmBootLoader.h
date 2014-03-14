/****************************************************************************/
/*! \file WrapperFmBootLoader.h
 *
 *  \brief  Definition file for class WrapperFmBootLoader
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

#ifndef WRAPPERFMBOOTLOADER_H
#define WRAPPERFMBOOTLOADER_H

#include <QFile>
#include <QObject>
#include <QEventLoop>
#include "WrapperBase.h"
#include "DeviceControl/Include/SlaveModules/BootLoader.h"

using namespace DeviceControl;

/****************************************************************************/
/*! \brief Wrapper for boot loader modules
 *
 *  This wrapper class provides a QtScript interface to boot loader modules.
 *
 *  Examples:
 *   \dontinclude bootloader.js
 *   \skipline [WrapperFmBootLoader]
 *   \until    [WrapperFmBootLoader]
 */
/****************************************************************************/
class WrapperFmBootLoader : public WrapperBase
{
    Q_OBJECT

public:
    WrapperFmBootLoader(QString Name, CBootLoader *pBootLoader, QObject *pParent);

    //****************************************************************************/
    //! The board info block contains informations about the board.
    //****************************************************************************/

    typedef struct {
        quint32 Signature;           //!< Signature to detect info block
        quint8  NodeType;            //!< Node type
        quint8  NodeClass;           //!< Node class
        quint8  VersionMajor;        //!< Major hardware version number
        quint8  VersionMinor;        //!< Minor hardware version number
        quint8  ProductionYear;      //!< Year of production
        quint8  ProductionMonth;     //!< Month of production
        quint8  ProductionDay;       //!< Day of production
        quint8  EndTestYear;         //!< Year of end-test
        quint8  EndTestMonth;        //!< Month of end-test
        quint8  EndTestDay;          //!< Day of end-test
        quint8  EndTestResult;       //!< End-test result (ok, failed, open)
        char    BoardName[16];       //!< Board name string
        char    SerialNum[32];       //!< Serial number string
        quint32 Checksum;            //!< Checksum to verfiy info block
    } bmBoardInfoBlock_t;

    //****************************************************************************/
    //! The bootloader info block contains informations about the bootloader
    //****************************************************************************/

    typedef struct {
        quint32 Signature;           //!< Signature for valid info block
        quint8  VersionMajor;        //!< Major hardware version number
        quint8  VersionMinor;        //!< Minor hardware version number
        quint8  CreationYear;        //!< Year of production
        quint8  CreationMonth;       //!< Month of production
        quint8  CreationDay;         //!< Day of production
        quint32 Checksum;            //!< Checksum to verfiy info block
    } bmBootInfoBlock_t;

public slots:
    bool UpdateFirmware(const QString &FirmwarePath);
    bool UpdateInfo(const QStringList &Info, quint8 UpdateType);
    bool BootFirmware();
    void Reset();

private slots:
    void OnUpdateFirmware(quint32 InstanceID, ReturnCode_t HdlInfo);
    void OnUpdateInfo(quint32 InstanceID, ReturnCode_t HdlInfo);

private:
    CBootLoader *m_pBootLoader;         //!< Pointer to CBootLoader
    QEventLoop m_LoopUpdateFirmware;    //!< Loop for blocking commands.
    QEventLoop m_LoopUpdateInfo;        //!< Loop for blocking commands.
    QEventLoop m_LoopEnableUpdate;      //!< Loop for blocking commands.
};

#endif // WRAPPERFMBOOTLOADER_H
