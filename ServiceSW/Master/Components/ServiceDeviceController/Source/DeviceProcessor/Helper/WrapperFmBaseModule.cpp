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

#include <QTextStream>
#include "ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmBaseModule.h"

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
    WrapperBase(Name, pParent), m_pBaseModule(pBaseModule)
{
    Reset();

    connect(m_pBaseModule, SIGNAL(ReportReset(quint32, ReturnCode_t)),
            this, SLOT(OnResetNode(quint32, ReturnCode_t)));

    connect(m_pBaseModule, SIGNAL(ReportHWInfo(quint32, ReturnCode_t, quint8, quint8, quint8, quint8, quint8)),
            this, SLOT(OnReportHWInfo(quint32, ReturnCode_t, quint8, quint8, quint8, quint8, quint8)));

    connect(m_pBaseModule, SIGNAL(ReportSWInfo(quint32, ReturnCode_t, quint16, quint8, quint8, quint8)),
            this, SLOT(OnReportSWInfo(quint32, ReturnCode_t, quint16, quint8, quint8, quint8)));

    connect(m_pBaseModule, SIGNAL(ReportNodeState(quint32, ReturnCode_t, NodeState_t)),
            this, SLOT(OnSetNodeState(quint32, ReturnCode_t, NodeState_t)));

    connect(m_pBaseModule, SIGNAL(ReportError(quint32, quint16, QDateTime)),
            this, SLOT(OnError(quint32, quint16, QDateTime)));

    connect(m_pBaseModule, SIGNAL(ReportSerialNumber(quint32, ReturnCode_t, QString)),
            this, SLOT(OnReportSerialNumber(quint32,ReturnCode_t,QString)));

}

/****************************************************************************/
/*!
 *  \brief Script-API: Resets the node
 *
 *  This method resets the base module of a CAN node. This also resets all
 *  function modules of that node.
 *
 *  Examples:
 *  \dontinclude basemodule.js
 *  \skipline [BaseModule.ResetNode]
 *  \until    [BaseModule.ResetNode]
 *
 *  \return true, if the setting value is success else false
 *
 */
