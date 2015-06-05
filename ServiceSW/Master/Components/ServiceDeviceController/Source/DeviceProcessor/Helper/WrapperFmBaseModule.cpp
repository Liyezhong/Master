/****************************************************************************/
/*! \file WrapperFmBaseModule.cpp
 *
 *  \brief  Implementation file for class WrapperFmBaseModule
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

#include "ServiceDeviceController/Include/DeviceProcessor/Helper/ResetData.h"
#include "ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmBaseModule.h"
#include "Global/Include/Utils.h"
#include <QTextStream>

/****************************************************************************/
/*!
 *  \brief Constructor of class WrapperFmBaseModule
 *
 *  \iparam   Name         The Name of base module used for QtScript
 *                         identifier.
 *  \iparam   pBaseModule  Pointer to CBaseModule
 *  \iparam   pParent      Optional pointer to parent for QObject Hierarchy.
 *
 */
/****************************************************************************/
WrapperFmBaseModule::WrapperFmBaseModule(QString Name, CBaseModule *pBaseModule, QObject *pParent):
    WrapperBase(Name, pParent, pBaseModule), m_pBaseModule(pBaseModule)
{
    Reset();

    mp_ResetData = new CResetData(*pBaseModule, this);

    qRegisterMetaType<EmergencyStopReason_t>("EmergencyStopReason_t");
    qRegisterMetaType<PowerState_t>("PowerState_t");
    qRegisterMetaType<NodeState_t>("NodeState_t");

    CONNECTSIGNALSLOT(m_pBaseModule, ReportNodeState(quint32, ReturnCode_t, NodeState_t, EmergencyStopReason_t, PowerState_t),
            this, OnSetNodeState(quint32, ReturnCode_t, NodeState_t, EmergencyStopReason_t, PowerState_t));
    CONNECTSIGNALSLOT(m_pBaseModule, ReportHWInfo(quint32, ReturnCode_t, quint8, quint8, QDate),
            this, OnReportHWInfo(quint32, ReturnCode_t, quint8, quint8, QDate));
    CONNECTSIGNALSLOT(m_pBaseModule, ReportSWInfo(quint32, ReturnCode_t, quint16, QDate),
            this, OnReportSWInfo(quint32, ReturnCode_t, quint16, QDate));
    CONNECTSIGNALSLOT(m_pBaseModule, ReportFormatMemoryAckn(quint32, ReturnCode_t),
            this, OnFormatMemoryAckn(quint32, ReturnCode_t));
    CONNECTSIGNALSLOT(m_pBaseModule, ReportSerialNumber(quint32, ReturnCode_t, QString),
            this, OnSerialNumber(quint32, ReturnCode_t, QString));
    CONNECTSIGNALSLOT(m_pBaseModule, ReportEndTestResult(quint32, ReturnCode_t, TestResult_t, QDate),
            this, OnEndTestResult(quint32, ReturnCode_t, TestResult_t, QDate));
    CONNECTSIGNALSLOT(m_pBaseModule, ReportLoaderInfo(quint32, ReturnCode_t, quint8, quint8, QDate),
            this, OnLoaderInfo(quint32, ReturnCode_t, quint8, quint8, QDate));
    CONNECTSIGNALSLOT(m_pBaseModule, ReportLifeCycleData(quint32, ReturnCode_t, quint32, quint16),
            this, OnLifeCycleData(quint32, ReturnCode_t, quint32, quint16));
    CONNECTSIGNALSLOT(m_pBaseModule, ReportLaunchDate(quint32, ReturnCode_t, bool, QDate),
            this, OnLaunchDate(quint32, ReturnCode_t, bool, QDate));
    CONNECTSIGNALSLOT(m_pBaseModule, ReportBoardName(quint32, ReturnCode_t, QString),
            this, OnBoardName(quint32, ReturnCode_t, QString));
    CONNECTSIGNALSLOT(m_pBaseModule, ReportError(quint32 , quint16 , quint16 , quint16 , QDateTime ),
            this, OnError(quint32 , quint16 , quint16 , quint16 , QDateTime ));
    CONNECTSIGNALSLOT(m_pBaseModule, ReportVoltageState(quint32, ReturnCode_t, PowerState_t, quint16, quint16),
            this, OnVoltageState(quint32, ReturnCode_t, PowerState_t, quint16, quint16));
    CONNECTSIGNALSLOT(m_pBaseModule, ReportCurrentState(quint32, ReturnCode_t, PowerState_t, quint16, quint16),
            this, OnCurrentState(quint32, ReturnCode_t, PowerState_t, quint16, quint16));
    CONNECTSIGNALSLOT(m_pBaseModule, ReportUniqueNumber(quint32, ReturnCode_t, QByteArray),
            this, OnUniqueNumber(quint32, ReturnCode_t, QByteArray));
    CONNECTSIGNALSLOT(m_pBaseModule, ReportModuleSerialNumber(quint32, ReturnCode_t, quint64),
            this, OnModuleSerialNumber(quint32, ReturnCode_t, quint64));
    CONNECTSIGNALSLOT(m_pBaseModule->GetBootLoader(), ReportUpdateBootLoader(quint32, ReturnCode_t),
            this, OnUpdateBootLoader(quint32, ReturnCode_t));

    m_ModuleCurrent = 0 ;
    m_ModuleVoltage = 0;
}

