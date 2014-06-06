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
#include "WrapperBase.h"
#include "DeviceControl/Include/SlaveModules/BaseModule.h"

using namespace DeviceControl;

/****************************************************************************/
/*! \brief Wrapper for the base module
 *
 *  This wrapper class provides a QtScript interface to the base modules.
 *
 *  Examples:
 *   \dontinclude basemodule.js
 *   \skipline [WrapperFmBaseModule]
 *   \until    [WrapperFmBaseModule]
 */
/****************************************************************************/
class WrapperFmBaseModule : public WrapperBase
{
    Q_OBJECT

public:
    WrapperFmBaseModule(QString Name, CBaseModule *pBaseModule, QObject *pParent);

public slots:
    bool ResetNode();
    void WaitForUpdate(bool Wait);
    bool EmergencyStop();
    bool ExitEmergencyStop();
    QString SetNodeState(quint8 NodeState);
    //quint8 ReqNodeState();
    QString GetHWInfo();
    QString GetSWInfo();
    QString GetSerialNumber();
    void Reset();

private slots:
    void OnResetNode(quint32 InstanceID, ReturnCode_t HdlInfo);
    void OnSetNodeState(quint32 InstanceID, ReturnCode_t ReturnCode, NodeState_t NodeState);
    void OnReportHWInfo(quint32 InstanceID, ReturnCode_t ReturnCode, quint8 VersionMajor, quint8 VersionMinor, quint8 Year, quint8 Month, quint8 Day);
    void OnReportSWInfo(quint32 InstanceID, ReturnCode_t ReturnCode, quint16 Version, quint8 Year, quint8 Month, quint8 Day);
    void OnReportSerialNumber(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, QString SerialNumber);

private:
    typedef struct {
        quint8  VersionMajor;
        quint8  VersionMinor;
        quint8  Year;
        quint8  Month;
        quint8  Day;
    } HWInfo_t;

    typedef struct {
        quint16 Version;
        quint8  Year;
        quint8  Month;
        quint8  Day;
    } SWInfo_t;

    CBaseModule *m_pBaseModule;     //!< Pointer to CBaseModule
    QEventLoop   m_LoopSetNodeState;    //!< Loop for blocking commands.
    QEventLoop   m_LoopResetNode;   //!< Loop for blocking commands.
    QEventLoop   m_LoopGetHWInfo;   //!< Loop for blocking commands.
    QEventLoop   m_LoopGetSWInfo;   //!< Loop for blocking commands.
    QEventLoop   m_LoopGetSerialNumber;   //!< Loop for blocking commands.

    HWInfo_t     m_HWInfo;          //!< hardware information
    SWInfo_t     m_SWInfo;          //!< software information
    NodeState_t  m_NodeState;           //!< actual node state
    QString      m_SerailNumber;
};

#endif // WRAPPERFMBASEMODULE_H
