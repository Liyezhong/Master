/****************************************************************************/
/*! \file MockIDeviceProcessing.h
 *
 *  \brief google-mock class for IDeviceProcessing Interface 
 *
 *   Version: $ 0.1
 *   Date:    $ 2014-02-17
 *   Author:  $ Songtao Yu
 *
 *
 *  \b Description:
 *
 *       This module contains the Mock interface to the IDeviceProcessing class
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2014 by Leica Microystems  Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef MOCK_IDEVICEPROCESSING_H
#define MOCK_IDEVICEPROCESSING_H

#include <QtGlobal>
#include<QDateTime>
#include "gmock/gmock.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "DataManager/Containers/InstrumentHistory/Include/Module.h"
namespace IDeviceProcessing
{
static int GetSerialNumber(QString& serNo)
{
    serNo = "1.1";
	return 1;
}
}
namespace DeviceControl
{

class FackIDeviceProcessing : public QObject
{
    Q_OBJECT
signals:
    //! Forward the 'intitialisation finished' notification
    void ReportInitializationFinished(quint32, ReturnCode_t);
    //! Forward the 'configuration finished' notification
    void ReportConfigurationFinished(quint32, ReturnCode_t);
    //! Forward the 'normal operation mode started' notification
    void ReportStartNormalOperationMode(quint32, ReturnCode_t);
    //! Forward error information
    void ReportError(quint32 instanceID, quint16 usErrorGroup, quint16 usErrorID,
                     quint16 usErrorData, QDateTime timeStamp);
    //! Forward error information
    void ReportErrorWithInfo(quint32 instanceID, quint16 usErrorGroup, quint16 usErrorID,
                             quint16 usErrorData, QDateTime timeStamp, QString strErrorInfo);
    //! Forward the 'Destroy finished' to IDeviceProcessing
    void ReportDestroyFinished();
	void ReportLevelSensorStatus1();
	void ReportFillingTimeOut2Min();
	void ReportDrainingTimeOut2Min();
	void ReportGetServiceInfo(ReturnCode_t, const DataManager::CModule&, const QString&);
};

class MockIDeviceProcessing : public FackIDeviceProcessing
{
public:
    void InitializationFinished()
	{
        emit ReportInitializationFinished(DEVICE_INSTANCE_ID_DEVPROC, DCL_ERR_FCT_CALL_SUCCESS);
	}

    void ConfigurationFinished()
	{
        emit ReportConfigurationFinished(DEVICE_INSTANCE_ID_DEVPROC, DCL_ERR_FCT_CALL_SUCCESS);
	}

public:
   //! Start device control layer configuration
    MOCK_METHOD0(StartConfigurationService, ReturnCode_t());

    //! Restart device control layer configuration
    MOCK_METHOD0(RestartConfigurationService, ReturnCode_t());

    //! Start device control layer diagnstic service
    MOCK_METHOD0(StartDiagnosticService, ReturnCode_t());

    //! Finisch device control layer diagnostic service
    MOCK_METHOD0(CloseDiagnosticService, ReturnCode_t());

    //! Start adjustment service
    MOCK_METHOD0(StartAdjustmentService, ReturnCode_t());

    //Air liquid device funcs
    MOCK_METHOD0(ALSetPressureCtrlON, ReturnCode_t());

    MOCK_METHOD0(ALSetPressureCtrlOFF, ReturnCode_t());

    MOCK_METHOD0(ALReleasePressure, ReturnCode_t());

    MOCK_METHOD1(ALPressure, ReturnCode_t(float targetPressure));
    MOCK_METHOD1(ALVaccum, ReturnCode_t(float targetPressure));

    MOCK_METHOD3(ALDraining, ReturnCode_t(quint32 DelayTime, float targetPressure, bool IgnorePressure));
    MOCK_METHOD3(IDForceDraining, ReturnCode_t(quint32 RVPos, float targetPressure, const QString&));
    MOCK_METHOD1(ALStopCmdExec, ReturnCode_t(quint8 CmdType));

    MOCK_METHOD2(ALFilling, ReturnCode_t(quint32 DelayTime, bool EnableInsufficientCheck));

    MOCK_METHOD0(ALGetRecentPressure, qreal());

    MOCK_METHOD1(ALSetTempCtrlON, ReturnCode_t(ALTempCtrlType_t Type));

    MOCK_METHOD5(ALSetTemperaturePid, ReturnCode_t(ALTempCtrlType_t Type, quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime));

    MOCK_METHOD3(ALStartTemperatureControl, ReturnCode_t(ALTempCtrlType_t Type, qreal NominalTemperature, quint8 SlopeTempChange));

    MOCK_METHOD2(ALGetRecentTemperature, qreal(ALTempCtrlType_t Type, quint8 Index));

    MOCK_METHOD1(ALGetTemperatureControlState, TempCtrlState_t(ALTempCtrlType_t Type));

    MOCK_METHOD0(ALTurnOnFan, ReturnCode_t());
    MOCK_METHOD0(ALTurnOffFan, ReturnCode_t());
    MOCK_METHOD0(ALAllStop, ReturnCode_t());
    MOCK_METHOD0(ALBreakAllOperation, ReturnCode_t());
    MOCK_METHOD1(ALSetPressureDrift, ReturnCode_t(qreal pressureDrift));
    MOCK_METHOD1(IDSealingCheck, ReturnCode_t(qreal ThresholdPressure));

    MOCK_METHOD7(ALStartTemperatureControlWithPID, ReturnCode_t(ALTempCtrlType_t Type, qreal NominalTemperature, quint8 SlopeTempChange, \
                quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime));

    //Rotary Valve device func
    MOCK_METHOD0(RVSetTempCtrlON, ReturnCode_t());

    MOCK_METHOD0(RVSetTempCtrlOFF, ReturnCode_t());

    MOCK_METHOD4(RVSetTemperaturePid, ReturnCode_t(quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime));

    MOCK_METHOD2(RVStartTemperatureControl, ReturnCode_t(qreal NominalTemperature, quint8 SlopeTempChange));

    MOCK_METHOD1(RVGetRecentTemperature, qreal(quint32 Index));

    MOCK_METHOD0(RVGetTemperatureControlState, TempCtrlState_t());

    //! Execute the move to intial position of the RV
    MOCK_METHOD1(RVReqMoveToInitialPosition, ReturnCode_t(RVPosition_t RVPosition));

    //! Position the oven cover
    MOCK_METHOD1(RVReqMoveToRVPosition, ReturnCode_t(RVPosition_t RVPosition));

    //! Request actual oven cover position
    MOCK_METHOD0(RVReqActRVPosition, RVPosition_t());

    MOCK_METHOD6(RVStartTemperatureControlWithPID, ReturnCode_t(qreal NominalTemperature, quint8 SlopeTempChange, quint16 MaxTemperature, \
                quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime));

    //Oven device func
    MOCK_METHOD1(OvenSetTempCtrlON, ReturnCode_t(OVENTempCtrlType_t Type));

    MOCK_METHOD5(OvenSetTemperaturePid, ReturnCode_t(OVENTempCtrlType_t Type, quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime));
    MOCK_METHOD3(OvenStartTemperatureControl, ReturnCode_t(OVENTempCtrlType_t Type, qreal NominalTemperature, quint8 SlopeTempChange));
    MOCK_METHOD2(OvenGetRecentTemperature, qreal(OVENTempCtrlType_t Type, quint8 Index));

    MOCK_METHOD0(OvenGetRecentLidStatus, quint16());

    MOCK_METHOD1(OvenGetTemperatureControlState, TempCtrlState_t(OVENTempCtrlType_t Type));

    MOCK_METHOD7(OvenStartTemperatureControlWithPID, ReturnCode_t(OVENTempCtrlType_t Type, qreal NominalTemperature, quint8 SlopeTempChange, \
                quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime));

    //Retort device func
    MOCK_METHOD1(RTSetTempCtrlON, ReturnCode_t(RTTempCtrlType_t Type));

    MOCK_METHOD1(RTSetTempCtrlOFF, ReturnCode_t(RTTempCtrlType_t Type));

    MOCK_METHOD5(RTSetTemperaturePid, ReturnCode_t(RTTempCtrlType_t Type, quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime));

    MOCK_METHOD3(RTStartTemperatureControl, ReturnCode_t(RTTempCtrlType_t Type, qreal NominalTemperature, quint8 SlopeTempChange));

    MOCK_METHOD7(RTStartTemperatureControlWithPID, ReturnCode_t(RTTempCtrlType_t Type, qreal NominalTemperature, quint8 SlopeTempChange, \
                quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime));
    MOCK_METHOD2(RTGetRecentTemperature, qreal(RTTempCtrlType_t Type, quint8 Index));

    MOCK_METHOD1(RTGetTemperatureControlState, TempCtrlState_t(RTTempCtrlType_t Type));

    MOCK_METHOD0(RTUnlock, ReturnCode_t());
    MOCK_METHOD0(RTLock, ReturnCode_t());
    MOCK_METHOD0(RTGetRecentLockStatus, quint16());

    //Periphery device func
    MOCK_METHOD0(PerTurnOffMainRelay, ReturnCode_t());
    MOCK_METHOD0(PerTurnOnMainRelay, ReturnCode_t());

    MOCK_METHOD2(IDBottleCheck, ReturnCode_t(QString ReagentGrpID, RVPosition_t TubePos));
    MOCK_METHOD2(GetSensorCurrent, quint16(const QString& DevName, quint8 Index));
    MOCK_METHOD1(IDGetSlaveVoltage, quint16(HimSlaveType_t Type));
    MOCK_METHOD1(IDGetSlaveCurrent, quint16(HimSlaveType_t Type));
    MOCK_METHOD1(OvenGetHeatingStatus, bool(OVENTempCtrlType_t Type));
    MOCK_METHOD1(GetHeaterSwitchType, quint8(const QString& DevName));
    MOCK_METHOD3(GetSlaveModuleReportError, ReportError_t(quint8 errorCode, const QString& devName, quint32 sensorName));
    MOCK_METHOD1(OvenSetTempCtrlOFF, ReturnCode_t(OVENTempCtrlType_t type));
    MOCK_METHOD1(ALSetTempCtrlOFF, ReturnCode_t(ALTempCtrlType_t type));
    MOCK_METHOD0(Destroy, void());
    MOCK_METHOD1(NotifySavedServiceInfor, void(const QString& deviceType));
    MOCK_METHOD0(ResetActiveCarbonFilterLifeTime, void());
    MOCK_METHOD1(IDSetAlarm, ReturnCode_t(int opcode));
    MOCK_METHOD2(ALControlValve, ReturnCode_t(quint8 ValveIndex, quint8 ValveState));
    MOCK_METHOD2(RVSetTemperatureSwitchState, ReturnCode_t(qint8 HeaterVoltage, qint8 AutoType));
    MOCK_METHOD3(RTSetTemperatureSwitchState, ReturnCode_t(RTTempCtrlType_t Type, qint8 HeaterVoltage, qint8 AutoType));
    MOCK_METHOD1(PerGetRecentAlarmStatus, quint16( qint8 type));
};

} //namespace

#endif /* MOCK_IDEVICEPROCESSING_H */
