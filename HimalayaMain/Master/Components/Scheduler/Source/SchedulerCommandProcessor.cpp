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
#include "Scheduler/Commands/Include/CmdALFilling.h"
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
#include "Scheduler/Commands/Include/CmdALFilling.h"
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
    HardwareMonitor_t strctHWMonitor ={0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, RV_UNDEF, 0.0, 0.0, 0, 0};

	strctHWMonitor.PressureAL			= mp_IDeviceProcessing->ALGetRecentPressure();
	strctHWMonitor.TempALLevelSensor	= mp_IDeviceProcessing->ALGetRecentTemperature(AL_LEVELSENSOR, 0);
	strctHWMonitor.TempALTube1			= mp_IDeviceProcessing->ALGetRecentTemperature(AL_TUBE1,0);
	strctHWMonitor.TempALTube2			= mp_IDeviceProcessing->ALGetRecentTemperature(AL_TUBE2,0);
	strctHWMonitor.TempRV1				= mp_IDeviceProcessing->RVGetRecentTemperature(0);
	strctHWMonitor.TempRV2				= mp_IDeviceProcessing->RVGetRecentTemperature(1);
	strctHWMonitor.PositionRV			= mp_IDeviceProcessing->RVReqActRVPosition();
    strctHWMonitor.TempRTBottom1		= mp_IDeviceProcessing->RTGetRecentTemperature(RT_BOTTOM,0);
    strctHWMonitor.TempRTBottom2		= mp_IDeviceProcessing->RTGetRecentTemperature(RT_BOTTOM,1);
	strctHWMonitor.TempRTSide			= mp_IDeviceProcessing->RTGetRecentTemperature(RT_SIDE,0);
    strctHWMonitor.TempOvenBottom1		= mp_IDeviceProcessing->OvenGetRecentTemperature(OVEN_BOTTOM,0);
    strctHWMonitor.TempOvenBottom2		= mp_IDeviceProcessing->OvenGetRecentTemperature(OVEN_BOTTOM,1);
	strctHWMonitor.TempOvenTop			= mp_IDeviceProcessing->OvenGetRecentTemperature(OVEN_TOP,0);
	strctHWMonitor.OvenLidStatus		= mp_IDeviceProcessing->OvenGetRecentLidStatus();
	strctHWMonitor.RetortLockStatus		= mp_IDeviceProcessing->RTGetRecentLockStatus();

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
    CONNECTSIGNALSLOT(mp_IDeviceProcessing, ReportDestroyFinished(), this, DevProcDestroyAckn());

    CONNECTSIGNALSLOT(this, NewCmdAdded(), this, OnNewCmdAdded());
    /*CONNECTSIGNALSLOT(mp_IDeviceProcessing, ReportConfigurationFinished(DevInstanceID_t, ReturnCode_t),
                      this, DevProcConfigurationAckn(DevInstanceID_t, ReturnCode_t));
    CONNECTSIGNALSLOT(mp_IDeviceProcessing, ReportStartNormalOperationMode(DevInstanceID_t, ReturnCode_t),
                      this, DevProcStartNormalOpModeAckn(DevInstanceID_t, ReturnCode_t));
    CONNECTSIGNALSLOT(mp_IDeviceProcessing, ReportError(DevInstanceID_t, quint16, quint16, quint16, const QDateTime &),
                      this, ThrowError(DevInstanceID_t, quint16, quint16, quint16, const QDateTime &));
    CONNECTSIGNALSLOT(mp_IDeviceProcessing, ReportDestroyFinished(), this, DevProcDestroyAckn());
    */


    /*
        while(true)
        {
    qDebug()<< "this is second thread, id is: "<<QThread::currentThreadId();
            if(newCmdComing())
            {
        qDebug()<< "sec thread got msg! current thread id is: "<<QThread::currentThreadId();
                CmdSchedulerCommandBase *scmd = dynamic_cast<CmdSchedulerCommandBase *> (m_currentCmd.GetPointerToUserData());
                scmd->Execute();
    //            (&m_currentCmd)->Execute();
            }
        usleep(200*1000);
        }
    */
}