/****************************************************************************/
/*!
 *  \brief Destructor of class WrapperFmBaseModule
 */
/****************************************************************************/
WrapperFmBaseModule::~WrapperFmBaseModule()
{
    try {
        delete mp_ResetData;
    }
    catch(...) {}
}

/****************************************************************************/
/*!
 *  \brief Script-API: Resets the node
 *
 *  This method resets the base module of a CAN node. This also resets all
 *  function modules of that node.
 *
 *  \return true, if the setting value is success else false
 */
/****************************************************************************/
bool WrapperFmBaseModule::ResetNode()
{
    return HandleErrorCode(m_pBaseModule->ReqReset());
}

/****************************************************************************/
/*!
 *  \brief Script-API: Lets the boot loader wait for an update
 *
 *  This method prompts the boot loader to wait for a firmware update, even
 *  if a valid firmware image is available on the node.
 *
 *  \iparam Wait  Wait for an update (true) or not (false)
 */
/****************************************************************************/
void WrapperFmBaseModule::WaitForUpdate(bool Wait)
{
    m_pBaseModule->GetBootLoader()->WaitForUpdate(Wait);
}

/****************************************************************************/
/*!
 *  \brief Script-API: Set node state
 *
 *      This method sets the node state.
 *
 *      Node states:
 *          NODE_STATE_UNDEFINED = 0,    Undefined state
 *          NODE_STATE_BOOTING   = 1,    Node is in boot loader
 *          NODE_STATE_STARTUP   = 2,    Firmware initialization
 *          NODE_STATE_IDENTIFY  = 3,    Node identification
 *          NODE_STATE_CONFIGURE = 4,    Node configuration
 *          NODE_STATE_NORMAL    = 5,    Normal operation mode
 *          NODE_STATE_ASSEMBLY  = 6,    Assembly mode
 *          NODE_STATE_SHUTDOWN  = 7,    Shutdown (going to standby)
 *          NODE_STATE_STANDBY   = 8     Standby
 *
 *  \iparam NodeState  State, which the node should enter.
 *
 *  \return state  string if the setting succeded or not
 *
 */
/****************************************************************************/
QString WrapperFmBaseModule::SetNodeState(quint8 NodeState)
{
    bool ok = HandleErrorCode(m_pBaseModule->SetNodeState(NodeState_t(NodeState)));
    if (!ok) {
        return "request error";
    }

    qint32 ret = m_LoopSetNodeState.exec();

    QString state;
    QTextStream out;
    out.setString(&state);

    if (ret == 1) {
        out << "SUCCESS";
    }
    else
        out << "ERROR";

    out << " Set " <<  NodeState_t(NodeState);

    return state;
}

/****************************************************************************/
/*!
 *  \brief Script-API: Request node state
 *
 *      This method requests the node state.
 *
 *      Node states:
 *          NODE_STATE_UNDEFINED = 0,    Undefined state
 *          NODE_STATE_BOOTING   = 1,    Node is in boot loader
 *          NODE_STATE_STARTUP   = 2,    Firmware initialization
 *          NODE_STATE_IDENTIFY  = 3,    Node identification
 *          NODE_STATE_CONFIGURE = 4,    Node configuration
 *          NODE_STATE_NORMAL    = 5,    Normal operation mode
 *          NODE_STATE_ASSEMBLY  = 6,    Assembly mode
 *          NODE_STATE_SHUTDOWN  = 7,    Shutdown (going to standby)
 *          NODE_STATE_STANDBY   = 8     Standby
 *
 *  \return Numerical representation of the node state or 0 (undefined)
 */
/****************************************************************************/
quint8 WrapperFmBaseModule::ReqNodeState()
{
    bool ok = HandleErrorCode(m_pBaseModule->ReqNodeState());
    if (!ok) {
        return 0;
    }

    qint32 ret = m_LoopReqNodeState.exec();
    if (ret == 1) {
        return (quint8)m_NodeState;
    }
    return 0;
}

/****************************************************************************/
/*!
 *  \brief   slot for node state changes
 *
 *  This slot is connected to the signal ReportNodeState.
 *
 *  \iparam InstanceID  Instance ID of the function module
 *  \iparam ReturnCode  ReturnCode of DeviceControl Layer
 *  \iparam NodeState   actual node state of CAN node
 *  \iparam EmergencyStopReason = No stop, heartbeat, master
 *  \iparam VoltageState = Good, warning, failed, unknown
 */
