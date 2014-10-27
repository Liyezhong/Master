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
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAborted.h"
#include "HimalayaDataContainer/Containers/ReagentStations/Commands/Include/CmdUpdateStationReagentStatus.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdKeepCassetteCount.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelected.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelectedReply.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdStationSuckDrain.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdLockStatus.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdUpdateProgramEndTime.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdRecoveryFromPowerFailure.h"
#include "HimalayaDataContainer/Containers/UserSettings/Commands/Include/CmdQuitAppShutdown.h"
#include "HimalayaDataContainer/Containers/UserSettings/Commands/Include/CmdParaffinMeltPointChanged.h"
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
#include <Global/Include/SystemPaths.h>
#include <DataManager/CommandInterface/Include/UserSettingsCommandInterface.h>

using namespace DataManager;

/*lint -e527 */
/*lint -e616 */
/*lint -e613 */

namespace Scheduler {

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
        , m_SchedulerMachine(new CSchedulerStateMachine(this))
        , mp_DataManager(NULL)
        , m_CurProgramStepIndex(-1)
        , m_FirstProgramStepIndex(0)
        , m_CurReagnetName("")
        , m_CurProgramID("")
        , m_NewProgramID("")
        , m_OvenLidStatus(0)
        , m_RetortLockStatus(UNDEFINED_VALUE)
        , m_ProcessCassetteCount(0)
        , mp_HeatingStrategy(NULL)
        , m_RefCleanup(Global::RefManager<Global::tRefType>::INVALID)
        , m_delayTime(0)
        , m_IsInSoakDelay(false)
        , m_hasParaffin(false)
        , m_Is5MinPause(false)
        , m_Is10MinPause(false)
        , m_Is15MinPause(false)
{
    memset(&m_TimeStamps, 0, sizeof(m_TimeStamps));
    m_CurErrEventID = DCL_ERR_FCT_NOT_IMPLEMENTED;
    m_IsPrecheckMoveRV = false;

    m_lastPVTime = 0;
    m_ProcessingPV = 3; // 0 for Pressure and 1 for Vacuum, 3 for avoiding message to pop up too many times
    m_completionNotifierSent = false;
    m_IsReleasePressureOfSoakFinish = false;
    m_ReleasePressureSucessOfSoakFinish = false;
    m_IsCleaningProgram = false;
    m_CleanAckSentGui = false;
    m_CurrentStepState = PSSM_INIT;
    m_IsSafeReagentState = false;
    m_CmdDrainSR_Click = false;
    m_StopFilling = false;
    m_CheckRemoteAlarmStatus = true;
    m_CheckLocalAlarmStatus = true;

    (void)m_ProgramStatusInfor.ReadProgramStatusFile();
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
    CONNECTSIGNALSLOT(mp_DataManager->mp_SettingsCommandInterface, ResetActiveCarbonFilterLifeTime(),
                     this, ResetActiveCarbonFilterLifetime());

    m_TickTimer.setInterval(500);

    //command queue reset
    m_SchedulerCmdQueue.clear();
    m_DeviceControlCmdQueue.clear();

    // now register commands
    RegisterCommands();

    //InitializeDevice();

    //for debug
    LogDebug(QString("Current state of Scheduler is: %1").arg(m_SchedulerMachine->GetCurrentState()));
    m_SchedulerMachine->Start();
}

void SchedulerMainThreadController::DevProcDestroyed()
{
    if (Global::RefManager<Global::tRefType>::INVALID != m_RefCleanup) {
          SendAcknowledgeOK(m_RefCleanup);
      }
}

void SchedulerMainThreadController::ReportGetServiceInfo(ReturnCode_t ReturnCode, const DataManager::CModule &ModuleInfo, const QString& DeviceType)
{
    Q_UNUSED(ReturnCode)
    //send command
    SendCommand(GetNewCommandRef(), Global::CommandShPtr_t(new MsgClasses::CmdModuleListUpdate(3000, ModuleInfo, DeviceType, false)));
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
        RaiseError(0, DCL_ERR_DEV_LA_FILLING_TIMEOUT_2MIN, m_CurrentScenario, true);
    }
}
void SchedulerMainThreadController::OnReportDrainingTimeOut2Min()
{
    QString ReagentGroup = m_CurProgramStepInfo.reagentGroup;
    quint32 Scenario = GetScenarioBySchedulerState(m_SchedulerMachine->GetCurrentState(),ReagentGroup);
    if (0 != Scenario)
    {
        RaiseError(0, DCL_ERR_DEV_LA_DRAINING_TIMEOUT_EMPTY_2MIN, m_CurrentScenario, true);
    }
}

void SchedulerMainThreadController::CleanupAndDestroyObjects()
{
}

void SchedulerMainThreadController::OnGoReceived()
{
   // m_TickTimer.start();
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

    if (CTRL_CMD_SHUTDOWN == newControllerCmd)
    {
        m_TickTimer.stop();
        m_SchedulerCommandProcessor->ShutDownDevice();
        //DequeueNonDeviceCommand();
        return;
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

    SchedulerCommandShPtr_t cmd;
    (void)PopDeviceControlCmdQueue(cmd);

    SchedulerStateMachine_t currentState = m_SchedulerMachine->GetCurrentState();
    m_SchedulerMachine->UpdateCurrentState(currentState);

    switch(currentState & 0xFF)
    {
    case SM_INIT:
        //In INIT state will do self test
        HardwareMonitor("INIT");
        HandleInitState(newControllerCmd, cmd);
        break;
    case SM_POWER_FAILURE:
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
        LogDebug("Self test is done");
        //send command to main controller to tell self test OK
        if(!m_ProgramStatusInfor.IsProgramFinished())//power failure
        {
            QString ProgramName = "";
            if(mp_DataManager&& mp_DataManager->GetProgramList()&&mp_DataManager->GetProgramList()->GetProgram(m_ProgramStatusInfor.GetProgramId()))
            {
                ProgramName = mp_DataManager->GetProgramList()->GetProgram(m_ProgramStatusInfor.GetProgramId())->GetName();
            }
            RaiseEvent(EVENT_SCHEDULER_POWER_FAILURE,QStringList()<<ProgramName<<QString("[%1]").arg(m_ProgramStatusInfor.GetStepID()));
            m_SchedulerMachine->EnterPowerFailure();
            MsgClasses::CmdRecoveryFromPowerFailure* commandPtr(
                        new MsgClasses::CmdRecoveryFromPowerFailure(5000,m_ProgramStatusInfor.GetProgramId(),
                                                                    m_ProgramStatusInfor.GetStepID(),
                                                                    m_ProgramStatusInfor.GetScenario(),
                                                                    GetLeftProgramStepsNeededTime(m_ProgramStatusInfor.GetProgramId(),m_ProgramStatusInfor.GetStepID()),
                                                                    m_ProgramStatusInfor.GetLastReagentGroup(),
                                                                    m_ProgramStatusInfor.GetStationList()));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        }
        else
        {
            m_SchedulerMachine->SendSchedulerInitComplete();
            MsgClasses::CmdProgramAcknowledge* commandPtr(new MsgClasses::CmdProgramAcknowledge(5000, DataManager::PROGRAM_READY));
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

void SchedulerMainThreadController::HandlePowerFailure(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd)
{
    Q_UNUSED(ctrlCmd);
    Q_UNUSED(cmd);

    QString curProgramID = m_ProgramStatusInfor.GetProgramId();
    quint32 scenario = m_ProgramStatusInfor.GetScenario();
    int StepID = m_ProgramStatusInfor.GetStepID();
    if(-1 == StepID)
    {
        StepID = 0;
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
    }
    else if(281 <= scenario && scenario <= 297)
    {
        m_IsCleaningProgram = true;
        m_CurrentStepState = PSSM_FILLING_LEVELSENSOR_HEATING;
    }

    RaiseError(0, DCL_ERR_DEV_POWERFAILURE, scenario, true);
    m_SchedulerMachine->SendErrorSignal();
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

void SchedulerMainThreadController::HandleIdleState(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd)
{
    Q_UNUSED(cmd)
    if (m_ProgramStatusInfor.IsRetortContaminted() && !m_CleanAckSentGui)
    {
        m_CleanAckSentGui = true;
        MsgClasses::CmdProgramAcknowledge* commandEnterCleaning(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::TAKE_OUT_SPECIMEN_WAIT_RUN_CLEANING));
        Q_ASSERT(commandEnterCleaning);
        Global::tRefType fRef = GetNewCommandRef();
        SendCommand(fRef, Global::CommandShPtr_t(commandEnterCleaning));
    }
    switch (ctrlCmd)
    {
    case CTRL_CMD_START:
        m_UsedStationIDs.clear();

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

        if(m_CurProgramStepIndex != -1)
        {
            //send command to main controller to tell program is starting
            MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_BEGIN));
            Q_ASSERT(commandPtrFinish);
            Global::tRefType fRef = GetNewCommandRef();
            SendCommand(fRef, Global::CommandShPtr_t(commandPtrFinish));

            QString ProgramName = mp_DataManager->GetProgramList()->GetProgram(m_CurProgramID)->GetName();
            RaiseEvent(EVENT_SCHEDULER_START_PROGRAM,QStringList()<<ProgramName);
            //start next step
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
                       <<m_CurReagnetName << DataManager::Helper::ConvertSecondsToTimeString(m_CurProgramStepInfo.durationInSeconds)
                       <<(m_CurProgramStepInfo.durationInSeconds > 0 ? QString("[%1]").arg(m_CurProgramStepInfo.durationInSeconds) : QString("Amb"))
                       <<PVMode);

            //send command to main controller to tell the left time
            QString strStep;
            if (m_NewProgramID.at(0) != 'C')
            {
                strStep = "Precheck...";
                quint32 leftSeconds = GetCurrentProgramStepNeededTime(m_CurProgramID);
                MsgClasses::CmdCurrentProgramStepInfor* commandPtr(new MsgClasses::CmdCurrentProgramStepInfor(5000,
                                                                                                              Global::UITranslator::TranslatorInstance().Translate(STR_SCHEDULER_PRECHECK),
                                                                                                              m_CurProgramStepIndex, leftSeconds));
                Q_ASSERT(commandPtr);
                Global::tRefType Ref = GetNewCommandRef();
                SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
            }
            else
                strStep= m_CurReagnetName;

            quint32 leftSeconds = GetCurrentProgramStepNeededTime(m_CurProgramID);
            MsgClasses::CmdCurrentProgramStepInfor* commandPtr(new MsgClasses::CmdCurrentProgramStepInfor(5000, strStep, m_CurProgramStepIndex, leftSeconds));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

            // Set current step to Init
            m_CurrentStepState = PSSM_INIT;

            //whether cleaning program or not
            if ( 'C' == ProgramName.at(0) )
            {
                RaiseEvent(EVENT_SCHEDULER_SET_RV_POSITION,QStringList()<<QString("[%1]").arg(m_ProgramStatusInfor.GetLastRVPosition()));
                CmdRVReqMoveToInitialPosition* cmdSet = new CmdRVReqMoveToInitialPosition(500, this);
                cmdSet->SetRVPosition(m_ProgramStatusInfor.GetLastRVPosition());
                m_SchedulerCommandProcessor->pushCmd(cmdSet);

                SchedulerCommandShPtr_t resCmdPtr;
                PopDeviceControlCmdQueue(resCmdPtr,"Scheduler::RVReqMoveToInitialPosition");
                ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;
                (void)resCmdPtr->GetResult(retCode);
                if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
                {
                    RaiseError(0, retCode, 0, true);
                }
                else
                {
                    m_SchedulerMachine->SendRunSignal();
                }
            }
            else
            {
                m_SchedulerMachine->SendRunSignal();
            }
        }
        break;
    default:
        break;
    }
}

void SchedulerMainThreadController::UpdateStationReagentStatus()
{
    MsgClasses::CmdUpdateStationReagentStatus* commandPtr = NULL;
    DataManager::CHimalayaUserSettings* pUserSetting = mp_DataManager->GetUserSettings();
    if (m_CurProgramID.at(0) == 'C')//process cleaning reagent
    {
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
            commandPtr = new MsgClasses::CmdUpdateStationReagentStatus(5000, m_UsedStationIDs, m_ProcessCassetteCount);//toDo: 100, should get the actual number
            m_ProcessCassetteCount = 0; // clear cassette when program finished or aborted.
        }
    }

    if(commandPtr)
    {
        Global::tRefType Ref = GetNewCommandRef();
        SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
    }
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

    if(CTRL_CMD_ABORT == ctrlCmd)
    {
        m_SchedulerMachine->NotifyAbort();
        //tell main controller scheduler is starting to abort
        MsgClasses::CmdProgramAcknowledge* commandPtrAbortBegin(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_ABORT_BEGIN));
        Q_ASSERT(commandPtrAbortBegin);
        Global::tRefType fRef = GetNewCommandRef();
        SendCommand(fRef, Global::CommandShPtr_t(commandPtrAbortBegin));
    }
    else
    {
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
                CmdALStopCmdExec* ALStopCmd = NULL;
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
                    // Stop filling at first
                    if(false == m_StopFilling)
                    {
                        ALStopCmd = new CmdALStopCmdExec(500, this);
                        ALStopCmd->SetCmdType(0);
                        m_SchedulerCommandProcessor->pushCmd(ALStopCmd);
                        m_StopFilling = true;
                    }
                    else if( (DCL_ERR_FCT_CALL_SUCCESS == retCode) && ("Scheduler::ALStopCmdExec" == cmdName) )
                    {
                        m_SchedulerMachine->SendResumeFillingLevelSensorHeating();
                        m_StopFilling = false;
                    }
                    break;
                case PSSM_RV_MOVE_TO_SEAL:
                    m_SchedulerMachine->SendResumeRVMoveToSeal();
                    break;
               case PSSM_PROCESSING:
                    m_SchedulerMachine->SendResumeProcessing();
                    break;
                case PSSM_PROCESSING_SR:
                     m_SchedulerMachine->SendResumeProcessingSR();
                     break;
               case PSSM_RV_MOVE_TO_TUBE:
                    m_SchedulerMachine->SendResumeRVMoveTube();
                    break;
                case PSSM_DRAINING:
                    m_SchedulerMachine->HandleRcRestartAtDrain(cmdName, retCode);
                    break;
                case PSSM_RV_POS_CHANGE:
                    m_SchedulerMachine->SendResumeRVPosChange();
                    break;
                case PSSM_CLEANING_DRY_STEP:
                    m_SchedulerMachine->SendResumeDryStep();
                    break;
                case PSSM_POWERFAILURE_FINISH:
                    if(200 == m_ProgramStatusInfor.GetScenario() || 211 == m_ProgramStatusInfor.GetScenario())
                    {
                        MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_FINISHED));
                        Q_ASSERT(commandPtrFinish);
                        Global::tRefType Ref = GetNewCommandRef();
                        SendCommand(Ref, Global::CommandShPtr_t(commandPtrFinish));
                    }
                    m_SchedulerMachine->SendRunComplete();
                    break;
                case PSSM_ABORTING:
                    m_SchedulerMachine->SendResumeAborting();
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
                if(mp_HeatingStrategy->CheckSensorsTemp(m_SchedulerCommandProcessor->HardwareMonitor()))
                {
                    LogDebug("Program Step Heating Rotary Valve heating rod OK");
                    m_SchedulerMachine->NotifyRVRodHeatingReady();
                }
            }
            else
            {
                m_SchedulerMachine->NotifyRVRodHeatingReady();
            }
        }
        else if (PSSM_FILLING_LEVELSENSOR_HEATING == stepState)
        {
            m_CurrentStepState = PSSM_FILLING_LEVELSENSOR_HEATING;
            if(mp_HeatingStrategy->CheckLevelSensorHeatingStatus())
            {
                LogDebug("Program Step Heating Level sensor stage OK");
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
                EnablePauseButton();
                Global::EventObject::Instance().RaiseEvent(EVENT_WAIT_FILLING_FINISH);
            }

            if( "Scheduler::ALFilling" == cmdName)
            {
                if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    LogDebug(QString("Program Step Filling OK"));
                    m_SchedulerMachine->NotifyFillFinished();
                }
                else
                {
                    LogDebug(QString("Program Step Filling failed"));
                    RaiseError(0, retCode, m_CurrentScenario, true);
                    m_SchedulerMachine->SendErrorSignal();
                }
            }
        }
        else if(PSSM_RV_MOVE_TO_SEAL == stepState)
        {
            m_CurrentStepState = PSSM_RV_MOVE_TO_SEAL;

            if(CTRL_CMD_PAUSE == ctrlCmd)
            {
                EnablePauseButton();
                Global::EventObject::Instance().RaiseEvent(EVENT_WAIT_FILLING_FINISH);
            }

            if(IsRVRightPosition(SEAL_POS))
            {
                m_SchedulerMachine->NotifyRVMoveToSealReady();
            }
            else
            {
                if(("Scheduler::RVReqMoveToRVPosition" == cmdName))
                {
                    if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
                    {
                        RaiseError(0, retCode, m_CurrentScenario, true);
                        m_SchedulerMachine->SendErrorSignal();
                    }
                }
            }
        }
        else if(PSSM_PROCESSING == stepState)
        {
            m_CurrentStepState = PSSM_PROCESSING;
            if(CTRL_CMD_PAUSE == ctrlCmd)
            {
                LogDebug(QString("Program Step Beginning Pause"));
                if(m_CurProgramStepInfo.isPressure || m_CurProgramStepInfo.isVacuum)
                {
                    AllStop();
                    m_lastPVTime = 0;
                }
                m_SchedulerMachine->NotifyPause(PSSM_PROCESSING);
            }
            else
            {
                qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();/*lint -e647 */
                qint64 period = m_CurProgramStepInfo.durationInSeconds * 1000;
                if((now - m_TimeStamps.CurStepSoakStartTime ) > (period))//Will finish Soaking
                {
                    if(!m_IsReleasePressureOfSoakFinish)
                    {
                        m_SchedulerCommandProcessor->pushCmd(new CmdALReleasePressure(500,  this));
                        m_IsReleasePressureOfSoakFinish = true;
                        m_ReleasePressureSucessOfSoakFinish = false;
                    }
                    else if("Scheduler::ALReleasePressure" == cmdName)
                    {
                        if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                        {
                            m_ReleasePressureSucessOfSoakFinish = true;
                        }
                        else
                        {
                            RaiseError(0, retCode, m_CurrentScenario, true);
                            m_SchedulerMachine->SendErrorSignal();
                        }
                    }
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
                         if(CTRL_CMD_DRAIN == ctrlCmd)
                         {
                              if(m_ReleasePressureSucessOfSoakFinish)
                              {
                                  LogDebug(QString("last Program Processing(Soak) Process finished"));
                                  m_SchedulerMachine->NotifyProcessingFinished();
                                  m_TimeStamps.CurStepSoakStartTime = 0;
                                  m_completionNotifierSent = false;
                                  m_IsReleasePressureOfSoakFinish = false;
                                  m_ReleasePressureSucessOfSoakFinish = false;
                              }
                         }
                    }
                    else
                    {
                        if(m_ReleasePressureSucessOfSoakFinish)
                        {
                            LogDebug(QString("Program Processing(Soak) Process finished"));
                            m_SchedulerMachine->NotifyProcessingFinished();
                            m_TimeStamps.CurStepSoakStartTime = 0;
                            m_IsReleasePressureOfSoakFinish = false;
                            m_ReleasePressureSucessOfSoakFinish = false;
                        }
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
                                    RaiseError(0, DCL_ERR_DEV_LA_SEALING_FAILED_PRESSURE, m_CurrentScenario, true);
                                }
                                else if (1 == m_ProcessingPV)
                                {
                                    m_ProcessingPV = 3; // Avoid the warning message to pop up too many times
                                    RaiseError(0, DCL_ERR_DEV_LA_SEALING_FAILED_VACUUM, m_CurrentScenario, true);
                                }
                            }
                        }

                    }
                }
            }
        }
        else if (PSSM_PROCESSING_SR == stepState)
        {
            m_CurrentStepState = PSSM_PROCESSING_SR;
            if(CTRL_CMD_DRAIN_SR == ctrlCmd)
            {
                m_IsSafeReagentState = true;
                m_SchedulerMachine->NotifyProcessingFinished();
            }
        }
        else if(PSSM_RV_MOVE_TO_TUBE == stepState)
        {
            m_CurrentStepState = PSSM_RV_MOVE_TO_TUBE;

            if (CTRL_CMD_PAUSE == ctrlCmd)
            {
               EnablePauseButton();
               Global::EventObject::Instance().RaiseEvent(EVENT_WAIT_DRAINING_FINISH);
            }

            if(IsRVRightPosition(TUBE_POS))
            {
                m_SchedulerMachine->NotifyRVMoveToTubeReady();
            }
            else
            {
                if(("Scheduler::RVReqMoveToRVPosition" == cmdName))
                {
                    if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
                    {
                        RaiseError(0, retCode, m_CurrentScenario, true);
                        m_SchedulerMachine->SendErrorSignal();
                    }
                }

            }
        }
        else if(PSSM_DRAINING == stepState)
        {
            m_CurrentStepState = PSSM_DRAINING;

             if (CTRL_CMD_PAUSE == ctrlCmd)
             {
                EnablePauseButton();
                Global::EventObject::Instance().RaiseEvent(EVENT_WAIT_DRAINING_FINISH);
             }

             //In case that Scheduler was recovered from Error
             if( "Scheduler::ALDraining"== cmdName)
             {
                if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    LogDebug(QString("Program Step Draining succeed!"));
                    if (false == m_IsSafeReagentState)
                    {
                        m_SchedulerMachine->NotifyDrainFinished();
                    }
                    else
                    {
                        m_IsSafeReagentState = false; // Reset
                        m_SchedulerMachine->NotifyProgramFinished();
                    }
                }
                else
                {
                    LogDebug(QString("Program Step Draining Build Pressure timeout"));
                    RaiseError(0, retCode, m_CurrentScenario, true);
                    m_SchedulerMachine->SendErrorSignal();
                }
             }
        }
        else if(PSSM_RV_POS_CHANGE == stepState)
        {
            m_CurrentStepState = PSSM_RV_POS_CHANGE;
            if(IsRVRightPosition(NEXT_TUBE_POS))
            {
               m_SchedulerMachine->NotifyStepFinished();
            }
            else
            {
                if("Scheduler::RVReqMoveToRVPosition" == cmdName)
                {
                    if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
                    {
                        RaiseError(0, retCode, m_CurrentScenario, true);
                        m_SchedulerMachine->SendErrorSignal();
                    }
                    else
                    {
                        // Do nothing, just wait
                    }
                }
            }
        }
        else if(PSSM_STEP_PROGRAM_FINISH == stepState)
        {
            m_UsedStationIDs.append(m_CurProgramStepInfo.stationID);
            RaiseEvent(EVENT_SCHEDULER_PROGRAM_STEP_FINISHED,QStringList()<<QString("[%1]").arg(m_CurProgramStepIndex));
            (void)this->GetNextProgramStepInformation(m_CurProgramID, m_CurProgramStepInfo);
            if(m_CurProgramStepIndex != -1)
            {
                m_ProgramStatusInfor.SetStepID(m_CurProgramStepIndex); ///For Powerfailure:store current step id
                //start next step
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
                           <<m_CurReagnetName << DataManager::Helper::ConvertSecondsToTimeString(m_CurProgramStepInfo.durationInSeconds)
                           <<(m_CurProgramStepInfo.durationInSeconds > 0 ? QString("[%1]").arg(m_CurProgramStepInfo.durationInSeconds) : QString("Amb"))
                           <<PVMode);
                m_SchedulerMachine->NotifyStepProgramFinished();
                //send command to main controller to tell the left time
                quint32 leftSeconds = GetCurrentProgramStepNeededTime(m_CurProgramID);

//                QTime leftTime(0,0,0);
//                leftTime = leftTime.addSecs(leftSeconds);
                MsgClasses::CmdCurrentProgramStepInfor* commandPtr(new MsgClasses::CmdCurrentProgramStepInfor(5000, m_CurReagnetName, m_CurProgramStepIndex, leftSeconds));
                Q_ASSERT(commandPtr);
                Global::tRefType Ref = GetNewCommandRef();
                SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
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
        }
        else if(PSSM_CLEANING_DRY_STEP == stepState)
        {
            m_CurrentStepState = PSSM_CLEANING_DRY_STEP;
            DoCleaningDryStep(ctrlCmd, cmd);
        }
        else if(PSSM_PROGRAM_FINISH == stepState)
        {
            //program finished
            QString ProgramName = mp_DataManager->GetProgramList()->GetProgram(m_CurProgramID)->GetName();
            RaiseEvent(EVENT_SCHEDULER_PROGRAM_FINISHED,QStringList()<<ProgramName);
            m_SchedulerMachine->SendRunComplete();
            //m_SchedulerMachine->Stop();
            //todo: tell main controller that program is complete
            UpdateStationReagentStatus();
            if(m_IsCleaningProgram)
            {
                m_IsCleaningProgram = false;
                m_ProgramStatusInfor.SetProgramFinished();
            }
            //send command to main controller to tell the left time
            QTime leftTime(0,0,0);
            MsgClasses::CmdCurrentProgramStepInfor* commandPtr(new MsgClasses::CmdCurrentProgramStepInfor(5000, "", m_CurProgramStepIndex, 0));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

            //send command to main controller to tell program finished
            MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_FINISHED));
            Q_ASSERT(commandPtrFinish);
            Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtrFinish));
        }
        else if(PSSM_PAUSE == stepState)
        {
            m_CurrentStepState = PSSM_PAUSE;
            if(CTRL_CMD_START == ctrlCmd)
            {
                // resume the program
                emit NotifyResume();

                // tell the main controller the program is resuming
                MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_BEGIN));
                Q_ASSERT(commandPtrFinish);
                Global::tRefType fRef = GetNewCommandRef();
                SendCommand(fRef, Global::CommandShPtr_t(commandPtrFinish));
                m_TimeStamps.PauseStartTime = 0;
            }
            else if(CTRL_CMD_USER_RESPONSE_PAUSE_ALARM == ctrlCmd)
            {
                m_TimeStamps.PauseStartTime = QDateTime::currentMSecsSinceEpoch();
                m_Is5MinPause = false;
                m_Is10MinPause = false;
                m_Is15MinPause = false;
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
        else if(PSSM_ABORTING == stepState)
        {
             m_CurrentStepState = PSSM_ABORTING;
             m_SchedulerMachine->HandlePSSMAbortingWorkFlow(cmdName, retCode);
        }
        else if(PSSM_ABORTED == stepState)
        {
             m_CurrentStepState = PSSM_ABORTED;
            //program finished
            AllStop();

            m_SchedulerMachine->SendRunComplete();
            // tell the main controller the program has been aborted
            MsgClasses::CmdProgramAborted* commandPtrAbortFinish(new MsgClasses::CmdProgramAborted(5000, m_ProgramStatusInfor.IsRetortContaminted()));
            Q_ASSERT(commandPtrAbortFinish);
            Global::tRefType fRef = GetNewCommandRef();
            SendCommand(fRef, Global::CommandShPtr_t(commandPtrAbortFinish));

            //update station/reagent using times
            UpdateStationReagentStatus();

            m_CurProgramStepIndex = -1;
            LogDebug("Program aborted!");
        }
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

        // During HandleErrorState, we need also record the commands recevied from DeviceControl
        RecvCommand_t recvCommand;
        recvCommand.retCode = retCode;
        recvCommand.cmdName = cmdName;
        m_RecvCommandList.push_back(recvCommand);
    }

    if (CTRL_CMD_DRAIN_SR == ctrlCmd)
    {
        m_CmdDrainSR_Click = true;
    }

    if (SM_ERR_WAIT == currentState && CTRL_CMD_NONE != ctrlCmd)
    {
        if(CTRL_CMD_RC_RESTART == ctrlCmd)
        {
            LogDebug("Go to RC_Restart");
            m_SchedulerMachine->EnterRcRestart();
        }
        if (CTRL_CMD_RC_REPORT == ctrlCmd)
        {
            LogDebug("Go to RC_Report");
        }
        else if(CTRL_CMD_RS_RV_GET_ORIGINAL_POSITION_AGAIN == ctrlCmd)
        {
            LogDebug("Go to RS_RV_GetOriginalPositionAgain");
            m_SchedulerMachine->EnterRsRVGetOriginalPositionAgain();
        }
        else if(CTRL_CMD_RS_STANDBY == ctrlCmd)
        {
            LogDebug("Go to RS_STANDBY!");
            m_SchedulerMachine->EnterRsStandBy();
        }
        else if(CTRL_CMD_RS_STANDBY_WITHTISSUE == ctrlCmd)
        {
            LogDebug("Go to RS_STandby_withTissue");
            m_SchedulerMachine->EnterRsStandByWithTissue();
        }
        else if (CTRL_CMD_RS_HEATINGERR30SRETRY == ctrlCmd)
        {
            LogDebug("Go to RS_HeatingErr30Retry");
            m_SchedulerMachine->EnterRsHeatingErr30SRetry();
        }
        else if (CTRL_CMD_RS_PRESSUREOVERRANGE_3SRETRY == ctrlCmd)
        {
            LogDebug("Go to RS_PressureOverRange_3SRetry");
            m_SchedulerMachine->EnterRsPressureOverRange3SRetry();
        }
        else if(CTRL_CMD_RS_TSENSORERR3MINRETRY == ctrlCmd)
        {
            LogDebug(QString("Go to RS_TSensorErr_3MinRetry"));
            m_SchedulerMachine->EnterRsTSensorErr3MinRetry();
        }
        else if(CTRL_CMD_RC_LEVELSENSOR_HEATING_OVERTIME == ctrlCmd)
        {
            LogDebug("Go to RC_Levelsensor_Heating_Overtime");
            m_SchedulerMachine->EnterRcLevelsensorHeatingOvertime();
        }
        else if(CTRL_CMD_RC_PRESSURE == ctrlCmd)
        {
            LogDebug("Go to RC_Pressure");
            m_SchedulerMachine->EnterRcPressure();
        }
        else if(CTRL_CMD_RC_VACUUM == ctrlCmd)
        {
            LogDebug("Go to RC_Vacuum");
            m_SchedulerMachine->EnterRcVacuum();
        }
        else if(CTRL_CMD_RC_FILLING == ctrlCmd)
        {
            LogDebug("Go to RC_Filling");
            m_SchedulerMachine->EnterRcFilling();
        }
        else if(CTRL_CMD_RC_DRAINING == ctrlCmd)
        {
            LogDebug("Go to RC_Draining");
            m_SchedulerMachine->EnterRcDraining();
        }
        else if(CTRL_CMD_RS_DRAINATONCE == ctrlCmd)
        {
            LogDebug("Go to RS_DrainAtOnce");
            m_SchedulerMachine->EnterRsDrainAtOnce();
        }
        else if(CTRL_CMD_RC_BOTTLECHECK_I == ctrlCmd)
        {
            LogDebug("Go to RC_BottleCheck_I");
            m_SchedulerMachine->EnterRcBottleCheckI();
        }
        else if(CTRL_CMD_RS_FILLINGAFTERFLUSH == ctrlCmd)
        {
            LogDebug("Go to RS_FillingAfterFlush");
            m_SchedulerMachine->EnterRsFillingAfterFlush();
        }
        else if(CTRL_CMD_RS_CHECK_BLOCKAGE == ctrlCmd)
        {
            LogDebug("Go to RS_Check_Blockage");
            m_SchedulerMachine->EnterRsCheckBlockage();;
        }
        else if(CTRL_CMD_RS_PAUSE == ctrlCmd)
        {
            LogDebug("Go to RS_Pause");
            m_SchedulerMachine->EnterRsPause();
        }
        else if(CTRL_CMD_RS_RV_WAITINGTEMPUP == ctrlCmd)
        {
            LogDebug("Go to RS_RV_WaitingTempUp");
            m_SchedulerMachine->EnterRsRVWaitingTempUp();
        }
        else if (CTRL_CMD_RS_TISSUE_PROTECT == ctrlCmd)
        {
            LogDebug("Go to RS_Tissue_Protect");
            m_SchedulerMachine->EnterRsTissueProtect();
        }
        else if (CTRL_CMD_RC_CHECK_RTLOCK == ctrlCmd)
        {
            LogDebug("Go to RC_Check_RTLock");
            m_SchedulerMachine->EnterRcCheckRTLock();
        }
        else if (CTRL_CMD_RC_REHEATING == ctrlCmd)
        {
            LogDebug("Go to RcReHeating");
            m_SchedulerMachine->EnterRcReHeating(m_ProgramStatusInfor.GetScenario(), m_ProgramStatusInfor.GetLastReagentGroup(), false);
        }
        else if (CTRL_CMD_RC_REHEATING_CLEANING == ctrlCmd)
        {
            LogDebug("Go to RcReHeating_Clean");
            m_SchedulerMachine->EnterRcReHeating(m_ProgramStatusInfor.GetScenario(), m_ProgramStatusInfor.GetLastReagentGroup(), true);
        }
        else if (CTRL_CMD_RS_REAGENTCHECK == ctrlCmd)
        {
            LogDebug("Go to Rs_ReagentCheck");
            m_SchedulerMachine->EnterRsReagentCheck();
        }
        else if (CTRL_CMD_RS_RV_MOVETOSEALPOSITION == ctrlCmd)
        {
            LogDebug("Go to Rs_RV_MoveToPosition3.5");
            m_SchedulerMachine->EnterRsRVMoveToSealPosition();
        }
        else
        {
        }
    }
    else if (SM_ERR_RS_STANDBY == currentState)
    {
        LogDebug("In RS_STandBy state");
        m_SchedulerMachine->HandleRsStandByWorkFlow(cmdName, retCode);
    }
    else if (SM_ERR_RS_HEATINGERR30SRETRY == currentState)
    {
        LogDebug("In RS_HeatingErr_30SRetry state");
        m_SchedulerMachine->HandleRsHeatingErr30SRetry(cmdName, retCode);
    }
    else if (SM_ERR_RS_PRESSUREOVERRANGE3SRETRY == currentState)
    {
        LogDebug("In RS_PressureOverRange_3SRetry");
        m_SchedulerMachine->HandleRsPressureOverRange3SRetry(cmdName, retCode);
    }
    else if(SM_ERR_RS_TSENSORERR_3MINRETRY == currentState)
    {
        LogDebug("In Rs_TSensorErr_3MinRetry State");
        m_SchedulerMachine->HandleRsTSensorErr3MinRetry(cmdName, retCode);
    }
    else if (SM_ERR_RS_STANDBY_WITH_TISSUE == currentState)
    {
        LogDebug("In RS_STandBy_WithTissue state");
        m_SchedulerMachine->HandleRsStandByWithTissueWorkFlow(cmdName, retCode);
    }
    else if (SM_ERR_RC_LEVELSENSOR_HEATING_OVERTIME == currentState)
    {
        LogDebug("In RC_Levelsensor_Heating_Overtime State");
        m_SchedulerMachine->HandleRcLevelSensorHeatingOvertimeWorkFlow();

    }
    else if(SM_ERR_RS_RV_GETORIGINALPOSITIONAGAIN == currentState)
    {
        LogDebug("In RS_RV_GET_ORIGINAL_POSITION_AGAIN state");
        m_SchedulerMachine->HandleRsRVGetOriginalPositionAgainWorkFlow(cmdName, retCode);
    }
    else if(SM_ERR_RC_PRESSURE == currentState)
    {
        LogDebug("In RC_Pressure state");
         m_SchedulerMachine->HandleRcPressureWorkFlow(cmdName, retCode);
    }
    else if(SM_ERR_RC_VACUUM == currentState)
    {
        LogDebug("In RC_Vacuum state");
         m_SchedulerMachine->HandleRcVacuumWorkFlow(cmdName, retCode);
    }
    else if(SM_ERR_RC_FILLING == currentState)
    {
        LogDebug("In RC_Filling state");
         m_SchedulerMachine->HandleRcFillingWorkFlow(cmdName, retCode);
    }
    else if(SM_ERR_RC_DRAINING == currentState)
    {
        LogDebug("In RC_Draining state");
         m_SchedulerMachine->HandleDrainingWorkFlow(cmdName, retCode);
    }
    else if(SM_ERR_RS_DRAINATONCE == currentState)
    {
        LogDebug("In RS_DrainAtOnce state");
        m_SchedulerMachine->HandleDrainingWorkFlow(cmdName, retCode);
    }
    else if(SM_ERR_RS_BOTTLECHECK_I == currentState)
    {
        LogDebug("In RC_BottleCheck_I state");
        m_SchedulerMachine->HandleRcBottleCheckIWorkFlow(cmdName, retCode);
    }
    else if(SM_ERR_RS_FILLINGAFTERFFLUSH == currentState)
    {
        LogDebug("In RS_FillingAfterFlush State");
        m_SchedulerMachine->HandleRsFillingAfterFlushWorkFlow(cmdName, retCode);
    }
    else if(SM_ERR_RS_CHECK_BLOCKAGE == currentState)
    {
        LogDebug("In RS_CHECK_BLOCKAGE state");
        m_SchedulerMachine->HandleRsCheckBlockageWorkFlow(cmdName, retCode);
    }
    else if(SM_ERR_RS_PS_PAUSE == currentState)
    {
        LogDebug("In RS_Pause state");
        m_SchedulerMachine->HandleRsPauseWorkFlow(ctrlCmd);
    }
    else if(SM_ERR_RS_RV_WAITINGTEMPUP == currentState)
    {
        LogDebug("In RS_RV_WaitingTempUp");
        m_SchedulerMachine->HandleRsRVWaitingTempUpWorkFlow(cmdName, retCode);
    }
    else if (SM_ERR_RS_TISSUE_PROTECT == currentState)
    {
        LogDebug("In RS_Tissue_Protect");
        m_SchedulerMachine->HandleRsTissueProtectWorkFlow(cmdName, retCode);
    }
    else if (SM_ERR_RC_CHECK_RTLOCK == currentState)
    {
        LogDebug("In Rc_Check_RTLock");
        m_SchedulerMachine->HandleRcCheckRTLockWorkFlow();
    }
    else if (SM_ERR_RC_REHEATING == currentState)
    {
        m_SchedulerMachine->HandleRcReHeatingWorkFlow(cmdName, retCode);
    }
    else if(SM_ERR_RS_REAGENTCHECK == currentState)
    {
        LogDebug("In Rs_ReagentCheck");
        m_SchedulerMachine->HandleRsReagentWorkFlow(cmdName, retCode, true);
    }
    else if(SM_ERR_RS_RV_MOVETOPOSITIONSEAL == currentState)
    {
        LogDebug("In Rs_Rv_MoveToPosition3.5");
        m_SchedulerMachine->HandleRsReagentWorkFlow(cmdName, retCode, false);
    }
    else
    {
        LogDebug(QString("In ERROR state unknown currentState: %1").arg(currentState));
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

    MsgClasses::CmdProgramAction* pCmdProgramAction = dynamic_cast<MsgClasses::CmdProgramAction*>(pt.GetPointerToUserData());
    if(pCmdProgramAction)
    {
        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_START)
        {
            m_NewProgramID = pCmdProgramAction->GetProgramID();
            m_delayTime = pCmdProgramAction->DelayTime();
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
    }

    NetCommands::CmdSystemAction* pCmdSystemAction = dynamic_cast<NetCommands::CmdSystemAction*>(pt.GetPointerToUserData());
    if(pCmdSystemAction)
    {
        QString cmd = pCmdSystemAction->GetActionString();
        //m_EventKey = pCmdSystemAction->GetEventKey();

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
            m_EventKey = pCmdSystemAction->GetEventKey();
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
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RS_CHECKREMOTEALARMSTATUS;
        }
        if (cmd == "rs_checklocalalarmstatus")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            return CTRL_CMD_RS_CHECKLOCALALARMSTATUS;
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
    }
    return CTRL_CMD_UNKNOWN;

}//When end of this function, the command in m_SchedulerCmdQueue will be destrory by share pointer (CommandShPtr_t) mechanism

