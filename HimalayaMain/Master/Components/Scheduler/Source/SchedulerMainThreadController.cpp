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
#include "Scheduler/Include/HimalayaDeviceEventCodes.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToInitialPosition.h"
#include "Scheduler/Commands/Include/CmdALStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdALFilling.h"
#include "Scheduler/Commands/Include/CmdALDraining.h"
#include "Scheduler/Commands/Include/CmdALPressure.h"
#include "Scheduler/Commands/Include/CmdALVaccum.h"
#include "Scheduler/Commands/Include/CmdALAllStop.h"
#include "Scheduler/Commands/Include/CmdRVStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdRTStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdOvenStartTemperatureControlWithPID.h"
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
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "HimalayaDataManager/Include/DataManager.h"
#include "HimalayaDataContainer/Containers/ProgramSettings/Include/ProgramSettings.h"
#include "Global/Include/GlobalDefines.h"
#include <Scheduler/Commands/Include/CmdRTLock.h>
#include <Scheduler/Commands/Include/CmdRTUnlock.h>
#include <HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdCurrentProgramStepInfor.h>
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAcknowledge.h"
#include "HimalayaDataContainer/Containers/ReagentStations/Commands/Include/CmdUpdateStationReagentStatus.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdKeepCassetteCount.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelected.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramSelectedReply.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdStationSuckDrain.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAcknowledge.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdLockStatus.h"
#include "HimalayaDataContainer/Containers/UserSettings/Commands/Include/CmdQuitAppShutdown.h"
#include "NetCommands/Include/CmdSystemAction.h"
#include "float.h"
#include "Global/Include/EventObject.h"
#include "Scheduler/Include/HeatingStrategy.h"


using namespace DataManager;

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//                                                                                 Project Specific
namespace Scheduler {

SchedulerMainThreadController::SchedulerMainThreadController(
        Global::gSourceType TheHeartBeatSource)
        : Threads::ThreadController(TheHeartBeatSource, "SchedulerMainThread")
        , m_TickTimer(this)
        , m_SchedulerCommandProcessorThread(new QThread())
        , m_SchedulerCommandProcessor(NULL)
        , m_SchedulerMachine(new CSchedulerStateMachine())
        , mp_DataManager(NULL)
        , m_CurProgramStepIndex(-1)
        , m_FirstProgramStepIndex(0)
        , m_CurReagnetName("")
        , m_CurProgramID("")
        , m_NewProgramID("")
        , m_PauseToBeProcessed(false)
        , m_ProcessCassetteCount(0)
        , m_OvenLidStatus(UNDEFINED_VALUE)
        , m_RetortLockStatus(UNDEFINED_VALUE)
        , mp_HeatingStrategy(NULL)
{
    memset(&m_TimeStamps, 0, sizeof(m_TimeStamps));
}

SchedulerMainThreadController::~SchedulerMainThreadController()
{
    delete m_SchedulerMachine;
    m_SchedulerMachine = NULL;
}

void SchedulerMainThreadController::RegisterCommands()
{
    // register commands
//    RegisterCommandForProcessing<HimalayaErrorHandler::CmdRaiseAlarm,
//                    SchedulerMainThreadController>(&SchedulerMainThreadController::OnRaiseAlarmLocalRemote, this);

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

}

void SchedulerMainThreadController::CreateAndInitializeObjects()
{

    //create the SchedulerCommandProcessor thread
    //m_SchedulerCommandProcessorThread = new QThread();
    ////m_SchedulerCommandProcessor = new SchedulerCommandProcessor(this, mp_IDeviceProcessing);
    //CONNECTSIGNALSLOT(&m_SchedulerCommandProcessor, timeout(), m_SchedulerCommandProcessor, run());

    m_SchedulerCommandProcessor->moveToThread(m_SchedulerCommandProcessorThread);
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessorThread, started(), m_SchedulerCommandProcessor, run());
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessor, destroyed(), m_SchedulerCommandProcessorThread, quit());
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessorThread, finished(), m_SchedulerCommandProcessorThread, deleteLater());
    m_SchedulerCommandProcessorThread->start();

    //timer setting
    CONNECTSIGNALSLOT(&m_TickTimer, timeout(), this, OnTickTimer());
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessor, DCLConfigurationFinished(ReturnCode_t),
                      this,OnDCLConfigurationFinished(ReturnCode_t))

    m_TickTimer.setInterval(500);

    CONNECTSIGNALSLOT(m_SchedulerMachine, sigOnRVPositionCheck(), this, MoveRVToInit());
    CONNECTSIGNALSLOT(m_SchedulerMachine, sigOnSealingCheck(), this, SealingCheck());
    CONNECTSIGNALSLOT(m_SchedulerMachine, sigOnInit(), this, StepStart());
    CONNECTSIGNALSLOT(m_SchedulerMachine, sigOnHeatLevelSensorTempS1(), this, HeatLevelSensor());
    CONNECTSIGNALSLOT(m_SchedulerMachine, sigOnHeatLevelSensorTempS2(), this, HeatLevelSensor());
    CONNECTSIGNALSLOT(m_SchedulerMachine, sigOnMoveToTubeBefore(), this, MoveRV());
    CONNECTSIGNALSLOT(m_SchedulerMachine, sigOnMoveToTubeAfter(), this, MoveRV());
    CONNECTSIGNALSLOT(m_SchedulerMachine, sigOnMoveToSeal(), this, MoveRV());
    CONNECTSIGNALSLOT(m_SchedulerMachine, sigOnFill(), this, Fill());
    CONNECTSIGNALSLOT(m_SchedulerMachine, sigOnSoak(), this, Soak());
    CONNECTSIGNALSLOT(m_SchedulerMachine, sigOnDrain(), this, Drain());
    CONNECTSIGNALSLOT(m_SchedulerMachine, sigOnAborting(), this, Abort());

    CONNECTSIGNALSLOT(m_SchedulerMachine, sigOnStopFill(), this, StopFill());
    CONNECTSIGNALSLOT(m_SchedulerMachine, sigOnStopDrain(), this, StopDrain());
    CONNECTSIGNALSLOT(m_SchedulerMachine, sigOnPause(), this, Pause());
    CONNECTSIGNALSLOT(m_SchedulerMachine, sigOnPauseDrain(), this, Pause());
    CONNECTSIGNALSLOT(m_SchedulerMachine, sigOnRsRvMoveToInitPosition(), this, MoveRVToInit());
    CONNECTSIGNALSLOT(m_SchedulerMachine, sigOnRCReport(), this, ShutdownRetortHeater());

    CONNECTSIGNALSLOT(m_SchedulerMachine, sigOnRsReleasePressure(), this, ReleasePressure());
    CONNECTSIGNALSLOT(m_SchedulerMachine, sigOnRsShutdownFailedHeater(), this, ShutdownFailedHeater());

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
    m_SchedulerCommandProcessorThread->wait();
}

void SchedulerMainThreadController::OnPowerFail(const Global::PowerFailStages PowerFailStage)
{
    Q_UNUSED(PowerFailStage)
}

void SchedulerMainThreadController::OnTickTimer()
{
    ControlCommandType_t newControllerCmd = PeekNonDeviceCommand();
    SchedulerCommandShPtr_t cmd;
    PopDeviceControlCmdQueue(cmd);

    SchedulerStateMachine_t currentState = m_SchedulerMachine->GetCurrentState();
    m_SchedulerMachine->UpdateCurrentState(currentState);
    LogDebug(QString("%1 Scheduler state: %2").arg(QDateTime::currentDateTime().toString()).arg(currentState,0,16));
    switch(currentState & 0xFF)
    {
    case SM_INIT:
        //refuse any main controller request if there is any
        //qDebug()<<"DBG"<<"Scheduler main controller state: INIT";
        break;
    case SM_IDLE:
        //qDebug()<<"DBG"<<"Scheduler main controller state: IDLE";
        HardwareMonitor( "IDLE" );
        HandleIdleState(newControllerCmd);
        break;
    case SM_BUSY:
        //qDebug()<<"DBG"<<"Scheduler main controller state: RUN";
        HardwareMonitor( m_CurProgramID );
        HandleRunState(newControllerCmd, cmd);
        break;
    case SM_ERROR:
        HardwareMonitor( "ERROR" );
        //qDebug()<<"DBG"<<"Scheduler main controller state: ERROR";
        //refuse any main controller request if there is any
        HandleErrorState(newControllerCmd, cmd, currentState);
        break;
    default:
        LogDebug(QString("Scheduler main controller gets unexpected state: %1").arg(currentState));
    }
}

void SchedulerMainThreadController::HandleIdleState(ControlCommandType_t ctrlCmd)
{
    switch (ctrlCmd)
    {
    case CTRL_CMD_START:
        m_UsedStationIDs.clear();

        //Check if it is a Cleaning Program or not?
        if (m_NewProgramID.at(0) == 'C')
        {
            int sep = m_NewProgramID.indexOf('_');
            m_CurProgramID = m_NewProgramID.left(sep);
            m_ReagentIdOfLastStep = m_NewProgramID.right(m_NewProgramID.count()- sep -1);
        }
        else
            m_CurProgramID = m_NewProgramID;

        m_NewProgramID = "";
        m_CurProgramStepIndex = -1;
        this->GetNextProgramStepInformation(m_CurProgramID, m_CurProgramStepInfo);
        if(m_CurProgramStepIndex != -1)
        {
            //send command to main controller to tell program is starting
            MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_BEGIN));
            Q_ASSERT(commandPtrFinish);
            Global::tRefType fRef = GetNewCommandRef();
            SendCommand(fRef, Global::CommandShPtr_t(commandPtrFinish));
            QString ProgramName = mp_DataManager->GetProgramList()->GetProgram(m_CurProgramID)->GetName();
            //LOG_STR_ARG(STR_START_PROGRAM, Global::FmtArgs()<<ProgramName);

            LogDebug(QString("Start Step: %1").arg(m_CurProgramID));
            m_SchedulerMachine->SendRunSignal();
            //send command to main controller to tell the left time
            quint32 leftSeconds = GetCurrentProgramStepNeededTime(m_CurProgramID);
            //LOG_STR_ARG(STR_CURRENT_PROGRAM_NAME_STEP_REAGENT_LEFTTIME,Global::FmtArgs()<< ProgramName << m_CurProgramStepIndex +1 << m_CurReagnetName << leftSeconds);
            QTime leftTime(0,0,0);
            leftTime = leftTime.addSecs(leftSeconds);
            MsgClasses::CmdCurrentProgramStepInfor* commandPtr(new MsgClasses::CmdCurrentProgramStepInfor(5000, m_CurReagnetName, m_CurProgramStepIndex, leftSeconds));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
            DequeueNonDeviceCommand();
        }
        break;
    default:
          DequeueNonDeviceCommand();
    }
}

