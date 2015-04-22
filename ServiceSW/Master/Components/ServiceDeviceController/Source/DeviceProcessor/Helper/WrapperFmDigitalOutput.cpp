/****************************************************************************/
/*! \file WrapperFmDigitalOutput.cpp
 *
 *  \brief  Implementation file for class WrapperFmDigitalOutput
 *
 *  \version  0.1
 *  \date     2012-04-04
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
#include "ServiceDeviceController/Include/DeviceProcessor/Helper/ResetData.h"
#include "ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmDigitalOutput.h"

/****************************************************************************/
/*!
 *  \brief Constructor of class WrapperFmDigitalOutput
 *
 *  \iparam   Name             The Name of the Device used for QtScript identifier.
 *  \iparam   pDigitalOutput   Pointer to CDigitalOutput
 *  \iparam   pParent          Optional pointer to parent for QObject Hierarchy.
 *
 */
/****************************************************************************/
WrapperFmDigitalOutput::WrapperFmDigitalOutput(QString Name, CDigitalOutput *pDigitalOutput, QObject *pParent):
    WrapperBase(Name, pParent, pDigitalOutput), m_pDigitalOutput(pDigitalOutput)
{
    Reset();

    mp_ResetData = new CResetData(*pDigitalOutput, this);
    (void)connect(m_pDigitalOutput, SIGNAL(ReportOutputValueAckn(quint32, ReturnCode_t, quint16)),
            this, SLOT(OnSetOutputValue(quint32, ReturnCode_t, quint16)));
    (void)connect(m_pDigitalOutput, SIGNAL(ReportActOutputValue(quint32, ReturnCode_t, quint16)),
            this, SLOT(OnGetOutputValue(quint32, ReturnCode_t, quint16)) );
    (void)connect(m_pDigitalOutput, SIGNAL(ReportLifeTimeData(quint32, ReturnCode_t, quint32, quint32)),
            this, SLOT(OnGetLifetimeData(quint32, ReturnCode_t, quint32, quint32)));
    (void)connect(m_pDigitalOutput, SIGNAL(ReportError(quint32, quint16, quint16, quint16, QDateTime)),
            this, SLOT(OnError(quint32, quint16, QDateTime)));
}

/****************************************************************************/
/*!
 *  \brief Destructor of class WrapperFmDigitalOutput
 */