#if 0
void SchedulerMainThreadController::DequeueNonDeviceCommand()
{
        if(!m_SchedulerCmdQueue.isEmpty())
        {
            m_SchedulerCmdQueue.dequeue();
        }
}
#endif

bool SchedulerMainThreadController::GetNextProgramStepInformation(const QString& ProgramID,
                                                                  ProgramStepInfor& programStepInfor,
                                                                  bool onlyGetFirstProgramStepIndex)
{
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
        //cleaning program
        if((m_CurProgramStepIndex + 1) == count)
        {
            programStepInfor.nextStationID = programStepInfor.stationID;
        }
        else
        {
            programStepInfor.nextStationID = this->GetStationIDFromProgramStep(m_CurProgramStepIndex + 1);
        }
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

    m_ProgramStationList.clear();
    m_StationList.clear();

    ListOfIDs_t unusedStationIDs = pDashboardDataStationList->GetOrderedListOfDashboardStationIDs();
    QList<StationUseRecord_t> usedStations;

    ProgramStationInfo_t stationInfo;
    for (int i = beginStep; i < pProgram->GetNumberOfSteps(); i++)
    {
        const CProgramStep* pProgramStep = pProgram->GetProgramStep(i);//use order index

        isLastStep = (i == (pProgram->GetNumberOfSteps() - 1));
        QString reagentID = pProgramStep->GetReagentID();
        stationInfo.ReagentGroupID =GetReagentGroupID(reagentID);
        stationInfo.StationID = this->SelectStationFromReagentID(reagentID, unusedStationIDs, usedStations, isLastStep);
        m_ProgramStationList.enqueue(stationInfo);
        m_StationList.push_back(stationInfo.StationID);
    }

    // Add two cleaning bottles for bottle check
    stationInfo.ReagentGroupID = "RG7";
    stationInfo.StationID = "S12";
    m_ProgramStationList.enqueue(stationInfo);
    stationInfo.ReagentGroupID = "RG8";
    stationInfo.StationID = "S13";
    m_ProgramStationList.enqueue(stationInfo);
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
    QList<QString> fixationStationList, dehydratingAbsstationList;
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
                if (reagentGroup->GetGroupID() == "RG1")//Fixation
                {
                    fixationStationList.append(pDashboardStation->GetDashboardStationID());
                }

                if (reagentGroup->GetGroupID() == "RG3")//Dehydrating,diluted
                {
                    stationList.append(pDashboardStation->GetDashboardStationID());
                }

                if (reagentGroup->GetGroupID() == "RG4")//Dehydrating absolute
                {
                    dehydratingAbsstationList.append(pDashboardStation->GetDashboardStationID());
                }
                continue;
            }

        }
    }// end of for

    if ( (reagentGroupID == "RG2") || (reagentGroupID == "RG3") || (reagentGroupID == "RG4") )//RG2:Water,RG3:Dehydrating diluted,RG4:Dehydrating absolute
    {
        if (stationList.isEmpty())//no Dehydrating,diluted
        {
            if (!fixationStationList.isEmpty())
                stationList = fixationStationList;
            else
                stationList = dehydratingAbsstationList;
        }
    }

    return true;
}

void SchedulerMainThreadController::SendTissueProtectMsg()
{
    if (false == m_CmdDrainSR_Click)
    {
        MsgClasses::CmdProgramAcknowledge* CmdTissueProtectDone = new MsgClasses::CmdProgramAcknowledge(5000,DataManager::TISSUE_PROTECT_PASSED);
        Q_ASSERT(CmdTissueProtectDone);
        Global::tRefType fRef = GetNewCommandRef();
        SendCommand(fRef, Global::CommandShPtr_t(CmdTissueProtectDone));
    }
    else
    {
        m_CmdDrainSR_Click = false;
    }
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
        MsgClasses::CmdProgramAcknowledge* CmdRetortCoverOpen = new MsgClasses::CmdProgramAcknowledge(5000,DataManager::RETORT_COVER_OPERN);
        Q_ASSERT(CmdRetortCoverOpen);
        Global::tRefType fRef = GetNewCommandRef();
        SendCommand(fRef, Global::CommandShPtr_t(CmdRetortCoverOpen));
    }
}

/**
 * @brief Check which step has no safe reagent in a program.
 * @param ProgramID The the program Id, which to be checked.
 * @return the step index of the given program.
 */
int SchedulerMainThreadController::WhichStepHasNoSafeReagent(const QString& ProgramID)
{
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


quint32 SchedulerMainThreadController::GetLeftProgramStepsNeededTime(const QString& ProgramID, int SpecifiedStepIndex)
{
    quint32 leftTime = 0;
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
    int Index = SpecifiedStepIndex;
    if (-1 == Index)
    {
        int programStepIDIndex(-1);
        if (-1 == m_CurProgramStepIndex)
        {
            programStepIDIndex = 0;
        }
        else
        {
            programStepIDIndex = m_CurProgramStepIndex ;
        }
        Index = programStepIDIndex;
    }

    for (int i = Index; i < pProgram->GetNumberOfSteps(); i++)
    {
        const CProgramStep* pProgramStep = pProgram->GetProgramStep(i);//use order index
        quint32 soakTime = pProgramStep->GetDurationInSeconds();
        leftTime += soakTime;
        leftTime += 5 +  2 * 2; //suppose RV need 5 seconds to move to the target station, and movment between tube and its seal need 2 seconds.
        leftTime += 60; //suppose need 60 seconds to fill
        leftTime += 40; //suppose need 40 seconds to drain
        leftTime += 20; //suppose need 20 seconds to heat level sensor

        if ("RG6" == GetReagentGroupID(pProgramStep->GetReagentID()))
        {
            leftTime += 10 * 60;
        }
        if (0 == i)
        {
            leftTime += m_delayTime;
        }
    }
    if(pProgram && pProgram->IsCleaningProgram()) // if cleaning program, add time for dry step
    {
        leftTime += TIME_FOR_CLEANING_DRY_STEP;
    }
    return leftTime;
}

quint32 SchedulerMainThreadController::GetCurrentProgramStepNeededTime(const QString& ProgramID)
{
    quint32 leftTime = 0;
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
        leftTime += 5 +  2 * 2; //suppose RV need 5 seconds to move to the target station, and movment between tube and its seal need 2 seconds.
        leftTime += 60; //suppose need 60 seconds to fill
        leftTime += 40; //suppose need 40 seconds to drain
        leftTime += 20; //suppose need 20 seconds to heat level sensor
        if (m_CurProgramStepInfo.reagentGroup == "RG6")
        {
            leftTime += 10 * 60;
        }
        if (0 == programStepIDIndex)
        {
            leftTime += m_delayTime;
        }
    }
    return leftTime;
}


//client-->master
void SchedulerMainThreadController::OnProgramAction(Global::tRefType Ref,
                                                    const MsgClasses::CmdProgramAction &Cmd)
{
    m_Mutex.lock();
    m_SchedulerCmdQueue.enqueue(Global::CommandShPtr_t(new MsgClasses::CmdProgramAction(Cmd.GetTimeout(), Cmd.GetProgramID(), Cmd.ProgramActionType(),
                                                                                        Cmd.DelayTime())));
    m_Mutex.unlock();
    this->SendAcknowledgeOK(Ref);

    QString ProgramName;
    if(mp_DataManager && mp_DataManager->GetProgramList() && mp_DataManager->GetProgramList()->GetProgram(Cmd.GetProgramID()))
    {
        ProgramName = mp_DataManager->GetProgramList()->GetProgram(Cmd.GetProgramID())->GetName();
    }

    if (Cmd.ProgramActionType() != DataManager::PROGRAM_START)
    {
        if(Cmd.DelayTime() > 0) // start new program
        {
            QDateTime EndDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(Cmd.DelayTime());
            RaiseEvent(EVENT_SCHEDULER_REC_START_PROGRAM,QStringList()<<ProgramName
                       <<EndDateTime.toString()); //log
        }
        else
        {
            RaiseEvent(EVENT_SCHEDULER_REC_CONTINUE_PROGRAM,QStringList()<<ProgramName); //log
        }
        return;
    }
    else if(Cmd.ProgramActionType() != DataManager::PROGRAM_PAUSE)
    {
        RaiseEvent(EVENT_SCHEDULER_REC_PAUSE_PROGRAM,QStringList()<<ProgramName); //log
    }
    else if(Cmd.ProgramActionType() != DataManager::PROGRAM_ABORT)
    {
        RaiseEvent(EVENT_SCHEDULER_REC_ABORT_PROGRAM,QStringList()<<ProgramName); //log
    }
    else if(Cmd.ProgramActionType() != DataManager::PROGRAM_DRAIN)
    {
        RaiseEvent(EVENT_SCHEDULER_REC_DRAIN_PROGRAM,QStringList()<<ProgramName); //log
    }

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

        //int usedExhaustSystem = pUserSetting->GetUseExhaustSystem();
        /*to do...
         *int warningthreshold = 0;
        if (1 == usedExhaustSystem)
        {
            warningthreshold = 30;
        }
        else
        {
            warningthreshold = 45;
        }*/

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

//response or recovery
void SchedulerMainThreadController::OnActionCommandReceived(Global::tRefType Ref, const NetCommands::CmdSystemAction & Cmd)
{
    Q_UNUSED(Ref)
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
    //m_ProcessCassetteCount = Cmd.CassetteCount();
    m_ProcessCassetteCount += Cmd.CassetteCount();
    this->SendAcknowledgeOK(Ref);
}

void SchedulerMainThreadController::OnProgramSelected(Global::tRefType Ref, const MsgClasses::CmdProgramSelected & Cmd)
{
    m_hasParaffin = false;
    this->SendAcknowledgeOK(Ref);
    QString curProgramID = Cmd.GetProgramID();
    m_CurProgramStepIndex = -1;
    (void)this->GetNextProgramStepInformation(curProgramID, m_CurProgramStepInfo, true);//only to get m_CurProgramStepIndex

    unsigned int timeProposed = 0;
    unsigned int paraffinMeltCostedtime = 0;
    unsigned int costedTimeBeforeParaffin = 0;
    int whichStep = 0;
    if (m_CurProgramStepIndex != -1)
    {
        timeProposed = GetLeftProgramStepsNeededTime(curProgramID, m_CurProgramStepIndex);

        m_FirstProgramStepIndex = m_CurProgramStepIndex;
        (void)this->PrepareProgramStationList(curProgramID, m_CurProgramStepIndex);
        m_CurProgramStepIndex = -1;

        paraffinMeltCostedtime = this->GetOvenHeatingTime();
        if (-1 != Cmd.ParaffinStepIndex())//has Paraffin
        {
            m_hasParaffin = true;
            costedTimeBeforeParaffin = timeProposed - GetLeftProgramStepsNeededTime(curProgramID, Cmd.ParaffinStepIndex());
        }

        //cheack safe reagent
        whichStep = WhichStepHasNoSafeReagent(curProgramID);
    }

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
    m_ProgramStatusInfor.UpdateOvenHeatingTime(QDateTime::currentMSecsSinceEpoch(),true,true);
    mp_HeatingStrategy->ResetTheOvenHeating();
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
        scenario = 002;
        break;
    case SM_INIT_SELFTEST:
        scenario = 002;
        break;
    case SM_IDLE:
        scenario = 004;
        break;
    case SM_BUSY:
        break;
    case SM_ERROR:
        break;
    case PSSM_INIT:
        break;
    case PSSM_PRETEST:
        if(m_IsCleaningProgram)
        {
            scenario = 0;
        }
        else
        {
            scenario = 200;
        }
        break;
    case PSSM_PAUSE:
        if(ReagentGroup == "RG6")
        {
            scenario = 260;
            reagentRelated = false;
        }
        else
        {
            scenario = 214;
            reagentRelated = true;
        }
        break;
    case PSSM_ABORTING:
        scenario = 206;
        break;
    case PSSM_ABORTED:
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
    case PSSM_PROCESSING_SR:
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
    if(m_ProgramStatusInfor.GetScenario() != scenario && scenario > 2)
    {
        m_ProgramStatusInfor.SetScenario(scenario);
    }
    return scenario;
}


void SchedulerMainThreadController::OnDCLConfigurationFinished(ReturnCode_t RetCode)
{
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
        m_SchedulerMachine->SendErrorSignal();
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
    m_TickTimer.start();

}

void SchedulerMainThreadController::HardwareMonitor(const QString& StepID)
{
    QString ReagentGroup = m_CurProgramStepInfo.reagentGroup;
    quint32 Scenario = GetScenarioBySchedulerState(m_SchedulerMachine->GetCurrentState(),ReagentGroup);

	HardwareMonitor_t strctHWMonitor = m_SchedulerCommandProcessor->HardwareMonitor();
    LogDebug(strctHWMonitor.toLogString());
    m_ProgramStatusInfor.UpdateOvenHeatingTime(QDateTime::currentMSecsSinceEpoch(),strctHWMonitor.OvenHeatingStatus);

    if("ERROR" == StepID)
    {
        mp_HeatingStrategy->SetHeatingStrategyScenario(0);
    }
    // Run Heating Strategy
    if ("ERROR" != StepID && 0 != Scenario)
    {
        m_CurrentScenario = Scenario; //Scenario for Run or Idle state
        DeviceControl::ReturnCode_t retCode = mp_HeatingStrategy->RunHeatingStrategy(strctHWMonitor, Scenario);
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {      
            LogDebug(QString("Heating Strategy got an error at event %1 and scenario %2").arg(retCode).arg(Scenario));
            RaiseError(0, retCode, Scenario, true);
            m_SchedulerMachine->SendErrorSignal();
        }
    }

    // Monitor the sensors' current
    if ("ERROR" != StepID && 0 != Scenario)
    {
        this->CheckSlaveSensorCurrentOverRange(Scenario);
    }

    // Monitor local and remote alarm
    if ("ERROR" != StepID && 0 != Scenario)
    {
        if (QFile::exists("TEST_BEAN"))
        {
            QFile file("TEST_BEAN");
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream in(&file);

            while (!in.atEnd())
            {
                QString line = in.readLine().trimmed();
                QStringList list = line.split("=");
                if (list.size() != 2)
                {
                    continue;
                }
                QString leftStr = static_cast<QString>(list.at(0));
                leftStr = leftStr.trimmed();
                if ("ENABLE_ALARM" == leftStr)
                {
                    QString rightStr = static_cast<QString>(list.at(1));
                    rightStr = rightStr.trimmed();
                    if ("0" == rightStr)
                    {
                        m_CheckRemoteAlarmStatus = false;
                        m_CheckLocalAlarmStatus = false;
                    }
                    break;
                }
            }
            file.close();
        }

        if (1 == strctHWMonitor.RemoteAlarmStatus && m_CheckRemoteAlarmStatus)
        {
            RaiseError(0, DCL_ERR_DEV_MC_REMOTEALARM_UNCONNECTED, Scenario, true);
            m_CheckRemoteAlarmStatus = false;
        }
        if (1 == strctHWMonitor.LocalAlarmStatus && m_CheckLocalAlarmStatus)
        {
            RaiseError(0, DCL_ERR_DEV_MC_LOCALALARM_UNCONNECTED, Scenario, true);
            m_CheckLocalAlarmStatus = false;
        }
    }

    // Monitor Slave module's voltage and current
    if ("ERROR" != StepID && 0 != Scenario)
    {
        // For voltage related
        if (strctHWMonitor.Slave3Voltage < (m_SlaveAttrList[0].Voltagerated24VDC - m_SlaveAttrList[0].VoltageTolerance24VDC)*1000)
        {
            LogDebug(QString("slave 3 voltage is: %1").arg(strctHWMonitor.Slave3Voltage));
            RaiseError(0,DCL_ERR_DEV_MC_VOLTAGE_24V_ASB3_OUTOFRANGE,Scenario,true);
            m_SchedulerMachine->SendErrorSignal();
        }
        if (strctHWMonitor.Slave5Voltage < (m_SlaveAttrList[1].Voltagerated24VDC - m_SlaveAttrList[1].VoltageTolerance24VDC)*1000)
        {
            LogDebug(QString("slave 5 voltage is: %1").arg(strctHWMonitor.Slave5Voltage));
            RaiseError(0,DCL_ERR_DEV_MC_VOLTAGE_24V_ASB5_OUTOFRANGE,Scenario,true);
            m_SchedulerMachine->SendErrorSignal();
        }
        if (strctHWMonitor.Slave15Voltage < (m_SlaveAttrList[2].Voltagerated24VDC - m_SlaveAttrList[2].VoltageTolerance24VDC)*1000)
        {
            LogDebug(QString("slave 15 voltage is: %1").arg(strctHWMonitor.Slave15Voltage));
            RaiseError(0,DCL_ERR_DEV_MC_VOLTAGE_24V_ASB15_OUTOFRANGE,Scenario,true);
            m_SchedulerMachine->SendErrorSignal();
        }

        // For current related
        if (strctHWMonitor.Slave3Current > m_SlaveAttrList[0].CurrentMax5VDC)
        {
            LogDebug(QString("slave 3 5V current is: %1").arg(strctHWMonitor.Slave3Current));
            RaiseError(0,DCL_ERR_DEV_MC_DC_5V_ASB3_OUTOFRANGE,Scenario,true);
            m_SchedulerMachine->SendErrorSignal();
        }
        if (strctHWMonitor.Slave5Current > m_SlaveAttrList[1].CurrentMax5VDC)
        {
            LogDebug(QString("slave 5 5V current is: %1").arg(strctHWMonitor.Slave5Current));
            RaiseError(0,DCL_ERR_DEV_MC_DC_5V_ASB5_OUTOFRANGE,Scenario,true);
            m_SchedulerMachine->SendErrorSignal();
        }

    }

    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.PressureAL))
	{
        m_PressureAL = strctHWMonitor.PressureAL;
        if ("ERROR" != StepID && "IDLE" != StepID && 0 != Scenario)
        {
            if(qAbs(m_PressureAL) >40.0 )
            {
                LogDebug(QString("The pressure in the error case is: %1").arg(m_PressureAL));
                RaiseError(0,DCL_ERR_DEV_LA_PRESSURESENSOR_OUTOFRANGE,Scenario,true);
                m_SchedulerMachine->SendErrorSignal();
            }
        }
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
        if (200 == Scenario)
        {
#if 0
            if (m_TempRV2 < 40)
            {
                LogDebug(QString("The RV(2) temperature is: %1, in scenario:%2").arg(m_TempRV2).arg(Scenario));
                RaiseError(0, DCL_ERR_DEV_RV_HEATING_TSENSOR2_LESSTHAN_30DEGREEC_OVERTIME, Scenario, true);
                m_SchedulerMachine->SendErrorSignal();
            }
#endif
        }
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
    if(strctHWMonitor.OvenLidStatus != UNDEFINED_VALUE)
    {
        if ( (m_OvenLidStatus == 0) && (strctHWMonitor.OvenLidStatus == 1) )
        {
            //oven is open
            if ( (Scenario >= 2 && Scenario <= 205) || (Scenario >= 211 && Scenario <= 257)
                 || (Scenario >= 281 && Scenario <= 297) )
            {
                LogDebug(QString("The oven lock is opened, EventID:%1, Scenario:%2").arg(DCL_ERR_DEV_WAXBATH_OVENCOVER_STATUS_OPEN).arg(Scenario));
                //RaiseError(0, DCL_ERR_DEV_WAXBATH_OVENCOVER_STATUS_OPEN, Scenario, true);
            }
            if(Scenario >= 271 && Scenario <= 277)
            {
                if ("ERROR" != StepID && "IDLE" != StepID)
                {
                    ReleasePressure();
                    RaiseError(0, DCL_ERR_DEV_WAXBATH_OVENCOVER_STATUS_OPEN, Scenario, true);
                    m_SchedulerMachine->SendErrorSignal();
                }
            }
            MsgClasses::CmdLockStatus* commandPtr(new MsgClasses::CmdLockStatus(5000, DataManager::PARAFFIN_BATH_LOCK, false));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

            if ("ERROR" != StepID)
            {
                MsgClasses::CmdProgramAcknowledge* CmdOvenCoverOpen = new MsgClasses::CmdProgramAcknowledge(5000,DataManager::OVEN_COVER_OPEN);
                Q_ASSERT(CmdOvenCoverOpen);
                Global::tRefType fRef = GetNewCommandRef();
                SendCommand(fRef, Global::CommandShPtr_t(CmdOvenCoverOpen));
            }
        }
        if ( (m_OvenLidStatus == 1) && (strctHWMonitor.OvenLidStatus == 0) )
        {
            //oven is closed
            MsgClasses::CmdLockStatus* commandPtr(new MsgClasses::CmdLockStatus(5000, DataManager::PARAFFIN_BATH_LOCK, true));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        }
        m_OvenLidStatus = strctHWMonitor.OvenLidStatus;
    }
    if(strctHWMonitor.RetortLockStatus != UNDEFINED_VALUE)
	{
        if(((m_RetortLockStatus == 0) ||(m_RetortLockStatus == UNDEFINED_VALUE))&&(strctHWMonitor.RetortLockStatus == 1))
        {
            if ("ERROR" != StepID && "IDLE" != StepID && m_CurrentStepState != PSSM_PAUSE)
            {
                // retort is open, turn on the fan
                m_SchedulerCommandProcessor->pushCmd(new CmdALTurnOnFan(500, this));
                SchedulerCommandShPtr_t pResHeatingCmd;
                PopDeviceControlCmdQueue(pResHeatingCmd,"Scheduler::ALTurnOnFan");
                ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;
                (void)pResHeatingCmd->GetResult(retCode);
                if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
                {
                    RaiseError(0, retCode, Scenario, true);
                    m_SchedulerMachine->SendErrorSignal();
                }
                SchedulerStateMachine_t currentState = m_SchedulerMachine->GetCurrentState();
                if((currentState & 0xF) == SM_BUSY && currentState != PSSM_PAUSE)
                {
                    RaiseError(0, DCL_ERR_DEV_LIDLOCK_CLOSE_STATUS_ERROR, Scenario, true);
                    m_SchedulerMachine->SendErrorSignal();
                }

                // Notify retort lid is opened
                MsgClasses::CmdProgramAcknowledge* CmdRetortCoverOpen = new MsgClasses::CmdProgramAcknowledge(5000,DataManager::RETORT_COVER_OPERN);
                Q_ASSERT(CmdRetortCoverOpen);
                Global::tRefType fRef = GetNewCommandRef();
                SendCommand(fRef, Global::CommandShPtr_t(CmdRetortCoverOpen));
            }

            MsgClasses::CmdLockStatus* commandPtr(new MsgClasses::CmdLockStatus(5000, DataManager::RETORT_LOCK, false));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));


        }
        if(((m_RetortLockStatus == 1) || (m_RetortLockStatus == UNDEFINED_VALUE))&&(strctHWMonitor.RetortLockStatus == 0))
		{
            if ("ERROR" != StepID && "IDLE" != StepID && m_CurrentStepState != PSSM_PAUSE)
            {
                // retort is closed, turn off the fan
                m_SchedulerCommandProcessor->pushCmd(new CmdALTurnOffFan(500, this));
                SchedulerCommandShPtr_t pResHeatingCmd;
                PopDeviceControlCmdQueue(pResHeatingCmd,"Scheduler::ALTurnOffFan");
                ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;
                (void)pResHeatingCmd->GetResult(retCode);
                if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
                {
                    RaiseError(0, retCode, Scenario, true);
                    m_SchedulerMachine->SendErrorSignal();
                }
            }

            MsgClasses::CmdLockStatus* commandPtr(new MsgClasses::CmdLockStatus(5000, DataManager::RETORT_LOCK, true));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
		}
        m_RetortLockStatus = strctHWMonitor.RetortLockStatus;
	}

    //Check No-Signal error for Retort sensors
    if (false == this->CheckRetortTempSensorNoSignal(Scenario, strctHWMonitor.TempRTBottom1))
    {
        RaiseError(0, DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_NOSIGNAL, Scenario, true);
        m_SchedulerMachine->SendErrorSignal();
    }
    if (false == this->CheckRetortTempSensorNoSignal(Scenario, strctHWMonitor.TempRTBottom2))
    {
        LogDebug("The RT bottom2 no signal");
        RaiseError(0, DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_NOSIGNAL, Scenario, true);
        m_SchedulerMachine->SendErrorSignal();
    }
    if (false == this->CheckRetortTempSensorNoSignal(Scenario, strctHWMonitor.TempRTSide))
    {
        RaiseError(0, DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_NOSIGNAL, Scenario, true);
        m_SchedulerMachine->SendErrorSignal();
    }
    //the Level Scenario no signal is the same with over range
    if (false == this->CheckLevelSensorNoSignal(Scenario, strctHWMonitor.TempALLevelSensor))
    {
        RaiseError(0, DCL_ERR_DEV_LEVELSENSOR_TEMPERATURE_OVERRANGE, Scenario, true);
        m_SchedulerMachine->SendErrorSignal();
    }
    m_PositionRV = strctHWMonitor.PositionRV;
}

