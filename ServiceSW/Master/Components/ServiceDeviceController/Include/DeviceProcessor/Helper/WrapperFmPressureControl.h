#ifndef WRAPPERFMPRESSURECONTROL_H
#define WRAPPERFMPRESSURECONTROL_H

#include <QObject>
#include <QTimer>
#include <QEventLoop>
#include "ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperBase.h"
#include "DeviceControl/Include/SlaveModules/PressureControl.h"
#include "DeviceControl/Include/SlaveModules/TemperatureControl.h"
#ifdef PRE_ALFA_TEST
using namespace DeviceControl;

class WrapperFmPressureControl : public WrapperBase
{

    Q_OBJECT

 public:
    typedef struct {
        quint8 Sensors;
        quint8 Fans;
        quint8 Heaters;
        quint8 Pids;
        quint16 Current;
    } HardwareStatus_t;


    typedef struct {
        qint8 TargetPressure;
        qint8 StaticDiff;
        qint32 Tv_Err;
        qint32 Tv_Delay;
        qint32 Polling_Time;
    }VaccumCfg_t;

    typedef struct {
        qint8 TargetPressure;
        qint8 StaticDiff;
        qint32 Tp_Err;
        qint32 Tp_Delay;
        qint32 Polling_Time;
    }PressureCfg_t;

    typedef struct {
        qint8 DariningPressure;
        qint8 UpLiminalPressure;
        qint8 DownLiminalPressure;
        qint32 DelayTime;
    }DrainingCfg_t;


    WrapperFmPressureControl(QString Name, CPressureControl *pPressureControl, QObject *pParent);

 public slots:
    bool StartPressureControl(quint8 flag, float NominalPressure);
    bool StopPressureControl();
    float GetPressure(quint8 Index = 0);
    QString GetPressureControlState();
    QString GetMainsVoltageState();
    bool IsPressureControlOn();
    bool SetFullPowerMode();
    bool SetPowerSaveMode();
    QString GetPowerOperationMode();
    bool ResetPumpOperatingTime(quint8 Index);
    quint32 GetPumpOperatingTime(quint8 Index);
    quint16 GetFanSpeed(quint8 Index);
    HardwareStatus_t *GetHardwareStatus();
    void Reset();
    bool SetValve(quint8 ValveIndex, quint8 ValveState);
    bool SetFan(quint8 State);
    bool SetTargetPressure(quint8 flag, float pressure);
    bool SetCalibration(bool Enable);
    qint32 Draining(quint32 DelayTime = 2000, quint32 TubePosition = 0);
    qint32 Sucking(quint32 DelayTime = 0, quint32 TubePosition = 0,  bool IsAgitation = false);
    bool Vaccum(void);
    bool Pressure(void);
    bool Agitation(quint32 TubePosition);
    bool ReleasePressure(void);
    void StopCompressor(void);

    void SetWorkingPressure(qint32 WorkingPressure);
    void GetWorkingPressure(void);
    void GetValveOperationTime(quint8 ValveIndex);
    void ResetValveOperationTime();
#if 1
    bool SetPressure(quint8 flag, float NominalPressure);
    float ReadPressureDrift(void);
    void WritePressureDrift(float PressureDrift);
#endif
private slots:
    void OnGetPressure(quint32, ReturnCode_t ReturnCode, quint8 Index, float Pressure);
    void OnSetPressure(quint32, ReturnCode_t ReturnCode, float Pressure);
    void OnPressureControlStatus(quint32, ReturnCode_t ReturnCode, PressureCtrlStatus_t PressureCtrlStatus, PressureCtrlMainsVoltage_t MainsVoltage);
    void OnSetPressureCtrlOpMode(quint32, ReturnCode_t ReturnCode, PressureCtrlOperatingMode_t PressureCtrlOpMode);
    void OnResetPumpOperatingTime(quint32, ReturnCode_t ReturnCode, quint8 Index);
    void OnGetPumpOperatingTime(quint32, ReturnCode_t ReturnCode, quint8 Index, quint32 OperatingTime);
    void OnGetFanSpeed(quint32, ReturnCode_t ReturnCode, quint8 Index, quint16 FanSpeed);
    void OnGetHardwareStatus(quint32, ReturnCode_t ReturnCode, quint8 Sensors, quint8 Fans,
                             quint8 Heaters, quint8 Pids, quint16 Current);
    void OnSetValve(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, quint8 ValveIndex, quint8 ValveState);
    void OnSetFan(quint32 /*InstanceID*/, ReturnCode_t ReturnCode, quint8 FanState);
    void DrainingTimerCB(void);
    void AgitationTimerCB(void);
    void SuckingTimerCB(void);
    void PressureTimerCB(void);
    void VaccumTimerCB(void);
    void ReleasePressureTimerCB(void);
    //void checkDrainingProgress(void);
    void OnLevelSensorState(quint32, ReturnCode_t ReturnCode, quint8 State);
#if 1
    void OnError(quint32 InstanceID, quint16 ErrorGroup, quint16 ErrorCode, quint16 ErrorData, QDateTime ErrorTime);
#endif
signals:
    /****************************************************************************/
    /*!
     *  \brief Pressure in range and out of range notification
     *
     *      This notification is reported whenever the pressure measured by
     *      the function module enters or leaves a predifined tolerance range.
     *
     *  \iparam ReturnCode = ReturnCode from the function module
     *  \iparam InRange = true if in range, false if out of range
     *  \iparam Pressure = Pressure currently measured
     */
    /****************************************************************************/
    void OnPressureRange(quint32, ReturnCode_t ReturnCode, bool InRange, float Pressure);