/****************************************************************************/
bool WrapperFmBaseModule::ResetNode()
{
    bool ok = HandleErrorCode(m_pBaseModule->ReqReset());
    if (!ok) {
        return false;
    }
    qint32 ret = m_LoopResetNode.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief Script-API: Lets the boot loader wait for an update
 *
 *  This method prompts the boot loader to wait for a firmware update, even
 *  if a valid firmware image is available on the node.
 *
 *  Examples:
 *  \dontinclude basemodule.js
 *  \skipline [BaseModule.WaitForUpdate]
 *  \until    [BaseModule.WaitForUpdate]
 *
 *  \iparam Wait  Wait for an update (true) or not (false)
 *
 */
/****************************************************************************/
void WrapperFmBaseModule::WaitForUpdate(bool Wait)
{
    m_pBaseModule->GetBootLoader()->WaitForUpdate(Wait);
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
void WrapperFmBaseModule::OnResetNode(quint32 InstanceID, ReturnCode_t ReturnCode)
{
    Q_UNUSED(InstanceID)

    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    if (m_LoopResetNode.isRunning()) {
        m_LoopResetNode.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}
bool WrapperFmBaseModule::EmergencyStop()
{
    ReturnCode_t retCode = m_pBaseModule->EnterEmcyStop();
    bool ok = HandleErrorCode(retCode);
    return ok;
}
bool WrapperFmBaseModule::ExitEmergencyStop()
{
    ReturnCode_t retCode = m_pBaseModule->ExitEmcyStop();
    bool ok = HandleErrorCode(retCode);
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
        out << "Board Info : version = " << dec << m_HWInfo.VersionMajor << "/" << m_HWInfo.VersionMinor;
        out << ", production date = " << m_HWInfo.Day << "." << m_HWInfo.Month << ".20" << m_HWInfo.Year;
    }
    else
        out << "error";

    return hwInfo;
}
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


void WrapperFmBaseModule::OnSetNodeState(quint32 InstanceID, ReturnCode_t ReturnCode, NodeState_t NodeState)
{
    Q_UNUSED(InstanceID)

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
      //  Log(tr("Emergency Stop Reason: %1; Voltage State: %2")
      //      .arg(EmergencyStopString).arg(PowerStateToString(VoltageState)));
    }

    if (m_LoopSetNodeState.isRunning()) {
        m_LoopSetNodeState.exit(ret);
    }
}
/****************************************************************************/
/*!
 *  \brief   slot for hardware info
 *
 *  \iparam InstanceID      ID of the FM (currently not needed)
 *  \iparam ReturnCode      ReturnCode of DeviceControl Layer
 *  \iparam VersionMajor    Version major number
 *  \iparam VersionMinor    Version minor number
 *  \iparam Year            production year
 *  \iparam Month           production month
 *  \iparam day             production day
 *
 */
/****************************************************************************/
void WrapperFmBaseModule::OnReportHWInfo(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, quint8 VersionMajor, quint8 VersionMinor, quint8 Year, quint8 Month, quint8 Day)
{
    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    } else {
        m_HWInfo.VersionMajor = VersionMajor;
        m_HWInfo.VersionMinor = VersionMinor;
        m_HWInfo.Year         = Year;
        m_HWInfo.Month        = Month;
        m_HWInfo.Day          = Day;
    }

    if (m_LoopGetHWInfo.isRunning()) {
        m_LoopGetHWInfo.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement for GetHWInfo (version major: %1, version minor: %2).").arg(m_HWInfo.VersionMajor).arg(m_HWInfo.VersionMinor));
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
        out << "SW Info <Base module> : version = " << dec << m_SWInfo.Version;
        out << ", creation date = " << m_SWInfo.Day << "." << m_SWInfo.Month << ".20" << m_SWInfo.Year;
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
 *  \iparam Year            production year
 *  \iparam Month           production month
 *  \iparam day             production day
 *
 */
/****************************************************************************/
void WrapperFmBaseModule::OnReportSWInfo(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, quint16 Version, quint8 Year, quint8 Month, quint8 Day)
{
    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    } else {
        m_SWInfo.Version = Version;
        m_SWInfo.Year    = Year;
        m_SWInfo.Month   = Month;
        m_SWInfo.Day     = Day;
    }

    if (m_LoopGetSWInfo.isRunning()) {
        m_LoopGetSWInfo.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement for GetSWInfo (version major: %1.").arg(m_HWInfo.VersionMajor));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API: Get serial number
 *
 *  This method get the serial number of the node's base module.
 *
 *  \return string with serial number
 *
 */
/****************************************************************************/
QString WrapperFmBaseModule::GetSerialNumber()
{
    bool ok = HandleErrorCode(m_pBaseModule->ReqSerialNumber());
    if (!ok) {
        // TODO: use const String from BaseWrapper
        return "request error";
    }
    qint32 ret = m_LoopGetSerialNumber.exec();

    QString SerialNumber;
    QTextStream out;
    out.setString(&SerialNumber);

    if (ret == 1) {
        out << "<Base module> : Serial Number = " << SerialNumber;
    }
    else
        out << "error";

    return SerialNumber;
}

/****************************************************************************/
/*!
 *  \brief   slot for software info
 *
 *  \iparam InstanceID      ID of the FM (currently not needed)
 *  \iparam ReturnCode      ReturnCode of DeviceControl Layer
 *  \iparam Version         Version number
 *  \iparam Year            production year
 *  \iparam Month           production month
 *  \iparam day             production day
 *
 */
/****************************************************************************/
void WrapperFmBaseModule::OnReportSerialNumber(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, QString SerialNumber)
{
    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    } else {
        m_SerailNumber = SerialNumber;
    }

    if (m_LoopGetSerialNumber.isRunning()) {
        m_LoopGetSerialNumber.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement for GetSerialNumber "));
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
