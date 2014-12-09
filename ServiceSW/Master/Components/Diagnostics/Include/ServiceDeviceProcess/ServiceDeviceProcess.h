/****************************************************************************/
/*! \file ServiceDeviceProcess.h
 *
 *  \brief Definition file for class ServiceDeviceProcess.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2014-09-09
 *  $Author:    $ Sunny Qu
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef SERV_DEV_PROC_H
#define SERV_DEV_PROC_H

#include <QEventLoop>
#include <QMap>
#include <QStringList>
#include <Global/Include/GlobalDefines.h>
#include "Core/Include/ServiceDefines.h"
#include "Diagnostics/Include/ServiceDeviceProcess/ServiceDeviceProcess.h"


namespace Diagnostics {

typedef enum {
    RETURN_OK = 1,
    RETURN_ERR_NULL_POINTER = -1,
    RETURN_ERR_FAIL = -2,
    RETURN_ABORT = -3
} ErrorCode_t;

/****************************************************************************/
/**
 * \brief This class is a helper to do handle device processing related commands.
 *
 * This class has thread controller and Interface for the device command
 * processing. It communicates with the real hardware with signals and slots.
 * \warning This class is not thread safe!
 */
/****************************************************************************/

class ServiceDeviceProcess : public QObject
{
    Q_OBJECT
private:
    static ServiceDeviceProcess* mp_Instance;  ///< The one and only instance.

    /****************************************************************************/
    /**
     * \brief Default constructor.
     */
    /****************************************************************************/
    ServiceDeviceProcess();

public:
    /****************************************************************************/
    /**
     * \brief Get pointer to instance.
     * \return  pointer to instance.
     */
    /****************************************************************************/
    static ServiceDeviceProcess* Instance();

    /****************************************************************************/
    /**
     * \brief To destroy this instance.
     */
    /****************************************************************************/
    static void Destroy();

public:

    /****************************************************************************/
    /**
     * \brief  To initiatize devices
     */
    /****************************************************************************/
    void Initialize();
    /****************************************************************************/
    /**
     * \brief Get Pointer to DataManager Object
     * \return True if Device Initialization is completed.
     */
    /****************************************************************************/
    bool IsInitialized();

    /****************************************************************************/
    /**
     *  \brief Set main relay status
     *  \iparam OnFlag = flag of status
     *  \return Error Code
     */
    /****************************************************************************/
    int MainRelaySetOnOff(bool OnFlag);

    /****************************************************************************/
    /**
     *  \brief Set Alarm status
     *  \iparam LocalRemote = Flag of Local or remote 1--local  2--remote
     *  \iparam OnFlag = flag of status
     *  \return Error Code
     */
    /****************************************************************************/
    int AlarmSetOnOff(int LocalRemote, bool OnFlag);

    /****************************************************************************/
    /**
     *  \brief Set Alarm status
     *  \iparam LocalRemote = Flag of Local or remote 1--local  2--remote
     *  \oparam RetState = result of status
     *  \return Error Code
     */
    /****************************************************************************/
    int AlarmGetState(int LocalRemote, qint32 *RetState);

    /****************************************************************************/
    /**
     *  \brief To get main control current
     *  \iparam SlaveType = the type of slave of HimSlaveType_t
     *  \oparam RetCurrent = result of current
     *  \return Error Code
     */
    /****************************************************************************/
    int MainControlGetCurrent(quint8 SlaveType, quint16 *RetCurrent);

    /****************************************************************************/
    /**
     *  \brief To get main control voltage
     *  \iparam SlaveType = the type of slave of HimSlaveType_t
     *  \oparam RetVoltage = result of voltage
     *  \return Error Code
     */
    /****************************************************************************/
    int MainControlGetVoltage(quint8 SlaveType, quint16 *RetVoltage);

    /****************************************************************************/
    /**
     *  \brief To Start oven heating
     *  \iparam TargetTempTop = the target temperature oven top
     *  \iparam TargetTempBottom = the target temperature ove bottom
     *  \return Error Code
     */
    /****************************************************************************/
    int OvenStartHeating(qreal TargetTempTop, qreal TargetTempBottom);