void SchedulerMainThreadController::UpdateStationReagentStatus()
{
    MsgClasses::CmdUpdateStationReagentStatus* commandPtr = NULL;
    DataManager::CHimalayaUserSettings* pUserSetting = mp_DataManager->GetUserSettings();
    if (m_CurProgramID.at(0) != 'C')//process cleaning reagent
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
    ReturnCode_t retCode;
    QString cmdName = "";
    QString ReagentGroup = m_CurProgramStepInfo.reagentGroup;
    quint32 Scenario = GetScenarioBySchedulerState(m_SchedulerMachine->GetCurrentState(),ReagentGroup);
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
        LogDebug(QString("Scheduler received command: ABORT"));
        m_SchedulerMachine->NotifyAbort();
        DequeueNonDeviceCommand();
        //tell main controller scheduler is starting to abort
        MsgClasses::CmdProgramAcknowledge* commandPtrAbortBegin(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_ABORT_BEGIN));
        Q_ASSERT(commandPtrAbortBegin);
        Global::tRefType fRef = GetNewCommandRef();
        SendCommand(fRef, Global::CommandShPtr_t(commandPtrAbortBegin));
    }
    else
    {
        SchedulerStateMachine_t stepState = m_SchedulerMachine->GetCurrentState();
        //qDebug()<<"DBG" << "Scheduler step statemachine: "<<stepState;
        if(PSSM_ST_INIT == stepState)
        {
            LogDebug("Precheck Init done");
            m_SchedulerMachine->NotifyStInitOK(); //todo: update later
            if(CTRL_CMD_PAUSE == ctrlCmd)
            {
                m_SchedulerMachine->NotifyPause(PSSM_INIT);
                DequeueNonDeviceCommand();
            }
        }
        else if(PSSM_ST_TEMP_CHECKING == stepState)
        {
            LogDebug("Precheck Temp Check OK");
            m_SchedulerMachine->NotifyStTempOK(); //todo: update later
            if(CTRL_CMD_PAUSE == ctrlCmd)
            {
                m_SchedulerMachine->NotifyPause(PSSM_INIT);
                DequeueNonDeviceCommand();
            }
        }
        else if(PSSM_ST_CURRENT_CHECKING == stepState)
        {
            LogDebug("Precheck Current Check OK");
            m_SchedulerMachine->NotifyStCurrentOK(); //todo: update later
            if(CTRL_CMD_PAUSE == ctrlCmd)
            {
                m_SchedulerMachine->NotifyPause(PSSM_INIT);
                DequeueNonDeviceCommand();
            }
        }
        else if(PSSM_ST_VOLTAGE_CHECKING == stepState)
        {
            LogDebug("Precheck Vlotage Check OK");
            m_SchedulerMachine->NotifyStVoltageOK(); //todo: update later
            if(CTRL_CMD_PAUSE == ctrlCmd)
            {
                m_SchedulerMachine->NotifyPause(PSSM_INIT);
                DequeueNonDeviceCommand();
            }
        }
        else if(PSSM_ST_RV_POSITION_CHECKING == stepState)
        {

            if((cmdName == "Scheduler::RVReqMoveToInitialPosition"))
            {
                if(( DCL_ERR_FCT_CALL_SUCCESS == retCode))
                {
                    LogDebug("Precheck Position Check OK");
                    m_SchedulerMachine->NotifyStRVPositionOK(); //todo: update later
                }
                else if(DCL_ERR_DEV_RV_MOTOR_INTERNALSTEPS_EXCEEDUPPERLIMIT == retCode)
                {
                    LogDebug("Precheck Position Check exceed upper limit");
                    Global::EventObject::Instance().RaiseEvent(0, 500030021, Scenario, false);
                    m_SchedulerMachine->SendErrorSignal();
                }
                else if(DCL_ERR_DEV_RV_MOTOR_INTERNALSTEPS_RETRY == retCode)
                {
                    LogDebug("Precheck Position Check internal step retry");
                    Global::EventObject::Instance().RaiseEvent(0, 500030011, Scenario, false);
                    m_SchedulerMachine->SendErrorSignal();
                }
                else
                {
                    LogDebug("Precheck Position Check Failed");
                    Global::EventObject::Instance().RaiseEvent(0, 500030001, Scenario, false);
                    m_SchedulerMachine->SendErrorSignal();
                }
            }
            if(CTRL_CMD_PAUSE == ctrlCmd)
            {
                m_SchedulerMachine->NotifyPause(PSSM_INIT);
                DequeueNonDeviceCommand();
            }
        }
        else if(PSSM_ST_PRESSURE_CHECKING == stepState)
        {
            HardwareMonitor_t strctHWMonitor = m_SchedulerCommandProcessor->HardwareMonitor();
            qreal currentPressure = strctHWMonitor.PressureAL;
            Q_UNUSED(currentPressure);
           // if((currentPressure < 0.0000001)&&(currentPressure > (-0.00000001))) //todo: verify pressure tolerence later
           // {
           //     Global::EventObject::Instance().RaiseEvent(0, 513040020, GetScenarioBySchedulerState(stepState, GetReagentGroupID(m_CurReagnetName)), true);
           //     m_SchedulerMachine->SendErrorSignal();
           // }
           // else
            {
                LogDebug("Precheck Pressure Check OK");
                m_SchedulerMachine->NotifyStPressureOK(); //todo: update later
            }

            if(CTRL_CMD_PAUSE == ctrlCmd)
            {
                AllStop();
                m_SchedulerMachine->NotifyPause(PSSM_INIT);
                DequeueNonDeviceCommand();
            }
        }
        else if(PSSM_ST_SEALING_CHECKING == stepState)
        {
            if(( DCL_ERR_FCT_CALL_SUCCESS == retCode)&&(cmdName == "Scheduler::IDSealingCheck"))
            {
                LogDebug("Precheck Sealing Check OK");
                ProgramStationInfo_t stationInfo = m_ProgramStationList.dequeue();
                RVPosition_t tubePos = GetRVTubePositionByStationID(stationInfo.StationID);
                QString reagentGrpId = stationInfo.ReagentGroupID;

                CmdIDBottleCheck* cmd  = new CmdIDBottleCheck(500, this);
                //todo: get delay time here
                cmd->SetReagentGrpID(reagentGrpId);
                cmd->SetTubePos(tubePos);
                m_SchedulerCommandProcessor->pushCmd(cmd);
                //LOG_STR_ARG(STR_PROGRAM_SELFTEST_CHECK_BOTTLE, Global::FmtArgs()<<stationInfo.StationID);
                m_SchedulerMachine->NotifyStSealingOK(); //todo: update later
            }
            else if(( DCL_ERR_FCT_CALL_SUCCESS != retCode)&&(cmdName == "Scheduler::IDSealingCheck"))
            {
                //Sealing check failed, raise event here
                LogDebug("Precheck Sealing Check Fail");
                //todo: raise event here
                m_SchedulerMachine->SendErrorSignal();

            }
            if(CTRL_CMD_PAUSE == ctrlCmd)
            {
                AllStop();
                m_SchedulerMachine->NotifyPause(PSSM_INIT);
                DequeueNonDeviceCommand();
            }
        }
        else if(PSSM_ST_STATION_CHECKING == stepState)
        {
            if(DCL_ERR_UNDEFINED != retCode)
            {
                quint32 resid = STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_UNEXPECTED;
                Q_UNUSED(resid);
                if(( DCL_ERR_FCT_CALL_SUCCESS == retCode)&&(cmdName == "Scheduler::IDBottleCheck"))
                {
                    LogDebug("Precheck Station Check OK");
                    m_SchedulerMachine->NotifyStGetStationcheckResult(); //todo: update later
                    resid = STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_OK;
                }
                else if( DCL_ERR_DEV_LA_BOTTLECHECK_FAILED_INSUFFICIENT == retCode)
                {
                    LogDebug("Precheck Station Check Insufficient");
                    m_SchedulerMachine->NotifyStGetStationcheckResult(); //todo: update later
                    resid = STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_NOT_FULL;
                }
                else if( DCL_ERR_DEV_LA_BOTTLECHECK_FAILED_BLOCKAGE == retCode)
                {
                    LogDebug("Precheck Station Check Blockage");
                    m_SchedulerMachine->NotifyStGetStationcheckResult(); //todo: update later
                    resid = STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_BLOCKAGE;
                }
                else if(DCL_ERR_DEV_LA_BOTTLECHECK_FAILED_EMPTY == retCode)
                {
                    LogDebug("Precheck Station Check Empty or Leakage");
                    m_SchedulerMachine->NotifyStGetStationcheckResult(); //todo: update later
                    resid = STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_EMPTY;
                }
                else if(DCL_ERR_DEV_LA_BOTTLECHECK_PRESSUREBUILD_FAILED == retCode)
                {
                    LogDebug("Precheck Station Check Failed");
                    m_SchedulerMachine->NotifyStGetStationcheckResult(); //todo: update later
                    resid = STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_TIMEOUT;
                }
                else if(DCL_ERR_UNDEFINED != retCode)
                {
                    LogDebug(QString("Unexpected ret code: %1").arg(retCode));
                    resid = STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_UNEXPECTED;
                }
                //LOG_STR_ARG(STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT, Global::tTranslatableStringList()<<Global::TranslatableString(resid));
            }
        }
        else if(PSSM_ST_STATION_CHECK_FINISH == stepState)
        {
            if(m_ProgramStationList.size() > 0)
            {
                ProgramStationInfo_t stationInfo = m_ProgramStationList.dequeue();
                LogDebug(QString("Precheck Station Check: %1").arg(stationInfo.StationID));
                RVPosition_t tubePos = GetRVTubePositionByStationID(stationInfo.StationID);
                QString reagentGrpId = stationInfo.ReagentGroupID;

                CmdIDBottleCheck* cmd  = new CmdIDBottleCheck(500, this);
                cmd->SetReagentGrpID(reagentGrpId);
                cmd->SetTubePos(tubePos);
                m_SchedulerCommandProcessor->pushCmd(cmd);
                m_SchedulerMachine->NotifyStStationLeft(); //todo: update later
            }
            else
            {
                m_SchedulerMachine->NotifyStStationOK(); //todo: update later
                LogDebug("Precheck Station Check All OK");
                //LOG_STR(STR_PROGRAM_SELFTEST_FINISH);
            }
            if(CTRL_CMD_PAUSE == ctrlCmd)
            {
                m_SchedulerMachine->NotifyPause(PSSM_INIT);
                DequeueNonDeviceCommand();
            }
        }
        else if(PSSM_ST_DONE == stepState)
        {
            LogDebug("Precheck DONE");
            m_SchedulerMachine->NotifyStDone(); //todo: update later
            if(CTRL_CMD_PAUSE == ctrlCmd)
            {
                m_SchedulerMachine->NotifyPause(PSSM_INIT);
                DequeueNonDeviceCommand();
            }
        }
        else if(PSSM_INIT == stepState)
        {
            if(CTRL_CMD_PAUSE == ctrlCmd)
            {
                AllStop();
                m_SchedulerMachine->NotifyPause(PSSM_INIT);
                DequeueNonDeviceCommand();
            }
            else
            {
                if(CheckStepTemperature()) //todo: verify later
                {
                    LogDebug("Program Step Init OK");
                    m_SchedulerMachine->NotifyTempsReady();
                }
            }
        }
        else if(PSSM_READY_TO_HEAT_LEVEL_SENSOR_S1 == stepState)
        {
            //todo: get data here
            if(CTRL_CMD_PAUSE == ctrlCmd)
            {
                m_SchedulerMachine->NotifyPause(PSSM_READY_TO_HEAT_LEVEL_SENSOR_S1);
                DequeueNonDeviceCommand();
            }
            else if(CheckLevelSensorTemperature(85))
            {
                LogDebug("Program Step Heating Level sensor stage 1 OK");
                m_SchedulerMachine->NotifyLevelSensorTempS1Ready();
            }
            else
            {
                //check heating time
                qint64 lsHeatStartTime = GetFunctionModuleStartworkTime(&m_FunctionModuleStatusList, CANObjectKeyLUT::FCTMOD_AL_LEVELSENSORTEMPCTRL);
                bool ok;
                CProgramSettings *pProgramSetting = NULL;
                mp_DataManager->GetProgramSettings(pProgramSetting);
                if(pProgramSetting)
                {
                    qint64 overtime = pProgramSetting->GetParameterValue("Retort", "level sensor", "HeatingOvertime", ok);
                    if((lsHeatStartTime != 0)&&(ok))
                    {
                        if(QDateTime::currentMSecsSinceEpoch() > (lsHeatStartTime + overtime*1000))
                        {
                            LogDebug("Program Step Heating Level sensor stage 1 timeout");
                            //level sensor heating overtime
                            Global::EventObject::Instance().RaiseEvent(0, 513015312, GetScenarioBySchedulerState(stepState, GetReagentGroupID(m_CurReagnetName)), true);
                            m_SchedulerMachine->SendErrorSignal();
                        }
                    }
                }
            }
        }
        else if(PSSM_READY_TO_HEAT_LEVEL_SENSOR_S2 == stepState)
        {
            //todo: get data here
            if(CTRL_CMD_PAUSE == ctrlCmd)
            {
                m_SchedulerMachine->NotifyPause(PSSM_READY_TO_HEAT_LEVEL_SENSOR_S2);
                DequeueNonDeviceCommand();
            }
            else if(CheckLevelSensorTemperature(85))
            {
                LogDebug("Program Step Heating Level sensor stage 2 OK");
                m_SchedulerMachine->NotifyLevelSensorTempS2Ready();
            }
            else
            {
                //check heating time
                qint64 lsHeatStartTime = GetFunctionModuleStartworkTime(&m_FunctionModuleStatusList, CANObjectKeyLUT::FCTMOD_AL_LEVELSENSORTEMPCTRL);
                bool ok;
                CProgramSettings *pProgramSetting = NULL;
                mp_DataManager->GetProgramSettings(pProgramSetting);
                if(pProgramSetting)
                {
                    qint64 overtime = pProgramSetting->GetParameterValue("Retort", "level sensor", "HeatingOvertime", ok);
                    if((lsHeatStartTime != 0)&&(ok))
                    {
                        if(QDateTime::currentMSecsSinceEpoch() > (lsHeatStartTime + overtime*1000))
                        {
                            //level sensor heating overtime
                            LogDebug("Program Step Heating Level sensor stage 2 timeout");
                            Global::EventObject::Instance().RaiseEvent(0, 513015312, GetScenarioBySchedulerState(stepState, GetReagentGroupID(m_CurReagnetName)), true);
                            m_SchedulerMachine->SendErrorSignal();
                        }
                    }
                }
            }
        }
        else if(PSSM_READY_TO_TUBE_BEFORE == stepState)
        {
            // get current step tube position here
            RVPosition_t targetPos = GetRVTubePositionByStationID(m_CurProgramStepInfo.stationID);
            if(m_PositionRV == targetPos)
            {
                if((CTRL_CMD_PAUSE == ctrlCmd)||(m_PauseToBeProcessed))
                {
                    m_SchedulerMachine->NotifyPause(PSSM_READY_TO_TUBE_BEFORE);
                    m_PauseToBeProcessed = false;
                    DequeueNonDeviceCommand();
                }
                else
                {
                    LogDebug(QString("Program Step Hit Tube(before) %1").arg(targetPos));
                    m_SchedulerMachine->NotifyHitTubeBefore();
                }
            }
            else
            {
                if("Scheduler::RVReqMoveToRVPosition" == cmdName)
                {
                    if(DCL_ERR_DEV_RV_MOTOR_INTERNALSTEPS_RETRY == retCode)
                    {
                        //fail to move to seal, raise event here
                        LogDebug(QString("Program Step Move to tube(before)%1 internal steps retry").arg(targetPos));
                        Global::EventObject::Instance().RaiseEvent(0, 500030011, Scenario, true);
                        m_SchedulerMachine->SendErrorSignal();
                    }
                    else if(DCL_ERR_DEV_RV_MOTOR_INTERNALSTEPS_EXCEEDUPPERLIMIT == retCode)
                    {
                        LogDebug(QString("Program Step Move to tube(before)%1 exceed upper limit").arg(targetPos));
                        Global::EventObject::Instance().RaiseEvent(0, 500030021, Scenario, true);
                        m_SchedulerMachine->SendErrorSignal();
                    }
                }

                if(CTRL_CMD_PAUSE == ctrlCmd)
                {
                    m_PauseToBeProcessed = true;
                }
            }
        }
        else if(PSSM_READY_TO_FILL == stepState)
        {
            if(CTRL_CMD_PAUSE == ctrlCmd)
            {
                LogDebug(QString("Scheduler step: READY_TO_FILL is abort to PAUSE"));
                AllStop();
                m_SchedulerMachine->NotifyPause(PSSM_READY_TO_FILL);
                DequeueNonDeviceCommand();
            }
            else if( "Scheduler::ALFilling" == cmdName)
            {
                if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    LogDebug(QString("Program Step Filling OK"));
                    m_SchedulerMachine->NotifyFillFinished();
                }
                else if( DCL_ERR_DEV_LA_FILLING_INSUFFICIENT == retCode)
                {
                    LogDebug(QString("Program Step Filling Insufficient"));
                    Global::EventObject::Instance().RaiseEvent(0, 500040161, Scenario, true);
                    m_SchedulerMachine->SendErrorSignal();
                }
            }
            else if(retCode != DCL_ERR_UNDEFINED)
            {
                //todo: error handling here
            }
        }
        else if(PSSM_READY_TO_SEAL == stepState)
        {
            RVPosition_t targetPos = GetRVSealPositionByStationID(m_CurProgramStepInfo.stationID);
            if(m_PositionRV == targetPos)
            {
                LogDebug(QString("Program Step Hit Seal %1").arg(targetPos));
                if((CTRL_CMD_PAUSE == ctrlCmd)||(m_PauseToBeProcessed))
                {
                    m_SchedulerMachine->NotifyPause(PSSM_READY_TO_SEAL);
                    m_PauseToBeProcessed = false;
                    DequeueNonDeviceCommand();
                }
                else
                {
                    m_SchedulerMachine->NotifyHitSeal();
                }
            }
            else
            {
                if("Scheduler::RVReqMoveToRVPosition" == cmdName)
                {
                    if(DCL_ERR_DEV_RV_MOTOR_INTERNALSTEPS_EXCEEDUPPERLIMIT == retCode)
                    {
                        //fail to move to seal, raise event here
                        LogDebug(QString("Program Step Move to Seal %1 Exceed upper limit").arg(targetPos));
                    }
                    else if(DCL_ERR_DEV_RV_MOTOR_INTERNALSTEPS_RETRY == retCode)
                    {
                        //fail to move to seal, raise event here
                        LogDebug(QString("Program Step Move to Seal %1 internal step retry").arg(targetPos));
                        Global::EventObject::Instance().RaiseEvent(0, 500030011, Scenario, true);
                        m_SchedulerMachine->SendErrorSignal();
                    }
                }
                if(CTRL_CMD_PAUSE == ctrlCmd)
                {
                    m_PauseToBeProcessed = true;
                }
            }
        }
        else if(PSSM_SOAK == stepState)
        {
            static qint64 lastPVTime = 0;
            static bool completionNotifierSent = false;
            if(CTRL_CMD_PAUSE == ctrlCmd)
            {
                if(m_CurProgramStepInfo.isPressure || m_CurProgramStepInfo.isVacuum)
                {
                    AllStop();
                    lastPVTime = 0;
                }
                m_SchedulerMachine->NotifyPause(PSSM_SOAK);
                DequeueNonDeviceCommand();
            }
            else
            {
                qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();
                //todo: 1/10 the time
                qint32 period = m_CurProgramStepInfo.durationInSeconds * 1000;
                //qint32 period = m_CurProgramStepInfo.durationInSeconds * 20;
                if((now - m_TimeStamps.CurStepSoakStartTime ) > (period))
                {
                    //if it is Cleaning program, need not notify user
                    if((m_CurProgramID.at(0) != 'C') && IsLastStep(m_CurProgramStepIndex, m_CurProgramID))
                    {
                         //this is last step, need to notice user
                         if(!completionNotifierSent)
                         {
                             MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_WILL_COMPLETE));
                             Q_ASSERT(commandPtrFinish);
                             Global::tRefType fRef = GetNewCommandRef();
                             SendCommand(fRef, Global::CommandShPtr_t(commandPtrFinish));
                             completionNotifierSent = true;
                         }
                         if(CTRL_CMD_DRAIN == ctrlCmd)
                         {
                             LogDebug(QString("Program Step Process finished"));
                             m_SchedulerMachine->NotifySoakFinished();
                             m_TimeStamps.CurStepSoakStartTime = 0;
                             completionNotifierSent = false;
                         }
                    }
                    else
                    {
                        LogDebug(QString("Program Step Process finished"));
                        m_SchedulerMachine->NotifySoakFinished();
                        m_TimeStamps.CurStepSoakStartTime = 0;
                    }
                }
                else
                {
                    if(m_CurProgramStepInfo.isPressure && m_CurProgramStepInfo.isVacuum)
                    {
                        // P/V take turns in 1 minute
                        if((now - lastPVTime)>60000)
                        {
                            if(((now - m_TimeStamps.CurStepSoakStartTime)/60000)%2 == 0)
                            {
                                Pressure();
                            }
                            else
                            {
                                Vaccum();
                            }
                            lastPVTime = now;
                        }
                    }
                }
            }
        }
        else if(PSSM_READY_TO_TUBE_AFTER == stepState)
        {

            // get current step tube position here
            RVPosition_t targetPos = GetRVTubePositionByStationID(m_CurProgramStepInfo.stationID);
            if(m_PositionRV == targetPos)
            {
                LogDebug(QString("Program Step Hit Tube(after) %1").arg(targetPos));
                if((CTRL_CMD_PAUSE == ctrlCmd)||(m_PauseToBeProcessed))
                {
                    RVPosition_t sealPos = GetRVSealPositionByStationID(m_CurProgramStepInfo.stationID);
                    if(RV_UNDEF != sealPos)
                    {
                        CmdRVReqMoveToRVPosition* cmd = new CmdRVReqMoveToRVPosition(500, this);
                        cmd->SetRVPosition(sealPos);
                        m_SchedulerCommandProcessor->pushCmd(cmd);
                    }
                    else
                    {
                        //todo: error handling
                        LogDebug(QString("Get invalid RV position: %1").arg(m_CurProgramStepInfo.stationID));
                    }
                }
                else
                {
                    m_SchedulerMachine->NotifyHitTubeAfter();
                }
            }
            else
            {
                if("Scheduler::RVReqMoveToRVPosition" == cmdName)
                {
                    if(DCL_ERR_DEV_RV_MOTOR_INTERNALSTEPS_RETRY == retCode)
                    {
                        //fail to move to seal, raise event here
                        LogDebug(QString("Program Step Move to Tube(after) %1 Internal Steps Failed").arg(targetPos));
                        Global::EventObject::Instance().RaiseEvent(0, 500030011, Scenario, true);
                        m_SchedulerMachine->SendErrorSignal();
                    }
                    else if(DCL_ERR_DEV_RV_MOTOR_INTERNALSTEPS_EXCEEDUPPERLIMIT == retCode)
                    {
                        m_SchedulerMachine->SendErrorSignal();
                    }
                }
                if(CTRL_CMD_PAUSE == ctrlCmd)
                {
                    m_PauseToBeProcessed = true;
                }
                if(m_PauseToBeProcessed)
                {
                    RVPosition_t sealPos = GetRVSealPositionByStationID(m_CurProgramStepInfo.stationID);
                    if(m_PositionRV == sealPos)
                    {
                        m_SchedulerMachine->NotifyPause(PSSM_READY_TO_TUBE_AFTER);
                        m_PauseToBeProcessed = false;
                        DequeueNonDeviceCommand();
                    }
                }
            }
        }
        else if(PSSM_READY_TO_DRAIN == stepState)
        {
            if((CTRL_CMD_PAUSE == ctrlCmd)||(m_PauseToBeProcessed))
            {
                if(m_PauseToBeProcessed)
                {
                    RVPosition_t sealPos = GetRVSealPositionByStationID(m_CurProgramStepInfo.stationID);
                    if(m_PositionRV == sealPos)
                    {
                        //release pressure
                        AllStop();
                        m_PauseToBeProcessed = false;
                        m_SchedulerMachine->NotifyPause(PSSM_READY_TO_DRAIN);
                        DequeueNonDeviceCommand();
                    }
                }
                else
                {
                    m_PauseToBeProcessed = true;
                    RVPosition_t sealPos = GetRVSealPositionByStationID(m_CurProgramStepInfo.stationID);
                    if(RV_UNDEF != sealPos)
                    {
                        CmdRVReqMoveToRVPosition* cmd = new CmdRVReqMoveToRVPosition(500, this);
                        cmd->SetRVPosition(sealPos);
                        m_SchedulerCommandProcessor->pushCmd(cmd);
                    }
                    else
                    {
                        //todo: error handling
                        LogDebug(QString("Get invalid RV position: %1").arg(m_CurProgramStepInfo.stationID));
                    }
                }
            }
            else if( "Scheduler::ALDraining"== cmdName)
            {
                if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    LogDebug(QString("Program Step Draining succeed!"));
                    m_SchedulerMachine->NotifyDrainFinished();
                }
                else if(DCL_ERR_DEV_LA_DRAINING_TIMEOUT_BUILDPRESSURE == retCode)
                {
                    LogDebug(QString("Program Step Draining Build Pressure timeout"));
                    Global::EventObject::Instance().RaiseEvent(0, 500040201, Scenario, true);
                    m_SchedulerMachine->SendErrorSignal();
                }
            }
            else if(DCL_ERR_UNDEFINED != retCode)
            {
                //todo: error handling here
            }
        }
        else if(PSSM_STEP_FINISH == stepState)
        {
            //todo: start next program step or finish all program
            m_UsedStationIDs.append(m_CurProgramStepInfo.stationID);
            LogDebug(QString("Program Step Finished"));
            this->GetNextProgramStepInformation(m_CurProgramID, m_CurProgramStepInfo);
            QString ProgramName = mp_DataManager->GetProgramList()->GetProgram(m_CurProgramID)->GetName();
            if(m_CurProgramStepIndex != -1)
            {
                //start next step
                LogDebug(QString("Start step %1").arg(m_CurProgramID));
                m_SchedulerMachine->NotifyStepFinished();
                //send command to main controller to tell the left time
                quint32 leftSeconds = GetCurrentProgramStepNeededTime(m_CurProgramID);


                QTime leftTime(0,0,0);
                leftTime = leftTime.addSecs(leftSeconds);
                MsgClasses::CmdCurrentProgramStepInfor* commandPtr(new MsgClasses::CmdCurrentProgramStepInfor(5000, m_CurReagnetName, m_CurProgramStepIndex, leftSeconds));
                Q_ASSERT(commandPtr);
                Global::tRefType Ref = GetNewCommandRef();
                SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

                //log
                //LOG_STR_ARG(STR_CURRENT_PROGRAM_NAME_STEP_REAGENT_LEFTTIME,Global::FmtArgs()<< ProgramName << m_CurProgramStepIndex + 1 << m_CurReagnetName << leftSeconds);
            }
            else
            {
                LogDebug(QString("All Steps finished."));
                m_SchedulerMachine->NotifyProgramFinished();

            }
        }
        else if(PSSM_PROGRAM_FINISH == stepState)
        {
            //program finished
            QString ProgramName = mp_DataManager->GetProgramList()->GetProgram(m_CurProgramID)->GetName();
            LogDebug(QString("Program finished!"));
            m_SchedulerMachine->SendRunComplete();
            //m_SchedulerMachine->Stop();
            //todo: tell main controller that program is complete
            UpdateStationReagentStatus();

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

            //LOG
            //LOG_STR_ARG(STR_FINISH_PROGRAM, Global::FmtArgs()<<ProgramName);
        }
        else if(PSSM_PAUSE == stepState)
        {
            if(CTRL_CMD_START == ctrlCmd)
            {
                // resume the program
                m_SchedulerMachine->NotifyResume();
                DequeueNonDeviceCommand();
                // tell the main controller the program is resuming
                MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_BEGIN));
                Q_ASSERT(commandPtrFinish);
                Global::tRefType fRef = GetNewCommandRef();
                SendCommand(fRef, Global::CommandShPtr_t(commandPtrFinish));
            }
        }
        else if(PSSM_PAUSE_DRAIN == stepState)
        {
            if(CTRL_CMD_START == ctrlCmd)
            {
                m_SchedulerMachine->NotifyResumeDrain();
                DequeueNonDeviceCommand();
                // tell the main controller the program is resuming
                MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_BEGIN));
                Q_ASSERT(commandPtrFinish);
                Global::tRefType fRef = GetNewCommandRef();
                SendCommand(fRef, Global::CommandShPtr_t(commandPtrFinish));
            }
        }
        else if(PSSM_ABORTING == stepState)
        {
            if((DCL_ERR_FCT_CALL_SUCCESS == retCode)&&("Scheduler::RVReqMoveToRVPosition" == cmdName))
            {
                this->Drain();
            }
            else if((DCL_ERR_FCT_CALL_SUCCESS == retCode)&&("Scheduler::ALDraining" == cmdName))
            {
                StopDrain();
                m_SchedulerMachine->NotifyAbort();
            }
        }
        else if(PSSM_ABORTED == stepState)
        {
            //program finished
            AllStop();
            LogDebug("Program aborted!");
            m_SchedulerMachine->SendRunComplete();
            // tell the main controller the program has been aborted
            MsgClasses::CmdProgramAcknowledge* commandPtrAbortFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_ABORT_FINISHED));
            Q_ASSERT(commandPtrAbortFinish);
            Global::tRefType fRef = GetNewCommandRef();
            SendCommand(fRef, Global::CommandShPtr_t(commandPtrAbortFinish));

            UpdateStationReagentStatus();

            //send command to main controller to tell the left time
            QTime leftTime(0,0,0);
            MsgClasses::CmdCurrentProgramStepInfor* commandPtr(new MsgClasses::CmdCurrentProgramStepInfor(5000, "", m_CurProgramStepIndex, 0));
            LogDebug("SchedulerMainThreadController::HandleRunState send command Program aborted!");
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        }
    }