void SchedulerMainThreadController::DataManager(DataManager::CDataManager *p_DataManager)
{
    mp_DataManager = p_DataManager;
}

bool SchedulerMainThreadController::PopDeviceControlCmdQueue(Scheduler::SchedulerCommandShPtr_t& PtrCmd)
{
    bool ret = false;
    m_MutexDeviceControlCmdQueue.lock();
    if(!m_DeviceControlCmdQueue.isEmpty())
    {
        PtrCmd = m_DeviceControlCmdQueue.dequeue();
        ret = true;
    }
    m_MutexDeviceControlCmdQueue.unlock();
    return ret;
}

void SchedulerMainThreadController::PopDeviceControlCmdQueue(Scheduler::SchedulerCommandShPtr_t& PtrCmd, const QString& CmdName)
{
    QQueue<Scheduler::SchedulerCommandShPtr_t>::iterator iter = m_DeviceControlCmdQueue.begin();
    m_MutexDeviceControlCmdQueue.lock();
    while (true)
    {
        if (m_DeviceControlCmdQueue.isEmpty())
        {
            (void)m_WaitCondition.wait(&m_MutexDeviceControlCmdQueue);
        }
        for (iter=m_DeviceControlCmdQueue.begin(); iter!=m_DeviceControlCmdQueue.end(); ++iter)
        {
            if ((*iter)->GetName() == CmdName)
            {
                break;
            }
        }
        if (iter == m_DeviceControlCmdQueue.end())
        {
            (void)m_WaitCondition.wait(&m_MutexDeviceControlCmdQueue);
        }
        else
        {
            break;
        }
    }

    // Get the command
    PtrCmd = *iter;
    (void)m_DeviceControlCmdQueue.removeOne(*iter);
    m_MutexDeviceControlCmdQueue.unlock();
}


