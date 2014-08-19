/****************************************************************************/
/*! \file WrapperFmDigitalInput.cpp
 *
 *  \brief  Implementation file for class WrapperFmDigitalInput
 *
 *  \version  0.1
 *  \date     2012-04-03
 *  \author   Suhasini S
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

#include"ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmDigitalInput.h"

const qint32 UNDEFINED = -1; //!< undefined digital in value


/****************************************************************************/
/*!
 *  \brief Constructor of class WrapperFmDigitalInput
 *
 *  \iparam   Name          The Name of Digital Input funtion module used for
 *                          QtScript identifier.
 *
 *  \iparam   pDigitalInput Pointer to CDigitalInput
 *  \iparam   pParent       Optional pointer to parent for QObject Hierarchy.
 *
 */
/****************************************************************************/
WrapperFmDigitalInput::WrapperFmDigitalInput(QString Name, CDigitalInput *pDigitalInput, QObject *pParent):
    WrapperBase(Name, pParent), m_pDigitalInput(pDigitalInput)
{
    Reset();

    connect(m_pDigitalInput, SIGNAL(ReportActInputValue(quint32, ReturnCode_t, quint16)),
            this, SLOT(OnGetValue(quint32, ReturnCode_t, quint16)));
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Read digital input value
 *
 *  This method performs reading input value of a Digital Input function module.
 *
 *  Examples:
 *  \dontinclude DigitalInput.js
 *  \skipline [DigitalIn.GetValue]
 *  \until    [DigitalIn.GetValue]
 *
 *  \todo needs to work on return value in case of error, string or int?
 *  \return unsigned 16bit digital input value or -1 in case of any
 *          error using a signed 32bit var
 *
 */
/****************************************************************************/
qint32 WrapperFmDigitalInput::GetValue()
{
    //Log(tr("GetValue"));
    bool ok = HandleErrorCode(m_pDigitalInput->ReqActInputValue());
    if (!ok) {
        return UNDEFINED;
    }
    qint32 ret = m_LoopGetValue.exec();
    if (ret != 1) {
        return UNDEFINED;
    }
    return m_CurrentInputValue;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Returns edge count
 *
 *  This method performs reset and return of edge count value.
 *
 *  Examples:
 *  \dontinclude DigitalInput.js
 *  \skipline [DigitalIn.ReadEdgeCount]
 *  \until    [DigitalIn.ReadEdgeCount]
 *
 *  \return Edge count value
 *
 */
/****************************************************************************/
qint32 WrapperFmDigitalInput::ReadEdgeCount()
{
    Log(tr("ReadEdgeCount"));
    // for testing:
    // Log(tr("ReadEdgeCount: %1").arg(m_EdgeCount));
    qint32 temp = m_EdgeCount;
    m_EdgeCount = 0;
    return temp;
}

/****************************************************************************/
/*!
 *  \brief   slot for read digital input value of a function module
 *
 *  This slot is connected to the signal, ReportActInputValue
 *
 *  \iparam ReturnCode ReturnCode of DeviceControl Layer
 *  \iparam InputValue Actual input value
 *
 */
/**************************************a**************************************/
void WrapperFmDigitalInput::OnGetValue(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, quint16 InputValue)
{
    emit ValueChanged(InputValue);

    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    qint32 LastInputValue = m_CurrentInputValue;
    m_CurrentInputValue = InputValue;
    if (m_LoopGetValue.isRunning()) {
        m_LoopGetValue.exit(ret);
    } else {
        if (m_CurrentInputValue != LastInputValue) {
            m_EdgeCount++;
            // for testing:
            // Log(tr("EdgeCount: %1").arg(m_EdgeCount));
        }
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
void WrapperFmDigitalInput::Reset()
{
    m_CurrentInputValue  = UNDEFINED;
    m_EdgeCount = 0;
}