    /****************************************************************************/
    /**
     *  \brief To stop oven heating
     *  \return Error Code
     */
    /****************************************************************************/
    int OvenStopHeating();

    /****************************************************************************/
    /**
     *  \brief To stop oven heating
     *  \return Error Code
     */
    /****************************************************************************/
    int OvenGetTemp(qreal *RetTempTop, qreal *RetTempBottom1, qreal *RetTempBottom2);

    /****************************************************************************/
    /**
     *  \brief To get oven current
     *  \oparam RetCurrentTop = result of oven top current
     *  \oparam RetCurrentBottom = result of oven bottom current
     *  \return Error Code
     */
    /****************************************************************************/
    int OvenGetCurrent(quint16 *RetCurrentTop, quint16 *RetCurrentBottom);

    /****************************************************************************/
    /**
     *  \brief To get oven cover sensor state
     *  \oparam RetCoverSensorState = result of oven cover sensor state  0--close 1--open
     *  \return Error Code
     */
    /****************************************************************************/
    int OvenGetCoverSensorState(qint32 *RetCoverSensorState);

    /****************************************************************************/
    /**
     *  \brief To get oven switch type
     *  \oparam RetSwithType = result of oven switch type
     *  \return Error Code
     */
    /****************************************************************************/
    int OvenGetSwitchType(int *RetSwithType);

    /****************************************************************************/
    /**
     *  \brief To get oven temperature control
     *  \oparam RetIsOn = result of oven temperature control true---on false---off
     *  \return Error Code
     */
    /****************************************************************************/
    int OvenTempControlIsOn(bool *RetIsOn);

    /****************************************************************************/
    /**
     *  \brief To start retort heating
     *  \iparam TargetTempSide = target temperature of retort side
     *  \iparam TargetTempBottom = target temperature of retort bottom
     *  \return Error Code
     */
    /****************************************************************************/
    int RetortStartHeating(qreal TargetTempSide, qreal TargetTempBottom);

    /****************************************************************************/
    /**
     *  \brief To stop retort heating
     *  \return Error Code
     */
    /****************************************************************************/
    int RetortStopHeating();

    /****************************************************************************/
    /**
     *  \brief To get retort temperature
     *  \oparam RetTempSide = result temperature of retort side
     *  \oparam RetTempBottom1 = result temperature of retort bottom 1
     *  \oparam RetTempBottom2 = result temperature of retort bottom 2
     *  \return Error Code
     */
    /****************************************************************************/
    int RetortGetTemp(qreal *RetTempSide, qreal *RetTempBottom1, qreal *RetTempBottom2);

    /****************************************************************************/
    /**
     *  \brief To get retort current
     *  \oparam RetCurrentSide = result current of retort side
     *  \oparam RetCurrentBottom = result current of retort bottom
     *  \return Error Code
     */
    /****************************************************************************/
    int RetortGetCurrent(quint16 *RetCurrentSide, quint16 *RetCurrentBottom);

    /****************************************************************************/
    /**
     *  \brief To get retort lid lock state
     *  \oparam RetLidLockState = result of retort lid lock state 0--close 1--open
     *  \return Error Code
     */
    /****************************************************************************/
    int RetortGetLidLockState(qint32 *RetLidLockState);

    /****************************************************************************/
    /**
     *  \brief To set retort temperature switch state
     *  \iparam SwitchState = switch state
     *  \iparam AutoSwitch = auto switch flag
     *  \return Error Code
     */
    /****************************************************************************/
    int RetortSetTemperatureSwitchState(qint8 SwitchState, qint8 AutoSwitch);

    /****************************************************************************/
    /**
     *  \brief To get retort heater switch type
     *  \oparam RetSwitchType = result swithc type
     *  \return Error Code
     */
    /****************************************************************************/
    int RetortGetHeaterSwitchType(quint8 *RetSwitchType);

