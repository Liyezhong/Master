/****************************************************************************/
/*! \file WrapperFmTempControl.cpp
 *
 *  \brief  Implementation file for class WrapperFmTempControl
 *
 *  \version  0.1
 *  \date     2012-04-09
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
#include "ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmTempControl.h"

const qint32 UNDEFINED = -1; //!< undefined value for temperature and control status
const qint32 TOLERANCE = 10; //!< tolerance value for calculating inside and outside range

/****************************************************************************/
/*!
 *  \brief Constructor of class WrapperFmTempControl
 *
 *  \iparam   Name           The Name of the Device used for QtScript identifier.
 *  \iparam   pTempControl   Pointer to CTemperatureControl
 *  \iparam   pParent        Optional pointer to parent for QObject Hierarchy.
 *
 */
/****************************************************************************/
WrapperFmTempControl::WrapperFmTempControl(QString Name, CTemperatureControl *pTempControl, QObject *pParent):
    WrapperBase(Name, pParent), m_pTempControl(pTempControl)
{
    Reset();
    (void)connect(m_pTempControl, SIGNAL(ReportActTemperature(quint32, ReturnCode_t, quint8, qreal)),
            this, SLOT(OnGetTemp(quint32, ReturnCode_t, quint8, qreal)));
    (void)connect(m_pTempControl, SIGNAL(ReportRefTemperature(quint32, ReturnCode_t, qreal)),
            this, SLOT(OnSetTemp(quint32, ReturnCode_t, qreal)));
    (void)connect(m_pTempControl, SIGNAL(ReportActStatus(quint32, ReturnCode_t, TempCtrlStatus_t, TempCtrlMainsVoltage_t)),
            this, SLOT(OnTempControlStatus(quint32, ReturnCode_t, TempCtrlStatus_t, TempCtrlMainsVoltage_t)));
    (void)connect(m_pTempControl, SIGNAL(ReportActOperatingMode(quint32, ReturnCode_t, TempCtrlOperatingMode_t)),
            this, SLOT(OnSetTempCtrlOpMode(quint32, ReturnCode_t, TempCtrlOperatingMode_t)));
    (void)connect(m_pTempControl, SIGNAL(ReportResetHeaterOperatingTime(quint32, ReturnCode_t, quint8)),
            this, SLOT(OnResetHeaterOperatingTime(quint32, ReturnCode_t, quint8)));
    (void)connect(m_pTempControl, SIGNAL(ReportHeaterOperatingTime(quint32, ReturnCode_t, quint8, quint32)),
            this, SLOT(OnGetHeaterOperatingTime(quint32, ReturnCode_t, quint8, quint32)));
    (void)connect(m_pTempControl, SIGNAL(ReportFanSpeed(quint32, ReturnCode_t, quint8, quint16)),
            this, SLOT(OnGetFanSpeed(quint32, ReturnCode_t, quint8, quint16)));
    //connect(m_pTempControl, SIGNAL(ReportHardwareStatus(quint32, ReturnCode_t, quint8, quint8, quint8, quint8, quint16)),
    //        this, SLOT(OnGetHardwareStatus(quint32, ReturnCode_t, quint8, quint8, quint8, quint8, quint16)));
    (void)connect(m_pTempControl, SIGNAL(ReportHardwareStatus(quint32, ReturnCode_t, quint8, quint8, quint8, quint8, quint16, quint8)),
            this, SLOT(OnGetHardwareStatus(quint32, ReturnCode_t, quint8, quint8, quint8, quint8, quint16, quint8)));
    (void)connect(m_pTempControl, SIGNAL(ReportTemperatureRange(quint32, ReturnCode_t, bool, qreal)),
            this, SLOT(OnTemperatureRange(quint32, ReturnCode_t, bool, qreal)));

#ifdef PRE_ALFA_TEST
#if 0

    (void)connect(m_pTempControl, SIGNAL(ReportLevelSensorState(quint32, ReturnCode_t, quint8)),
                    this, SLOT(OnLevelSensorState(quint32, ReturnCode_t, quint8)));
#endif
    (void)connect(m_pTempControl, SIGNAL(ReportSetPidAckn(quint32, ReturnCode_t, quint16, quint16, quint16, quint16)),
            this, SLOT(OnSetTempPid(quint32, ReturnCode_t, quint16, quint16, quint16, quint16)));
    (void)connect(m_pTempControl, SIGNAL(ReportError(quint32, quint16, quint16, quint16, QDateTime)),
            this, SLOT(OnError(quint32,quint16,quint16,quint16,QDateTime)));
    (void)connect(m_pTempControl, SIGNAL(ReportSetSwitchState(quint32, ReturnCode_t, qint8, qint8)),
            this, SLOT(OnSetSwitchState(quint32,ReturnCode_t,qint8,qint8)));



    memset( &m_LastGetTempTime, 0 , sizeof(m_LastGetTempTime));
#endif
}