template <class DP>
void SchedulerCommandProcessor<DP>::OnNewCmdAdded4Slot()
{
    if(newCmdComing())
    {
        //qDebug()<< "sec thread got msg! current thread id is: "<<QThread::currentThreadId();
//        LOG_STR_ARG(STR_EXECUTE_COMMAND,Global::tTranslatableStringList()<<Global::TranslatableString(m_currentCmd->GetParameters()));
        this->ExecuteCmd();
        mp_SchedulerThreadController->PushDeviceControlCmdQueue(m_currentCmd);
//        LOG_STR_ARG(STR_EXECUTE_COMMAND_RESULT,Global::tTranslatableStringList()<<Global::TranslatableString(m_currentCmd->GetName())
//                    <<Global::TranslatableString(m_currentCmd->GetStrResult()));
    }
}

template <class DP>
void SchedulerCommandProcessor<DP>::pushCmd4Slot(CmdSchedulerCommandBase* cmd)
{
    m_CmdMutex.lock();
    m_Cmds.push_front(Scheduler::SchedulerCommandShPtr_t(cmd));
    m_CmdMutex.unlock();
    emit NewCmdAdded();
}

template <class DP>
bool SchedulerCommandProcessor<DP>::newCmdComing()
{
    bool ret = false;
    m_CmdMutex.lock();
    if(!m_Cmds.isEmpty())
    {
        ret = true;
       m_currentCmd =  m_Cmds.dequeue();
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
void SchedulerCommandProcessor<DP>::DevProcDestroyAckn4Slot()
{
}

template <class DP>
void SchedulerCommandProcessor<DP>::ExecuteCmd()
{
	QString cmdName = m_currentCmd->GetName();

	if ("Scheduler::StartConfigurationService" == cmdName)
	{
		m_currentCmd->SetResult(mp_IDeviceProcessing->StartConfigurationService());
	}
	else if ("Scheduler::RestartConfigurationService" == cmdName)
	{
		m_currentCmd->SetResult(mp_IDeviceProcessing->RestartConfigurationService());
	}
	else if  ("Scheduler::StartDiagnosticService" == cmdName)
	{
		m_currentCmd->SetResult(mp_IDeviceProcessing->StartDiagnosticService());
	}
	else if  ("Scheduler::CloseDiagnosticService" == cmdName)
	{
		m_currentCmd->SetResult(mp_IDeviceProcessing->CloseDiagnosticService());
	}
	else if  ("Scheduler::StartAdjustmentService" == cmdName)
	{
		m_currentCmd->SetResult(mp_IDeviceProcessing->StartAdjustmentService());
	}
	else if  ("Scheduler::ALSetPressureCtrlON" == cmdName)
	{
		m_currentCmd->SetResult(mp_IDeviceProcessing->ALSetPressureCtrlON());
	}
	else if  ("Scheduler::ALSetPressureCtrlOFF" == cmdName)
	{
		m_currentCmd->SetResult(mp_IDeviceProcessing->ALSetPressureCtrlOFF());
	}
	else if  ("Scheduler::ALReleasePressure" == cmdName)
	{
		m_currentCmd->SetResult(mp_IDeviceProcessing->ALReleasePressure());
	}
	else if  ("Scheduler::ALPressure" == cmdName)
	{
		m_currentCmd->SetResult(mp_IDeviceProcessing->ALPressure());
	}
	else if  ("Scheduler::ALVaccum" == cmdName)
	{
		m_currentCmd->SetResult(mp_IDeviceProcessing->ALVaccum());
	}
	else if  ("Scheduler::ALDraining" == cmdName)
	{
        m_currentCmd->SetResult(mp_IDeviceProcessing->ALDraining(qSharedPointerDynamicCast<CmdALDraining>(m_currentCmd)->GetDelayTime()));
	}
	else if  ("Scheduler::ALFilling" == cmdName)
	{
        m_currentCmd->SetResult(mp_IDeviceProcessing->ALFilling(qSharedPointerDynamicCast<CmdALFilling>(m_currentCmd)->GetDelayTime(),
                                                                qSharedPointerDynamicCast<CmdALFilling>(m_currentCmd)->GetEnableInsufficientCheck()));
	}
	else if  ("Scheduler::ALGetRecentPressure" == cmdName)
	{
        m_currentCmd->SetResult(mp_IDeviceProcessing->ALGetRecentPressure());
	}
	else if  ("Scheduler::ALSetTempCtrlON" == cmdName)
	{
        m_currentCmd->SetResult(mp_IDeviceProcessing->ALSetTempCtrlON(qSharedPointerDynamicCast<CmdALSetTempCtrlON>(m_currentCmd)->GetType()));
	}
	else if  ("Scheduler::ALSetTempCtrlOFF" == cmdName)
	{
        //m_currentCmd->SetResult(mp_IDeviceProcessing->ALSetTempCtrlOFF(dynamic_cast<CmdALSetTempCtrlOFF*>(m_currentCmd)->GetType()));
	}
	else if  ("Scheduler::ALSetTemperaturePid" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->ALSetTemperaturePid( qSharedPointerDynamicCast<CmdALSetTemperaturePid>(m_currentCmd)->GetType(),
                                                                            qSharedPointerDynamicCast<CmdALSetTemperaturePid>(m_currentCmd)->GetMaxTemperature(),
                                                                            qSharedPointerDynamicCast<CmdALSetTemperaturePid>(m_currentCmd)->GetControllerGain(),
                                                                            qSharedPointerDynamicCast<CmdALSetTemperaturePid>(m_currentCmd)->GetResetTime(),
                                                                            qSharedPointerDynamicCast<CmdALSetTemperaturePid>(m_currentCmd)->GetDerivativeTime()
																		   ) );
	}
	else if  ("Scheduler::ALStartTemperatureControl" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->ALStartTemperatureControl( qSharedPointerDynamicCast<CmdALStartTemperatureControl>(m_currentCmd)->GetType(),
                                                                                  qSharedPointerDynamicCast<CmdALStartTemperatureControl>(m_currentCmd)->GetNominalTemperature(),
                                                                                  qSharedPointerDynamicCast<CmdALStartTemperatureControl>(m_currentCmd)->GetSlopeTempChange()
																				) );
	}
	else if  ("Scheduler::ALGetRecentTemperature" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->ALGetRecentTemperature( qSharedPointerDynamicCast<CmdALGetRecentTemperature>(m_currentCmd)->GetType(),
                                                                               qSharedPointerDynamicCast<CmdALGetRecentTemperature>(m_currentCmd)->GetIndex()
																			 ) );
	}
	else if  ("Scheduler::ALGetTemperatureControlState" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->ALGetTemperatureControlState( qSharedPointerDynamicCast<CmdALGetTemperatureControlState>(m_currentCmd)->GetType() ) );
	}
	else if  ("Scheduler::ALTurnOnFan" == cmdName)
	{
		m_currentCmd->SetResult( mp_IDeviceProcessing->ALTurnOnFan() );
	}
	else if  ("Scheduler::ALTurnOffFan" == cmdName)
	{
		m_currentCmd->SetResult( mp_IDeviceProcessing->ALTurnOffFan() );
	}
	else if  ("Scheduler::RVSetTempCtrlON" == cmdName)
	{
		m_currentCmd->SetResult( mp_IDeviceProcessing->RVSetTempCtrlON() );
	}
	else if  ("Scheduler::RVSetTempCtrlOFF" == cmdName)
	{
		m_currentCmd->SetResult( mp_IDeviceProcessing->RVSetTempCtrlOFF() );
	}
	else if  ("Scheduler::RVSetTemperaturePid" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->RVSetTemperaturePid( qSharedPointerDynamicCast<CmdRVSetTemperaturePid>(m_currentCmd)->GetMaxTemperature(),
                                                                            qSharedPointerDynamicCast<CmdRVSetTemperaturePid>(m_currentCmd)->GetControllerGain(),
                                                                            qSharedPointerDynamicCast<CmdRVSetTemperaturePid>(m_currentCmd)->GetResetTime(),
                                                                            qSharedPointerDynamicCast<CmdRVSetTemperaturePid>(m_currentCmd)->GetDerivativeTime()
																		  ) );
	}
	else if  ("Scheduler::RVStartTemperatureControl" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->RVStartTemperatureControl( qSharedPointerDynamicCast<CmdRVStartTemperatureControl>(m_currentCmd)->GetNominalTemperature(),
                                                                                  qSharedPointerDynamicCast<CmdRVStartTemperatureControl>(m_currentCmd)->GetSlopeTempChange()
																				) );
	}
	else if  ("Scheduler::RVGetRecentTemperature" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->RVGetRecentTemperature( qSharedPointerDynamicCast<CmdRVGetRecentTemperature>(m_currentCmd)->GetIndex() ) );
	}
	else if  ("Scheduler::RVGetTemperatureControlState" == cmdName)
	{
		m_currentCmd->SetResult( mp_IDeviceProcessing->RVGetTemperatureControlState() );
	}
	else if  ("Scheduler::RVReqMoveToInitialPosition" == cmdName)
	{
		m_currentCmd->SetResult( mp_IDeviceProcessing->RVReqMoveToInitialPosition() );
	}
	else if  ("Scheduler::RVReqMoveToRVPosition" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->RVReqMoveToRVPosition( qSharedPointerDynamicCast<CmdRVReqMoveToRVPosition>(m_currentCmd)->GetRVPosition() ) );
	}
	else if  ("Scheduler::RVReqActRVPosition" == cmdName)
	{
		m_currentCmd->SetResult( mp_IDeviceProcessing->RVReqActRVPosition() );
	}
	else if  ("Scheduler::OvenSetTempCtrlON" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->OvenSetTempCtrlON(qSharedPointerDynamicCast<CmdOvenSetTempCtrlON>(m_currentCmd)->GetType() ) );
	}
	else if  ("Scheduler::OvenSetTempCtrlOFF" == cmdName)
	{
      //  m_currentCmd->SetResult( mp_IDeviceProcessing->OvenSetTempCtrlOFF(dynamic_cast<CmdOvenSetTempCtrlOFF*>(m_currentCmd)->GetType() ) );
	}
	else if  ("Scheduler::OvenSetTemperaturePid" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->OvenSetTemperaturePid( qSharedPointerDynamicCast<CmdOvenSetTemperaturePid>(m_currentCmd)->GetType(),
                                                                              qSharedPointerDynamicCast<CmdOvenSetTemperaturePid>(m_currentCmd)->GetMaxTemperature(),
                                                                              qSharedPointerDynamicCast<CmdOvenSetTemperaturePid>(m_currentCmd)->GetControllerGain(),
                                                                              qSharedPointerDynamicCast<CmdOvenSetTemperaturePid>(m_currentCmd)->GetResetTime(),
                                                                              qSharedPointerDynamicCast<CmdOvenSetTemperaturePid>(m_currentCmd)->GetDerivativeTime()
																			) );
	}
	else if  ("Scheduler::OvenStartTemperatureControl" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->OvenStartTemperatureControl( qSharedPointerDynamicCast<CmdOvenStartTemperatureControl>(m_currentCmd)->GetType(),
                                                                                    qSharedPointerDynamicCast<CmdOvenStartTemperatureControl>(m_currentCmd)->GetNominalTemperature(),
                                                                                    qSharedPointerDynamicCast<CmdOvenStartTemperatureControl>(m_currentCmd)->GetSlopeTempChange()
                                                                                  ) );
	}
	else if  ("Scheduler::OvenGetRecentTemperature" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->OvenGetRecentTemperature( qSharedPointerDynamicCast<CmdOvenGetRecentTemperature>(m_currentCmd)->GetType(),
                                                                                 qSharedPointerDynamicCast<CmdOvenGetRecentTemperature>(m_currentCmd)->GetIndex() ) );
	}
	else if  ("Scheduler::OvenGetTemperatureControlState" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->OvenGetTemperatureControlState( qSharedPointerDynamicCast<CmdOvenGetTemperatureControlState>(m_currentCmd)->GetType() ) );
	}
	else if  ("Scheduler::RTSetTempCtrlON" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->RTSetTempCtrlON( qSharedPointerDynamicCast<CmdRTSetTempCtrlON>(m_currentCmd)->GetType() ) );
	}
	else if  ("Scheduler::RTSetTempCtrlOFF" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->RTSetTempCtrlOFF( qSharedPointerDynamicCast<CmdRTSetTempCtrlOFF>(m_currentCmd)->GetType() ) );
	}
	else if  ("Scheduler::RTSetTemperaturePid" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->RTSetTemperaturePid( qSharedPointerDynamicCast<CmdRTSetTemperaturePid>(m_currentCmd)->GetType(),
                                                                            qSharedPointerDynamicCast<CmdRTSetTemperaturePid>(m_currentCmd)->GetMaxTemperature(),
                                                                            qSharedPointerDynamicCast<CmdRTSetTemperaturePid>(m_currentCmd)->GetControllerGain(),
                                                                            qSharedPointerDynamicCast<CmdRTSetTemperaturePid>(m_currentCmd)->GetResetTime(),
                                                                            qSharedPointerDynamicCast<CmdRTSetTemperaturePid>(m_currentCmd)->GetDerivativeTime()
																		  ) );
	}
	else if  ("Scheduler::RTStartTemperatureControl" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->RTStartTemperatureControl( qSharedPointerDynamicCast<CmdRTStartTemperatureControl>(m_currentCmd)->GetType(),
                                                                                  qSharedPointerDynamicCast<CmdRTStartTemperatureControl>(m_currentCmd)->GetNominalTemperature(),
                                                                                  qSharedPointerDynamicCast<CmdRTStartTemperatureControl>(m_currentCmd)->GetSlopeTempChange()
																				) );
	}
	else if  ("Scheduler::RTGetRecentTemperature" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->RTGetRecentTemperature( qSharedPointerDynamicCast<CmdRTGetRecentTemperature>(m_currentCmd)->GetType(),
                                                                               qSharedPointerDynamicCast<CmdRTGetRecentTemperature>(m_currentCmd)->GetIndex()
																			 ) );
	}
	else if  ("Scheduler::RTGetTemperatureControlState" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->RTGetTemperatureControlState( qSharedPointerDynamicCast<CmdRTGetTemperatureControlState>(m_currentCmd)->GetType()) );
	}
	else if  ("Scheduler::RTUnlock" == cmdName)
	{
		m_currentCmd->SetResult( mp_IDeviceProcessing->RTUnlock() );
	}
	else if  ("Scheduler::RTLock" == cmdName)
	{
		m_currentCmd->SetResult( mp_IDeviceProcessing->RTLock() );
	}
	else if  ("Scheduler::PerTurnOffMainRelay" == cmdName)
	{
		m_currentCmd->SetResult( mp_IDeviceProcessing->PerTurnOffMainRelay() );
	}
	else if  ("Scheduler::PerTurnOnMainRelay" == cmdName)
	{
		m_currentCmd->SetResult( mp_IDeviceProcessing->PerTurnOnMainRelay() );
	}
	else if  ("Scheduler::RVStartTemperatureControlWithPID" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->RVStartTemperatureControlWithPID( qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(m_currentCmd)->GetNominalTemperature(),
                                                                                         qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(m_currentCmd)->GetSlopeTempChange(),
                                                                                         qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(m_currentCmd)->GetMaxTemperature(),
                                                                                         qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(m_currentCmd)->GetControllerGain(),
                                                                                         qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(m_currentCmd)->GetResetTime(),
                                                                                         qSharedPointerDynamicCast<CmdRVStartTemperatureControlWithPID>(m_currentCmd)->GetDerivativeTime()
																					   ) );
	}
	else if  ("Scheduler::OvenStartTemperatureControlWithPID" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->OvenStartTemperatureControlWithPID( qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(m_currentCmd)->GetType(),
                                                                                           qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(m_currentCmd)->GetNominalTemperature(),
                                                                                           qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(m_currentCmd)->GetSlopeTempChange(),
                                                                                           qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(m_currentCmd)->GetMaxTemperature(),
                                                                                           qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(m_currentCmd)->GetControllerGain(),
                                                                                           qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(m_currentCmd)->GetResetTime(),
                                                                                           qSharedPointerDynamicCast<CmdOvenStartTemperatureControlWithPID>(m_currentCmd)->GetDerivativeTime()
																						) );
	}
	else if  ("Scheduler::RTStartTemperatureControlWithPID" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->RTStartTemperatureControlWithPID( qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(m_currentCmd)->GetType(),
                                                                                         qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(m_currentCmd)->GetNominalTemperature(),
                                                                                         qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(m_currentCmd)->GetSlopeTempChange(),
                                                                                         qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(m_currentCmd)->GetMaxTemperature(),
                                                                                         qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(m_currentCmd)->GetControllerGain(),
                                                                                         qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(m_currentCmd)->GetResetTime(),
                                                                                         qSharedPointerDynamicCast<CmdRTStartTemperatureControlWithPID>(m_currentCmd)->GetDerivativeTime()
																						) );
	}
	else if  ("Scheduler::ALStartTemperatureControlWithPID" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->ALStartTemperatureControlWithPID( qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(m_currentCmd)->GetType(),
                                                                                         qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(m_currentCmd)->GetNominalTemperature(),
                                                                                         qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(m_currentCmd)->GetSlopeTempChange(),
                                                                                         qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(m_currentCmd)->GetMaxTemperature(),
                                                                                         qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(m_currentCmd)->GetControllerGain(),
                                                                                         qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(m_currentCmd)->GetResetTime(),
                                                                                         qSharedPointerDynamicCast<CmdALStartTemperatureControlWithPID>(m_currentCmd)->GetDerivativeTime()
																					   ) );
	}
	else if  ("Scheduler::IDBottleCheck" == cmdName)
	{
        m_currentCmd->SetResult( mp_IDeviceProcessing->IDBottleCheck( qSharedPointerDynamicCast<CmdIDBottleCheck>(m_currentCmd)->GetReagentGrpID(),
                                                                      qSharedPointerDynamicCast<CmdIDBottleCheck>(m_currentCmd)->GetTubePos() ) );
	}
	else if  ("Scheduler::ALAllStop" == cmdName)
	{
		m_currentCmd->SetResult( mp_IDeviceProcessing->ALAllStop() );
	}
    else if ("Scheduler::IDSealingCheck" == cmdName)
    {
        m_currentCmd->SetResult( mp_IDeviceProcessing->IDSealingCheck(qSharedPointerDynamicCast<CmdIDSealingCheck>(m_currentCmd)->GetThresholdPressure()));
    }
}

}// end of namespace Scheduler


#ifdef GOOGLE_MOCK
template class Scheduler::SchedulerCommandProcessor<DeviceControl::MockIDeviceProcessing>;
#else
template class Scheduler::SchedulerCommandProcessor<DeviceControl::IDeviceProcessing>;
#endif
