// ================================================================================================
/*! @file Himalaya/HimalayaMain/Master/Components/Scheduler/Source/SchedulerMainThreadController.cpp
 *
 *  @brief
 *  Implementation of class Scheduler::SchedulerMainThreadController.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-03-06
 *  $Author:    $ Abe Yang
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
#include "Scheduler/Include/SchedulerEventCodes.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToInitialPosition.h"
#include "Scheduler/Commands/Include/CmdALStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdALFilling.h"
#include "Scheduler/Commands/Include/CmdALDraining.h"
#include "Scheduler/Commands/Include/CmdALPressure.h"
#include "Scheduler/Commands/Include/CmdALReleasePressure.h"
#include "Scheduler/Commands/Include/CmdALVaccum.h"
#include "Scheduler/Commands/Include/CmdALAllStop.h"
#include "Scheduler/Commands/Include/CmdRVStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdRTStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdOvenStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdALReleasePressure.h"
#include "Scheduler/Commands/Include/CmdPerTurnOnMainRelay.h"
#include "Scheduler/Commands/Include/CmdPerTurnOffMainRelay.h"
#include "Scheduler/Commands/Include/CmdALTurnOnFan.h"
#include "Scheduler/Commands/Include/CmdALTurnOffFan.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToRVPosition.h"
#include "Scheduler/Commands/Include/CmdIDBottleCheck.h"
#include "Scheduler/Commands/Include/CmdIDSealingCheck.h"
#include "Scheduler/Commands/Include/CmdRVSetTempCtrlOFF.h"
#include "Scheduler/Commands/Include/CmdRTSetTempCtrlOFF.h"
#include "Scheduler/Commands/Include/CmdOvenSetTempCtrlOFF.h"
#include "Scheduler/Commands/Include/CmdALSetTempCtrlOFF.h"
#include "Scheduler/Commands/Include/CmdRmtLocAlarm.h"
#include "Scheduler/Commands/Include/CmdALStopCmdExec.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "HimalayaDataManager/Include/DataManager.h"
#include "HimalayaDataContainer/Containers/ProgramSettings/Include/ProgramSettings.h"
#include "Global/Include/GlobalDefines.h"
#include <Scheduler/Commands/Include/CmdRTLock.h>
#include <Scheduler/Commands/Include/CmdRTUnlock.h>
#include <HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdCurrentProgramStepInfor.h>
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAcknowledge.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdEnterCleaningProgram.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAborted.h"
#include "HimalayaDataContainer/Containers/ReagentStations/Commands/Include/CmdUpdateStationReagentStatus.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdKeepCassetteCount.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdTakeOutSpecimenFinished.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelected.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelectedReply.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdStationSuckDrain.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdLockStatus.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdUpdateProgramEndTime.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdRecoveryFromPowerFailure.h"
#include "HimalayaDataContainer/Containers/UserSettings/Commands/Include/CmdQuitAppShutdown.h"
#include "HimalayaDataContainer/Containers/UserSettings/Commands/Include/CmdParaffinMeltPointChanged.h"
#include "HimalayaDataContainer/Containers/UserSettings/Commands/Include/CmdBottleCheck.h"
#include "HimalayaDataContainer/Containers/UserSettings/Commands/Include/CmdBottleCheckReply.h"
#include "Scheduler/Commands/Include/CmdSavedServiceInfor.h"
#include "DataManager/Containers/InstrumentHistory/Commands/Include/CmdModuleListUpdate.h"
#include "NetCommands/Include/CmdSystemAction.h"
#include "Global/Include/UITranslator.h"
#ifdef GOOGLE_MOCK
#include "Scheduler/Test/Mock/MockIDeviceProcessing.h"
#endif
#include "float.h"
#include "Global/Include/EventObject.h"
#include "Scheduler/Include/HeatingStrategy.h"
#include <unistd.h>
#include <QtAlgorithms>
#include "Global/Include/SystemPaths.h"
#include "Global/Include/Utils.h"
#include "Global/Include/Commands/CmdShutDown.h"
#include <DataManager/CommandInterface/Include/UserSettingsCommandInterface.h>

using namespace DataManager;

/*lint -e527 */
/*lint -e616 */
/*lint -e613 */

namespace Scheduler {

/****************************************************************************/
/**
 *  \brief  Declaration of function QPairComp
 *  \param  va1 - const QPair<QString, QString>
 *  \param  va2 - QPair<QString, QString>
 *  \return bool from QPairComp
 */
 /****************************************************************************/
static bool QPairComp(const QPair<QString, QString>& va1, const QPair<QString, QString>& va2)
{
    RVPosition_t tube1 = SchedulerMainThreadController::GetRVTubePositionByStationID(va1.first);
    RVPosition_t tube2 = SchedulerMainThreadController::GetRVTubePositionByStationID(va2.first);
    return tube1 < tube2;
}

SchedulerMainThreadController::SchedulerMainThreadController(
        Global::gSourceType TheHeartBeatSource)
        : Threads::ThreadController(TheHeartBeatSource, "SchedulerMainThread")
        , m_TickTimer(this)
        , m_SchedulerCommandProcessorThread(new QThread())
        #ifdef GOOGLE_MOCK
        , m_SchedulerCommandProcessor(new SchedulerCommandProcessor<MockIDeviceProcessing>(this))
        #else
        , m_SchedulerCommandProcessor(new SchedulerCommandProcessor<IDeviceProcessing>(this))
        #endif
        , m_SchedulerMachine(NULL)
        , mp_DataManager(NULL)
        , m_CurProgramStepIndex(-1)
        , m_FirstProgramStepIndex(0)
        , m_CurReagnetName("")
        , m_CurProgramID("")
        , m_NewProgramID("")
        , m_OvenLidStatus(0)
        , m_RetortLockStatus(UNDEFINED_VALUE)
        , m_ProcessCassetteCount(0)
        , m_ProcessCassetteNewCount(0)
        , mp_HeatingStrategy(NULL)
        , m_RefCleanup(Global::RefManager<Global::tRefType>::INVALID)
        , m_delayTime(0)
        , m_IsInSoakDelay(false)
        , m_hasParaffin(false)
        , m_Is5MinPause(false)
        , m_Is10MinPause(false)
        , m_Is15MinPause(false)
        ,m_InternalErrorRecv(false)
        ,m_bWaitToPause(false)
        ,m_ReagentExpiredFlag("")
{
    memset(&m_TimeStamps, 0, sizeof(m_TimeStamps));
    m_CurErrEventID = DCL_ERR_FCT_NOT_IMPLEMENTED;
    m_CurrentScenario = 1;
    m_IsPrecheckMoveRV = false;

    m_lastPVTime = 0;
    m_ProcessingPV = 3; // 0 for Pressure and 1 for Vacuum, 3 for avoiding message to pop up too many times
    m_completionNotifierSent = false;
    m_IsReleasePressureOfSoakFinish = false;
    m_IsCleaningProgram = false;
    m_CleanAckSentGui = false;
    m_CurrentStepState = PSSM_INIT;
    m_ReEnterFilling = 0;
    m_TimeReEnterFilling = 0;
    m_CheckRemoteAlarmStatus = true;
    m_CheckLocalAlarmStatus = true;
    m_LocalAlarmPreviousStatus = -1;
    m_RemoteAlarmPreviousStatus = -1;
    m_PssmStepFinSeq = 0;
    m_CheckOvenCover = true;
    m_TransitionPeriod = false;
    m_PowerFailureStep = POWERFAILURE_INIT;
    m_IsWaitHeatingRV = false;
    m_IsSendMsgForWaitHeatRV = false;
    m_IsErrorStateForHM = false;
    m_IsProcessing = false;
    m_IdleState = IDLE_HEATING_RV;
    m_RVPositioinChSeqForIdle = 0;
    m_PressureStartTime = 0;
    m_IsTakeSpecimen = false;
    m_CountTheLogSenserData = 0;
    m_CheckTheHardwareStatus = 0;
    m_IsFirstProcessingForDelay = true;

    ResetTheTimeParameter();
    m_DisableAlarm = Global::Workaroundchecking("DISABLE_ALARM");
    memset(&m_CleaningDry, 0, sizeof(m_CleaningDry));
}

SchedulerMainThreadController::~SchedulerMainThreadController()
{
    try {
        if (m_SchedulerMachine != NULL)
            delete m_SchedulerMachine;
        m_SchedulerMachine = NULL;
    }
    CATCHALL_DTOR();
}

void SchedulerMainThreadController::RegisterCommands()
{

    RegisterCommandForProcessing<MsgClasses::CmdProgramAction,
                    SchedulerMainThreadController>(&SchedulerMainThreadController::OnProgramAction, this);

    RegisterCommandForProcessing<NetCommands::CmdSystemAction,
                    SchedulerMainThreadController>(&SchedulerMainThreadController::OnActionCommandReceived, this);

    RegisterCommandForProcessing<MsgClasses::CmdKeepCassetteCount,
                    SchedulerMainThreadController>(&SchedulerMainThreadController::OnKeepCassetteCount, this);

    RegisterCommandForProcessing<MsgClasses::CmdProgramSelected,
                    SchedulerMainThreadController>(&SchedulerMainThreadController::OnProgramSelected, this);

    RegisterCommandForProcessing<MsgClasses::CmdQuitAppShutdown,
                    SchedulerMainThreadController>(&SchedulerMainThreadController::OnQuitAppShutdown, this);

    RegisterCommandForProcessing<MsgClasses::CmdSavedServiceInfor,
                    SchedulerMainThreadController>(&SchedulerMainThreadController::OnSavedServiceInfor, this);

    RegisterCommandForProcessing<MsgClasses::CmdParaffinMeltPointChanged,
                    SchedulerMainThreadController>(&SchedulerMainThreadController::OnParaffinMeltPointChanged, this);

    RegisterCommandForProcessing<MsgClasses::CmdBottleCheck,
                    SchedulerMainThreadController>(&SchedulerMainThreadController::OnBottleCheck, this);

    RegisterCommandForProcessing<MsgClasses::CmdTakeOutSpecimenFinished,
                    SchedulerMainThreadController>(&SchedulerMainThreadController::OnTakeOutSpecimenFinished, this);

    RegisterAcknowledgeForProcessing<Global::AckOKNOK, SchedulerMainThreadController>
            (&SchedulerMainThreadController::OnAcknowledge, this);

}

void SchedulerMainThreadController::CreateAndInitializeObjects()
{

    m_SchedulerCommandProcessor->moveToThread(m_SchedulerCommandProcessorThread);
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessorThread, started(), m_SchedulerCommandProcessor, run());
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessor, destroyed(), m_SchedulerCommandProcessorThread, quit());
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessorThread, finished(), m_SchedulerCommandProcessorThread, deleteLater());
    m_SchedulerCommandProcessorThread->start();

    //timer setting
    CONNECTSIGNALSLOT(&m_TickTimer, timeout(), this, OnTickTimer());
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessor,DCLConfigurationFinished(ReturnCode_t),this,OnDCLConfigurationFinished(ReturnCode_t));
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessor, DeviceProcessDestroyed(),this, DevProcDestroyed());
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessor, ReportGetServiceInfo(ReturnCode_t, const DataManager::CModule&, const QString&),
                     this, ReportGetServiceInfo(ReturnCode_t, const DataManager::CModule&, const QString&));
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessor, ReportFillingTimeOut2Min(),this, OnReportFillingTimeOut2Min());
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessor, ReportDrainingTimeOut2Min(),this, OnReportDrainingTimeOut2Min());
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessor, ReportError(quint32, quint16, quint16, quint16, QDateTime),this, OnReportError(quint32, quint16, quint16, quint16, QDateTime));
    CONNECTSIGNALSLOT(mp_DataManager->mp_SettingsCommandInterface, ResetActiveCarbonFilterLifeTime(),
                     this, ResetActiveCarbonFilterLifetime());
    m_TickTimer.setInterval(500);

    //command queue reset
    m_SchedulerCmdQueue.clear();
    m_DeviceControlCmdQueue.clear();

    // now register commands
    RegisterCommands();

    //InitializeDevice();

    //For Event-Scenaro-Error map list, we store it into m_pXMLEventList
    m_pESEXMLInfo = QSharedPointer<EventScenarioErrXMLInfo>(new EventScenarioErrXMLInfo(Global::SystemPaths::Instance().GetSettingsPath() + QDir::separator() + "EventScenarioErrorMap.xml"));
    if (m_pESEXMLInfo->InitXMLInfo() == false)
    {
        //
    }
}

void SchedulerMainThreadController::DevProcDestroyed()
{
    if (Global::RefManager<Global::tRefType>::INVALID != m_RefCleanup) {
          SendAcknowledgeOK(m_RefCleanup);
      }
}

void SchedulerMainThreadController::ReportGetServiceInfo(ReturnCode_t ReturnCode, const DataManager::CModule &ModuleInfo, const QString& DeviceType)
{
    //Q_UNUSED(ReturnCode)
    LogDebug(QString("LifeCycle:ReportGetServiceInfo:%1,%2").arg(ReturnCode).arg(DeviceType));
    //send command
    SendCommand(GetNewCommandRef(), Global::CommandShPtr_t(new MsgClasses::CmdModuleListUpdate(30000, ModuleInfo, DeviceType, false)));
}

void SchedulerMainThreadController::ResetActiveCarbonFilterLifetime()
{
    m_SchedulerCommandProcessor->ResetActiveCarbonFilterLifetime();
}
void SchedulerMainThreadController::OnReportFillingTimeOut2Min()
{
    QString ReagentGroup = m_CurProgramStepInfo.reagentGroup;
    quint32 Scenario = GetScenarioBySchedulerState(m_SchedulerMachine->GetCurrentState(),ReagentGroup);
    if (0 != Scenario)
    {
        SendOutErrMsg(DCL_ERR_DEV_LA_FILLING_TIMEOUT_2MIN, false);
    }
}
void SchedulerMainThreadController::OnReportDrainingTimeOut2Min()
{
    QString ReagentGroup = m_CurProgramStepInfo.reagentGroup;
    quint32 Scenario = GetScenarioBySchedulerState(m_SchedulerMachine->GetCurrentState(),ReagentGroup);
    if (0 != Scenario)
    {
        SendOutErrMsg(DCL_ERR_DEV_LA_DRAINING_TIMEOUT_EMPTY_2MIN, false);
    }
}

void SchedulerMainThreadController::OnReportError(quint32 instanceID, quint16 usErrorGroup, quint16 usErrorID, quint16 usErrorData, QDateTime timeStamp)
{
    LogDebug(QString("In OnReportError, instanceID=%1, usErrorGroup=%2, usErrorID=%3, usErrorData=%4 and timeStamp=%5")
             .arg(instanceID, 0, 16).arg(usErrorGroup).arg(usErrorID).arg(usErrorData).arg(timeStamp.toString()));

#if 1
    if(usErrorID == DCL_ERR_TIMEOUT || usErrorID == DCL_ERR_CANCOMMUTOR_COMM_FAILED)
    {
        if (false == m_InternalErrorRecv)
        {
            m_InternalErrorRecv = true;
            SendOutErrMsg(DCL_ERR_DEV_INTER_SW_ERROR, false);
        }
    }
#endif
}

void SchedulerMainThreadController::CleanupAndDestroyObjects()
{
}

void SchedulerMainThreadController::OnGoReceived()
{
}

void SchedulerMainThreadController::OnStopReceived()
{
    m_TickTimer.stop();
    m_SchedulerCommandProcessorThread->quit();
    (void)m_SchedulerCommandProcessorThread->wait();
}

void SchedulerMainThreadController::OnPowerFail(const Global::PowerFailStages PowerFailStage)
{
    Q_UNUSED(PowerFailStage)
}

/****************************************************************************/
void SchedulerMainThreadController::OnAcknowledge(Global::tRefType Ref, const Global::AckOKNOK &Ack)
{
    Q_UNUSED(Ref)
    Q_UNUSED(Ack)
}
void SchedulerMainThreadController::OnTickTimer()
{
    ControlCommandType_t newControllerCmd = PeekNonDeviceCommand();

#if 1
    if (CTRL_CMD_RS_SHUTDOWN == newControllerCmd)
    {
        //send shutdown command to MasterThreadController
        SendCommand(GetNewCommandRef(), Global::CommandShPtr_t(new Global::CmdShutDown(true)));
        m_TickTimer.stop();
        m_ProgramStatusInfor.SetErrorFlag(0);
        m_SchedulerCommandProcessor->ShutDownDevice();
        return;
    }
#endif
    if (CTRL_CMD_SHUTDOWN == newControllerCmd)
    {
        m_TickTimer.stop();
        m_ProgramStatusInfor.SetErrorFlag(0);
        m_ProgramStatusInfor.SetProgramID("");
        m_SchedulerCommandProcessor->ShutDownDevice();
        //DequeueNonDeviceCommand();
        return;
    }

    if (CTRL_CMD_TAKEOUT_SPECIMEN_FINISH == newControllerCmd)
    {
        m_IsTakeSpecimen = true;
    }

    // For remote alarm and local alarm checking
    if (CTRL_CMD_RS_CHECKREMOTEALARMSTATUS == newControllerCmd)
    {
        m_CheckRemoteAlarmStatus = true;
    }

    if (CTRL_CMD_RS_CHECKLOCALALARMSTATUS == newControllerCmd)
    {
        m_CheckLocalAlarmStatus = true;
    }

    if (CTRL_CMD_CHECK_OVEN_COVER == newControllerCmd)
    {
        m_CheckOvenCover = true;
    }
    if (CTRL_CMD_OPEN_OVEN_CHANGE_HEATING_PARAFFIN == newControllerCmd)
    {
        m_CheckOvenCover = true;
        quint32 ParaffinMeltPoint = 64;
        if (mp_DataManager != NULL && mp_DataManager->GetUserSettings() != NULL)
        {
            ParaffinMeltPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
        }
        m_ProgramStatusInfor.ResetOvenHeatingTime(ParaffinMeltPoint,true);
        mp_HeatingStrategy->ResetTheOvenHeating();
    }


    SchedulerStateMachine_t currentState = m_SchedulerMachine->GetCurrentState();
    m_SchedulerMachine->UpdateCurrentState(currentState);

    // We only handle Abort in Idle and Busy
    if(CTRL_CMD_ABORT == newControllerCmd && ((currentState & 0xFF) == SM_IDLE || (currentState & 0xFF) == SM_BUSY))
    {
        //tell main controller scheduler is starting to abort
        MsgClasses::CmdProgramAcknowledge* commandPtrAbortBegin(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_ABORT_BEGIN));
        Q_ASSERT(commandPtrAbortBegin);
        Global::tRefType fRef = GetNewCommandRef();
        SendCommand(fRef, Global::CommandShPtr_t(commandPtrAbortBegin));
        m_TransitionPeriod = true;
        SendOutErrMsg(DCL_ERR_DEV_INTER_ABORT);
        return;
    }

    SchedulerCommandShPtr_t cmd;
    (void)PopDeviceControlCmdQueue(cmd);
    switch(currentState & 0xFF)
    {
    case SM_INIT:
        //In INIT state will do self test
        HardwareMonitor("INIT");
        HandleRmtLocAlarm(newControllerCmd);
        HandleInitState(newControllerCmd, cmd);
        break;
    case SM_POWER_FAILURE:
        HandleRmtLocAlarm(newControllerCmd);
        HandlePowerFailure(newControllerCmd, cmd);
        break;
    case SM_IDLE:
        //qDebug()<<"DBG"<<"Scheduler main controller state: IDLE";
        HardwareMonitor( "IDLE" );
        HandleRmtLocAlarm(newControllerCmd);
        HandleIdleState(newControllerCmd,cmd);
        break;
    case SM_BUSY:
        //qDebug()<<"DBG"<<"Scheduler main controller state: RUN";
        HardwareMonitor( m_CurProgramID );
        HandleRmtLocAlarm(newControllerCmd);
        HandleRunState(newControllerCmd, cmd);
        break;
    case SM_ERROR:
        //qDebug()<<"DBG"<<"Scheduler main controller state: ERROR";
        HardwareMonitor( "ERROR" );
        HandleRmtLocAlarm(newControllerCmd);
        HandleErrorState(newControllerCmd, cmd, currentState);
        break;
    default:
        LogDebug(QString("Scheduler main controller gets unexpected state: %1").arg(currentState));
    }
}

void SchedulerMainThreadController::OnSelfTestDone(bool flag)
{
    if(flag)
    {
        // Turn on fan, and we will open it all the time
        m_SchedulerCommandProcessor->pushCmd(new CmdALTurnOnFan(500, this), false);

        bool bErrorHandlingFailed = m_ProgramStatusInfor.GetErrorFlag();
        if(!m_ProgramStatusInfor.IsProgramFinished())//power failure
        {
            MsgClasses::CmdRecoveryFromPowerFailure* commandPtr(
                        new MsgClasses::CmdRecoveryFromPowerFailure(5000,m_ProgramStatusInfor.GetProgramId(),
                                                                    m_ProgramStatusInfor.GetStepID(),
                                                                    m_ProgramStatusInfor.GetScenario(),
                                                                    GetLeftProgramStepsNeededTime(m_ProgramStatusInfor.GetProgramId(), m_ProgramStatusInfor.GetStepID()),
                                                                    m_ProgramStatusInfor.GetLastReagentGroup(),
                                                                    m_ProgramStatusInfor.GetStationList(),
                                                                    bErrorHandlingFailed, m_ProgramStatusInfor.GetStationID()));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
            if(1 == bErrorHandlingFailed)
            {
                RaiseError(0, DCL_ERR_DEV_INTER_POWERFAILURE_AFTER_ERRHANDLING_FAILED, m_ProgramStatusInfor.GetScenario(), true);
                m_SchedulerMachine->SendErrorSignal();
            }
            else
            {
                QString ProgramName = "";
                if(mp_DataManager&& mp_DataManager->GetProgramList()&&mp_DataManager->GetProgramList()->GetProgram(m_ProgramStatusInfor.GetProgramId()))
                {
                    ProgramName = mp_DataManager->GetProgramList()->GetProgram(m_ProgramStatusInfor.GetProgramId())->GetName();
                }
                RaiseEvent(EVENT_SCHEDULER_POWER_FAILURE,QStringList()<<ProgramName<<QString("[%1]").arg(m_ProgramStatusInfor.GetStepID()));
                m_SchedulerMachine->EnterPowerFailure();
            }
        }
        else
        {
            if(1 == bErrorHandlingFailed)
            {
                m_ProgramStatusInfor.SetErrorFlag(0);
            }
            OnEnterIdleState();
            m_IsTakeSpecimen = true;
            m_SchedulerMachine->SendSchedulerInitComplete();
            MsgClasses::CmdProgramAcknowledge* commandPtr(new MsgClasses::CmdProgramAcknowledge(5000, DataManager::PROGRAM_SELFTEST_PASSED));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        }
        mp_HeatingStrategy->ResetTheOvenHeating();
    }
    else
    {
        LogDebug("Self test is failed");
        MsgClasses::CmdProgramAcknowledge* commandPtr(new MsgClasses::CmdProgramAcknowledge(5000, DataManager::PROGRAM_SELFTEST_FAILED));
        Q_ASSERT(commandPtr);
        Global::tRefType Ref = GetNewCommandRef();
        SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        m_SchedulerMachine->SendErrorSignal();
    }
}

void SchedulerMainThreadController::OnEnterIdleState()
{
    // Disable Bottle Check button
    MsgClasses::CmdProgramAcknowledge* commandPtr(new MsgClasses::CmdProgramAcknowledge(5000, DataManager::DISABLE_BOTTLE_CHECK));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

    //for bottle check and Error restart in idle state , the cmd take out specimen is OK
    if((4 == m_CurrentScenario && SM_IDLE == m_CurrentStepState) || 7 == m_CurrentScenario)
    {
        m_IsTakeSpecimen = true;
    }
    m_IsWaitHeatingRV = true;
    m_IsSendMsgForWaitHeatRV = false;
    m_IdleState = IDLE_HEATING_RV;
    m_RVPositioinChSeqForIdle = 0;
    m_PressureStartTime = 0;

    if (m_ProgramStatusInfor.IsRetortContaminted() && !m_CleanAckSentGui)
    {
        m_CleanAckSentGui = true;
        MsgClasses::CmdEnterCleaningProgram* commandEnterCleaning(new MsgClasses::CmdEnterCleaningProgram(5000, m_ProgramStatusInfor.GetLastReagentGroup()));
        Q_ASSERT(commandEnterCleaning);
        Global::tRefType fRef = GetNewCommandRef();
        SendCommand(fRef, Global::CommandShPtr_t(commandEnterCleaning));
    }
}

void SchedulerMainThreadController::OnExitIdleState()
{
    // Disable Bottle Check button
    MsgClasses::CmdProgramAcknowledge* commandPtr(new MsgClasses::CmdProgramAcknowledge(5000, DataManager::DISABLE_BOTTLE_CHECK));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
}

void SchedulerMainThreadController::HandlePowerFailure(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd)
{
    Q_UNUSED(ctrlCmd);
    Q_UNUSED(cmd);

    if(POWERFAILURE_INIT == m_PowerFailureStep)
    {
        if(m_SchedulerCommandProcessor != NULL)
        {
            m_SchedulerCommandProcessor->ALSetPressureDrift(m_ProgramStatusInfor.GetPressureDriftOffset());
        }

        QString curProgramID = m_ProgramStatusInfor.GetProgramId();
        if (curProgramID.isNull() || curProgramID.isEmpty())
        {
            return;
        }

        quint32 scenario = m_ProgramStatusInfor.GetScenario();
        int StepID = m_ProgramStatusInfor.GetStepID();
        if(-1 == StepID)
        {
            StepID = 0;
        }

        if(curProgramID.at(0) == 'C')
        {
            m_IsCleaningProgram = true;
        }
        m_CurProgramID = curProgramID;
        m_CurrentScenario = scenario;
        m_FirstProgramStepIndex = 0;

        (void)this->PrepareProgramStationList(curProgramID, 0);

        m_CurProgramStepIndex = StepID - 1;
        (void)this->GetNextProgramStepInformation(curProgramID, m_CurProgramStepInfo);

        if(203 == scenario)
        {
            m_IsCleaningProgram = true;
            m_CurrentStepState = PSSM_CLEANING_DRY_STEP;
            SendOutErrMsg(DCL_ERR_DEV_POWERFAILURE_CLEANING_MSGBOX, false);
        }
        else if(281 <= scenario && scenario <= 297)
        {
            m_IsCleaningProgram = true;
            m_CurrentStepState = PSSM_FILLING_LEVELSENSOR_HEATING;
            SendOutErrMsg(DCL_ERR_DEV_POWERFAILURE_CLEANING_MSGBOX, false);
        }
        m_PowerFailureStep = POWERFAILURE_RUN;
    }
    else if(POWERFAILURE_RUN == m_PowerFailureStep)
    {
      SendOutErrMsg(DCL_ERR_DEV_POWERFAILURE);
    }
}


void SchedulerMainThreadController::HandleInitState(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd)
{
    Q_UNUSED(ctrlCmd)
    m_CurrentScenario = GetScenarioBySchedulerState(m_SchedulerMachine->GetCurrentState(), m_CurProgramStepInfo.reagentGroup);
    ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;
    QString cmdName = "";

    /*lint -e58 */
    if(cmd != NULL)
    {
        if(!(cmd->GetResult(retCode)))
        {
            retCode = DCL_ERR_UNDEFINED;
        }
        cmdName = cmd->GetName();
    }

    SchedulerStateMachine_t stepState = m_SchedulerMachine->GetCurrentState();
    if(SM_INIT_SELFTEST == stepState)
    {
        m_SchedulerMachine->HandleSelfTestWorkFlow(cmdName, retCode);
    }
}

void SchedulerMainThreadController::PrepareForIdle(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd)
{
    Q_UNUSED(ctrlCmd);
    if(m_ProgramStatusInfor.IsRetortContaminted())
    {
       if(!m_IsSendMsgForWaitHeatRV)
       {
           MsgClasses::CmdProgramAcknowledge* commandPtr(new MsgClasses::CmdProgramAcknowledge(5000, DataManager::WAIT_ROTARY_VALVE_HEATING_PROMPT));
           Q_ASSERT(commandPtr);
           Global::tRefType Ref = GetNewCommandRef();
           SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
           m_IsSendMsgForWaitHeatRV = true;
       }
       else
       {
           if(mp_HeatingStrategy->CheckRV2TemperatureSenseorsStatus())
           {
               MsgClasses::CmdProgramAcknowledge* commandPtr(new MsgClasses::CmdProgramAcknowledge(5000, DataManager::PROGRAM_READY));
               Q_ASSERT(commandPtr);
               Global::tRefType Ref = GetNewCommandRef();
               SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
               m_IsWaitHeatingRV = false;
               m_IsSendMsgForWaitHeatRV = false;
           }
       }
    }
    else
    {
        if(!m_IsSendMsgForWaitHeatRV)
        {
            MsgClasses::CmdProgramAcknowledge* commandPtr(new MsgClasses::CmdProgramAcknowledge(5000, DataManager::WAIT_ROTARY_VALVE_HEATING_PROMPT));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
            m_IsSendMsgForWaitHeatRV = true;
        }
        if(!m_IsTakeSpecimen)
            return;
        ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;
        QString cmdName = "";

        if(cmd != NULL)
        {
            if(!(cmd->GetResult(retCode)))
            {
                retCode = DCL_ERR_UNDEFINED;
            }
            cmdName = cmd->GetName();
        }

        switch(m_IdleState)
        {
            case IDLE_HEATING_RV:
                if(mp_HeatingStrategy->CheckRV2TemperatureSenseorsStatus())
                {
                    m_IdleState = IDLE_DRAIN_10S;
                }
                break;
            case IDLE_DRAIN_10S:
                if(0 == m_RVPositioinChSeqForIdle)
                {
                    CmdALPressure* CmdPressure = new CmdALPressure(500, this);
                    CmdPressure->SetTargetPressure(30.0);
                    m_SchedulerCommandProcessor->pushCmd(CmdPressure);
                    m_PressureStartTime = QDateTime::currentMSecsSinceEpoch();
                    m_RVPositioinChSeqForIdle++;
                    LogDebug("Drain for 10 seconds.");
                }
                else if(1 == m_RVPositioinChSeqForIdle)
                {
                    if(QDateTime::currentMSecsSinceEpoch() - m_PressureStartTime > 10*1000)
                    {
                        ReleasePressure();
                        m_RVPositioinChSeqForIdle++;
                    }
                }
                else if ("Scheduler::ALReleasePressure" == cmdName)
                {
                    m_RVPositioinChSeqForIdle = 0;
                    m_IdleState = IDLE_MOVE_RV_INITIALIZE;
                }
                break;
            case IDLE_MOVE_RV_INITIALIZE:
                if(0 == m_RVPositioinChSeqForIdle)
                {
                    m_SchedulerCommandProcessor->pushCmd(new CmdRVReqMoveToInitialPosition(500, this));
                    m_RVPositioinChSeqForIdle++;
                }
                else if("Scheduler::RVReqMoveToInitialPosition" == cmdName)
                {
                    if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
                    {
                        m_IdleState = IDLE_MOVE_TUBE_2;
                        RaiseEvent(EVENT_SCHEDULER_MOVETO_INITIALIZE_POSITION_SUCCESS);
                    }
                    else
                    {
                        SendOutErrMsg(retCode);
                    }
                    m_RVPositioinChSeqForIdle = 0;
                }
                break;
            case IDLE_MOVE_TUBE_2:
                if(0 == m_RVPositioinChSeqForIdle)
                {
                   CmdRVReqMoveToRVPosition *CmdMvRV = new CmdRVReqMoveToRVPosition(500, this);
                    CmdMvRV->SetRVPosition(DeviceControl::RV_TUBE_2);
                    m_SchedulerCommandProcessor->pushCmd(CmdMvRV);
                    m_RVPositioinChSeqForIdle++;
                }
                else if("Scheduler::RVReqMoveToRVPosition" == cmdName)
                {
                    if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                    {
                        m_IdleState = IDLE_READY_OK;
                        m_ProgramStatusInfor.SetLastRVPosition(DeviceControl::RV_TUBE_2);
                        // Enable Bottle Check button
                        MsgClasses::CmdProgramAcknowledge* commandPtr(new MsgClasses::CmdProgramAcknowledge(5000, DataManager::ENABLE_BOTTLE_CHECK));
                        Q_ASSERT(commandPtr);
                        Global::tRefType Ref = GetNewCommandRef();
                        SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
                    }
                    else
                    {
                        SendOutErrMsg(retCode);
                    }
                    m_RVPositioinChSeqForIdle = 0;
                }
                break;
            case IDLE_READY_OK:
                {
                    MsgClasses::CmdProgramAcknowledge* commandPtr(new MsgClasses::CmdProgramAcknowledge(5000, DataManager::PROGRAM_READY));
                    Q_ASSERT(commandPtr);
                    Global::tRefType Ref = GetNewCommandRef();
                    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
                    m_IsWaitHeatingRV = false;
                    m_IsSendMsgForWaitHeatRV = false;
                    m_IsTakeSpecimen = false;
                }
                break;
            default:
                break;
        }
    }
}

void SchedulerMainThreadController::HandleIdleState(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd)
{
    m_CurrentStepState = SM_IDLE;
    Q_UNUSED(cmd)
    if(m_IsWaitHeatingRV)
    {
        PrepareForIdle(ctrlCmd, cmd);
        return;
    }
    switch (ctrlCmd)
    {
    case CTRL_CMD_START:
        m_IsProcessing = false;
        m_IsFirstProcessingForDelay = true;
        //Check if it is a Cleaning Program or not?
        if (m_NewProgramID.at(0) == 'C')
        {
            m_CurProgramID = m_NewProgramID;
            m_IsCleaningProgram = true;
            m_CleanAckSentGui = false;
        }
        else
        {
            m_CurProgramID = m_NewProgramID;
            m_IsCleaningProgram = false;
        }
        m_ProgramStatusInfor.SetProgramID(m_CurProgramID);
        m_ProgramStatusInfor.SetStationList(m_StationList);
        (void)this->GetNextProgramStepInformation(m_CurProgramID, m_CurProgramStepInfo);
        m_ProgramStatusInfor.SetStepID(m_CurProgramStepIndex); ///For Powerfailure:store current step id
        m_ProgramStatusInfor.SetStationID(m_CurProgramStepInfo.stationID);

        if(m_CurProgramStepIndex != -1)
        {
            //send command to main controller to tell program is starting
            MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_BEGIN));
            Q_ASSERT(commandPtrFinish);
            Global::tRefType fRef = GetNewCommandRef();
            SendCommand(fRef, Global::CommandShPtr_t(commandPtrFinish));

            QString ProgramName = mp_DataManager->GetProgramList()->GetProgram(m_CurProgramID)->GetName();
            RaiseEvent(EVENT_SCHEDULER_START_PROGRAM, QStringList() << ProgramName << "" + m_ReagentExpiredFlag);

            //send command to main controller to tell the left time
            MsgClasses::CmdCurrentProgramStepInfor* commandPtr(new MsgClasses::CmdCurrentProgramStepInfor(5000,
                                                                                                          Global::UITranslator::TranslatorInstance().Translate(STR_SCHEDULER_PRECHECK),
                                                                                                          m_CurProgramStepIndex, m_EndTimeAndStepTime.PreTestTime));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

            // Set current step to Init
            m_CurrentStepState = PSSM_INIT;
            mp_HeatingStrategy->Init260ParamList(true);

            // Start up PssmPrecheck state machine
            m_SchedulerMachine->StartPreCheck();

            m_SchedulerMachine->SendRunSignal();
        }
        break;
    case CTRL_CMD_BOTTLE_CHECK:
        // Set current step to Init
        m_CurrentStepState = PSSM_BOTTLE_CHECK;
        m_SchedulerMachine->SendRunSignal();
        break;
    default:
        break;
    }
}

void SchedulerMainThreadController::UpdateStationReagentStatus(bool bOnlyNew)
{
    m_UsedStationIDs.clear();
    m_UsedStationIDs.append(m_CurProgramStepInfo.stationID);
    MsgClasses::CmdUpdateStationReagentStatus* commandPtr = NULL;
    DataManager::CHimalayaUserSettings* pUserSetting = mp_DataManager->GetUserSettings();

    if (m_CurProgramID.isNull() || m_CurProgramID.isEmpty())
    {
        return;
    }

    if (m_CurProgramID.at(0) == 'C')//process cleaning reagent
    {
        QString nextStationID  = this->GetStationIDFromProgramStep(m_CurProgramStepIndex + 1);
        if (nextStationID == m_CurProgramStepInfo.stationID)
            return;

        Global::RMSOptions_t rmsMode = pUserSetting->GetModeRMSCleaning();
        if (rmsMode == Global::RMS_CYCLES)
        {
            commandPtr = new MsgClasses::CmdUpdateStationReagentStatus(5000, m_UsedStationIDs, 0);
        }
    }
    else //process reagent
    {
        Global::RMSOptions_t rmsMode = pUserSetting->GetModeRMSProcessing();
        if (rmsMode == Global::RMS_CYCLES)
        {
            commandPtr = new MsgClasses::CmdUpdateStationReagentStatus(5000, m_UsedStationIDs, 0);
        }
        else
        {
            int tempCount = 0;
            if (bOnlyNew)
                tempCount = m_ProcessCassetteNewCount;
            else
                tempCount = m_ProcessCassetteCount;
            commandPtr = new MsgClasses::CmdUpdateStationReagentStatus(5000, m_UsedStationIDs, tempCount);//toDo: 100, should get the actual number
        }
    }

    if(commandPtr)
    {
        Global::tRefType Ref = GetNewCommandRef();
        SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
    }
}

void SchedulerMainThreadController::SendProgramAcknowledge(DataManager::ProgramAcknownedgeType_t ackType)
{
    MsgClasses::CmdProgramAcknowledge* commandPtrWaitForDrain(new MsgClasses::CmdProgramAcknowledge(5000, ackType));
    Q_ASSERT(commandPtrWaitForDrain);
    Global::tRefType fRef = GetNewCommandRef();
    SendCommand(fRef, Global::CommandShPtr_t(commandPtrWaitForDrain));
}

void SchedulerMainThreadController::HandleRunState(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd)
{
    ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;
    QString cmdName = "";

    if(cmd != NULL)
    {
        if(!(cmd->GetResult(retCode)))
        {
            retCode = DCL_ERR_UNDEFINED;
        }
        cmdName = cmd->GetName();
    }


    SchedulerStateMachine_t stepState = m_SchedulerMachine->GetCurrentState();

    if(PSSM_INIT == stepState)
    {
        if(CTRL_CMD_PAUSE == ctrlCmd)
        {
            AllStop();
            m_SchedulerMachine->NotifyPause(PSSM_INIT);
        }
        else
        {
            /*lint -e525 */
            switch (m_CurrentStepState)
            {
            case PSSM_INIT:
                m_SchedulerMachine->SendRunPreTest();
                break;
            case PSSM_PRETEST:
                m_SchedulerMachine->SendRunPreTest();
                break;
            case PSSM_FILLING_RVROD_HEATING:
                m_SchedulerMachine->SendResumeFillingRVRodHeating();
                break;
            case PSSM_FILLING_LEVELSENSOR_HEATING:
            case PSSM_FILLING:
                m_SchedulerMachine->SendResumeFillingLevelSensorHeating();
                break;
            case PSSM_RV_MOVE_TO_SEAL:
                m_SchedulerMachine->SendResumeRVMoveToSeal();
                break;
            case PSSM_PROCESSING:
                m_SchedulerMachine->SendResumeProcessing();
                break;
            case PSSM_RV_MOVE_TO_TUBE:
                m_SchedulerMachine->SendResumeRVMoveTube();
                break;
            case PSSM_DRAINING:
            case PSSM_RV_POS_CHANGE:
                m_SchedulerMachine->SendResumeRVPosChange();
                break;
            case PSSM_CLEANING_DRY_STEP:
                m_SchedulerMachine->SendResumeDryStep();
                break;
            case PSSM_POWERFAILURE_FINISH:
                if(m_IsCleaningProgram && 200 == m_ProgramStatusInfor.GetScenario())
                {
                    MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::CLEANING_PROGRAM_COMPLETE_AS_SAFE_REAGENT));
                    Q_ASSERT(commandPtrFinish);
                    Global::tRefType Ref = GetNewCommandRef();
                    SendCommand(Ref, Global::CommandShPtr_t(commandPtrFinish));
                }
                else
                {
                    if( !m_ProgramStatusInfor.IsRetortContaminted() )
                    {
                        MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_FINISHED));
                        Q_ASSERT(commandPtrFinish);
                        Global::tRefType Ref = GetNewCommandRef();
                        SendCommand(Ref, Global::CommandShPtr_t(commandPtrFinish));
                        m_IsTakeSpecimen = true;
                    }
                }
                m_SchedulerMachine->SendRunComplete();
                break;
            case PSSM_BOTTLE_CHECK:
                m_SchedulerMachine->SendRunBottleCheck();
                break;
            default:
                break;
            }

        }
    }
    else if (PSSM_PRETEST == stepState)
    {
       m_CurrentStepState = PSSM_PRETEST;
       m_SchedulerMachine->HandlePssmPreTestWorkFlow(cmdName, retCode);
    }
    else if (PSSM_FILLING_RVROD_HEATING == stepState)
    {
        m_CurrentStepState = PSSM_FILLING_RVROD_HEATING;
        if(m_CurProgramStepInfo.reagentGroup == "RG6")
        {
            if(CTRL_CMD_PAUSE == ctrlCmd)
            {
                SendProgramAcknowledge(SHOW_PAUSE_MSG_DLG);
                m_bWaitToPause = true;
            }

            if(mp_HeatingStrategy->Check260SensorsTemp())
            {
                LogDebug("Program Step Heating Rotary Valve heating rod OK");
                if (m_bWaitToPause)
                {
                   //dismiss the prompt of pausing
                   SendProgramAcknowledge(DISMISS_PAUSING_MSG_DLG);
                   m_bWaitToPause = false;
                   LogDebug(QString("Program Step Beginning Pause"));
                   m_SchedulerMachine->NotifyPause(SM_UNDEF);
                   return;
                }
                m_SchedulerMachine->NotifyRVRodHeatingReady();
            }
        }
        else
        {
            if(CTRL_CMD_PAUSE == ctrlCmd)
            {
                m_SchedulerMachine->NotifyPause(SM_UNDEF);
                return;
            }

            m_SchedulerMachine->NotifyRVRodHeatingReady();
        }
    }
    else if (PSSM_FILLING_LEVELSENSOR_HEATING == stepState)
    {
        m_CurrentStepState = PSSM_FILLING_LEVELSENSOR_HEATING;

        if(CTRL_CMD_PAUSE == ctrlCmd)
        {
            SendProgramAcknowledge(SHOW_PAUSE_MSG_DLG);
            m_bWaitToPause = true;
        }

        if(mp_HeatingStrategy->CheckLevelSensorHeatingStatus())
        {
            m_IsProcessing = false;
            LogDebug("Program Step Heating Level sensor stage OK");
            if (m_bWaitToPause)
            {
                //dismiss the prompt of waiting for pause
                SendProgramAcknowledge(DISMISS_PAUSING_MSG_DLG);
                m_bWaitToPause = false;
                LogDebug(QString("Program Step Beginning Pause"));
                m_SchedulerMachine->NotifyPause(SM_UNDEF);
                return;
            }
            m_SchedulerMachine->NotifyLevelSensorHeatingReady();
        }
        else
        {
            // Do nothing, just wait for status of level sensor
        }
    }
    else if(PSSM_FILLING == stepState)
    {
        m_CurrentStepState = PSSM_FILLING;

        if(CTRL_CMD_PAUSE == ctrlCmd)
        {
            SendProgramAcknowledge(SHOW_PAUSE_MSG_DLG);
            m_bWaitToPause = true;
        }

        if( "Scheduler::ALFilling" == cmdName)
        {
            if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                RaiseEvent(EVENT_SCHEDULER_FILLING_SUCCESSFULLY);
                m_SchedulerMachine->NotifyFillFinished();
            }
            else
            {
                LogDebug(QString("Program Step Filling failed"));
                SendOutErrMsg(retCode);
            }
        }
    }
    else if(PSSM_RV_MOVE_TO_SEAL == stepState)
    {
        m_CurrentStepState = PSSM_RV_MOVE_TO_SEAL;

        if(CTRL_CMD_PAUSE == ctrlCmd)
        {
            SendProgramAcknowledge(SHOW_PAUSE_MSG_DLG);
            m_bWaitToPause = true;
        }

        if(IsRVRightPosition(SEAL_POS))
        {
            if(m_CurProgramStepIndex > 0)
            {
                //endTime
                CalculateTheGapTimeAndBufferTime(false, true);
            }
            else
            {
                LogDebug(QString("The step:%1 gap time:%2").arg(m_CurProgramStepIndex + 1).arg( m_EndTimeAndStepTime.GapTime));
            }

            if (m_bWaitToPause)
            {
                //dismiss the prompt of waiting for pause
                SendProgramAcknowledge(DISMISS_PAUSING_MSG_DLG);
                m_bWaitToPause = false;
                LogDebug(QString("Program Step Beginning Pause"));
                m_SchedulerMachine->NotifyPause(SM_UNDEF);
                return;
            }
            m_SchedulerMachine->NotifyRVMoveToSealReady();
        }
        else
        {
            if(("Scheduler::RVReqMoveToRVPosition" == cmdName))
            {
                if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
                {
                    SendOutErrMsg(retCode);
                }
            }
        }
    }
    else if(PSSM_PROCESSING == stepState)
    {
        m_CurrentStepState = PSSM_PROCESSING;
        if(CTRL_CMD_PAUSE == ctrlCmd)
        {
            if(m_CurProgramStepInfo.isPressure || m_CurProgramStepInfo.isVacuum)
            {
                AllStop();
                m_lastPVTime = 0;
            }
            LogDebug(QString("Program Step Beginning Pause"));
            m_SchedulerMachine->NotifyPause(PSSM_PROCESSING);
        }
        else
        {
            if( "RG6" == m_CurProgramStepInfo.reagentGroup && IsLastStep(m_CurProgramStepIndex, m_CurProgramID) )
            {
                if(m_EndTimeAndStepTime.WarningFlagForStepTime)
                {
                    SendOutErrMsg(DCL_ERR_DEV_LA_PROCESS_INTERVAL_TIMEOUT_4MIN, false);
                    m_EndTimeAndStepTime.WarningFlagForStepTime = false;
                }
                else if(m_EndTimeAndStepTime.WarningFlagForBufferTime)
                {
                    SendOutErrMsg(DCL_ERR_DEV_LA_ENDTIME_TIMEOUT, false);
                    m_EndTimeAndStepTime.WarningFlagForBufferTime = false;
                }
            }
            qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();/*lint -e647 */
            qint64 period = m_CurProgramStepInfo.durationInSeconds * 1000;
            if ("RG6" == m_CurProgramStepInfo.reagentGroup && IsLastStep(m_CurProgramStepIndex, m_CurProgramID))
            {
                period += m_EndTimeAndStepTime.BufferTime;
            }
            if((now - m_TimeStamps.CurStepSoakStartTime ) > period)//Will finish Soaking
            {
                if(!m_IsReleasePressureOfSoakFinish)
                {
                    m_SchedulerCommandProcessor->pushCmd(new CmdALReleasePressure(500,  this));
                    m_IsReleasePressureOfSoakFinish = true;
                }
                else if("Scheduler::ALReleasePressure" == cmdName)
                {
                    if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                    {
                        //if it is Cleaning program, need not notify user
                        if((m_CurProgramID.at(0) != 'C') && IsLastStep(m_CurProgramStepIndex, m_CurProgramID))
                        {
                            //this is last step, need to notice user
                            if(!m_completionNotifierSent)
                            {
                                MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_WILL_COMPLETE));
                                Q_ASSERT(commandPtrFinish);
                                Global::tRefType fRef = GetNewCommandRef();
                                SendCommand(fRef, Global::CommandShPtr_t(commandPtrFinish));
                                m_completionNotifierSent = true;
                            }
                        }
                        else
                        {
                            LogDebug(QString("Program Processing(Soak) Process finished"));
                            m_SchedulerMachine->NotifyProcessingFinished();
                            m_TimeStamps.CurStepSoakStartTime = 0;
                            m_IsProcessing = false;
                        }
                    }
                    else
                    {
                        SendOutErrMsg(retCode);
                    }
                }
                if(CTRL_CMD_DRAIN == ctrlCmd && m_completionNotifierSent)
                {
                    LogDebug(QString("last Program Processing(Soak) Process finished"));
                    m_SchedulerMachine->NotifyProcessingFinished();
                    m_TimeStamps.CurStepSoakStartTime = 0;
                    m_completionNotifierSent = false;
                    m_IsProcessing = false;
                }
            }
            else // Begin to Soak
            {
                if (now > m_TimeStamps.ProposeSoakStartTime)
                {
                    if(m_IsInSoakDelay)
                    {
                        if(m_CurProgramStepInfo.isPressure ^ m_CurProgramStepInfo.isVacuum)
                        {
                            if(m_CurProgramStepInfo.isPressure)
                            {
                                m_ProcessingPV = 0;
                                Pressure();
                            }else if(m_CurProgramStepInfo.isVacuum)
                            {
                                m_ProcessingPV = 1;
                                Vaccum();
                            }
                            m_lastPVTime = now;
                        }
                        m_IsInSoakDelay = false;
                    }
                    else if(m_CurProgramStepInfo.isPressure && m_CurProgramStepInfo.isVacuum)
                    {
                        // P/V take turns in 1 minute
                        if((now - m_lastPVTime)>60000)
                        {
                            if(((now - m_TimeStamps.CurStepSoakStartTime)/60000)%2 == 0)
                            {
                                m_ProcessingPV = 0;
                                Pressure();
                            }
                            else
                            {
                                m_ProcessingPV = 1;
                                Vaccum();
                            }
                            m_lastPVTime = now;
                        }
                    }

                    // Check if Pressure or Vacuum operation reaches abs(25) in 30 seconds
                    if ((now - m_lastPVTime) > 30*1000 && m_lastPVTime != 0)
                    {
                        if (qAbs(m_PressureAL) < 25.0)
                        {
                            if (0 == m_ProcessingPV) // for Pressure
                            {
                                m_ProcessingPV = 3; // Avoid the warning message to pop up too many times
                                SendOutErrMsg(DCL_ERR_DEV_LA_SEALING_FAILED_PRESSURE, false);
                            }
                            else if (1 == m_ProcessingPV)
                            {
                                m_ProcessingPV = 3; // Avoid the warning message to pop up too many times
                                SendOutErrMsg(DCL_ERR_DEV_LA_SEALING_FAILED_VACUUM, false);
                            }
                        }
                    }

                }
            }
        }
    }
    else if(PSSM_RV_MOVE_TO_TUBE == stepState)
    {
        m_CurrentStepState = PSSM_RV_MOVE_TO_TUBE;

        if(CTRL_CMD_PAUSE == ctrlCmd)
        {
            SendProgramAcknowledge(SHOW_PAUSE_MSG_DLG);
            m_bWaitToPause = true;
        }
        m_SchedulerMachine->HandlePssmMoveTubeWorkflow(cmdName, retCode);
    }
    else if(PSSM_DRAINING == stepState)
    {
        m_CurrentStepState = PSSM_DRAINING;

        if(CTRL_CMD_PAUSE == ctrlCmd)
        {
            SendProgramAcknowledge(SHOW_PAUSE_MSG_DLG);
            m_bWaitToPause = true;
        }

        //In case that Scheduler was recovered from Error
        if( "Scheduler::ALDraining"== cmdName)
        {
            LogDebug("ALDraining has been got in Scheduler");
            if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                RaiseEvent(EVENT_SCHEDULER_DRAINING_SUCCESSFULLY);
                if (m_bWaitToPause)
                {
                     //dismiss the prompt of waiting for pause
                    SendProgramAcknowledge(DISMISS_PAUSING_MSG_DLG);
                    m_bWaitToPause = false;
                    LogDebug(QString("Program Step Beginning Pause"));
                    m_SchedulerMachine->NotifyPause(SM_UNDEF);
                    return;
                }
                m_SchedulerMachine->NotifyDrainFinished();
            }
            else
            {
                LogDebug(QString("Program Step Draining Build Pressure timeout"));
                SendOutErrMsg(retCode);
            }
        }
    }
    else if(PSSM_RV_POS_CHANGE == stepState)
    {
        m_CurrentStepState = PSSM_RV_POS_CHANGE;
        if(CTRL_CMD_PAUSE == ctrlCmd)
        {
            SendProgramAcknowledge(SHOW_PAUSE_MSG_DLG);
            m_bWaitToPause = true;
        }

        if(IsRVRightPosition(NEXT_TUBE_POS))
        {
            if (m_bWaitToPause)
            {
                //dismiss the prompt of waiting for pause
                SendProgramAcknowledge(DISMISS_PAUSING_MSG_DLG);
                m_bWaitToPause = false;
                LogDebug(QString("Program Step Beginning Pause"));
                m_SchedulerMachine->NotifyPause(SM_UNDEF);
                return;
            }

            m_SchedulerMachine->NotifyStepFinished();
        }
        else
        {
            if("Scheduler::RVReqMoveToRVPosition" == cmdName)
            {
                if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
                {
                    SendOutErrMsg(retCode);
                }
            }
        }
    }
    else if(PSSM_STEP_PROGRAM_FINISH == stepState)
    {
        m_CurrentStepState = PSSM_STEP_PROGRAM_FINISH;
        if (0 == m_PssmStepFinSeq)
        {
            m_UnknownErrorLogVector.clear();
            RaiseEvent(EVENT_SCHEDULER_PROGRAM_STEP_FINISHED,QStringList()<<QString("[%1]").arg(m_CurProgramStepIndex));
            m_PssmStepFinSeq++;
        }
        else if (1 == m_PssmStepFinSeq)
        {
            (void)this->GetNextProgramStepInformation(m_CurProgramID, m_CurProgramStepInfo);
            if(m_CurProgramStepIndex != -1)
            {
                m_ProgramStatusInfor.SetStepID(m_CurProgramStepIndex); ///For Powerfailure:store current step id
                m_ProgramStatusInfor.SetStationID(m_CurProgramStepInfo.stationID);
                m_SchedulerMachine->NotifyStepProgramFinished();
            }
            else
            {
                if(m_IsCleaningProgram)
                { // need run cleaning dry step
                    m_SchedulerMachine->NotifyEnterCleaningDryStep();
                }
                else
                {
                    m_SchedulerMachine->NotifyProgramFinished();
                }

            }
            m_PssmStepFinSeq = 0;
        }
    }
    else if(PSSM_CLEANING_DRY_STEP == stepState)
    {
        m_CurrentStepState = PSSM_CLEANING_DRY_STEP;
        DoCleaningDryStep(ctrlCmd, cmd);
    }
    else if(PSSM_PROGRAM_FINISH == stepState)
    {
        m_CurrentStepState = PSSM_PROGRAM_FINISH;

        //program finished
        QString ProgramName = mp_DataManager->GetProgramList()->GetProgram(m_CurProgramID)->GetName();
        RaiseEvent(EVENT_SCHEDULER_PROGRAM_FINISHED,QStringList()<<ProgramName);
        m_SchedulerMachine->SendRunComplete();

        //send command to main controller to tell the left time
        QTime leftTime(0,0,0);
        MsgClasses::CmdCurrentProgramStepInfor* commandPtr(new MsgClasses::CmdCurrentProgramStepInfor(5000, "", m_CurProgramStepIndex, 0));
        Q_ASSERT(commandPtr);
        Global::tRefType Ref = GetNewCommandRef();
        SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

        if(m_ProgramStatusInfor.IsRetortContaminted())
        {
            //send command to main controller to tell program finished
            MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_FINISHED));
            Q_ASSERT(commandPtrFinish);
            Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtrFinish));
        }
        else
        {
            MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_FINISHED_NO_CONTAMINATED));
            Q_ASSERT(commandPtrFinish);
            Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtrFinish));
            m_ProgramStatusInfor.SetProgramFinished();
        }

        if(m_IsCleaningProgram)
        {
            m_IsTakeSpecimen = true;
            m_IsCleaningProgram = false;
            m_ProgramStatusInfor.SetProgramFinished();
        }
    }
    else if(PSSM_PAUSE == stepState)
    {
        m_CurrentStepState = PSSM_PAUSE;
        if(CTRL_CMD_START == ctrlCmd)
        {
            // resume the program
            emit NotifyResume();
            LogDebug("The program is resume from pasue.");

            // tell the main controller the program is resuming
            MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_BEGIN));
            Q_ASSERT(commandPtrFinish);
            Global::tRefType fRef = GetNewCommandRef();
            SendCommand(fRef, Global::CommandShPtr_t(commandPtrFinish));
        }
        else if(CTRL_CMD_USER_RESPONSE_PAUSE_ALARM == ctrlCmd)//get from user's response and EventConfig.xml
        {
            m_TimeStamps.PauseStartTime = QDateTime::currentMSecsSinceEpoch();
            m_Is5MinPause = false;
            m_Is10MinPause = false;
        }
        else
        {
            qint64 now = QDateTime::currentMSecsSinceEpoch();
            if(!m_Is5MinPause && (now - m_TimeStamps.PauseStartTime) >= 5 * 60* 1000 &&
                    (now - m_TimeStamps.PauseStartTime) < 10 * 60* 1000)
            {
                m_Is5MinPause = true;
                Global::EventObject::Instance().RaiseEvent(EVENT_DEVICE_ALARM_PAUSE_5MINTUES);
            }
            else if(!m_Is10MinPause && (now - m_TimeStamps.PauseStartTime) >= 10 * 60* 1000 &&
                     (now - m_TimeStamps.PauseStartTime) < 15 * 60* 1000)
            {
                m_Is10MinPause = true;
                Global::EventObject::Instance().RaiseEvent(EVENT_LOCAL_ALARM_PAUSE_10MINTUES);
            }
            else if(!m_Is15MinPause && (now - m_TimeStamps.PauseStartTime) >= 15 * 60* 1000)
            {
                m_Is15MinPause = true;

                ProgramAcknownedgeType_t type =  DataManager::PROGRAM_PAUSE_TIMEOUT_15MINTUES;
                MsgClasses::CmdProgramAcknowledge* commandPtrPauseEnable(new MsgClasses::CmdProgramAcknowledge(5000, type));
                Q_ASSERT(commandPtrPauseEnable);
                Global::tRefType fRef = GetNewCommandRef();
                SendCommand(fRef, Global::CommandShPtr_t(commandPtrPauseEnable));

                Global::EventObject::Instance().RaiseEvent(EVENT_REMOTE_ALARM_PAUSE_15MINTUES);
            }

        }

    }
    else if (PSSM_BOTTLE_CHECK == stepState)
    {
        m_CurrentStepState = PSSM_BOTTLE_CHECK;
        m_SchedulerMachine->HandlePssmBottleCheckWorkFlow(cmdName, retCode);
    }
}

