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
#include "Scheduler/Commands/Include/CmdRVReqMoveToCurrentTubePosition.h"
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
#include "Scheduler/Commands/Include/CmdALPurge.h"
#include "Scheduler/Commands/Include/CmdRVSetTemperatureSwitchState.h"
#include "Scheduler/Commands/Include/CmdRTSetTemperatureSwitchState.h"
#include "future"

#ifdef GOOGLE_MOCK
#include <gmock/gmock.h>
#include "DeviceControl/Test/Mock/MockDeviceControl.h"
#else
#include "DeviceControl/Include/Interface/IDeviceProcessing.h"
#endif
#include"DeviceControl/Include/Simulation/DeviceControlSim.h"

namespace Scheduler{
/*lint -e534 */
/*lint -e613 */
/*lint -e525 */

template <class DP>
SchedulerCommandProcessor<DP>::SchedulerCommandProcessor(SchedulerMainThreadController* controller) :
    mp_SchedulerThreadController(controller),
    mp_IDeviceProcessing(NULL)
{
    //timer setting

}
template <class DP>
SchedulerCommandProcessor<DP>::~SchedulerCommandProcessor()
{
#ifndef GOOGLE_MOCK
    try {
        if (mp_IDeviceProcessing != NULL)
            delete mp_IDeviceProcessing;
        mp_IDeviceProcessing = NULL;
    }
    CATCHALL_DTOR();
#endif
    m_TickTimer.stop();
}

template <class DP>
HardwareMonitor_t SchedulerCommandProcessor<DP>::HardwareMonitor(const QString& sender)
{
    HardwareMonitor_t strctHWMonitor;
    memset(&strctHWMonitor, 0, sizeof(strctHWMonitor));

    strctHWMonitor.PressureAL			= mp_IDeviceProcessing->WithSender(sender)->ALGetRecentPressure();
    strctHWMonitor.TempALLevelSensor	= mp_IDeviceProcessing->WithSender(sender)->ALGetRecentTemperature(AL_LEVELSENSOR1, 0);
    strctHWMonitor.TempALTube1			= mp_IDeviceProcessing->WithSender(sender)->ALGetRecentTemperature(AL_TUBE1,0);
    strctHWMonitor.TempALTube2			= mp_IDeviceProcessing->WithSender(sender)->ALGetRecentTemperature(AL_TUBE2,0);
    strctHWMonitor.TempRV1				= mp_IDeviceProcessing->WithSender(sender)->RVGetRecentTemperature(0);
    strctHWMonitor.TempRV2				= mp_IDeviceProcessing->WithSender(sender)->RVGetRecentTemperature(1);
    strctHWMonitor.PositionRV			= mp_IDeviceProcessing->WithSender(sender)->RVReqActRVPosition();
    strctHWMonitor.LowerLimit           = mp_IDeviceProcessing->WithSender(sender)->GetCurrentLowerLimit();
    strctHWMonitor.TempRTBottom1		= mp_IDeviceProcessing->WithSender(sender)->RTGetRecentTemperature(RT_BOTTOM,0);
    strctHWMonitor.TempRTBottom2		= mp_IDeviceProcessing->WithSender(sender)->RTGetRecentTemperature(RT_BOTTOM,1);
    strctHWMonitor.TempRTSide			= mp_IDeviceProcessing->WithSender(sender)->RTGetRecentTemperature(RT_SIDE,0);
    strctHWMonitor.TempOvenBottom1		= mp_IDeviceProcessing->WithSender(sender)->OvenGetRecentTemperature(OVEN_BOTTOM,0);
    strctHWMonitor.TempOvenBottom2		= mp_IDeviceProcessing->WithSender(sender)->OvenGetRecentTemperature(OVEN_BOTTOM,1);
    strctHWMonitor.TempOvenTop			= mp_IDeviceProcessing->WithSender(sender)->OvenGetRecentTemperature(OVEN_TOP,0);
    strctHWMonitor.OvenLidStatus		= mp_IDeviceProcessing->WithSender(sender)->OvenGetRecentLidStatus();
    strctHWMonitor.RetortLockStatus		= mp_IDeviceProcessing->WithSender(sender)->RTGetRecentLockStatus();
    strctHWMonitor.CurrentHeatRetortOven        = mp_IDeviceProcessing->WithSender(sender)->GetSensorCurrent("Retort",0);
    strctHWMonitor.CurrentHeatLevelSensorTubes = mp_IDeviceProcessing->WithSender(sender)->GetSensorCurrent("LA",0);
    strctHWMonitor.CurrentRVTemp        = mp_IDeviceProcessing->WithSender(sender)->GetSensorCurrent("RV",0);
    strctHWMonitor.Slave3Voltage        = mp_IDeviceProcessing->WithSender(sender)->IDGetSlaveVoltage(Slave_3);
    strctHWMonitor.Slave5Voltage        = mp_IDeviceProcessing->WithSender(sender)->IDGetSlaveVoltage(Slave_5);
    strctHWMonitor.Slave15Voltage       = mp_IDeviceProcessing->WithSender(sender)->IDGetSlaveVoltage(Slave_15);
    strctHWMonitor.Slave3Current        = mp_IDeviceProcessing->WithSender(sender)->IDGetSlaveCurrent(Slave_3);
    strctHWMonitor.Slave5Current        = mp_IDeviceProcessing->WithSender(sender)->IDGetSlaveCurrent(Slave_5);
    strctHWMonitor.Slave15Current       = mp_IDeviceProcessing->WithSender(sender)->IDGetSlaveCurrent(Slave_15);
    strctHWMonitor.OvenHeatingStatus    = mp_IDeviceProcessing->WithSender(sender)->OvenGetHeatingStatus(OVEN_BOTTOM) && mp_IDeviceProcessing->WithSender(sender)->OvenGetHeatingStatus(OVEN_TOP) ;
    strctHWMonitor.LocalAlarmStatus     = mp_IDeviceProcessing->WithSender(sender)->PerGetRecentAlarmStatus(0);
    strctHWMonitor.RemoteAlarmStatus    = mp_IDeviceProcessing->WithSender(sender)->PerGetRecentAlarmStatus(1);
    strctHWMonitor.LATube1HeatingStatus = mp_IDeviceProcessing->WithSender(sender)->ALGetHeatingStatus(AL_TUBE1);
    strctHWMonitor.LATube2HeatingStatus = mp_IDeviceProcessing->WithSender(sender)->ALGetHeatingStatus(AL_TUBE2);
    return strctHWMonitor;
}

template <class DP>
void SchedulerCommandProcessor<DP>::run4Slot()
{
    qRegisterMetaType<ReturnCode_t>("ReturnCode_t");
    qRegisterMetaType<quint32>("quint32");


#ifndef GOOGLE_MOCK
    //Initialize IDeviceProcessing
    mp_IDeviceProcessing = new DP(20);


#endif

    //connect(mp_IDeviceProcessing, SIGNAL(ReportInitializationFinished(DevInstanceID_t, ReturnCode_t)), this, SLOT(DevProcInitialisationAckn(DevInstanceID_t, ReturnCode_t)), Qt::QueuedConnection);
    CONNECTSIGNALSLOT(mp_IDeviceProcessing, ReportInitializationFinished(quint32, ReturnCode_t),
                      this, DevProcInitialisationAckn(quint32, ReturnCode_t));
    CONNECTSIGNALSLOT(mp_IDeviceProcessing, ReportConfigurationFinished(quint32, ReturnCode_t),
                      this, DevProcConfigurationAckn(quint32, ReturnCode_t));
    CONNECTSIGNALSLOT(mp_IDeviceProcessing, ReportStartNormalOperationMode(quint32, ReturnCode_t),
                      this, DevProcStartNormalOpModeAckn(quint32, ReturnCode_t));
    CONNECTSIGNALSIGNAL(mp_IDeviceProcessing, ReportError(quint32, quint16, quint16, quint16, QDateTime),
                      this, ReportError(quint32, quint16, quint16, quint16, QDateTime));
    CONNECTSIGNALSIGNAL(mp_IDeviceProcessing, ReportDestroyFinished(), this, DeviceProcessDestroyed());
    CONNECTSIGNALSIGNAL(mp_IDeviceProcessing, ReportLevelSensorStatus1(), this, ReportLevelSensorStatus1());
    CONNECTSIGNALSIGNAL(mp_IDeviceProcessing, ReportFillingTimeOut2Min(), this, ReportFillingTimeOut2Min());
    CONNECTSIGNALSIGNAL(mp_IDeviceProcessing, ReportDrainingTimeOut2Min(), this, ReportDrainingTimeOut2Min());
//    CONNECTSIGNALSIGNAL(mp_IDeviceProcessing, ReporExecuteCmdtGetServiceInfo(ReturnCode_t, const DataManager::CModule&, const QString&),
//                     this, ReportGetServiceInfo(ReturnCode_t, const DataManager::CModule&, const QString&));
    CONNECTSIGNALSLOT(&m_TickTimer, timeout(), this, OnTickTimer());
    CONNECTSIGNALSLOT(this, SigShutDownDevice(), this, OnShutDownDevice());
    CONNECTSIGNALSLOT(this, SigNotifySavedServiceInfor(const QString&), this, OnNotifySavedServiceInfor(const QString&));
    CONNECTSIGNALSLOT(this, SigResetActiveCarbonFilterLifetime(quint32), this, OnResetActiveCarbonFilterLifetime(quint32));
    m_TickTimer.setInterval(500);
    m_TickTimer.start();
    if(dynamic_cast<DeviceControl::DeviceControlSim*>(mp_IDeviceProcessing) != NULL)
    {
        auto pSim = dynamic_cast<DeviceControl::DeviceControlSim*>(mp_IDeviceProcessing);
        pSim->Start();
    }
}

template <class DP>
void SchedulerCommandProcessor<DP>::OnTickTimer4Slot()
{
    Scheduler::SchedulerCommandShPtr_t scmd;
    while (newCmdComing(scmd))
    {
        this->ExecuteCmd(scmd);
        if (scmd->GetResponse())
        {
            mp_SchedulerThreadController->PushDeviceControlCmdQueue(scmd);
        }
    }
}

template <class DP>
void SchedulerCommandProcessor<DP>:: pushCmd4Slot(CmdSchedulerCommandBase* cmd, bool response)
{
    if (response)
    {
        cmd->SetResponse(true);
    }
    else
    {
        cmd->SetResponse(false);
    }
    m_CmdMutex.lock();
    m_Cmds.push_back(Scheduler::SchedulerCommandShPtr_t(cmd));
    m_CmdMutex.unlock();
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

    Retorts* retorts = mp_IDeviceProcessing->GetRetorts();
    foreach (auto retort, retorts->m_Retorts)
    {
        m_Sender << retort.Name;
    }
    emit DCLConfigurationFinished(hdlInfo, m_Sender);
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
    QString sender = scmd.data()->GetSender();

        if ("Scheduler::StartConfigurationService" == cmdName)
        {
            std::future<ReturnCode_t> f = std::async(std::launch::async, [this](){return this->mp_IDeviceProcessing->StartConfigurationService();});
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
            scmd->SetResult(mp_IDeviceProcessing->WithSender(sender)->ALSetPressureCtrlON());
            mp_SchedulerThreadController->LogDebug("==CMD==:Set pressure ctrl ON.");
        }
        else if  ("Scheduler::ALSetPressureCtrlOFF" == cmdName)
        {
            scmd->SetResult(mp_IDeviceProcessing->WithSender(sender)->ALSetPressureCtrlOFF());
            mp_SchedulerThreadController->LogDebug("==CMD==:Set pressure ctrl OFF.");
        }
        else if  ("Scheduler::ALReleasePressure" == cmdName)
        {
            scmd->SetResult(mp_IDeviceProcessing->WithSender(sender)->ALReleasePressure());
            mp_SchedulerThreadController->LogDebug("==CMD==:Release pressure.");
        }
        else if  ("Scheduler::ALPressure" == cmdName)
        {
            scmd->SetResult(mp_IDeviceProcessing->WithSender(sender)->ALPressure(qSharedPointerDynamicCast<CmdALPressure>(scmd)->GetTargetPressure()));
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:Set pressure [%1]").arg(qSharedPointerDynamicCast<CmdALPressure>(scmd)->GetTargetPressure()));
        }
        else if  ("Scheduler::ALVaccum" == cmdName)
        {
            scmd->SetResult(mp_IDeviceProcessing->WithSender(sender)->ALVaccum(qSharedPointerDynamicCast<CmdALVaccum>(scmd)->GetTargetPressure()));
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:Set vacuum [%1]").arg(qSharedPointerDynamicCast<CmdALVaccum>(scmd)->GetTargetPressure()));
        }
        else if  ("Scheduler::ALDraining" == cmdName)
        {
            scmd->SetResult(mp_IDeviceProcessing->WithSender(sender)->ALDraining(qSharedPointerDynamicCast<CmdALDraining>(scmd)->GetDelayTime(), qSharedPointerDynamicCast<CmdALDraining>(scmd)->GetDrainPressure(), qSharedPointerDynamicCast<CmdALDraining>(scmd)->GetIgnorePressure()));
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:Draining, delay time:%1, target pressure:%2, ingnore pressure:%3")
                                                   .arg(qSharedPointerDynamicCast<CmdALDraining>(scmd)->GetDelayTime())
                                                   .arg(qSharedPointerDynamicCast<CmdALDraining>(scmd)->GetDrainPressure())
                                                   .arg(qSharedPointerDynamicCast<CmdALDraining>(scmd)->GetIgnorePressure()));
        }
        else if  ("Scheduler::IDForceDraining" == cmdName)
        {
            scmd->SetResult(mp_IDeviceProcessing->WithSender(sender)->IDForceDraining(qSharedPointerDynamicCast<CmdIDForceDraining>(scmd)->GetRVPosition(), qSharedPointerDynamicCast<CmdIDForceDraining>(scmd)->GetDrainPressure(), qSharedPointerDynamicCast<CmdIDForceDraining>(scmd)->GetReagentGrpID() ));
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:Force drainging, RV pos:%1, target pressure:%2, reagent ID:%3")
                                                   .arg(qSharedPointerDynamicCast<CmdIDForceDraining>(scmd)->GetRVPosition())
                                                   .arg(qSharedPointerDynamicCast<CmdIDForceDraining>(scmd)->GetDrainPressure())
                                                   .arg(qSharedPointerDynamicCast<CmdIDForceDraining>(scmd)->GetReagentGrpID()) );
        }
        else if  ("Scheduler::ALFilling" == cmdName)
        {
            scmd->SetResult(mp_IDeviceProcessing->WithSender(sender)->ALFilling(qSharedPointerDynamicCast<CmdALFilling>(scmd)->GetDelayTime(),
                                                                    qSharedPointerDynamicCast<CmdALFilling>(scmd)->GetEnableInsufficientCheck(),
                                                                    qSharedPointerDynamicCast<CmdALFilling>(scmd)->GetSafeReagent4Paraffin()));
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:Filling, delay time:%1, insufficient check:%2, SafeReagent4Paraffin: %3")
                                                   .arg(qSharedPointerDynamicCast<CmdALFilling>(scmd)->GetDelayTime())
                                                   .arg(qSharedPointerDynamicCast<CmdALFilling>(scmd)->GetEnableInsufficientCheck())
                                                   .arg(qSharedPointerDynamicCast<CmdALFilling>(scmd)->GetSafeReagent4Paraffin()));
        }
        else if ("Scheduler::ALStopCmdExec" == cmdName)
        {
            scmd->SetResult(mp_IDeviceProcessing->WithSender(sender)->ALStopCmdExec(qSharedPointerDynamicCast<CmdALStopCmdExec>(scmd)->GetCmdType()));
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:Stop cmd execute type:%1").arg(qSharedPointerDynamicCast<CmdALStopCmdExec>(scmd)->GetCmdType()) );
        }
        else if  ("Scheduler::ALGetRecentPressure" == cmdName)
        {
            scmd->SetResult(mp_IDeviceProcessing->WithSender(sender)->ALGetRecentPressure());
        }
        else if  ("Scheduler::ALSetTempCtrlON" == cmdName)
        {
            scmd->SetResult(mp_IDeviceProcessing->WithSender(sender)->ALSetTempCtrlON(qSharedPointerDynamicCast<CmdALSetTempCtrlON>(scmd)->GetType()));
            mp_SchedulerThreadController->LogDebug("==CMD==:AL Set temp ctrl ON.");
        }
        else if  ("Scheduler::ALSetTempCtrlOFF" == cmdName)
        {
            scmd->SetResult(mp_IDeviceProcessing->WithSender(sender)->ALSetTempCtrlOFF(qSharedPointerDynamicCast<CmdALSetTempCtrlOFF>(scmd)->Gettype()));
            mp_SchedulerThreadController->LogDebug("==CMD==:AL Set temp ctrl OFF.");
        }
        else if  ("Scheduler::ALSetTemperaturePid" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->ALSetTemperaturePid( qSharedPointerDynamicCast<CmdALSetTemperaturePid>(scmd)->GetType(),
                                                                                qSharedPointerDynamicCast<CmdALSetTemperaturePid>(scmd)->GetMaxTemperature(),
                                                                                qSharedPointerDynamicCast<CmdALSetTemperaturePid>(scmd)->GetControllerGain(),
                                                                                qSharedPointerDynamicCast<CmdALSetTemperaturePid>(scmd)->GetResetTime(),
                                                                                qSharedPointerDynamicCast<CmdALSetTemperaturePid>(scmd)->GetDerivativeTime()
                                                                               ) );
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:AL set temp pid, type:%1, MaxTemp:%2, P:%3, I:%4, D:%5.")
                                                   .arg(qSharedPointerDynamicCast<CmdALSetTemperaturePid>(scmd)->GetType())
                                                   .arg(qSharedPointerDynamicCast<CmdALSetTemperaturePid>(scmd)->GetMaxTemperature())
                                                   .arg(qSharedPointerDynamicCast<CmdALSetTemperaturePid>(scmd)->GetControllerGain())
                                                   .arg(qSharedPointerDynamicCast<CmdALSetTemperaturePid>(scmd)->GetResetTime())
                                                   .arg(qSharedPointerDynamicCast<CmdALSetTemperaturePid>(scmd)->GetDerivativeTime()) );
        }
        else if  ("Scheduler::ALStartTemperatureControl" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->ALStartTemperatureControl( qSharedPointerDynamicCast<CmdALStartTemperatureControl>(scmd)->GetType(),
                                                                                      qSharedPointerDynamicCast<CmdALStartTemperatureControl>(scmd)->GetNominalTemperature(),
                                                                                      qSharedPointerDynamicCast<CmdALStartTemperatureControl>(scmd)->GetSlopeTempChange()
                                                                                    ) );
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:AL StartTempCtrl, type:%1, temp:%2, slope:%3.")
                                                   .arg(qSharedPointerDynamicCast<CmdALStartTemperatureControl>(scmd)->GetType())
                                                   .arg(qSharedPointerDynamicCast<CmdALStartTemperatureControl>(scmd)->GetNominalTemperature())
                                                   .arg(qSharedPointerDynamicCast<CmdALStartTemperatureControl>(scmd)->GetSlopeTempChange()) );
        }
        else if  ("Scheduler::ALGetRecentTemperature" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->ALGetRecentTemperature( qSharedPointerDynamicCast<CmdALGetRecentTemperature>(scmd)->GetType(),
                                                                                   qSharedPointerDynamicCast<CmdALGetRecentTemperature>(scmd)->GetIndex()
                                                                                 ) );
        }
        else if  ("Scheduler::ALGetTemperatureControlState" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->ALGetTemperatureControlState( qSharedPointerDynamicCast<CmdALGetTemperatureControlState>(scmd)->GetType() ) );
        }
        else if  ("Scheduler::ALTurnOnFan" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->ALTurnOnFan() );
            mp_SchedulerThreadController->LogDebug("==CMD==:AL turn on fan.");
        }
        else if  ("Scheduler::ALTurnOffFan" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->ALTurnOffFan() );
            mp_SchedulerThreadController->LogDebug("==CMD==:AL turn off fan.");
        }
        else if  ("Scheduler::RVSetTempCtrlON" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RVSetTempCtrlON() );
            mp_SchedulerThreadController->LogDebug("==CMD==:RV set temp ctrl ON.");
        }
        else if  ("Scheduler::RVSetTempCtrlOFF" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RVSetTempCtrlOFF() );
            mp_SchedulerThreadController->LogDebug("==CMD==:RV set temp ctrl OFF.");
        }
        else if  ("Scheduler::RVSetTemperaturePid" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RVSetTemperaturePid( qSharedPointerDynamicCast<CmdRVSetTemperaturePid>(scmd)->GetMaxTemperature(),
                                                                                qSharedPointerDynamicCast<CmdRVSetTemperaturePid>(scmd)->GetControllerGain(),
                                                                                qSharedPointerDynamicCast<CmdRVSetTemperaturePid>(scmd)->GetResetTime(),
                                                                                qSharedPointerDynamicCast<CmdRVSetTemperaturePid>(scmd)->GetDerivativeTime()
                                                                              ) );
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:RV set temp, MaxTemp:%1, P:%2, I:%3, D:%4.")
                                                   .arg(qSharedPointerDynamicCast<CmdRVSetTemperaturePid>(scmd)->GetMaxTemperature())
                                                   .arg(qSharedPointerDynamicCast<CmdRVSetTemperaturePid>(scmd)->GetControllerGain())
                                                   .arg(qSharedPointerDynamicCast<CmdRVSetTemperaturePid>(scmd)->GetResetTime())
                                                   .arg(qSharedPointerDynamicCast<CmdRVSetTemperaturePid>(scmd)->GetDerivativeTime()) );
        }
        else if  ("Scheduler::RVStartTemperatureControl" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RVStartTemperatureControl( qSharedPointerDynamicCast<CmdRVStartTemperatureControl>(scmd)->GetNominalTemperature(),
                                                                                      qSharedPointerDynamicCast<CmdRVStartTemperatureControl>(scmd)->GetSlopeTempChange()
                                                                                    ) );
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:RV StartTempCtrl, temp:%1, slope:%2")
                                                   .arg(qSharedPointerDynamicCast<CmdRVStartTemperatureControl>(scmd)->GetNominalTemperature())
                                                   .arg(qSharedPointerDynamicCast<CmdRVStartTemperatureControl>(scmd)->GetSlopeTempChange()) );
        }
        else if  ("Scheduler::RVGetRecentTemperature" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RVGetRecentTemperature( qSharedPointerDynamicCast<CmdRVGetRecentTemperature>(scmd)->GetIndex() ) );
        }
        else if  ("Scheduler::RVGetTemperatureControlState" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RVGetTemperatureControlState() );
        }
        else if  ("Scheduler::RVReqMoveToInitialPosition" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RVReqMoveToInitialPosition( qSharedPointerDynamicCast<CmdRVReqMoveToInitialPosition>(scmd)->GetRVPosition() ) );
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:Move RV to init pos, target pos:%1")
                                                   .arg(qSharedPointerDynamicCast<CmdRVReqMoveToInitialPosition>(scmd)->GetRVPosition()) );
        }
        else if  ("Scheduler::RVReqMoveToRVPosition" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RVReqMoveToRVPosition( qSharedPointerDynamicCast<CmdRVReqMoveToRVPosition>(scmd)->GetRVPosition(),
                                                                                              qSharedPointerDynamicCast<CmdRVReqMoveToRVPosition>(scmd)->GetCheckAbsolutePosition()) );
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:Move RV to pos:%1").arg(qSharedPointerDynamicCast<CmdRVReqMoveToRVPosition>(scmd)->GetRVPosition(),
                                                                                            qSharedPointerDynamicCast<CmdRVReqMoveToRVPosition>(scmd)->GetCheckAbsolutePosition()));
        }
        else if  ("Scheduler::RVReqMoveToCurrentTubePosition" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RVReqMoveToCurrentTubePosition( qSharedPointerDynamicCast<CmdRVReqMoveToCurrentTubePosition>(scmd)->GetRVPosition() ) );
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:Move RV to current tube pos:%1").arg(qSharedPointerDynamicCast<CmdRVReqMoveToCurrentTubePosition>(scmd)->GetRVPosition()));
        }
        else if  ("Scheduler::RVReqActRVPosition" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RVReqActRVPosition() );
        }
        else if  ("Scheduler::OvenSetTempCtrlON" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->OvenSetTempCtrlON(qSharedPointerDynamicCast<CmdOvenSetTempCtrlON>(scmd)->GetType() ) );
            mp_SchedulerThreadController->LogDebug("==CMD==:Oven set temp ctrl ON.");
        }
        else if  ("Scheduler::OvenSetTempCtrlOFF" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->OvenSetTempCtrlOFF(qSharedPointerDynamicCast<CmdOvenSetTempCtrlOFF>(scmd)->Gettype() ) );
            mp_SchedulerThreadController->LogDebug("==CMD==:Oven set temp ctrl OFF.");
        }
        else if  ("Scheduler::OvenSetTemperaturePid" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->OvenSetTemperaturePid( qSharedPointerDynamicCast<CmdOvenSetTemperaturePid>(scmd)->GetType(),
                                                                                  qSharedPointerDynamicCast<CmdOvenSetTemperaturePid>(scmd)->GetMaxTemperature(),
                                                                                  qSharedPointerDynamicCast<CmdOvenSetTemperaturePid>(scmd)->GetControllerGain(),
                                                                                  qSharedPointerDynamicCast<CmdOvenSetTemperaturePid>(scmd)->GetResetTime(),
                                                                                  qSharedPointerDynamicCast<CmdOvenSetTemperaturePid>(scmd)->GetDerivativeTime()
                                                                                ) );
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:Oven set temp, type:%1, MaxTemp:%2, P:%3, I:%4, D:%5.")
                                                   .arg(qSharedPointerDynamicCast<CmdOvenSetTemperaturePid>(scmd)->GetType())
                                                   .arg(qSharedPointerDynamicCast<CmdOvenSetTemperaturePid>(scmd)->GetMaxTemperature())
                                                   .arg(qSharedPointerDynamicCast<CmdOvenSetTemperaturePid>(scmd)->GetControllerGain())
                                                   .arg(qSharedPointerDynamicCast<CmdOvenSetTemperaturePid>(scmd)->GetResetTime())
                                                   .arg(qSharedPointerDynamicCast<CmdOvenSetTemperaturePid>(scmd)->GetDerivativeTime()) );
        }
        else if  ("Scheduler::OvenStartTemperatureControl" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->OvenStartTemperatureControl( qSharedPointerDynamicCast<CmdOvenStartTemperatureControl>(scmd)->GetType(),
                                                                                        qSharedPointerDynamicCast<CmdOvenStartTemperatureControl>(scmd)->GetNominalTemperature(),
                                                                                        qSharedPointerDynamicCast<CmdOvenStartTemperatureControl>(scmd)->GetSlopeTempChange()
                                                                                      ) );
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:Oven StartTempCtrl, type:%1, temp:%2, slope:%3")
                                                   .arg(qSharedPointerDynamicCast<CmdOvenStartTemperatureControl>(scmd)->GetType())
                                                   .arg(qSharedPointerDynamicCast<CmdOvenStartTemperatureControl>(scmd)->GetNominalTemperature())
                                                   .arg(qSharedPointerDynamicCast<CmdOvenStartTemperatureControl>(scmd)->GetSlopeTempChange()) );
        }
        else if  ("Scheduler::OvenGetRecentTemperature" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->OvenGetRecentTemperature( qSharedPointerDynamicCast<CmdOvenGetRecentTemperature>(scmd)->GetType(),
                                                                                     qSharedPointerDynamicCast<CmdOvenGetRecentTemperature>(scmd)->GetIndex() ) );
        }
        else if  ("Scheduler::OvenGetTemperatureControlState" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->OvenGetTemperatureControlState( qSharedPointerDynamicCast<CmdOvenGetTemperatureControlState>(scmd)->GetType() ) );
        }
        else if  ("Scheduler::RTSetTempCtrlON" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RTSetTempCtrlON( qSharedPointerDynamicCast<CmdRTSetTempCtrlON>(scmd)->GetType() ) );
            mp_SchedulerThreadController->LogDebug("==CMD==:RT set temp ctrl ON.");
        }
        else if  ("Scheduler::RTSetTempCtrlOFF" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RTSetTempCtrlOFF( qSharedPointerDynamicCast<CmdRTSetTempCtrlOFF>(scmd)->GetType() ) );
            mp_SchedulerThreadController->LogDebug("==CMD==:RT set temp ctrl OFF.");
        }
        else if  ("Scheduler::RTSetTemperaturePid" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RTSetTemperaturePid( qSharedPointerDynamicCast<CmdRTSetTemperaturePid>(scmd)->GetType(),
                                                                                qSharedPointerDynamicCast<CmdRTSetTemperaturePid>(scmd)->GetMaxTemperature(),
                                                                                qSharedPointerDynamicCast<CmdRTSetTemperaturePid>(scmd)->GetControllerGain(),
                                                                                qSharedPointerDynamicCast<CmdRTSetTemperaturePid>(scmd)->GetResetTime(),
                                                                                qSharedPointerDynamicCast<CmdRTSetTemperaturePid>(scmd)->GetDerivativeTime()
                                                                              ) );
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:RT set temp, type:%1, MaxTemp:%2, P:%3, I:%4, D:%5.")
                                                   .arg(qSharedPointerDynamicCast<CmdRTSetTemperaturePid>(scmd)->GetType())
                                                   .arg(qSharedPointerDynamicCast<CmdRTSetTemperaturePid>(scmd)->GetMaxTemperature())
                                                   .arg(qSharedPointerDynamicCast<CmdRTSetTemperaturePid>(scmd)->GetControllerGain())
                                                   .arg(qSharedPointerDynamicCast<CmdRTSetTemperaturePid>(scmd)->GetResetTime())
                                                   .arg(qSharedPointerDynamicCast<CmdRTSetTemperaturePid>(scmd)->GetDerivativeTime()) );
        }
        else if  ("Scheduler::RTStartTemperatureControl" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RTStartTemperatureControl( qSharedPointerDynamicCast<CmdRTStartTemperatureControl>(scmd)->GetType(),
                                                                                      qSharedPointerDynamicCast<CmdRTStartTemperatureControl>(scmd)->GetNominalTemperature(),
                                                                                      qSharedPointerDynamicCast<CmdRTStartTemperatureControl>(scmd)->GetSlopeTempChange()
                                                                                    ) );
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:RT StartTempCtrl, type:%1, temp:%2, slope:%3.")
                                                   .arg(qSharedPointerDynamicCast<CmdRTStartTemperatureControl>(scmd)->GetType())
                                                   .arg(qSharedPointerDynamicCast<CmdRTStartTemperatureControl>(scmd)->GetNominalTemperature())
                                                   .arg(qSharedPointerDynamicCast<CmdRTStartTemperatureControl>(scmd)->GetSlopeTempChange()) );
        }
        else if  ("Scheduler::RTGetRecentTemperature" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RTGetRecentTemperature( qSharedPointerDynamicCast<CmdRTGetRecentTemperature>(scmd)->GetType(),
                                                                                   qSharedPointerDynamicCast<CmdRTGetRecentTemperature>(scmd)->GetIndex()
                                                                                 ) );
        }
        else if  ("Scheduler::RTGetTemperatureControlState" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RTGetTemperatureControlState( qSharedPointerDynamicCast<CmdRTGetTemperatureControlState>(scmd)->GetType()) );
        }
        else if  ("Scheduler::RTUnlock" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RTUnlock() );
        }
        else if  ("Scheduler::RTLock" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RTLock() );
        }
        else if  ("Scheduler::PerTurnOffMainRelay" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->PerTurnOffMainRelay() );
            mp_SchedulerThreadController->LogDebug("==CMD==:Turn off main relay.");
        }
        else if  ("Scheduler::PerTurnOnMainRelay" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->PerTurnOnMainRelay() );
            mp_SchedulerThreadController->LogDebug("==CMD==:Turn on main relay.");
        }
        else if  ("Scheduler::RVStartTemperatureControlWithPID" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RVStartTemperatureControlWithPID( qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(scmd)->GetNominalTemperature(),
                                                                                             qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(scmd)->GetSlopeTempChange(),
                                                                                             qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(scmd)->GetMaxTemperature(),
                                                                                             qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(scmd)->GetControllerGain(),
                                                                                             qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(scmd)->GetResetTime(),
                                                                                             qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(scmd)->GetDerivativeTime()
                                                                                           ) );
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:RV StartTempCtrl, temp:%1, slope:%2, MaxTemp:%3, P:%4, I:%5, D:%6.")
                                                   .arg(qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(scmd)->GetNominalTemperature())
                                                   .arg(qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(scmd)->GetSlopeTempChange())
                                                   .arg(qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(scmd)->GetMaxTemperature())
                                                   .arg(qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(scmd)->GetControllerGain())
                                                   .arg(qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(scmd)->GetResetTime())
                                                   .arg(qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(scmd)->GetDerivativeTime()) );
        }
        else if  ("Scheduler::OvenStartTemperatureControlWithPID" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->OvenStartTemperatureControlWithPID( qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(scmd)->GetType(),
                                                                                               qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(scmd)->GetNominalTemperature(),
                                                                                               qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(scmd)->GetSlopeTempChange(),
                                                                                               qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(scmd)->GetMaxTemperature(),
                                                                                               qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(scmd)->GetControllerGain(),
                                                                                               qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(scmd)->GetResetTime(),
                                                                                               qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(scmd)->GetDerivativeTime()
                                                                                            ) );
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:Oven StartTempCtrl, type:%1, temp:%2, slope:%3, Max:%4, P:%5, I:%6, D:%7.")
                                                   .arg(qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(scmd)->GetType())
                                                   .arg(qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(scmd)->GetNominalTemperature())
                                                   .arg(qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(scmd)->GetSlopeTempChange())
                                                   .arg(qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(scmd)->GetMaxTemperature())
                                                   .arg(qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(scmd)->GetControllerGain())
                                                   .arg(qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(scmd)->GetResetTime())
                                                   .arg(qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(scmd)->GetDerivativeTime()) );
        }
        else if  ("Scheduler::RTStartTemperatureControlWithPID" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RTStartTemperatureControlWithPID( qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(scmd)->GetType(),
                                                                                             qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(scmd)->GetNominalTemperature(),
                                                                                             qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(scmd)->GetSlopeTempChange(),
                                                                                             qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(scmd)->GetMaxTemperature(),
                                                                                             qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(scmd)->GetControllerGain(),
                                                                                             qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(scmd)->GetResetTime(),
                                                                                             qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(scmd)->GetDerivativeTime()
                                                                                            ) );
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:RT StartTempCtrl, type:%1, temp:%2, slope:%3, MaxTemp:%4, P:%5, I:%6, D:%7.")
                                                   .arg(qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(scmd)->GetType())
                                                   .arg(qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(scmd)->GetNominalTemperature())
                                                   .arg(qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(scmd)->GetSlopeTempChange())
                                                   .arg(qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(scmd)->GetMaxTemperature())
                                                   .arg(qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(scmd)->GetControllerGain())
                                                   .arg(qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(scmd)->GetResetTime())
                                                   .arg(qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(scmd)->GetDerivativeTime()) );
        }
        else if  ("Scheduler::ALStartTemperatureControlWithPID" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->ALStartTemperatureControlWithPID( qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(scmd)->GetType(),
                                                                                             qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(scmd)->GetNominalTemperature(),
                                                                                             qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(scmd)->GetSlopeTempChange(),
                                                                                             qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(scmd)->GetMaxTemperature(),
                                                                                             qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(scmd)->GetControllerGain(),
                                                                                             qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(scmd)->GetResetTime(),
                                                                                             qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(scmd)->GetDerivativeTime()
                                                                                           ) );
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:AL StartTempCtrl, type:%1, temp:%2, slope:%3, MaxTemp:%4, P:%5, I:%6, D:%7.")
                                                   .arg(qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(scmd)->GetType())
                                                   .arg(qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(scmd)->GetNominalTemperature())
                                                   .arg(qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(scmd)->GetSlopeTempChange())
                                                   .arg(qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(scmd)->GetMaxTemperature())
                                                   .arg(qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(scmd)->GetControllerGain())
                                                   .arg(qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(scmd)->GetResetTime())
                                                   .arg(qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(scmd)->GetDerivativeTime()) );
        }
        else if  ("Scheduler::IDBottleCheck" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->IDBottleCheck( qSharedPointerDynamicCast<CmdIDBottleCheck>(scmd)->GetReagentGrpID(),
                                                                          qSharedPointerDynamicCast<CmdIDBottleCheck>(scmd)->GetTubePos() ) );
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:Bottle check, reagent group id:%1, tube pos:%2.")
                                                   .arg(qSharedPointerDynamicCast<CmdIDBottleCheck>(scmd)->GetReagentGrpID())
                                                   .arg(qSharedPointerDynamicCast<CmdIDBottleCheck>(scmd)->GetTubePos()) );
        }
        else if  ("Scheduler::ALAllStop" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->ALAllStop() );
            mp_SchedulerThreadController->LogDebug("==CMD==:All stop.");
        }
        else if ("Scheduler::IDSealingCheck" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->IDSealingCheck(qSharedPointerDynamicCast<CmdIDSealingCheck>(scmd)->GetThresholdPressure(),
                                                                  qSharedPointerDynamicCast<CmdIDSealingCheck>(scmd)->GetSealPosition() ) );
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:sealing check, threshold pressure:%1, seal pos:%2.")
                                                   .arg(qSharedPointerDynamicCast<CmdIDSealingCheck>(scmd)->GetThresholdPressure())
                                                   .arg(qSharedPointerDynamicCast<CmdIDSealingCheck>(scmd)->GetSealPosition()) );
        }
        else if ("Scheduler::RmtLocAlarm" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->IDSetAlarm(qSharedPointerDynamicCast<CmdRmtLocAlarm>(scmd)->GetRmtLocOpcode()));
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:set alarm type:%1.").arg(qSharedPointerDynamicCast<CmdRmtLocAlarm>(scmd)->GetRmtLocOpcode()) );
        }
        else if ("Scheduler::ALControlValve" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->ALControlValve(qSharedPointerDynamicCast<CmdALControlValve>(scmd)->GetValveIndex(),
                             qSharedPointerDynamicCast<CmdALControlValve>(scmd)->GetValveState()) );
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:AL control valve, index:%1, state:%2")
                                                   .arg(qSharedPointerDynamicCast<CmdALControlValve>(scmd)->GetValveIndex())
                                                   .arg(qSharedPointerDynamicCast<CmdALControlValve>(scmd)->GetValveState()) );
        }
        else if ("Scheduler::RVSetTemperatureSwitchState" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RVSetTemperatureSwitchState(qSharedPointerDynamicCast<CmdRVSetTemperatureSwitchState>(scmd)->GetHeaterVoltage(),
                                                                               qSharedPointerDynamicCast<CmdRVSetTemperatureSwitchState>(scmd)->GetAutoType()) );
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:ASB3 set temp switch state, voltage:%1, auto type:%2.")
                                                   .arg(qSharedPointerDynamicCast<CmdRVSetTemperatureSwitchState>(scmd)->GetHeaterVoltage())
                                                   .arg(qSharedPointerDynamicCast<CmdRVSetTemperatureSwitchState>(scmd)->GetAutoType()) );
        }
        else if ("Scheduler::RTSetTemperatureSwitchState" == cmdName)
        {
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->RTSetTemperatureSwitchState( qSharedPointerDynamicCast<CmdRTSetTemperatureSwitchState>(scmd)->GetType(),
                                                                                qSharedPointerDynamicCast<CmdRTSetTemperatureSwitchState>(scmd)->GetHeaterVoltage(),
                                                                                qSharedPointerDynamicCast<CmdRTSetTemperatureSwitchState>(scmd)->GetAutoType()) );
            mp_SchedulerThreadController->LogDebug(QString("==CMD==:ASB5 set temp switch state, voltage:%1, auto type:%2.")
                                                   .arg(qSharedPointerDynamicCast<CmdRTSetTemperatureSwitchState>(scmd)->GetHeaterVoltage())
                                                   .arg(qSharedPointerDynamicCast<CmdRTSetTemperatureSwitchState>(scmd)->GetAutoType()) );
        }
        else if ("Scheduler::ALPurge" == cmdName)
        {
            //scmd->SetResult( DCL_ERR_FCT_CALL_SUCCESS );
            scmd->SetResult( mp_IDeviceProcessing->WithSender(sender)->ALPurge(qSharedPointerDynamicCast<CmdALPurge>(scmd)->GetPurgeTimes()));
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
void SchedulerCommandProcessor<DP>::ResetActiveCarbonFilterLifetime(quint32 setVal)
{
    emit SigResetActiveCarbonFilterLifetime(setVal);
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
        auto sender = "RetortA";
        mp_IDeviceProcessing->WithSender(sender)->RVSetTempCtrlOFF();
        mp_IDeviceProcessing->WithSender(sender)->OvenSetTempCtrlOFF(OVEN_TOP);
        mp_IDeviceProcessing->WithSender(sender)->OvenSetTempCtrlOFF(OVEN_BOTTOM);
        mp_IDeviceProcessing->WithSender(sender)->ALSetTempCtrlOFF(AL_TUBE1);
        mp_IDeviceProcessing->WithSender(sender)->ALSetTempCtrlOFF(AL_TUBE2);
        mp_IDeviceProcessing->WithSender(sender)->ALSetTempCtrlOFF(AL_LEVELSENSOR1);
        mp_IDeviceProcessing->WithSender(sender)->RTSetTempCtrlOFF(RT_BOTTOM);
        mp_IDeviceProcessing->WithSender(sender)->RTSetTempCtrlOFF(RT_SIDE);
        mp_IDeviceProcessing->WithSender(sender)->PerTurnOffMainRelay();

        mp_IDeviceProcessing->WithSender(sender)->RVGetTemperatureControlState();
        mp_IDeviceProcessing->WithSender(sender)->OvenGetTemperatureControlState(OVEN_TOP);
        mp_IDeviceProcessing->WithSender(sender)->OvenGetTemperatureControlState(OVEN_BOTTOM);
        mp_IDeviceProcessing->WithSender(sender)->ALGetTemperatureControlState(AL_TUBE1);
        mp_IDeviceProcessing->WithSender(sender)->ALGetTemperatureControlState(AL_TUBE2);
        mp_IDeviceProcessing->WithSender(sender)->ALGetTemperatureControlState(AL_LEVELSENSOR1);
        mp_IDeviceProcessing->WithSender(sender)->RTGetTemperatureControlState(RT_BOTTOM);
        mp_IDeviceProcessing->WithSender(sender)->RTGetTemperatureControlState(RT_SIDE);

        mp_IDeviceProcessing->WithSender(sender)->ALTurnOffFan();

        sender = "RetortB";
        mp_IDeviceProcessing->WithSender(sender)->RVSetTempCtrlOFF();
        mp_IDeviceProcessing->WithSender(sender)->OvenSetTempCtrlOFF(OVEN_TOP);
        mp_IDeviceProcessing->WithSender(sender)->OvenSetTempCtrlOFF(OVEN_BOTTOM);
        mp_IDeviceProcessing->WithSender(sender)->ALSetTempCtrlOFF(AL_TUBE1);
        mp_IDeviceProcessing->WithSender(sender)->ALSetTempCtrlOFF(AL_TUBE2);
        mp_IDeviceProcessing->WithSender(sender)->ALSetTempCtrlOFF(AL_LEVELSENSOR2);
        mp_IDeviceProcessing->WithSender(sender)->RTSetTempCtrlOFF(RT_BOTTOM);
        mp_IDeviceProcessing->WithSender(sender)->RTSetTempCtrlOFF(RT_SIDE);
        mp_IDeviceProcessing->WithSender(sender)->PerTurnOffMainRelay();

        mp_IDeviceProcessing->WithSender(sender)->RVGetTemperatureControlState();
        mp_IDeviceProcessing->WithSender(sender)->OvenGetTemperatureControlState(OVEN_TOP);
        mp_IDeviceProcessing->WithSender(sender)->OvenGetTemperatureControlState(OVEN_BOTTOM);
        mp_IDeviceProcessing->WithSender(sender)->ALGetTemperatureControlState(AL_TUBE1);
        mp_IDeviceProcessing->WithSender(sender)->ALGetTemperatureControlState(AL_TUBE2);
        mp_IDeviceProcessing->WithSender(sender)->ALGetTemperatureControlState(AL_LEVELSENSOR2);
        mp_IDeviceProcessing->WithSender(sender)->RTGetTemperatureControlState(RT_BOTTOM);
        mp_IDeviceProcessing->WithSender(sender)->RTGetTemperatureControlState(RT_SIDE);

        mp_IDeviceProcessing->WithSender(sender)->ALTurnOffFan();

        mp_IDeviceProcessing->Destroy();     // trigger shutdown of device processing layer
    }
}

template <class DP>
void SchedulerCommandProcessor<DP>::OnNotifySavedServiceInfor4Slot(const QString& deviceType)
{
    mp_IDeviceProcessing->NotifySavedServiceInfor(deviceType);
}

template <class DP>
void SchedulerCommandProcessor<DP>::OnResetActiveCarbonFilterLifetime4Slot(quint32 setVal)
{
    mp_IDeviceProcessing->ResetActiveCarbonFilterLifeTime(setVal);
}

}// end of namespace Scheduler


#ifdef GOOGLE_MOCK
template class Scheduler::SchedulerCommandProcessor<DeviceControl::MockDeviceControl>;
#else
template class Scheduler::SchedulerCommandProcessor<DeviceControl::DeviceControlSim>;
template class Scheduler::SchedulerCommandProcessor<DeviceControl::IDeviceProcessing>;
#endif