#if 0
    //work around to avoid continus START cmd
    if(CTRL_CMD_START == ctrlCmd)
    {
        if((PSSM_PAUSE != stepState)&&(PSSM_PAUSE_DRAIN != stepState))
        {
            DequeueNonDeviceCommand();
        }
    }
#endif
}

void SchedulerMainThreadController::HandleErrorState(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd, SchedulerStateMachine_t currentState)
{
    ReturnCode_t retCode;
    QString cmdName = "";
    if(cmd != NULL)
    {
        if(!(cmd->GetResult(retCode)))
        {
            retCode = DCL_ERR_UNDEFINED;
        }
        cmdName = cmd->GetName();
    }

    if (SM_ERR_WAIT == currentState)
    {
        LogDebug("Scheduler waitting event handler give instruction!");
        if(CTRL_CMD_RC_RESTART == ctrlCmd)
        {
            if(((m_SchedulerMachine->GetPreviousState()) & 0xFFFF)==PSSM_ST)
            {
                m_SchedulerMachine->NotifyResumeToSelftest();
            }
            else
            {
                m_SchedulerMachine->NotifyResume();
            }
            DequeueNonDeviceCommand();
        }
        else if(CTRL_CMD_RC_REPORT == ctrlCmd)
        {
            LogDebug("Go to move to RC_Report");
            m_SchedulerMachine->NotifyRCReport();
            DequeueNonDeviceCommand();
        }
        else if(CTRL_CMD_RS_GET_ORIGINAL_POSITION_AGAIN == ctrlCmd)
        {
            LogDebug("Go to RS RV Move To Initial Position!");
            m_SchedulerMachine->NotifyRsRvMoveToInitPosition();
            DequeueNonDeviceCommand();
        }
        else if(CTRL_CMD_RS_STANDBY == ctrlCmd)
        {
            LogDebug("Go to RS_STANDBY!");
            m_SchedulerMachine->NotifyRsReleasePressure();
            DequeueNonDeviceCommand();
        }
        else
        {
            LogDebug(QString("Unknown Command: %1").arg(ctrlCmd, 0, 16));
        }
    }
    else if(SM_ERR_RS_RV_MOVING_TO_INIT_POS == currentState)
    {
        LogDebug(QString("RS_RV_GET_ORIGINAL_POSITION_AGAIN Response: %1").arg(retCode));
        if( "Scheduler::RVReqMoveToInitialPosition" == cmdName)
        {
            if( DCL_ERR_FCT_CALL_SUCCESS == retCode )
            {
                LogDebug("Response Move to initial position again succeed!");
                Global::EventObject::Instance().RaiseEvent(m_EventKey, 0, 0, true);
                m_SchedulerMachine->NotifyRsRvMoveToInitPositionFinished();
            }
            else
            {
                LogDebug("Response Move to initial position again failed!");
                Global::EventObject::Instance().RaiseEvent(m_EventKey, 0, 0, false);
                m_SchedulerMachine->NotifyRsRvMoveToInitPositionFinished();
            }
        }
    }
    else if(SM_ERR_RC_REPORT == currentState)
    {
        Global::EventObject::Instance().RaiseEvent(m_EventKey, 0, 0, true);
        m_SchedulerMachine->NotifyRsRvMoveToInitPositionFinished();
    }
    else if(SM_ERR_RS_RELEASE_PRESSURE == currentState)
    {
        m_SchedulerMachine->NotifyRsShutdownFailedHeater();
    }
    else if(SM_ERR_RS_SHUTDOWN_FAILED_HEATER == currentState)
    {
        m_SchedulerMachine->NotifyRsShutdownFailedHeaterFinished();
        Global::EventObject::Instance().RaiseEvent(m_EventKey, 0, 0, true);
    }
}

ControlCommandType_t SchedulerMainThreadController::PeekNonDeviceCommand()
{

    if(m_SchedulerCmdQueue.isEmpty())
        return CTRL_CMD_NONE;

    Global::CommandShPtr_t pt = m_SchedulerCmdQueue.head();
    MsgClasses::CmdQuitAppShutdown* pCmdShutdown = dynamic_cast<MsgClasses::CmdQuitAppShutdown*>(pt.GetPointerToUserData());
    if(pCmdShutdown)
    {
        if (pCmdShutdown->QuitAppShutdownActionType() == DataManager::QUITAPPSHUTDOWNACTIONTYPE_SHUTDOWN)
        {
            return CTRL_CMD_SHUTDOWN;
        }
        if (pCmdShutdown->QuitAppShutdownActionType() == DataManager::QUITAPPSHUTDOWNACTIONTYPE_QUITAPP)
        {
            return CTRL_CMD_QUITAPP;
        }
    }

    MsgClasses::CmdProgramAction* pCmdProgramAction = dynamic_cast<MsgClasses::CmdProgramAction*>(pt.GetPointerToUserData());
    if(pCmdProgramAction)
    {
        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_START)
        {
            m_NewProgramID = pCmdProgramAction->GetProgramID();
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
    }

    NetCommands::CmdSystemAction* pCmdSystemAction = dynamic_cast<NetCommands::CmdSystemAction*>(pt.GetPointerToUserData());
    if(pCmdSystemAction)
    {
        QString cmd = pCmdSystemAction->GetActionString();
        m_EventKey = pCmdSystemAction->GetEventKey();

        LogDebug(QString("Get action: %1").arg(cmd));

        if(cmd == "RS_RV_GetOriginalPositionAgain")
        {
            return CTRL_CMD_RS_GET_ORIGINAL_POSITION_AGAIN;
        }
        if(cmd == "RC_Restart")
        {
            return CTRL_CMD_RC_RESTART;
        }
        if(cmd == "RC_Report")
        {
            return CTRL_CMD_RC_REPORT;
        }
        if(cmd == "RS_Standby")
        {
            return CTRL_CMD_RS_STANDBY;
        }
    }
//    HimalayaErrorHandler::CmdRaiseAlarm* pCmdRaiseAlarm = dynamic_cast<HimalayaErrorHandler::CmdRaiseAlarm*>(pt.GetPointerToUserData());
//    if (pCmdRaiseAlarm)
//    {
//        if(pCmdRaiseAlarm->m_localAlarm)
//        {

//        }
//        else
//        {

//        }
//        //todo temp. return set remote alarm, update later
//        return CTRL_CMD_SET_REMOTE_ALARM;
//    }
    return CTRL_CMD_UNKNOWN;

}//When end of this function, the command in m_SchedulerCmdQueue will be destrory by share pointer (CommandShPtr_t) mechanism

void SchedulerMainThreadController::DequeueNonDeviceCommand()
{
        if(!m_SchedulerCmdQueue.isEmpty())
        {
            m_SchedulerCmdQueue.dequeue();
        }
}

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
                    const CReagent* pLastReagent = pReagentList->GetReagent(m_ReagentIdOfLastStep);
                    if (!pLastReagent)
                        return false;

                    const CReagent* pCurReagent = pReagentList->GetReagent(reagentID);

                    //RG7:Cleaning Solvent, RG8:Cleaning Alcohol
                    QStringList list;
                    list << "RG1"<<"RG2"<<"RG3"<<"RG4"<<"RG5";
                    if (list.contains(pLastReagent->GetGroupID()) && pCurReagent->GetGroupID() == "RG7")
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
        programStepInfor.durationInSeconds = pProgramStep->GetDurationInSeconds();
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
                                                                 QList<StationUseRecord_t>& usedStations, bool IsLastStep)
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
        return this->SelectStationByReagent(pReagent,  unusedStationIDs,
                                          usedStations, IsLastStep, rmsMode);
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

//    ListOfIDs_t* stepIDs = pProgram->OrderedListOfStepIDs();
    bool isLastStep = false;

    m_ProgramStationList.clear();
    m_StationList.clear();

    ListOfIDs_t unusedStationIDs = pDashboardDataStationList->GetOrderedListOfDashboardStationIDs();
    QList<StationUseRecord_t> usedStations;

    for (int i = beginStep; i < pProgram->GetNumberOfSteps(); i++)
    {
        const CProgramStep* pProgramStep = pProgram->GetProgramStep(i);//use order index
        ProgramStationInfo_t stationInfo;
        isLastStep = (i == (pProgram->GetNumberOfSteps() - 1));
        QString reagentID = pProgramStep->GetReagentID();
        stationInfo.ReagentGroupID =GetReagentGroupID(reagentID);
        stationInfo.StationID = this->SelectStationFromReagentID(reagentID, unusedStationIDs, usedStations, isLastStep);
        m_ProgramStationList.enqueue(stationInfo);
        m_StationList.push_back(stationInfo.StationID);
    }

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
    int index = SpecifiedStepIndex;
    if (-1 == index)
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
        index = programStepIDIndex;
    }

    for (int i = index; i < pProgram->GetNumberOfSteps(); i++)
    {
        const CProgramStep* pProgramStep = pProgram->GetProgramStep(i);//use order index
        quint32 soakTime = pProgramStep->GetDurationInSeconds();
        leftTime += soakTime;
        bool isPressure = (pProgramStep->GetPressure() == "On");
        bool isVaccum = (pProgramStep->GetVacuum() == "On");
        if (isPressure ^ isVaccum)
        {
            leftTime += 30;//suppose pressure or vaccum need 30 seconds to build up
        }
        else if(isPressure && isVaccum)
        {
            leftTime += 30 * (soakTime / 60); //suppose P/V take turns in 1 min, and each takes 30 seconds to build up
        }
        leftTime += 5 +  2 * 2; //suppose RV need 5 seconds to move to the target station, and movment between tube and its seal need 2 seconds.
        leftTime += 60; //suppose need 60 seconds to fill
        leftTime += 40; //suppose need 40 seconds to drain
        leftTime += 20; //suppose need 20 seconds to heat level sensor
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
        bool isPressure = (pProgramStep->GetPressure() == "On");
        bool isVaccum = (pProgramStep->GetVacuum() == "On");
        if (isPressure ^ isVaccum)
        {
            leftTime += 30;//suppose pressure or vaccum need 30 seconds to build up
        }
        else if(isPressure && isVaccum)
        {
            leftTime += 30 * (soakTime / 60); //suppose P/V take turns in 1 min, and each takes 30 seconds to build up
        }
        leftTime += 5 +  2 * 2; //suppose RV need 5 seconds to move to the target station, and movment between tube and its seal need 2 seconds.
        leftTime += 60; //suppose need 60 seconds to fill
        leftTime += 40; //suppose need 40 seconds to drain
        leftTime += 20; //suppose need 20 seconds to heat level sensor
    }
    return leftTime;
}

//void SchedulerMainThreadController::OnRaiseAlarmLocalRemote(Global::tRefType Ref,
//                                                            const HimalayaErrorHandler::CmdRaiseAlarm &Cmd)
//{
//    Q_UNUSED(Ref);
//    m_Mutex.lock();
//    //m_SchedulerCmdQueue.enqueue(Global::CommandShPtr_t(new HimalayaErrorHandler::CmdRaiseAlarm(Cmd.m_localAlarm)));
//    m_Mutex.unlock();
//    this->SendAcknowledgeOK(Ref);
//}

//client-->master
void SchedulerMainThreadController::OnProgramAction(Global::tRefType Ref,
                                                    const MsgClasses::CmdProgramAction &Cmd)
{
    m_Mutex.lock();
    m_SchedulerCmdQueue.enqueue(Global::CommandShPtr_t(new MsgClasses::CmdProgramAction(Cmd.GetTimeout(), Cmd.GetProgramID(), Cmd.ProgramActionType(),
                                                                                        Cmd.ProgramEndDateTime())));
    m_Mutex.unlock();
    this->SendAcknowledgeOK(Ref);

    //Check for Service
    DataManager::CHimalayaUserSettings* pUserSetting = mp_DataManager->GetUserSettings();

    int operationHours = pUserSetting->GetOperationHours();
    if (operationHours >= 365 * 24)
    {
       Global::EventObject::Instance().RaiseEvent(EVENT_SERVICE_OPERATIONTIME_OVERDUE);
    }


    int activeCarbonHours = pUserSetting->GetActiveCarbonHours();
    int usedExhaustSystem = pUserSetting->GetUseExhaustSystem();
    int warningthreshold = 0;
    if (1 == usedExhaustSystem)
    {
        warningthreshold = 5* 30 * 24;
    }
    else
    {
        warningthreshold = 10 * 30 * 24;
    }

    if (activeCarbonHours >= warningthreshold)
    {
       Global::EventObject::Instance().RaiseEvent(EVENT_SERVICE_ACTIVECARBONTIME_OVERDUE);
    }

    //log
    quint32 cmdid = 0;
    Q_UNUSED(cmdid);
    if (Cmd.ProgramActionType() == DataManager::PROGRAM_START)
    {
        cmdid = STR_PROGRAM_COMMAND_START_PROGRAM;
    }
    else if(Cmd.ProgramActionType() == DataManager::PROGRAM_PAUSE)
    {
        cmdid = STR_PROGRAM_COMMAND_PAUSE_PROGRAM;
    }
    else if(Cmd.ProgramActionType() == DataManager::PROGRAM_ABORT)
    {
        cmdid = STR_PROGRAM_COMMAND_ABORT_PROGRAM;
    }
    else if(Cmd.ProgramActionType() == DataManager::PROGRAM_DRAIN)
    {
        cmdid = STR_PROGRAM_COMMAND_DRAIN;
    }
    //LOG_STR_ARG(STR_SCHDEULER_RECEIVE_MASTER_ACTION_COMMAND, Global::tTranslatableStringList()<<Global::TranslatableString(cmdid));
}

//response or recovery
void SchedulerMainThreadController::OnActionCommandReceived(Global::tRefType Ref, const NetCommands::CmdSystemAction & Cmd)
{
    Q_UNUSED(Ref)
    m_Mutex.lock();
    NetCommands::CmdSystemAction *p_CmdSystemAction = new NetCommands::CmdSystemAction();
    p_CmdSystemAction->SetActionString(Cmd.GetActionString());
    p_CmdSystemAction->SetEventKey(Cmd.GetEventKey());
    m_SchedulerCmdQueue.enqueue(Global::CommandShPtr_t(p_CmdSystemAction));
    m_Mutex.unlock();
}

void SchedulerMainThreadController::OnKeepCassetteCount(Global::tRefType Ref, const MsgClasses::CmdKeepCassetteCount & Cmd)
{
    m_ProcessCassetteCount = Cmd.CassetteCount();
    this->SendAcknowledgeOK(Ref);
}

void SchedulerMainThreadController::OnProgramSelected(Global::tRefType Ref, const MsgClasses::CmdProgramSelected & Cmd)
{
    this->SendAcknowledgeOK(Ref);
    QString curProgramID;
    QString strProgramID = Cmd.GetProgramID();
    if (strProgramID.at(0) == 'C')
    {
        int sep = strProgramID.indexOf('_');
        curProgramID = strProgramID.left(sep);
        m_ReagentIdOfLastStep = strProgramID.right(strProgramID.count()- sep -1);//m_ReagentIdOfLastStep used in GetNextProgramStepInformation(...)
    }
    else
        curProgramID = strProgramID;

    m_CurProgramStepIndex = -1;
    this->GetNextProgramStepInformation(curProgramID, m_CurProgramStepInfo, true);//only to get m_CurProgramStepIndex

    unsigned int timeProposed = 0;
    unsigned int paraffinMeltCostedtime = 0;
    unsigned int costedTimeBeforeParaffin = 0;
    int whichStep = 0;
    if (m_CurProgramStepIndex != -1)
    {
        timeProposed = GetLeftProgramStepsNeededTime(curProgramID, m_CurProgramStepIndex);

        m_FirstProgramStepIndex = m_CurProgramStepIndex;
        this->PrepareProgramStationList(curProgramID, m_CurProgramStepIndex);
        m_CurProgramStepIndex = -1;

        paraffinMeltCostedtime = this->GetOvenHeatingTime();
        if (-1 != Cmd.ParaffinStepIndex())//has Paraffin
        {
            costedTimeBeforeParaffin = timeProposed - GetLeftProgramStepsNeededTime(curProgramID, Cmd.ParaffinStepIndex());
        }

        //cheack safe reagent
        whichStep = WhichStepHasNoSafeReagent(curProgramID);
    }

    //send back the proposed program end time
    MsgClasses::CmdProgramSelectedReply* commandPtr(new MsgClasses::CmdProgramSelectedReply(5000, timeProposed,
                                                                                paraffinMeltCostedtime,
                                                                                costedTimeBeforeParaffin,
                                                                                whichStep,
                                                                                m_StationList));
    Q_ASSERT(commandPtr);
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

}