    /****************************************************************************/
    /**
     *  \brief To get retort temperature control state
     *  \oparam RetIsOn = result retort temperature control
     *  \return Error Code
     */
    /****************************************************************************/
    int RetortTempControlIsOn(bool *RetIsOn);

    /****************************************************************************/
    /**
     *  \brief To start liquid tube heating
     *  \iparam TargetTemp = target temperature of liquid tube
     *  \return Error Code
     */
    /****************************************************************************/
    int LiquidTubeStartHeating(qreal TargetTemp);

    /****************************************************************************/
    /**
     *  \brief To stop liquid tube heating
     *  \return Error Code
     */
    /****************************************************************************/
    int LiquidTubeStopHeating();

    /****************************************************************************/
    /**
     *  \brief To get liquid tube temperature
     *  \oparam RetTemp = result of the temperature
     *  \return Error Code
     */
    /****************************************************************************/
    int LiquidTubeGetTemp(qreal *RetTemp);

    /****************************************************************************/
    /**
     *  \brief To get liquid tube current
     *  \oparam RetCurrent = result of the current
     *  \return Error Code
     */
    /****************************************************************************/
    int LiquidTubeGetCurrent(quint16 *RetCurrent);

    /****************************************************************************/
    /**
     *  \brief To get liquid tube temperature control state
     *  \oparam RetIsOn = result of the temperature control state
     *  \return Error Code
     */
    /****************************************************************************/
    int LiquidTubeTempControlIsOn(bool *RetIsOn);

    /****************************************************************************/
    /**
     *  \brief To start air tube heating
     *  \iparam TargetTemp = target temperature of the air tube
     *  \return Error Code
     */
    /****************************************************************************/
    int AirTubeStartHeating(qreal TargetTemp);

    /****************************************************************************/
    /**
     *  \brief To stop air tube heating
     *  \return Error Code
     */
    /****************************************************************************/
    int AirTubeStopHeating();

    /****************************************************************************/
    /**
     *  \brief To get air tube temperature
     *  \oparam RetTemp = result temperature of the air tube
     *  \return Error Code
     */
    /****************************************************************************/
    int AirTubeGetTemp(qreal *RetTemp);

    /****************************************************************************/
    /**
     *  \brief To get air tube current
     *  \oparam RetCurrent = result current of the air tube
     *  \return Error Code
     */
    /****************************************************************************/
    int AirTubeGetCurrent(quint16 *RetCurrent);

    /****************************************************************************/
    /**
     *  \brief To get air tube temperature control state
     *  \oparam RetIsOn = result temperature control state of the air tube
     *  \return Error Code
     */
    /****************************************************************************/
    int AirTubeTempControlIsOn(bool *RetIsOn);

    /****************************************************************************/
    /**
     *  \brief To start rotary valve heating
     *  \iparam TargetTemp = target temperature of rotary valve
     *  \return Error Code
     */
    /****************************************************************************/
    int RVStartHeating(qreal TargetTemp);

    /****************************************************************************/
    /**
     *  \brief To stop rotary valve heating
     *  \return Error Code
     */
    /****************************************************************************/
    int RVStopHeating();

    /****************************************************************************/
    /**
     *  \brief To get rotary valve temperature
     *  \oparam RetTempSensor1 = result temperature of rotary valve sensor1
     *  \oparam RetTempSensor2 = result temperature of rotary valve sensor2
     *  \return Error Code
     */
    /****************************************************************************/
    int RVGetTemp(qreal *RetTempSensor1, qreal* RetTempSensor2);

    /****************************************************************************/
    /**
     *  \brief To get rotary valve current
     *  \oparam RetCurrent = result current of rotary valve
     *  \return Error Code
     */
    /****************************************************************************/
    int RVGetCurrent(quint16 *RetCurrent);

    /****************************************************************************/
    /**
     *  \brief To initialize rotary valve position
     *  \iparam TubeFlag = flag of tube of sealing position
     *  \iparam Position = target position
     *  \return Error Code
     */
    /****************************************************************************/
    int RVInitialize(bool TubeFlag=false, quint32 Position=0);

