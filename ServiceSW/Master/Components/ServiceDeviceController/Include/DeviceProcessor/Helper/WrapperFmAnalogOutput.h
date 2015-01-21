/****************************************************************************/
/*! \file WrapperFmAnalogOutput.h
 *
 *  \brief  Definition file for class WrapperFmAnalogOutput
 *
 *  \version  0.1
 *  \date     2012-04-04
 *  \author   Jerin Antony
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

#ifndef WRAPPERFMANALOGOUTPUT_H
#define WRAPPERFMANALOGOUTPUT_H

#include <QObject>
#include <QEventLoop>
#include "ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperBase.h"
#include "DeviceControl/Include/SlaveModules/AnalogOutput.h"

using namespace DeviceControl;

/****************************************************************************/
/*! \brief Wrapper for Analog Output function modules
 *
 *  This wrapper class provides a QtScript interface to Analog Output function
 *  modules.
 *
 *  Examples:
 *   \skipline [WrapperFmAnalogOutput]
 *   \until    [WrapperFmAnalogOutput]
 */
/****************************************************************************/
class WrapperFmAnalogOutput : public WrapperBase
{
    Q_OBJECT

public:
    WrapperFmAnalogOutput(QString Name, CAnalogOutput *pAnalogOutput, QObject *pParent);

public slots:
    bool SetValue(quint16 OutputValue, quint16 Duration = 0, quint16 Delay = 0);
    quint16 GetValue();
    void Reset();

private slots:
    void OnSetValue(quint32 /*InstanceID*/, ReturnCode_t HdlInfo, quint16 OutputValue);
    void OnGetValue(quint32 /*InstanceID*/, ReturnCode_t HdlInfo, quint16 OutputValue);

private:
    CAnalogOutput *m_pAnalogOutput;  //!< Pointer to CAnalogOutput

    QEventLoop m_LoopGetValue;  //!< Loop for blocking commands.
    QEventLoop m_LoopSetValue;  //!< Loop for blocking commands.

    quint16 m_SetOutputValue;   //!< Set analog output value.
    quint16 m_SetDuration;      //!< Set duration
    quint16 m_SetDelay;         //!< Set delay
    quint16 m_ReadOutputValue;  //!< Read analog input value.
};

#endif // WRAPPERFMANALOGOUTPUT_H

// vi: set ts=4 sw=4 et:
