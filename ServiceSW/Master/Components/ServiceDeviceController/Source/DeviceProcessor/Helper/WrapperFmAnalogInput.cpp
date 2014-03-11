/****************************************************************************/
/*! \file WrapperFmAnalogInput.cpp
 *
 *  \brief  Implementation file for class WrapperFmAnalogInput
 *
 *  \version  0.1
 *  \date     2012-04-03
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

#include "ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmAnalogInput.h"

/****************************************************************************/
/*!
 *  \brief Constructor of class WrapperFmAnalogInput
 *
 *  \iparam   Name         The Name of Analog Input function module used for
 *                         QtScript identifier.
 *  \iparam   pAnalogInput Pointer to CAnalogInput
 *  \iparam   pParent      Optional pointer to parent for QObject Hierarchy.
 *
 */
/****************************************************************************/
WrapperFmAnalogInput::WrapperFmAnalogInput(QString Name, CAnalogInput *pAnalogInput, QObject *pParent):
    WrapperBase(Name, pParent), m_pAnalogInput(pAnalogInput)
{
    Reset();

    connect(m_pAnalogInput, SIGNAL(ReportSetState(quint32, ReturnCode_t)),
            this, SLOT(OnSetState(quint32, ReturnCode_t)));

    connect(m_pAnalogInput, SIGNAL(ReportActInputValue(quint32, ReturnCode_t, quint16)),
            this, SLOT(OnGetValue(quint32, ReturnCode_t, quint16)));
}

/****************************************************************************/
/*!
 *  \brief Script-API: Enables and disables the module
 *
 *  This method enables and disables the analog input module.
 *
 *  Examples:
 *  \dontinclude analoginput.js
 *  \skipline [AnalogIn.SetState]
 *  \until    [AnalogIn.SetState]
 *
 *  \iparam State = Enable/disable flag
 *
 *  \return true, if the setting value is success else false
 */
/****************************************************************************/
bool WrapperFmAnalogInput::SetState(bool Enable)
{
    Log(tr("SetState"));
    m_State = Enable;

    bool ok = HandleErrorCode(m_pAnalogInput->SetState(Enable));
    if (!ok) {
        return false;
    }
    qint32 ret = m_LoopSetState.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief   slot for setting the module state
 *
 *  This slot is connected to the signal, ReportSetState.
 *
 *  \iparam ReturnCode  ReturnCode of DeviceControl Layer
 */
/****************************************************************************/
void WrapperFmAnalogInput::OnSetState(quint32 /*InstanceID*/, ReturnCode_t ReturnCode)
{
    qint32 ret = 1;
    if (HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    if (m_LoopSetState.isRunning()) {
        m_LoopSetState.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API: Reads the Analog Input value
 *
 *  This method performs reading input value of a Analog Input function module.
 *
 *  Examples:
 *  \dontinclude analoginput.js
 *  \skipline [AnalogIn.GetValue]
 *  \until    [AnalogIn.GetValue]
 *
 *  \todo needs to work on return value in case of error
 *  \return the input value read or 0 in case of error
 *
 */
/****************************************************************************/
qint16 WrapperFmAnalogInput::GetValue()
{
   // Log(tr("GetValue"));
    m_CurrentInputValue = 0; //setting to 0 incase of error
    bool ok = HandleErrorCode(m_pAnalogInput->ReqActInputValue());
    if (!ok) {
        return 0;
    }
    qint32 ret = m_LoopGetValue.exec();
    if (ret != 1) {
        Log(tr("error"));
    }
    return m_CurrentInputValue;
}

/****************************************************************************/
/*!
 *  \brief   slot for gettig the analog input value
 *
 *  This slot is connected to the signal, ReportActInputValue
 *
 *  \iparam ReturnCode ReturnCode of DeviceControl Layer
 *  \iparam InputValue Read analog input value
 *
 */
/****************************************************************************/
void WrapperFmAnalogInput::OnGetValue(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, quint16 InputValue)
{
    emit ValueChanged(InputValue);

    qint32 ret = 1;
    m_CurrentInputValue = InputValue;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    if (m_LoopGetValue.isRunning()) {
        m_LoopGetValue.exit(ret);
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
void WrapperFmAnalogInput::Reset()
{
    m_State = 0;
    m_CurrentInputValue = 0;
}