void SchedulerMainThreadController::HandleErrorState(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd, SchedulerStateMachine_t currentState)
{
    ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;
    QString cmdName = "";

    if (false == cmd.isNull())
    {
        if(!(cmd->GetResult(retCode)))
        {
            retCode = DCL_ERR_UNDEFINED;
        }
        cmdName = cmd->GetName();
    }

    if (SM_ERR_WAIT == currentState && CTRL_CMD_NONE != ctrlCmd)
    {
        if(CTRL_CMD_RC_RESTART == ctrlCmd)
        {
            m_ProgramStatusInfor.SetErrorFlag(0);
            m_SchedulerMachine->EnterRcRestart();
        }
        if (CTRL_CMD_RC_REPORT == ctrlCmd)
        {
            //LogDebug("Go to RC_Report");
        }
        else if(CTRL_CMD_RS_RV_GET_ORIGINAL_POSITION_AGAIN == ctrlCmd)
        {
            m_SchedulerMachine->EnterRsRVGetOriginalPositionAgain();
        }
        else if(CTRL_CMD_RS_STANDBY == ctrlCmd)
        {
            m_SchedulerMachine->EnterRsStandBy();
        }
        else if(CTRL_CMD_RS_STANDBY_WITHTISSUE == ctrlCmd)
        {
            m_SchedulerMachine->EnterRsStandByWithTissue();
        }
        else if (CTRL_CMD_RS_HEATINGERR30SRETRY == ctrlCmd)
        {
            m_SchedulerMachine->EnterRsHeatingErr30SRetry();
        }
        else if (CTRL_CMD_RS_PRESSUREOVERRANGE_3SRETRY == ctrlCmd)
        {
            m_SchedulerMachine->EnterRsPressureOverRange3SRetry();
        }
        else if(CTRL_CMD_RS_TSENSORERR3MINRETRY == ctrlCmd)
        {
            m_SchedulerMachine->EnterRsTSensorErr3MinRetry();
        }
        else if(CTRL_CMD_RC_LEVELSENSOR_HEATING_OVERTIME == ctrlCmd)
        {
            m_SchedulerMachine->EnterRcLevelsensorHeatingOvertime();
        }
        else if(CTRL_CMD_RC_PRESSURE == ctrlCmd)
        {
            m_SchedulerMachine->EnterRcPressure();
        }
        else if(CTRL_CMD_RC_VACUUM == ctrlCmd)
        {
            m_SchedulerMachine->EnterRcVacuum();
        }
        else if(CTRL_CMD_RC_FILLING == ctrlCmd)
        {
            m_SchedulerMachine->EnterRcFilling();
        }
        else if(CTRL_CMD_RC_DRAINING == ctrlCmd)
        {
            m_SchedulerMachine->EnterRcDraining();
        }
        else if(CTRL_CMD_RS_DRAINATONCE == ctrlCmd)
        {
            m_SchedulerMachine->EnterRsDrainAtOnce();
        }
        else if(CTRL_CMD_RC_BOTTLECHECK_I == ctrlCmd)
        {
            m_SchedulerMachine->EnterRcBottleCheckI();
        }
        else if(CTRL_CMD_RS_FILLINGAFTERFLUSH == ctrlCmd)
        {
            m_SchedulerMachine->EnterRsFillingAfterFlush();
        }
        else if(CTRL_CMD_RS_CHECK_BLOCKAGE == ctrlCmd)
        {
            m_SchedulerMachine->EnterRsCheckBlockage();
        }
        else if(CTRL_CMD_RS_PAUSE == ctrlCmd)
        {
            m_SchedulerMachine->EnterRsPause();
        }
        else if(CTRL_CMD_RS_RV_WAITINGTEMPUP == ctrlCmd)
        {
            m_SchedulerMachine->EnterRsRVWaitingTempUp();
        }
        else if (CTRL_CMD_RS_TISSUE_PROTECT == ctrlCmd)
        {
            m_SchedulerMachine->EnterRsTissueProtect();
        }
        else if (CTRL_CMD_RC_CHECK_RTLOCK == ctrlCmd)
        {
            m_SchedulerMachine->EnterRcCheckRTLock();
        }
        else if (CTRL_CMD_RC_REHEATING == ctrlCmd)
        {
            m_SchedulerMachine->EnterRcReHeating(m_ProgramStatusInfor.GetScenario(), m_ProgramStatusInfor.GetLastReagentGroup(), false);
        }
        else if (CTRL_CMD_RC_REHEATING_CLEANING == ctrlCmd)
        {
            m_SchedulerMachine->EnterRcReHeating(m_ProgramStatusInfor.GetScenario(), m_ProgramStatusInfor.GetLastReagentGroup(), true);
        }
        else if (CTRL_CMD_RS_REAGENTCHECK == ctrlCmd)
        {
            m_SchedulerMachine->EnterRsReagentCheck();
        }
        else if (CTRL_CMD_RS_RV_MOVETOSEALPOSITION == ctrlCmd)
        {
            m_SchedulerMachine->EnterRsRVMoveToSealPosition();
        }
        else if (CTRL_CMD_RS_ABORT == ctrlCmd)
        {
            m_ProgramStatusInfor.SetErrorFlag(0);
            m_SchedulerMachine->EnterRsAbort();
        }
        else
        {
        }
    }
    else if (SM_ERR_RS_STANDBY == currentState)
    {
        m_SchedulerMachine->HandleRsStandByWorkFlow(cmdName, retCode);
    }
    else if (SM_ERR_RS_HEATINGERR30SRETRY == currentState)
    {
        m_SchedulerMachine->HandleRsHeatingErr30SRetry(cmdName, retCode);
    }
    else if (SM_ERR_RS_PRESSUREOVERRANGE3SRETRY == currentState)
    {
        m_SchedulerMachine->HandleRsPressureOverRange3SRetry(cmdName, retCode);
    }
    else if(SM_ERR_RS_TSENSORERR_3MINRETRY == currentState)
    {
        m_SchedulerMachine->HandleRsTSensorErr3MinRetry(cmdName, retCode);
    }
    else if (SM_ERR_RS_STANDBY_WITH_TISSUE == currentState)
    {
        m_SchedulerMachine->HandleRsStandByWithTissueWorkFlow(cmdName, retCode);
    }
    else if (SM_ERR_RC_LEVELSENSOR_HEATING_OVERTIME == currentState)
    {
        m_SchedulerMachine->HandleRcLevelSensorHeatingOvertimeWorkFlow();

    }
    else if(SM_ERR_RS_RV_GETORIGINALPOSITIONAGAIN == currentState)
    {
        m_SchedulerMachine->HandleRsRVGetOriginalPositionAgainWorkFlow(cmdName, retCode);
    }
    else if(SM_ERR_RC_PRESSURE == currentState)
    {
         m_SchedulerMachine->HandleRcPressureWorkFlow(cmdName, retCode);
    }
    else if(SM_ERR_RC_VACUUM == currentState)
    {
         m_SchedulerMachine->HandleRcVacuumWorkFlow(cmdName, retCode);
    }
    else if(SM_ERR_RC_FILLING == currentState)
    {
         m_SchedulerMachine->HandleRcFillingWorkFlow(cmdName, retCode);
    }
    else if(SM_ERR_RC_DRAINING == currentState)
    {
         m_SchedulerMachine->HandleDrainingWorkFlow(cmdName, retCode);
    }
    else if(SM_ERR_RS_DRAINATONCE == currentState)
    {
        m_SchedulerMachine->HandleDrainingWorkFlow(cmdName, retCode);
    }
    else if(SM_ERR_RS_BOTTLECHECK_I == currentState)
    {
        m_SchedulerMachine->HandleRcBottleCheckIWorkFlow(cmdName, retCode);
    }
    else if(SM_ERR_RS_FILLINGAFTERFFLUSH == currentState)
    {
        m_SchedulerMachine->HandleRsFillingAfterFlushWorkFlow(cmdName, retCode);
    }
    else if(SM_ERR_RS_CHECK_BLOCKAGE == currentState)
    {
        m_SchedulerMachine->HandleRsCheckBlockageWorkFlow(cmdName, retCode);
    }
    else if(SM_ERR_RS_PS_PAUSE == currentState)
    {
        m_SchedulerMachine->HandleRsPauseWorkFlow(ctrlCmd);
    }
    else if(SM_ERR_RS_RV_WAITINGTEMPUP == currentState)
    {
        m_SchedulerMachine->HandleRsRVWaitingTempUpWorkFlow(cmdName, retCode);
    }
    else if (SM_ERR_RS_TISSUE_PROTECT == currentState)
    {
        m_SchedulerMachine->HandleRsTissueProtectWorkFlow(cmdName, retCode, ctrlCmd);
    }
    else if (SM_ERR_RC_CHECK_RTLOCK == currentState)
    {
        m_SchedulerMachine->HandleRcCheckRTLockWorkFlow();
    }
    else if (SM_ERR_RC_REHEATING == currentState)
    {
        m_SchedulerMachine->HandleRcReHeatingWorkFlow(cmdName, retCode);
    }
    else if(SM_ERR_RS_RV_MOVETOPOSITIONSEAL == currentState)
    {
        m_SchedulerMachine->HandleRsMoveToPSeal(cmdName, retCode);
    }
    else if (SM_ERR_RC_RESTART == currentState)
    {
        m_SchedulerMachine->HandleRcRestart(cmdName);
        //error restart should reheating the scenario, so the heating strategy scenario should reset
        mp_HeatingStrategy->SetHeatingStrategyScenario(0);
    }
    else if (SM_ERR_RS_ABORT == currentState)
    {
        m_SchedulerMachine->HandleRsAbortWorkFlow(cmdName, retCode);
    }
}

ControlCommandType_t SchedulerMainThreadController::PeekNonDeviceCommand()
{

    if(m_SchedulerCmdQueue.isEmpty())
        return CTRL_CMD_NONE;
    m_Mutex.lock();
    Global::CommandShPtr_t pt = m_SchedulerCmdQueue.dequeue();
    m_Mutex.unlock();

    MsgClasses::CmdQuitAppShutdown* pCmdShutdown = dynamic_cast<MsgClasses::CmdQuitAppShutdown*>(pt.GetPointerToUserData());
    if(pCmdShutdown)
    {	
		return CTRL_CMD_SHUTDOWN;
    }

    MsgClasses::CmdBottleCheck* pCmdBottleCheck = dynamic_cast<MsgClasses::CmdBottleCheck*>(pt.GetPointerToUserData());
    if(pCmdBottleCheck)
    {
        m_CheckOvenCover = true;
        return CTRL_CMD_BOTTLE_CHECK;
    }

    MsgClasses::CmdTakeOutSpecimenFinished* pCmdTakeOutSpecimenFinished = dynamic_cast<MsgClasses::CmdTakeOutSpecimenFinished*>(pt.GetPointerToUserData());
    if(pCmdTakeOutSpecimenFinished)
    {
        return CTRL_CMD_TAKEOUT_SPECIMEN_FINISH;
    }

    MsgClasses::CmdProgramAction* pCmdProgramAction = dynamic_cast<MsgClasses::CmdProgramAction*>(pt.GetPointerToUserData());
    if(pCmdProgramAction)
    {
        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_START)
        {
            m_NewProgramID = pCmdProgramAction->GetProgramID();
            m_delayTime = pCmdProgramAction->DelayTime();
            m_ReagentExpiredFlag = pCmdProgramAction->GetReagentExpiredFlag();
            LogDebug(QString("Get the delay time: %1 seconds.").arg(m_delayTime));
            return CTRL_CMD_START;
        }
        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_PAUSE)
        {
            return CTRL_CMD_PAUSE;
        }
        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_ABORT)
        {
            return CTRL_CMD_ABORT;
        }
        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_DRAIN)
        {
            return CTRL_CMD_DRAIN;
        }
        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_DRAIN_SR)
        {
            return CTRL_CMD_DRAIN_SR;
        }
        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_OVEN_COVER_OPEN)
        {
            return CTRL_CMD_OVEN_COVER_OPEN;
        }
        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_RETORT_COVER_OPEN)
        {
            return CTRL_CMD_RETORT_COVER_OPEN;
        }
        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_POWER_FAILURE_MSG)
        {
            return CTRL_CMD_POWER_FAILURE_MEG;
        }
    }

    NetCommands::CmdSystemAction* pCmdSystemAction = dynamic_cast<NetCommands::CmdSystemAction*>(pt.GetPointerToUserData());
    if(pCmdSystemAction)
    {
        QString cmd = pCmdSystemAction->GetActionString();

        LogDebug(QString("Get action: %1").arg(cmd));
        cmd = cmd.toLower();

        if(cmd == "rs_rv_getoriginalpositionagain")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RS_RV_GET_ORIGINAL_POSITION_AGAIN;
        }
        if(cmd == "rc_restart")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RC_RESTART;
        }
        if (cmd == "rc_report")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RC_REPORT;
        }
        if(cmd == "rs_standby")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RS_STANDBY;
        }
        if(cmd == "rs_standby_withtissue")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RS_STANDBY_WITHTISSUE;
        }
        if (cmd == "rs_heatingerr30sretry")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RS_HEATINGERR30SRETRY;
        }
        if (cmd == "rs_pressureoverrange_3sretry")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RS_PRESSUREOVERRANGE_3SRETRY;
        }
        if (cmd == "rs_tsensorerr3minretry")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RS_TSENSORERR3MINRETRY;
        }
        if (cmd == "rc_levelsensor_heating_overtime")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RC_LEVELSENSOR_HEATING_OVERTIME;
        }
        if (cmd == "rc_pressure")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RC_PRESSURE;
        }
        if (cmd == "rc_vacuum")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RC_VACUUM;
        }
        if (cmd == "rc_filling")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RC_FILLING;
        }
        if (cmd == "rc_draining")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RC_DRAINING;
        }
        if (cmd == "rs_drainatonce")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RS_DRAINATONCE;
        }
        if (cmd == "rc_bottlecheck_i")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RC_BOTTLECHECK_I;
        }
        if (cmd == "rs_fillingafterflush")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RS_FILLINGAFTERFLUSH;
        }
        if (cmd == "rs_check_blockage")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RS_CHECK_BLOCKAGE;
        }
        if (cmd == "rs_pause")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RS_PAUSE;
        }
        if (cmd == "rs_rv_waitingtempup")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RS_RV_WAITINGTEMPUP;
        }
        if (cmd == "rs_tissue_protect")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RS_TISSUE_PROTECT;
        }
        if (cmd == "user_response_pause_alarm")
        {
            //warning action not need eventkey
            //m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_USER_RESPONSE_PAUSE_ALARM;
        }
        if (cmd == "rc_check_rtlock")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RC_CHECK_RTLOCK;
        }
        if (cmd == "rc_reheating")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RC_REHEATING;
        }
        if (cmd == "rc_reheating_cleaning")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RC_REHEATING_CLEANING;
        }
        if (cmd == "rs_reagentcheck")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RS_REAGENTCHECK;
        }
        if (cmd == "rs_rv_movetoposition3.5")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RS_RV_MOVETOSEALPOSITION;
        }
        if (cmd == "rs_checkremotealarmstatus")
        {
            //warning action not need eventkey
            //m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RS_CHECKREMOTEALARMSTATUS;
        }
        if (cmd == "rs_checklocalalarmstatus")
        {
            //warning action not need eventkey
            //m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RS_CHECKLOCALALARMSTATUS;
        }
        if (cmd == "rs_abort")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RS_ABORT;
        }
        if (cmd == "checkovencover")
        {
            //warning action not need eventkey
            //m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_CHECK_OVEN_COVER;
        }
        if (cmd == "open_oven_change_heating_paraffin")
        {
            //warning action not need eventkey
            //m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_OPEN_OVEN_CHANGE_HEATING_PARAFFIN;
        }
        if (cmd.startsWith("ALARM_", Qt::CaseInsensitive))
        {
            QString str = cmd;
            int opcode = str.remove("ALARM_", Qt::CaseInsensitive).toInt();
            switch (opcode) {
            case 5:
                return  CTRL_CMD_ALARM_RMT_ON;
            case 4:
                return  CTRL_CMD_ALARM_RMT_OFF;
            case 3:
                return  CTRL_CMD_ALARM_LOC_ON;
            case 2:
                return  CTRL_CMD_ALARM_LOC_OFF;
            case -1:
                return  CTRL_CMD_ALARM_ALL_OFF;
            }
        }
        if(cmd == "rs_shutdown")
        {
            return CTRL_CMD_RS_SHUTDOWN;
        }
    }
    return CTRL_CMD_UNKNOWN;

}//When end of this function, the command in m_SchedulerCmdQueue will be destrory by share pointer (CommandShPtr_t) mechanism