/****************************************************************************/
/*!
 *  \brief Script-API:Set reference temperature
 *
 *  Examples:
 *  \dontinclude tempcontrol.js
 *  \skipline [TempControl.StartTemperatureControl]
 *  \until    [TempControl.StartTemperatureControl]
 *
 *  \iparam  NominalTemperature   Reference temperature
 *
 *  \return true, if the set function succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmTempControl::StartTemperatureControl(qreal NominalTemperature, quint8 SlopeTempChange)
{
#if 0
#ifndef PRE_ALFA_TEST
    Log(tr("StartTemperatureControl"));
#endif
    m_TargetTemperature = NominalTemperature;
    m_TargetTempCtrlStatus = TEMPCTRL_STATUS_ON;
    if (GetTemperatureControlState() == "Error") {
        Log(tr("Not able to read the temperature control status"));
        return false;
    }
    if (IsTemperatureControlOn()) {
        return false;
    }
    if (IsTemperatureControlOff()) {
            //Set the nominal temperature
            if (!SetTemperature(NominalTemperature, SlopeTempChange)) {
            Log(tr("Not able to set temperature"));
            return false;
            }
            //ON the temperature control
            if (!SetTemperatureControlStatus(TEMPCTRL_STATUS_ON)) {
            Log(tr("Not able to start temperature control"));
            return false;
            }
    }
    return true;
#else //refer to Brandon's request to combine "set temp" with "enable temp control"
    m_TargetTemperature = NominalTemperature;
    m_TargetTempCtrlStatus = TEMPCTRL_STATUS_ON;
    if (GetTemperatureControlState() == "Error")
    {
        Log(tr("Not able to read the temperature control status"));
        return false;
    }
    //Set the nominal temperature
    if (!SetTemperature(NominalTemperature, SlopeTempChange)) {
        Log(tr("Not able to set temperature"));
        return false;
    }
    return true;
#endif
}

/****************************************************************************/
/*!
 *  \brief  slot for set temperature
 *
 *  This slot is connected to the signal ReportRefTemperature
 *
 *  \iparam   ReturnCode    ReturnCode of function level Layer
 *  \iparam   Temperature   Actual temperature
 *
 */
