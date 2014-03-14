/****************************************************************************/
/*! \file WrapperFmAnalogOutput.cpp
 *
 *  \brief  Implementation file for class WrapperFmAnalogOutput
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

#include "ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmAnalogOutput.h"

/****************************************************************************/
/*!
 *  \brief Constructor of class WrapperFmAnalogOutput
 *
 *  \iparam   Name          The Name of Analog Output function module used for
 *                          QtScript identifier.
 *  \iparam   pAnalogOutput Pointer to CAnalogOutput
 *  \iparam   pParent       Optional pointer to parent for QObject Hierarchy.
 *
 */
/****************************************************************************/
WrapperFmAnalogOutput::WrapperFmAnalogOutput(QString Name, CAnalogOutput *pAnalogOutput, QObject *pParent):
    WrapperBase(Name, pParent), m_pAnalogOutput(pAnalogOutput)
{
    Reset();

    connect( m_pAnalogOutput, SIGNAL(ReportOutputValueAckn(quint32, ReturnCode_t, quint16)),
             this, SLOT(OnSetValue(quint32, ReturnCode_t, quint16)));
    connect( m_pAnalogOutput, SIGNAL(ReportActOutputValue(quint32, ReturnCode_t, quint16)),
             this, SLOT(OnGetValue(quint32, ReturnCode_t, quint16)));
}

/****************************************************************************/
/*!
 *  \brief Script-API: Sets the Analog output value.
 *
 *  Analog output value of a Analog Output function module can be set using
 *  this method.
 *
 *  Examples:
 *  \dontinclude analogoutput.js
 *  \skipline [AnalogOut.SetValue]
 *  \until    [AnalogOut.SetValue]
 *
 *  \iparam OutputValue The output value to be set
 *
 *  \return true, if the setting value is success else false
 *
 */
/****************************************************************************/
bool WrapperFmAnalogOutput::SetValue(quint16 OutputValue, quint16 Duration, quint16 Delay)
{
    Log(tr("SetValue"));
    m_SetOutputValue = OutputValue;
    m_SetDuration = Duration;
    m_SetDelay = Delay;

    bool ok = HandleErrorCode(m_pAnalogOutput->SetOutputValue(OutputValue, Duration, Delay));
    if (!ok) {
        return false;
    }
    qint32 ret = m_LoopSetValue.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief   slot for setting the analog output value
 *
 *  This slot is connected to the signal, ReportOutputValueAckn.
 *
 *  \iparam ReturnCode  ReturnCode of DeviceControl Layer
 *  \iparam OutputValue The analog output value to be set
 *
 */
/****************************************************************************/
void WrapperFmAnalogOutput::OnSetValue(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, quint16 OutputValue)
{
    qint32 ret = 1;
    m_ReadOutputValue = OutputValue;
    if (HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    if (m_LoopSetValue.isRunning()) {
        if (m_ReadOutputValue != m_SetOutputValue) {
            Log(tr("ERROR: Analog Output not set."));
            ret = -1;
        }
        m_LoopSetValue.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API: Reads the set analog output value
 *
 *  This method performs the reading of the output value set for the Analog
 *  Output function module.
 *
 *  Examples:
 *  \dontinclude analogoutput.js
 *  \skipline [AnalogOut.GetValue]
 *  \until    [AnalogOut.GetValue]
 *
 *  \todo needs to work on return value in case of error
 *  \return the output value read or 0 incase of error
 *
 */
/****************************************************************************/
quint16 WrapperFmAnalogOutput::GetValue()
{
   // Log(tr("GetValue"));
    bool ok = HandleErrorCode(m_pAnalogOutput->ReqOutputValue());
    if (!ok) {
        return 0;
    }
    m_ReadOutputValue = 0; //setting to 0 incase of error
    qint32 ret = m_LoopGetValue.exec();
    if (ret != 1) {
        Log(tr("error"));
    }
    return m_ReadOutputValue;
}

/****************************************************************************/
/*!
 *  \brief   slot for getting the analog output value
 *
 *  This slot is connected to the signal, ReportActOutputValue
 *
 *  \iparam ReturnCode  ReturnCode of DeviceControl Layer
 *  \iparam OutputValue Read anlaog output value
 *
 */
/****************************************************************************/
void WrapperFmAnalogOutput::OnGetValue(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, quint16 OutputValue)
{
    qint32 ret = 1;
    m_ReadOutputValue = OutputValue;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    if (m_LoopGetValue.isRunning()) {
        m_LoopGetValue.exit(ret);
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
void WrapperFmAnalogOutput::Reset() {
    m_SetOutputValue = 0;
    m_ReadOutputValue = 0;
}

// vi: set ts=4 sw=4 et:
