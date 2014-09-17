// ================================================================================================
/*! @file Himalaya/HimalayaMain/Master/Components/Scheduler/Source/SchedulerCommandProcessor.cpp
 *
 *  @brief
 *  Implementation of class Scheduler::SchedulerCommandProcessor.
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
#include "Scheduler/Include/SchedulerMainThreadController.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "Scheduler/Commands/Include/CmdStartConfigurationService.h"
#include "Scheduler/Commands/Include/CmdRestartConfigurationService.h"
#include "Scheduler/Commands/Include/CmdStartDiagnosticService.h"
#include "Scheduler/Commands/Include/CmdCloseDiagnosticService.h"
#include "Scheduler/Commands/Include/CmdStartAdjustmentService.h"
#include "Scheduler/Commands/Include/CmdALSetPressureCtrlON.h"
#include "Scheduler/Commands/Include/CmdALSetPressureCtrlOFF.h"
#include "Scheduler/Commands/Include/CmdALReleasePressure.h"
#include "Scheduler/Commands/Include/CmdALPressure.h"
#include "Scheduler/Commands/Include/CmdALVaccum.h"
#include "Scheduler/Commands/Include/CmdALDraining.h"
#include "Scheduler/Commands/Include/CmdIDForceDraining.h"
#include "Scheduler/Commands/Include/CmdALFilling.h"
#include "Scheduler/Commands/Include/CmdALStopCmdExec.h"
#include "Scheduler/Commands/Include/CmdALGetRecentPressure.h"
#include "Scheduler/Commands/Include/CmdALSetTempCtrlON.h"
#include "Scheduler/Commands/Include/CmdALSetTempCtrlOFF.h"
#include "Scheduler/Commands/Include/CmdALSetTemperaturePid.h"
#include "Scheduler/Commands/Include/CmdALStartTemperatureControl.h"
#include "Scheduler/Commands/Include/CmdALGetRecentTemperature.h"
#include "Scheduler/Commands/Include/CmdALGetTemperatureControlState.h"
#include "Scheduler/Commands/Include/CmdALTurnOnFan.h"
#include "Scheduler/Commands/Include/CmdALTurnOffFan.h"
#include "Scheduler/Commands/Include/CmdRVSetTempCtrlON.h"
#include "Scheduler/Commands/Include/CmdRVSetTempCtrlOFF.h"
#include "Scheduler/Commands/Include/CmdRVSetTemperaturePid.h"
#include "Scheduler/Commands/Include/CmdRVStartTemperatureControl.h"
#include "Scheduler/Commands/Include/CmdRVGetRecentTemperature.h"
#include "Scheduler/Commands/Include/CmdRVGetTemperatureControlState.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToInitialPosition.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToRVPosition.h"
#include "Scheduler/Commands/Include/CmdRVReqActRVPosition.h"
#include "Scheduler/Commands/Include/CmdOvenSetTempCtrlON.h"
#include "Scheduler/Commands/Include/CmdOvenSetTempCtrlOFF.h"
#include "Scheduler/Commands/Include/CmdOvenSetTemperaturePid.h"
#include "Scheduler/Commands/Include/CmdOvenStartTemperatureControl.h"
#include "Scheduler/Commands/Include/CmdOvenGetRecentTemperature.h"
#include "Scheduler/Commands/Include/CmdOvenGetTemperatureControlState.h"
#include "Scheduler/Commands/Include/CmdRTSetTempCtrlON.h"
#include "Scheduler/Commands/Include/CmdRTSetTempCtrlOFF.h"
#include "Scheduler/Commands/Include/CmdRTSetTemperaturePid.h"
#include "Scheduler/Commands/Include/CmdRTStartTemperatureControl.h"
#include "Scheduler/Commands/Include/CmdRTGetRecentTemperature.h"
#include "Scheduler/Commands/Include/CmdRTGetTemperatureControlState.h"
#include "Scheduler/Commands/Include/CmdRTUnlock.h"
#include "Scheduler/Commands/Include/CmdRTLock.h"
#include "Scheduler/Commands/Include/CmdPerTurnOffMainRelay.h"
#include "Scheduler/Commands/Include/CmdPerTurnOnMainRelay.h"
#include "Scheduler/Commands/Include/CmdRVStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdOvenStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdRTStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdALStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdIDBottleCheck.h"
#include "Scheduler/Commands/Include/CmdALAllStop.h"
#include "Scheduler/Commands/Include/CmdIDSealingCheck.h"
#include "Scheduler/Commands/Include/CmdRmtLocAlarm.h"
#include "Scheduler/Commands/Include/CmdALControlValve.h"
#include "Scheduler/Commands/Include/CmdRVSetTemperatureSwitchState.h"
#include "Scheduler/Commands/Include/CmdRTSetTemperatureSwitchState.h"

#ifdef GOOGLE_MOCK
#include <gmock/gmock.h>
#include "Scheduler/Test/Mock/MockIDeviceProcessing.h"
#else
#include "DeviceControl/Include/Interface/IDeviceProcessing.h"
#endif

namespace Scheduler{


template <class DP>
SchedulerCommandProcessor<DP>::SchedulerCommandProcessor(SchedulerMainThreadController *controller) :
    mp_SchedulerThreadController(controller),
    mp_IDeviceProcessing(NULL)
{

}
template <class DP>
SchedulerCommandProcessor<DP>::~SchedulerCommandProcessor()
{
#ifndef GOOGLE_MOCK
    delete mp_IDeviceProcessing;
    mp_IDeviceProcessing = NULL;
#endif
}

template <class DP>
HardwareMonitor_t SchedulerCommandProcessor<DP>::HardwareMonitor()
{
    HardwareMonitor_t strctHWMonitor;
    memset(&strctHWMonitor, 0, sizeof(strctHWMonitor));

	strctHWMonitor.PressureAL			= mp_IDeviceProcessing->ALGetRecentPressure();
	strctHWMonitor.TempALLevelSensor	= mp_IDeviceProcessing->ALGetRecentTemperature(AL_LEVELSENSOR, 0);
	strctHWMonitor.TempALTube1			= mp_IDeviceProcessing->ALGetRecentTemperature(AL_TUBE1,0);
	strctHWMonitor.TempALTube2			= mp_IDeviceProcessing->ALGetRecentTemperature(AL_TUBE2,0);
    strctHWMonitor.TempRV1				= mp_IDeviceProcessing->RVGetRecentTemperature(1);
    strctHWMonitor.TempRV2				= mp_IDeviceProcessing->RVGetRecentTemperature(0);
	strctHWMonitor.PositionRV			= mp_IDeviceProcessing->RVReqActRVPosition();
    strctHWMonitor.TempRTBottom1		= mp_IDeviceProcessing->RTGetRecentTemperature(RT_BOTTOM,0);
    strctHWMonitor.TempRTBottom2		= mp_IDeviceProcessing->RTGetRecentTemperature(RT_BOTTOM,1);
	strctHWMonitor.TempRTSide			= mp_IDeviceProcessing->RTGetRecentTemperature(RT_SIDE,0);
    strctHWMonitor.TempOvenBottom1		= mp_IDeviceProcessing->OvenGetRecentTemperature(OVEN_BOTTOM,0);
    strctHWMonitor.TempOvenBottom2		= mp_IDeviceProcessing->OvenGetRecentTemperature(OVEN_BOTTOM,1);
	strctHWMonitor.TempOvenTop			= mp_IDeviceProcessing->OvenGetRecentTemperature(OVEN_TOP,0);
	strctHWMonitor.OvenLidStatus		= mp_IDeviceProcessing->OvenGetRecentLidStatus();
	strctHWMonitor.RetortLockStatus		= mp_IDeviceProcessing->RTGetRecentLockStatus();
    strctHWMonitor.CurrentRTSide        = mp_IDeviceProcessing->GetSensorCurrent("Retort",0);
    strctHWMonitor.CurrentRTBottom      = mp_IDeviceProcessing->GetSensorCurrent("Retort",1);
    strctHWMonitor.CurrentOvenTop       = mp_IDeviceProcessing->GetSensorCurrent("Oven",0);
    strctHWMonitor.CurrentOvenBottom    = mp_IDeviceProcessing->GetSensorCurrent("Oven",1);
    strctHWMonitor.CurrentLALevelSensor = mp_IDeviceProcessing->GetSensorCurrent("LA",0);
    strctHWMonitor.CurrentLATube1       = mp_IDeviceProcessing->GetSensorCurrent("LA",1);
    strctHWMonitor.CurrentLATube2       = mp_IDeviceProcessing->GetSensorCurrent("LA",2);
    strctHWMonitor.CurrentRVTemp        = mp_IDeviceProcessing->GetSensorCurrent("RV",0);
    strctHWMonitor.Slave3Voltage        = mp_IDeviceProcessing->IDGetSlaveVoltage(Slave_3);
    strctHWMonitor.Slave5Voltage        = mp_IDeviceProcessing->IDGetSlaveVoltage(Slave_5);
    strctHWMonitor.Slave15Voltage       = mp_IDeviceProcessing->IDGetSlaveVoltage(Slave_15);
    strctHWMonitor.Slave3Current        = mp_IDeviceProcessing->IDGetSlaveCurrent(Slave_3);
    strctHWMonitor.Slave5Current        = mp_IDeviceProcessing->IDGetSlaveCurrent(Slave_5);
    strctHWMonitor.Slave15Current       = mp_IDeviceProcessing->IDGetSlaveCurrent(Slave_15);
    strctHWMonitor.OvenHeatingStatus    = mp_IDeviceProcessing->OvenGetHeatingStatus(OVEN_BOTTOM) && mp_IDeviceProcessing->OvenGetHeatingStatus(OVEN_TOP) ;
    return strctHWMonitor;
}

template <class DP>
void SchedulerCommandProcessor<DP>::run4Slot()
{
    qRegisterMetaType<ReturnCode_t>("ReturnCode_t");
    qRegisterMetaType<quint32>("quint32");


#ifndef GOOGLE_MOCK
    //Initialize IDeviceProcessing
    mp_IDeviceProcessing = new DP();
#endif

    //connect(mp_IDeviceProcessing, SIGNAL(ReportInitializationFinished(DevInstanceID_t, ReturnCode_t)), this, SLOT(DevProcInitialisationAckn(DevInstanceID_t, ReturnCode_t)), Qt::QueuedConnection);
    CONNECTSIGNALSLOT(mp_IDeviceProcessing, ReportInitializationFinished(quint32, ReturnCode_t),
                      this, DevProcInitialisationAckn(quint32, ReturnCode_t));
    CONNECTSIGNALSLOT(mp_IDeviceProcessing, ReportConfigurationFinished(quint32, ReturnCode_t),
                      this, DevProcConfigurationAckn(quint32, ReturnCode_t));
    CONNECTSIGNALSLOT(mp_IDeviceProcessing, ReportStartNormalOperationMode(quint32, ReturnCode_t),
                      this, DevProcStartNormalOpModeAckn(quint32, ReturnCode_t));
    CONNECTSIGNALSLOT(mp_IDeviceProcessing, ReportError(quint32, quint16, quint16, quint16, const QDateTime &),
                      this, ThrowError(quint32, quint16, quint16, quint16, const QDateTime &));
    CONNECTSIGNALSIGNAL(mp_IDeviceProcessing, ReportDestroyFinished(), this, DeviceProcessDestroyed());
    CONNECTSIGNALSIGNAL(mp_IDeviceProcessing, ReportLevelSensorStatus1(), this, ReportLevelSensorStatus1());
    CONNECTSIGNALSIGNAL(mp_IDeviceProcessing, ReportFillingTimeOut2Min(), this, ReportFillingTimeOut2Min());
    CONNECTSIGNALSIGNAL(mp_IDeviceProcessing, ReportDrainingTimeOut2Min(), this, ReportDrainingTimeOut2Min());
    CONNECTSIGNALSIGNAL(mp_IDeviceProcessing, ReportGetServiceInfo(ReturnCode_t, const DataManager::CModule&, const QString&),
                     this, ReportGetServiceInfo(ReturnCode_t, const DataManager::CModule&, const QString&));
    CONNECTSIGNALSLOT(this, NewCmdAdded(), this, OnNewCmdAdded());
    CONNECTSIGNALSLOT(this, SigShutDownDevice(), this, OnShutDownDevice());
    CONNECTSIGNALSLOT(this, SigNotifySavedServiceInfor(const QString&), this, OnNotifySavedServiceInfor(const QString&));
    CONNECTSIGNALSLOT(this, SigResetActiveCarbonFilterLifetime(), this, OnResetActiveCarbonFilterLifetime());

}

template <class DP>
void SchedulerCommandProcessor<DP>::OnNewCmdAdded4Slot()
{
    Scheduler::SchedulerCommandShPtr_t scmd;
    if(newCmdComing(scmd))
    {
        this->ExecuteCmd(scmd);
        mp_SchedulerThreadController->PushDeviceControlCmdQueue(scmd);
    }
}

template <class DP>
void SchedulerCommandProcessor<DP>:: pushCmd4Slot(CmdSchedulerCommandBase* cmd)
{
    m_CmdMutex.lock();
    m_Cmds.push_front(Scheduler::SchedulerCommandShPtr_t(cmd));
    m_CmdMutex.unlock();
    emit NewCmdAdded();
}

template <class DP>
bool SchedulerCommandProcessor<DP>::newCmdComing(Scheduler::SchedulerCommandShPtr_t& scmd)
{
    bool ret = false;
    m_CmdMutex.lock();
    if(!m_Cmds.isEmpty())
    {
        ret = true;
        scmd =  m_Cmds.dequeue();
    }
    m_CmdMutex.unlock();
    return ret;
}

template <class DP>
void SchedulerCommandProcessor<DP>::DevProcInitialisationAckn4Slot(quint32 instanceID, ReturnCode_t configResult)
{
    // interface implementation runs in DeviceControl-Thread
    ReturnCode_t retCode;

    qDebug()<< "Configurating DeviceControl Layer...";

    qDebug() << "  CApplicationControl::DevProcInitialisationAckn" << " " << instanceID << " " << configResult;


    QString SerialNo;
    if (IDeviceProcessing::GetSerialNumber(SerialNo)) {
        qDebug() << "  Serial number is " << SerialNo;
    } else {
        qDebug() << "  Error: getting serial number failed.";
    }
    retCode = mp_IDeviceProcessing->StartConfigurationService();
    if(retCode != DCL_ERR_FCT_CALL_SUCCESS)
    {
        qDebug() << "  Error starting configuration service: " << retCode;
    }
}

template <class DP>
void SchedulerCommandProcessor<DP>::DevProcConfigurationAckn4Slot(quint32 instanceID, ReturnCode_t hdlInfo)
{
    // interface implementation runs in DeviceControl-Thread
    qDebug() << "  SchedulerCommandProcessor::DevProcConfigurationAckn" << instanceID << "ReturnCode" << hdlInfo;
    if(hdlInfo == DCL_ERR_FCT_CALL_SUCCESS)
    {
      //successfully configured the devices
    }
    else
    {
        //some error happens
    }
    emit DCLConfigurationFinished(hdlInfo);
}

template <class DP>
void SchedulerCommandProcessor<DP>::DevProcStartNormalOpModeAckn4Slot(quint32 instanceID, ReturnCode_t hdlInfo)
{
    qDebug() << "  SchedulerCommandProcessor::DevProcStartNormalOpModeAckn " << instanceID << " " << hdlInfo;
}

template <class DP>
void SchedulerCommandProcessor<DP>::ThrowError4Slot(quint32 instanceID, quint16 usErrorGroup, quint16 usErrorID, quint16 usErrorData,const QDateTime & TimeStamp)
{
    Q_UNUSED(instanceID)
    Q_UNUSED(usErrorGroup)
    Q_UNUSED(usErrorID)
    Q_UNUSED(usErrorData)
    Q_UNUSED(TimeStamp)
    // Platform/Master/Components/DeviceControl/Include/Global/
    //   DeviceControlError.h
    //   DeviceControlGlobal.h
    //WriteToLog(
     //                       QString() + "Thrown DCL Error:" +
     //                       "  Module:" +  QString().setNum(instanceID, 16) +
     //                       ", Group:"  +  QString().setNum(usErrorGroup, 16) +
     //                       ", ID:"     +  QString().setNum(usErrorID, 16) +
     //                       ", Data:"   +  QString().setNum(usErrorData, 16) +
     //                       ", TimeStamp:" + TimeStamp.toString()
     //                       );
}

template <class DP>
void SchedulerCommandProcessor<DP>::ExecuteCmd(Scheduler::SchedulerCommandShPtr_t& scmd)
{
    QString cmdName = scmd->GetName();

	if ("Scheduler::StartConfigurationService" == cmdName)
	{
        scmd->SetResult(mp_IDeviceProcessing->StartConfigurationService());
	}
	else if ("Scheduler::RestartConfigurationService" == cmdName)
	{
        scmd->SetResult(mp_IDeviceProcessing->RestartConfigurationService());
	}
	else if  ("Scheduler::StartDiagnosticService" == cmdName)
	{
        scmd->SetResult(mp_IDeviceProcessing->StartDiagnosticService());
	}
	else if  ("Scheduler::CloseDiagnosticService" == cmdName)
	{
        scmd->SetResult(mp_IDeviceProcessing->CloseDiagnosticService());
	}
	else if  ("Scheduler::StartAdjustmentService" == cmdName)
	{
        scmd->SetResult(mp_IDeviceProcessing->StartAdjustmentService());
	}
	else if  ("Scheduler::ALSetPressureCtrlON" == cmdName)
	{
        scmd->SetResult(mp_IDeviceProcessing->ALSetPressureCtrlON());
	}
	else if  ("Scheduler::ALSetPressureCtrlOFF" == cmdName)
	{
        scmd->SetResult(mp_IDeviceProcessing->ALSetPressureCtrlOFF());
	}
	else if  ("Scheduler::ALReleasePressure" == cmdName)
	{
        scmd->SetResult(mp_IDeviceProcessing->ALReleasePressure());
	}
	else if  ("Scheduler::ALPressure" == cmdName)
	{
        scmd->SetResult(mp_IDeviceProcessing->ALPressure(qSharedPointerDynamicCast<CmdALPressure>(scmd)->GetTargetPressure()));
	}
	else if  ("Scheduler::ALVaccum" == cmdName)
	{
        scmd->SetResult(mp_IDeviceProcessing->ALVaccum());
	}
	else if  ("Scheduler::ALDraining" == cmdName)
	{
        scmd->SetResult(mp_IDeviceProcessing->ALDraining(qSharedPointerDynamicCast<CmdALDraining>(scmd)->GetDelayTime(), qSharedPointerDynamicCast<CmdALDraining>(scmd)->GetDrainPressure()));
	}
    else if  ("Scheduler::IDForceDraining" == cmdName)
    {
        scmd->SetResult(mp_IDeviceProcessing->IDForceDraining(qSharedPointerDynamicCast<CmdIDForceDraining>(scmd)->GetRVPosition(), qSharedPointerDynamicCast<CmdIDForceDraining>(scmd)->GetDrainPressure()));
    }
	else if  ("Scheduler::ALFilling" == cmdName)
	{
        scmd->SetResult(mp_IDeviceProcessing->ALFilling(qSharedPointerDynamicCast<CmdALFilling>(scmd)->GetDelayTime(),
                                                                qSharedPointerDynamicCast<CmdALFilling>(scmd)->GetEnableInsufficientCheck()));
	}
    else if ("Scheduler::ALStopCmdExec" == cmdName)
    {
        scmd->SetResult(mp_IDeviceProcessing->ALStopCmdExec(qSharedPointerDynamicCast<CmdALStopCmdExec>(scmd)->GetCmdType()));
    }
	else if  ("Scheduler::ALGetRecentPressure" == cmdName)
	{
        scmd->SetResult(mp_IDeviceProcessing->ALGetRecentPressure());
	}
	else if  ("Scheduler::ALSetTempCtrlON" == cmdName)
	{
        scmd->SetResult(mp_IDeviceProcessing->ALSetTempCtrlON(qSharedPointerDynamicCast<CmdALSetTempCtrlON>(scmd)->GetType()));
	}
	else if  ("Scheduler::ALSetTempCtrlOFF" == cmdName)
	{
        scmd->SetResult(mp_IDeviceProcessing->ALSetTempCtrlOFF(qSharedPointerDynamicCast<CmdALSetTempCtrlOFF>(scmd)->Gettype()));
	}
	else if  ("Scheduler::ALSetTemperaturePid" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->ALSetTemperaturePid( qSharedPointerDynamicCast<CmdALSetTemperaturePid>(scmd)->GetType(),
                                                                            qSharedPointerDynamicCast<CmdALSetTemperaturePid>(scmd)->GetMaxTemperature(),
                                                                            qSharedPointerDynamicCast<CmdALSetTemperaturePid>(scmd)->GetControllerGain(),
                                                                            qSharedPointerDynamicCast<CmdALSetTemperaturePid>(scmd)->GetResetTime(),
                                                                            qSharedPointerDynamicCast<CmdALSetTemperaturePid>(scmd)->GetDerivativeTime()
																		   ) );
	}
	else if  ("Scheduler::ALStartTemperatureControl" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->ALStartTemperatureControl( qSharedPointerDynamicCast<CmdALStartTemperatureControl>(scmd)->GetType(),
                                                                                  qSharedPointerDynamicCast<CmdALStartTemperatureControl>(scmd)->GetNominalTemperature(),
                                                                                  qSharedPointerDynamicCast<CmdALStartTemperatureControl>(scmd)->GetSlopeTempChange()
																				) );
	}
	else if  ("Scheduler::ALGetRecentTemperature" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->ALGetRecentTemperature( qSharedPointerDynamicCast<CmdALGetRecentTemperature>(scmd)->GetType(),
                                                                               qSharedPointerDynamicCast<CmdALGetRecentTemperature>(scmd)->GetIndex()
																			 ) );
	}
	else if  ("Scheduler::ALGetTemperatureControlState" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->ALGetTemperatureControlState( qSharedPointerDynamicCast<CmdALGetTemperatureControlState>(scmd)->GetType() ) );
	}
	else if  ("Scheduler::ALTurnOnFan" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->ALTurnOnFan() );
	}
	else if  ("Scheduler::ALTurnOffFan" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->ALTurnOffFan() );
	}
	else if  ("Scheduler::RVSetTempCtrlON" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->RVSetTempCtrlON() );
	}
	else if  ("Scheduler::RVSetTempCtrlOFF" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->RVSetTempCtrlOFF() );
	}
	else if  ("Scheduler::RVSetTemperaturePid" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->RVSetTemperaturePid( qSharedPointerDynamicCast<CmdRVSetTemperaturePid>(scmd)->GetMaxTemperature(),
                                                                            qSharedPointerDynamicCast<CmdRVSetTemperaturePid>(scmd)->GetControllerGain(),
                                                                            qSharedPointerDynamicCast<CmdRVSetTemperaturePid>(scmd)->GetResetTime(),
                                                                            qSharedPointerDynamicCast<CmdRVSetTemperaturePid>(scmd)->GetDerivativeTime()
																		  ) );
	}
	else if  ("Scheduler::RVStartTemperatureControl" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->RVStartTemperatureControl( qSharedPointerDynamicCast<CmdRVStartTemperatureControl>(scmd)->GetNominalTemperature(),
                                                                                  qSharedPointerDynamicCast<CmdRVStartTemperatureControl>(scmd)->GetSlopeTempChange()
																				) );
	}
	else if  ("Scheduler::RVGetRecentTemperature" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->RVGetRecentTemperature( qSharedPointerDynamicCast<CmdRVGetRecentTemperature>(scmd)->GetIndex() ) );
	}
	else if  ("Scheduler::RVGetTemperatureControlState" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->RVGetTemperatureControlState() );
	}
	else if  ("Scheduler::RVReqMoveToInitialPosition" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->RVReqMoveToInitialPosition( qSharedPointerDynamicCast<CmdRVReqMoveToInitialPosition>(scmd)->GetRVPosition() ) );
	}
	else if  ("Scheduler::RVReqMoveToRVPosition" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->RVReqMoveToRVPosition( qSharedPointerDynamicCast<CmdRVReqMoveToRVPosition>(scmd)->GetRVPosition() ) );
	}
	else if  ("Scheduler::RVReqActRVPosition" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->RVReqActRVPosition() );
	}
	else if  ("Scheduler::OvenSetTempCtrlON" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->OvenSetTempCtrlON(qSharedPointerDynamicCast<CmdOvenSetTempCtrlON>(scmd)->GetType() ) );
	}
	else if  ("Scheduler::OvenSetTempCtrlOFF" == cmdName)
    {
        scmd->SetResult( mp_IDeviceProcessing->OvenSetTempCtrlOFF(qSharedPointerDynamicCast<CmdOvenSetTempCtrlOFF>(scmd)->Gettype() ) );
	}
	else if  ("Scheduler::OvenSetTemperaturePid" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->OvenSetTemperaturePid( qSharedPointerDynamicCast<CmdOvenSetTemperaturePid>(scmd)->GetType(),
                                                                              qSharedPointerDynamicCast<CmdOvenSetTemperaturePid>(scmd)->GetMaxTemperature(),
                                                                              qSharedPointerDynamicCast<CmdOvenSetTemperaturePid>(scmd)->GetControllerGain(),
                                                                              qSharedPointerDynamicCast<CmdOvenSetTemperaturePid>(scmd)->GetResetTime(),
                                                                              qSharedPointerDynamicCast<CmdOvenSetTemperaturePid>(scmd)->GetDerivativeTime()
																			) );
	}
	else if  ("Scheduler::OvenStartTemperatureControl" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->OvenStartTemperatureControl( qSharedPointerDynamicCast<CmdOvenStartTemperatureControl>(scmd)->GetType(),
                                                                                    qSharedPointerDynamicCast<CmdOvenStartTemperatureControl>(scmd)->GetNominalTemperature(),
                                                                                    qSharedPointerDynamicCast<CmdOvenStartTemperatureControl>(scmd)->GetSlopeTempChange()
                                                                                  ) );
	}
	else if  ("Scheduler::OvenGetRecentTemperature" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->OvenGetRecentTemperature( qSharedPointerDynamicCast<CmdOvenGetRecentTemperature>(scmd)->GetType(),
                                                                                 qSharedPointerDynamicCast<CmdOvenGetRecentTemperature>(scmd)->GetIndex() ) );
	}
	else if  ("Scheduler::OvenGetTemperatureControlState" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->OvenGetTemperatureControlState( qSharedPointerDynamicCast<CmdOvenGetTemperatureControlState>(scmd)->GetType() ) );
	}
	else if  ("Scheduler::RTSetTempCtrlON" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->RTSetTempCtrlON( qSharedPointerDynamicCast<CmdRTSetTempCtrlON>(scmd)->GetType() ) );
	}
	else if  ("Scheduler::RTSetTempCtrlOFF" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->RTSetTempCtrlOFF( qSharedPointerDynamicCast<CmdRTSetTempCtrlOFF>(scmd)->GetType() ) );
	}
	else if  ("Scheduler::RTSetTemperaturePid" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->RTSetTemperaturePid( qSharedPointerDynamicCast<CmdRTSetTemperaturePid>(scmd)->GetType(),
                                                                            qSharedPointerDynamicCast<CmdRTSetTemperaturePid>(scmd)->GetMaxTemperature(),
                                                                            qSharedPointerDynamicCast<CmdRTSetTemperaturePid>(scmd)->GetControllerGain(),
                                                                            qSharedPointerDynamicCast<CmdRTSetTemperaturePid>(scmd)->GetResetTime(),
                                                                            qSharedPointerDynamicCast<CmdRTSetTemperaturePid>(scmd)->GetDerivativeTime()
																		  ) );
	}
	else if  ("Scheduler::RTStartTemperatureControl" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->RTStartTemperatureControl( qSharedPointerDynamicCast<CmdRTStartTemperatureControl>(scmd)->GetType(),
                                                                                  qSharedPointerDynamicCast<CmdRTStartTemperatureControl>(scmd)->GetNominalTemperature(),
                                                                                  qSharedPointerDynamicCast<CmdRTStartTemperatureControl>(scmd)->GetSlopeTempChange()
																				) );
	}
	else if  ("Scheduler::RTGetRecentTemperature" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->RTGetRecentTemperature( qSharedPointerDynamicCast<CmdRTGetRecentTemperature>(scmd)->GetType(),
                                                                               qSharedPointerDynamicCast<CmdRTGetRecentTemperature>(scmd)->GetIndex()
																			 ) );
	}
	else if  ("Scheduler::RTGetTemperatureControlState" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->RTGetTemperatureControlState( qSharedPointerDynamicCast<CmdRTGetTemperatureControlState>(scmd)->GetType()) );
	}
	else if  ("Scheduler::RTUnlock" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->RTUnlock() );
	}
	else if  ("Scheduler::RTLock" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->RTLock() );
	}
	else if  ("Scheduler::PerTurnOffMainRelay" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->PerTurnOffMainRelay() );
	}
	else if  ("Scheduler::PerTurnOnMainRelay" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->PerTurnOnMainRelay() );
	}
	else if  ("Scheduler::RVStartTemperatureControlWithPID" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->RVStartTemperatureControlWithPID( qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(scmd)->GetNominalTemperature(),
                                                                                         qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(scmd)->GetSlopeTempChange(),
                                                                                         qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(scmd)->GetMaxTemperature(),
                                                                                         qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(scmd)->GetControllerGain(),
                                                                                         qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(scmd)->GetResetTime(),
                                                                                         qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(scmd)->GetDerivativeTime()
																					   ) );
	}
	else if  ("Scheduler::OvenStartTemperatureControlWithPID" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->OvenStartTemperatureControlWithPID( qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(scmd)->GetType(),
                                                                                           qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(scmd)->GetNominalTemperature(),
                                                                                           qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(scmd)->GetSlopeTempChange(),
                                                                                           qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(scmd)->GetMaxTemperature(),
                                                                                           qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(scmd)->GetControllerGain(),
                                                                                           qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(scmd)->GetResetTime(),
                                                                                           qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(scmd)->GetDerivativeTime()
																						) );
	}
	else if  ("Scheduler::RTStartTemperatureControlWithPID" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->RTStartTemperatureControlWithPID( qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(scmd)->GetType(),
                                                                                         qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(scmd)->GetNominalTemperature(),
                                                                                         qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(scmd)->GetSlopeTempChange(),
                                                                                         qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(scmd)->GetMaxTemperature(),
                                                                                         qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(scmd)->GetControllerGain(),
                                                                                         qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(scmd)->GetResetTime(),
                                                                                         qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(scmd)->GetDerivativeTime()
																						) );
	}
	else if  ("Scheduler::ALStartTemperatureControlWithPID" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->ALStartTemperatureControlWithPID( qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(scmd)->GetType(),
                                                                                         qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(scmd)->GetNominalTemperature(),
                                                                                         qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(scmd)->GetSlopeTempChange(),
                                                                                         qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(scmd)->GetMaxTemperature(),
                                                                                         qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(scmd)->GetControllerGain(),
                                                                                         qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(scmd)->GetResetTime(),
                                                                                         qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(scmd)->GetDerivativeTime()
																					   ) );
	}
	else if  ("Scheduler::IDBottleCheck" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->IDBottleCheck( qSharedPointerDynamicCast<CmdIDBottleCheck>(scmd)->GetReagentGrpID(),
                                                                      qSharedPointerDynamicCast<CmdIDBottleCheck>(scmd)->GetTubePos() ) );
	}
	else if  ("Scheduler::ALAllStop" == cmdName)
	{
        scmd->SetResult( mp_IDeviceProcessing->ALAllStop() );
	}
    else if ("Scheduler::IDSealingCheck" == cmdName)
    {
        scmd->SetResult( mp_IDeviceProcessing->IDSealingCheck(qSharedPointerDynamicCast<CmdIDSealingCheck>(scmd)->GetThresholdPressure()));
    }
    else if ("Scheduler::RmtLocAlarm" == cmdName)
    {
        scmd->SetResult( mp_IDeviceProcessing->IDSetAlarm(qSharedPointerDynamicCast<CmdRmtLocAlarm>(scmd)->GetRmtLocOpcode()));
    }
    else if ("Scheduler::ALControlValve" == cmdName)
    {
        scmd->SetResult( mp_IDeviceProcessing->ALControlValve(qSharedPointerDynamicCast<CmdALControlValve>(scmd)->GetValveIndex(),
                         qSharedPointerDynamicCast<CmdALControlValve>(scmd)->GetValveState()) );
    }
    else if ("Scheduler::RVSetTemperatureSwitchState" == cmdName)
    {
        scmd->SetResult( mp_IDeviceProcessing->RVSetTemperatureSwitchState(qSharedPointerDynamicCast<CmdRVSetTemperatureSwitchState>(scmd)->GetHeaterVoltage(),
                                                                           qSharedPointerDynamicCast<CmdRVSetTemperatureSwitchState>(scmd)->GetAutoType()) );
    }
    else if ("Scheduler::RTSetTemperatureSwitchState" == cmdName)
    {
        scmd->SetResult( mp_IDeviceProcessing->RTSetTemperatureSwitchState( qSharedPointerDynamicCast<CmdRTSetTemperatureSwitchState>(scmd)->GetType(),
                                                                            qSharedPointerDynamicCast<CmdRTSetTemperatureSwitchState>(scmd)->GetHeaterVoltage(),
                                                                            qSharedPointerDynamicCast<CmdRTSetTemperatureSwitchState>(scmd)->GetAutoType()) );
    }
}

template <class DP>
void SchedulerCommandProcessor<DP>::ShutDownDevice()
{
    emit SigShutDownDevice();
}

template <class DP>
void SchedulerCommandProcessor<DP>::NotifySavedServiceInfor(const QString& deviceType)
{
    emit SigNotifySavedServiceInfor(deviceType);
}

template <class DP>
void SchedulerCommandProcessor<DP>::ResetActiveCarbonFilterLifetime()
{
    emit SigResetActiveCarbonFilterLifetime();
}

template <class DP>
ReportError_t SchedulerCommandProcessor<DP>::GetSlaveModuleReportError(quint8 CANErrCode, const QString& devName, quint32 sensorName)
{
    return mp_IDeviceProcessing->GetSlaveModuleReportError(CANErrCode, devName, sensorName);
}

template <class DP>
void SchedulerCommandProcessor<DP>::OnShutDownDevice4Slot()
{
    if (mp_IDeviceProcessing) {
        mp_IDeviceProcessing->RVSetTempCtrlOFF();
        mp_IDeviceProcessing->OvenSetTempCtrlOFF(OVEN_TOP);
        mp_IDeviceProcessing->OvenSetTempCtrlOFF(OVEN_BOTTOM);
        mp_IDeviceProcessing->ALSetTempCtrlOFF(AL_TUBE1);
        mp_IDeviceProcessing->ALSetTempCtrlOFF(AL_TUBE2);
        mp_IDeviceProcessing->ALSetTempCtrlOFF(AL_LEVELSENSOR);
        mp_IDeviceProcessing->RTSetTempCtrlOFF(RT_BOTTOM);
        mp_IDeviceProcessing->RTSetTempCtrlOFF(RT_SIDE);
        mp_IDeviceProcessing->PerTurnOffMainRelay();

        mp_IDeviceProcessing->RVGetTemperatureControlState();
        mp_IDeviceProcessing->OvenGetTemperatureControlState(OVEN_TOP);
        mp_IDeviceProcessing->OvenGetTemperatureControlState(OVEN_BOTTOM);
        mp_IDeviceProcessing->ALGetTemperatureControlState(AL_TUBE1);
        mp_IDeviceProcessing->ALGetTemperatureControlState(AL_TUBE2);
        mp_IDeviceProcessing->ALGetTemperatureControlState(AL_LEVELSENSOR);
        mp_IDeviceProcessing->RTGetTemperatureControlState(RT_BOTTOM);
        mp_IDeviceProcessing->RTGetTemperatureControlState(RT_SIDE);
        mp_IDeviceProcessing->Destroy();     // trigger shutdown of device processing layer
    }
}

template <class DP>
void SchedulerCommandProcessor<DP>::OnNotifySavedServiceInfor4Slot(const QString& deviceType)
{
    mp_IDeviceProcessing->NotifySavedServiceInfor(deviceType);
}

template <class DP>
void SchedulerCommandProcessor<DP>::OnResetActiveCarbonFilterLifetime4Slot()
{
    mp_IDeviceProcessing->ResetActiveCarbonFilterLifeTime();
}

}// end of namespace Scheduler


#ifdef GOOGLE_MOCK
template class Scheduler::SchedulerCommandProcessor<DeviceControl::MockIDeviceProcessing>;
#else
template class Scheduler::SchedulerCommandProcessor<DeviceControl::IDeviceProcessing>;
#endif