    /****************************************************************************/
    /**
     *  \brief To move rotary valve position
     *  \iparam TubeFlag = flag of tube of sealing position
     *  \iparam Position = target position
     *  \return Error Code
     */
    /****************************************************************************/
    int RVMovePosition(bool TubeFlag, int Position);

    /****************************************************************************/
    /**
     *  \brief To get rotary valve position
     *  \oparam TubeFlag = flag of tube of sealing position
     *  \oparam Position = result position of rotary valve
     *  \return Error Code
     */
    /****************************************************************************/
    int RVGetPosition(bool* TubeFlag, qint32 *Position);

    /****************************************************************************/
    /**
     *  \brief To set rotary valve temperature switch state
     *  \iparam SwitchState = temperature switch state
     *  \iparam AutoSwitch = flag of auto switch
     *  \return Error Code
     */
    /****************************************************************************/
    int RVSetTemperatureSwitchState(qint8 SwitchState, qint8 AutoSwitch);

    /****************************************************************************/
    /**
     *  \brief To get rotary valve heat switch state
     *  \oparam RetSwitchType = result of heat switch state
     *  \return Error Code
     */
    /****************************************************************************/
    int RVGetHeaterSwitchType(quint8 *RetSwitchType);

    /****************************************************************************/
    /**
     *  \brief To get rotary valve temperature control state
     *  \oparam RetIsOn = result of temperature control state
     *  \return Error Code
     */
    /****************************************************************************/
    int RVTempControlIsOn(bool *RetIsOn);

    /****************************************************************************/
    /**
     *  \brief To start level sensor heating
     *  \iparam QuickFlag = flag of quick heating
     *  \iparam WaterFlag = flag of with water
     *  \return Error Code
     */
    /****************************************************************************/
    int LSStartHeating(bool QuickFlag, bool WaterFlag);

    /****************************************************************************/
    /**
     *  \brief To stop level sensor heating
     *  \return Error Code
     */
    /****************************************************************************/
    int LSStopHeating();

    /****************************************************************************/
    /**
     *  \brief To get level sensor temperature
     *  \oparam RetTemp = result temperature of level sensor
     *  \return Error Code
     */
    /****************************************************************************/
    int LSGetTemp(qreal *RetTemp);

    /****************************************************************************/
    /**
     *  \brief To get level sensor current
     *  \oparam RetCurrent = result current of level sensor
     *  \return Error Code
     */
    /****************************************************************************/
    int LSGetCurrent(quint16 *RetCurrent);

    /****************************************************************************/
    /**
     *  \brief To heating level sensor
     *  \oparam WaterFlag = flag of heating with water
     *  \return Error Code
     */
    /****************************************************************************/
    int LSHeatingLevelSensor(bool WaterFlag);

    /****************************************************************************/
    /**
     *  \brief To get level sensor heating temperature control state
     *  \oparam RetIsOn = result of temperature control state
     *  \return Error Code
     */
    /****************************************************************************/
    int LSTempControlIsOn(bool *RetIsOn);


    /****************************************************************************/
    /**
     *  \brief To build pressure
     *  \iparam TargetPressure = target pressure of build
     *  \return Error Code
     */
    /****************************************************************************/
    int PumpBuildPressure(float TargetPressure);

    /****************************************************************************/
    /**
     *  \brief To release pressure
     *  \return Error Code
     */
    /****************************************************************************/
    int PumpReleasePressure();

    /****************************************************************************/
    /**
     *  \brief To set pressure
     *  \iparam Flag = flag of pressure set
     *  \iparam Pressure = target pressure to set
     *  \return Error Code
     */
    /****************************************************************************/
    int PumpSetPressure(quint8 Flag, float Pressure);

    /****************************************************************************/
    /**
     *  \brief To get pressure
     *  \iparam RetPressure = result pressure of current
     *  \return Error Code
     */
    /****************************************************************************/
    int PumpGetPressure(float *RetPressure);

    /****************************************************************************/
    /**
     *  \brief To set fan state
     *  \iparam OnFlag = flag of on or off
     *  \return Error Code
     */
    /****************************************************************************/
    int PumpSetFan(quint8 OnFlag);

