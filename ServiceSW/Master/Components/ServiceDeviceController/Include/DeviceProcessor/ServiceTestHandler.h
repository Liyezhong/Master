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

//class DeviceControl::IDeviceProcessing;

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
    static ServiceTestHandler* mp_Instance;  ///< The one and only instance.

    /****************************************************************************/
    /**
     * \brief Default constructor.
     *  \iparam iDevProc = reference of IDeviceProcessing
     */
    /****************************************************************************/
    ServiceTestHandler(DeviceControl::IDeviceProcessing &iDevProc);

public:
    /****************************************************************************/
    /**
     *  \brief Get pointer to instance.
     *  \iparam iDevProc = reference of IDeviceProcessing
     *  \return  pointer to instance.
     */
    /****************************************************************************/
    static ServiceTestHandler* NewInstance(DeviceControl::IDeviceProcessing &iDevProc);

    /****************************************************************************/
    /**
     * \brief Get reference to instance.
     * \return  Reference to instance.
     */
    /****************************************************************************/
    static ServiceTestHandler* Instance();

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
     *  \brief To handle request
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void HandleRequest(QString ReqName, QStringList Params);
private:
    /****************************************************************************/
    /**
     * \brief Helper function to create wrappers
     */
    /****************************************************************************/
    void CreateWrappers();

    /****************************************************************************/
    /**
     *  \brief To set main relay state
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void MainRelaySetOnOff(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To set alarm state
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void AlarmSetOnOff(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get alarm state
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void AlarmGetState(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get main control current
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void MainControlGetCurrent(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get main control voltage
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void MainControlGetVoltage(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To start oven heating
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void OvenStartHeating(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To stop oven heating
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void OvenStopHeating(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get oven temperature
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void OvenGetTemp(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get oven current
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void OvenGetCurrent(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get oven cover sensor state
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void OvenGetCoverSensorState(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get oven switch type
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void OvenGetSwithType(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get oven temperature control state
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void OvenTempControlIsOn(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To start retort heating
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void RetortStartHeating(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To stop retort heating
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void RetortStopHeating(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get retort temperature
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void RetortGetTemp(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get retort current
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void RetortGetCurrent(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get retort lid lock
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void RetortGetLidLockState(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To set retort temperature switch state
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void RetortSetTemperatureSwitchState(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get retort heater switch type
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void RetortGetHeaterSwitchType(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get retort temperature control state
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void RetortTempControlIsOn(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief start liquid tube heating
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void LiquidTubeStartHeating(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To stop liquid tube heating
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void LiquidTubeStopHeating(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get liquid tube temperature
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void LiquidTubeGetTemp(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get liquid tube current
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void LiquidTubeGetCurrent(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get liquid tube temperature control state
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void LiquidTubeTempControlIsOn(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To start air tube heating
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void AirTubeStartHeating(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To stop air tube heating
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void AirTubeStopHeating(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get air tube temperature
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void AirTubeGetTemp(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get air tube current
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void AirTubeGetCurrent(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get air tube temperature control state
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void AirTubeTempControlIsOn(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To start rotary valve heating
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void RVStartHeating(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To stop rotary valve heating
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void RVStopHeating(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get rotary valve temperature
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void RVGetTemp(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get rotary valve current
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void RVGetCurrent(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To initialize rotary valve position
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void RVInitialize(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To move rotary valve position
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void RVMovePosition(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get rotary valve position
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void RVGetPosition(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To set rotary valve temperature switch state
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void RVSetTemperatureSwitchState(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get rotary valve heater switch type
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void RVGetHeaterSwitchType(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get rotary valve temperature control state
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void RVTempControlIsOn(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To start level sensor heating
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void LSStartHeating(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To stop level sensor heating
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void LSStopHeating(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get level sensor temperature
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void LSGetTemp(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get level sensor current
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void LSGetCurrent(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To heating level sensor
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void LSHeatingLevelSensor(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get level sensor temperature control state
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void LSTempControlIsOn(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To build pressure
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void PumpBuildPressure(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To release pressrue
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void PumpReleasePressure(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To set pressure
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void PumpSetPressure(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get pressure
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void PumpGetPressure(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To set fan state
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void PumpSetFan(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To set valve state
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void PumpSetValve(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To set stop compressor
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void PumpStopCompressor(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get pump status
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void PumpGetStatus(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To start sucking
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void PumpSucking(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To start draining
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void PumpDraining(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To read pressure drift
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void PumpReadPressureDrift(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To write pressure drift
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void PumpWritePressureDrift(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To get slave module report error
     *  \iparam ReqName = the name of request
     *  \iparam Params = the list of param
     */
    /****************************************************************************/
    void GetSlaveModuleReportError(QString& ReqName, QStringList& Params);

    /****************************************************************************/
    /**
     *  \brief To pause process
     *  \iparam MillSeconds = the mill seconds to wait
     */
    /****************************************************************************/
    void Pause(quint32 MillSeconds);

