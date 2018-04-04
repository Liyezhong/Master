// ================================================================================================
/*! @file Himalaya/HimalayaMain/Master/Components/Scheduler/Include/SchedulerCommandProcessor.h
 *
 *  @brief
 *  Definition of class Scheduler::SchedulerCommandProcessor.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-03-10
 *  $Author:    $ L. Yang
 *
 *  @b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2013 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 * ================================================================================================
*/



#ifndef SCHEDULERCOMMANDPROCESSOR_H
#define SCHEDULERCOMMANDPROCESSOR_H

#include <QObject>
#include <QQueue>
#include <QMutex>

#include "Global/Include/Commands/Command.h"
#include "DeviceControl/Include/Interface/IDeviceProcessing.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "DeviceControl/Include/Devices/RotaryValveDevice.h"
#include "DeviceControl/Include/Devices/AirLiquidDevice.h"
#include "DeviceControl/Include/Devices/RetortDevice.h"
#include "DeviceControl/Include/Devices/OvenDevice.h"
#include "DeviceControl/Include/Devices/PeripheryDevice.h"
#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"


using namespace DeviceControl;
//{
//    class IDeviceProcessing;
//}

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of struct HardwareMonitor_t
 */
/****************************************************************************/
typedef struct
{
    qreal			TempRTBottom1;      ///<  Definition/Declaration of variable TempRTBottom1
    qreal			TempRTBottom2;      ///<  Definition/Declaration of variable TempRTBottom2
    qreal			TempOvenBottom1;    ///<  Definition/Declaration of variable TempOvenBottom1
    qreal			TempOvenBottom2;    ///<  Definition/Declaration of variable TempOvenBottom2
	qreal			PressureAL;       ///<  Definition/Declaration of variable PressureAL
	qreal			TempALLevelSensor;       ///<  Definition/Declaration of variable TempALLevelSensor
    bool            OvenHeatingStatus;      ///< true -- heating, false, not heating.
	qreal			TempALTube1;       ///<  Definition/Declaration of variable TempALTube1
	qreal			TempALTube2;       ///<  Definition/Declaration of variable TempALTube2
	qreal			TempRV1;       ///<  Definition/Declaration of variable TempRV1
	qreal			TempRV2;       ///<  Definition/Declaration of variable TempRV2
	RVPosition_t	PositionRV;       ///<  Definition/Declaration of variable PositionRV
    quint32         LowerLimit;         ///< Definition/Declaration of variable LowerLimit
	qreal			TempRTSide;       ///<  Definition/Declaration of variable TempRTSide
	qreal			TempOvenTop;       ///<  Definition/Declaration of variable TempOvenTop
	quint16			OvenLidStatus;       ///<  Definition/Declaration of variable OvenLidStatus
	quint16			RetortLockStatus;       ///<  Definition/Declaration of variable RetortLockStatus
    quint16         CurrentHeatRetortOven;          ///< Current for heating RT and OVEN
    quint16         CurrentHeatLevelSensorTubes;   ///< Current for heating Level sensor and tubes
    quint16         CurrentRVTemp;          ///< Current of Rotary Valve temperautre module
    quint16         Slave3Voltage;          ///< Slave3 Voltage
    quint16         Slave5Voltage;          ///< Slave5 Voltage
    quint16         Slave15Voltage;         ///< Slave15 Voltage
    quint16         Slave3Current;          ///< Slave3 Current
    quint16         Slave5Current;          ///< Slave5 Current
    quint16         Slave15Current;         ///< Slave15 Current
    quint16         LocalAlarmStatus;       ///< Local alarm status
    quint16         RemoteAlarmStatus;      ///< Remote alarm status
    bool            LATube1HeatingStatus;   ///< LA tube1 heating status
    bool            LATube2HeatingStatus;   ///< LA tube2 heating status

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function toLogString
     *
     *  \return from toLogString
     */
    /****************************************************************************/
//    QString toLogString(){
//        return QString("RTb1(%1)RTb2(%2)RTs(%3)RTl(%4)")
//                .arg(TempRTBottom1).arg(TempRTBottom2).arg(TempRTSide).arg(RetortLockStatus) +
//                QString("OVb1(%1)OVb2(%2)OVt(%3)OVl(%4)CurrentHeatRTOV(%5)")
//                .arg(TempOvenBottom1).arg(TempOvenBottom2).arg(TempOvenTop).arg(OvenLidStatus).arg(CurrentHeatRetortOven) +
//                QString("RV1(%1)RV2(%2)RVp(%3)CurrentHeatRV(%4)")
//                .arg(TempRV1).arg(TempRV2).arg(PositionRV).arg(CurrentRVTemp) +
//                QString("ALp(%1)ALls(%2)ALt1(%3)ALt2(%4)CurrentHeatLSTB(%5)")
//                .arg(PressureAL).arg(TempALLevelSensor).arg(TempALTube1).arg(TempALTube2).arg(CurrentHeatLevelSensorTubes) +
//                QString("ASB3Current(%1)ASB5Curent(%2)ASB15Current(%3)")
//                .arg(Slave3Current).arg(Slave5Current).arg(Slave15Current);
//    }

    QString toLogString(){

    // TempRV1 TempRV2 TempOvTp TempOvB1 TempOvB2 TempRtB1 TempRtB2 TempRtSd TempLS TempTb1 TempTb2 Pressure CurRV CurRtOv CurLT Cur3 Cur5 Cur15 RvPos RtLock OvLid

        return  QString("%1%2")          // TempRV1 TempRV2
                .arg(TempRV1,9,'g',5).arg(TempRV2,9,'g',5) +
                QString("%1%2%3%4%5%6")  // TempOvTp TempOvB1 TempOvB2 TempRtB1 TempRtB2 TempRtSd
                .arg(TempOvenTop,9,'g',5).arg(TempOvenBottom1,9,'g',5).arg(TempOvenBottom2,9,'g',5).arg(TempRTBottom1,9,'g',5).arg(TempRTBottom2,9,'g',5).arg(TempRTSide,9,'g',5) +
                QString("%1%2%3")        // TempLS TempTb1 TempTb2
                .arg(TempALLevelSensor,9,'g',5).arg(TempALTube1,9,'g',5).arg(TempALTube2,9,'g',5) +
                QString("%1")            // Pressure
                .arg(PressureAL,9,'g',5) +
                QString("%1%2%3%4%5%6")  // CurRV CurRtOv CurLT Cur3 Cur5 Cur15
                .arg(CurrentRVTemp,6).arg(CurrentHeatRetortOven,6).arg(CurrentHeatLevelSensorTubes,6).arg(Slave3Current,6).arg(Slave5Current,6).arg(Slave15Current,6) +
                QString("%1%2%3")        // RvPos RtLock OvLid
                .arg(PositionRV,6).arg(RetortLockStatus,2).arg(OvenLidStatus,2);
    }

} HardwareMonitor_t;

class SchedulerMainThreadController;

/****************************************************************************/
/**
 * @brief command processing thread class for the main scheduler thread.
 *
 */
/****************************************************************************/
class SchedulerCommandProcessorBase : public QObject
{
    Q_OBJECT
public:
    /****************************************************************************/
    /**
     * @brief constructor.
     *
     */
    /****************************************************************************/
    SchedulerCommandProcessorBase():m_TickTimer(this)
    {

    }
    virtual ~SchedulerCommandProcessorBase(){}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function HardwareMonitor
	 *
	 *  \return from HardwareMonitor
	 */
	/****************************************************************************/
    virtual HardwareMonitor_t HardwareMonitor(const QString& ) = 0;
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function ALBreakAllOperation
	 *
	 *  \return from ALBreakAllOperation
	 */
	/****************************************************************************/
	virtual DeviceControl::ReturnCode_t ALBreakAllOperation() = 0;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ALGetRecentPressure
     *
     *  \return from ALGetRecentPressure
     */
    /****************************************************************************/
    virtual qreal ALGetRecentPressure() = 0;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ALSetPressureDrift
     *
     *  \param pressureDrift = qreal type parameter
     *
     *  \return from ALSetPressureDrift
     */
    /****************************************************************************/
    virtual void ALSetPressureDrift(qreal pressureDrift) = 0;

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ShutDownDevice
     *
     *  \return from void
     */
    /****************************************************************************/
    virtual void ShutDownDevice() = 0;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifySavedServiceInfor
     *
     *  \param deviceType =  device type
     *
     *  \return from void
     */
    /****************************************************************************/
    virtual void NotifySavedServiceInfor(const QString& deviceType) = 0;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ResetActiveCarbonFilterLifetime
     *  \param  setVal -- lifetime for carbon filter
     *
     */
    /****************************************************************************/
    virtual void ResetActiveCarbonFilterLifetime(quint32 setVal) = 0;
    /****************************************************************************/
    /*!
     *  \brief  Get report error from salve module
     *  \param  CANErrCode - quint8
     *  \param  devName - device name
     *  \param  sensorName - quint32 sensor name
     *
     *  \return ReportError_t
     */
    /****************************************************************************/
    virtual ReportError_t GetSlaveModuleReportError(quint8 CANErrCode, const QString& devName, quint32 sensorName=0) = 0;

    /****************************************************************************/
    /*!
     *  \brief  Get heater switch type from salve module
     *
     *  \param  devName - QString device name
     *
     *  \return quint8
     */
    /****************************************************************************/
    virtual quint8 GetHeaterSwitchType(const QString& devName) = 0;

signals:
    /****************************************************************************/
    /**
     * @brief emit the signal when command be finished!
     *  \param  cmd
     *  \param  result
     */
    /****************************************************************************/
    void onCmdFinished(Global::CommandShPtr_t *cmd, bool result);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal DCLConfigurationFinished
     *  \param  RetCode
     */
    /****************************************************************************/
    void DCLConfigurationFinished(ReturnCode_t RetCode);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SigShutDownDevice
     */
    /****************************************************************************/
    void SigShutDownDevice();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SigNotifySavedServiceInfor
     *  \param  deviceType - QString
     */
    /****************************************************************************/
    void SigNotifySavedServiceInfor(const QString& deviceType);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SigResetActiveCarbonFilterLifetime
     *  \param  setVal -- lifetime for carbon filter
     */
    /****************************************************************************/
    void SigResetActiveCarbonFilterLifetime(quint32 setVal);

    /************************************************************************************/
    /*!
     *  \brief  Forward AirLiquid's 'level sensor status change to 1' to Heating strategy
     */
    /************************************************************************************/
    void ReportLevelSensorStatus1();

    /************************************************************************************/
    /*!
     *  \brief  Forward AirLiquid Filling Warning DCL_ERR_DEV_LA_FILLING_TIMEOUT_2MIN
     *          to SchedulerMainThreadController
     */
    /************************************************************************************/
    void ReportFillingTimeOut2Min();

    /************************************************************************************/
    /*!
     *  \brief  Forward AirLiquid Filling Warning DCL_ERR_DEV_LA_DRAINING_TIMEOUT_EMPTY_2MIN
     *          to SchedulerMainThreadController
     */
    /************************************************************************************/
    void ReportDrainingTimeOut2Min();

    /************************************************************************************/
    /*!
     *  \brief  report device process has been shutdown
     */
    /************************************************************************************/
    void DeviceProcessDestroyed();
    /****************************************************************************/
    /*!
     *  \brief  Returns the service information of a device
     *
     *  \param ReturnCode = ReturnCode of Device Control Layer
     *  \param ModuleInfo = Contains the service information
     *  \param deviceType - QString
     */
    /****************************************************************************/
    void ReportGetServiceInfo(ReturnCode_t ReturnCode, const DataManager::CModule &ModuleInfo, const QString& deviceType);

    /****************************************************************************/
    /*!
     *  \brief  Forward error information to IDeviceProcessing
     *
     *  \iparam instanceID = Instance identifier of this function module instance
     *  \iparam usErrorGroup = Error group
     *  \iparam usErrorID = Error ID
     *  \iparam usErrorData = Additional error information
     *  \iparam timeStamp = Error time
     */
    /****************************************************************************/
    void ReportError(quint32 instanceID, quint16 usErrorGroup, quint16 usErrorID, quint16 usErrorData, QDateTime timeStamp);
public slots:

    /****************************************************************************/
    /**
     * @brief thread's callback function
     *
     */
    /****************************************************************************/
    virtual void run() { this->run4Slot(); }

