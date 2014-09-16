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

#ifndef SERVICE_DEVICE_PROCESS_H
#define SERVICE_DEVICE_PROCESS_H


#include <Global/Include/GlobalDefines.h>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Core/Include/ServiceDefines.h"
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmDigitalInput.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmDigitalOutput.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmBaseModule.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmBootLoader.h>
#include "DeviceControl/Include/Interface/IDeviceProcessing.h"

using namespace DeviceControl;

class WrapperUtils;
class WrapperFmTempControl;
class WrapperFmStepperMotor;
class WrapperFmPressureControl;


namespace Diagnostics {

class DeviceControl::IDeviceProcessing;

typedef enum {
    RETURN_OK = 0,
    RETURN_ERR_NULL_POINTER = -1,
    RETURN_ERR_FAIL = 1
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
    static ServiceDeviceProcess* mp_Instance;
    ServiceDeviceProcess();

public:
    static ServiceDeviceProcess* Instance();
    static void Destroy();

public:
    void SetIDevProc(DeviceControl::IDeviceProcessing* iDevProc);

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
    /****************************************************************************/
    /**
     * \brief Helper function to create wrappers
     */
    /****************************************************************************/
    void CreateWrappers();

    ErrorCode_t MainRelaySetOnOff(bool OnFlag);
    ErrorCode_t AlarmSetOnOff(int LocalRemote, bool OnFlag);
    ErrorCode_t AlarmGetState(int LocalRemote, qint32 *RetState);
    ErrorCode_t MainControlGetCurrent(HimSlaveType_t SlaveType, quint16 *RetCurrent);
    ErrorCode_t MainControlGetVoltage(HimSlaveType_t SlaveType, quint16 *RetVoltage);

    ErrorCode_t OvenStartHeating(qreal TargetTempTop, qreal TargetTempBottom);
    ErrorCode_t OvenStopHeating();
    ErrorCode_t OvenGetTemp(qreal *RetTempTop, qreal *RetTempBottom1, qreal *RetTempBottom2);
    ErrorCode_t OvenGetCurrent(qreal *RetCurrentTop, qreal *RetCurrentBottom);
    ErrorCode_t OvenGetCoverSensorState(qint32 *RetCoverSensorState);
    ErrorCode_t OvenGetSwithType(int *RetSwithType);

    ErrorCode_t RetortStartHeating(qreal TargetTempSide, qreal TargetTempBottom);
    ErrorCode_t RetortStopHeating();
    ErrorCode_t RetortGetTemp(qreal *RetTempSide, qreal *RetTempBottom1, qreal *RetTempBottom2);
    ErrorCode_t RetortGetCurrent(qreal *RetCurrentSide, qreal *RetCurrentBottom);
    ErrorCode_t RetortGetLidLockState(qint32 *RetLidLockState);

    ErrorCode_t LiquidTubeStartHeating(qreal TargetTemp);
    ErrorCode_t LiquidTubeStopHeating();
    ErrorCode_t LiquidTubeGetTemp(qreal *RetTemp);
    ErrorCode_t LiquidTubeGetCurrent(quint16 *RetCurrent);

    ErrorCode_t AirTubeStartHeating(qreal TargetTemp);
    ErrorCode_t AirTubeStopHeating();
    ErrorCode_t AirTubeGetTemp(qreal *RetTemp);
    ErrorCode_t AirTubeGetCurrent(quint16 *RetCurrent);

    ErrorCode_t RVStartHeating(qreal TargetTemp);
    ErrorCode_t RVStopHeating();
    ErrorCode_t RVGetTemp(qreal *RetTempSensor1, qreal* RetTempSensor2);
    ErrorCode_t RVGetCurrent(quint16 *RetCurrent);
    ErrorCode_t RVInitialize();
    ErrorCode_t RVMovePosition(bool TubeFlag, int Position);

    ErrorCode_t LSStartHeating(bool QuickFlag, bool WaterFlag);
    ErrorCode_t LSStopHeating();
    ErrorCode_t LSGetTemp(qreal *RetTemp);
    ErrorCode_t LSGetCurrent(qreal *RetCurrent);

    ErrorCode_t PumpBuildPressure(float TargetPressure);
    ErrorCode_t PumpReleasePressure();
    ErrorCode_t PumpGetPressure(float *RetPressure);
    ErrorCode_t PumpSetFan(bool OnFlag);
    ErrorCode_t PumpSetValve(quint8 ValveIndex, quint8 ValveState);
    ErrorCode_t PumpStopCompressor();
    ErrorCode_t PumpSucking(int DelayTime);
    ErrorCode_t PumpDraining();

    void Pause(quint32 MillSeconds);

private:
    DeviceControl::IDeviceProcessing*           m_rIdevProc;           //!< IDeviceProcessing reference to interact with device
    bool                        m_IsConfigured;         //!< Indicates if configuration is successfully completed

    WrapperUtils*               mp_Utils;
    // Temperature control modules
    WrapperFmTempControl*       mp_TempOvenTop;
    WrapperFmTempControl*       mp_TempOvenBottom;

    WrapperFmTempControl*       mp_TempRetortSide;
    WrapperFmTempControl*       mp_TempRetortBottom;

    WrapperFmDigitalInput*      mp_DIRetortLid;

    WrapperFmDigitalInput*      mp_DIOven;
    WrapperFmDigitalOutput*     mp_DOMainRelay;

    WrapperFmTempControl*       mp_TempTubeLiquid;
    WrapperFmTempControl*       mp_TempTubeAir;

    WrapperFmStepperMotor*      mp_MotorRV;
    WrapperFmTempControl*       mp_TempRV;
    WrapperFmPressureControl*   mp_PressPump;

    WrapperFmTempControl*       mp_TempLSensor;

    WrapperFmDigitalInput*      mp_DIRemoteAlarm;
    WrapperFmDigitalInput*      mp_DILocalAlarm;

    WrapperFmDigitalOutput*      mp_DORemoteAlarm;
    WrapperFmDigitalOutput*      mp_DOLocalAlarm;

    WrapperFmBaseModule*        mp_BaseModule3;
    WrapperFmBaseModule*        mp_BaseModule5;
    WrapperFmBaseModule*        mp_BaseModule15;

};

} // end namespace Diagnostics

#endif // SERVICE_DEVICE_PROCESS_H
