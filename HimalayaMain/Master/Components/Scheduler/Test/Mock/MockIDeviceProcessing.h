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
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2014 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef MOCK_IDEVICEPROCESSING_H
#define MOCK_IDEVICEPROCESSING_H

#include <QtGlobal>
#include "gmock/gmock.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"

namespace DeviceControl 
{

class MockIDeviceProcessing : public QObject
{
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

    MOCK_METHOD0(ALPressure, ReturnCode_t());
    MOCK_METHOD0(ALVaccum, ReturnCode_t());

    MOCK_METHOD1(ALDraining, ReturnCode_t(quint32 DelayTime));

    MOCK_METHOD1(ALFilling, ReturnCode_t(quint32 DelayTime));

    MOCK_METHOD0(ALGetRecentPressure, qreal());

    MOCK_METHOD1(ALSetTempCtrlON, ReturnCode_t(ALTempCtrlType_t Type));

    MOCK_METHOD5(ALSetTemperaturePid, ReturnCode_t(ALTempCtrlType_t Type, quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime));

    MOCK_METHOD3(ALStartTemperatureControl, ReturnCode_t(ALTempCtrlType_t Type, qreal NominalTemperature, quint8 SlopeTempChange));

    MOCK_METHOD2(ALGetRecentTemperature, ReturnCode_t(ALTempCtrlType_t Type, quint8 Index));

    MOCK_METHOD1(ALGetTemperatureControlState, TempCtrlState_t(ALTempCtrlType_t Type));

    MOCK_METHOD0(ALTurnOnFan, ReturnCode_t());
    MOCK_METHOD0(ALTurnOffFan, ReturnCode_t());
    MOCK_METHOD0(ALAllStop, ReturnCode_t());
    MOCK_METHOD0(ALBreakAllOperation, ReturnCode_t());
    MOCK_METHOD1(ALSetPressureDrift, ReturnCode_t(qreal pressureDrift));

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
    MOCK_METHOD0(RVReqMoveToInitialPosition, ReturnCode_t());

    //! Position the oven cover
    MOCK_METHOD1(RVReqMoveToRVPosition, ReturnCode_t(RVPosition_t RVPosition));

    //! Request actual oven cover position
    MOCK_METHOD0(RVReqActRVPosition, RVPosition_t());

    MOCK_METHOD6(RVStartTemperatureControlWithPID, ReturnCode_t(qreal NominalTemperature, quint8 SlopeTempChange, quint16 MaxTemperature, \
                quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime));

    //Oven device func
    MOCK_METHOD1(OvenSetTempCtrlON, ReturnCode_t(OVENTempCtrlType_t Type));

    MOCK_METHOD5(OvenSetTemperaturePid, ReturnCode_t(OVENTempCtrlType_t Type, quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime));

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

	//signals related
	MOCK_METHOD2(ReportInitializationFinished, void(DevInstanceID_t, ReturnCode_t));
	MOCK_METHOD2(ReportConfigurationFinished, void(DevInstanceID_t, ReturnCode_t));
	MOCK_METHOD2(ReportStartNormalOperationMode, void(DevInstanceID_t, ReturnCode_t));
	MOCK_METHOD5(ReportError, void(DevInstanceID_t instanceID, quint16 usErrorGroup, quint16 usErrorID, quint16 usErrorData, QDateTime timeStamp));
    MOCK_METHOD6(ReportErrorWithInfo, void(DevInstanceID_t instanceID, quint16 usErrorGroup, quint16 usErrorID, quint16 usErrorData, QDateTime timeStamp, QString strErrorInfo));
	MOCK_METHOD0(ReportDestroyFinished, void());

};

} //namespace

#endif /* MOCK_IDEVICEPROCESSING_H */
