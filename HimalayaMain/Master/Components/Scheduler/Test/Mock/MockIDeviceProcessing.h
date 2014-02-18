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
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef MOCK_IDEVICEPROCESSING_H
#define MOCK_IDEVICEPROCESSING_H

#include <gmock.h>

namespace DeviceControl
{
class MockIDeviceProcessing 
{
public:
    MockIDeviceProcessing();
    ~MockIDeviceProcessing();


    //! Start device control layer configuration
	MOCK_METHOD0(StartConfigurationService, ReturnCode_t());

    //! Restart device control layer configuration
	MOCK_METHOD0(RestartConfigurationService, ReturnCode_t());

    //! Start device control layer diagnstic service
	MOCK_METHOD0(StartDiagnosticService, ReturnCode_t());
    ReturnCode_t StartDiagnosticService();
    //! Finisch device control layer diagnostic service
    ReturnCode_t CloseDiagnosticService();

    //! Start adjustment service
    ReturnCode_t StartAdjustmentService();

    //Air liquid device funcs
    ReturnCode_t ALSetPressureCtrlON();
    ReturnCode_t ALSetPressureCtrlOFF();
    ReturnCode_t ALReleasePressure(void);
    ReturnCode_t ALPressure();
    ReturnCode_t ALDraining(quint32 DelayTime);
    ReturnCode_t ALFilling(quint32 DelayTime);
    qreal ALGetRecentPressure();
    ReturnCode_t ALSetTempCtrlON(ALTempCtrlType_t Type);
    ReturnCode_t ALSetTemperaturePid(ALTempCtrlType_t Type, quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime);
    ReturnCode_t ALStartTemperatureControl(ALTempCtrlType_t Type, qreal NominalTemperature, quint8 SlopeTempChange);
    qreal ALGetRecentTemperature(ALTempCtrlType_t Type, quint8 Index);
    TempCtrlState_t ALGetTemperatureControlState(ALTempCtrlType_t Type);
    ReturnCode_t ALTurnOnFan();
    ReturnCode_t ALTurnOffFan();
    ReturnCode_t ALAllStop();
    ReturnCode_t ALStartTemperatureControlWithPID(ALTempCtrlType_t Type, qreal NominalTemperature, quint8 SlopeTempChange, quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime);
    //Rotary Valve device func
    ReturnCode_t RVSetTempCtrlON();
    ReturnCode_t RVSetTempCtrlOFF();
    ReturnCode_t RVSetTemperaturePid(quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime);
    ReturnCode_t RVStartTemperatureControl(qreal NominalTemperature, quint8 SlopeTempChange);
    qreal RVGetRecentTemperature(quint32 Index);
    TempCtrlState_t RVGetTemperatureControlState();
    //! Execute the move to intial position of the RV
    ReturnCode_t RVReqMoveToInitialPosition();
    //! Position the oven cover
    ReturnCode_t RVReqMoveToRVPosition( RVPosition_t RVPosition);
    //! Request actual oven cover position
    RVPosition_t RVReqActRVPosition();
    ReturnCode_t RVStartTemperatureControlWithPID(qreal NominalTemperature, quint8 SlopeTempChange, quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime);
    //Oven device func
    ReturnCode_t OvenSetTempCtrlON(OVENTempCtrlType_t Type);
    ReturnCode_t OvenSetTemperaturePid(OVENTempCtrlType_t Type, quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime);
    qreal OvenGetRecentTemperature(OVENTempCtrlType_t Type, quint8 Index);
    quint16 OvenGetRecentLidStatus();
    TempCtrlState_t OvenGetTemperatureControlState(OVENTempCtrlType_t Type);
    ReturnCode_t OvenStartTemperatureControlWithPID(OVENTempCtrlType_t Type, qreal NominalTemperature, quint8 SlopeTempChange, quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime);
    //Retort device func
    ReturnCode_t RTSetTempCtrlON(RTTempCtrlType_t Type);
    ReturnCode_t RTSetTempCtrlOFF(RTTempCtrlType_t Type);
    ReturnCode_t RTSetTemperaturePid(RTTempCtrlType_t Type, quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime);
    ReturnCode_t RTStartTemperatureControl(RTTempCtrlType_t Type, qreal NominalTemperature, quint8 SlopeTempChange);
    ReturnCode_t RTStartTemperatureControlWithPID(RTTempCtrlType_t Type, qreal NominalTemperature, quint8 SlopeTempChange, quint16 MaxTemperature, quint16 ControllerGain, quint16 ResetTime, quint16 DerivativeTime);
    qreal RTGetRecentTemperature(RTTempCtrlType_t Type, quint8 Index);
    TempCtrlState_t RTGetTemperatureControlState(RTTempCtrlType_t Type);
    ReturnCode_t RTUnlock();
    ReturnCode_t RTLock();
    quint16 RTGetRecentLockStatus();
    //Periphery device func
    ReturnCode_t PerTurnOffMainRelay();
};

} //namespace

#endif /* MOCK_IDEVICEPROCESSING_H */