private:
    DeviceControl::IDeviceProcessing           &m_rIdevProc;           //!< IDeviceProcessing reference to interact with device
    bool                        m_IsConfigured;         //!< Indicates if configuration is successfully completed

    WrapperUtils*               mp_Utils;               //!< Wrapper of Utils
    // Temperature control modules
    WrapperFmTempControl*       mp_TempOvenTop;         //!< Wrapper of oven top
    WrapperFmTempControl*       mp_TempOvenBottom;      //!< Wrapper of oven bottom

    WrapperFmTempControl*       mp_TempRetortSide;      //!< Wrapper of retort side
    WrapperFmTempControl*       mp_TempRetortBottom;    //!< Wrapper of retort bottom

    WrapperFmDigitalInput*      mp_DIRetortLid;         //!< Wrapper of retort lid

    WrapperFmDigitalInput*      mp_DIOven;              //!< Wrapper of oven
    WrapperFmDigitalOutput*     mp_DOMainRelay;         //!< Wrapper of main relay

    WrapperFmTempControl*       mp_TempTubeLiquid;      //!< Wrapper of liquid tube
    WrapperFmTempControl*       mp_TempTubeAir;         //!< Wrapper of airt tube

    WrapperFmStepperMotor*      mp_MotorRV;             //!< Wrapper of motor rotary valve
    WrapperFmTempControl*       mp_TempRV;              //!< Wrapper of rotary valve temperature
    WrapperFmPressureControl*   mp_PressPump;           //!< Wrapper of pressure

    WrapperFmTempControl*       mp_TempLSensor;         //!< Wrapper of level sensor

    WrapperFmDigitalInput*      mp_DIRemoteAlarm;       //!< Wrapper of remote alarm
    WrapperFmDigitalInput*      mp_DILocalAlarm;        //!< Wrapper of local alarm

    WrapperFmDigitalOutput*      mp_DORemoteAlarm;      //!< Wrapper of remote alarm
    WrapperFmDigitalOutput*      mp_DOLocalAlarm;       //!< Wrapper of local alarm

    WrapperFmBaseModule*        mp_BaseModule3;         //!< Wrapper of asb3 module
    WrapperFmBaseModule*        mp_BaseModule5;         //!< Wrapper of asb5 module
    WrapperFmBaseModule*        mp_BaseModule15;        //!< Wrapper of asb15 module

    bool m_OvenTempControlIsOn;                         //!< Flag of oven temperature control state
    bool m_RetortTempControlIsOn;                       //!< Flag of retort temperature control state
    bool m_RVTempControlIsOn;                           //!< Flag of rotary valve temperature control state
    bool m_ATubeTempControlIsOn;                        //!< Flag of air tube temperature control state
    bool m_LTubeTempControlIsOn;                        //!< Flag of liquid tube temperature control state
    bool m_LSTempControlIsOn;                           //!< Flag of level sensor temperature control state
    bool m_RVTubeFlag;                                  //!< Flag of Rotary valve position state
    quint8 m_RVCurrentPosition;                         //!< store rotary valve current position

signals:
    /****************************************************************************/
    /**
     *  \brief signal for return service request result
     *  \iparam ReqName = name of request
     *  \iparam ErroCode = code of error
     *  \iparam Results = list of result
     */
    /****************************************************************************/
    void ReturnServiceRequestResult(QString ReqName, int ErrorCode, QStringList Results);


};

} // end namespace DeviceControl

#endif // SERVICE_TEST_HANDLER_H