    /****************************************************************************/
    /**
     *  \brief To get fan state
     *  \return Error Code
     */
    /****************************************************************************/
    bool PumpGetFan();


    /****************************************************************************/
    /**
     *  \brief To get pump state
     *  \return Error Code
     */
    /****************************************************************************/
    bool PumpGetStatus();

   /****************************************************************************/
    /**
     *  \brief To set valve of pump
     *  \iparam ValveIndex = index of valve
     *  \iparam ValveState = state of valve
     *  \return Error Code
     */
    /****************************************************************************/
    int PumpSetValve(quint8 ValveIndex, quint8 ValveState);

    /****************************************************************************/
     /**
      *  \brief To get valve of pump
     *  \iparam ValveIndex = index of valve
     *  \iparam ValveState = state of valve
      */
     /****************************************************************************/
     void PumpGetValve(quint8 &ValveIndex, quint8 &ValveState);

    /****************************************************************************/
    /**
     *  \brief To stop compressor
     *  \return Error Code
     */
    /****************************************************************************/
    int PumpStopCompressor();

    /****************************************************************************/
    /**
     *  \brief To start sucking
     *  \iparam DelayTime = suck delay time
     *  \return Error Code
     */
    /****************************************************************************/
    int PumpSucking(quint32 DelayTime=0);

    /****************************************************************************/
    /**
     *  \brief To start draining
     *  \iparam DelayTime = drain delay time
     *  \return Error Code
     */
    /****************************************************************************/
    int PumpDraining(quint32 DelayTime=0);

    /****************************************************************************/
    /**
     *  \brief To read pressure dirft
     *  \oparam RetPressureDrift = result of pressure drift
     *  \return Error Code
     */
    /****************************************************************************/
    int PumpReadPressureDrift(float *RetPressureDrift);

    /****************************************************************************/
    /**
     *  \brief To write pressure dirft
     *  \oparam PressureDrift = target of pressure drift
     *  \return Error Code
     */
    /****************************************************************************/
    int PumpWritePressureDrift(float PressureDrift);

    /****************************************************************************/
    /**
     *  \brief To get slave module retort error
     *  \iparam ErrorCode = the code of error
     *  \iparam DevName  = device of name
     *  \ipram SensorName = name of sensor
     *  \return Error Code
     */
    /****************************************************************************/
    int GetSlaveModuleReportError(quint8 ErrorCode, const QString& DevName, quint32 SensorName);

    /****************************************************************************/
    /**
     *  \brief To pause time
     *  \iparam MillSeconds = target of pause time of mill senconds
     */
    /****************************************************************************/
    void Pause(quint32 MillSeconds);

private:
    QMap<QString, QEventLoop*>      m_EventLoopMap;  //!< store event loop
    QMap<QString, QStringList>      m_ResultsMap;    //!< store results
    bool                            m_IsInitialized;    //!< flag of initialized
    bool                            m_FanStatus;     //!< store fan status, on/off
    QMap<qint8, qint8>                  m_ValveStatus;     //!< store valve status

public slots:

    /****************************************************************************/
    /**
     *  \brief slot of handle service result
     *  \iparam ReqName = request name
     *  \iparam Error = the code of error
     *  \iparam Results = list of result
     */
    /****************************************************************************/
    void HandleServResult(QString ReqName, int Error, QStringList Results);
signals:

    /****************************************************************************/
    /**
     *  \brief signal to send service request
     *  \iparam ReqName = request name
     *  \iparam Params = list of params
     */
    /****************************************************************************/
    void SendServRequest(QString ReqName, QStringList Params);
private:

    /****************************************************************************/
    /**
     *  \brief to get response for block
     *  \iparam ReqName = request name
     *  \iparam TimeOutSeconds = time of seconds
     *  \return Error Code
     */
    /****************************************************************************/
    int GetResponse(QString ReqName, int TimeOutSeconds=10);
};

} // end namespace Diagnostics

#endif // SERV_DEV_PROC_H