void SchedulerMainThreadController::OnQuitAppShutdown(Global::tRefType Ref, const MsgClasses::CmdQuitAppShutdown & Cmd)
{
    m_Mutex.lock();
    m_SchedulerCmdQueue.enqueue(Global::CommandShPtr_t(new MsgClasses::CmdQuitAppShutdown(Cmd.GetTimeout(), Cmd.QuitAppShutdownActionType())));
    m_Mutex.unlock();
    this->SendAcknowledgeOK(Ref);
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
    qint32 scenario = 0;
    bool reagentRelated = false;
    switch(State)
    {
    case SM_UNDEF:
        break;
    case SM_INIT:
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
    case PSSM_READY_TO_HEAT_LEVEL_SENSOR_S1:
        scenario = 211;
        reagentRelated = true;
        break;
    case PSSM_READY_TO_HEAT_LEVEL_SENSOR_S2:
        scenario = 211;
        reagentRelated = true;
        break;
    case PSSM_READY_TO_TUBE_BEFORE:
        scenario = 217;
        reagentRelated = true;
        break;
    case PSSM_READY_TO_FILL:
        scenario = 212;
        reagentRelated = true;
        break;
    case PSSM_READY_TO_SEAL:
        scenario = 213;
        reagentRelated = true;
        break;
    case PSSM_SOAK:
        scenario = 214;
        reagentRelated = true;
        break;
    case PSSM_READY_TO_TUBE_AFTER:
        scenario = 215;
        reagentRelated = true;
        break;
    case PSSM_READY_TO_DRAIN:
        scenario = 216;
        reagentRelated = true;
        break;
    case PSSM_STEP_FINISH:
        break;
    case PSSM_PROGRAM_FINISH:
        break;
    case PSSM_PAUSE:
        scenario = 206;
        break;
    case PSSM_PAUSE_DRAIN:
        scenario = 206;
        break;
    case PSSM_ABORTING:
        scenario = 206;
        break;
    case PSSM_ABORTED:
        scenario = 206;
        break;
    case PSSM_ST:
        scenario = 200;
        break;
    case PSSM_ST_INIT:
        scenario = 200;
        break;
    case PSSM_ST_TEMP_CHECKING:
        scenario = 200;
        break;
    case PSSM_ST_CURRENT_CHECKING:
        scenario = 200;
        break;
    case PSSM_ST_VOLTAGE_CHECKING:
        scenario = 200;
        break;
    case PSSM_ST_RV_POSITION_CHECKING:
        scenario = 200;
        break;
    case PSSM_ST_PRESSURE_CHECKING:
        scenario = 200;
        break;
    case PSSM_ST_SEALING_CHECKING:
        scenario = 200;
        break;
    case PSSM_ST_STATION_CHECKING:
        scenario = 200;
        break;
    case PSSM_ST_STATION_CHECK_FINISH:
        scenario = 200;
        break;
    case PSSM_ST_DONE:
        scenario = 200;
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
    return scenario;
}


void SchedulerMainThreadController::OnDCLConfigurationFinished(ReturnCode_t RetCode)
{
    if(RetCode == DCL_ERR_FCT_CALL_SUCCESS)
    {
        ReturnCode_t retCode;

        //hardware not ready yet
        m_SchedulerCommandProcessor->pushCmd(new CmdPerTurnOnMainRelay(500, this));
        SchedulerCommandShPtr_t resPerTurnOnRelay;
        while(!PopDeviceControlCmdQueue(resPerTurnOnRelay));
        resPerTurnOnRelay->GetResult(retCode);
        if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            //todo: error handling
            LogDebug(QString("Failed turn on main relay, return code: %1").arg(retCode));
            goto ERROR;
        }
        SetFunctionModuleWork(&m_FunctionModuleStatusList, CANObjectKeyLUT::FCTMOD_PER_MAINRELAYDO, true);

        SetFunctionModuleWork(&m_FunctionModuleStatusList, CANObjectKeyLUT::FCTMOD_RETORT_SIDETEMPCTRL, true);

        SetFunctionModuleWork(&m_FunctionModuleStatusList, CANObjectKeyLUT::FCTMOD_RETORT_BOTTOMTEMPCTRL, true);

        SetFunctionModuleWork(&m_FunctionModuleStatusList, CANObjectKeyLUT::FCTMOD_RV_TEMPCONTROL, true);

        SetFunctionModuleWork(&m_FunctionModuleStatusList, CANObjectKeyLUT::FCTMOD_OVEN_BOTTOMTEMPCTRL, true);

        SetFunctionModuleWork(&m_FunctionModuleStatusList, CANObjectKeyLUT::FCTMOD_OVEN_TOPTEMPCTRL, true);

        SetFunctionModuleWork(&m_FunctionModuleStatusList, CANObjectKeyLUT::FCTMOD_AL_TUBE1TEMPCTRL, true);

        SetFunctionModuleWork(&m_FunctionModuleStatusList, CANObjectKeyLUT::FCTMOD_AL_TUBE2TEMPCTRL, true);

        bool ok;
        qreal pressureDrift = mp_DataManager->GetProgramSettings()->GetParameterValue("LA", "Base", "PressureDrift", ok);
        if((ok)&&(pressureDrift != 0)&&(pressureDrift<3)&&(pressureDrift>(-3)))//todo: verify this
        {
            m_SchedulerCommandProcessor->ALSetPressureDrift(pressureDrift);
        }
        else
        {
            quint32 counter = 0;
            pressureDrift = UNDEFINED_4_BYTE;
            while((pressureDrift == UNDEFINED_4_BYTE)&&(counter <10))
            {
                pressureDrift= m_SchedulerCommandProcessor->ALGetRecentPressure();
                counter++;
            }
            if(UNDEFINED_4_BYTE != pressureDrift)
            {
                mp_DataManager->GetProgramSettings()->SetParameterValue("LA", "Base", "PressureDrift", pressureDrift);
                m_SchedulerCommandProcessor->ALSetPressureDrift(pressureDrift);
            }
            else
            {
                mp_DataManager->GetProgramSettings()->SetParameterValue("LA", "Base", "PressureDrift", 0);
                m_SchedulerCommandProcessor->ALSetPressureDrift(0);
            }
        }
        CreateFunctionModuleStatusList(&m_FunctionModuleStatusList);

        // set state machine "init" to "idle" (David)
        m_SchedulerMachine->SendSchedulerInitComplete();
        //for debug
        LogDebug(QString("Current state of Scheduler is: %1").arg(m_SchedulerMachine->GetCurrentState()));
        //send command to main controller to tell init complete
        MsgClasses::CmdProgramAcknowledge* commandPtr(new MsgClasses::CmdProgramAcknowledge(5000, DataManager::PROGRAM_READY));
        Q_ASSERT(commandPtr);
        Global::tRefType Ref = GetNewCommandRef();
        SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
    }
    else
    {
ERROR:
        //send command to main controller to tell init failed
        MsgClasses::CmdProgramAcknowledge* commandPtr(new MsgClasses::CmdProgramAcknowledge(5000, DataManager::PROGRAM_SELFTEST_FAILED));
        Q_ASSERT(commandPtr);
        Global::tRefType Ref = GetNewCommandRef();
        SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

        if(RetCode == DCL_ERR_TIMEOUT)
        {
            LogDebug("Some devices are not found during DCL's initialization period.");
        }
        //error happend
        // set state machine "init" to "error" (David)
        m_SchedulerMachine->SendErrorSignal();
        //for debug
        LogDebug(QString("Error while init, Current state of Scheduler is: %1").arg(m_SchedulerMachine->GetCurrentState()));
    }
    m_TickTimer.start();

	// Create HeatingStrategy
	mp_HeatingStrategy = QSharedPointer<HeatingStrategy>(new HeatingStrategy(this,
							m_SchedulerCommandProcessor, mp_DataManager));

}

void SchedulerMainThreadController::HardwareMonitor(const QString& StepID)
{
    Q_UNUSED(StepID)

    QString ReagentGroup = m_CurProgramStepInfo.reagentGroup;
    quint32 Scenario = GetScenarioBySchedulerState(m_SchedulerMachine->GetCurrentState(),ReagentGroup);
       // if(StepID == "IDLE")
	HardwareMonitor_t strctHWMonitor = m_SchedulerCommandProcessor->HardwareMonitor();

    // Run Heating Strategy
    DeviceControl::ReturnCode_t retCode = mp_HeatingStrategy->RunHeatingStrategy(strctHWMonitor, Scenario);
    if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
    {
        //Error Handling
    }
    if(strctHWMonitor.PressureAL == UNDEFINED_VALUE)
	{
        m_PressureAL = strctHWMonitor.PressureAL;
	}
    if(m_PressureAL < -45 || m_PressureAL > 45){
        quint32 EVENT_LA_PressureSensor_OutofRange = 34; //only for test
        Global::EventObject::Instance().RaiseEvent(0,EVENT_LA_PressureSensor_OutofRange,Scenario,true);
    }
	if(strctHWMonitor.TempALLevelSensor != UNDEFINED_VALUE)
	{
        m_TempALLevelSensor = strctHWMonitor.TempALLevelSensor;
	}
	if(strctHWMonitor.TempALTube1 != UNDEFINED_VALUE)
	{
        m_TempALTube1 = strctHWMonitor.TempALTube1;
	}
	if(strctHWMonitor.TempALTube2 != UNDEFINED_VALUE)
	{
        m_TempALTube2 = strctHWMonitor.TempALTube2;
	}
	if(strctHWMonitor.TempRV1 != UNDEFINED_VALUE)
	{
        m_TempRV1 = strctHWMonitor.TempRV1;
	}
	if(strctHWMonitor.TempRV2 != UNDEFINED_VALUE)
	{
        m_TempRV2 = strctHWMonitor.TempRV2;
	}
	if(strctHWMonitor.TempRTBottom != UNDEFINED_VALUE)
	{
        m_TempRTBottom = strctHWMonitor.TempRTBottom;
	}
	if(strctHWMonitor.TempRTSide!= UNDEFINED_VALUE)
	{
        m_TempRTSide = strctHWMonitor.TempRTSide;
	}
	if(strctHWMonitor.TempOvenBottom != UNDEFINED_VALUE)
	{
        m_TempOvenBottom = strctHWMonitor.TempOvenBottom;
	}
	if(strctHWMonitor.TempOvenTop != UNDEFINED_VALUE)
	{
        m_TempOvenTop = strctHWMonitor.TempOvenTop;
	}
	if(strctHWMonitor.OvenLidStatus != UNDEFINED_VALUE)
    {
        if(((m_OvenLidStatus == 0) || (m_OvenLidStatus == UNDEFINED_VALUE))&&(strctHWMonitor.OvenLidStatus == 1))
        {
            //oven is open
            MsgClasses::CmdLockStatus* commandPtr(new MsgClasses::CmdLockStatus(5000, DataManager::PARAFFIN_BATH_LOCK, false));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        }
        if(((m_OvenLidStatus == 1) || (m_OvenLidStatus == UNDEFINED_VALUE))&&(strctHWMonitor.OvenLidStatus == 0))
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
            // retort is open, turn on the fan
            m_SchedulerCommandProcessor->pushCmd(new CmdALTurnOnFan(500, this));
            if((m_SchedulerMachine->GetCurrentState() & 0xF)== SM_BUSY)
            {
                Global::EventObject::Instance().RaiseEvent(0, 500010461, Scenario, true);
                m_SchedulerMachine->SendErrorSignal();
            }
            MsgClasses::CmdLockStatus* commandPtr(new MsgClasses::CmdLockStatus(5000, DataManager::RETORT_LOCK, false));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        }
        if(((m_RetortLockStatus == 1) || (m_RetortLockStatus == UNDEFINED_VALUE))&&(strctHWMonitor.RetortLockStatus == 0))
		{
           // retort is closed, turn off the fan
			m_SchedulerCommandProcessor->pushCmd(new CmdALTurnOffFan(500, this));
            MsgClasses::CmdLockStatus* commandPtr(new MsgClasses::CmdLockStatus(5000, DataManager::RETORT_LOCK, true));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
		}
        m_RetortLockStatus = strctHWMonitor.RetortLockStatus;
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

void SchedulerMainThreadController::PushDeviceControlCmdQueue(Scheduler::SchedulerCommandShPtr_t CmdPtr)
{
    m_MutexDeviceControlCmdQueue.lock();
    m_DeviceControlCmdQueue.push_back(CmdPtr);
    m_MutexDeviceControlCmdQueue.unlock();
}

void SchedulerMainThreadController::StepStart()
{

}

void SchedulerMainThreadController::HeatLevelSensor()
{
    SchedulerStateMachine_t stepState = m_SchedulerMachine->GetCurrentState();
    if(PSSM_READY_TO_HEAT_LEVEL_SENSOR_S1 == stepState)
    {
        CmdALStartTemperatureControlWithPID* cmd  = new CmdALStartTemperatureControlWithPID(500, this);
        cmd->SetType(AL_LEVELSENSOR);
        //todo: get temperature here
        cmd->SetNominalTemperature(90);
        cmd->SetSlopeTempChange(10);
        cmd->SetMaxTemperature(120);
        cmd->SetControllerGain(1212);
        cmd->SetResetTime(1000);
        cmd->SetDerivativeTime(80);
        m_SchedulerCommandProcessor->pushCmd(cmd);
        SetFunctionModuleWork(&m_FunctionModuleStatusList, CANObjectKeyLUT::FCTMOD_AL_LEVELSENSORTEMPCTRL, true);
    }
    else if(PSSM_READY_TO_HEAT_LEVEL_SENSOR_S2 == stepState)
    {
        CmdALStartTemperatureControlWithPID* cmd  = new CmdALStartTemperatureControlWithPID(500, this);
        cmd->SetType(AL_LEVELSENSOR);
        //todo: get temperature here
        cmd->SetNominalTemperature(90);
        cmd->SetSlopeTempChange(10);
        cmd->SetMaxTemperature(120);
        cmd->SetControllerGain(200);
        cmd->SetResetTime(1000);
        cmd->SetDerivativeTime(0);
        m_SchedulerCommandProcessor->pushCmd(cmd);
    }
}

void SchedulerMainThreadController::SealingCheck()
{
    LogDebug("Send cmd to DCL to start sealing test. ");
    CmdIDSealingCheck* cmd = new CmdIDSealingCheck(500, this);
    cmd->SetThresholdPressure(10);
    m_SchedulerCommandProcessor->pushCmd(cmd);
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
    LogDebug("Send cmd to DCL to let Release Pressure.");
    this->AllStop();
}

void SchedulerMainThreadController::ShutdownFailedHeater()
{
   QList<FunctionModuleStatus_t> failList =  this->GetFailedFunctionModuleList(&m_FunctionModuleStatusList);
   if(failList.count() > 0)
   {
       QListIterator<FunctionModuleStatus_t> iter(failList);
       FunctionModuleStatus_t fmStatus;
       while(iter.hasNext())
       {
           CmdSchedulerCommandBase* cmd;
           fmStatus = iter.next();
           switch (fmStatus.FctModID)
           {
           case CANObjectKeyLUT::FCTMOD_RV_TEMPCONTROL:
               m_SchedulerCommandProcessor->pushCmd(new CmdRVSetTempCtrlOFF(500, this));
               SetFunctionModuleWork(&m_FunctionModuleStatusList, CANObjectKeyLUT::FCTMOD_RV_TEMPCONTROL, false);
               break;
           case CANObjectKeyLUT::FCTMOD_AL_LEVELSENSORTEMPCTRL:
               cmd = new CmdALSetTempCtrlOFF(500, this);
               (dynamic_cast<CmdALSetTempCtrlOFF*>(cmd))->Settype(AL_LEVELSENSOR);
               m_SchedulerCommandProcessor->pushCmd(cmd);
               SetFunctionModuleWork(&m_FunctionModuleStatusList, CANObjectKeyLUT::FCTMOD_AL_LEVELSENSORTEMPCTRL, false);
               break;
           case CANObjectKeyLUT::FCTMOD_AL_TUBE1TEMPCTRL:
               cmd = new CmdALSetTempCtrlOFF(500, this);
               (dynamic_cast<CmdALSetTempCtrlOFF*>(cmd))->Settype(AL_TUBE1);
               m_SchedulerCommandProcessor->pushCmd(cmd);
               SetFunctionModuleWork(&m_FunctionModuleStatusList, CANObjectKeyLUT::FCTMOD_AL_TUBE1TEMPCTRL, false);
               break;
           case CANObjectKeyLUT::FCTMOD_AL_TUBE2TEMPCTRL:
               cmd = new CmdALSetTempCtrlOFF(500, this);
               (dynamic_cast<CmdALSetTempCtrlOFF*>(cmd))->Settype(AL_TUBE2);
               m_SchedulerCommandProcessor->pushCmd(cmd);
               SetFunctionModuleWork(&m_FunctionModuleStatusList, CANObjectKeyLUT::FCTMOD_AL_TUBE2TEMPCTRL, false);
               break;
           case CANObjectKeyLUT::FCTMOD_OVEN_TOPTEMPCTRL:
               cmd = new CmdOvenSetTempCtrlOFF(500, this);
               (dynamic_cast<CmdOvenSetTempCtrlOFF*>(cmd))->Settype(OVEN_TOP);
               m_SchedulerCommandProcessor->pushCmd(cmd);
               SetFunctionModuleWork(&m_FunctionModuleStatusList, CANObjectKeyLUT::FCTMOD_OVEN_TOPTEMPCTRL, false);
               break;
           case CANObjectKeyLUT::FCTMOD_OVEN_BOTTOMTEMPCTRL:
               cmd = new CmdOvenSetTempCtrlOFF(500, this);
               (dynamic_cast<CmdOvenSetTempCtrlOFF*>(cmd))->Settype(OVEN_BOTTOM);
               m_SchedulerCommandProcessor->pushCmd(cmd);
               SetFunctionModuleWork(&m_FunctionModuleStatusList, CANObjectKeyLUT::FCTMOD_OVEN_BOTTOMTEMPCTRL, false);
               break;
           case CANObjectKeyLUT::FCTMOD_RETORT_BOTTOMTEMPCTRL:
               cmd = new CmdRTSetTempCtrlOFF(500, this);
               (dynamic_cast<CmdRTSetTempCtrlOFF*>(cmd))->SetType(RT_BOTTOM);
               m_SchedulerCommandProcessor->pushCmd(cmd);
               SetFunctionModuleWork(&m_FunctionModuleStatusList, CANObjectKeyLUT::FCTMOD_RETORT_BOTTOMTEMPCTRL, false);
               break;
           case CANObjectKeyLUT::FCTMOD_RETORT_SIDETEMPCTRL:
               cmd = new CmdRTSetTempCtrlOFF(500, this);
               (dynamic_cast<CmdRTSetTempCtrlOFF*>(cmd))->SetType(RT_SIDE);
               m_SchedulerCommandProcessor->pushCmd(cmd);
               SetFunctionModuleWork(&m_FunctionModuleStatusList, CANObjectKeyLUT::FCTMOD_RETORT_SIDETEMPCTRL, false);
               break;
           }
        }
   }
}

void SchedulerMainThreadController::MoveRV()
{
    SchedulerStateMachine_t stepState = m_SchedulerMachine->GetCurrentState();
    CmdRVReqMoveToRVPosition* cmd = new CmdRVReqMoveToRVPosition(500, this);

    if(PSSM_READY_TO_TUBE_BEFORE == stepState)
    {
        //get target position here
        RVPosition_t targetPos = GetRVTubePositionByStationID(m_CurProgramStepInfo.stationID);
        if(RV_UNDEF != targetPos)
        {
            LogDebug(QString("Move to RV tubeposition: %1").arg(targetPos));
            cmd->SetRVPosition(targetPos);
            m_SchedulerCommandProcessor->pushCmd(cmd);
        }
        else
        {
           //todo: error handling
           LogDebug(QString("Get invalid RV position: %1").arg(m_CurProgramStepInfo.stationID));
        }
    }
    else if(PSSM_READY_TO_TUBE_AFTER == stepState)
    {
        //get target position here
        RVPosition_t targetPos = GetRVTubePositionByStationID(m_CurProgramStepInfo.stationID);
        if(RV_UNDEF != targetPos)
        {
            LogDebug(QString("Move to RV tube position: %1").arg(targetPos));
            cmd->SetRVPosition(targetPos);
            m_SchedulerCommandProcessor->pushCmd(cmd);
        }
        else
        {
           //todo: error handling
           LogDebug(QString("Get invalid RV position: %1").arg(m_CurProgramStepInfo.stationID));
        }
    }
    else if(PSSM_READY_TO_SEAL == stepState)
    {
        //get target position here
        RVPosition_t targetPos = GetRVSealPositionByStationID(m_CurProgramStepInfo.stationID);
        if(RV_UNDEF != targetPos)
        {
            LogDebug(QString("Move to RV seal position: %1").arg(targetPos));
            cmd->SetRVPosition(targetPos);
            m_SchedulerCommandProcessor->pushCmd(cmd);
        }
        else
        {
           //todo: error handling
           LogDebug(QString("Get invalid RV position: %1").arg(m_CurProgramStepInfo.stationID));
        }
    }
}

void SchedulerMainThreadController::Fill()
{
    LogDebug("Send cmd to DCL to Fill");
    CmdALFilling* cmd  = new CmdALFilling(500, this);
    //todo: get delay time here
    cmd->SetDelayTime(2000);
    m_SchedulerCommandProcessor->pushCmd(cmd);

    // acknowledge to gui
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(5000,m_CurProgramStepInfo.stationID , true, true));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
}

