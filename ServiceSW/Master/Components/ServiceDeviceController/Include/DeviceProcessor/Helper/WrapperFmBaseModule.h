/****************************************************************************/
/*! \file WrapperFmBaseModule.h
 *
 *  \brief  Definition file for class WrapperFmBaseModule
 *
 *  \version  0.1
 *  \date     2012-05-21
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

#ifndef WRAPPERFMBASEMODULE_H
#define WRAPPERFMBASEMODULE_H

#include <QFile>
#include <QObject>
#include <QEventLoop>
#include "ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperBase.h"
#include "DeviceControl/Include/SlaveModules/BaseModule.h"

using namespace DeviceControl;

class CResetData;

/****************************************************************************/
/*! \brief Wrapper for the base module
 *
 *  This wrapper class provides a QtScript interface to the base modules.
 */
/****************************************************************************/
class WrapperFmBaseModule : public WrapperBase
{
    Q_OBJECT

    //! Hardware version
    Q_PROPERTY(quint16 HardwareVersion READ GetHardwareVersion)
    //! Software version
    Q_PROPERTY(quint16 SoftwareVersion READ GetSoftwareVersion)

public:
    WrapperFmBaseModule(QString Name, CBaseModule *pBaseModule, QObject *pParent);
    ~WrapperFmBaseModule();

    /****************************************************************************/
    /*!
     *  \brief  Getter for the hardware version property
     *
     *  \return Hardware version (major version: MSB, minor version: LSB)
     */
    /****************************************************************************/
    quint32 GetHardwareVersion() { return (m_HWInfo.VersionMajor << 8) | m_HWInfo.VersionMinor; }
    /****************************************************************************/
    /*!
     *  \brief  Getter for the software version property
     *
     *  \return Software version
     */
    /****************************************************************************/
    quint32 GetSoftwareVersion() { return m_SWInfo.Version; }

public slots:
    bool ResetNode();
    void WaitForUpdate(bool Wait);
    bool EmergencyStop();
    bool ExitEmergencyStop();
    QString SetNodeState(quint8 NodeState);
    quint8 ReqNodeState();
    QString GetHWInfo();
    QString GetSWInfo();
    QString GetSWVersions();
    QString GetBootloaderInfo();
    QString GetEndTestResultInfo();
    bool ReqDataReset();
    bool ReqFormatMemory();
    QString ReqSerialNumber();
    bool ReqEndTestResult();
    bool ReqLoaderInfo();
    bool ReqLifeCycleData();
    bool ReqLaunchDate();
    QString ReqBoardName();
    bool ConfigureVoltageMonitor(bool Enable, quint8 Filter, quint8 SamplingPeriod,
                                 quint16 GoodThreshold, quint16 FailThreshold);
    bool ReqVoltageState();
    bool ConfigureCurrentMonitor(bool Enable, quint8 Filter, quint8 SamplingPeriod,
                                 quint16 GoodThreshold, quint16 FailThreshold);
    bool ReqCurrentState();
    QString ReqUniqueNumber();
    QString ReqModuleSerialNumber();
    bool SetModuleSerialNumber(QString ModuleSerialNumber);
    bool UpdateBootLoader(const QString &BootLoaderPath);
    void Reset();

private slots:
    void OnSetNodeState(quint32 InstanceID, ReturnCode_t ReturnCode, NodeState_t NodeState,
                        EmergencyStopReason_t EmergencyStopReason, PowerState_t VoltageState);
    void OnReportHWInfo(quint32 InstanceID, ReturnCode_t ReturnCode, quint8 VersionMajor, quint8 VersionMinor, QDate Date);
    void OnReportSWInfo(quint32 InstanceID, ReturnCode_t ReturnCode, quint16 Version, QDate Date);
    void OnFormatMemoryAckn(quint32 InstanceID, ReturnCode_t ReturnCode);
    void OnSerialNumber(quint32 InstanceID, ReturnCode_t ReturnCode, QString SerialNumber);
    void OnEndTestResult(quint32 InstanceID, ReturnCode_t ReturnCode, TestResult_t Result, QDate Date);
    void OnLoaderInfo(quint32 InstanceID, ReturnCode_t ReturnCode, quint8 VersionMajor, quint8 VersionMinor, QDate Date);
    void OnLifeCycleData(quint32 InstanceID, ReturnCode_t ReturnCode, quint32 OperationTime, quint16 StartupCycles);
    void OnLaunchDate(quint32 InstanceID, ReturnCode_t ReturnCode, bool Launched, QDate Date);
    void OnBoardName(quint32 InstanceID, ReturnCode_t ReturnCode, QString BoardName);
    void OnVoltageState(quint32 InstanceID, ReturnCode_t ReturnCode, PowerState_t State, quint16 Value, quint16 Failures);
    void OnCurrentState(quint32 InstanceID, ReturnCode_t ReturnCode, PowerState_t State, quint16 Value, quint16 Failures);
    void OnUniqueNumber(quint32 InstanceID, ReturnCode_t ReturnCode, QByteArray UniqueNumber);
    void OnModuleSerialNumber(quint32 InstanceID, ReturnCode_t ReturnCode, quint64 ModuleSerialNumber);
    void OnUpdateBootLoader(quint32 InstanceID, ReturnCode_t HdlInfo);

private:
    static QString PowerStateToString(PowerState_t State);