    /****************************************************************************/
    /**
     * @brief controller call this function to add a new command to process.
     *  \param cmd
     *  \param response
     */
    /****************************************************************************/
    virtual void pushCmd(CmdSchedulerCommandBase *cmd, bool response=true) { this->pushCmd4Slot(cmd, response); }
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot DevProcInitialisationAckn
     *  \param  instanceID
     *  \param  configResult
     */
    /****************************************************************************/
    virtual void DevProcInitialisationAckn(quint32 instanceID, ReturnCode_t configResult)
    {
        this->DevProcInitialisationAckn4Slot(instanceID, configResult);
    }
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot DevProcConfigurationAckn
     *  \param  instanceID
     *  \param  hdlInfo
     */
    /****************************************************************************/
    virtual void DevProcConfigurationAckn(quint32 instanceID, ReturnCode_t hdlInfo)
    {
        this->DevProcConfigurationAckn4Slot(instanceID, hdlInfo);
    }
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot DevProcStartNormalOpModeAckn
     *  \param  instanceID
     *  \param  hdlInfo
     */
    /****************************************************************************/
    virtual void DevProcStartNormalOpModeAckn(quint32 instanceID, ReturnCode_t hdlInfo)
    {
        this->DevProcStartNormalOpModeAckn4Slot(instanceID, hdlInfo);
    }
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot ThrowError
     *  \param  instanceID
     *  \param  usErrorGroup
     *  \param  usErrorID
     *  \param  usErrorData
     *  \param  TimeStamp
     */
    /****************************************************************************/
    virtual void ThrowError(quint32 instanceID, quint16 usErrorGroup, quint16 usErrorID, quint16 usErrorData,const QDateTime & TimeStamp)
    {
        this->ThrowError4Slot(instanceID, usErrorGroup, usErrorID, usErrorData, TimeStamp);
    }

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot OnShutDownDevice
     */
    /****************************************************************************/
    virtual void OnShutDownDevice()
    {
        this->OnShutDownDevice4Slot();
    }

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot OnNotifySavedServiceInfor
     *  \param  deviceType - QString
     */
    /****************************************************************************/
    virtual void OnNotifySavedServiceInfor(const QString& deviceType)
    {
        this->OnNotifySavedServiceInfor4Slot(deviceType);
    }

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of slot OnResetActiveCarbonFilterLifetime\
     *  \param  setVal -- lifetime for carbon filter
     */
    /****************************************************************************/
    virtual void OnResetActiveCarbonFilterLifetime(quint32 setVal)
    {
        this->OnResetActiveCarbonFilterLifetime4Slot(setVal);
    }

    /****************************************************************************/
    /**
     *  \brief For advancing the time
     */
    /****************************************************************************/
    virtual void OnTickTimer()
    {
        this->OnTickTimer4Slot();
    }

private:

    /****************************************************************************/
    /**
     * @brief check if there are new command comming.
     *
     */
    /****************************************************************************/
    virtual bool newCmdComing(Scheduler::SchedulerCommandShPtr_t& scmd) =0;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function run4Slot
     *
     *  \return from run4Slot
     */
    /****************************************************************************/
    virtual void run4Slot() = 0;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function pushCmd4Slot
     *
     *  \param cmd = CmdSchedulerCommandBase type parameter
     *  \param response = reponse flag
     *
     *  \return from pushCmd4Slot
     */
    /****************************************************************************/
    virtual void pushCmd4Slot(CmdSchedulerCommandBase *cmd, bool response) = 0;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function DevProcInitialisationAckn4Slot
     *
     *  \param instanceID = quint32 type parameter
     *  \param configResult =  ReturnCode_t type parameter
     *
     *  \return from DevProcInitialisationAckn4Slot
     */
    /****************************************************************************/
    virtual void DevProcInitialisationAckn4Slot(quint32 instanceID, ReturnCode_t configResult) = 0;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function DevProcConfigurationAckn4Slot
     *
     *  \param instanceID = quint32 type parameter
     *  \param hdlInfo =  ReturnCode_t type parameter
     *
     *  \return from DevProcConfigurationAckn4Slot
     */
    /****************************************************************************/
    virtual void DevProcConfigurationAckn4Slot(quint32 instanceID, ReturnCode_t hdlInfo) = 0;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function DevProcStartNormalOpModeAckn4Slot
     *
     *  \param instanceID = quint32 type parameter
     *  \param hdlInfo =  ReturnCode_t type parameter
     *
     *  \return from DevProcStartNormalOpModeAckn4Slot
     */
    /****************************************************************************/
    virtual void DevProcStartNormalOpModeAckn4Slot(quint32 instanceID, ReturnCode_t hdlInfo) = 0;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ThrowError4Slot
     *
     *  \param instanceID = quint32 type parameter
     *  \param usErrorGroup =  quint16 type parameter
     *  \param usErrorID =  quint16 type parameter
     *  \param usErrorData =  quint16 type parameter
     *  \param TimeStamp = const QDateTime type parameter
     *
     *  \return from ThrowError4Slot
     */
    /****************************************************************************/
    virtual void ThrowError4Slot(quint32 instanceID, quint16 usErrorGroup, quint16 usErrorID, quint16 usErrorData,const QDateTime & TimeStamp) = 0;

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function OnTickTimer4Slot
     *
     *  \return from OnTickTimer4Slot
     */
    /****************************************************************************/
    virtual void OnTickTimer4Slot() = 0;

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function OnShutDownDevice4Slot
     *
     *  \return from OnShutDownDevice4Slot
     */
    /****************************************************************************/
    virtual void OnShutDownDevice4Slot()= 0 ;
    virtual void OnNotifySavedServiceInfor4Slot(const QString& deviceType) = 0;
    virtual void OnResetActiveCarbonFilterLifetime4Slot(quint32 setVal) = 0;

