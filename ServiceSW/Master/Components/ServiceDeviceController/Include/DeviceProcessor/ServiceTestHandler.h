/****************************************************************************/
/*! \file ServiceTestHandler.h
 *
 *  \brief Definition file for class ServiceTestHandler.
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

#ifndef SERVICE_TEST_HANDLER_H
#define SERVICE_TEST_HANDLER_H


#include <Global/Include/GlobalDefines.h>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Core/Include/ServiceDefines.h"
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmDigitalInput.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmDigitalOutput.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmBaseModule.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmBootLoader.h>
#include "DeviceControl/Include/Interface/IDeviceProcessing.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"

using namespace DeviceControl;

class WrapperUtils;
class WrapperFmTempControl;
class WrapperFmStepperMotor;
class WrapperFmPressureControl;


namespace DeviceControl {

class DeviceControl::IDeviceProcessing;

typedef enum {
    RETURN_OK = 1,
    RETURN_ERR_NULL_POINTER = -1,
    RETURN_ERR_FAIL = -2
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

class ServiceTestHandler : public QObject
{
    Q_OBJECT
private:
    static ServiceTestHandler* mp_Instance;
    ServiceTestHandler(DeviceControl::IDeviceProcessing &iDevProc);

public:
    static ServiceTestHandler* NewInstance(DeviceControl::IDeviceProcessing &iDevProc);
    static ServiceTestHandler* Instance();
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

    void HandleRequest(QString ReqName, QStringList Params);
private:
    /****************************************************************************/
    /**
     * \brief Helper function to create wrappers
     */
    /****************************************************************************/
    void CreateWrappers();

    void MainRelaySetOnOff(QString& ReqName, QStringList& Params);
    void AlarmSetOnOff(QString& ReqName, QStringList& Params);
    void AlarmGetState(QString& ReqName, QStringList& Params);
    void MainControlGetCurrent(QString& ReqName, QStringList& Params);
    void MainControlGetVoltage(QString& ReqName, QStringList& Params);

    void OvenStartHeating(QString& ReqName, QStringList& Params);
    void OvenStopHeating(QString& ReqName, QStringList& Params);
    void OvenGetTemp(QString& ReqName, QStringList& Params);
    void OvenGetCurrent(QString& ReqName, QStringList& Params);
    void OvenGetCoverSensorState(QString& ReqName, QStringList& Params);
    void OvenGetSwithType(QString& ReqName, QStringList& Params);
    void OvenTempControlIsOn(QString& ReqName, QStringList& Params);

    void RetortStartHeating(QString& ReqName, QStringList& Params);
    void RetortStopHeating(QString& ReqName, QStringList& Params);
    void RetortGetTemp(QString& ReqName, QStringList& Params);
    void RetortGetCurrent(QString& ReqName, QStringList& Params);
    void RetortGetLidLockState(QString& ReqName, QStringList& Params);
    void RetortSetTemperatureSwitchState(QString& ReqName, QStringList& Params);
    void RetortGetHeaterSwitchType(QString& ReqName, QStringList& Params);
    void RetortTempControlIsOn(QString& ReqName, QStringList& Params);


    void LiquidTubeStartHeating(QString& ReqName, QStringList& Params);
    void LiquidTubeStopHeating(QString& ReqName, QStringList& Params);
    void LiquidTubeGetTemp(QString& ReqName, QStringList& Params);
    void LiquidTubeGetCurrent(QString& ReqName, QStringList& Params);
    void LiquidTubeTempControlIsOn(QString& ReqName, QStringList& Params);

    void AirTubeStartHeating(QString& ReqName, QStringList& Params);
    void AirTubeStopHeating(QString& ReqName, QStringList& Params);
    void AirTubeGetTemp(QString& ReqName, QStringList& Params);
    void AirTubeGetCurrent(QString& ReqName, QStringList& Params);
    void AirTubeTempControlIsOn(QString& ReqName, QStringList& Params);


    void RVStartHeating(QString& ReqName, QStringList& Params);
    void RVStopHeating(QString& ReqName, QStringList& Params);
    void RVGetTemp(QString& ReqName, QStringList& Params);
    void RVGetCurrent(QString& ReqName, QStringList& Params);
    void RVInitialize(QString& ReqName, QStringList& Params);
    void RVMovePosition(QString& ReqName, QStringList& Params);
    void RVSetTemperatureSwitchState(QString& ReqName, QStringList& Params);
    void RVGetHeaterSwitchType(QString& ReqName, QStringList& Params);
    void RVTempControlIsOn(QString& ReqName, QStringList& Params);

    void LSStartHeating(QString& ReqName, QStringList& Params);
    void LSStopHeating(QString& ReqName, QStringList& Params);
    void LSGetTemp(QString& ReqName, QStringList& Params);
    void LSGetCurrent(QString& ReqName, QStringList& Params);
    void LSHeatingLevelSensor(QString& ReqName, QStringList& Params);
    void LSTempControlIsOn(QString& ReqName, QStringList& Params);

    void PumpBuildPressure(QString& ReqName, QStringList& Params);
    void PumpReleasePressure(QString& ReqName, QStringList& Params);
    void PumpSetPressure(QString& ReqName, QStringList& Params);
    void PumpGetPressure(QString& ReqName, QStringList& Params);
    void PumpSetFan(QString& ReqName, QStringList& Params);
    void PumpSetValve(QString& ReqName, QStringList& Params);
    void PumpStopCompressor(QString& ReqName, QStringList& Params);
    void PumpSucking(QString& ReqName, QStringList& Params);
    void PumpDraining(QString& ReqName, QStringList& Params);
    void PumpReadPressureDrift(QString& ReqName, QStringList& Params);
    void PumpWritePressureDrift(QString& ReqName, QStringList& Params);

    void GetSlaveModuleReportError(QString& ReqName, QStringList& Params);
    void Pause(quint32 MillSeconds);

private:
    DeviceControl::IDeviceProcessing           &m_rIdevProc;           //!< IDeviceProcessing reference to interact with device
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

signals:
    void ReturnServiceRequestResult(QString ReqName, int ErrorCode, QStringList Results);


};

} // end namespace DeviceControl

#endif // SERVICE_TEST_HANDLER_H
