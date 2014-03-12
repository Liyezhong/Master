/****************************************************************************/
/*! \file ResetData.h
 *
 *  \brief  Definition file for class CResetData
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

#ifndef RESETDATA_H
#define RESETDATA_H

#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "DeviceControl/Include/SlaveModules/Module.h"
#include <QEventLoop>
#include <QObject>

using namespace DeviceControl;

/****************************************************************************/
/*!
 *  \brief  Resets the data partition of a slave module
 *
 *      The reset data command is implemented in the same way in different
 *      slave modules. This implements a single function call that returns,
 *      when the command is finished.
 */
/****************************************************************************/
class CResetData : public QObject {
    Q_OBJECT

public:
    CResetData(CModule &Module, QObject *p_Parent);
    ReturnCode_t ReqDataReset();

private slots:
    void OnDataResetAckn(quint32 InstanceID, ReturnCode_t HdlInfo);

private:
    CModule &m_Module;          //!< Module used to reset the data
    QEventLoop m_LoopDataReset; //!< Loop for blocking commands.
};

#endif // RESETDATA_H