    SchedulerCommandProcessorBase(const SchedulerCommandProcessorBase&);              			///< Not implemented.
    const SchedulerCommandProcessorBase& operator=(const SchedulerCommandProcessorBase&);		///< Not implemented.

protected:
    QTimer m_TickTimer;       ///<  Definition/Declaration of variable m_TickTimer
};


/****************************************************************************/
/**
 * @brief command processing thread class for the main scheduler thread.
 *
 */
/****************************************************************************/
template <class DP>
class SchedulerCommandProcessor : public SchedulerCommandProcessorBase 
{
    friend class TestSchedulerCommandProcessor;
public:
    /****************************************************************************/
    /**
     * @brief constructor and destructor.
     * \param controller
     *
     */
    /****************************************************************************/
    explicit SchedulerCommandProcessor(SchedulerMainThreadController* controller);
    ~SchedulerCommandProcessor();
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function HardwareMonitor
	 *
	 *  \return from HardwareMonitor
	 */
	/****************************************************************************/
    virtual HardwareMonitor_t HardwareMonitor(const QString& sender);
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function ALBreakAllOperation
	 *
	 *  \return from ALBreakAllOperation
	 */
	/****************************************************************************/
	virtual DeviceControl::ReturnCode_t ALBreakAllOperation() { return mp_IDeviceProcessing->ALBreakAllOperation(); } 
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ALGetRecentPressure
     *
     *  \return from ALGetRecentPressure
     */
    /****************************************************************************/
    virtual qreal ALGetRecentPressure() { return mp_IDeviceProcessing->ALGetRecentPressure(); }
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ALSetPressureDrift
     *
     *  \param pressureDrift = qreal type parameter
     *
     *  \return from ALSetPressureDrift
     */
    /****************************************************************************/
    virtual void ALSetPressureDrift(qreal pressureDrift) { (void)mp_IDeviceProcessing->ALSetPressureDrift(pressureDrift); }
#ifdef GOOGLE_MOCK
    void SetIDeviceProcessing(DP* IDeviceProcessing) { mp_IDeviceProcessing = IDeviceProcessing; }
#endif

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ShutDownDevice
     */
    /****************************************************************************/
    virtual void ShutDownDevice();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function NotifySavedServiceInfor
     *
     *  \param deviceType = device type
     *
     */
    /****************************************************************************/
    virtual void NotifySavedServiceInfor(const QString& deviceType);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ResetActiveCarbonFilterLifetime
     *  \param  setVal -- lifetime for carbon filter
     *
     */
    /****************************************************************************/
    virtual void ResetActiveCarbonFilterLifetime(quint32 setVal);

    /****************************************************************************/
    /*!
     *  \brief  Get report error from salve module
     *  \param  CANErrCode - quint8
     *  \param  devName - QString sensor name
     *  \param  sensorName - quint32
     *  \return ReportError_t
     */
    /****************************************************************************/
    virtual ReportError_t GetSlaveModuleReportError(quint8 CANErrCode, const QString& devName, quint32 sensorName=0);