/****************************************************************************/
void WrapperFmBaseModule::OnSetNodeState(quint32 InstanceID, ReturnCode_t ReturnCode, NodeState_t NodeState,
                                         EmergencyStopReason_t EmergencyStopReason, PowerState_t VoltageState)
{
    Q_UNUSED(InstanceID)

//    QString str = QString("%1 %2 %3 %4 %5").arg(InstanceID).arg(ReturnCode).arg(NodeState).arg(EmergencyStopReason).arg(VoltageState);

//    qDebug()<< "OnSetNodeState ------- "<< str;

    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    } else {
        m_NodeState = NodeState;

        QString state;
        QTextStream out;
        out.setString(&state);
        out << "NOTICE: Node State is " <<  m_NodeState;
        Log(state);

        QString EmergencyStopString;

        switch(EmergencyStopReason) {
            case RESET_EMERGENCY_STOP:
                EmergencyStopString = "No Stop";
                break;
            case STOPPED_BY_HEARTBEAT:
                EmergencyStopString = "Heartbeat Loss";
                break;
            case STOPPED_BY_NOTSTOP:
                EmergencyStopString = "Master Request";
                break;
            case STOPPED_BY_VOLTAGE:
                EmergencyStopString = "Supply Voltage";
                break;
            default:
                EmergencyStopString = "Undefined";
                break;
        }

        Log(tr("Emergency Stop Reason: %1; Voltage State: %2")
            .arg(EmergencyStopString).arg(PowerStateToString(VoltageState)));
    }

    if (m_LoopSetNodeState.isRunning()) {
        m_LoopSetNodeState.exit(ret);
    }
    if (m_LoopReqNodeState.isRunning()) {
        m_LoopReqNodeState.exit(ret);
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API: Enter Emergency stop state
 *
 *  This method emits an emergency stop request to all CAN nodes.
 *  Be aware that also this request is send to a specific node, it will
 *  be received and processed by all nodes and all function modules.
 *  Because this request is broadcasted to all nodes there will be no
 *  response received.
 *
 *  \return true, if sending the request was successfull
 *
 */
/****************************************************************************/
bool WrapperFmBaseModule::EmergencyStop()
{
    bool ok = HandleErrorCode(m_pBaseModule->EnterEmcyStop());
    return ok;
}

/****************************************************************************/
/*!
 *  \brief Script-API: Exit emergency stop state
 *
 *  This method emits an emergency stop exit request to all CAN nodes.
 *  Be aware that also this request is send to a specific node, it will
 *  be received and processed by all nodes and all function modules.
 *  Because this request is broadcasted to all nodes there will be no
 *  response received.
 *
 *  \return true, if sending the exit request was successfull
 *
 */
/****************************************************************************/
bool WrapperFmBaseModule::ExitEmergencyStop()
{
    bool ok = HandleErrorCode(m_pBaseModule->ExitEmcyStop());
    return ok;
}

/****************************************************************************/
/*!
 *  \brief Script-API: Get hardware info
 *
 *  This method get the hardware version and production of a CAN node.
 *
 *  \return hwInfo string with hardware version number and production date
 *
 */
/****************************************************************************/
QString WrapperFmBaseModule::GetHWInfo()
{
    bool ok = HandleErrorCode(m_pBaseModule->ReqHWInfo());
    if (!ok) {
        // TODO: use const String from BaseWrapper
        return "request error";
    }

    qint32 ret = m_LoopGetHWInfo.exec();

    QString hwInfo;
    QTextStream out;
    out.setString(&hwInfo);

    if (ret == 1) {
        QString date = QString("%1-%2-%3").arg(m_HWInfo.Date.year()).arg(m_HWInfo.Date.month()).arg(m_HWInfo.Date.day());

        out << dec << m_HWInfo.VersionMajor << "/" << m_HWInfo.VersionMinor << "/" << date;

        //out << "HW Info : version = " << dec << m_HWInfo.VersionMajor << "/" << m_HWInfo.VersionMinor;
        //out << ", production date = " << m_HWInfo.Date.toString();
    }
    else
        out << "error";

    return hwInfo;
}

/****************************************************************************/
/*!
 *  \brief  Slot for hardware info
 *
 *  \iparam ReturnCode      ReturnCode of DeviceControl Layer
 *  \iparam VersionMajor    Version major number
 *  \iparam VersionMinor    Version minor number
 *  \iparam Date            Production date
 */
/****************************************************************************/
void WrapperFmBaseModule::OnReportHWInfo(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, quint8 VersionMajor,
                                         quint8 VersionMinor, QDate Date)
{
    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    } else {
        m_HWInfo.VersionMajor = VersionMajor;
        m_HWInfo.VersionMinor = VersionMinor;
        m_HWInfo.Date         = Date;
    }

    if (m_LoopGetHWInfo.isRunning()) {
        m_LoopGetHWInfo.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement for GetHWInfo (version major: %1, version minor: %2).")
		     .arg(m_HWInfo.VersionMajor).arg(m_HWInfo.VersionMinor));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API: Get software info
 *
 *  This method get the software version and creation of the node's base module.
 *
 *  \return swInfo string with software version number and creation date
 *
 */
/****************************************************************************/
QString WrapperFmBaseModule::GetSWInfo()
{
    bool ok = HandleErrorCode(m_pBaseModule->ReqSWInfo());
    if (!ok) {
        // TODO: use const String from BaseWrapper
        return "request error";
    }

    qint32 ret = m_LoopGetSWInfo.exec();

    QString swInfo;
    QTextStream out;
    out.setString(&swInfo);

    if (ret == 1) {
        QString date = QString("%1-%2-%3").arg(m_SWInfo.Date.year()).arg(m_SWInfo.Date.month()).arg(m_SWInfo.Date.day());
        out << dec << m_SWInfo.Version << "/" << date;
      //  out << "SW Info <Base module> : version = " << dec << m_SWInfo.Version;
      //  out << ", creation date = " << m_SWInfo.Date.toString();
    }
    else
        out << "error";

    return swInfo;
}

/****************************************************************************/
/*!
 *  \brief   slot for software info
 *
 *  \iparam InstanceID      ID of the FM (currently not needed)
 *  \iparam ReturnCode      ReturnCode of DeviceControl Layer
 *  \iparam Version         Version number
 *  \iparam Date            Production date
 *
 */
/****************************************************************************/
void WrapperFmBaseModule::OnReportSWInfo(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, quint16 Version, QDate Date)
{
    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    } else {
        m_SWInfo.Version = Version;
        m_SWInfo.Date    = Date;
    }

    if (m_LoopGetSWInfo.isRunning()) {
        m_LoopGetSWInfo.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement for GetSWInfo (version major: %1.")
		.arg(m_HWInfo.VersionMajor));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API: Get software version of all modules
 *
 *  This method get the software version of base module and all function modules.
 *
 *  \return swVersion string with software version numbers of all modules
 *
 */
/****************************************************************************/
QString WrapperFmBaseModule::GetSWVersions()
{
    QString swVersionStr;
    QTextStream out;
    out.setString(&swVersionStr);

    // version numbers from CAN-HardwareID msg
    out << "CAN Proctocol Version = " << m_pBaseModule->GetCANVersion() << "\n";
    out << "SW Version = " << m_pBaseModule->GetSWVersion() << "\n";

    // sw version numbers from CAN-Config msg
    quint16 moduleID;
    for (moduleID = MODULE_ID_BASEMODULE; moduleID <= MODULE_ID_UART; moduleID++)
    {
        quint16 swVersion = m_pBaseModule->GetModuleSWVersion((CModuleConfig::CANObjectType_t)moduleID);
        if (0 == swVersion)
            continue;
        out << "SW Version <";
        switch (moduleID)
        {
        case MODULE_ID_BASEMODULE:
            out << "MODULE_ID_BASEMODULE";
            break;
        case MODULE_ID_DIGITAL_OUT:
            out << "MODULE_ID_DIGITAL_OUT";
            break;
        case MODULE_ID_DIGITAL_IN:
            out << "MODULE_ID_DIGITAL_IN";
            break;
        case MODULE_ID_ANALOG_OUT:
            out << "MODULE_ID_ANALOG_OUT";
            break;
        case MODULE_ID_ANALOG_IN:
            out << "MODULE_ID_ANALOG_IN";
            break;
        case MODULE_ID_STEPPER:
            out << "MODULE_ID_STEPPER";
            break;
        case MODULE_ID_JOYSTICK:
            out << "MODULE_ID_JOYSTICK";
            break;
        case MODULE_ID_RFID11785:
            out << "MODULE_ID_RFID11785";
            break;
        case MODULE_ID_RFID15693:
            out << "MODULE_ID_RFID15693";
            break;
        case MODULE_ID_TEMPERATURE:
            out << "MODULE_ID_TEMPERATURE";
            break;
        case MODULE_ID_UART:
            out << "MODULE_ID_UART";
            break;
        case MODULE_ID_SIMUALTION:
            out << "MODULE_ID_SIMUALTION";
            break;
        default:
            out << "unknown module-ID";
            break;
        }
        out << "> = " << dec <<  swVersion << "\n";
    }

    swVersionStr.chop(1);
    return swVersionStr;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Request a data reset
 *
 *      This method resets the non-volatile storage data of all partitions on
 *      a Slave.
 *
 *  \return true, if the setting value is success else false
 */
/****************************************************************************/
bool WrapperFmBaseModule::ReqDataReset()
{
    return HandleErrorCode(mp_ResetData->ReqDataReset());
}


/****************************************************************************/
/*!
 *  \brief  Script-API: Request the formatting of the memory
 *
 *      This method formats all partitions on the non-volatile storage of a
 *      Slave.
 *
 *  \return true, if the setting value is success else false
 */
/****************************************************************************/
bool WrapperFmBaseModule::ReqFormatMemory()
{
    bool ok = HandleErrorCode(m_pBaseModule->ReqFormatMemory());
    if (!ok) {
        return false;
    }

    qint32 ret = m_LoopFormatMemory.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief  Slot for the format memory
 *
 *      This slot is connected to the signal ReportFormatMemoryAckn.
 *
 *  \iparam InstanceID = Instance ID of the function module
 *  \iparam ReturnCode = ReturnCode of DeviceControl Layer
 */
/****************************************************************************/
void WrapperFmBaseModule::OnFormatMemoryAckn(quint32 InstanceID, ReturnCode_t ReturnCode)
{
    Q_UNUSED(InstanceID)

    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    if (m_LoopFormatMemory.isRunning()) {
        m_LoopFormatMemory.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Request the serial number of the slave
 *
 *      This method fetches the serial number of the slave's microcontroller.
 *
 *  \return Serial number or an empty string in case of an error
 */
/****************************************************************************/
QString WrapperFmBaseModule::ReqSerialNumber()
{
    bool ok = HandleErrorCode(m_pBaseModule->ReqSerialNumber());
    if (!ok) {
        return QString();
    }

    qint32 ret = m_LoopSerialNumber.exec();
    if (ret == 1) {
        return m_SerialNumber;
    }
    return QString();
}

/****************************************************************************/
/*!
 *  \brief  Slot for the serial number
 *
 *      This slot is connected to the signal ReportSerialNumber.
 *
 *  \iparam InstanceID = Instance ID of the function module
 *  \iparam ReturnCode = ReturnCode of DeviceControl Layer
 *  \iparam SerialNumber = Serial number string
 */
/****************************************************************************/
void WrapperFmBaseModule::OnSerialNumber(quint32 InstanceID, ReturnCode_t ReturnCode, QString SerialNumber)
{
    Q_UNUSED(InstanceID)
    m_SerialNumber = SerialNumber;

    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    if (m_LoopSerialNumber.isRunning()) {
        m_LoopSerialNumber.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Request the end test result of the slave
 *
 *      This method fetches the end test result and the end test date.
 *
 *  \return true, if the setting value is success else false
 */
/****************************************************************************/
bool WrapperFmBaseModule::ReqEndTestResult()
{
    bool ok = HandleErrorCode(m_pBaseModule->ReqEndTestResult());
    if (!ok) {
        return false;
    }

    qint32 ret = m_LoopEndTestResult.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief  Slot for the end test result
 *
 *      This slot is connected to the signal ReportEndTestResult.
 *
 *  \iparam InstanceID = Instance identifier of this function module instance
 *  \iparam ReturnCode = DCL_ERR_FCT_CALL_SUCCESS, otherwise the error code
 *  \iparam Result = Open, passed, failed
 *  \iparam Date = End test date
 */
/****************************************************************************/
void WrapperFmBaseModule::OnEndTestResult(quint32 InstanceID, ReturnCode_t ReturnCode, TestResult_t Result, QDate Date)
{
    Q_UNUSED(InstanceID)
    QString ResultString;

    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    else {
        switch(Result) {
            case TEST_OPEN:
                ResultString = "Open";
                break;
            case TEST_PASSED:
                ResultString = "Passed";
                break;
            case TEST_FAILED:
                ResultString = "Failed";
                break;
            default:
                ResultString = "Undefined";
                break;
        }

        m_EndTestResultInfo.TestResult = ResultString;

        ResultString.append(tr(" (%1)").arg((qint32)Result));


        m_EndTestResultInfo.Date = Date;
    }

    if (m_LoopEndTestResult.isRunning()) {
        if (ret == 1) {
            Log(tr("End Test Result: %1; End Test Date: %2").arg(ResultString).arg(Date.toString()));
        }
        m_LoopEndTestResult.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Request the boot loader version information
 *
 *      This method fetches the version and the release date of the boot
 *      loader.
 *
 *  \return true, if the setting value is success else false
 */
/****************************************************************************/
bool WrapperFmBaseModule::ReqLoaderInfo()
{
    bool ok = HandleErrorCode(m_pBaseModule->ReqLoaderInfo());
    if (!ok) {
        return false;
    }

    qint32 ret = m_LoopLoaderInfo.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief  Slot for the boot loader information
 *
 *      This slot is connected to the signal ReportLoaderInfo.
 *
 *  \iparam InstanceID = Instance identifier of this function module instance
 *  \iparam ReturnCode = DCL_ERR_FCT_CALL_SUCCESS, otherwise the error code
 *  \iparam VersionMajor = Boot loader major version number
 *  \iparam VersionMinor = Boot loader minor version number
 *  \iparam Date = Boot loader release date
 */
/****************************************************************************/
void WrapperFmBaseModule::OnLoaderInfo(quint32 InstanceID, ReturnCode_t ReturnCode, quint8 VersionMajor,
                                       quint8 VersionMinor, QDate Date)
{
    Q_UNUSED(InstanceID)

    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    else {
        m_BootloaderInfo.VersionMajor = VersionMajor;
        m_BootloaderInfo.VersionMinor = VersionMinor;
        m_BootloaderInfo.Date = Date;
    }

    if (m_LoopLoaderInfo.isRunning()) {
        if (ret == 1) {
            Log(tr("Boot Loader Version: %1.%2; Boot Loader Release Date: %3")
                .arg(VersionMajor).arg(VersionMinor).arg(Date.toString()));
        }
        m_LoopLoaderInfo.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API: Get bootloader info
 *
 *  This method get the bootloader version and production of a CAN node.
 *
 *  \return hwInfo string with hardware version number and production date
 *
 */
/****************************************************************************/
QString WrapperFmBaseModule::GetBootloaderInfo()
{
    bool ok = HandleErrorCode(m_pBaseModule->ReqLoaderInfo());
    if (!ok) {
        // TODO: use const String from BaseWrapper
        return "request error";
    }

    qint32 ret = m_LoopLoaderInfo.exec();

    QString loaderInfo;
    QTextStream out;
    out.setString(&loaderInfo);

    if (ret == 1) {
        QString date = QString("%1-%2-%3").arg(m_BootloaderInfo.Date.year()).arg(m_BootloaderInfo.Date.month()).arg(m_BootloaderInfo.Date.day());
        out << dec << m_BootloaderInfo.VersionMajor << "/" << m_BootloaderInfo.VersionMinor << "/" << date;
    }
    else
        out << "error";

    return loaderInfo;
}

/****************************************************************************/
/*!
 *  \brief Script-API: Get end test result info
 *
 *  This method get the end test result and date of board.
 *
 *  \return endTestInfo string with test result and test date
 *
 */
/****************************************************************************/
QString WrapperFmBaseModule::GetEndTestResultInfo()
{
    bool ok = HandleErrorCode(m_pBaseModule->ReqEndTestResult());
    if (!ok) {
        // TODO: use const String from BaseWrapper
        return "request error";
    }

    qint32 ret = m_LoopEndTestResult.exec();

    QString endTestInfo;
    QTextStream out;
    out.setString(&endTestInfo);

    if (ret == 1) {
        QString date = QString("%1-%2-%3").arg(m_EndTestResultInfo.Date.year()).arg(m_EndTestResultInfo.Date.month()).arg(m_EndTestResultInfo.Date.day());
        out << dec << m_EndTestResultInfo.TestResult << "/"<< date;
    }
    else
        out << "error";

    return endTestInfo;
}


/****************************************************************************/
/*!
 *  \brief  Script-API: Request the lifecycle data
 *
 *      This method fetches the total operation time of the slave and the
 *      number of times it has already been booted.
 *
 *  \return true, if the setting value is success else false
 */
/****************************************************************************/
bool WrapperFmBaseModule::ReqLifeCycleData()
{
    bool ok = HandleErrorCode(m_pBaseModule->ReqLifeCycleData());
    if (!ok) {
        return false;
    }

    qint32 ret = m_LoopLifeCycleData.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief  Slot for the lifecycle data
 *
 *      This slot is connected to the signal ReportLifeCycleData.
 *
 *  \iparam InstanceID = Instance identifier of this function module instance
 *  \iparam ReturnCode = DCL_ERR_FCT_CALL_SUCCESS, otherwise the error code
 *  \iparam OperationTime = Total microcontroller uptime in minutes
 *  \iparam StartupCycles = Number of times the microcontroller was started
 */
/****************************************************************************/
void WrapperFmBaseModule::OnLifeCycleData(quint32 InstanceID, ReturnCode_t ReturnCode,
                                          quint32 OperationTime, quint16 StartupCycles)
{
    Q_UNUSED(InstanceID)

    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }

    if (m_LoopLifeCycleData.isRunning()) {
        if (ret == 1) {
            Log(tr("Operation Time: %1 minutes; Startup cycles: %2").arg(OperationTime).arg(StartupCycles));
        }
        m_LoopLifeCycleData.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Request the launch date
 *
 *      This method fetches the launch state and the launch date of the
 *      slave. The launch state indicates that the launch date is valid. The
 *      launch date is the date of the first start of the slave.
 *
 *  \return true, if the setting value is success else false
 */
/****************************************************************************/
bool WrapperFmBaseModule::ReqLaunchDate()
{
    bool ok = HandleErrorCode(m_pBaseModule->ReqLaunchDate());
    if (!ok) {
        return false;
    }

    qint32 ret = m_LoopLaunchDate.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief  Slot for the launch date
 *
 *      This slot is connected to the signal ReportLaunchDate.
 *
 *  \iparam InstanceID = Instance identifier of this function module instance
 *  \iparam ReturnCode = DCL_ERR_FCT_CALL_SUCCESS, otherwise the error code
 *  \iparam Launched = Launched or not launched
 *  \iparam Date = Launch date
 */
/****************************************************************************/
void WrapperFmBaseModule::OnLaunchDate(quint32 InstanceID, ReturnCode_t ReturnCode, bool Launched, QDate Date)
{
    Q_UNUSED(InstanceID)

    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }

    if (m_LoopLaunchDate.isRunning()) {
        Log(tr("Launch State: %1; Launch Date: %2").arg(Launched).arg(Date.toString()));
        m_LoopLaunchDate.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Request the board name
 *
 *      This method fetches the board name string from the slave.
 *
 *  \return true, if the setting value is success else false
 */
/****************************************************************************/
QString WrapperFmBaseModule::ReqBoardName()
{
    bool ok = HandleErrorCode(m_pBaseModule->ReqBoardName());
    if (!ok) {
        return QString();
    }

    qint32 ret = m_LoopBoardName.exec();
    if (ret == 1) {
        return m_BoardName;
    }
    return QString();
}

/****************************************************************************/
/*!
 *  \brief  Slot for the board name
 *
 *      This slot is connected to the signal ReportBoardName.
 *
 *  \iparam InstanceID = Instance ID of the function module
 *  \iparam ReturnCode = ReturnCode of DeviceControl Layer
 *  \iparam BoardName = Name of the board
 */
/****************************************************************************/
void WrapperFmBaseModule::OnBoardName(quint32 InstanceID, ReturnCode_t ReturnCode, QString BoardName)
{
    Q_UNUSED(InstanceID)
    m_BoardName = BoardName;

    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    if (m_LoopBoardName.isRunning()) {
        m_LoopBoardName.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API: Configures the voltage monitor
 *
 *  This method configures the monitor of the main supply voltage of a slave.
 *
 *  \iparam Enable = Enables or disables monitoring
 *  \iparam Filter = Number of coefficients for the running average filter
 *  \iparam SamplingPeriod = Time between two input samples in milliseconds
 *  \iparam GoodThreshold = The state is good when it is above this limit
 *  \iparam FailThreshold = The state is failed when it is below this limit
 *
 *  \return true, if the setting value is success else false
 */
/****************************************************************************/
bool WrapperFmBaseModule::ConfigureVoltageMonitor(bool Enable, quint8 Filter, quint8 SamplingPeriod,
                                                  quint16 GoodThreshold, quint16 FailThreshold)
{
    return HandleErrorCode(m_pBaseModule->ConfigureVoltageMonitor(Enable, Filter, SamplingPeriod,
                                                                  GoodThreshold, FailThreshold));
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Request the voltage monitor state
 *
 *      This method fetches state of the monitor watching the main supply
 *      voltage of the slave.
 *
 *  \return true, if the setting value is success else false
 */
/****************************************************************************/
bool WrapperFmBaseModule::ReqVoltageState()
{
    bool ok = HandleErrorCode(m_pBaseModule->ReqVoltageState());
    if (!ok) {
        return false;
    }

    qint32 ret = m_LoopVoltageState.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief  Slot for the voltage state
 *
 *      This slot is connected to the signal ReportVoltageState.
 *
 *  \iparam InstanceID = Instance ID of the function module
 *  \iparam ReturnCode = ReturnCode of DeviceControl Layer
 *  \iparam State = Good, warning, failed, unknown
 *  \iparam Value = Voltage measured in millivolts
 *  \iparam Failures = Number of voltage failures
 */
/****************************************************************************/
void WrapperFmBaseModule::OnVoltageState(quint32 InstanceID, ReturnCode_t ReturnCode, PowerState_t State,
                                         quint16 Value, quint16 Failures)
{
    Q_UNUSED(InstanceID)

    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    else {
        m_ModuleVoltage = Value;

        qDebug()<<"InstanceID:"<<InstanceID<<"  Voltage State:"<<PowerStateToString(State)<<" Voltage:"<<Value;

        Log(tr("Voltage State: %1; Voltage: %2 mV; Failures: %3")
            .arg(PowerStateToString(State)).arg(Value).arg(Failures));
    }

    if (m_LoopVoltageState.isRunning()) {
        m_LoopVoltageState.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API: Configures the current monitor
 *
 *  This method configures the monitor of the main supply current of a slave.
 *
 *  \iparam Enable = Enables or disables monitoring
 *  \iparam Filter = Number of coefficients for the running average filter
 *  \iparam SamplingPeriod = Time between two input samples in milliseconds
 *  \iparam GoodThreshold = The state is good when it is below this limit
 *  \iparam FailThreshold = The state is failed when it is above this limit
 *
 *  \return true, if the setting value is success else false
 */
/****************************************************************************/
bool WrapperFmBaseModule::ConfigureCurrentMonitor(bool Enable, quint8 Filter, quint8 SamplingPeriod,
                                                  quint16 GoodThreshold, quint16 FailThreshold)
{
    return HandleErrorCode(m_pBaseModule->ConfigureCurrentMonitor(Enable, Filter, SamplingPeriod,
                                                                  GoodThreshold, FailThreshold));
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Request the current monitor state
 *
 *      This method fetches state of the monitor watching the main supply
 *      current of the slave.
 *
 *  \return true, if the setting value is success else false
 */
/****************************************************************************/
bool WrapperFmBaseModule::ReqCurrentState()
{
    bool ok = HandleErrorCode(m_pBaseModule->ReqCurrentState());
    if (!ok) {
        return false;
    }

    qint32 ret = m_LoopCurrentState.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief  Slot for the current state
 *
 *      This slot is connected to the signal ReportCurrentState.
 *
 *  \iparam InstanceID = Instance ID of the function module
 *  \iparam ReturnCode = ReturnCode of DeviceControl Layer
 *  \iparam State = Good, warning, failed, unknown
 *  \iparam Value = Current measured in milliamperes
 *  \iparam Failures = Number of current failures
 */
/****************************************************************************/
void WrapperFmBaseModule::OnCurrentState(quint32 InstanceID, ReturnCode_t ReturnCode,
                                         PowerState_t State, quint16 Value, quint16 Failures)
{
    Q_UNUSED(InstanceID)

    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    else {

        m_ModuleCurrent = Value;

        Log(tr("Current State: %1; Current: %2 mA; Failures: %3")
            .arg(PowerStateToString(State)).arg(Value).arg(Failures));
    }

    if (m_LoopCurrentState.isRunning()) {
        m_LoopCurrentState.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Request the unique number of the slave
 *
 *      This method fetches the unique number of the slave's microcontroller.
 *      It returns the unique number as a string of hexadecimal numbers.
 *
 *  \return Unique number or an empty string in case of an error
 */
/****************************************************************************/
QString WrapperFmBaseModule::ReqUniqueNumber()
{
    bool ok = HandleErrorCode(m_pBaseModule->ReqUniqueNumber());
    if (!ok) {
        return QString();
    }

    QTextStream out;

    qint32 ret = m_LoopUniqueNumber.exec();
    if (ret == 1) {
        QString UniqueNumber;

        for (qint8 i = 0; i < m_UniqueNumber.size(); i++) {
            QString Digit = QString("%1").arg((qint32)m_UniqueNumber.at(i), 2, 16, QChar('0'));
            out << Digit;
            UniqueNumber.append(Digit.right(2));
        }
        return UniqueNumber;
    }
    return QString();
}

/****************************************************************************/
/*!
 *  \brief  Slot for the unique number
 *
 *      This slot is connected to the signal ReportUniqueNumber.
 *
 *  \iparam InstanceID = Instance ID of the function module
 *  \iparam ReturnCode = ReturnCode of DeviceControl Layer
 *  \iparam UniqueNumber = Byte array of the unique number
 */
/****************************************************************************/
void WrapperFmBaseModule::OnUniqueNumber(quint32 InstanceID, ReturnCode_t ReturnCode, QByteArray UniqueNumber)
{
    Q_UNUSED(InstanceID)
    m_UniqueNumber = UniqueNumber;

    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    if (m_LoopUniqueNumber.isRunning()) {
        m_LoopUniqueNumber.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Request the module serial number
 *
 *      This method fetches the serial number of the module. It returns the
 *      unique number as a string of decimal numbers.
 *
 *  \return Unique number or an empty string in case of an error
 */
/****************************************************************************/
QString WrapperFmBaseModule::ReqModuleSerialNumber()
{
    bool ok = HandleErrorCode(m_pBaseModule->ReqModuleSerialNumber());
    if (!ok) {
        return QString();
    }

    qint32 ret = m_LoopModuleSerial.exec();
    if (ret == 1) {
        return QString::number(m_ModuleSerialNumber);
    }
    return QString();
}

/****************************************************************************/
/*!
 *  \brief  Slot for the module serial number
 *
 *      This slot is connected to the signal ReportUniqueNumber.
 *
 *  \iparam InstanceID = Instance ID of the function module
 *  \iparam ReturnCode = ReturnCode of DeviceControl Layer
 *  \iparam ModuleSerialNumber = Module serial number
 */
/****************************************************************************/
void WrapperFmBaseModule::OnModuleSerialNumber(quint32 InstanceID, ReturnCode_t ReturnCode, quint64 ModuleSerialNumber)
{
    Q_UNUSED(InstanceID)
    m_ModuleSerialNumber = ModuleSerialNumber;

    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    if (m_LoopModuleSerial.isRunning()) {
        m_LoopModuleSerial.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API: Sets the module serial number
 *
 *  This method sets the module serial number.
 *
 *  \iparam ModuleSerialNumber = Module serial number
 *
 *  \return true, if the setting value is success else false
 */
/****************************************************************************/
bool WrapperFmBaseModule::SetModuleSerialNumber(QString ModuleSerialNumber)
{
    bool Ok;
    quint64 Number = ModuleSerialNumber.toULongLong(&Ok);

    if (!Ok) {
        return false;
    }
    return HandleErrorCode(m_pBaseModule->SetModuleSerialNumber(Number));
}

/****************************************************************************/
/*!
 *  \brief Script-API: Updates the boot loader image
 *
 *  This method updates the boot loader on a Slave using its boot loader. It
 *  reads a file from the local storage and forwards it to the base module
 *  component of the Slave.
 *
 *  \iparam BootLoaderPath The path to the boot loader image file
 *
 *  \return true, if the setting value is success else false
 */
/****************************************************************************/
bool WrapperFmBaseModule::UpdateBootLoader(const QString &BootLoaderPath)
{
    bool ok = HandleErrorCode(m_pBaseModule->GetBootLoader()->UpdateBootLoader(BootLoaderPath));
    if (!ok) {
        return false;
    }

    qint32 ret = m_LoopUpdateBootLoader.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief   slot for the boot loader update
 *
 *  This slot is connected to the signal ReportUpdateBootLoader.
 *
 *  \iparam InstanceID  Instance ID of the function module
 *  \iparam ReturnCode  ReturnCode of DeviceControl Layer
 */
/****************************************************************************/
void WrapperFmBaseModule::OnUpdateBootLoader(quint32 InstanceID, ReturnCode_t ReturnCode)
{
    Q_UNUSED(InstanceID)

    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    if (m_LoopUpdateBootLoader.isRunning()) {
        m_LoopUpdateBootLoader.exit(ret);
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
void WrapperFmBaseModule::Reset()
{
}

/****************************************************************************/
/*!
 *  \brief  Converts a power state enum into a printable string
 *
 *  \iparam State = Good, warning, failed, unknown
 *
 *  \return String representation of the power state
 */
/****************************************************************************/
QString WrapperFmBaseModule::PowerStateToString(PowerState_t State)
{
    QString String;

    switch(State) {
        case POWER_GOOD:
            String = "Good";
            break;
        case POWER_WARNING:
            String = "Warning";
            break;
        case POWER_FAILED:
            String = "Failed";
            break;
        case POWER_UNKNOWN:
            String = "Unknown";
            break;
        default:
            String = "Undefined";
            break;
    }
    String.append(tr(" (%1)").arg((qint32)State));
    return String;
}