bool SchedulerMainThreadController::GetNextProgramStepInformation(const QString& ProgramID,
                                                                  ProgramStepInfor& programStepInfor,
                                                                  bool onlyGetFirstProgramStepIndex)
{
    if (ProgramID.isNull() ||  ProgramID.isEmpty())
    {
        return false;
    }
    if (!mp_DataManager)
    {
        Q_ASSERT(false);
        return false;//need error handling
    }

    CDataProgramList* pDataProgramList = mp_DataManager->GetProgramList();
    if (!pDataProgramList)
    {
        Q_ASSERT(false);
        return false;
    }

    CProgram* pProgram = const_cast<CProgram*>(pDataProgramList->GetProgram(ProgramID));
    if (!pProgram)
        return false;

    const CProgramStep* pProgramStep(NULL);
    QString reagentID("");
    bool bSkipCurrentStep = false;
    int count = pProgram->GetNumberOfSteps();
    do
    {
        if (-1 == m_CurProgramStepIndex)
        {
            m_CurProgramStepIndex = 0;
        }
        else
        {
            ++m_CurProgramStepIndex;
        }

        if (m_CurProgramStepIndex > count && bSkipCurrentStep)
        {
            m_CurProgramStepIndex = -1;//cannot find the needed step
            return false;
        }

        pProgramStep = pProgram->GetProgramStep(m_CurProgramStepIndex);//use order index
        if (pProgramStep)
        {
            reagentID = pProgramStep->GetReagentID();
            //Check reagent compatible
            if (ProgramID.at(0) == 'C')
            {
                if (CDataReagentList* pReagentList =  mp_DataManager->GetReagentList())
                {
                    const CReagent* pCurReagent = pReagentList->GetReagent(reagentID);
                    QString strLastReagentGroupId = m_ProgramStatusInfor.GetLastReagentGroup();
                    //RG7:Cleaning Solvent, RG8:Cleaning Alcohol
                    QStringList list;
                    list << "RG1"<<"RG2"<<"RG3"<<"RG4"<<"RG5";
                    if (list.contains(strLastReagentGroupId) && pCurReagent->GetGroupID() == "RG7")
                    {
                        bSkipCurrentStep = true;
                    }
                    else
                        bSkipCurrentStep = false;
                }
            }
        }
    } while (bSkipCurrentStep);
    
    if (onlyGetFirstProgramStepIndex)
        return true;
        
    if (pProgramStep)
    {
        programStepInfor.stationID  = this->GetStationIDFromProgramStep(m_CurProgramStepIndex);
        int soakTime = pProgramStep->GetDurationInSeconds();
        if (0 == m_CurProgramStepIndex && m_delayTime > 0)
        {
            soakTime = soakTime + m_delayTime;
        }
        programStepInfor.durationInSeconds = soakTime;
        programStepInfor.temperature = pProgramStep->GetTemperature().toInt();
        programStepInfor.isPressure = (pProgramStep->GetPressure() == "On");
        programStepInfor.isVacuum = (pProgramStep->GetVacuum() == "On");
        m_CurReagnetName = GetReagentName(reagentID);
        programStepInfor.reagentGroup = GetReagentGroupID(reagentID);
        //next station
        if((m_CurProgramStepIndex + 1) == count)
        {
            programStepInfor.nextStationID = programStepInfor.stationID;
        }
        else
        {
            programStepInfor.nextStationID = this->GetStationIDFromProgramStep(m_CurProgramStepIndex + 1);
        }
    }
    else
    {
        m_CurProgramStepIndex = -1;
    }
    return true;
}

QString SchedulerMainThreadController::GetStationIDFromProgramStep(int ProgramStepIndex)
{
    ProgramStepIndex = ProgramStepIndex - m_FirstProgramStepIndex;

    if (ProgramStepIndex < 0 || (ProgramStepIndex > m_StationList.size()-1) || m_StationList.empty())
    {
        return "";
    }
    return m_StationList.at(ProgramStepIndex);
}

QString SchedulerMainThreadController::SelectStationFromReagentID(const QString& ReagentID, ListOfIDs_t& unusedStationIDs,
                                                                 QList<StationUseRecord_t>& usedStations, bool isLastStep)
{
    if (!mp_DataManager)
        return "";

    DataManager::CHimalayaUserSettings* pUserSetting = mp_DataManager->GetUserSettings();
    Global::RMSOptions_t rmsMode = Global::RMS_UNDEFINED;
    if (this->IsCleaningReagent(ReagentID))
    {
        rmsMode = pUserSetting->GetModeRMSCleaning();
    }
    else
    {
        rmsMode = pUserSetting->GetModeRMSProcessing();
    }

    if (CDataReagentList* pReagentList =  mp_DataManager->GetReagentList())
    {
        const CReagent* pReagent = pReagentList->GetReagent(ReagentID);
        return this->SelectStationByReagent(pReagent,  unusedStationIDs, usedStations, isLastStep, rmsMode);
    }

    return "";
}


QString SchedulerMainThreadController::SelectStationByReagent(const CReagent* pReagent,
                                                                    ListOfIDs_t& unusedStationIDs,
                                                                    QList<StationUseRecord_t>& usedStations,
                                                                    bool bFindNewestOne,
                                                                    Global::RMSOptions_t rmsMode) const
{
    Q_UNUSED(bFindNewestOne);
    Q_UNUSED(usedStations);
    Q_UNUSED(rmsMode);

    if (!pReagent)
        return "";

    CDashboardDataStationList* pDashboardDataStationList = mp_DataManager->GetStationList();
    if (!pDashboardDataStationList)
        return "";

    CDataReagentGroupList* pDataReagentGroupList = mp_DataManager->GetReagentGroupList();
    if (!pDataReagentGroupList)
        return "";

    const QString& ReagentID = pReagent->GetReagentID();
    bool isCleaningReagent = pDataReagentGroupList->GetReagentGroup(pReagent->GetGroupID())->IsCleaningReagentGroup();

    for (int i = 0; i < unusedStationIDs.count(); i++)
    {
        CDashboardStation* pDashboardStation = pDashboardDataStationList->GetDashboardStation(unusedStationIDs.at(i));
        if (pDashboardStation->GetDashboardReagentID() == ReagentID)
        {
                if (!isCleaningReagent)
                    unusedStationIDs.removeAt(i);
                return pDashboardStation->GetDashboardStationID();
        }
    }
    return "";
}

bool SchedulerMainThreadController::PrepareProgramStationList(const QString& ProgramID, int beginStep)
{
    if (ProgramID.isEmpty() || ProgramID.isNull())
    {
        return false;
    }

    if (!mp_DataManager)
    {
        Q_ASSERT(false);
        return false;//need error handling
    }

    CDataProgramList* pDataProgramList = mp_DataManager->GetProgramList();
    if (!pDataProgramList)
    {
        Q_ASSERT(false);
        return false;
    }

    CDashboardDataStationList* pDashboardDataStationList = mp_DataManager->GetStationList();
    if (!pDashboardDataStationList)
    {
        Q_ASSERT(false);
        return false;
    }

    CProgram* pProgram = const_cast<CProgram*>(pDataProgramList->GetProgram(ProgramID));

    bool isLastStep = false;

    m_StationList.clear();
    m_StationAndReagentList.clear();

    ProgramStationInfo_t stationInfo;

    ListOfIDs_t unusedStationIDs = pDashboardDataStationList->GetOrderedListOfDashboardStationIDs();
    QList<StationUseRecord_t> usedStations;

    for (int i = beginStep; i < pProgram->GetNumberOfSteps(); i++)
    {
        const CProgramStep* pProgramStep = pProgram->GetProgramStep(i);//use order index

        isLastStep = (i == (pProgram->GetNumberOfSteps() - 1));
        QString reagentID = pProgramStep->GetReagentID();
        stationInfo.ReagentGroupID =GetReagentGroupID(reagentID);
        stationInfo.StationID = this->SelectStationFromReagentID(reagentID, unusedStationIDs, usedStations, isLastStep);
        m_StationList.push_back(stationInfo.StationID);
        m_StationAndReagentList.push_back(stationInfo);
    }
    return true;
}

bool SchedulerMainThreadController::GetSafeReagentForSpecial(int index, QString& reagentGroupID, QList<QString>& stationList)
{
    if(index >= m_StationAndReagentList.size())
        return false;
    ProgramStationInfo_t stationInfo = m_StationAndReagentList.at(index);
    reagentGroupID = stationInfo.ReagentGroupID;
    stationList.push_back(stationInfo.StationID);
    return true;
}

/**
 * @brief For the given reagent group, based some rules, get the station list of Safe Reagent
 * @param reagentGroupID the given reagent group.
 * @param stationList The station list contains the stations fitting the requirements.
 * @return indicate Whether or not this function executes successfully
 */
bool SchedulerMainThreadController::GetSafeReagentStationList(const QString& reagentGroupID, QList<QString>& stationList)
{
//    QList<QString> fixationStationList, dehydratingAbsstationList;
    CDataReagentList* pReagentList =  mp_DataManager->GetReagentList();
    if (!pReagentList)
        return false;

    CDashboardDataStationList* pDashboardDataStationList = mp_DataManager->GetStationList();
    if (!pDashboardDataStationList)
        return false;

    ListOfIDs_t stationIDs = pDashboardDataStationList->GetOrderedListOfDashboardStationIDs();
    for (int i = 0; i < stationIDs.count(); i++)
    {
        CDashboardStation* pDashboardStation = pDashboardDataStationList->GetDashboardStation(stationIDs.at(i));
        if (!pDashboardStation)
            continue;

        const CReagent* pReagent = pReagentList->GetReagent(pDashboardStation->GetDashboardReagentID());
        if (!pReagent)
            continue;

        if (pDashboardStation->GetDashboardReagentStatus() == "Empty")
            continue;

        if (CReagentGroup* reagentGroup = mp_DataManager->GetReagentGroupList()->GetReagentGroup(pReagent->GetGroupID()))
        {
            //Fixation:RG1, Clearing: RG5, Paraffin:RG6
            if ( (reagentGroupID == "RG1") || (reagentGroupID == "RG5") || (reagentGroupID == "RG6"))
            {
                if (reagentGroup->GetGroupID() == reagentGroupID)
                {
                    stationList.append(pDashboardStation->GetDashboardStationID());
                }
                continue;
            }

            if ( (reagentGroupID == "RG2") || (reagentGroupID == "RG3") || (reagentGroupID == "RG4") )//RG2:Water,RG3:Dehydrating diluted,RG4:Dehydrating absolute
            {

                if (reagentGroup->GetGroupID() == "RG3")//Dehydrating,diluted
                {
                    stationList.append(pDashboardStation->GetDashboardStationID());
                }
                continue;
            }

        }
    }// end of for

    return true;
}

void SchedulerMainThreadController::SendTissueProtectMsg()
{
    MsgClasses::CmdProgramAcknowledge* CmdTissueProtectDone = new MsgClasses::CmdProgramAcknowledge(5000,DataManager::TISSUE_PROTECT_PASSED);
    Q_ASSERT(CmdTissueProtectDone);
    Global::tRefType fRef = GetNewCommandRef();
    SendCommand(fRef, Global::CommandShPtr_t(CmdTissueProtectDone));
}

void SchedulerMainThreadController::SendCoverLidOpenMsg()
{
    if(DCL_ERR_DEV_WAXBATH_OVENCOVER_STATUS_OPEN == m_CurErrEventID)
    {
        MsgClasses::CmdProgramAcknowledge* CmdOvenCoverOpen = new MsgClasses::CmdProgramAcknowledge(5000,DataManager::OVEN_COVER_OPEN);
        Q_ASSERT(CmdOvenCoverOpen);
        Global::tRefType fRef = GetNewCommandRef();
        SendCommand(fRef, Global::CommandShPtr_t(CmdOvenCoverOpen));
    }
    else if(DCL_ERR_DEV_LIDLOCK_CLOSE_STATUS_ERROR == m_CurErrEventID)
    {
        MsgClasses::CmdProgramAcknowledge* CmdRetortCoverOpen = new MsgClasses::CmdProgramAcknowledge(5000,DataManager::CANCEL_RETORT_LID_OPEN_MSG_PROMPT);
        Q_ASSERT(CmdRetortCoverOpen);
        Global::tRefType fRef = GetNewCommandRef();
        SendCommand(fRef, Global::CommandShPtr_t(CmdRetortCoverOpen));
    }
    m_TickTimer.start();
}

void SchedulerMainThreadController::OnEnterPssMStepFin()
{
    m_PssmStepFinSeq = 0;
    m_TickTimer.start();
}

void SchedulerMainThreadController::OnEnterPssMProgFin()
{
    m_TickTimer.start();
}

void SchedulerMainThreadController::OnEnterDryStepState()
{
    m_CleaningDry.CurrentState = CDS_READY;
    m_CleaningDry.StepStartTime = 0;
    m_CleaningDry.warningReport = false;
    m_TickTimer.start();
}

/**
 * @brief Check which step has no safe reagent in a program.
 * @param ProgramID The the program Id, which to be checked.
 * @return the step index of the given program.
 */
int SchedulerMainThreadController::WhichStepHasNoSafeReagent(const QString& ProgramID)
{
    if (ProgramID.isNull() || ProgramID.isEmpty())
    {
        return -1;
    }

    if (!mp_DataManager)
    {
        Q_ASSERT(false);
        return -1;
    }

    CDataProgramList* pDataProgramList = mp_DataManager->GetProgramList();
    if (!pDataProgramList)
    {
        Q_ASSERT(false);
        return -1;
    }

    QList<QString> stationList;
    CProgram* pProgram = const_cast<CProgram*>(pDataProgramList->GetProgram(ProgramID));
    for (int i = 0; i < pProgram->GetNumberOfSteps(); i++)
    {
        const CProgramStep* pProgramStep = pProgram->GetProgramStep(i);
        if (!pProgramStep)
            continue;
        bool ret = GetSafeReagentStationList(GetReagentGroupID(pProgramStep->GetReagentID()), stationList);
        if (ret && stationList.isEmpty())
        {
            return i;
        }
    }

    return -1;
}

void SchedulerMainThreadController::ResetTheTimeParameter()
{
    m_EndTimeAndStepTime.WarningFlagForStepTime = false;
    m_EndTimeAndStepTime.WarningFlagForBufferTime = false;
    m_EndTimeAndStepTime.FirstParaffinIndex = 0;
    m_EndTimeAndStepTime.PreTestTime = 0;
    m_EndTimeAndStepTime.ParaffinStepsCostTime = 0;
    m_EndTimeAndStepTime.Scenario260CostTime = 0;
    m_EndTimeAndStepTime.GapTime = 0;
    m_EndTimeAndStepTime.StartTime = 0;
    m_EndTimeAndStepTime.EndTime = 0;
    m_EndTimeAndStepTime.UserSetEndTime = 0;
    m_EndTimeAndStepTime.BufferTime = 0;
    m_EndTimeAndStepTime.TotalParaffinProcessingTime = 0;
    m_EndTimeAndStepTime.LastParaffinProcessingTime = 0;
}

void SchedulerMainThreadController::CalculateTheGapTimeAndBufferTime(bool IsStartTime, bool IsEndTime)
{
    if(IsStartTime)
    {
        m_EndTimeAndStepTime.StartTime = QDateTime::currentMSecsSinceEpoch();
    }
    else if(IsEndTime && (m_CurProgramID.at(0) != 'C'))
    {
        m_EndTimeAndStepTime.EndTime = QDateTime::currentMSecsSinceEpoch();
        if(m_CurProgramStepIndex == m_EndTimeAndStepTime.FirstParaffinIndex)
        {
            //this time shall not count in the period between StartTime and EndTime of secnario 260
            m_EndTimeAndStepTime.EndTime -= m_EndTimeAndStepTime.Scenario260CostTime * 1000;
        }
        if(m_EndTimeAndStepTime.EndTime - m_EndTimeAndStepTime.StartTime > 4000 * 60)
        {
            if(m_CurProgramStepInfo.reagentGroup != "RG6")
            {
                m_EndTimeAndStepTime.WarningFlagForStepTime = true;
                RaiseEvent(EVENT_SCHEDULER_FOR_WARNING_FLAG, QStringList()<<QString("[%1]").arg(m_CurProgramStepIndex)<<QString("[%1]").arg(m_CurProgramStepIndex+1));
            }
        }
        qint64 LeftTime = m_EndTimeAndStepTime.EndTime - m_EndTimeAndStepTime.StartTime - m_EndTimeAndStepTime.GapTime;
        if(0 < LeftTime && LeftTime < (TIME_FOR_FIX_TIME * 1000))
        {
            m_EndTimeAndStepTime.GapTime = TIME_FOR_FIX_TIME * 1000 - (m_EndTimeAndStepTime.EndTime - m_EndTimeAndStepTime.StartTime) - m_EndTimeAndStepTime.GapTime;
            if(m_EndTimeAndStepTime.GapTime < 0)
            {
                m_EndTimeAndStepTime.GapTime = 0;
            }
        }
        else
        {
            m_EndTimeAndStepTime.GapTime = 0;
        }
        LogDebug(QString("The step:%1 gap time:%2 millisecond.").arg(m_CurProgramStepIndex + 1).arg( m_EndTimeAndStepTime.GapTime));

        if(IsLastStep(m_CurProgramStepIndex, m_CurProgramID))
        {
            m_EndTimeAndStepTime.BufferTime = m_EndTimeAndStepTime.UserSetEndTime - QDateTime::currentMSecsSinceEpoch() - m_EndTimeAndStepTime.LastParaffinProcessingTime * 1000;
            if(m_EndTimeAndStepTime.BufferTime < 0) // delay
            {
				if(m_EndTimeAndStepTime.BufferTime < -300000)// warning if the delay time is more than 5 minutes.
				{
                	m_EndTimeAndStepTime.WarningFlagForBufferTime = true;
                	LogDebug(QString("The program will be completed %1 millisecond later").arg(m_EndTimeAndStepTime.BufferTime));
				}
                m_EndTimeAndStepTime.BufferTime = 0;
            }
            else if(m_EndTimeAndStepTime.BufferTime > 0)//finish in advance
            {
				// after the 10% compensation with paraffin,  the advanced time is still more than 5 minutes; then rise warning message.
				if(m_EndTimeAndStepTime.BufferTime > m_EndTimeAndStepTime.TotalParaffinProcessingTime * 1000 * 0.1  + 300000)
				{
                	m_EndTimeAndStepTime.WarningFlagForBufferTime = true;
                	LogDebug(QString("The program will be completed %1 millisecond in advance.")
							.arg(m_EndTimeAndStepTime.BufferTime - m_EndTimeAndStepTime.TotalParaffinProcessingTime * 1000 * 0.1));
				}
				if(m_EndTimeAndStepTime.BufferTime > m_EndTimeAndStepTime.TotalParaffinProcessingTime * 1000 * 0.1)
				{
                	m_EndTimeAndStepTime.BufferTime = m_EndTimeAndStepTime.TotalParaffinProcessingTime * 1000 * 0.1;
				}
            }
        }
    }
}

quint32 SchedulerMainThreadController::GetMoveSteps(qint32 firstPos, qint32 endPos)
{
    quint32 MoveSteps = 0;
    MoveSteps = ((endPos > firstPos)?(endPos - firstPos):(firstPos - endPos));//lint !e656 !e641
    if(MoveSteps > 15)
    {
        if(MoveSteps < 32)
        {
            MoveSteps = 32 - MoveSteps;
        }
        else
        {
            MoveSteps = 0;
        }
    }
    return MoveSteps;
}

quint32 SchedulerMainThreadController::GetPreTestTime()
{
    //level heating + RV move initialize  + pressure calibration + sealing check + move first tube
    qint64 preTesttTime = 20 + 30 + 50 + 60 + 10;

    if(m_IsCleaningProgram)
    {
        //the cleaning pret-est should pressure 20 seconds
        preTesttTime += 20;
    }
    return preTesttTime;
}