/****************************************************************************/
WrapperFmDigitalOutput::~WrapperFmDigitalOutput()
{
    try {
        delete mp_ResetData;
    }
    catch(...) {}
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set output value to a digital output function module
 *
 *  This method performs the setting of ouput to high to a Digital Output
 *  function module
 *
 *  Examples:
 *  \skipline    [DigitalOut.SetHigh]
 *  \until       [DigitalOut.SetHigh]
 *
 *  \return true, if the output value set has succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmDigitalOutput::SetHigh()
{
    Log(tr("SetHigh"));
    return SetValue(1);
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set output value to a digital output function module
 *
 *  This method performs the setting of ouput to low to a Digital Output
 *  function module
 *
 *  Examples:
 *  \skipline    [DigitalOut.SetHigh]
 *  \until       [DigitalOut.SetHigh]
 *
 *  \return true, if the output value set has succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmDigitalOutput::SetLow()
{
   Log(tr("SetLow"));
   return SetValue(0);
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Set output value to a digital output function module
 *
 *  This method performs the setting of ouput value to a Digital Output
 *  function module
 *
 *  Examples:
 *  \skipline    [DigitalOut.SetValue]
 *  \until       [DigitalOut.SetValue]
 *
 *  \iparam OutputValue  Unsigned 16bit integer value to be set.
 *  \iparam Duration     Duration in ms of output signal
 *  \iparam Delay        Delay in ms until new output signal is set
 *
 *  \return true, if the output value set has succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmDigitalOutput::SetValue(quint16 OutputValue, quint16 Duration, quint16 Delay)
{
    m_TargetOutputValue = OutputValue;

    bool ok = HandleErrorCode(m_pDigitalOutput->SetOutputValue(m_TargetOutputValue, Duration, Delay));
    if (!ok) {
        return false;
    }

    QTimer timer;
    SetEventLoopTimeOut(timer, m_LoopSetOutputValue);
    qint32 ret = m_LoopSetOutputValue.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief   slot associated with digital output value setting
 *
 *  This slot is connected to the signal, ReportOutputValueAckn
 *
 *  \iparam ReturnCode  ReturnCode of function level Layer
 *  \iparam OutputValue Actual output value
 *
 */
/****************************************************************************/
void WrapperFmDigitalOutput::OnSetOutputValue(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, quint16 OutputValue)
{
    qint32 ret = 1;
    m_CurrentOutputValue = OutputValue;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    if (m_LoopSetOutputValue.isRunning()) {
        if (m_TargetOutputValue != m_CurrentOutputValue) {
            Log(tr("ERROR: Target output value not set."));
            ret = -1;
        }
        m_TargetOutputValue = 0;
        m_LoopSetOutputValue.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Read digital output value
 *
 *  This method performs, reading output of a Digital Output function module
 *
 *  Examples:
 *  \skipline    [DigitalOut.GetValue]
 *  \until       [DigitalOut.GetValue]
 *
 *  \todo needs to work on return value in case of error
 *
 *  \return Digital ouput value or 0 in case of any error
 *
 */
/****************************************************************************/
quint16 WrapperFmDigitalOutput::GetValue()
{
   // Log(tr("GetValue"));
    bool ok = HandleErrorCode(m_pDigitalOutput->ReqOutputValue());
    if (!ok) {
        return 0;
    }

    QTimer timer;
    SetEventLoopTimeOut(timer, m_LoopGetOutputValue);
    qint32 ret = m_LoopGetOutputValue.exec();
    if (ret != 1) {
      return 0;
    }
    return m_CurrentOutputValue;
}

/****************************************************************************/
/*!
 *  \brief   slot associated with digital output reading
 *
 *  This slot is connected to the signal, ReportActOutputValue
 *
 *  \iparam ReturnCode  ReturnCode of function level Layer
 *  \iparam OutputVal   Actual output value
 *
 */
/****************************************************************************/
void WrapperFmDigitalOutput::OnGetOutputValue(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, quint16 OutputVal)
{
    qint32 ret = 1;
    m_CurrentOutputValue = OutputVal;
    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    if (m_LoopGetOutputValue.isRunning()) {
        m_TargetOutputValue = 0 ;
         m_LoopGetOutputValue.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}
/****************************************************************************/
/*!
 *  \brief  Script-API: Read lifetime data
 *
 *  This method gets the time an digital output was switched on in minutes
 *  and the number of times it was switched on or off.
 *
 *  Examples:
 *  \skipline    [DigitalOut.GetLifetime]
 *  \until       [DigitalOut.GetLifetime]
 *
 *  \todo needs to work on return value in case of error
 *
 *  \return Digital ouput value or 0 in case of any error
 *
 */
/****************************************************************************/
bool WrapperFmDigitalOutput::GetLifetimeData()
{
    bool ok = HandleErrorCode(m_pDigitalOutput->ReqLifeTimeData());
    if (!ok) {
        return false;
    }

    QTimer timer;
    SetEventLoopTimeOut(timer, m_LoopGetLifetimeData);
    qint32 ret = m_LoopGetLifetimeData.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief   slot associated with lifetime reading
 *
 *  This slot is connected to the signal, ReportLifeTimeData
 *
 *  \iparam ReturnCode  ReturnCode of function level Layer
 *  \iparam Lifetime    Active time in minutes
 *  \iparam Lifecycles  Number of times the output was switched
 */
/****************************************************************************/
void WrapperFmDigitalOutput::OnGetLifetimeData(quint32 /*InstanceID*/, ReturnCode_t ReturnCode,
                                               quint32 Lifetime, quint32 Lifecycles)
{
    qint32 ret = 1;

    m_Lifetime = Lifetime;
    m_Lifecycles = Lifecycles;

    if (!HandleErrorCode(ReturnCode)) {
        ret = -1;
    }
    if (m_LoopGetLifetimeData.isRunning()) {
        m_LoopGetLifetimeData.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Request a data reset
 *
 *      This method resets the non-volatile storage data of all partitions on
 *      a Slave.
 *
 *  Examples:
 *  \skipline [DigitalOut.ReqDataReset]
 *  \until    [DigitalOut.ReqDataReset]
 *
 *  \return true, if the setting value is success else false
 */
/****************************************************************************/
bool WrapperFmDigitalOutput::ReqDataReset()
{
    return HandleErrorCode(mp_ResetData->ReqDataReset());
}

/****************************************************************************/
/*!
 *  \brief Script-API: Reset internal states
 *
 *  The internal states of the instance is cleared by calling this method.
 *  This method is used to reset the instance if the system is hung.
 *  Data memebers are set to its default values.
 *
 */
/****************************************************************************/
void WrapperFmDigitalOutput::Reset()
{
        m_TargetOutputValue  = 0;
        m_CurrentOutputValue = 0;
}

quint32 WrapperFmDigitalOutput::GetLifetime()
{
    if(GetLifetimeData())
    {
        return m_Lifetime;
    }
    else
    {
        return 0;
    }
}

quint32 WrapperFmDigitalOutput::GetLifecycles()
{
    if(GetLifetimeData())
    {
        return m_Lifecycles;
    }
    else
    {
        return 0;
    }
}
