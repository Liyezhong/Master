/****************************************************************************/
/*! \file WrapperFmTempControl.h
 *
 *  \brief  Definition file for class WrapperFmTempControl
 *
 *  \version  0.1
 *  \date     2012-04-06
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
#ifndef WRAPPERFMTEMPCONTROL_H
#define WRAPPERFMTEMPCONTROL_H
#include <QObject>
#include <QTimer>
#include <QEventLoop>
#include "WrapperBase.h"
#include "DeviceControl/Include/SlaveModules/TemperatureControl.h"

using namespace DeviceControl;

/****************************************************************************/
/*! \brief Wrapper for Temperature control function module
 *
 *  This wrapper class provides a QtScript interface to 'temperature control' function module
 *
 *
 *  Examples:
 *   \dontinclude tempcontrol.js
 *   \skipline [WrapperFmTempControl]
 *   \until    [WrapperFmTempControl]
 */
/****************************************************************************/
class WrapperFmTempControl : public WrapperBase
{
    Q_OBJECT

 public:
    typedef struct {
        quint8 Sensors;
        quint8 Fans;
        quint8 Heaters;
        quint8 Pids;
        quint16 Current;
        quint8 HeaterSwitchType;
    } HardwareStatus_t;

    WrapperFmTempControl(QString Name, CTemperatureControl *pTempControl, QObject *pParent);

 public slots:
    bool StartTemperatureControl(qreal NominalTemperature, quint8 SlopeTempChange=99);
    bool StopTemperatureControl();
    qreal GetTemperature(quint8 Index = 0);
    QString GetTemperatureControlState();
    QString GetMainsVoltageState();
    bool IsTemperatureControlOn();
    bool SetFullPowerMode();
    bool SetPowerSaveMode();
    QString GetPowerOperationMode();
    bool ResetHeaterOperatingTime(quint8 Index);
    quint32 GetHeaterOperatingTime(quint8 Index);
    quint16 GetFanSpeed(quint8 Index);
    HardwareStatus_t *GetHardwareStatus();
    quint16 GetCurrent();
    void Reset();
#ifdef PRE_ALFA_TEST
    bool SetTemperaturePid(quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime);
#endif

private slots:
    void OnGetTemp(quint32, ReturnCode_t ReturnCode, quint8 Index, qreal Temperature);
    void OnSetTemp(quint32, ReturnCode_t ReturnCode, qreal Temperature);
    void OnTempControlStatus(quint32, ReturnCode_t ReturnCode, TempCtrlStatus_t TempCtrlStatus, TempCtrlMainsVoltage_t MainsVoltage);
    void OnSetTempCtrlOpMode(quint32, ReturnCode_t ReturnCode, TempCtrlOperatingMode_t TempCtrlOpMode);
    void OnResetHeaterOperatingTime(quint32, ReturnCode_t ReturnCode, quint8 Index);
    void OnGetHeaterOperatingTime(quint32, ReturnCode_t ReturnCode, quint8 Index, quint32 OperatingTime);
    void OnGetFanSpeed(quint32, ReturnCode_t ReturnCode, quint8 Index, quint16 FanSpeed);
//    void OnGetHardwareStatus(quint32, ReturnCode_t ReturnCode, quint8 Sensors, quint8 Fans,
//                             quint8 Heaters, quint8 Pids, quint16 Current);
    void OnGetHardwareStatus(quint32, ReturnCode_t ReturnCode, quint8 Sensors, quint8 Fans,
                                               quint8 Heaters, quint8 Pids, quint16 Current, quint8 HeaterSwitchType);
#ifdef PRE_ALFA_TEST
    //void OnLevelSensorState(quint32, ReturnCode_t ReturnCode, quint8 State);
    void OnSetTempPid(quint32, ReturnCode_t ReturnCode, quint16 MaxTemperature,
                      quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime);
    void OnError(quint32 InstanceID, quint16 ErrorGroup, quint16 ErrorCode, quint16 ErrorData, QDateTime ErrorTime);
#endif
    /****************************************************************************/
    /*!
     *  \brief Temperature in range and out of range notification
     *
     *      This notification is reported whenever the temperature measured by
     *      the function module enters or leaves a predifined tolerance range.
     *
     *  \iparam ReturnCode = ReturnCode from the function module
     *  \iparam InRange = true if in range, false if out of range
     *  \iparam Temperature = Temperature currently measured
     */
    /****************************************************************************/
    void OnTemperatureRange(quint32 InstanceID, ReturnCode_t ReturnCode, bool InRange, qreal Temperature);
  private:
    bool SetOperatingMode(TempCtrlOperatingMode_t TempCtrlOpMode);
    bool SetTempCtrlON();
    bool SetTemperature(qreal NominalTemperature, quint8 SlopeTempChange=0);
    bool SetTemperatureControlStatus(TempCtrlStatus_t TempCtrlStatus);
    bool IsTemperatureControlOff();
    bool IsInsideRange();
    bool IsOutsideRange();

    QEventLoop m_LoopSetTemperature;            //!< Loop for blocking commands.
    QEventLoop m_LoopGetTemperature;            //!< Loop for blocking commands.
    QEventLoop m_LoopSetTemperatureControl;     //!< Loop for blocking commands.
    QEventLoop m_LoopGetTemperatureControl;     //!< Loop for blocking commands.
    QEventLoop m_LoopGetVoltageState;           //!< Loop for blocking commands.
    QEventLoop m_LoopSetOperatingMode;          //!< Loop for blocking commands.
    QEventLoop m_LoopResetHeaterOperatingTime;  //!< Loop for blocking commands.
    QEventLoop m_LoopGetHeaterOperatingTime;    //!< Loop for blocking commands.
    QEventLoop m_LoopGetFanSpeed;               //!< Loop for blocking commands.
    QEventLoop m_LoopGetHardwareStatus;         //!< Loop for blocking commands.
#ifdef PRE_ALFA_TEST
    QEventLoop m_LoopSetTemperaturePid;         //!< Loop for blocking commands.
#endif

    CTemperatureControl *m_pTempControl;            //!< Pointer to the Temperature Control function module
    qreal m_TargetTemperature;                      //!< Target temperature; for verification of action result.
    qreal m_CurrentTemperature;                     //!< Current temperature
    TempCtrlStatus_t m_TargetTempCtrlStatus;        //!< Target temperature control status; for verification of action result.
    TempCtrlStatus_t m_CurrentTempCtrlStatus;       //!< Current temperature control status
    TempCtrlOperatingMode_t m_TargetOperatingMode;  //!< Target operatig mode; for verification of action result.
    TempCtrlOperatingMode_t m_CurrentOperatingMode; //!< Current operating mode
    quint32 m_HeaterOperatingTime;                  //!< Heater operating time in seconds
    quint16 m_FanSpeed;                             //!< Fan speed in revelations per minute
    HardwareStatus_t m_HardwareStatus;              //!< Hardware configuration of the Slave
    TempCtrlMainsVoltage_t m_MainsVoltageStatus;    //!< Mains voltage state of the heaters
    qint64 m_LastGetTempTime[50];
};

#endif // WRAPPERFMTEMPCONTROL_H