quint32 SchedulerMainThreadController::GetLeftProgramStepsNeededTime(const QString& ProgramID, int BeginProgramStepID)
{
    quint32 leftTime = 0;

    if (ProgramID.isNull() || ProgramID.isEmpty())
    {
        return 0;
    }
    if (!mp_DataManager)
    {
        Q_ASSERT(false);
        return 0;//error handling
    }

    CDataProgramList* pDataProgramList = mp_DataManager->GetProgramList();
    if (!pDataProgramList)
    {
        Q_ASSERT(false);
        return 0;
    }

    CProgram* pProgram = const_cast<CProgram*>(pDataProgramList->GetProgram(ProgramID));
    if (!pProgram)
    {
        Q_ASSERT(false);
        return 0;
    }

    bool IsFirstParraffin = false;
    bool CleaningProgram = false;

    QString strLastReagentGroupId("");
    QString CurrentReagentID("");
    QStringList list;
    quint32 CleaningKnowonTime = 0;
    bool SkipCurrentStep = false;

    if(pProgram->IsCleaningProgram())
    {
        CleaningProgram = true;
        strLastReagentGroupId = m_ProgramStatusInfor.GetLastReagentGroup();
        list << "RG1"<<"RG2"<<"RG3"<<"RG4"<<"RG5";
        CleaningKnowonTime += TIME_FOR_HEATING_LEVEL_SENSOR;
        CleaningKnowonTime += TIME_FOR_FILLING;
        CleaningKnowonTime += TIME_FOR_MOVE_SEAL + TIME_FOR_MOVE_TUBE;
        CleaningKnowonTime += TIME_FOR_PRESSURE_CHECK;
        CleaningKnowonTime += TIME_FOR_DRAIN;
    }
    for (int i = BeginProgramStepID; i < pProgram->GetNumberOfSteps(); i++)
    {
        const CProgramStep* pProgramStep = pProgram->GetProgramStep(i);//use order index
        quint32 soakTime = pProgramStep->GetDurationInSeconds();
        if(CleaningProgram)
        {
            //Cleaning program
            if (CDataReagentList* pReagentList =  mp_DataManager->GetReagentList())
            {
                CurrentReagentID = pProgramStep->GetReagentID();
                const CReagent* pCurReagent = pReagentList->GetReagent(CurrentReagentID);
                if (list.contains(strLastReagentGroupId) && pCurReagent->GetGroupID() == "RG7")
                {
                    SkipCurrentStep = true;
                }
                else
                    SkipCurrentStep = false;
            }
            if(!SkipCurrentStep)
            {
                leftTime += soakTime;
                leftTime += CleaningKnowonTime;
            }
        }
        else
        {
            //normal program
            leftTime += soakTime;
            if(0 == i)
            {
                leftTime += TIME_FOR_HEATING_LEVEL_SENSOR;
                leftTime += TIME_FOR_FILLING;
                leftTime += TIME_FOR_MOVE_SEAL + TIME_FOR_MOVE_TUBE;
                leftTime += TIME_FOR_PRESSURE_CHECK;
                leftTime += TIME_FOR_FIX_TIME * (pProgram->GetNumberOfSteps() - 1);
            }
            if ("RG6" == GetReagentGroupID(pProgramStep->GetReagentID()))
            {
                if(!IsFirstParraffin)
                {
                    m_EndTimeAndStepTime.FirstParaffinIndex = i;
                    IsFirstParraffin = true;
                    int userInputMeltingPoint = 70;
                    if(mp_DataManager->GetUserSettings())
                    {
                        userInputMeltingPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
                    }
                    if(50 <= userInputMeltingPoint && userInputMeltingPoint <= 64)
                    {
                        m_EndTimeAndStepTime.Scenario260CostTime = 5 * 60;
                    }
                    else if(65 <= userInputMeltingPoint && userInputMeltingPoint <= 70)
                    {
                        m_EndTimeAndStepTime.Scenario260CostTime = 8 * 60;
                    }
                    leftTime += m_EndTimeAndStepTime.Scenario260CostTime;
                    m_EndTimeAndStepTime.ParaffinStepsCostTime += m_EndTimeAndStepTime.Scenario260CostTime;
                    m_EndTimeAndStepTime.TotalParaffinProcessingTime = 0;
                }
                if(i == (pProgram->GetNumberOfSteps() - 1))
                {
                    m_EndTimeAndStepTime.LastParaffinProcessingTime = soakTime;
                }
                m_EndTimeAndStepTime.TotalParaffinProcessingTime += soakTime;
                m_EndTimeAndStepTime.ParaffinStepsCostTime += TIME_FOR_FIX_TIME;
            }
        }
    }

    m_EndTimeAndStepTime.PreTestTime = GetPreTestTime();
    leftTime += m_EndTimeAndStepTime.PreTestTime;
    if(CleaningProgram) // if cleaning program, add time for dry step
    {
        leftTime += TIME_FOR_MOVE_NEXT_TUBE;
        leftTime += TIME_FOR_CLEANING_DRY_STEP + TIME_FOR_COOLING_DOWN;
    }
    return leftTime;
}

quint32 SchedulerMainThreadController::GetCurrentProgramStepNeededTime(const QString& ProgramID)
{
    quint32 leftTime = 0;
    if (ProgramID.isNull() || ProgramID.isEmpty())
    {
        return 0;
    }
    if (!mp_DataManager)
    {
        Q_ASSERT(false);
        return 0;//error handling
    }

    CDataProgramList* pDataProgramList = mp_DataManager->GetProgramList();
    if (!pDataProgramList)
    {
        Q_ASSERT(false);
        return 0;
    }

    CProgram* pProgram = const_cast<CProgram*>(pDataProgramList->GetProgram(ProgramID));

    int programStepIDIndex(-1);
    if (-1 == m_CurProgramStepIndex)
    {
        programStepIDIndex = 0;
    }
    else
    {
        programStepIDIndex = m_CurProgramStepIndex ;
    }

    const CProgramStep* pProgramStep = pProgram->GetProgramStep(programStepIDIndex);
    if (pProgramStep)
    {
        quint32 soakTime = pProgramStep->GetDurationInSeconds();
        leftTime += soakTime;

        leftTime += TIME_FOR_HEATING_LEVEL_SENSOR;
        leftTime += TIME_FOR_FILLING;
        leftTime += TIME_FOR_MOVE_SEAL + TIME_FOR_MOVE_TUBE + TIME_FOR_MOVE_NEXT_TUBE;
        leftTime += TIME_FOR_PRESSURE_CHECK;
        leftTime += TIME_FOR_DRAIN + (m_EndTimeAndStepTime.GapTime / 1000);

        if (programStepIDIndex == m_EndTimeAndStepTime.FirstParaffinIndex)
        {
            leftTime += m_EndTimeAndStepTime.Scenario260CostTime;
        }
        if (0 == programStepIDIndex)
        {
            leftTime += m_delayTime;
        }

        if(IsLastStep(programStepIDIndex, m_CurProgramID))
        {
            leftTime += m_EndTimeAndStepTime.BufferTime;
            if("RG6" == GetReagentGroupID(pProgramStep->GetReagentID()))
            {
                //last paraffin will drain delay 25 seconds
                leftTime += 25;
            }
        }
    }
    return leftTime;
}

void SchedulerMainThreadController::CheckCarbonFilterExpired()
{
    //Check for Service
    DataManager::CHimalayaUserSettings* pUserSetting = mp_DataManager->GetUserSettings();
    QString strLastOperateResetDate = pUserSetting->GetOperationLastResetDate();
    if (!strLastOperateResetDate.isEmpty())
    {
        QDateTime lastOperateResetDate = QDateTime::fromString(strLastOperateResetDate);
        int diffDays = lastOperateResetDate.daysTo(Global::AdjustedTime::Instance().GetCurrentDateTime());
        if (diffDays >= 365)
        {
           Global::EventObject::Instance().RaiseEvent(EVENT_SERVICE_OPERATIONTIME_OVERDUE);
        }
    }

    QString strLastActiveCarbonResetDate = pUserSetting->GetActiveCarbonLastResetDate();
    if (!strLastActiveCarbonResetDate.isEmpty())
    {
        QDateTime lastActiveCarbonResetDate = QDateTime::fromString(strLastActiveCarbonResetDate);
        int diffDaysActiveCarbon = lastActiveCarbonResetDate.daysTo(Global::AdjustedTime::Instance().GetCurrentDateTime());
        if ((diffDaysActiveCarbon >= 45) && (diffDaysActiveCarbon < 60))
        {
           Global::EventObject::Instance().RaiseEvent(EVENT_SERVICE_ACTIVECARBONTIME_OVERDUE_WARNING);
        }
        else if (diffDaysActiveCarbon >= 60)
        {
            Global::EventObject::Instance().RaiseEvent(EVENT_SERVICE_ACTIVECARBONTIME_OVERDUE_ALARM);
        }
    }
}

//client-->master
void SchedulerMainThreadController::OnProgramAction(Global::tRefType Ref,
                                                    const MsgClasses::CmdProgramAction &Cmd)
{
    m_Mutex.lock();
    m_SchedulerCmdQueue.enqueue(Global::CommandShPtr_t(new MsgClasses::CmdProgramAction(Cmd.GetTimeout(), Cmd.GetProgramID(), Cmd.ProgramActionType(),
                                                                                        Cmd.DelayTime(), Cmd.ProgramRunDuration(), Cmd.GetReagentExpiredFlag())));
    m_Mutex.unlock();
    this->SendAcknowledgeOK(Ref);

    QString ProgramName;
    if(mp_DataManager && mp_DataManager->GetProgramList() && mp_DataManager->GetProgramList()->GetProgram(Cmd.GetProgramID()))
    {
        ProgramName = mp_DataManager->GetProgramList()->GetProgram(Cmd.GetProgramID())->GetName();
    }

    SchedulerStateMachine_t stepState = m_SchedulerMachine->GetCurrentState();
    if ((PSSM_PAUSE != stepState) && (Cmd.ProgramActionType() == DataManager::PROGRAM_START))
    {
        if(Cmd.ProgramRunDuration() > 0) // start new program
        {
            QDateTime EndDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(Cmd.ProgramRunDuration());
            m_EndTimeAndStepTime.UserSetEndTime = QDateTime::currentDateTime().addSecs(Cmd.ProgramRunDuration()).toMSecsSinceEpoch();
            RaiseEvent(EVENT_SCHEDULER_REC_START_PROGRAM, QStringList()<<ProgramName
                       <<EndDateTime.toString()); //log
        }
        else
        {
            RaiseEvent(EVENT_SCHEDULER_REC_CONTINUE_PROGRAM,QStringList()<<ProgramName); //log
        }
        CheckCarbonFilterExpired();
        return;
    }
    else if(Cmd.ProgramActionType() == DataManager::PROGRAM_PAUSE)
    {
        RaiseEvent(EVENT_SCHEDULER_REC_PAUSE_PROGRAM,QStringList()<<ProgramName); //log
    }
    else if(Cmd.ProgramActionType() == DataManager::PROGRAM_ABORT)
    {
        RaiseEvent(EVENT_SCHEDULER_REC_ABORT_PROGRAM,QStringList()<<ProgramName); //log
    }
    else if(Cmd.ProgramActionType() == DataManager::PROGRAM_DRAIN)
    {
        RaiseEvent(EVENT_SCHEDULER_REC_DRAIN_PROGRAM,QStringList()<<ProgramName); //log
    }
}

//response or recovery
void SchedulerMainThreadController::OnActionCommandReceived(Global::tRefType Ref, const NetCommands::CmdSystemAction & Cmd)
{
    this->SendAcknowledgeOK(Ref);
    NetCommands::CmdSystemAction *p_CmdSystemAction = new NetCommands::CmdSystemAction();
    p_CmdSystemAction->SetActionString(Cmd.GetActionString());
    p_CmdSystemAction->SetEventKey(Cmd.GetEventKey());
    m_Mutex.lock();
    m_SchedulerCmdQueue.enqueue(Global::CommandShPtr_t(p_CmdSystemAction));
    m_Mutex.unlock();
    RaiseEvent(EVENT_SCHEDULER_REC_ERROR_ACTION,QStringList()<<Cmd.GetActionString()); //only log
}

void SchedulerMainThreadController::OnKeepCassetteCount(Global::tRefType Ref, const MsgClasses::CmdKeepCassetteCount & Cmd)
{
    m_ProcessCassetteCount += Cmd.CassetteCount();
    m_ProcessCassetteNewCount = Cmd.CassetteCount();
    this->SendAcknowledgeOK(Ref);
	SchedulerStateMachine_t previousState = m_SchedulerMachine->GetPreviousState();
    if (previousState == PSSM_RV_MOVE_TO_SEAL ||
        previousState == PSSM_PROCESSING ||
        previousState == PSSM_RV_MOVE_TO_TUBE)
    {
        this->UpdateStationReagentStatus(true);
    }
}

void SchedulerMainThreadController::OnProgramSelected(Global::tRefType Ref, const MsgClasses::CmdProgramSelected & Cmd)
{
    m_hasParaffin = false;
    this->SendAcknowledgeOK(Ref);
    m_delayTime = 0;
    QString curProgramID = Cmd.GetProgramID();
    m_CurProgramStepIndex = -1;
    (void)this->GetNextProgramStepInformation(curProgramID, m_CurProgramStepInfo, true);//only to get m_CurProgramStepIndex

    ResetTheTimeParameter();
    unsigned int timeProposed = 0;
    unsigned int paraffinMeltCostedtime = 0;
    unsigned int costedTimeBeforeParaffin = 0;
    int whichStep = 0;
    if (m_CurProgramStepIndex != -1)
    {
        m_FirstProgramStepIndex = m_CurProgramStepIndex;
        (void)this->PrepareProgramStationList(curProgramID, m_CurProgramStepIndex);
        timeProposed = GetLeftProgramStepsNeededTime(curProgramID);
        m_CurProgramStepIndex = -1;

        paraffinMeltCostedtime = this->GetOvenHeatingTime();
        int paraffinStepIndex = Cmd.ParaffinStepIndex();
        if (-1 != paraffinStepIndex)//has Paraffin
        {
            m_hasParaffin = true;
            int timeParaffinSteps = GetLeftProgramStepsNeededTime(curProgramID, paraffinStepIndex);
            costedTimeBeforeParaffin = timeProposed - timeParaffinSteps;
        }

        //cheack safe reagent
        whichStep = WhichStepHasNoSafeReagent(curProgramID);
    }
    else
    {
        m_StationList.clear();
    }

    m_ProcessCassetteCount = 0;
    m_ProcessCassetteNewCount = 0;
    m_CurrentBottlePosition.ReagentGrpId = "";
    m_CurrentBottlePosition.RvPos = RV_UNDEF;
    //send back the proposed program end time
    MsgClasses::CmdProgramSelectedReply* commandPtr(new MsgClasses::CmdProgramSelectedReply(5000, timeProposed,
                                                                                paraffinMeltCostedtime,
                                                                                costedTimeBeforeParaffin,
                                                                                whichStep,
                                                                                GetSecondsForMeltingParaffin(),
                                                                                m_StationList,
                                                                                m_FirstProgramStepIndex));
    Q_ASSERT(commandPtr);
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

}

void SchedulerMainThreadController::OnQuitAppShutdown(Global::tRefType Ref, const MsgClasses::CmdQuitAppShutdown & Cmd)
{
    this->SendAcknowledgeOK(Ref);
    m_Mutex.lock();
    m_SchedulerCmdQueue.enqueue(Global::CommandShPtr_t(new MsgClasses::CmdQuitAppShutdown(Cmd.GetTimeout(), Cmd.QuitAppShutdownActionType())));
    m_Mutex.unlock();
    m_RefCleanup = Ref;
    RaiseEvent(EVENT_SCHEDULER_REC_ACTION_SHUTDOWN);//log the action
}

void SchedulerMainThreadController::OnSavedServiceInfor(Global::tRefType Ref, const MsgClasses::CmdSavedServiceInfor & Cmd)
{
    this->SendAcknowledgeOK(Ref);
    m_SchedulerCommandProcessor->NotifySavedServiceInfor(Cmd.DeviceType());
}

void SchedulerMainThreadController::OnParaffinMeltPointChanged(Global::tRefType Ref, const MsgClasses::CmdParaffinMeltPointChanged & Cmd)
{
    this->SendAcknowledgeOK(Ref);
    RaiseEvent(EVENT_SCHEDULER_REC_CHANGE_MELTING_POINT,QStringList()<<QString("[%1]").arg(Cmd.GetLastMeltPoint())
               <<QString("[%1]").arg(Cmd.GetCurrentMeltPoint()));
    quint32 ParaffinMeltPoint = 64;
    if (mp_DataManager != NULL && mp_DataManager->GetUserSettings() != NULL)
    {
        ParaffinMeltPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
    }
    m_ProgramStatusInfor.ResetOvenHeatingTime(ParaffinMeltPoint,Cmd.IsReheating());;
    mp_HeatingStrategy->ResetTheOvenHeating();
}

void SchedulerMainThreadController::OnBottleCheck(Global::tRefType Ref, const MsgClasses::CmdBottleCheck & Cmd)
{
    this->SendAcknowledgeOK(Ref);
    m_Mutex.lock();
    m_SchedulerCmdQueue.enqueue(Global::CommandShPtr_t(new MsgClasses::CmdBottleCheck(Cmd.GetTimeout())));
    m_Mutex.unlock();
}

void SchedulerMainThreadController::OnTakeOutSpecimenFinished(Global::tRefType Ref, const MsgClasses::CmdTakeOutSpecimenFinished & Cmd)
{
    this->SendAcknowledgeOK(Ref);
    m_Mutex.lock();
    m_SchedulerCmdQueue.enqueue(Global::CommandShPtr_t(new MsgClasses::CmdTakeOutSpecimenFinished(Cmd.GetTimeout())));
    m_Mutex.unlock();
}

bool SchedulerMainThreadController::IsCleaningReagent(const QString& ReagentID)
{
    if (!mp_DataManager)
    {
        return false;
        Q_ASSERT(0);
    }

    if (CDataReagentList* pReagentList =  mp_DataManager->GetReagentList())
    {
        const CReagent* pReagent = pReagentList->GetReagent(ReagentID);
        if (CReagentGroup*  reagentGroup = mp_DataManager->GetReagentGroupList()->GetReagentGroup(pReagent->GetGroupID()))
        {
            return reagentGroup->IsCleaningReagentGroup();
        }
    }
    return false;
}

QString SchedulerMainThreadController::GetReagentName(const QString& ReagentID)
{
    if (!mp_DataManager)
        return "";

    if (CDataReagentList* pReagentList =  mp_DataManager->GetReagentList())
    {
        const CReagent* pReagent = pReagentList->GetReagent(ReagentID);
        if (pReagent)
            return pReagent->GetReagentName();
    }

    return "";
}

QString SchedulerMainThreadController::GetReagentGroupID(const QString& ReagentID)
{
    if (!mp_DataManager)
        return "";

    if (CDataReagentList* pReagentList =  mp_DataManager->GetReagentList())
    {
        const CReagent* pReagent = pReagentList->GetReagent(ReagentID);
        if (pReagent)
            return pReagent->GetGroupID();
    }

    return "";
}

qint32 SchedulerMainThreadController::GetScenarioBySchedulerState(SchedulerStateMachine_t State, QString ReagentGroup)
{
    quint32 scenario = 0;
    bool reagentRelated = false;
    switch(State)
    {
    case SM_UNDEF:
        break;
    case SM_INIT:
        scenario = 2;
        break;
    case SM_INIT_SELFTEST:
        scenario = 2;
        break;
    case SM_IDLE:
        if(m_IsWaitHeatingRV)
        {
            scenario = 3;
        }
        else
        {
            scenario = 4;
        }
        break;
    case SM_BUSY:
        break;
    case SM_ERROR:
        break;
    case PSSM_INIT:
        break;
    case PSSM_PRETEST:
        scenario = 200;
        break;
    case PSSM_PAUSE:
        scenario = 206;
        break;
    case PSSM_FILLING_RVROD_HEATING:
        if(ReagentGroup == "RG6")
        {
            scenario = 260;
            reagentRelated = false;
        }
        else
        {
            scenario = 211;
            reagentRelated = true;
        }
        break;
    case PSSM_FILLING_LEVELSENSOR_HEATING:
        scenario = 211;
        reagentRelated = true;
        break;
    case PSSM_FILLING:
        scenario = 212;
        reagentRelated = true;
        break;
    case PSSM_RV_MOVE_TO_SEAL:
        scenario = 213;
        reagentRelated = true;
        break;
    case PSSM_PROCESSING:
        scenario = 214;
        reagentRelated = true;
        break;
    case PSSM_RV_MOVE_TO_TUBE:
        scenario = 215;
        reagentRelated = true;
        break;
    case PSSM_DRAINING:
        scenario = 216;
        reagentRelated = true;
        break;
    case PSSM_RV_POS_CHANGE:
        scenario = 217;
        reagentRelated = true;
        break;
    case PSSM_STEP_PROGRAM_FINISH:
        break;
    case PSSM_PROGRAM_FINISH:
        break;
    case PSSM_CLEANING_DRY_STEP:
        scenario = 203;
        break;
    case PSSM_BOTTLE_CHECK:
        scenario = 7;
        break;
    default:
        break;
    }
    if(reagentRelated)
    {
        if(ReagentGroup == "RG1") //Fixation
        {
        }
        else if(ReagentGroup == "RG2") //Water
        {
            scenario += 10;
        }
        else if(ReagentGroup == "RG3") //Dehydrating diluted
        {
            scenario += 20;
        }
        else if(ReagentGroup == "RG4") //Dehydrating absolute
        {
            scenario += 30;
        }
        else if(ReagentGroup == "RG5") //Clearing
        {
            scenario += 40;
        }
        else if(ReagentGroup == "RG6") //Paraffin
        {
            scenario += 60;
        }
        else if(ReagentGroup == "RG7") //Cleaning solvent
        {
            scenario += 70;
        }
        else if(ReagentGroup == "RG8") //Cleaning Alcohol
        {
            scenario += 80;
        }
    }
    if(m_ProgramStatusInfor.GetScenario() != scenario && scenario > 2 && scenario != 206)
    {
        m_ProgramStatusInfor.SetScenario(scenario);
    }
    return scenario;
}


void SchedulerMainThreadController::OnDCLConfigurationFinished(ReturnCode_t RetCode)
{
    // Turn off local/remote alarm by default
    CmdRmtLocAlarm *cmd = new CmdRmtLocAlarm(500, this);
    cmd->SetRmtLocOpcode(-1);
    m_SchedulerCommandProcessor->pushCmd(cmd);

    bool working = false;
    if(RetCode == DCL_ERR_FCT_CALL_SUCCESS)
    {
        RaiseEvent(EVENT_SCHEDULER_SLAVE_BOARD_INITIALIZED_SUCCESSFULLY);//log
        ReturnCode_t retCode;
        //hardware not ready yet
        m_SchedulerCommandProcessor->pushCmd(new CmdPerTurnOnMainRelay(500, this));
        SchedulerCommandShPtr_t resPerTurnOnRelay;
        PopDeviceControlCmdQueue(resPerTurnOnRelay, "Scheduler::PerTurnOnMainRelay");
        (void)resPerTurnOnRelay->GetResult(retCode); 
        if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            RaiseEvent(EVENT_SCHEDULER_OPEN_MAIN_RELAY_FAILURE, QStringList()<<QString::number(retCode)); //log and GUI
        }
        else
        {
            RaiseEvent(EVENT_SCHEDULER_OPEN_MAIN_RELAY_SUCCESSFULLY);//log
            working = true;
        }
    }
    else
    {
        RaiseEvent(EVENT_SCHEDULER_SLAVE_BOARD_INITIALIZED_FAILURE,QStringList()<<QString::number(RetCode));
    }

    if(!working)
    {
        //send command to main controller to tell init failed
        MsgClasses::CmdProgramAcknowledge* commandPtr(new MsgClasses::CmdProgramAcknowledge(5000, DataManager::PROGRAM_SELFTEST_FAILED));
        Q_ASSERT(commandPtr);
        Global::tRefType Ref = GetNewCommandRef();
        SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        return;
    }

    // Get the slave module attribute list
    if (false == this->ConstructSlaveModuleAttrList("ASB3"))
    {
        return;
    }
    if (false == this->ConstructSlaveModuleAttrList("ASB5"))
    {
        return;
    }
    if (false == this->ConstructSlaveModuleAttrList("ASB15"))
    {
        return;
    }
	// Create HeatingStrategy
    mp_HeatingStrategy = QSharedPointer<HeatingStrategy>(new HeatingStrategy(this, m_SchedulerCommandProcessor, mp_DataManager));
    m_SchedulerMachine = new CSchedulerStateMachine(this);
    //for debug
    LogDebug(QString("Current state of Scheduler is: %1").arg(m_SchedulerMachine->GetCurrentState()));
    m_SchedulerMachine->Start();
    //m_TickTimer.start();

}