    //! Hardware version information
    typedef struct {
        quint8 VersionMajor;    //!< Hardware major version number
        quint8 VersionMinor;    //!< Hardware minor version number
        QDate Date;             //!< Hardware version release date
    } HWInfo_t;

    //! Software version information
    typedef struct {
        quint16 Version;        //!< Software version number
        QDate Date;             //!< Software version release date
    } SWInfo_t;

    typedef struct {
        quint8 VersionMajor;    //!< Bootloader major version number
        quint8 VersionMinor;    //!< Bootloader major version number
        QDate Date;             //!< Bootloader version release date
    } BootLoaderInfo_t;

    typedef struct {
        QString TestResult;   //!< End test result
        QDate Date;         //!< End test date
    } EndTestResultInfo_t;

    CBaseModule *m_pBaseModule;         //!< Pointer to CBaseModule
    QEventLoop   m_LoopSetNodeState;    //!< Loop for blocking commands.
    QEventLoop   m_LoopReqNodeState;    //!< Loop for blocking commands.
    QEventLoop   m_LoopGetHWInfo;       //!< Loop for blocking commands.
    QEventLoop   m_LoopGetSWInfo;       //!< Loop for blocking commands.
    QEventLoop   m_LoopFormatMemory;    //!< Loop for blocking commands.
    QEventLoop   m_LoopSerialNumber;    //!< Loop for blocking commands.
    QEventLoop   m_LoopEndTestResult;   //!< Loop for blocking commands.
    QEventLoop   m_LoopLoaderInfo;      //!< Loop for blocking commands.
    QEventLoop   m_LoopLifeCycleData;   //!< Loop for blocking commands.
    QEventLoop   m_LoopLaunchDate;      //!< Loop for blocking commands.
    QEventLoop   m_LoopBoardName;       //!< Loop for blocking commands.
    QEventLoop   m_LoopVoltageState;    //!< Loop for blocking commands.
    QEventLoop   m_LoopCurrentState;    //!< Loop for blocking commands.
    QEventLoop   m_LoopUniqueNumber;    //!< Loop for blocking commands.
    QEventLoop   m_LoopModuleSerial;    //!< Loop for blocking commands.
    QEventLoop   m_LoopUpdateBootLoader;//!< Loop for blocking commands.
    HWInfo_t     m_HWInfo;              //!< hardware information
    SWInfo_t     m_SWInfo;              //!< software information
    NodeState_t  m_NodeState;           //!< actual node state
    QString      m_SerialNumber;        //!< Serial number of the slave
    QString      m_BoardName;           //!< Board name of the slave
    QByteArray   m_UniqueNumber;        //!< Unique number of the slave
    quint64      m_ModuleSerialNumber;  //!< Module serial number
    CResetData  *mp_ResetData;          //!< Support for the reset data command
    BootLoaderInfo_t    m_BootloaderInfo; //!< Bootloader information
    EndTestResultInfo_t       m_EndTestResultInfo;     //!< End test information

};

#endif // WRAPPERFMBASEMODULE_H