void SchedulerMainThreadController::PushDeviceControlCmdQueue(Scheduler::SchedulerCommandShPtr_t CmdPtr)
{
    m_MutexDeviceControlCmdQueue.lock();
    m_DeviceControlCmdQueue.push_back(CmdPtr);
    m_WaitCondition.wakeAll();
    m_MutexDeviceControlCmdQueue.unlock();
}

bool SchedulerMainThreadController::BottleCheck(quint32 bottleSeq)
{
    /*lint -e574 */
    if (bottleSeq >= (quint32)m_ProgramStationList.size())
    {
        return false;
    }

    ProgramStationInfo_t stationInfo = m_ProgramStationList.at(bottleSeq);
    RVPosition_t tubePos = GetRVTubePositionByStationID(stationInfo.StationID);
    QString reagentGrpId = stationInfo.ReagentGroupID;

    m_CurrentBottlePosition.ReagentGrpId = reagentGrpId;
    m_CurrentBottlePosition.RvPos = tubePos;

    LogDebug(QString("Bottle check for tube %1").arg(tubePos));
    CmdIDBottleCheck* cmd  = new CmdIDBottleCheck(500, this);
    cmd->SetReagentGrpID(reagentGrpId);
    cmd->SetTubePos(tubePos);
    m_SchedulerCommandProcessor->pushCmd(cmd);
    return true;
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
}

void SchedulerMainThreadController::MoveRVToInit()
{
    LogDebug("Send cmd to DCL to let RV move to init position. ");
    m_SchedulerCommandProcessor->pushCmd(new CmdRVReqMoveToInitialPosition(500, this));
}

void SchedulerMainThreadController::ShutdownRetortHeater()
{
    //todo: add code to shutdown retort heaters
    LogDebug("Shut down all retort heaters!");
}

void SchedulerMainThreadController::ReleasePressure()
{
    RaiseEvent(EVENT_SCHEDULER_RELEASE_PREASURE);
    m_SchedulerCommandProcessor->pushCmd(new CmdALReleasePressure(500, this));
}