void SchedulerMainThreadController::HardwareMonitor(const QString& StepID)
{
    if(m_CheckTheHardwareStatus <= 2)  // Wait for about 500ms*3 = 1500ms to start hardware monitor
    {
        m_CheckTheHardwareStatus++;
        return;
    }
    QString ReagentGroup = m_CurProgramStepInfo.reagentGroup;
    quint32 Scenario = GetScenarioBySchedulerState(m_SchedulerMachine->GetCurrentState(),ReagentGroup);
    m_IsErrorStateForHM = false;

    HardwareMonitor_t strctHWMonitor = m_SchedulerCommandProcessor->HardwareMonitor();
    // log to Sensor data file
    m_CountTheLogSenserData++;
    if(m_CountTheLogSenserData >= 2)
    {
        SchedulerLogging::getInstance().logSensorData(strctHWMonitor.toLogString());
        m_CountTheLogSenserData = 0;
    }
    if(StepID.compare("INIT") != 0)
    {
        m_ProgramStatusInfor.UpdateOvenHeatingTime(QDateTime::currentMSecsSinceEpoch(),strctHWMonitor.OvenHeatingStatus);
    }

    // Monitor local and remote alarm
#if 0
    if (false == m_DisableAlarm)
    {
        if (1 == strctHWMonitor.RemoteAlarmStatus && m_CheckRemoteAlarmStatus)
        {
            SendOutErrMsg(DCL_ERR_DEV_MC_REMOTEALARM_UNCONNECTED, false);
            m_CheckRemoteAlarmStatus = false;
        }
        if (1 == strctHWMonitor.LocalAlarmStatus && m_CheckLocalAlarmStatus)
        {
            SendOutErrMsg(DCL_ERR_DEV_MC_LOCALALARM_UNCONNECTED, false);
            m_CheckLocalAlarmStatus = false;
        }
    }
#else
    if (1 == strctHWMonitor.RemoteAlarmStatus && m_RemoteAlarmPreviousStatus < 1) //alarm was unplugged
    {
        RaiseError(0, DCL_ERR_DEV_MC_REMOTEALARM_UNCONNECTED, m_CurrentScenario, true);
        m_RemoteAlarmPreviousStatus = 1;
    }
    else if(0 == strctHWMonitor.RemoteAlarmStatus && m_RemoteAlarmPreviousStatus == 1)// alarm was plugged
    {
        m_RemoteAlarmPreviousStatus = 0;
        RaiseError(0, DCL_ERR_DEV_MC_REMOTEALARM_UNCONNECTED, 0,true, false);
    }
    if (1 == strctHWMonitor.LocalAlarmStatus && m_LocalAlarmPreviousStatus < 1) //alarm was unplugged
    {
        RaiseError(0, DCL_ERR_DEV_MC_LOCALALARM_UNCONNECTED, m_CurrentScenario, true);
        m_LocalAlarmPreviousStatus = 1;
    }
    else if(0 == strctHWMonitor.LocalAlarmStatus && m_LocalAlarmPreviousStatus == 1) //alarm was unplugged
    {
        RaiseError(0, DCL_ERR_DEV_MC_LOCALALARM_UNCONNECTED, 0,true, false);
        m_LocalAlarmPreviousStatus = 0;
    }
#endif

    if(strctHWMonitor.RetortLockStatus != UNDEFINED_VALUE)
    {
        if(((m_RetortLockStatus == 0) ||(m_RetortLockStatus == UNDEFINED_VALUE))&&(strctHWMonitor.RetortLockStatus == 1))
        {
            MsgClasses::CmdLockStatus* commandPtr(new MsgClasses::CmdLockStatus(5000, DataManager::RETORT_LOCK, false));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        }
        if(((m_RetortLockStatus == 1) || (m_RetortLockStatus == UNDEFINED_VALUE))&&(strctHWMonitor.RetortLockStatus == 0))
        {
            MsgClasses::CmdLockStatus* commandPtr(new MsgClasses::CmdLockStatus(5000, DataManager::RETORT_LOCK, true));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        }
        m_RetortLockStatus = strctHWMonitor.RetortLockStatus;
    }

    if("ERROR" == StepID)
    {
        m_IsErrorStateForHM = true;
    }

    // Monitor the sensors' current
    if ("ERROR" != StepID && 0 != Scenario)
    {
        m_CurrentScenario = Scenario;
        this->CheckSlaveAllSensor(Scenario, strctHWMonitor);
    }

    if(strctHWMonitor.OvenLidStatus != UNDEFINED_VALUE)
    {
        m_OvenLidStatus = strctHWMonitor.OvenLidStatus;
    }
    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.PressureAL))
    {
        m_PressureAL = strctHWMonitor.PressureAL;
    }
    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.TempALLevelSensor))
	{
        m_TempALLevelSensor = strctHWMonitor.TempALLevelSensor;
	}
    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.TempALTube1))
	{
        m_TempALTube1 = strctHWMonitor.TempALTube1;
	}
    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.TempALTube2))
	{
        m_TempALTube2 = strctHWMonitor.TempALTube2;
	}
    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.TempRV1))
	{
        m_TempRV1 = strctHWMonitor.TempRV1;
	}
    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.TempRV2))
	{
        m_TempRV2 = strctHWMonitor.TempRV2;
	}
    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.TempRTBottom1))
	{
        m_TempRTBottom = strctHWMonitor.TempRTBottom1;
    }
    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.TempRTSide))
	{
        m_TempRTSide = strctHWMonitor.TempRTSide;
	}
    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.TempOvenBottom1))
	{
        m_TempOvenBottom = strctHWMonitor.TempOvenBottom1;
	}
    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.TempOvenTop))
	{
        m_TempOvenTop = strctHWMonitor.TempOvenTop;
    }
    m_PositionRV = strctHWMonitor.PositionRV;
}

void SchedulerMainThreadController::DataManager(DataManager::CDataManager *p_DataManager)
{
    mp_DataManager = p_DataManager;

    // Get the whole station list
    ListOfDashboardStation_t stationList = mp_DataManager->GetStationList()->GetDashboardStationList();
    ListOfDashboardStation_t::iterator iter = stationList.begin();
    while (iter != stationList.end())
    {
        QString stationId = iter.value()->GetDashboardStationID();
        QString reagentId = iter.value()->GetDashboardReagentID();
        QString reagentGroupId = "";
        if (!reagentId.isEmpty())
        {
            reagentGroupId = mp_DataManager->GetReagentList()->GetReagent(reagentId)->GetGroupID();
        }
        QPair<QString, QString> pair(stationId, reagentGroupId);
        m_DashboardStationList.push_back(pair);
        iter++;
    }

    //sort the vector
    qSort(m_DashboardStationList.begin(), m_DashboardStationList.end(), QPairComp);
}

bool SchedulerMainThreadController::PopDeviceControlCmdQueue(Scheduler::SchedulerCommandShPtr_t& PtrCmd)
{
    bool ret = false;
    if (m_TransitionPeriod)
    {
        return ret;
    }
    m_MutexDeviceControlCmdQueue.lock();
    if(!m_DeviceControlCmdQueue.isEmpty())
    {
        PtrCmd = m_DeviceControlCmdQueue.dequeue();
        LogDebug(QString("CMD has been poped up,name is: %1 and current status is: %2").arg(PtrCmd->GetName()).arg(m_SchedulerMachine->GetCurrentState()));
        ret = true;
    }
    m_MutexDeviceControlCmdQueue.unlock();
    return ret;
}

void SchedulerMainThreadController::PopDeviceControlCmdQueue(Scheduler::SchedulerCommandShPtr_t& PtrCmd, const QString& CmdName)
{
    while (true)
    {
        m_MutexDeviceControlCmdQueue.lock();
        for (QQueue<Scheduler::SchedulerCommandShPtr_t>::iterator iter=m_DeviceControlCmdQueue.begin(); iter!=m_DeviceControlCmdQueue.end(); ++iter)
        {
            if ((*iter)->GetName() == CmdName)
            {
                PtrCmd = *iter;
                (void)m_DeviceControlCmdQueue.removeOne(*iter);
                m_MutexDeviceControlCmdQueue.unlock();
                return;
            }
        }
        m_MutexDeviceControlCmdQueue.unlock();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}


void SchedulerMainThreadController::PushDeviceControlCmdQueue(Scheduler::SchedulerCommandShPtr_t CmdPtr)
{
    m_MutexDeviceControlCmdQueue.lock();
    m_DeviceControlCmdQueue.push_back(CmdPtr);
    LogDebug(QString("CMD %1 has been pushed").arg(CmdPtr->GetName()));
    m_MutexDeviceControlCmdQueue.unlock();
}

void SchedulerMainThreadController::RcBottleCheckI()
{
    if(m_CurrentBottlePosition.RvPos != RV_UNDEF && !m_CurrentBottlePosition.ReagentGrpId.isEmpty())
    {
        LogDebug(QString("BottleCheckI check for tube %1").arg(m_CurrentBottlePosition.RvPos));
        CmdIDBottleCheck* cmd  = new CmdIDBottleCheck(500, this);
        cmd->SetReagentGrpID(m_CurrentBottlePosition.ReagentGrpId);
        cmd->SetTubePos(m_CurrentBottlePosition.RvPos);
        m_SchedulerCommandProcessor->pushCmd(cmd);
    }
    else
    {
        LogDebug(QString("BottleCheckI RvPosition:%1, ReagentGrpId is empty").arg(m_CurrentBottlePosition.RvPos));
    }
    m_TickTimer.start();
}

void SchedulerMainThreadController::MoveRVToInit()
{
    LogDebug("Send cmd to DCL to let RV move to init position. ");
    m_SchedulerCommandProcessor->pushCmd(new CmdRVReqMoveToInitialPosition(500, this));
}


void SchedulerMainThreadController::ReleasePressure()
{
    RaiseEvent(EVENT_SCHEDULER_RELEASE_PREASURE);
    m_SchedulerCommandProcessor->pushCmd(new CmdALReleasePressure(500, this));
}

void SchedulerMainThreadController::OnEnterPssmProcessing()
{

    m_IsReleasePressureOfSoakFinish = false;
    // We only release pressure if neither P or V exists.
    if(!m_IsProcessing)
    {
        m_IsProcessing = true;
        RaiseEvent(EVENT_SCHEDULER_START_PROCESSING);
        if (!m_CurProgramStepInfo.isPressure && !m_CurProgramStepInfo.isVacuum)
        {
            RaiseEvent(EVENT_SCHEDULER_RELEASE_PREASURE);
            m_SchedulerCommandProcessor->pushCmd(new CmdALReleasePressure(500,  this));
        }
        if(0 == m_CurProgramStepIndex)
        {
            //the first reagent should need delay time
            if(m_IsFirstProcessingForDelay)
            {
                m_IsFirstProcessingForDelay = false;
                m_TimeStamps.ProposeSoakStartTime = QDateTime::currentDateTime().addSecs(m_delayTime).toMSecsSinceEpoch();
            }
        }
        else
        {
            m_TimeStamps.ProposeSoakStartTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
        }
        m_TimeStamps.CurStepSoakStartTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
        LogDebug(QString("The duration time:%1 seconds.").arg(m_CurProgramStepInfo.durationInSeconds));
        m_lastPVTime = 0;
        m_completionNotifierSent = false;

        if ((0 == m_CurProgramStepIndex) && (m_delayTime > 0))
        {
            m_IsInSoakDelay = true;
            m_TickTimer.start();
            return;
        }
        if(m_CurProgramStepInfo.isPressure ^ m_CurProgramStepInfo.isVacuum)
        {
            if(m_CurProgramStepInfo.isPressure)
            {
                m_ProcessingPV = 0;
                Pressure();
            }
            else
            {
                m_ProcessingPV = 1;
                Vaccum();
            }
        }
    }

    m_TickTimer.start();
}

bool SchedulerMainThreadController::IsRVRightPosition(RVPosition_type type)
{
    RVPosition_t targetPos = RV_UNDEF;
    bool ret = false;

    if(INITIALIZE_POS == type)
    {
        targetPos = RV_TUBE_1;
    }
    else if(TUBE_POS == type )
    {
        targetPos = GetRVTubePositionByStationID(m_CurProgramStepInfo.stationID);
    }
    else if(SEAL_POS == type)
    {
        targetPos = GetRVSealPositionByStationID(m_CurProgramStepInfo.stationID);
    }
    else if(NEXT_TUBE_POS == type)
    {
        targetPos = GetRVTubePositionByStationID(m_CurProgramStepInfo.nextStationID);
    }

    if (m_PositionRV == targetPos)
    {
        ret = true;
        if(SEAL_POS == type)
        {
            LogDebug(QString("RV hit seal position: %1").arg(targetPos));
        }
        else
        {
            LogDebug(QString("RV hit tube position: %1").arg(targetPos));
        }
        m_ProgramStatusInfor.SetLastRVPosition(targetPos);
    }
    return ret;
}

void SchedulerMainThreadController::DoCleaningDryStep(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd)
{
    Q_UNUSED(ctrlCmd)
    ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;
    CmdRVReqMoveToRVPosition* CmdMvRV = NULL;
    MsgClasses::CmdCurrentProgramStepInfor* commandPtr = NULL;
    Global::tRefType Ref;

    switch (m_CleaningDry.CurrentState)
    {
    case CDS_READY:
        RaiseEvent(EVENT_SCHEDULER_START_DRY_PROCESSING);
        commandPtr = new MsgClasses::CmdCurrentProgramStepInfor(5000, Global::UITranslator::TranslatorInstance().Translate(STR_SCHEDULER_DRY_PROCESSING),
                                                                m_CurProgramStepIndex, TIME_FOR_CLEANING_DRY_STEP + TIME_FOR_COOLING_DOWN);
        Q_ASSERT(commandPtr);
        Ref = GetNewCommandRef();
        SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        m_CleaningDry.CurrentState = CDS_MOVE_TO_SEALING_13;
        break;
    case CDS_MOVE_TO_SEALING_13:
        CmdMvRV = new CmdRVReqMoveToRVPosition(500, this);
        CmdMvRV->SetRVPosition(DeviceControl::RV_SEAL_13);
        m_SchedulerCommandProcessor->pushCmd(CmdMvRV);
        m_CleaningDry.CurrentState = CDS_WAIT_HIT_POSITION;
        break;
    case CDS_WAIT_HIT_POSITION:
        if(cmd != NULL && ("Scheduler::RVReqMoveToRVPosition" == cmd->GetName()))
        {
            (void)cmd->GetResult(retCode);
            if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                m_ProgramStatusInfor.SetLastRVPosition(DeviceControl::RV_SEAL_13);
                m_CleaningDry.CurrentState = CDS_WAIT_HIT_TEMPERATURE;
            }
            else
            {
                m_CleaningDry.CurrentState = CDS_READY;
                m_CleaningDry.StepStartTime = 0;
                SendOutErrMsg(retCode);
            }
        }
        break;
    case CDS_WAIT_HIT_TEMPERATURE:
        m_CleaningDry.CurrentState = CDS_VACUUM;
        break;
    case CDS_VACUUM:
        Vaccum();
        m_CleaningDry.CurrentState = CDS_WAIT_HIT_PPRESSURE;
        break;
    case CDS_WAIT_HIT_PPRESSURE:
        if(cmd != NULL && ("Scheduler::ALVaccum" == cmd->GetName()))
        {
            (void)cmd->GetResult(retCode);
            if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
            {
                m_CleaningDry.CurrentState = CDS_READY;
                m_CleaningDry.StepStartTime = 0;
                SendOutErrMsg(retCode);
            }
            else
            {
                m_CleaningDry.CurrentState = CDS_WAITING_DRY;
                m_CleaningDry.StepStartTime = QDateTime::currentMSecsSinceEpoch();
            }
        }
        break;
    case CDS_WAITING_DRY:
        if(QDateTime::currentMSecsSinceEpoch() - m_CleaningDry.StepStartTime >= 600000) // drying 10 minutes
        {
            m_CleaningDry.CurrentState = CDS_STOP_VACUUM;
        }

        //Check if pressure reaches negative 25kpa, if no, report out warning message
    #if 0
        if (QDateTime::currentMSecsSinceEpoch() - m_CleaningDry.StepStartTime >= 120*1000 && m_CleaningDry.warningReport == false)
        {
           m_CleaningDry. warningReport = true;
            if (qAbs(m_PressureAL) < 25.0)
            {
                SendOutErrMsg(DCL_ERR_DEV_LA_SEALING_FAILED_VACUUM, false);
            }
        }
    #endif
        break;
    case CDS_STOP_VACUUM:
        ReleasePressure();
        m_CleaningDry.CurrentState = CDS_STOP_HEATING;
        break;
    case CDS_STOP_HEATING:
        mp_HeatingStrategy->StopTemperatureControl("RTSide");
        mp_HeatingStrategy->StopTemperatureControl("RTBottom");
        RaiseEvent(Event_SCHEDULER_COOLING_DOWN);
        m_CleaningDry.StepStartTime = QDateTime::currentMSecsSinceEpoch();
        m_CleaningDry.CurrentState = CDS_WAIT_COOLDWON;
        break;
    case CDS_WAIT_COOLDWON:
        if(m_TempRTBottom < 40 && m_TempRTSide < 40)
        {
            m_CleaningDry.CurrentState = CDS_MOVE_TO_TUBE_13;
            break;
        }

        if(QDateTime::currentMSecsSinceEpoch() - m_CleaningDry.StepStartTime >= TIME_FOR_COOLING_DOWN * 1000)
        {
            MsgClasses::CmdProgramAcknowledge* CmdCoolingDownFaild = new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_COOLING_DWON);
            Q_ASSERT(CmdCoolingDownFaild);
            Global::tRefType fRef = GetNewCommandRef();
            SendCommand(fRef, Global::CommandShPtr_t(CmdCoolingDownFaild));
            m_CleaningDry.CurrentState = CDS_MOVE_TO_TUBE_13;
        }
        break;
    case CDS_MOVE_TO_TUBE_13:
        CmdMvRV = new CmdRVReqMoveToRVPosition(500, this);
        CmdMvRV->SetRVPosition(DeviceControl::RV_TUBE_13);
        m_SchedulerCommandProcessor->pushCmd(CmdMvRV);
        m_CleaningDry.CurrentState = CDS_WAIT_HIT_TUBE_13;
        break;
    case CDS_WAIT_HIT_TUBE_13:
        if(cmd != NULL && ("Scheduler::RVReqMoveToRVPosition" == cmd->GetName()))
        {
            (void)cmd->GetResult(retCode);
            if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                m_ProgramStatusInfor.SetLastRVPosition(DeviceControl::RV_TUBE_13);
                m_CleaningDry.CurrentState = CDS_SUCCESS;
            }
            else
            {
                m_CleaningDry.CurrentState = CDS_READY;
                m_CleaningDry.StepStartTime = 0;
                SendOutErrMsg(retCode);
            }
        }
        break;
    case CDS_SUCCESS:
        RaiseEvent(EVENT_SCHEDULER_FINISHED_DRY_PROCESSING);
        m_SchedulerMachine->NotifyProgramFinished();
        m_CleaningDry.CurrentState = CDS_READY;
        m_CleaningDry.StepStartTime = 0;
        break;
    case CDS_ERROR:
        break;
    }
}


bool SchedulerMainThreadController::MoveRV(RVPosition_type type)
{
    /*lint -e593 */
    CmdRVReqMoveToRVPosition* cmd = new CmdRVReqMoveToRVPosition(500, this);
    RVPosition_t targetPos = RV_UNDEF;

    if(TUBE_POS == type ) //tube position
    {
        //get target position here
        targetPos = GetRVTubePositionByStationID(m_CurProgramStepInfo.stationID);
        RaiseEvent(EVENT_SCHEDULER_MOVE_RV_TUBE_POSITION,QStringList()<<m_CurProgramStepInfo.stationID);
    }
    else if(SEAL_POS == type) //seal positon
    {
        //get target position here
        targetPos = GetRVSealPositionByStationID(m_CurProgramStepInfo.stationID);
        RaiseEvent(EVENT_SCHEDULER_MOVE_RV_SEALING_POSITION,QStringList()<<m_CurProgramStepInfo.stationID);
    }
    else if(NEXT_TUBE_POS == type)
    {
        //get target position here
        targetPos = GetRVTubePositionByStationID(m_CurProgramStepInfo.nextStationID);
        RaiseEvent(EVENT_SCHEDULER_MOVE_RV_TUBE_POSITION,QStringList()<<m_CurProgramStepInfo.nextStationID);
    }

    if(RV_UNDEF != targetPos)
    {
        cmd->SetRVPosition(targetPos);
        m_SchedulerCommandProcessor->pushCmd(cmd);
        return true;
    }
    else
    {
       LogDebug(QString("Error in MV RV to station: %1").arg(m_CurProgramStepInfo.stationID));
       return false;
    }
}

void SchedulerMainThreadController::Fill()
{
    RaiseEvent(EVENT_SCHEDULER_START_FILLING);
    CmdALFilling* cmd  = new CmdALFilling(500, this);

    // only cleaning program need to suck another 2 seconds after level sensor triggering.
    if(!m_CurProgramID.isEmpty() && m_CurProgramID.at(0) == 'C')
    {
        cmd->SetDelayTime(2000);
    }
    else
    {
        cmd->SetDelayTime(0);
    }
    QString ReagentGroup = m_CurProgramStepInfo.reagentGroup;
    m_ProgramStatusInfor.SetLastReagentGroup(ReagentGroup);
    quint32 Scenario = GetScenarioBySchedulerState(m_SchedulerMachine->GetCurrentState(),ReagentGroup);
    // For paraffin, Insufficient Check is NOT needed.
    if (272 == Scenario)
    {
        cmd->SetEnableInsufficientCheck(false);
    }
    else
    {
        cmd->SetEnableInsufficientCheck(true);
    }
    m_SchedulerCommandProcessor->pushCmd(cmd);

    // acknowledge to gui
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(15000,m_CurProgramStepInfo.stationID , true, true, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

    // Update station reagent status
    this->UpdateStationReagentStatus(false);
    m_TickTimer.start();
}
bool SchedulerMainThreadController::ShutdownFailedHeaters()
{
    HeaterType_t heaterType = this->GetFailerHeaterType();
    switch (heaterType)
    {
    case LEVELSENSOR:
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StopTemperatureControl("LevelSensor"))
        {
            return true;
        }
        break;
    case LATUBE1:
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StopTemperatureControl("LA_Tube1"))
        {
            return true;
        }
        break;
    case LATUBE2:
        if (DCL_ERR_FCT_CALL_SUCCESS ==mp_HeatingStrategy->StopTemperatureControl("LA_Tube2"))
        {
            return true;
        }
        break;
    case RETORT:
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StopTemperatureControl("RTBottom") &&
                DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StopTemperatureControl("RTSide"))
        {
            return true;
        }
        break;
    case OVEN:
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StopTemperatureControl("OvenTop") &&
                DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StopTemperatureControl("OvenBottom"))
        {
            return true;
        }
        break;
    case RV:
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StopTemperatureControl("RV"))
        {
            return true;
        }
        break;
    case ASB5:
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StopTemperatureControl("RTBottom") &&
                DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StopTemperatureControl("RTSide") &&
                DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StopTemperatureControl("OvenTop") &&
                DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StopTemperatureControl("OvenBottom"))
        {
            return true;
        }
        break;
    case FAN:
    {
        m_SchedulerCommandProcessor->pushCmd(new CmdALTurnOffFan(500, this));
        SchedulerCommandShPtr_t pResHeatingCmd;
        PopDeviceControlCmdQueue(pResHeatingCmd, "Scheduler::ALTurnOffFan");
        ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;
        (void)pResHeatingCmd->GetResult(retCode);
        if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
        {
            return true;
        }
    }
        break;
    default:
        return true;
    }

    return false;
}

bool SchedulerMainThreadController::RestartFailedHeaters()
{
    // Reopen Main Relay before restarting heaters
    m_SchedulerCommandProcessor->pushCmd(new CmdPerTurnOnMainRelay(500, this), false);

    HeaterType_t heaterType = this->GetFailerHeaterType();
    switch (heaterType)
    {
    case LEVELSENSOR:
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StartTemperatureControl("LevelSensor"))
        {
            return true;
        }
        break;
    case LATUBE1:
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StartTemperatureControl("LA_Tube1"))
        {
            return true;
        }
        break;
    case LATUBE2:
        if (DCL_ERR_FCT_CALL_SUCCESS ==mp_HeatingStrategy->StartTemperatureControl("LA_Tube2"))
        {
            return true;
        }
        break;
    case RETORT:
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StartTemperatureControl("RTBottom") &&
                DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StartTemperatureControl("RTSide"))
        {
            return true;
        }
        break;
    case OVEN:
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StartTemperatureControl("OvenTop") &&
                DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StartTemperatureControl("OvenBottom"))
        {
            return true;
        }
        break;
    case RV:
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StartTemperatureControl("RV"))
        {
            return true;
        }
        break;
    case ASB5:
        if (DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StartTemperatureControl("RTBottom") &&
                DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StartTemperatureControl("RTSide")&&
                DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StartTemperatureControl("OvenTop") &&
                DCL_ERR_FCT_CALL_SUCCESS == mp_HeatingStrategy->StartTemperatureControl("OvenBottom"))
        {
            return true;
        }
        break;
    case FAN:
    {
        m_SchedulerCommandProcessor->pushCmd(new CmdALTurnOnFan(500, this));
        SchedulerCommandShPtr_t pResHeatingCmd;
        PopDeviceControlCmdQueue(pResHeatingCmd, "Scheduler::ALTurnOnFan");
        ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;
        (void)pResHeatingCmd->GetResult(retCode);
        if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
        {
            return true;
        }
    }
        break;
    default:
        return true;
    }

    return false;
}