void SchedulerMainThreadController::StopFill()
{
    // acknowledge to gui
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(5000,m_CurProgramStepInfo.stationID , false, true));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
}

void SchedulerMainThreadController::Soak()
{
    if(m_TimeStamps.CurStepSoakStartTime == 0)
    {
        m_TimeStamps.CurStepSoakStartTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
        LogDebug(QString("Start to soak, start time stamp is: %1").arg(m_TimeStamps.CurStepSoakStartTime));
    }
    if(m_CurProgramStepInfo.isPressure ^ m_CurProgramStepInfo.isVacuum)
    {
        if(m_CurProgramStepInfo.isPressure)
        {
            Pressure();
        }
        else
        {
            Vaccum();
        }
    }
}

void SchedulerMainThreadController::Drain()
{
    LogDebug("Send cmd to DCL to Drain");
    CmdALDraining* cmd  = new CmdALDraining(500, this);
    //todo: get delay time here
    cmd->SetDelayTime(2000);
    m_SchedulerCommandProcessor->pushCmd(cmd);

    // acknowledge to gui
    LogDebug("Notice GUI Draining started");
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(5000,m_CurProgramStepInfo.stationID , true, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
}

void SchedulerMainThreadController::StopDrain()
{
    // acknowledge to gui
    LogDebug("Notice GUI Draining stopped");
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(5000,m_CurProgramStepInfo.stationID , false, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
}

void SchedulerMainThreadController::Pressure()
{
    LogDebug("Send cmd to DCL to Pressure");
    m_SchedulerCommandProcessor->pushCmd(new CmdALPressure(500, this));
}

void SchedulerMainThreadController::Vaccum()
{
    LogDebug("Send cmd to DCL to Vaccum");
    m_SchedulerCommandProcessor->pushCmd(new CmdALVaccum(500, this));
}

void SchedulerMainThreadController::AllStop()
{
    LogDebug("Send cmd to DCL to ALLStop");
    m_SchedulerCommandProcessor->ALBreakAllOperation();
    m_SchedulerCommandProcessor->pushCmd(new CmdALAllStop(500, this));
}

void SchedulerMainThreadController::Pause()
{
    //send command to main controller to tell program is actually pasued
    LogDebug("Notice GUI program paused");
    MsgClasses::CmdProgramAcknowledge* commandPtrPauseFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_PAUSE_FINISHED));
    Q_ASSERT(commandPtrPauseFinish);
    Global::tRefType fRef = GetNewCommandRef();
    SendCommand(fRef, Global::CommandShPtr_t(commandPtrPauseFinish));
}

bool SchedulerMainThreadController::CheckStepTemperature()
{
    //todo: remove this when hardware is ready
    return true;
    qint32 targetTemp = m_CurProgramStepInfo.temperature;
    return ((targetTemp < m_TempRTBottom)&&(targetTemp < m_TempRTSide));
}

bool SchedulerMainThreadController::CheckLevelSensorTemperature(qreal targetTemperature)
{
    // get target temperature here
    return (m_TempALLevelSensor > targetTemperature);
}

void SchedulerMainThreadController::Abort()
{
    RVPosition_t targetPos = GetRVTubePositionByStationID(m_CurProgramStepInfo.stationID);
    if(RV_UNDEF != targetPos)
    {
        if((m_PositionRV != targetPos))
        {
            LogDebug(QString("Abouting program, send cmd to DCL to move RV to %1").arg(targetPos));
            CmdRVReqMoveToRVPosition* cmd = new CmdRVReqMoveToRVPosition(500, this);
            cmd->SetRVPosition(targetPos);
            m_SchedulerCommandProcessor->pushCmd(cmd);
        }
        else
        {
            if(m_SchedulerMachine->GetPreviousState() != PSSM_READY_TO_DRAIN)
            {
                this->Drain();
            }
            else
            {
                LogDebug("Already in draining process, abort will happen when draining finished.");
            }
        }
    }
}


RVPosition_t SchedulerMainThreadController::GetRVTubePositionByStationID(const QString stationID)
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

RVPosition_t SchedulerMainThreadController::GetRVSealPositionByStationID(const QString stationID)
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