void SchedulerMainThreadController::OnEnterPssmProcessing()
{
    // We only release pressure if neither P or V exists.
    RaiseEvent(EVENT_SCHEDULER_START_PROCESSING);
    if (!m_CurProgramStepInfo.isPressure && !m_CurProgramStepInfo.isVacuum)
    {
        RaiseEvent(EVENT_SCHEDULER_RELEASE_PREASURE);
        m_SchedulerCommandProcessor->pushCmd(new CmdALReleasePressure(500,  this));
    }
    m_TimeStamps.ProposeSoakStartTime = QDateTime::currentDateTime().addSecs(m_delayTime).toMSecsSinceEpoch();
    m_TimeStamps.CurStepSoakStartTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

    m_lastPVTime = 0;
    m_completionNotifierSent = false;
    m_IsReleasePressureOfSoakFinish = false;

    if ((0 == m_CurProgramStepIndex) && (m_delayTime > 0))
    {
        m_IsInSoakDelay = true;
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
    typedef enum
    {
        CDS_READY,
        CDS_MOVE_TO_SEALING_13,
        CDS_WAIT_HIT_POSITION,
        CDS_WAIT_HIT_TEMPERATURE,
        CDS_VACUUM,
        CDS_WAIT_HIT_PPRESSURE,
        CDS_WAITING_DRY,
        CDS_STOP_HEATING_VACUUM,
        CDS_SUCCESS,
        CDS_ERROR
    }DryStepsStateMachine;
    ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;
    static DryStepsStateMachine CurrentState = CDS_READY;
    static quint64 StepStartTime = 0;
    static bool warningReport = false;
    CmdRVReqMoveToRVPosition* CmdMvRV = NULL;
    MsgClasses::CmdCurrentProgramStepInfor* commandPtr = NULL;
    Global::tRefType Ref;
    switch (CurrentState)
    {
    case CDS_READY:
        RaiseEvent(EVENT_SCHEDULER_START_DRY_PROCESSING);
        commandPtr = new MsgClasses::CmdCurrentProgramStepInfor(5000, Global::UITranslator::TranslatorInstance().Translate(STR_SCHEDULER_DRY_PROCESSING),
                                                                m_CurProgramStepIndex, TIME_FOR_CLEANING_DRY_STEP);
        Q_ASSERT(commandPtr);
        Ref = GetNewCommandRef();
        SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        CurrentState = CDS_MOVE_TO_SEALING_13;
    case CDS_MOVE_TO_SEALING_13:
        qDebug() << "CDS_MOVE_TO_SEALING_13";
        CmdMvRV = new CmdRVReqMoveToRVPosition(500, this);
        CmdMvRV->SetRVPosition(DeviceControl::RV_SEAL_13);
        m_SchedulerCommandProcessor->pushCmd(CmdMvRV);
        CurrentState = CDS_WAIT_HIT_POSITION;
        break;
    case CDS_WAIT_HIT_POSITION:
        qDebug() << "CDS_WAIT_HIT_POSITION";
        if(cmd != NULL && ("Scheduler::RVReqMoveToRVPosition" == cmd->GetName()))
        {
            (void)cmd->GetResult(retCode);
            if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                m_ProgramStatusInfor.SetLastRVPosition(DeviceControl::RV_SEAL_13);
                CurrentState = CDS_WAIT_HIT_TEMPERATURE;
            }
            else
            {
                RaiseError(0, retCode, m_CurrentScenario, true);
                CurrentState = CDS_READY;
                m_SchedulerMachine->SendErrorSignal();
                StepStartTime = 0;
            }
        }
        break;
    case CDS_WAIT_HIT_TEMPERATURE:
        qDebug() << "CDS_WAIT_HIT_TEMPERATURE";
        CurrentState = CDS_VACUUM;
        break;
    case CDS_VACUUM:
        qDebug() << "CDS_VACUUM";
        Vaccum();
        CurrentState = CDS_WAIT_HIT_PPRESSURE;
        break;
    case CDS_WAIT_HIT_PPRESSURE:
        qDebug() << "CDS_WAIT_HIT_PPRESSURE";
        if(cmd != NULL && ("Scheduler::ALVaccum" == cmd->GetName()))
        {
            (void)cmd->GetResult(retCode);
            if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
            {
                RaiseError(0, retCode, m_CurrentScenario, true);
                CurrentState = CDS_READY;
                m_SchedulerMachine->SendErrorSignal();
                StepStartTime = 0;;
            }
            else
            {
                CurrentState = CDS_WAITING_DRY;
                StepStartTime = QDateTime::currentMSecsSinceEpoch();
            }
        }
        break;
    case CDS_WAITING_DRY:
        qDebug() << "CDS_WAITING_DRY";
        if(QDateTime::currentMSecsSinceEpoch() - StepStartTime >= 600000) // drying 10 minutes
        {
            CurrentState = CDS_STOP_HEATING_VACUUM;
        }

        //Check if pressure reaches negative 25kpa, if no, report out warning message
        if (QDateTime::currentMSecsSinceEpoch() - StepStartTime >= 120*1000 && warningReport == false)
        {
            warningReport = true;
            if (qAbs(m_PressureAL) < 25.0)
            {
                RaiseError(0, DCL_ERR_DEV_LA_SEALING_FAILED_VACUUM, m_CurrentScenario, true);
            }
        }
        break;
    case CDS_STOP_HEATING_VACUUM:
        qDebug() << "CDS_STOP_HEATING_VACUUM";
        ReleasePressure();
        CurrentState = CDS_SUCCESS;
        break;
    case CDS_SUCCESS:
        RaiseEvent(EVENT_SCHEDULER_FINISHED_DRY_PROCESSING);
        m_SchedulerMachine->NotifyProgramFinished();
        m_ProgramStatusInfor.SetLastReagentGroup("");// set Retort to clean
        CurrentState = CDS_READY;
        StepStartTime = 0;
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
        m_ProgramStatusInfor.SetLastReagentGroup(m_CurProgramStepInfo.reagentGroup); /// only store ReagentGroup for normal program
    }
    QString ReagentGroup = m_CurProgramStepInfo.reagentGroup;
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
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(5000,m_CurProgramStepInfo.stationID , true, true, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
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
    LogDebug(QString("The temp1:%1, the temp2:%2, The temp3:%3").arg(temp1).arg(temp2).arg(temp3));
    //Check No-Signal error
    if (qAbs(temp1 - 299) <= 0.000000000001 || qAbs(temp2 - 299) <= 0.000000000001 || qAbs(temp3 - 299) <= 0.000000000001)
    {
        return false;
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

    // acknowledge to gui
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(5000, StationID, true, true, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
}

void SchedulerMainThreadController::StopFillRsTissueProtect(const QString& StationID)
{
    // acknowledge to gui
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(5000,StationID, false, true, false));
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
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(5000,m_CurProgramStepInfo.stationID , false, true, false));
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
    m_SchedulerCommandProcessor->pushCmd(cmd);

    LogDebug("Notice GUI Draining started");
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(5000,m_CurProgramStepInfo.stationID , true, false, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
}

void SchedulerMainThreadController::Drain()
{
    RaiseEvent(EVENT_SCHEDULER_DRAINING);
    CmdALDraining* cmd  = new CmdALDraining(500, this);
    //todo: get delay time here
    cmd->SetDelayTime(5000);
    m_SchedulerCommandProcessor->pushCmd(cmd);

    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(5000,m_CurProgramStepInfo.stationID , true, false, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
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
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(5000,m_CurProgramStepInfo.stationID , true, false, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
}

void SchedulerMainThreadController::OnBeginDrain()
{
    // acknowledge to gui
    LogDebug("Notice GUI Draining started");
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(5000,m_CurProgramStepInfo.stationID , true, false, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
}

void SchedulerMainThreadController::OnStopDrain()
{
    // acknowledge to gui
    LogDebug("Notice GUI Draining stopped");
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(5000,m_CurProgramStepInfo.stationID , false, false, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
}

void SchedulerMainThreadController::Pressure()
{
    RaiseEvent(EVENT_SCHEDULER_SET_PRESSURE,QStringList()<<QString("[%1]").arg(AL_TARGET_PRESSURE_POSITIVE));
    m_SchedulerCommandProcessor->pushCmd(new CmdALPressure(500, this));
}

void SchedulerMainThreadController::HighPressure()
{

    RaiseEvent(EVENT_SCHEDULER_SET_PRESSURE,QStringList()<<QString("[%1]").arg(40));
    CmdALPressure* cmd = new CmdALPressure(500, this);
    cmd->SetTargetPressure(40.0);
    m_SchedulerCommandProcessor->pushCmd(cmd);
}

void SchedulerMainThreadController::Vaccum()
{

    RaiseEvent(EVENT_SCHEDULER_SET_PRESSURE,QStringList()<<QString("[%1]").arg(AL_TARGET_PRESSURE_NEGATIVE));
    m_SchedulerCommandProcessor->pushCmd(new CmdALVaccum(500, this));
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
    //update the remaining time for the current step
    m_CurProgramStepInfo.durationInSeconds = m_CurProgramStepInfo.durationInSeconds - ((QDateTime::currentDateTime().toMSecsSinceEpoch() - m_TimeStamps.CurStepSoakStartTime) / 1000);
    m_TimeStamps.PauseStartTime = QDateTime::currentMSecsSinceEpoch();

    //send command to main controller to tell program is actually pasued
    MsgClasses::CmdProgramAcknowledge* commandPtrPauseFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_PAUSE_FINISHED));
    Q_ASSERT(commandPtrPauseFinish);
    Global::tRefType fRef = GetNewCommandRef();
    SendCommand(fRef, Global::CommandShPtr_t(commandPtrPauseFinish));
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

    if (mp_DataManager != NULL && ! mp_DataManager->GetUserSettings())
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
    quint32 ParaffinMeltPoint = 64;
    if (mp_DataManager != NULL && ! mp_DataManager->GetUserSettings())
    {
        ParaffinMeltPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
    }
    quint32 StillNeedHeatingTime = m_ProgramStatusInfor.GetRemaingTimeForMeltingParffin(ParaffinMeltPoint);
    quint32 HeatedTime = 0;
    QString TotalTime = "12h";
    if(12 * 60 * 60 >= StillNeedHeatingTime)
    {
        HeatedTime = 12 * 60 * 60 - StillNeedHeatingTime;
    }
    if(ParaffinMeltPoint > 64 && (15 * 60 * 60 >= StillNeedHeatingTime))
    {
        TotalTime = "15h";
        HeatedTime = 15 * 60 * 60 - StillNeedHeatingTime;
    }
    RaiseEvent(EVENT_SCHEDULER_OVEN_HEATING_TIME,QStringList()<<DataManager::Helper::ConvertSecondsToTimeString(StillNeedHeatingTime)<<TotalTime);
    if(QFile::exists("TEST_ISSAC")) // linear agr only for testing
    {
        return m_ProgramStatusInfor.GetOvenHeatingTime(ParaffinMeltPoint);
    }
    return HeatedTime;
}

quint64 SchedulerMainThreadController::GetOvenHeatingRemainingTime()
{
    quint32 ParaffinMeltPoint = 64;
    if (mp_DataManager != NULL && mp_DataManager->GetUserSettings() != NULL)
    {
        ParaffinMeltPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
    }
    return m_ProgramStatusInfor.GetRemaingTimeForMeltingParffin(ParaffinMeltPoint);
}

qint64 SchedulerMainThreadController::GetPreTestTime()
{
    quint32 BottleCount = m_ProgramStationList.size();
    qint64 preTesttTime = 0;
    if( (BottleCount - 2) > 0)
    {
        preTesttTime = (4 + 1 * (BottleCount - 2)) * 60;
    }
    else
    {
        preTesttTime = 4 * 60;
    }
    return preTesttTime;
}

bool SchedulerMainThreadController::IsLastStep(int currentStepIndex, const QString& currentProgramID)
{
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
    else if (DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_ABNORMAL == m_CurErrEventID
             || DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR1_OUTOFRANGE == m_CurErrEventID
             || DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_NOTREACHTARGETTEMP == m_CurErrEventID)
    {
        return LATUBE1;
    }
    else if (DCL_ERR_DEV_LA_TUBEHEATING_TUBE2_ABNORMAL == m_CurErrEventID
             || DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR2_OUTOFRANGE == m_CurErrEventID
             || DCL_ERR_DEV_LA_TUBEHEATING_TUBE2_NOTREACHTARGETTEMP == m_CurErrEventID)
    {
        return LATUBE2;
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
    if (200 == Scenario || 205 == Scenario || (Scenario >=211 && Scenario <=217)
            || (Scenario >= 252 && Scenario <= 257) || 260 == Scenario || (Scenario >= 217 && Scenario <=277)
            || (Scenario >= 222 && Scenario <= 227) || (Scenario >= 231 && Scenario <=237)
            || (Scenario >= 241 && Scenario <=247) || 251 == Scenario || 002 == Scenario
            || (Scenario >= 281 && Scenario <= 297) )
    {
        //if (qFuzzyCompare((qAbs(HWTemp-299+1)), 0.0+1))
        if (qAbs(HWTemp-299) < 0.000000000001)
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
        if (qAbs(HWTemp-299) < 0.000000000001)
        {
            return false;
        }
    }
    return true;
}

void SchedulerMainThreadController::CheckSlaveSensorCurrentOverRange(quint32 Scenario)
{
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
        RaiseError(0,DCL_ERR_DEV_ASB5_AC_CURRENT_OUTOFRANGE, Scenario, true);
        m_SchedulerMachine->SendErrorSignal();
    }
    if (reportError2.instanceID != 0)
    {
        RaiseError(0,DCL_ERR_DEV_ASB5_AC_CURRENT_OUTOFRANGE, Scenario, true);
        m_SchedulerMachine->SendErrorSignal();
    }
    if (reportError3.instanceID != 0 )
    {
        RaiseError(0,DCL_ERR_DEV_ASB5_AC_CURRENT_OUTOFRANGE, Scenario, true);
        m_SchedulerMachine->SendErrorSignal();
    }
    if (reportError4.instanceID != 0)
    {
        RaiseError(0,DCL_ERR_DEV_ASB5_AC_CURRENT_OUTOFRANGE, Scenario, true);
        m_SchedulerMachine->SendErrorSignal();
    }
    if (reportError5.instanceID != 0)
    {
        //RaiseError(0,DCL_ERR_DEV_WAXBATH_BOTTOM_HEATINGPAD_CURRENT_OUTOFRANGE, Scenario, true);
        //m_SchedulerMachine->SendErrorSignal();
    }
    if (reportError6.instanceID != 0)
    {
        //RaiseError(0,DCL_ERR_DEV_WAXBATH_BOTTOM_HEATINGPAD_CURRENT_OUTOFRANGE, Scenario, true);
       // m_SchedulerMachine->SendErrorSignal();
    }
    if (reportError7.instanceID != 0)
    {
        //RaiseError(0,DCL_ERR_DEV_WAXBATH_BOTTOM_HEATINGPAD_CURRENT_OUTOFRANGE, Scenario, true);
       // m_SchedulerMachine->SendErrorSignal();
    }
    if (reportError8.instanceID != 0)
    {
        LogDebug(QString("In fan error state, Current is: %1").arg(reportError8.errorData));
        RaiseError(0,DCL_ERR_DEV_LA_STATUS_EXHAUSTFAN, Scenario, true);
        m_SchedulerMachine->SendErrorSignal();
    }

    if (reportError9.instanceID != 0)
    {
        RaiseError(0,DCL_ERR_DEV_RV_HEATING_CURRENT_OUTOFRANGE, Scenario, true);
        m_SchedulerMachine->SendErrorSignal();
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
    ProgramAcknownedgeType_t type =  DataManager::PROGRAM_SYSTEM_EEEOR;
    MsgClasses::CmdProgramAcknowledge* commandPtrSystemError(new MsgClasses::CmdProgramAcknowledge(5000, type));
    Q_ASSERT(commandPtrSystemError);
    Global::tRefType fRef = GetNewCommandRef();
    SendCommand(fRef, Global::CommandShPtr_t(commandPtrSystemError));

    m_TimeStamps.SystemErrorStartTime = QDateTime::currentMSecsSinceEpoch();
}

void SchedulerMainThreadController::OnEnterRcRestart()
{
    ProgramAcknownedgeType_t type =  DataManager::PROGRAM_SYSTEM_RC_RESTART;
    MsgClasses::CmdProgramAcknowledge* commandPtrRcRestart(new MsgClasses::CmdProgramAcknowledge(5000, type));
    Q_ASSERT(commandPtrRcRestart);
    Global::tRefType fRef = GetNewCommandRef();
    SendCommand(fRef, Global::CommandShPtr_t(commandPtrRcRestart));

    if ((PSSM_PROCESSING != m_CurrentStepState) && (PSSM_PROCESSING_SR != m_CurrentStepState) && (-1 != m_CurProgramStepIndex))
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
    RaiseEvent(EVENT_SCHEDULER_WAITING_FOR_FILLING_PARAFFIN);
}

void SchedulerMainThreadController::OnPreTestDone()
{
    ProgramAcknownedgeType_t type =  DataManager::PROGRAM_PRETEST_DONE;
    MsgClasses::CmdProgramAcknowledge* commandPreTest(new MsgClasses::CmdProgramAcknowledge(5000, type));
    Q_ASSERT(commandPreTest);
    Global::tRefType fRef = GetNewCommandRef();
    SendCommand(fRef, Global::CommandShPtr_t(commandPreTest));
}

}