bool SchedulerMainThreadController::CheckSensorTempOverange()
{
    HeaterType_t heaterType = GetFailerHeaterType();
    qreal temp1 = 0;
    qreal temp2 = 0;
    qreal temp3 = 0;

    switch (heaterType)
    {
    case LEVELSENSOR:
        temp1 = m_SchedulerCommandProcessor->HardwareMonitor().TempALLevelSensor;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("LevelSensor", temp1) )
            return false;
        break;
    case LATUBE1:
        temp1 = m_SchedulerCommandProcessor->HardwareMonitor().TempALTube1;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("LA_Tube1", temp1) )
            return false;
        break;
    case LATUBE2:
        temp1 = m_SchedulerCommandProcessor->HardwareMonitor().TempALTube2;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("LA_Tube2", temp1) )
            return false;
        break;
    case RV:
        temp1 = m_SchedulerCommandProcessor->HardwareMonitor().TempRV1;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("RVRod", temp1) )
            return false;
        temp2 = m_SchedulerCommandProcessor->HardwareMonitor().TempRV2;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("RVOutle", temp2) )
            return false;
        break;
    case RETORT:
        temp1 = m_SchedulerCommandProcessor->HardwareMonitor().TempRTSide;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("RTSide", temp1) )
            return false;
        temp2 = m_SchedulerCommandProcessor->HardwareMonitor().TempRTBottom1;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("RTBottom", temp2) )
            return false;
        if (false == mp_HeatingStrategy->CheckRTBottomsDifference(m_SchedulerCommandProcessor->HardwareMonitor().TempRTBottom1,
                                                         m_SchedulerCommandProcessor->HardwareMonitor().TempRTBottom2))
        {
            return false;
        }
        temp3 = m_SchedulerCommandProcessor->HardwareMonitor().TempRTBottom2;
        if(false == mp_HeatingStrategy->CheckSensorTempOverRange("RTBottom", temp3))
        {
            return false;
        }
        break;
    case RETORT_NOSIGNAL:
        temp1 = m_SchedulerCommandProcessor->HardwareMonitor().TempRTSide;
        temp2 = m_SchedulerCommandProcessor->HardwareMonitor().TempRTBottom1;
        temp3 = m_SchedulerCommandProcessor->HardwareMonitor().TempRTBottom2;
        if (qFuzzyCompare(temp1,299) || qFuzzyCompare(temp2,299) || qFuzzyCompare(temp3,299))
        {
            return false;
        }
        break;
    case OVEN:
        temp1 = m_SchedulerCommandProcessor->HardwareMonitor().TempOvenTop;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("OvenTop", temp1) )
            return false;
        temp2 = m_SchedulerCommandProcessor->HardwareMonitor().TempOvenBottom1;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("OvenBottom", temp2) )
            return false;
        temp3 = m_SchedulerCommandProcessor->HardwareMonitor().TempOvenBottom2;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("OvenBottom", temp3))
            return false;
        break;
    case ASB5:
        temp1 = m_SchedulerCommandProcessor->HardwareMonitor().TempRTSide;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("RTSide", temp1) )
            return false;
        temp2 = m_SchedulerCommandProcessor->HardwareMonitor().TempRTBottom1;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("RTBottom", temp2) )
            return false;
        if (false == mp_HeatingStrategy->CheckRTBottomsDifference(m_SchedulerCommandProcessor->HardwareMonitor().TempRTBottom1,
                                                         m_SchedulerCommandProcessor->HardwareMonitor().TempRTBottom2))
        {
            return false;
        }
        temp3 = m_SchedulerCommandProcessor->HardwareMonitor().TempRTBottom2;
        if(false == mp_HeatingStrategy->CheckSensorTempOverRange("RTBottom", temp3))
        {
            return false;
        }
        temp1 = m_SchedulerCommandProcessor->HardwareMonitor().TempOvenTop;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("OvenTop", temp1) )
            return false;
        temp2 = m_SchedulerCommandProcessor->HardwareMonitor().TempOvenBottom1;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("OvenBottom", temp2) )
            return false;
        temp3 = m_SchedulerCommandProcessor->HardwareMonitor().TempOvenBottom2;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("OvenBottom", temp3))
            return false;
        break;
    default:
        break;
    }
    return true;
}

void SchedulerMainThreadController::FillRsTissueProtect(const QString& StationID, bool EnableInsufficientCheck)
{
    LogDebug("Send cmd to DCL to Fill in Rs_Tissue_Protect");
    CmdALFilling* cmd  = new CmdALFilling(500, this);
    cmd->SetDelayTime(0);

    cmd->SetEnableInsufficientCheck(EnableInsufficientCheck);
    m_SchedulerCommandProcessor->pushCmd(cmd);

    if(!m_IsCleaningProgram)
    {
        m_ProgramStatusInfor.SetLastReagentGroup(m_CurProgramStepInfo.reagentGroup);
    }
    // acknowledge to gui
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(15000, StationID, true, true, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
}

void SchedulerMainThreadController::StopFillRsTissueProtect(const QString& StationID)
{
    // acknowledge to gui
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(15000,StationID, false, true, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
}

bool SchedulerMainThreadController::CheckSlaveTempModulesCurrentRange(quint8 interval)
{
    ReportError_t reportError1;
    memset(&reportError1, 0, sizeof(reportError1));
    ReportError_t reportError2;
    memset(&reportError2, 0, sizeof(reportError2));
    ReportError_t reportError3;
    memset(&reportError3, 0, sizeof(reportError3));
    ReportError_t reportError4;
    memset(&reportError4, 0, sizeof(reportError4));

    qint64 now = QDateTime::currentMSecsSinceEpoch();

    HeaterType_t heaterType = this->GetFailerHeaterType();
    switch (heaterType)
    {
    case LEVELSENSOR:
        reportError1 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE,"LA", AL_LEVELSENSOR);
        if (reportError1.instanceID != 0 && (now-reportError1.errorTime) <= interval*1000)
        {
            LogDebug("Level Sensor current out of range");
            return false;
        }
        break;
    case LATUBE1:
         reportError1 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "LA", AL_TUBE1);
         if (reportError1.instanceID != 0 && (now-reportError1.errorTime) <= interval*1000)
         {
             LogDebug("LA Tube1 current out of range");
             return false;
         }
         break;
    case LATUBE2:
        reportError1 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "LA", AL_TUBE2);
        if (reportError1.instanceID != 0 && (now-reportError1.errorTime) <= interval*1000)
        {
            LogDebug("LA Tube2 current out of range");
            return false;
        }
        break;
    case RV:
        reportError1 =  m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "RV");
        if (reportError1.instanceID != 0 && (now-reportError1.errorTime) <= interval*1000)
        {
            LogDebug("RV current out of range");
            return false;
        }
        break;
        // Retort and Oven are in the same card, so any heater goes wrong, we will stop all the others.
    case RETORT:
    case OVEN:
    case ASB5:
        reportError1 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Retort", RT_BOTTOM);
        reportError2 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Retort", RT_SIDE);
        reportError3 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Oven", OVEN_TOP);
        reportError4 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Oven", OVEN_BOTTOM);
        if (reportError1.instanceID != 0 && (now-reportError1.errorTime) <= interval*1000)
        {
            LogDebug("Retort Bottom out of range");
            return false;
        }
        if (reportError2.instanceID != 0 && (now-reportError2.errorTime) <= interval*1000)
        {
            LogDebug("Retort Top out of range");
            return false;
        }
        if (reportError3.instanceID != 0 && (now-reportError3.errorTime) <= interval*1000)
        {
            LogDebug("Oven Top out of range");
            return false;
        }
        if (reportError4.instanceID != 0 && (now-reportError4.errorTime) <= interval*1000)
        {
            LogDebug("Oven Bottom out of range");
            return false;
        }
        break;
    case FAN:
        reportError1 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(PRESS_FAN_OUT_OF_RANGE, "LA", AL_FAN);
        if (reportError1.instanceID != 0 && (now-reportError1.errorTime) <= interval*1000)
        {
            LogDebug("Fan current out of range");
            return false;
        }
        break;
    default:
        break;
    }
    return true;
}

void SchedulerMainThreadController::OnStopFill()
{
    // acknowledge to gui
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(15000,m_CurProgramStepInfo.stationID , false, true, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
}

void SchedulerMainThreadController::RCDrain()
{
    LogDebug("Send cmd to DCL to RcDrain");
    CmdALDraining* cmd  = new CmdALDraining(500, this);
    cmd->SetDelayTime(5000);
    cmd->SetDrainPressure(40.0);
    cmd->SetIgnorePressure(true);
    m_SchedulerCommandProcessor->pushCmd(cmd);

    LogDebug("Notice GUI Draining started");
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(15000,m_CurProgramStepInfo.stationID , true, false, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
    m_TickTimer.start();
}

void SchedulerMainThreadController::Drain()
{
    RaiseEvent(EVENT_SCHEDULER_DRAINING);
    CmdALDraining* cmd  = new CmdALDraining(500, this);

    quint32 gapTime = m_EndTimeAndStepTime.GapTime;
    if( "RG6" == m_CurProgramStepInfo.reagentGroup && IsLastStep(m_CurProgramStepIndex, m_CurProgramID) )
    {
        cmd->SetDelayTime(25000+gapTime);
    }
    else
    {
        cmd->SetDelayTime(5000+gapTime);
    }
    m_SchedulerCommandProcessor->pushCmd(cmd);

    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(15000,m_CurProgramStepInfo.stationID , true, false, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
    m_TickTimer.start();
}

void SchedulerMainThreadController::RcDrainAtOnce()
{
    LogDebug("Send cmd to DCL to Drain in RC_Drain_AtOnce");
    CmdALDraining* cmd  = new CmdALDraining(500, this);
    //todo: get delay time here
    cmd->SetDelayTime(5000);
    cmd->SetIgnorePressure(true);
    m_SchedulerCommandProcessor->pushCmd(cmd);

    // acknowledge to gui
    LogDebug("Notice GUI Draining started");
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(15000,m_CurProgramStepInfo.stationID , true, false, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
    m_TickTimer.start();
}

void SchedulerMainThreadController::OnBeginDrain()
{
    // acknowledge to gui
    LogDebug("Notice GUI Draining started");
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(15000,m_CurProgramStepInfo.stationID , true, false, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
}

void SchedulerMainThreadController::OnStopDrain()
{
    // acknowledge to gui
    LogDebug("Notice GUI Draining stopped");
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(15000,m_CurProgramStepInfo.stationID , false, false, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
}

void SchedulerMainThreadController::Pressure()
{
    RaiseEvent(EVENT_SCHEDULER_SET_PRESSURE,QStringList()<<QString("[%1]").arg(AL_TARGET_PRESSURE_POSITIVE));
    m_SchedulerCommandProcessor->pushCmd(new CmdALPressure(500, this));
    m_TickTimer.start();
}

void SchedulerMainThreadController::HighPressure()
{

    RaiseEvent(EVENT_SCHEDULER_SET_PRESSURE,QStringList()<<QString("[%1]").arg(40));
    CmdALPressure* cmd = new CmdALPressure(500, this);
    cmd->SetTargetPressure(40.0);
    m_SchedulerCommandProcessor->pushCmd(cmd);
    m_TickTimer.start();
}

void SchedulerMainThreadController::Vaccum()
{

    RaiseEvent(EVENT_SCHEDULER_SET_PRESSURE,QStringList()<<QString("[%1]").arg(AL_TARGET_PRESSURE_NEGATIVE));
    m_SchedulerCommandProcessor->pushCmd(new CmdALVaccum(500, this));
    m_TickTimer.start();
}

void SchedulerMainThreadController::AllStop()
{
    LogDebug("Send cmd to DCL to ALLStop");
    (void)m_SchedulerCommandProcessor->ALBreakAllOperation();
    m_SchedulerCommandProcessor->pushCmd(new CmdALAllStop(500, this));
}

void SchedulerMainThreadController::Pause()
{
    RaiseEvent(EVENT_SCHEDULER_OVEN_PAUSE);

    //First of all, release pressure
    m_SchedulerCommandProcessor->pushCmd(new CmdALReleasePressure(500,this), false);

    //update the remaining time for the current step
    if (PSSM_PROCESSING == m_CurrentStepState)
    {
        m_IsProcessing = false;
        m_CurProgramStepInfo.durationInSeconds = m_CurProgramStepInfo.durationInSeconds - ((QDateTime::currentDateTime().toMSecsSinceEpoch() - m_TimeStamps.CurStepSoakStartTime) / 1000);
    }
    LogDebug("SchedulerMainThreadController Paused");
    m_TimeStamps.PauseStartTime = QDateTime::currentMSecsSinceEpoch();
    m_Is5MinPause = false;
    m_Is10MinPause = false;
    m_Is15MinPause = false;
    //send command to main controller to tell program is actually pasued
    MsgClasses::CmdProgramAcknowledge* commandPtrPauseFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_PAUSE_FINISHED));
    Q_ASSERT(commandPtrPauseFinish);
    Global::tRefType fRef = GetNewCommandRef();
    SendCommand(fRef, Global::CommandShPtr_t(commandPtrPauseFinish));
    m_TickTimer.start();
}


bool SchedulerMainThreadController::CheckLevelSensorTemperature(qreal targetTemperature)
{
    // get target temperature here
    return (m_TempALLevelSensor > targetTemperature);
}
RVPosition_t SchedulerMainThreadController::GetRVTubePositionByStationID(const QString& stationID)
{
    RVPosition_t ret = RV_UNDEF;
    bool ok;
    if(!stationID.isEmpty())
    {
        int position = stationID.right(stationID.length() - 1).toInt(&ok, 10);
        if(ok)
        {
            if(stationID.left(1) == "S")
            {
                ret = (RVPosition_t)(position * 2 - 1);
            }
            else if(stationID.left(1) == "P")
            {
                ret = (RVPosition_t)((position + 13) * 2 - 1);
            }
        }
    }
    return ret;
}

RVPosition_t SchedulerMainThreadController::GetRVSealPositionByStationID(const QString& stationID)
{
    RVPosition_t ret = RV_UNDEF;
    bool ok;
    if(!stationID.isEmpty())
    {
        int position = stationID.right(stationID.length() - 1).toInt(&ok, 10);
        if(ok)
        {
            if(stationID.left(1) == "S")
            {
                ret = (RVPosition_t)(position * 2);
            }
            else if(stationID.left(1) == "P")
            {
                ret = (RVPosition_t)((position + 13) * 2);
            }
        }
    }
    return ret;
}

quint32 SchedulerMainThreadController::GetSecondsForMeltingParaffin()
{
    quint32 TotalSecondForMeltingParaffin = 12 * 60 * 60;

    if (mp_DataManager != NULL && mp_DataManager->GetUserSettings())
    {
        if(mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath() > 64)
        {
            TotalSecondForMeltingParaffin = 15 * 60 * 60;
        }
    }

    if(QFile::exists("TEST_ISSAC")) // only for testing time of paraffin
    {
        QFile Test("TEST_ISSAC");
        if (Test.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&Test);
            if(!in.atEnd())
            {
                QString line = in.readLine();
                bool ok = false;
                quint32 time = line.toUInt(&ok);
                if(ok && time > 0)
                {
                    TotalSecondForMeltingParaffin = time * 60;
                }
            }
            Test.close();
        }
    }
    return TotalSecondForMeltingParaffin;

}

quint64 SchedulerMainThreadController::GetOvenHeatingTime()
{
    RaiseEvent(EVENT_SCHEDULER_OVEN_HEATING_TIME,
               QStringList()<<DataManager::Helper::ConvertSecondsToTimeString(m_ProgramStatusInfor.GetRemaingTimeForMeltingParffin()/1000));
    return m_ProgramStatusInfor.GetOvenHeatingTime() / 1000;
}

qint64 SchedulerMainThreadController::GetOvenHeatingRemainingTime()
{
    return m_ProgramStatusInfor.GetRemaingTimeForMeltingParffin();
}

void SchedulerMainThreadController::InitProgramStatus()
{
    quint32 ParaffinMeltPoint = 64;
    if (mp_DataManager != NULL && mp_DataManager->GetUserSettings() != NULL)
    {
        ParaffinMeltPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
    }
    if(!QFile::exists("../Settings/ProgramStatus.txt"))
    {
        RaiseEvent(EVENT_SCHEDULER_READ_PROGRAM_STATUS_FILE_FAIL);
    }
	m_ProgramStatusInfor.InitProgramStatus(ParaffinMeltPoint);
}

bool SchedulerMainThreadController::IsLastStep(int currentStepIndex, const QString& currentProgramID)
{
    if (currentProgramID.isNull() || currentProgramID.isEmpty())
    {
        return false;
    }
    if (!mp_DataManager)
    {
        Q_ASSERT(false);
        return false;//need error handling
    }

    CDataProgramList* pDataProgramList = mp_DataManager->GetProgramList();
    if (!pDataProgramList)
    {
        Q_ASSERT(false);
        return false;
    }

    CProgram* pProgram = const_cast<CProgram*>(pDataProgramList->GetProgram(currentProgramID));
//    ListOfIDs_t* stepIDs = pProgram->OrderedListOfStepIDs();
    return (pProgram->GetNumberOfSteps() == (currentStepIndex + 1));
}

HeaterType_t SchedulerMainThreadController::GetFailerHeaterType()
{
    if (DCL_ERR_DEV_RETORT_LEVELSENSOR_HEATING_OVERTIME == m_CurErrEventID
            || DCL_ERR_DEV_LEVELSENSOR_TEMPERATURE_OVERRANGE == m_CurErrEventID)
    {
        return LEVELSENSOR;
    }
    else if (DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR1_OUTOFRANGE == m_CurErrEventID
             || DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_NOTREACHTARGETTEMP == m_CurErrEventID)
    {
        return LATUBE1;
    }
#if 0
    else if (DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR2_OUTOFRANGE == m_CurErrEventID
             || DCL_ERR_DEV_LA_TUBEHEATING_TUBE2_NOTREACHTARGETTEMP == m_CurErrEventID)
#endif
    else if (DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR2_OUTOFRANGE == m_CurErrEventID)
    {
        return LATUBE2;
    }
    else if (DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_ABNORMAL == m_CurErrEventID)
    {
        return LATUBE1ABNORMAL;
    }
    else if (DCL_ERR_DEV_LA_TUBEHEATING_TUBE2_ABNORMAL == m_CurErrEventID)
    {
        return LATUBE2ABNORMAL;
    }
    else if (DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_NOSIGNAL == m_CurErrEventID
             || DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_NOSIGNAL == m_CurErrEventID
             || DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_NOSIGNAL == m_CurErrEventID)
    {
        return RETORT_NOSIGNAL;
    }
    else if ("50001" == QString::number(m_CurErrEventID).left(5))
    {
        return RETORT;
    }
    else if ("50002" == QString::number(m_CurErrEventID).left(5))
    {
        return OVEN;
    }
    else if ("50003" == QString::number(m_CurErrEventID).left(5))
    {
        return RV;
    }
    else if (DCL_ERR_DEV_ASB5_AC_CURRENT_OUTOFRANGE == m_CurErrEventID)
    {
        return ASB5;
    }
    else if (DCL_ERR_DEV_LA_STATUS_EXHAUSTFAN == m_CurErrEventID)
    {
        return FAN;
    }

    return UNKNOWN;
}

bool SchedulerMainThreadController::CheckRetortTempSensorNoSignal(quint32 Scenario,qreal HWTemp)
{
    if (002 == Scenario || 200 == Scenario || 203 == Scenario
            || (Scenario != 214 && Scenario >=211 && Scenario <=217)
            || (Scenario != 224 && Scenario >= 222 && Scenario <= 227)
            || (Scenario != 234 && Scenario >= 232 && Scenario <= 237)
            || (Scenario != 244 && Scenario >= 241 && Scenario <= 247)
            || (Scenario != 254 && Scenario >= 251 && Scenario <= 257)
            || (Scenario != 274 && Scenario >= 271 && Scenario <=277)
            || (Scenario >= 281 && Scenario <= 297) )
    {
        //if (qFuzzyCompare((qAbs(HWTemp-299+1)), 0.0+1))
        if (qFuzzyCompare(HWTemp,299))
        {
            return false;
        }
    }
    return true;
}

bool SchedulerMainThreadController::CheckLevelSensorNoSignal(quint32 Scenario, qreal HWTemp)
{
    if (200 == Scenario || 203 == Scenario || (Scenario >= 211 && Scenario <= 257)
        || (Scenario >= 271 && Scenario <= 297) )
    {
        if (qFuzzyCompare(HWTemp,299))
        {
            return false;
        }
    }
    return true;
}

void SchedulerMainThreadController::CheckSlaveAllSensor(quint32 Scenario, const HardwareMonitor_t& strctHWMonitor)
{
    // Run Heating Strategy
    if (Scenario > 2)
    {
        DeviceControl::ReturnCode_t retCode = mp_HeatingStrategy->RunHeatingStrategy(strctHWMonitor, Scenario);
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            if(DCL_ERR_DEV_RV_HEATING_TSENSOR2_LESSTHAN_40DEGREEC_OVERTIME == retCode)
            {
                MsgClasses::CmdProgramAcknowledge* commandPtr(new MsgClasses::CmdProgramAcknowledge(5000, DataManager::DISMISS_ROTARY_VALVE_HEATING_PROMPT));
                Q_ASSERT(commandPtr);
                Global::tRefType Ref = GetNewCommandRef();
                SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
            }
            SendOutErrMsg(retCode);
            if(m_IsErrorStateForHM)
                return ;
        }
    }

    ReportError_t reportError1;
    memset(&reportError1, 0, sizeof(reportError1));
    ReportError_t reportError2;
    memset(&reportError2, 0, sizeof(reportError2));
    ReportError_t reportError3;
    memset(&reportError3, 0, sizeof(reportError3));
    ReportError_t reportError4;
    memset(&reportError4, 0, sizeof(reportError4));
    ReportError_t reportError5;
    memset(&reportError5, 0, sizeof(reportError5));
    ReportError_t reportError6;
    memset(&reportError6, 0, sizeof(reportError6));
    ReportError_t reportError7;
    memset(&reportError7, 0, sizeof(reportError7));
    ReportError_t reportError8;
    memset(&reportError8, 0, sizeof(reportError8));
    ReportError_t reportError9;
    memset(&reportError9, 0, sizeof(reportError9));
    reportError1 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Retort", RT_BOTTOM);
    reportError2 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Retort", RT_SIDE);
    reportError3 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Oven", OVEN_TOP);
    reportError4 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Oven", OVEN_BOTTOM);
    reportError5 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "LA", AL_LEVELSENSOR);
    reportError6 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "LA", AL_TUBE1);
    reportError7 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "LA", AL_TUBE2);
    reportError8 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(PRESS_FAN_OUT_OF_RANGE, "LA", AL_FAN);
    reportError9 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "RV",0);
    if (reportError1.instanceID != 0)
    {
        LogDebug(QString("Current out of range, ASB5 AC current is: %1").arg(reportError1.errorData));
        SendOutErrMsg(DCL_ERR_DEV_ASB5_AC_CURRENT_OUTOFRANGE);
        if(m_IsErrorStateForHM)
            return ;
    }
    if (reportError2.instanceID != 0)
    {
        LogDebug(QString("Current out of range, ASB5 AC current is: %1").arg(reportError2.errorData));
        SendOutErrMsg(DCL_ERR_DEV_ASB5_AC_CURRENT_OUTOFRANGE);
        if(m_IsErrorStateForHM)
            return ;
    }
    if (reportError3.instanceID != 0 )
    {
        LogDebug(QString("Current out of range, ASB5 AC current is: %1").arg(reportError3.errorData));
        SendOutErrMsg(DCL_ERR_DEV_ASB5_AC_CURRENT_OUTOFRANGE);
        if(m_IsErrorStateForHM)
            return ;
    }
    if (reportError4.instanceID != 0)
    {
        LogDebug(QString("Current out of range, ASB5 AC current is: %1").arg(reportError4.errorData));
        SendOutErrMsg(DCL_ERR_DEV_ASB5_AC_CURRENT_OUTOFRANGE);
        if(m_IsErrorStateForHM)
            return ;
    }
    if (reportError5.instanceID != 0)
    {
        LogDebug(QString("Current out of range, level sensor current is: %1").arg(reportError5.errorData));
        //SendOutErrMsg(DCL_ERR_DEV_WAXBATH_BOTTOM_HEATINGPAD_CURRENT_OUTOFRANGE);
    }
    if (reportError6.instanceID != 0)
    {
        LogDebug(QString("Current out of range, AL tube1 current is: %1").arg(reportError6.errorData));
        //SendOutErrMsg(DCL_ERR_DEV_WAXBATH_BOTTOM_HEATINGPAD_CURRENT_OUTOFRANGE);
    }
    if (reportError7.instanceID != 0)
    {
        LogDebug(QString("Current out of range, AL tube2 current is: %1").arg(reportError7.errorData));
        //SendOutErrMsg(DCL_ERR_DEV_WAXBATH_BOTTOM_HEATINGPAD_CURRENT_OUTOFRANGE);
    }
    if (reportError8.instanceID != 0)
    {
        LogDebug(QString("Current out of range, Current is: %1").arg(reportError8.errorData));
        SendOutErrMsg(DCL_ERR_DEV_LA_STATUS_EXHAUSTFAN);
        if(m_IsErrorStateForHM)
            return ;
    }
    if (reportError9.instanceID != 0)
    {
        LogDebug(QString("Current out of range, the current is :%1").arg(reportError9.errorData));
        SendOutErrMsg(DCL_ERR_DEV_RV_HEATING_CURRENT_OUTOFRANGE);
        if(m_IsErrorStateForHM)
            return ;
    }

    // For voltage related
    quint16 slave3UpperLimit = (m_SlaveAttrList[0].Voltagerated24VDC + m_SlaveAttrList[0].VoltageTolerance24VDC)*1000;
    quint16 slave3LowerLimit = (m_SlaveAttrList[0].Voltagerated24VDC - m_SlaveAttrList[0].VoltageTolerance24VDC)*1000;
    if (strctHWMonitor.Slave3Voltage < slave3LowerLimit || strctHWMonitor.Slave3Voltage > slave3UpperLimit)
    {
        LogDebug(QString("slave 3 voltage is: %1").arg(strctHWMonitor.Slave3Voltage));
        SendOutErrMsg(DCL_ERR_DEV_MC_VOLTAGE_24V_ASB3_OUTOFRANGE);
        if(m_IsErrorStateForHM)
            return ;
    }

    quint16 slave5UpperLimit =  (m_SlaveAttrList[1].Voltagerated24VDC + m_SlaveAttrList[1].VoltageTolerance24VDC)*1000;
    quint16 slave5LowerLimit =  (m_SlaveAttrList[1].Voltagerated24VDC - m_SlaveAttrList[1].VoltageTolerance24VDC)*1000;
    if (strctHWMonitor.Slave5Voltage < slave5LowerLimit || strctHWMonitor.Slave5Voltage > slave5UpperLimit)
    {
        LogDebug(QString("slave 5 voltage is: %1").arg(strctHWMonitor.Slave5Voltage));
        SendOutErrMsg(DCL_ERR_DEV_MC_VOLTAGE_24V_ASB5_OUTOFRANGE);
        if(m_IsErrorStateForHM)
            return ;
    }

    quint16 slave15UpperLimit = (m_SlaveAttrList[2].Voltagerated24VDC + m_SlaveAttrList[2].VoltageTolerance24VDC)*1000;
    quint16 slave15LowerLimit = (m_SlaveAttrList[2].Voltagerated24VDC - m_SlaveAttrList[2].VoltageTolerance24VDC)*1000;
    if (strctHWMonitor.Slave15Voltage < slave15LowerLimit || strctHWMonitor.Slave15Voltage > slave15UpperLimit)
    {
        LogDebug(QString("slave 15 voltage is: %1").arg(strctHWMonitor.Slave15Voltage));
        SendOutErrMsg(DCL_ERR_DEV_MC_VOLTAGE_24V_ASB15_OUTOFRANGE);
        if(m_IsErrorStateForHM)
            return ;
    }

    // For current related
    if (strctHWMonitor.Slave3Current > m_SlaveAttrList[0].CurrentMax5VDC)
    {
        LogDebug(QString("slave 3 5V current is: %1").arg(strctHWMonitor.Slave3Current));
        SendOutErrMsg(DCL_ERR_DEV_MC_DC_5V_ASB3_OUTOFRANGE);
        if(m_IsErrorStateForHM)
            return ;
    }
    if (strctHWMonitor.Slave5Current > m_SlaveAttrList[1].CurrentMax5VDC)
    {
        LogDebug(QString("slave 5 5V current is: %1").arg(strctHWMonitor.Slave5Current));
        SendOutErrMsg(DCL_ERR_DEV_MC_DC_5V_ASB5_OUTOFRANGE);
        if(m_IsErrorStateForHM)
            return ;
    }

    //Check No-Signal error for Retort sensors
    if (false == this->CheckRetortTempSensorNoSignal(Scenario, strctHWMonitor.TempRTBottom1))
    {
        SendOutErrMsg(DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_NOSIGNAL);
        if(m_IsErrorStateForHM)
            return ;
    }
    if (false == this->CheckRetortTempSensorNoSignal(Scenario, strctHWMonitor.TempRTBottom2))
    {
        LogDebug("The RT bottom2 no signal");
        SendOutErrMsg(DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_NOSIGNAL);
        if(m_IsErrorStateForHM)
            return ;
    }
    if (false == this->CheckRetortTempSensorNoSignal(Scenario, strctHWMonitor.TempRTSide))
    {
        SendOutErrMsg(DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_NOSIGNAL);
        if(m_IsErrorStateForHM)
            return ;
    }
    //the Level Scenario no signal is the same with over range
    if (false == this->CheckLevelSensorNoSignal(Scenario, strctHWMonitor.TempALLevelSensor))
    {
        SendOutErrMsg(DCL_ERR_DEV_LEVELSENSOR_TEMPERATURE_OVERRANGE);
        if(m_IsErrorStateForHM)
            return ;
    }