/****************************************************************************/
void WrapperFmTempControl::OnSetTemp(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, qreal Temperature)
{
    qint32 ret = 1;
    m_CurrentTemperature[0] = Temperature;
    if (!HandleErrorCode(ReturnCode)) {
        ret = UNDEFINED;
    }
    if (m_LoopSetTemperature.isRunning()) {
        if (m_CurrentTemperature[0] != m_TargetTemperature) {
            Log(tr("ERROR: Target temp is not reached."));
            ret = UNDEFINED;
        }
        m_TargetTemperature = UNDEFINED;
        m_LoopSetTemperature.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API:Get actual temperature
 *
 * This method performs reading of actual temperature
 *
 *  Examples:
 *  \dontinclude tempcontrol.js
 *  \skipline [TempControl.GetTemperature]
 *  \until    [TempControl.GetTemperature]
 *
 *  \return actualtemperature, else UNDEFINED in caseof error
 *
 */
/****************************************************************************/
qreal WrapperFmTempControl::GetTemperature(quint8 Index)
{
#ifdef PRE_ALFA_TEST
    //static qint64 m_LastGetTempTime[16][8] = {0};
    //quint8 FMIndex = (Index & 0xF0) >> 4;
    //quint8 SensorIndex = Index & 0x0F;
    quint8 SensorIndex = Index;
    qint64 Now = QDateTime::currentMSecsSinceEpoch();
    qreal RetValue = m_CurrentTemperature[Index];
    if((Now - m_LastGetTempTime[SensorIndex]) >= 200) // check if 500 msec has passed since last read
    {
        bool ok = HandleErrorCode(m_pTempControl->ReqActTemperature(SensorIndex));
        if (!ok)
        {
            RetValue = UNDEFINED;
        }
        else
        {
            qint32 ret = m_LoopGetTemperature.exec();
            if (ret != 1)
            {
                RetValue = UNDEFINED;
            }
            else
            {
                RetValue = m_CurrentTemperature[Index];
            }
            m_LastGetTempTime[SensorIndex] = Now;
        }
    }
    return RetValue;
#else
    bool ok = HandleErrorCode(m_pTempControl->ReqActTemperature(Index));
    if (!ok) {
        return UNDEFINED;
    }
    qint32 ret = m_LoopGetTemperature.exec();
    if (ret != 1) {
      return UNDEFINED;
    }

    return m_CurrentTemperature;
#endif
}

/****************************************************************************/
/*!
 *  \brief Helper function, to set the temperature of Oven
 *
 *  \iparam NominalTemperature Temperature to be set
 *
 *  \return true, if the temperature set succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmTempControl::SetTemperature(qreal NominalTemperature, quint8 SlopeTempChange)
{
    m_TargetTemperature = NominalTemperature;
    bool ok = HandleErrorCode(m_pTempControl->SetTemperature(NominalTemperature, SlopeTempChange));
    if (!ok) {
        return false;
    }
    qint32 ret = m_LoopSetTemperature.exec();
    return (ret == 1);
}


/****************************************************************************/
/*!
 *  \brief  slot for get temperature
 *
 *  This slot is connected to the signal ReportActTemperature
 *
 *  \iparam   ReturnCode    ReturnCode of function level Layer
 *  \iparam   Temp          Actual temperature
 *
 */
/****************************************************************************/
void WrapperFmTempControl::OnGetTemp(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, quint8 Index, qreal Temp)
{
    Q_UNUSED(Index)

    qint32 ret = 1;
    m_CurrentTemperature[Index] = Temp;
    if (!HandleErrorCode(ReturnCode)) {
        ret = UNDEFINED;
    }
    if (m_LoopGetTemperature.isRunning()) {
             m_LoopGetTemperature.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Helper function, to set temperature control status
 *
 *  \return true, if set temperature control status ON has succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmTempControl::SetTempCtrlON()
{
    Log(tr("SetTempCtrlON"));
    return SetTemperatureControlStatus(TEMPCTRL_STATUS_ON);
}

/****************************************************************************/
/*!
 *  \brief Script-API:Set temperature control status
 *
 *   This method sets the temperature control status OFF
 *
 *  Examples:
 *  \dontinclude tempcontrol.js
 *  \skipline [TempControl.StopTemperatureControl]
 *  \until    [TempControl.StopTemperatureControl]
 *
 *  \return true, if set temperature control status off has succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmTempControl::StopTemperatureControl()
{
    Log(tr("StopTemperatureControl"));
    return SetTemperatureControlStatus(TEMPCTRL_STATUS_OFF);
}

/****************************************************************************/
/*!
 *  \brief Helper function, to set the temperature control status
 *
 *  \iparam TempCtrlStatus   Status of the temperature control
 *
 *  \return true, if the temperature control status setting has succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmTempControl::SetTemperatureControlStatus(TempCtrlStatus_t TempCtrlStatus)
{
    m_TargetTempCtrlStatus = TempCtrlStatus;
    bool ok = HandleErrorCode(m_pTempControl->SetStatus(TempCtrlStatus));
    if (!ok) {
        return false;
    }
    //qint32 ret = m_LoopSetTemperatureControl.exec();
    //return (ret == 1);
    return true;
}

/****************************************************************************/
/*!
 *  \brief  slot for temperature control status
 *
 *  This slot is connected to the signal ReportTempCtrlStatus
 *
 *  \iparam   ReturnCode      ReturnCode of function level Layer
 *  \iparam   TempCtrlStatus  Status of the temperature control
 *  \iparam   MainsVoltage    Mains voltage state of the heaters
 *
 */
/****************************************************************************/
void WrapperFmTempControl::OnTempControlStatus(quint32 /*InstanceID*/, ReturnCode_t ReturnCode,
                                               TempCtrlStatus_t TempCtrlStatus, TempCtrlMainsVoltage_t MainsVoltage)
{
    qint32 ret = 1;
    m_CurrentTempCtrlStatus = TempCtrlStatus;
    m_MainsVoltageStatus = MainsVoltage;
    if (!HandleErrorCode(ReturnCode)) {
        ret = UNDEFINED;
    }
    // GetPosition can be called during asynchronous SetPosition, so
    // process GetPosition first.
    if (m_LoopGetTemperatureControl.isRunning()) {
        m_LoopGetTemperatureControl.exit(ret);
    }
    else if (m_LoopGetVoltageState.isRunning()) {
        m_LoopGetVoltageState.exit(ret);

    }
    else if (m_LoopSetTemperatureControl.isRunning()){
        if(m_TargetTempCtrlStatus != m_CurrentTempCtrlStatus) {
           Log(tr("ERROR: Target temp control status is not reached."));
           ret = UNDEFINED;
        } else {
           Log(tr("Done."));
        }
        m_TargetTempCtrlStatus = TEMPCTRL_STATUS_UNDEF;
        m_LoopSetTemperatureControl.exit(ret);
    }
    else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API:Set temperature control operation mode
 *
 *  This method performs setting of temperature control operation mode as full
 *  power mode.
 *
 *  Examples:
 *  \dontinclude tempcontrol.js
 *  \skipline [TempControl.SetFullPowerMode]
 *  \until    [TempControl.SetFullPowerMode]
 *
 *  \return true, if the mode setting has succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmTempControl::SetFullPowerMode()
{
    Log(tr("SetFullPowerMode"));
    return SetOperatingMode(TEMPCTRL_OPMODE_FULL);
}

/****************************************************************************/
/*!
 *  \brief Script-API:Set temperature control operation mode
 *
 *  This method performs setting of temperature control operation mode as full
 *  power save mode.
 *
 *  Examples:
 *  \dontinclude tempcontrol.js
 *  \skipline [TempControl.SetPowerSaveMode]
 *  \until    [TempControl.SetPowerSaveMode]
 *
 *  \return true, if the mode setting has succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmTempControl::SetPowerSaveMode()
{
    Log(tr("SetPowerSaveMode"));
    return SetOperatingMode(TEMPCTRL_OPMODE_HOLD);
}

/****************************************************************************/
/*!
 *  \brief Helper function to set the operation mode
 *
 *  \iparam TempCtrlOpMode   Operation mode
 *
 *  \return true, if the mode setting has succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmTempControl::SetOperatingMode(TempCtrlOperatingMode_t TempCtrlOpMode)
{
    m_TargetOperatingMode = TempCtrlOpMode;
    bool ok = HandleErrorCode(m_pTempControl->SetOperatingMode(TempCtrlOpMode));
    if (!ok) {
        return false;
    }
    qint32 ret = m_LoopSetOperatingMode.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief  slot for set temperature control operation mode
 *
 *  This slot is connected to the signal ReportTempCtrlOpMode
 *
 *  \iparam   ReturnCode      ReturnCode of function level Layer
 *  \iparam   TempCtrlOpMode  Operation mode
 *
 */
/****************************************************************************/
void WrapperFmTempControl::OnSetTempCtrlOpMode(quint32, ReturnCode_t ReturnCode, TempCtrlOperatingMode_t TempCtrlOpMode)
{
    qint32 ret = 1;
    m_CurrentOperatingMode = TempCtrlOpMode;
    if (!HandleErrorCode(ReturnCode)) {
        ret = UNDEFINED;
    }
    if (m_LoopSetOperatingMode.isRunning()) {
        if (m_TargetOperatingMode != m_CurrentOperatingMode) {
            Log(tr("ERROR: Target temp ctrl operating mode not set."));
            ret = UNDEFINED;
        }
        m_TargetOperatingMode = TEMPCTRL_OPMODE_UNDEF;
        m_LoopSetOperatingMode.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API:Reset heater operating time
 *
 *  This method performs resetting of the operating time of a single heater
 *  back to null.
 *
 *  Examples:
 *  \dontinclude tempcontrol.js
 *  \skipline [TempControl.ResetHeaterOperatingTime]
 *  \until    [TempControl.ResetHeaterOperatingTime]
 *
 *  \iparam Index = Index number of the heating element
 *
 *  \return true, if the mode setting has succeeded, else false
 *
 */
/****************************************************************************/
bool WrapperFmTempControl::ResetHeaterOperatingTime(quint8 Index)
{
    bool ok = HandleErrorCode(m_pTempControl->ResetHeaterOperatingTime(Index));
    if (!ok) {
        return false;
    }
    qint32 ret = m_LoopResetHeaterOperatingTime.exec();
    return (ret == 1);
}

/****************************************************************************/
/*!
 *  \brief  slot for reseting the heater operating time
 *
 *  This slot is connected to the signal ReportResetHeaterOperatingTime
 *
 *  \iparam   ReturnCode    ReturnCode of function level Layer
 *  \iparam   Index         Index number of the heating element
 *
 */
/****************************************************************************/
void WrapperFmTempControl::OnResetHeaterOperatingTime(quint32, ReturnCode_t ReturnCode, quint8 Index)
{
    Q_UNUSED(Index)
    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = UNDEFINED;
    }
    if (m_LoopResetHeaterOperatingTime.isRunning()) {
        m_LoopResetHeaterOperatingTime.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API:Get heater operating time
 *
 *  This method performs getting of the operating time of a single heater in
 *  seconds.
 *
 *  Examples:
 *  \dontinclude tempcontrol.js
 *  \skipline [TempControl.GetHeaterOperatingTime]
 *  \until    [TempControl.GetHeaterOperatingTime]
 *
 *  \iparam Index = Index number of the heating element
 *
 *  \return Heater operating time in seconds
 */
/****************************************************************************/
quint32 WrapperFmTempControl::GetHeaterOperatingTime(quint8 Index)
{
    bool ok = HandleErrorCode(m_pTempControl->GetHeaterOperatingTime(Index));
    if (!ok) {
        return UNDEFINED;
    }
    qint32 ret = m_LoopGetHeaterOperatingTime.exec();
    if (ret != 1) {
        return UNDEFINED;
    }
    return m_HeaterOperatingTime;
}

/****************************************************************************/
/*!
 *  \brief  slot for getting the heater operating time
 *
 *  This slot is connected to the signal ReportHeaterOperatingTime
 *
 *  \iparam   ReturnCode    ReturnCode of function level Layer
 *  \iparam   Index         Index number of the heating element
 *  \iparam   OperatingTime Heater operating time in seconds
 */
/****************************************************************************/
void WrapperFmTempControl::OnGetHeaterOperatingTime(quint32, ReturnCode_t ReturnCode, quint8 Index, quint32 OperatingTime)
{
    Q_UNUSED(Index)
    m_HeaterOperatingTime = OperatingTime;
    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = UNDEFINED;
    }
    if (m_LoopGetHeaterOperatingTime.isRunning()) {
        m_LoopGetHeaterOperatingTime.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API:Get fan speed
 *
 *  This method performs getting of the speed of a single ventilation fan in
 *  revelations per second.
 *
 *  Examples:
 *  \dontinclude tempcontrol.js
 *  \skipline [TempControl.GetFanSpeed]
 *  \until    [TempControl.GetFanSpeed]
 *
 *  \iparam Index = Index number of the fan
 *
 *  \return Ventilation fan speed in revelations per second
 */
/****************************************************************************/
quint16 WrapperFmTempControl::GetFanSpeed(quint8 Index)
{
    bool ok = HandleErrorCode(m_pTempControl->GetFanSpeed(Index));
    if (!ok) {
        return UNDEFINED;
    }
    qint32 ret = m_LoopGetFanSpeed.exec();
    if (ret != 1) {
        return UNDEFINED;
    }
    return m_FanSpeed;
}

/****************************************************************************/
/*!
 *  \brief  slot for getting the ventilation fan speed
 *
 *  This slot is connected to the signal ReportFanSpeed
 *
 *  \iparam   ReturnCode    ReturnCode of function level Layer
 *  \iparam   Index         Index number of the heating element
 *  \iparam   FanSpeed      Fan speed in revelations per minute
 */
/****************************************************************************/
void WrapperFmTempControl::OnGetFanSpeed(quint32, ReturnCode_t ReturnCode, quint8 Index, quint16 FanSpeed)
{
    Q_UNUSED(Index)
    m_FanSpeed = FanSpeed;
    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = UNDEFINED;
    }
    if (m_LoopGetFanSpeed.isRunning()) {
        m_LoopGetFanSpeed.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

/****************************************************************************/
/*!
 *  \brief Script-API:Get hardware information
 *
 *  This method performs getting of the Slave's hardware information. This
 *  includes: number of temperature sensors, number of ventilation fans,
 *  number of heaters, number of PID controllers in the control loop, current
 *  through the heating circuit in milliamperes.
 *
 *  Examples:
 *  \dontinclude tempcontrol.js
 *  \skipline [TempControl.GetHardwareStatus]
 *  \until    [TempControl.GetHardwareStatus]
 *
 *  \return Hardware information
 */
/****************************************************************************/
WrapperFmTempControl::HardwareStatus_t *WrapperFmTempControl::GetHardwareStatus()
{
    bool ok = HandleErrorCode(m_pTempControl->GetHardwareStatus());
    if (!ok) {
        return NULL;
    }
    qint32 ret = m_LoopGetHardwareStatus.exec();
    if (ret != 1) {
        return NULL;
    }
    return &m_HardwareStatus;
}


/****************************************************************************/
/*!
 *  \brief Script-API:Get current information
 *
 *  This method performs getting of the Slave's hardware information and
 *  returns only current through the heating circuit in milliamperes.
 *
 *  Examples:
 *  \dontinclude tempcontrol.js
 *  \skipline [TempControl.GetCurrent]
 *  \until    [TempControl.GetCurrent]
 *
 *  \return Hardware information
 */
/****************************************************************************/
quint16 WrapperFmTempControl::GetCurrent()
{
    if ( GetHardwareStatus() != NULL ) {
        return m_HardwareStatus.Current;
    }
    else {
        return 0;
    }
}

quint8 WrapperFmTempControl::GetHeaterSwitchType()
{
    if ( GetHardwareStatus() != NULL ) {
        return m_HardwareStatus.HeaterSwitchType;
    }
    else {
        return 0;
    }
}

/****************************************************************************/
/*!
 *  \brief  slot for getting the hardware information
 *
 *  This slot is connected to the signal ReportHardwareStatus
 *
 *  \iparam ReturnCode = ReturnCode of function level Layer
 *  \iparam Sensors = Number of temperature sensors connected to the board
 *  \iparam Fans = Number of ventilation fans connected to the board
 *  \iparam Heaters = Number of heating elements connected to the board
 *  \iparam Pids = Number of PID controllers in the control loop
 *  \iparam Current = Current through the heatinf circuit in milliamperes
 */
/****************************************************************************/
void WrapperFmTempControl::OnGetHardwareStatus(quint32, ReturnCode_t ReturnCode, quint8 Sensors, quint8 Fans,
                                               quint8 Heaters, quint8 Pids, quint16 Current, quint8 HeaterSwitchType)
{
    m_HardwareStatus.Sensors = Sensors;
    m_HardwareStatus.Fans = Fans;
    m_HardwareStatus.Heaters = Heaters;
    m_HardwareStatus.Pids = Pids;
    m_HardwareStatus.Current = Current;
    m_HardwareStatus.HeaterSwitchType = HeaterSwitchType;
    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = UNDEFINED;
    }
    if (m_LoopGetHardwareStatus.isRunning()) {
        m_LoopGetHardwareStatus.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

#ifdef PRE_ALFA_TEST
#if 0
void WrapperFmTempControl::OnLevelSensorState(quint32, ReturnCode_t ReturnCode, quint8 State)
{
    Log(tr("New Level Sensor State is: %1").arg(State));
}
#endif


void WrapperFmTempControl::OnSetTempPid(quint32, ReturnCode_t ReturnCode, quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime)
{
    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = UNDEFINED;
    }
    if (m_LoopSetTemperaturePid.isRunning()) {
        m_LoopSetTemperaturePid.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}

void WrapperFmTempControl::OnError(quint32 InstanceID, quint16 ErrorGroup, quint16 ErrorCode, quint16 ErrorData, QDateTime ErrorTime)
{
    Log(tr("Temp control get error, Instance ID: %1, Error Group: %2, Error Code: %3, Error Data: %4").arg(GetNameByInstanceID(InstanceID)).arg(ErrorGroup).arg(ErrorCode).arg(ErrorData));
}
#endif

void WrapperFmTempControl::OnTemperatureRange(quint32 InstanceID, ReturnCode_t ReturnCode, bool InRange, qreal Temperature)
{
    if(InRange)
    {
        Log(tr("Temp control temp in range, Instance ID: %1, temperature is: %2").arg(GetNameByInstanceID(InstanceID)).arg(Temperature));
    }
    else
    {
        Log(tr("Temp control temp out of range, Instance ID: %1, temperature is: %2").arg(GetNameByInstanceID(InstanceID)).arg(Temperature));
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
void WrapperFmTempControl::Reset()
{
   m_TargetTemperature = UNDEFINED;
   for (int i=0; i< MAX_TEMP_SENSOR_NUM; i++)
   {
       m_CurrentTemperature[i] = UNDEFINED;
   }
   m_TargetTempCtrlStatus = TEMPCTRL_STATUS_UNDEF;
   m_CurrentTempCtrlStatus = TEMPCTRL_STATUS_UNDEF;
   m_TargetOperatingMode = TEMPCTRL_OPMODE_UNDEF;
   m_CurrentOperatingMode  = TEMPCTRL_OPMODE_UNDEF;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Checks the temperature control mode
 *
 *  Examples:
 *  \dontinclude tempcontrol.js
 *  \skipline [TempControl.GetPowerOperationMode]
 *  \until    [TempControl.GetPowerOperationMode]
 *
 *  \return string representation of the operation mode:
 *   full power mode, full safe mode, undefined
 *
 */
/****************************************************************************/
QString WrapperFmTempControl::GetPowerOperationMode()
{
    Log(tr("GetPowerOperationMode"));
    QString OperationMode = "undefined";
    if(m_CurrentOperatingMode == TEMPCTRL_OPMODE_FULL) {
        OperationMode = "full power mode";
    } else if(m_CurrentOperatingMode == TEMPCTRL_OPMODE_HOLD) {
        OperationMode = "powersave mode";
    }
    return OperationMode;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Check if the temperature control is ON or not
 *
 *  This method checks if the temperature is ON
 *
 *  Examples:
 *  \dontinclude tempcontrol.js
 *  \skipline [TempControl.IsTemperatureControllOn]
 *  \until    [TempControl.IsTemperatureControllOn]
 *
 *  \return true, if the temperature control state is InsideRange or
 *  OutsideRange else false
 *
 */
/****************************************************************************/
bool WrapperFmTempControl::IsTemperatureControlOn()
{
   return (m_CurrentTempCtrlStatus == TEMPCTRL_STATUS_ON);
}

/****************************************************************************/
/*!
 *  \brief  Helper function to check if the temperature control is Off
 *
 *  This method checks if the wrapper oven temperature control state is Off.
 *
 *  \return true, if the temperature control is Off, else false
 *
 */
/****************************************************************************/
bool WrapperFmTempControl::IsTemperatureControlOff()
{
    return (m_CurrentTempCtrlStatus == TEMPCTRL_STATUS_OFF);
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Check the temperature control state
 *
 *  This method checks if the temperature control state is
 *  Off/IsideRange/OutSideRange
 *
 *  Examples:
 *  \dontinclude tempcontrol.js
 *  \skipline [TempControl.GetTemperatureControlState]
 *  \until    [TempControl.GetTemperatureControlState]
 *
 *  \return string representation of temperature control state:
 *  Off, InsideRange, OutSideRange
 *
 */
/****************************************************************************/
QString WrapperFmTempControl::GetTemperatureControlState()
{
#ifndef PRE_ALFA_TEST
    Log(tr("GetTemperatureControlState"));
#endif
    bool ok = HandleErrorCode(m_pTempControl->ReqStatus());
    if (!ok) {
        m_CurrentTempCtrlStatus = TEMPCTRL_STATUS_UNDEF;
        return "Error";
    }
    qint32 ret = m_LoopGetTemperatureControl.exec();
    QString controlstate = "";
    if (ret != 1) {
        controlstate = "Error";
    } else if (IsTemperatureControlOn()) {
        if (IsInsideRange()) {
            controlstate = "InsideRange";
        } else if (IsOutsideRange()){
            controlstate = "OutsideRange";
        }
    } else if (IsTemperatureControlOff()){
        controlstate = "Off";
    }
    return controlstate;
}

/****************************************************************************/
/*!
 *  \brief  Helper function to check if the control state is in InsideRange
 *
 *  This method checks if the temperature control state is inside range.
 *
 *  \return true, if the temperature control is InsideRange, else false
 *
 */
/****************************************************************************/
bool WrapperFmTempControl::IsInsideRange()
{
    if(GetTemperature(0) != UNDEFINED) {
        if((m_TargetTemperature != UNDEFINED) || (m_CurrentTemperature[0] != UNDEFINED)) {
            if ((m_CurrentTemperature[0] > m_TargetTemperature - TOLERANCE)||
                    (m_CurrentTemperature[0] < m_TargetTemperature + TOLERANCE)) {
                return true;
            } else {
                return false;
            }
        }
    }
    Log(tr("Error"));
    return false;
}

/****************************************************************************/
/*!
 *  \brief  Helper function to check if the control state is in OutsideRange
 *
 *  This method checks if the temperature control state is outside range.
 *
 *  \return true, if the temperature control is OutsideRange, else false
 *
 */
/****************************************************************************/
bool WrapperFmTempControl::IsOutsideRange()
{
    if(GetTemperature(0) != UNDEFINED) {
        if((m_TargetTemperature != UNDEFINED) || (m_CurrentTemperature[0] != UNDEFINED)) {
            if ((m_CurrentTemperature[0] < m_TargetTemperature - TOLERANCE)||
                    (m_CurrentTemperature[0] > m_TargetTemperature + TOLERANCE)) {
                return true;
            } else {
                return false;
            }
        }
    }
    Log(tr("Error"));
    return false;
}

/****************************************************************************/
/*!
 *  \brief  Script-API: Check the mains voltage state
 *
 *  This method checks if the mains voltage state is in the 220V or the 110V
 *  range
 *
 *  Examples:
 *  \dontinclude tempcontrol.js
 *  \skipline [TempControl.GetMainsVoltageState]
 *  \until    [TempControl.GetMainsVoltageState]
 *
 *  \return string representation of the mains voltage state:
 *  220V, 110V
 *
 */
/****************************************************************************/
QString WrapperFmTempControl::GetMainsVoltageState()
{
    Log(tr("GetMainsVoltageState"));
    bool ok = HandleErrorCode(m_pTempControl->ReqStatus());
    if (!ok) {
        m_MainsVoltageStatus = TEMPCTRL_VOLTAGE_UNDEF;
        return "Error";
    }
    qint32 ret = m_LoopGetVoltageState.exec();
    QString voltagestate = "";
    if (ret != 1) {
        voltagestate = "Error";
    } else if (m_MainsVoltageStatus == TEMPCTRL_VOLTAGE_220V) {
        voltagestate = "220V";
    } else if (m_MainsVoltageStatus == TEMPCTRL_VOLTAGE_110V){
        voltagestate = "110V";
    }
    return voltagestate;
}

#ifdef PRE_ALFA_TEST
bool WrapperFmTempControl::SetTemperaturePid(quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime)
{
    Log(tr("SetTemperaturePID"));
    bool ok = HandleErrorCode(m_pTempControl->SetTemperaturePid(MaxTemperature, ControllerGain, ResetTime, DerivativeTime));
    if (!ok) {
        return false;
    }
    qint32 ret = m_LoopSetTemperaturePid.exec();
    return (ret == 1);
}
bool WrapperFmTempControl::SetTemperatureSwitchState(qint8 SwitchState, qint8 AutoSwitch)
{
    Log(tr("SetTemperatureSwitchState"));
    bool ok = HandleErrorCode(m_pTempControl->SetSwitchState(SwitchState, AutoSwitch));
    if (!ok) {
        return false;
    }
    qint32 ret = m_LoopSetSwitchState.exec();
    return (ret == 1);
}

void WrapperFmTempControl::OnSetSwitchState(quint32, ReturnCode_t ReturnCode, qint8 SwitchState, qint8 AutoSwitch)
{
    qint32 ret = 1;
    if (!HandleErrorCode(ReturnCode)) {
        ret = UNDEFINED;
    }
    if (m_LoopSetSwitchState.isRunning()) {
        m_LoopSetSwitchState.exit(ret);
    } else {
        Log(tr("NOTICE: Unexpected action acknowledgement."));
    }
}
#endif