  private:
    bool SetOperatingMode(PressureCtrlOperatingMode_t PressureCtrlOpMode);
    bool SetPressureCtrlON();
    bool IsPIDDataSteady(float TargetValue, float CurrentValue, float Tolerance, qint32 Num, bool Init);
    bool SetPressureControlStatus(PressureCtrlStatus_t PressureCtrlStatus);
    bool IsPressureControlOff();
    bool IsInsideRange();
    bool IsOutsideRange();



    QEventLoop m_LoopSetPressure;               //!< Loop for blocking commands.
    QEventLoop m_LoopGetPressure;               //!< Loop for blocking commands.
    QEventLoop m_LoopSetPressureControl;        //!< Loop for blocking commands.
    QEventLoop m_LoopGetPressureControl;        //!< Loop for blocking commands.
    QEventLoop m_LoopGetVoltageState;           //!< Loop for blocking commands.
    QEventLoop m_LoopSetOperatingMode;          //!< Loop for blocking commands.
    QEventLoop m_LoopResetHeaterOperatingTime;  //!< Loop for blocking commands.
    QEventLoop m_LoopGetHeaterOperatingTime;    //!< Loop for blocking commands.
    QEventLoop m_LoopGetFanSpeed;               //!< Loop for blocking commands.
    QEventLoop m_LoopGetHardwareStatus;         //!< Loop for blocking commands.
    QEventLoop m_LoopSetValve;                  //!< Loop for blocking commands.
    QEventLoop m_LoopSetFan;                    //!< Loop for blocking commands.

    QEventLoop m_LoopDrainingTimer;
    QEventLoop m_LoopSuckingTimer;
    QEventLoop m_LoopSuckingLevelSensor;
    QEventLoop m_LoopPressureTimer;
    QEventLoop m_LoopVaccumTimer;
    QEventLoop m_LoopAgitationTimer;
    QEventLoop m_LoopReleasePressureTimer;

    CPressureControl *m_pPressureControl;                //!< Pointer to the Pressure Control function module
    float m_TargetPressure;                              //!< Target pressure; for verification of action result.
    float m_CurrentPressure;                             //!< Current pressure
    PressureCtrlStatus_t m_TargetPressureCtrlStatus;     //!< Target pressure control status; for verification of action result.
    PressureCtrlStatus_t m_CurrentPressureCtrlStatus;    //!< Current pressure control status
    PressureCtrlOperatingMode_t m_TargetOperatingMode;   //!< Target operatig mode; for verification of action result.
    PressureCtrlOperatingMode_t m_CurrentOperatingMode;  //!< Current operating mode
    quint32 m_HeaterOperatingTime;                       //!< Heater operating time in seconds
    quint16 m_FanSpeed;                                  //!< Fan speed in revelations per minute
    HardwareStatus_t m_HardwareStatus;                   //!< Hardware configuration of the Slave
    PressureCtrlMainsVoltage_t m_MainsVoltageStatus;     //!< Mains voltage state of the heaters

    quint32 m_SuckingTime[50];  // in mec, idx 1-13 is port 1-13, idx 0 is unused
    DrainingCfg_t m_DrainingCfgData;
    PressureCfg_t m_PressureCfgData;
    VaccumCfg_t m_VaccumCfgData;
    qint32 m_WorkingPressurePositive;
    qint32 m_WorkingPressureNegative;
    float m_PressureDrift;
};

#endif //PRE_ALFA_TEST

#endif // WRAPPERFMPRESSURECONTROL_H