#if 0
    // remove the pressure out of range error
    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.PressureAL))
    {
        if(qAbs(strctHWMonitor.PressureAL) > 40.0 )
        {
            LogDebug(QString("The pressure in the error case is: %1").arg(strctHWMonitor.PressureAL));
            SendOutErrMsg(DCL_ERR_DEV_LA_PRESSURESENSOR_OUTOFRANGE);
            if(m_IsErrorStateForHM)
                return ;
        }
    }
#endif

    if(strctHWMonitor.OvenLidStatus != UNDEFINED_VALUE && 1 == strctHWMonitor.OvenLidStatus)
    {
        //oven is open
        if(m_CheckOvenCover)
        {
            if ( (Scenario >= 2 && Scenario <= 260) || (Scenario >= 281 && Scenario <= 297) )
            {
                SendOutErrMsg(DCL_ERR_DEV_WAXBATH_OVENCOVER_STATUS_OPEN, false);
                m_CheckOvenCover = false;
            }
        }
        if(Scenario >= 271 && Scenario <= 277)
        {
            ReleasePressure();
            SendOutErrMsg(DCL_ERR_DEV_WAXBATH_OVENCOVER_STATUS_OPEN);
            if(m_IsErrorStateForHM)
                return ;
        }
    }
    if(strctHWMonitor.RetortLockStatus != UNDEFINED_VALUE)
    {
        if(1 == strctHWMonitor.RetortLockStatus && m_CurrentStepState != PSSM_PAUSE)
        {
            if (Scenario >=200 && Scenario <= 297)
            {
                // Notify retort lid is opened
                MsgClasses::CmdProgramAcknowledge* CmdRetortCoverOpen = new MsgClasses::CmdProgramAcknowledge(5000,DataManager::RETORT_COVER_OPERN);
                Q_ASSERT(CmdRetortCoverOpen);
                Global::tRefType fRef = GetNewCommandRef();
                SendCommand(fRef, Global::CommandShPtr_t(CmdRetortCoverOpen));

                SendOutErrMsg(DCL_ERR_DEV_LIDLOCK_CLOSE_STATUS_ERROR);
            }
            else if (007 == Scenario)
            {
                if (!m_SchedulerMachine->NonRVErrorOccuredInBottleCheck())
                {

                    SendOutErrMsg(DCL_ERR_DEV_LIDLOCK_CLOSE_STATUS_ERROR, false);
                    // Notify retort lid is opened
                    MsgClasses::CmdProgramAcknowledge* CmdRetortCoverOpen = new MsgClasses::CmdProgramAcknowledge(5000,DataManager::RETORT_COVER_OPERN);
                    Q_ASSERT(CmdRetortCoverOpen);
                    Global::tRefType fRef = GetNewCommandRef();
                    SendCommand(fRef, Global::CommandShPtr_t(CmdRetortCoverOpen));
                }
            }
            if(m_IsErrorStateForHM)
                return ;
        }
    }
}

void SchedulerMainThreadController::EnablePauseButton()
{
    if (!m_IsCleaningProgram)
        EnablePauseButton(true);
}

void SchedulerMainThreadController::DisablePauseButton()
{
    if (!m_IsCleaningProgram)
        EnablePauseButton(false);
}

void SchedulerMainThreadController::EnablePauseButton(bool bEnable)
{
    ProgramAcknownedgeType_t type =  DataManager::PROGRAM_PAUSE_ENABLE;
    if (!bEnable)
    {
        type =  DataManager::PROGRAM_PAUSE_DISABLE;
    }
    MsgClasses::CmdProgramAcknowledge* commandPtrPauseEnable(new MsgClasses::CmdProgramAcknowledge(5000, type));
    Q_ASSERT(commandPtrPauseEnable);
    Global::tRefType fRef = GetNewCommandRef();
    SendCommand(fRef, Global::CommandShPtr_t(commandPtrPauseEnable));
}

void SchedulerMainThreadController::HandleRmtLocAlarm(quint32 ctrlcmd)
{
    int opcode = -1;
    switch (ctrlcmd) {
    case CTRL_CMD_ALARM_RMT_ON:
        opcode = 5;
        break;
    case CTRL_CMD_ALARM_RMT_OFF:
        opcode = 4;
        break;
    case CTRL_CMD_ALARM_LOC_ON:
        opcode = 3;
        break;
    case CTRL_CMD_ALARM_LOC_OFF:
        opcode = 2;
        break;
    case CTRL_CMD_ALARM_ALL_OFF:
        opcode = -1;
        break;
    case CTRL_CMD_POWER_FAILURE_MEG:
    case CTRL_CMD_DRAIN_SR:
        Global::AlarmHandler::Instance().reset();
        opcode = -1;
        break;
    default:
        return ;
    }

    CmdRmtLocAlarm *cmd = new CmdRmtLocAlarm(500, this);
    cmd->SetRmtLocOpcode(opcode);
    m_SchedulerCommandProcessor->pushCmd(cmd);
}

bool SchedulerMainThreadController::ConstructSlaveModuleAttrList(QString moduleName)
{
    bool ok = false;
    DataManager::FunctionKey_t funcKey;
    funcKey.key = moduleName;
    funcKey.name = "";
    funcKey.sequence = "";
    qreal maxCurrent = mp_DataManager->GetProgramSettings()->GetParameterValue("MainControl", funcKey, "CurrentMax5DC", ok);
    if (false == ok)
    {
        return false;
    }
    qreal voltage = mp_DataManager->GetProgramSettings()->GetParameterValue("MainControl", funcKey, "VoltageRated24VDC", ok);
    if (false == ok)
    {
        return false;
    }
    qreal voltageTolerance = mp_DataManager->GetProgramSettings()->GetParameterValue("MainControl", funcKey, "VoltageTolerance24VDC", ok);
    if (false == ok)
    {
        return false;
    }

    SlaveAttr_t attrList;
    attrList.CurrentMax5VDC = maxCurrent;
    attrList.Voltagerated24VDC = voltage;
    attrList.VoltageTolerance24VDC = voltageTolerance;
    m_SlaveAttrList.push_back(attrList);
    return true;
}

void SchedulerMainThreadController::OnSystemError()
{
    if(m_CurrentScenario > 2)
    {
        m_ProgramStatusInfor.SetErrorFlag(1);
    }
    if(PSSM_PROCESSING == m_CurrentStepState)
    {
        if(m_completionNotifierSent)
        {
            MsgClasses::CmdProgramAcknowledge* command(new MsgClasses::CmdProgramAcknowledge(5000,
                                                                    DataManager::CANCEL_PROGRAM_WILL_COMPLETE_PROMPT));
            Global::tRefType ref = GetNewCommandRef();
            SendCommand(ref, Global::CommandShPtr_t(command));
            m_completionNotifierSent = false;
        }
    }

    ProgramAcknownedgeType_t type =  DataManager::PROGRAM_SYSTEM_EEEOR;
    MsgClasses::CmdProgramAcknowledge* commandPtrSystemError(new MsgClasses::CmdProgramAcknowledge(5000, type));
    Q_ASSERT(commandPtrSystemError);
    Global::tRefType fRef = GetNewCommandRef();
    SendCommand(fRef, Global::CommandShPtr_t(commandPtrSystemError));

    m_TimeStamps.SystemErrorStartTime = QDateTime::currentMSecsSinceEpoch();
}

void SchedulerMainThreadController::OnBackToBusy()
{
    ProgramAcknownedgeType_t type =  DataManager::PROGRAM_SYSTEM_RC_RESTART;
    MsgClasses::CmdProgramAcknowledge* commandPtrRcRestart(new MsgClasses::CmdProgramAcknowledge(5000, type));
    Q_ASSERT(commandPtrRcRestart);
    Global::tRefType fRef = GetNewCommandRef();
    SendCommand(fRef, Global::CommandShPtr_t(commandPtrRcRestart));

    if ((PSSM_PROCESSING != m_CurrentStepState) && (-1 != m_CurProgramStepIndex))
    {
        qint64 delta = (QDateTime::currentMSecsSinceEpoch() - m_TimeStamps.SystemErrorStartTime) / 1000;
        MsgClasses::CmdUpdateProgramEndTime* commandUpdateProgramEndTime(new MsgClasses::CmdUpdateProgramEndTime(5000, delta));
        Q_ASSERT(commandUpdateProgramEndTime);
        Global::tRefType tfRef = GetNewCommandRef();
        SendCommand(tfRef, Global::CommandShPtr_t(commandUpdateProgramEndTime));
    }
}

void SchedulerMainThreadController::OnFillingHeatingRV()
{
    quint32 leftSeconds = GetCurrentProgramStepNeededTime(m_CurProgramID);
    MsgClasses::CmdCurrentProgramStepInfor* commandPtr(new MsgClasses::CmdCurrentProgramStepInfor(5000, m_CurReagnetName, m_CurProgramStepIndex, leftSeconds));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

    QString PVMode = "/";
    if(m_CurProgramStepInfo.isPressure)
    {
        PVMode = "P/";
    }
    if(m_CurProgramStepInfo.isVacuum)
    {
        PVMode += "V";
    }
    RaiseEvent(EVENT_SCHEDULER_PROGRAM_STEP_START,QStringList()<<QString("[%1]").arg(m_CurProgramStepIndex)
               << m_CurProgramStepInfo.stationID
               <<m_CurReagnetName << DataManager::Helper::ConvertSecondsToTimeString(m_CurProgramStepInfo.durationInSeconds)
               <<(m_CurProgramStepInfo.temperature > 0 ? QString("[%1]").arg(m_CurProgramStepInfo.temperature) : QString("Amb"))
               <<PVMode);

    if(m_CurProgramStepInfo.reagentGroup == "RG6")
    {
        mp_HeatingStrategy->Init260ParamList();
        RaiseEvent(EVENT_SCHEDULER_WAITING_FOR_FILLING_PARAFFIN);
    }

    this->EnablePauseButton();
    m_TickTimer.start();
}

void SchedulerMainThreadController::OnPreTestDone()
{
    ProgramAcknownedgeType_t type =  DataManager::PROGRAM_PRETEST_DONE;
    MsgClasses::CmdProgramAcknowledge* commandPreTest(new MsgClasses::CmdProgramAcknowledge(5000, type));
    Q_ASSERT(commandPreTest);
    Global::tRefType fRef = GetNewCommandRef();
    SendCommand(fRef, Global::CommandShPtr_t(commandPreTest));
}

qreal SchedulerMainThreadController::GetLastPressureOffset()
{
    return m_ProgramStatusInfor.GetPressureDriftOffset();
}

void SchedulerMainThreadController::SetLastPressureOffset(qreal pressureOffset)
{
    m_ProgramStatusInfor.SetPressureDriftOffset(pressureOffset);
}

void SchedulerMainThreadController::SendPowerFailureMsg()
{
    MsgClasses::CmdProgramAcknowledge* CmdPowerFailureMsg = new MsgClasses::CmdProgramAcknowledge(5000,DataManager::POWER_FAILURE_MSG);
    Q_ASSERT(CmdPowerFailureMsg);
    Global::tRefType fRef = GetNewCommandRef();
    SendCommand(fRef, Global::CommandShPtr_t(CmdPowerFailureMsg));
}

void SchedulerMainThreadController::OnExitedInitState()
{
    InitProgramStatus();
}

void SchedulerMainThreadController::GetStringIDList(quint32 ErrorID,
                                                    Global::tTranslatableStringList &EventStringParList,
                                                    Global::tTranslatableStringList &EventRDStringParList)
{
    quint32 InternalSteps = 0;
    quint32 RVPosition = 0;
    switch(ErrorID)
    {
        case 513040012:
        case 513040161:
        case 513040163:
            EventStringParList<<QString("%1").arg(ErrorID);
            EventRDStringParList<<QString("%1").arg(m_CurrentBottlePosition.StationID);
            break;
        case 513040203:
        case 513040201:
            EventStringParList<<QString("%1").arg(ErrorID);
            EventRDStringParList<<QString("%1").arg(m_CurrentBottlePosition.StationID)<<QString("%1").arg(m_SchedulerCommandProcessor->HardwareMonitor().PressureAL);
            break;
        case 513040014:
        case 513040241:
        case 511040101:
        case 511040221:
            EventStringParList<<QString("%1").arg(ErrorID);
            EventRDStringParList<<QString("%1").arg(m_CurrentBottlePosition.StationID);
            break;
        case 513040501:
        case 512040520:
        case 512040523:
            EventStringParList<<QString("%1").arg(ErrorID);
            EventRDStringParList<<QString("%1").arg(m_SchedulerCommandProcessor->HardwareMonitor().TempALTube1);
            break;
        case 513040511:
        case 512040530:
        case 512040533:
            EventStringParList<<QString("%1").arg(ErrorID);
            EventRDStringParList<<QString("%1").arg(m_SchedulerCommandProcessor->HardwareMonitor().TempALTube2);
            break;
        case 513030031:
        case 513030033:
            EventStringParList<<QString("%1").arg(ErrorID);
            RVPosition = (quint32)m_ProgramStatusInfor.GetLastRVPosition();
            InternalSteps =m_SchedulerCommandProcessor->HardwareMonitor().PositionRV - RVPosition;
            EventRDStringParList<<QString("%1").arg(RVPosition)<<QString("%1").arg(m_CurrentBottlePosition.RvPos)
                               <<QString("%1").arg(m_SchedulerCommandProcessor->HardwareMonitor().LowerLimit)
                               <<QString("%1").arg(InternalSteps);
            break;
        case 513030011:
        case 513030012:
            EventStringParList<<QString("%1").arg(ErrorID);
            RVPosition = (quint32)m_ProgramStatusInfor.GetLastRVPosition();
            EventRDStringParList<<QString("%1").arg(RVPosition)<<QString("%1").arg(m_CurrentBottlePosition.RvPos);
            break;
        case 513040020:
        case 511040021:
        case 511040022:
        case 512040052:
        case 512040053:
            EventStringParList<<QString("%1").arg(ErrorID);
            EventRDStringParList<<QString("%1").arg(m_PressureAL);
            break;
        case 513060020:
            EventStringParList<<QString("%1").arg(ErrorID);
            EventRDStringParList<<QString("%1").arg(ErrorID);
            break;
        default:
            EventStringParList << QString("%1").arg(ErrorID);
            break;
    }
}

void SchedulerMainThreadController::SendSafeReagentFinishedCmd()
{
    QString ProgramName = mp_DataManager->GetProgramList()->GetProgram(m_CurProgramID)->GetName();
    RaiseEvent(EVENT_SCHEDULER_PROGRAM_FINISHED,QStringList()<<ProgramName);

    //send command to main controller to tell program finished
    Global::tRefType Ref = GetNewCommandRef();
    if(m_IsCleaningProgram)
    {
        MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::CLEANING_PROGRAM_COMPLETE_AS_SAFE_REAGENT));
        Q_ASSERT(commandPtrFinish);
        Ref = GetNewCommandRef();
        SendCommand(Ref, Global::CommandShPtr_t(commandPtrFinish));
    }
    else
    {
        if(m_ProgramStatusInfor.IsRetortContaminted())
        {
            MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_FINISHED_AS_SAFE_REAGENT));
            Q_ASSERT(commandPtrFinish);
            Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtrFinish));
        }
        else
        {
            MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_FINISHED_NO_CONTAMINATED));
            Q_ASSERT(commandPtrFinish);
            Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtrFinish));
        }
    }
    m_ProgramStatusInfor.SetErrorFlag(0);
}

void SchedulerMainThreadController::CompleteRsAbort()
{
    //program finished
    AllStop();

    m_SchedulerMachine->SendRunComplete();

    // tell the main controller the program has been aborted
    MsgClasses::CmdProgramAborted* commandPtrAbortFinish(new MsgClasses::CmdProgramAborted(5000, m_ProgramStatusInfor.IsRetortContaminted()));
    Q_ASSERT(commandPtrAbortFinish);
    Global::tRefType fRef = GetNewCommandRef();
    SendCommand(fRef, Global::CommandShPtr_t(commandPtrAbortFinish));

    m_CurProgramStepIndex = -1;
    LogDebug("Program aborted!");
    RaiseEvent(EVENT_SCHEDULER_OVEN_ABORTED);
}

void SchedulerMainThreadController::SendOutErrMsg(ReturnCode_t EventId, bool IsErrorMsg)
{
    //First of all, we added one workaround for scenario 7 (bottle check)
    if (7 == m_CurrentScenario)
    {
        m_SchedulerMachine->CheckNonRVErr4BottleCheck(EventId);
    }
    bool ret = false;
    ret = RaiseError(0, EventId, m_CurrentScenario, true);
    if(!ret)
    {
        //log
        QString temp = QString::number(EventId) + QString::number(m_CurrentScenario);
        if(-1 == m_UnknownErrorLogVector.indexOf(temp))
        {
            RaiseEvent(EVENT_SCHEDULER_UNKNOW_ERROR, QStringList()<<QString("[%1]").arg(EventId)<<QString("[%1]").arg(m_CurrentScenario));
            m_UnknownErrorLogVector.push_back(temp);
        }
    }
    else
    {
        if(IsErrorMsg)
        {
            m_SchedulerMachine->SendErrorSignal();
            m_IsErrorStateForHM = true;
        }
    }
}

/*lint -e641 */
bool SchedulerMainThreadController::RaiseError(const quint32 EventKey, ReturnCode_t EventID, const quint32 Scenario,
                          const bool ActionResult, const bool Active)
{
    if(EventKey == 0)
    {
        // If EventID is less than 0x1000, the error is SW internal error. In this case, we always use below one
        if (EventID < 0x1000)
        {
            LogDebug(QString("SW Internal error ID is: %1").arg(EventID));
            EventID = DCL_ERR_DEV_INTER_SW_ERROR;
        }

        quint32 ErrorID = m_pESEXMLInfo->GetErrorCode(EventID,Scenario);
        if(ErrorID == 0)//unknow error
        {
            return false;
        }
        // We only update current event ID when current status is NOT error.
        SchedulerStateMachine_t currentState = m_SchedulerMachine->GetCurrentState();
        if (SM_ERROR != (currentState & 0xFF))
        {
            m_CurErrEventID = EventID;
        }

        Global::tTranslatableStringList EventStringParList;
        Global::tTranslatableStringList EventRDStringParList;
        GetStringIDList(ErrorID, EventStringParList, EventRDStringParList);
        Global::EventObject::Instance().RaiseEvent(EventKey, ErrorID, ActionResult,Active, EventStringParList, EventRDStringParList);
    }
    else
    {
        Global::EventObject::Instance().RaiseEvent(EventKey, 0, ActionResult,Active);
    }
    return true;
}

void SchedulerMainThreadController::DismissPausingMsgDlg()
{
     SendProgramAcknowledge(DISMISS_PAUSING_MSG_DLG);
}

void SchedulerMainThreadController::SendBottleCheckReply(const QString& stationId, DataManager::BottleCheckStatusType_t type)
{
    MsgClasses::CmdBottleCheckReply* commandPtr(new MsgClasses::CmdBottleCheckReply(15000, stationId, type));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref2 = GetNewCommandRef();
    SendCommand(Ref2, Global::CommandShPtr_t(commandPtr));
}


}