    /****************************************************************************/
    /*!
     *  \brief  Get heater switch type from salve module
     *
     *  \param  devName - device name
     *
     *  \return quint8
     */
    /****************************************************************************/
    virtual quint8 GetHeaterSwitchType(const QString& devName) {return mp_IDeviceProcessing->GetHeaterSwitchType(devName); }
private:

    /****************************************************************************/
    /**
     * @brief check if there are new command comming.
     *
     */
    /****************************************************************************/
    virtual bool newCmdComing(Scheduler::SchedulerCommandShPtr_t& scmd);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function run4Slot
     *
     *  \return from run4Slot
     */
    /****************************************************************************/
    virtual void run4Slot();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function pushCmd4Slot
     *
     *  \param cmd = CmdSchedulerCommandBase type parameter
     *  \param response = command response flag
     *
     *  \return from pushCmd4Slot
     */
    /****************************************************************************/
    virtual void pushCmd4Slot(CmdSchedulerCommandBase *cmd, bool response);    
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function DevProcInitialisationAckn4Slot
     *
     *  \param instanceID = quint32 type parameter
     *  \param configResult =  ReturnCode_t type parameter
     *
     *  \return from DevProcInitialisationAckn4Slot
     */
    /****************************************************************************/
    virtual void DevProcInitialisationAckn4Slot(quint32 instanceID, ReturnCode_t configResult);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function DevProcConfigurationAckn4Slot
     *
     *  \param instanceID = quint32 type parameter
     *  \param hdlInfo =  ReturnCode_t type parameter
     *
     *  \return from DevProcConfigurationAckn4Slot
     */
    /****************************************************************************/
    virtual void DevProcConfigurationAckn4Slot(quint32 instanceID, ReturnCode_t hdlInfo);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function DevProcStartNormalOpModeAckn4Slot
     *
     *  \param instanceID = quint32 type parameter
     *  \param hdlInfo =  ReturnCode_t type parameter
     *
     *  \return from DevProcStartNormalOpModeAckn4Slot
     */
    /****************************************************************************/
    virtual void DevProcStartNormalOpModeAckn4Slot(quint32 instanceID, ReturnCode_t hdlInfo);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ThrowError4Slot
     *
     *  \param instanceID = quint32 type parameter
     *  \param usErrorGroup =  quint16 type parameter
     *  \param usErrorID =  quint16 type parameter
     *  \param usErrorData =  quint16 type parameter
     *  \param TimeStamp = const QDateTime type parameter
     *
     *  \return from ThrowError4Slot
     */
    /****************************************************************************/
    virtual void ThrowError4Slot(quint32 instanceID, quint16 usErrorGroup, quint16 usErrorID, quint16 usErrorData,const QDateTime & TimeStamp);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function OnShutDownDevice4Slot
     *
     *  \return from OnShutDownDevice4Slot
     */
    /****************************************************************************/
    virtual void OnShutDownDevice4Slot();
    virtual void OnNotifySavedServiceInfor4Slot(const QString& deviceType);
    virtual void OnResetActiveCarbonFilterLifetime4Slot(quint32 setVal);
    virtual void OnTickTimer4Slot();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function ExecuteCmd
     *
     *  \return from ExecuteCmd
     */
    /****************************************************************************/
    void ExecuteCmd(Scheduler::SchedulerCommandShPtr_t& scmd);
    SchedulerCommandProcessor(const SchedulerCommandProcessor&);                      ///< Not implemented.
    const SchedulerCommandProcessor& operator=(const SchedulerCommandProcessor&);     ///< Not implemented.

private:
    SchedulerMainThreadController *mp_SchedulerThreadController;       ///<  Definition/Declaration of variable mp_SchedulerThreadController
    DP *mp_IDeviceProcessing;       ///<  Definition/Declaration of variable mp_IDeviceProcessing

    QQueue<Scheduler::SchedulerCommandShPtr_t> m_Cmds;       ///<  Definition/Declaration of variable m_Cmds
    QMutex m_CmdMutex;       ///<  Definition/Declaration of variable m_CmdMutex
    QString m_Sender;
};

} // end of namespace Scheduler

#endif // SCHEDULERCOMMANDPROCESSOR_H