bool SchedulerMainThreadController::SelfTest(ReturnCode_t RetCode)
{
    Q_UNUSED(RetCode);
#if 0
    bool retValue = false;
    SelfTestStateMachine_t selfTestState = mp_SelfTestStateMachine->GetCurrentState();
    if(SELF_TEST_INIT == selfTestState)
    {
        bool ok;
        bool goon = false;
		LOG_STR(STR_PROGRAM_SELFTEST_START);
        LOG_STR(STR_PROGRAM_SELFTEST_CHECK_TEMP_PRESSURE);
        // check oven heat time
        qreal parameter = mp_DataManager->GetProgramSettings()->GetParameterValue( "Oven", "oven heating",  "HeatingOvertime", ok);
        if(ok)
        {
             goon |= (GetOvenHeatingTime() > parameter);
        }

        // check temperature
        parameter = mp_DataManager->GetProgramSettings()->GetParameterValue( "Rotary Valve", "Heating",  "TemperatureOverrange", ok);
        if(ok)
        {
             goon |= (m_TempRV1 < parameter);
             goon |= (m_TempRV2 < parameter);
        }
        parameter = mp_DataManager->GetProgramSettings()->GetParameterValue( "Oven", "oven heating",  "TemperatureOverrange", ok);
        if(ok)
        {
             goon |= (m_TempOvenTop < parameter);
             goon |= (m_TempOvenBottom < parameter);
        }
        parameter = mp_DataManager->GetProgramSettings()->GetParameterValue( "Retort", "Retort heating",  "TemperatureOverrange", ok);
        if(ok)
        {
             goon |= (m_TempRTBottom < parameter);
             goon |= (m_TempRTSide < parameter);
        }
        parameter = mp_DataManager->GetProgramSettings()->GetParameterValue( "Retort", "level sensor",  "TemperatureOverrange", ok);
        if(ok)
        {
             goon |= (m_TempALLevelSensor < parameter);
        }
        parameter = mp_DataManager->GetProgramSettings()->GetParameterValue( "LA", "HeatingOfRetortToRVTube",  "TemperatureOverrange", ok);
        if(ok)
        {
             goon |= (m_TempALTube1 < parameter);
        }
        parameter = mp_DataManager->GetProgramSettings()->GetParameterValue( "LA", "HeatingOfRetortToRVWaxTrap",  "TemperatureOverrange", ok);
        if(ok)
        {
             goon |= (m_TempALTube2 < parameter);
        }
        // check pressure
        parameter = mp_DataManager->GetProgramSettings()->GetParameterValue( "LA", "Release",  "AmbientThresholdPressure", ok);
        if(ok)
        {
             goon |= (m_PressureAL < parameter);
             goon |= (m_PressureAL > (parameter * (-1)));
        }
        // check Rotary valve's position
        goon |= (m_PositionRV != RV_UNDEF);

        if(goon)
        {
            mp_SelfTestStateMachine->NotifyTempsReady();
        }
        else
        {
            //todo: raise error
        }
    }
    else if(SELF_TEST_TEMP_READY == selfTestState)
    {
        ProgramStationInfo_t stationInfo = m_ProgramStationList.dequeue();
        RVPosition_t tubePos = GetRVTubePositionByStationID(stationInfo.StationID);
        QString reagentGrpId = stationInfo.ReagentGroupID;

        CmdIDBottleCheck* cmd  = new CmdIDBottleCheck(500, mp_IDeviceProcessing, this);
        //todo: get delay time here
        cmd->SetReagentGrpID(reagentGrpId);
        cmd->SetTubePos(tubePos);
        m_SchedulerCommandProcessor->pushCmd(cmd);
        mp_SelfTestStateMachine->NotifyCheckStation();

        LOG_STR_ARG(STR_PROGRAM_SELFTEST_CHECK_BOTTLE, Global::FmtArgs()<<stationInfo.StationID);

    }
    else if(SELF_TEST_BOTTLE_CHECKING == selfTestState)
    {
        quint32 resid = STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_UNEXPECTED;
        if( DCL_ERR_DEV_BOTTLE_CHECK_OK == RetCode)
        {
            mp_SelfTestStateMachine->NotifyGotCheckStationResult();
            resid = STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_OK;
        }
        else if( DCL_ERR_DEV_BOTTLE_CHECK_NOT_FULL == RetCode)
        {
            mp_SelfTestStateMachine->NotifyGotCheckStationResult();
            resid = STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_NOT_FULL;
        }
        else if( DCL_ERR_DEV_LA_BOTTLECHECK_FAILED_BLOCKAGE == RetCode)
        {
            mp_SelfTestStateMachine->NotifyGotCheckStationResult();
            resid = STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_BLOCKAGE;
        }
        else if(DCL_ERR_DEV_LA_BOTTLECHECK_FAILED_EMPTY == RetCode)
        {
            mp_SelfTestStateMachine->NotifyGotCheckStationResult();
            resid = STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_EMPTY;
        }
        else if(DCL_ERR_DEV_BOTTLE_CHECK_ERROR == RetCode)
        {
            mp_SelfTestStateMachine->NotifyGotCheckStationResult();
            resid = STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_ERROR;
        }
        else if(DCL_ERR_DEV_LA_BOTTLECHECK_PRESSUREBUILD_FAILED == RetCode)
        {
            mp_SelfTestStateMachine->NotifyGotCheckStationResult();
            resid = STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_TIMEOUT;
        }
        else if(DCL_ERR_UNDEFINED != RetCode)
        {
            qDebug()<<"DBG" << "Unexpected ret code: "<< RetCode;
            resid = STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT_UNEXPECTED;
        }
        LOG_STR_ARG(STR_PROGRAM_SELFTEST_BOTTLE_CHECK_RESULT, Global::tTranslatableStringList()<<Global::TranslatableString(resid));
    }
    else if(SELF_TEST_BOTTLE_CHECK_FINISH == selfTestState)
    {
        if(m_ProgramStationList.size() > 0)
        {
            ProgramStationInfo_t stationInfo = m_ProgramStationList.dequeue();
            RVPosition_t tubePos = GetRVTubePositionByStationID(stationInfo.StationID);
            QString reagentGrpId = stationInfo.ReagentGroupID;

            CmdIDBottleCheck* cmd  = new CmdIDBottleCheck(500, mp_IDeviceProcessing, this);
            cmd->SetReagentGrpID(reagentGrpId);
            cmd->SetTubePos(tubePos);
            m_SchedulerCommandProcessor->pushCmd(cmd);
            mp_SelfTestStateMachine->NotifyCheckStation();
        }
        else
        {
            mp_SelfTestStateMachine->NotifyCheckStaionFinished();
            LOG_STR(STR_PROGRAM_SELFTEST_FINISH);
        }
    }
    else if(SELF_TEST_FINISH == selfTestState)
    {
        retValue = true;
    }
    else
    {
        //should not get here
        qDebug()<<"DBG"<<"error when run Selftest state machine";
    }

    return retValue;
#endif
    return true;
}

qint64 SchedulerMainThreadController::GetOvenHeatingTime()
{
    if(m_TimeStamps.OvenStartHeatingTime != 0)
    {
        return  (QDateTime::currentDateTime().toMSecsSinceEpoch() - m_TimeStamps.OvenStartHeatingTime)/1000;
    }
    else
    {
        return 0;
    }
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

bool SchedulerMainThreadController::CreateFunctionModuleStatusList(QList<FunctionModuleStatus_t>* pList)
{
    if(pList)
    {
        FunctionModuleStatus_t fmStatus;
        fmStatus.IsAvialable = true;
        fmStatus.IsWorking = false;
        fmStatus.StartTime = 0;
        fmStatus.StopTime = 0;
        fmStatus.FctModID = CANObjectKeyLUT::FCTMOD_PER_MAINRELAYDO;
        pList->push_back(fmStatus);
        fmStatus.FctModID = CANObjectKeyLUT::FCTMOD_RV_MOTOR;
        pList->push_back(fmStatus);
        fmStatus.FctModID = CANObjectKeyLUT::FCTMOD_RV_TEMPCONTROL;
        pList->push_back(fmStatus);
        fmStatus.FctModID = CANObjectKeyLUT::FCTMOD_AL_PRESSURECTRL;
        pList->push_back(fmStatus);
        fmStatus.FctModID = CANObjectKeyLUT::FCTMOD_AL_LEVELSENSORTEMPCTRL;
        pList->push_back(fmStatus);
        fmStatus.FctModID = CANObjectKeyLUT::FCTMOD_AL_TUBE1TEMPCTRL;
        pList->push_back(fmStatus);
        fmStatus.FctModID = CANObjectKeyLUT::FCTMOD_AL_TUBE2TEMPCTRL;
        pList->push_back(fmStatus);
        fmStatus.FctModID = CANObjectKeyLUT::FCTMOD_AL_FANDO;
        pList->push_back(fmStatus);
        fmStatus.FctModID = CANObjectKeyLUT::FCTMOD_OVEN_TOPTEMPCTRL;
        pList->push_back(fmStatus);
        fmStatus.FctModID = CANObjectKeyLUT::FCTMOD_OVEN_BOTTOMTEMPCTRL;
        pList->push_back(fmStatus);
        fmStatus.FctModID = CANObjectKeyLUT::FCTMOD_OVEN_LIDDI;
        pList->push_back(fmStatus);
        fmStatus.FctModID = CANObjectKeyLUT::FCTMOD_RETORT_BOTTOMTEMPCTRL;
        pList->push_back(fmStatus);
        fmStatus.FctModID = CANObjectKeyLUT::FCTMOD_RETORT_SIDETEMPCTRL;
        pList->push_back(fmStatus);
        fmStatus.FctModID = CANObjectKeyLUT::FCTMOD_RETORT_LOCKDO;
        pList->push_back(fmStatus);
        fmStatus.FctModID = CANObjectKeyLUT::FCTMOD_RETORT_LOCKDI;
        pList->push_back(fmStatus);
        fmStatus.FctModID = CANObjectKeyLUT::FCTMOD_PER_REMOTEALARMCTRLDO;
        pList->push_back(fmStatus);
        fmStatus.FctModID = CANObjectKeyLUT::FCTMOD_PER_LOCALALARMCTRLDO;
        pList->push_back(fmStatus);
        return true;
    }
    else
    {
        return false;
    }
}

bool SchedulerMainThreadController::SetFunctionModuleWork(QList<FunctionModuleStatus_t>* pList, CANObjectKeyLUT::CANObjectIdentifier_t ID, bool isWorking)
{
    if(pList)
    {
        QListIterator<FunctionModuleStatus_t> iter(*pList);
        quint32 idx = 0;
        FunctionModuleStatus_t fmStatus;
        while(iter.hasNext())
        {
            fmStatus = iter.next();
            if(fmStatus.FctModID == ID)
            {
                fmStatus.IsWorking = isWorking;
                if(isWorking)
                {
                    fmStatus.StartTime = QDateTime::currentMSecsSinceEpoch();
                }
                else
                {
                    fmStatus.StopTime = QDateTime::currentMSecsSinceEpoch();
                }
                (*pList)[idx] = fmStatus;
                return true;
            }
            idx++;
        }
    }
    return false;
}

bool SchedulerMainThreadController::SetFunctionModuleHealth(QList<FunctionModuleStatus_t>* pList, CANObjectKeyLUT::CANObjectIdentifier_t ID, bool isHealth)
{
    if(pList)
    {
        QListIterator<FunctionModuleStatus_t> iter(*pList);
        quint32 idx = 0;
        FunctionModuleStatus_t fmStatus;
        while(iter.hasNext())
        {
            fmStatus = iter.next();
            if(fmStatus.FctModID == ID)
            {
                fmStatus.IsHealth = isHealth;
                (*pList)[idx] = fmStatus;
                return true;
            }
            idx++;
        }
    }
    return false;
}

bool SchedulerMainThreadController::SetFunctionModuleStarttime(QList<FunctionModuleStatus_t>* pList, CANObjectKeyLUT::CANObjectIdentifier_t ID)
{
    if(pList)
    {
        QListIterator<FunctionModuleStatus_t> iter(*pList);
        quint32 idx = 0;
        FunctionModuleStatus_t fmStatus;
        while(iter.hasNext())
        {
            fmStatus = iter.next();
            if(fmStatus.FctModID == ID)
            {
                fmStatus.StartTime = QDateTime::currentMSecsSinceEpoch();
                (*pList)[idx] = fmStatus;
                return true;
            }
            idx++;
        }
    }
    return false;
}

bool SchedulerMainThreadController::SetFunctionModuleStoptime(QList<FunctionModuleStatus_t>* pList, CANObjectKeyLUT::CANObjectIdentifier_t ID)
{
    if(pList)
    {
        QListIterator<FunctionModuleStatus_t> iter(*pList);
        quint32 idx = 0;
        FunctionModuleStatus_t fmStatus;
        while(iter.hasNext())
        {
            fmStatus = iter.next();
            if(fmStatus.FctModID == ID)
            {
                fmStatus.StopTime = QDateTime::currentMSecsSinceEpoch();
                (*pList)[idx] = fmStatus;
                return true;
            }
            idx++;
        }
    }
    return false;
}

QList<FunctionModuleStatus_t> SchedulerMainThreadController::GetFailedFunctionModuleList(QList<FunctionModuleStatus_t>* pList)
{
    //Failed: isWorking == true + isHealth == false
    QList<FunctionModuleStatus_t> unhealthFMList;
    if(pList)
    {
        QListIterator<FunctionModuleStatus_t> iter(*pList);
        FunctionModuleStatus_t fmStatus;
        while(iter.hasNext())
        {
            fmStatus = iter.next();
            if((fmStatus.IsWorking)&&(!fmStatus.IsHealth))
            {
                unhealthFMList.push_back(fmStatus);
            }
        }
    }
    return unhealthFMList;
}

qint64 SchedulerMainThreadController::GetFunctionModuleStartworkTime(QList<FunctionModuleStatus_t>* pList, CANObjectKeyLUT::CANObjectIdentifier_t ID)
{
    if(pList)
    {
        QListIterator<FunctionModuleStatus_t> iter(*pList);
        FunctionModuleStatus_t fmStatus;
        while(iter.hasNext())
        {
            fmStatus = iter.next();
            if(fmStatus.FctModID == ID)
            {
                return fmStatus.StartTime;
            }
        }
    }
    return 0;
}
}
