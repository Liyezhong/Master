/****************************************************************************/
/*! \file ResetData.cpp
 *
 *  \brief  Implementation file for class CResetData
 *
 *  \version  0.1
 *  \date     2012-11-21
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

#include "ResetData.h"
#include "WrapperBase.h"
#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include <QDebug>

/****************************************************************************/
/*!
 *  \brief  Constructor of class CResetData
 *
 *  \iparam Module = Module used to reset the data
 *  \iparam p_Parent = Pointer to the parent object
 */
/****************************************************************************/
CResetData::CResetData(CModule &Module, QObject *p_Parent) :
    QObject(p_Parent), m_Module(Module)
{
    CONNECTSIGNALSLOT(&m_Module, ReportDataResetAckn(quint32, ReturnCode_t),
                      this, OnDataResetAckn(quint32, ReturnCode_t));
}

/****************************************************************************/
/*!
 *  \brief  Request a data reset
 *
 *      This method resets the non-volatile storage data of all partitions on
 *      a Slave.
 *
 *  \return DCL error code
 */
/****************************************************************************/
ReturnCode_t CResetData::ReqDataReset()
{
    ReturnCode_t ReturnCode = m_Module.ReqDataReset();
    if (DCL_ERR_FCT_CALL_SUCCESS != ReturnCode) {
        return ReturnCode;
    }

    qint32 ret = m_LoopDataReset.exec();
    if (ret == 1) {
        return DCL_ERR_FCT_CALL_SUCCESS;
    }

    return DCL_ERR_FCT_CALL_FAILED;
}

/****************************************************************************/
/*!
 *  \brief  Slot for the data reset
 *
 *      This slot is connected to the signal ReportDataResetAckn.
 *
 *  \iparam InstanceID = Instance ID of the function module
 *  \iparam ReturnCode = ReturnCode of DeviceControl Layer
 */
/****************************************************************************/
void CResetData::OnDataResetAckn(quint32 InstanceID, ReturnCode_t ReturnCode)
{
    Q_UNUSED(InstanceID)

    qint32 ret = 1;
    if (DCL_ERR_FCT_CALL_SUCCESS != ReturnCode) {
        ret = -1;
    }
    if (m_LoopDataReset.isRunning()) {
        m_LoopDataReset.exit(ret);
    } else {
        qDebug() << tr("NOTICE: Unexpected action acknowledgement.");
    }
}
