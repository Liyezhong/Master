// ================================================================================================
/*! @file Himalaya/HimalayaMain/Master/Components/Scheduler/Source/SchedulerMainThreadController.cpp
 *
 *  @brief
 *  Implementation of class Scheduler::SchedulerMainThreadController.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2018-03-06
 *  $Author:    $ Dixiong Li
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
#include "Scheduler/Include/SchedulerStateHandler.h"
#include "Scheduler/Include/SchedulerEventCodes.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToInitialPosition.h"
#include "Scheduler/Commands/Include/CmdALStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdALFilling.h"
#include "Scheduler/Commands/Include/CmdALDraining.h"
#include "Scheduler/Commands/Include/CmdIDForceDraining.h"
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
#include "float.h"
#include "Global/Include/EventObject.h"
#include "Scheduler/Include/HeatingStrategy.h"
#include <unistd.h>
#include <QtAlgorithms>
#include "Global/Include/SystemPaths.h"
#include "Global/Include/Utils.h"
#include "Global/Include/Commands/CmdShutDown.h"
#include <DataManager/CommandInterface/Include/UserSettingsCommandInterface.h>
#include <DataManager/Helper/Include/DataManagerEventCodes.h>
#include <Scheduler/Include/SchedulerMainThreadController.h>

using namespace DataManager;

namespace Scheduler{
CSchedulerStateHandler::CSchedulerStateHandler(QString RetortName, SchedulerMainThreadController* SchedulerThreadController, DataManager::CDataManager* dataManager):
    m_RetortName(RetortName)
  , mp_SchedulerThreadController(SchedulerThreadController)
  , mp_DataManager(dataManager)
  , m_CurReagnetName("")
  , m_CurProgramID("")
  , m_NewProgramID("")
  , m_delayTime(0)
  , m_ProcessCassetteCount(0)
  , m_ProcessCassetteNewCount(0)
  , m_CurProgramStepIndex(-1)
  , m_FirstProgramStepIndex(0)
  , m_Is5MinPause(false)
  , m_Is10MinPause(false)
  , m_Is15MinPause(false)
  , m_hasParaffin(false)
{
    memset(&m_TimeStamps, 0, sizeof(m_TimeStamps));
    memset(&m_CleaningDry, 0, sizeof(m_CleaningDry));

    ResetTheTimeParameter();

    m_CurrentScenario = 1;
    m_IsCleaningProgram = false;
    m_CurrentStepState = PSSM_INIT;

    m_SchedulerCommandProcessor = SchedulerThreadController->GetSchedCommandProcessor();

    m_SchedulerMachine = new CSchedulerStateMachine(SchedulerThreadController, this);
}

CSchedulerStateHandler::~CSchedulerStateHandler()
{
    try {
        if (m_SchedulerMachine != NULL)
            delete m_SchedulerMachine;
        m_SchedulerMachine = NULL;
    }
    CATCHALL_DTOR();
}

void CSchedulerStateHandler::ResetTheTimeParameter()
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

int CSchedulerStateHandler::WhichStepHasNoSafeReagent(const QString& ProgramID)
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
        bool ret = GetSafeReagentStationList(GetReagentGroupID(pProgramStep->GetReagentID()), false, true, stationList);
        if (ret && stationList.isEmpty())
        {
            return i;
        }
    }

    return -1;
}

void CSchedulerStateHandler::HandleStateCommand(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd)
{
    SchedulerStateMachine_t currentState = m_SchedulerMachine->GetCurrentState();
    m_SchedulerMachine->UpdateCurrentState(currentState);

    qDebug()<<"********** HandleStateCommand currentState:"<<currentState;
    qDebug()<<QTime::currentTime()<<"********** HandleStateCommand CommandType_t:"<<ctrlCmd;
    if (cmd != NULL)
    {
        qDebug()<<"********** HandleStateCommand Command retort name:"<<cmd->GetSender();
        qDebug()<<"********** HandleStateCommand Command name:"<<cmd->GetName();
    }


    SchedulerCommandShPtr_t sCmd;
    if (cmd != NULL && cmd->GetSender() == m_RetortName)
    {
        sCmd = cmd;
    }

    // We only handle Abort in Idle and Busy
    if(CTRL_CMD_ABORT == ctrlCmd && ((currentState & 0xFF) == SM_IDLE || (currentState & 0xFF) == SM_BUSY))
    {
        //tell main controller scheduler is starting to abort
        MsgClasses::CmdProgramAcknowledge* commandPtrAbortBegin(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_ABORT_BEGIN, m_RetortName));
        Q_ASSERT(commandPtrAbortBegin);
        Global::tRefType fRef = mp_SchedulerThreadController->GetNewCommandRef();
        mp_SchedulerThreadController->SendCommand(fRef, Global::CommandShPtr_t(commandPtrAbortBegin));
        m_TransitionPeriod = true;
        //SendOutErrMsg(DCL_ERR_DEV_INTER_ABORT);//the abort procedure is treated as an error
        return;
    }

    switch(currentState & 0xFF)
    {
    case SM_INIT:
        //In INIT state will do self test
        HardwareMonitor("INIT");
        HandleRmtLocAlarm(ctrlCmd);
        HandleInitState(ctrlCmd, sCmd);
        break;
    case SM_POWER_FAILURE:
        HandleRmtLocAlarm(ctrlCmd);
        HandlePowerFailure(ctrlCmd, sCmd);
        break;
    case SM_IDLE:
        qDebug()<<"DBG"<<"Scheduler main controller state: IDLE";
        HardwareMonitor( "IDLE" );
        HandleRmtLocAlarm(ctrlCmd);
        HandleIdleState(ctrlCmd,cmd);
        break;
    case SM_BUSY:
        qDebug()<<"DBG"<<"Scheduler main controller state: RUN";
        HardwareMonitor( m_CurProgramID );
        HandleRmtLocAlarm(ctrlCmd);
        HandleRunState(ctrlCmd, sCmd);
        break;
    case SM_ERROR:
        qDebug()<<"DBG"<<"Scheduler main controller state: ERROR";
        HardwareMonitor( "ERROR" );
        HandleRmtLocAlarm(ctrlCmd);
        HandleErrorState(ctrlCmd, sCmd, currentState);
        break;
    default:
        mp_SchedulerThreadController->LogDebug(QString("Scheduler main controller gets unexpected state: %1").arg(currentState));
    }
}

void CSchedulerStateHandler::ProgramSelectedReply(Global::tRefType Ref, const QString& ProgramID, int paraffinStepIndex)
{
    m_hasParaffin = false;
    GetNextProgramStepInformation(ProgramID, m_CurProgramStepInfo, true);//only to get m_CurProgramStepIndex
    ResetTheTimeParameter();

    unsigned int timeProposed = 0;
    unsigned int paraffinMeltCostedtime = 0;
    unsigned int costedTimeBeforeParaffin = 0;
    int whichStep = 0;
    if (m_CurProgramStepIndex != -1)
    {
        m_FirstProgramStepIndex = m_CurProgramStepIndex;
        PrepareProgramStationList(ProgramID, m_CurProgramStepIndex);
        timeProposed = GetLeftProgramStepsNeededTime(ProgramID);
        m_CurProgramStepIndex = -1;

        paraffinMeltCostedtime = mp_SchedulerThreadController->GetOvenHeatingTime();
        if (-1 != paraffinStepIndex)//has Paraffin
        {
            m_hasParaffin = true;
            int timeParaffinSteps = GetLeftProgramStepsNeededTime(ProgramID, paraffinStepIndex);
            costedTimeBeforeParaffin = timeProposed - timeParaffinSteps;
        }

        //cheack safe reagent
        whichStep = WhichStepHasNoSafeReagent(ProgramID);
    }
    else
    {
        m_StationList.clear();
    }

    m_ProcessCassetteCount = 0;
    m_ProcessCassetteNewCount = 0;
    //send back the proposed program end time
    MsgClasses::CmdProgramSelectedReply* commandPtr(new MsgClasses::CmdProgramSelectedReply(5000, m_RetortName, timeProposed,
                                                                                paraffinMeltCostedtime,
                                                                                costedTimeBeforeParaffin,
                                                                                whichStep,
                                                                                mp_SchedulerThreadController->GetSecondsForMeltingParaffin(),
                                                                                m_StationList,
                                                                                m_FirstProgramStepIndex));
    Q_ASSERT(commandPtr);
    mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
}

void CSchedulerStateHandler::HandleInitState(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd)
{
    Q_UNUSED(ctrlCmd)
    Q_UNUSED(cmd)
//    m_CurrentScenario = GetScenarioBySchedulerState(m_SchedulerMachine->GetCurrentState(), m_CurProgramStepInfo.reagentGroup);
//    ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;
//    QString cmdName = "";

//    /*lint -e58 */
//    if(cmd != NULL)
//    {
//        if(!(cmd->GetResult(retCode)))
//        {
//            retCode = DCL_ERR_UNDEFINED;
//        }
//        cmdName = cmd->GetName();
//    }

//    SchedulerStateMachine_t stepState = m_SchedulerMachine->GetCurrentState();
//    if(SM_INIT_SELFTEST == stepState)
//    {
//        m_SchedulerMachine->HandleSelfTestWorkFlow(cmdName, retCode);
//    }
    qDebug()<<"************* HandleInitState, ctrlcmd:"<<ctrlCmd;
    m_SchedulerMachine->SendSchedulerInitComplete();
}

void CSchedulerStateHandler::HandleIdleState(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd)
{
    m_CurrentStepState = SM_IDLE;
    Q_UNUSED(cmd)
    qDebug()<<"************* HandleIdleState, ctrlcmd:"<<ctrlCmd;
    if(m_IsWaitHeatingRV)
    {
        //TO do ---
        //PrepareForIdle(ctrlCmd, cmd);
        MsgClasses::CmdProgramAcknowledge* commandPtr(new MsgClasses::CmdProgramAcknowledge(5000, DataManager::PROGRAM_READY));
        Q_ASSERT(commandPtr);
        Global::tRefType Ref = mp_SchedulerThreadController->GetNewCommandRef();
        mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        m_IsWaitHeatingRV = false;

        //In this case, master SW will be in idle state
        mp_SchedulerThreadController->SendSystemBusy2GUI(false);
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
            MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_BEGIN, m_RetortName));
            Q_ASSERT(commandPtrFinish);
            Global::tRefType fRef = mp_SchedulerThreadController->GetNewCommandRef();
            mp_SchedulerThreadController->SendCommand(fRef, Global::CommandShPtr_t(commandPtrFinish));
            //SendProgramStart();

            if(mp_DataManager && mp_DataManager->GetProgramList() && mp_DataManager->GetProgramList()->GetProgram(m_CurProgramID))
            {
                const DataManager::CProgram* program = mp_DataManager->GetProgramList()->GetProgram(m_CurProgramID);
                if (!m_ReagentExpiredFlag.isEmpty())
                {
                    int index = m_ReagentExpiredFlag.indexOf(':');
                    QString reagentID = m_ReagentExpiredFlag.left(index);
                    QString stationID = m_ReagentExpiredFlag.mid(index + 1);
                    CReagent* reagent = mp_DataManager->GetReagentList()->GetReagent(reagentID);
                    Global::tTranslatableStringList  expiredTranslatableStringList;
                    QStringList expiredParamStringList;
                    if(program->IsLeicaProgram()) {
                        expiredTranslatableStringList << Global::TranslatableString(program->GetNameID().toUInt())
                                               << Global::TranslatableString(reagent->GetReagentNameID().toUInt())
                                               << stationID;
                        Global::EventObject::Instance().RaiseEvent(EVENT_SCHEDULER_START_PROGRAM_WITH_EXPIRED_REAGENT, expiredTranslatableStringList);
                    }
                    else {
                        expiredParamStringList << program->GetName() << reagent->GetReagentName() << stationID;
                        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_PROGRAM_WITH_EXPIRED_REAGENT, expiredParamStringList);
                    }
                }

                Global::tTranslatableStringList  translatableStringList;
                QStringList paramStringList;
                if(program->IsLeicaProgram()) {
                    translatableStringList << Global::TranslatableString(program->GetNameID().toUInt());
                }
                else {
                    paramStringList << program->GetName();
                }

                if(program->IsLeicaProgram())
                {
                    Global::EventObject::Instance().RaiseEvent(EVENT_SCHEDULER_START_PROGRAM, translatableStringList);
                }
                else
                {
                    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_PROGRAM, paramStringList);
                }
            }

            //send command to main controller to tell the left time
            MsgClasses::CmdCurrentProgramStepInfor* commandPtr(new MsgClasses::CmdCurrentProgramStepInfor(5000,
                                                                                                          m_RetortName,
                                                                                                          Global::UITranslator::TranslatorInstance().Translate(STR_SCHEDULER_PRECHECK),
                                                                                                          m_CurProgramStepIndex, m_EndTimeAndStepTime.PreTestTime));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = mp_SchedulerThreadController->GetNewCommandRef();
            mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
            //SendLeftTime();

            // Set current step to Init
            m_CurrentStepState = PSSM_INIT;
            //mp_HeatingStrategy->Init260ParamList(true);

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

void CSchedulerStateHandler::HandlePowerFailure(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd)
{
    Q_UNUSED(ctrlCmd);
    Q_UNUSED(cmd);
#if 0
    if(POWERFAILURE_INIT == m_PowerFailureStep)
    {
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
            //In scenario 200 of Cleaning program, we treat it to scenario 281
            if (200 == scenario)
            {
                m_CurrentScenario = 281;
                scenario = 281;
            }
            else
            {
                m_CurrentScenario = scenario;
            }
        }
        else
        {
            m_CurrentScenario = scenario;
        }
        m_CurProgramID = curProgramID;
        m_FirstProgramStepIndex = 0;

        (void)this->PrepareProgramStationList(curProgramID, 0);

        m_CurProgramStepIndex = StepID - 1;
        (void)this->GetNextProgramStepInformation(curProgramID, m_CurProgramStepInfo);

        if(203 == scenario)
        {
            m_IsCleaningProgram = true;
            SendOutErrMsg(DCL_ERR_DEV_POWERFAILURE_CLEANING_MSGBOX, false);
        }
        else if(281 <= scenario && scenario <= 297)
        {
            m_IsCleaningProgram = true;
            if (283 != scenario && 285 != scenario && 293 != scenario && 295 != scenario)
            {
                SendOutErrMsg(DCL_ERR_DEV_POWERFAILURE_CLEANING_MSGBOX, false);
            }
        }
        m_PowerFailureStep = POWERFAILURE_RUN;
    }
    else if(POWERFAILURE_RUN == m_PowerFailureStep)
    {
        SendOutErrMsg(DCL_ERR_DEV_POWERFAILURE);
    }
#endif
}

void CSchedulerStateHandler::HandleRunState(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd)
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
                if (m_bWaitToPause || m_bWaitToPauseCmdYes)
                {
                     //dismiss the prompt of waiting for pause
                    //TO do --- SendProgramAcknowledge(DISMISS_PAUSING_MSG_DLG);
                    if (m_bWaitToPause)
                    {
                        m_bWaitToPause = false;
                        m_SchedulerMachine->UpdateCurrentState(PSSM_DRAINING);
                    }
                    if (m_bWaitToPauseCmdYes)
                    {
                        m_bWaitToPauseCmdYes = false;
                        m_SchedulerMachine->UpdateCurrentState(PSSM_FILLING_RVROD_HEATING);
                    }
                    mp_SchedulerThreadController->LogDebug(QString("Program Step Beginning Pause"));
                    m_SchedulerMachine->NotifyPause(SM_UNDEF);
                    return;
                }
                else
                {
                    m_SchedulerMachine->SendResumeRVPosChange();
                }
                break;
            case PSSM_RV_POS_CHANGE:
                m_SchedulerMachine->SendResumeRVPosChange();
                break;
            case PSSM_CLEANING_DRY_STEP:
                m_SchedulerMachine->SendResumeDryStep();
                break;
            case PSSM_POWERFAILURE_FINISH:
                if(m_ProgramStatusInfor.IsRetortContaminted())
                {
                    //send command to main controller to tell program finished
                    MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_FINISHED_AS_POWER_FAILURE, m_RetortName));
                    Q_ASSERT(commandPtrFinish);
                    Global::tRefType Ref = mp_SchedulerThreadController->GetNewCommandRef();
                    mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtrFinish));
                    //sendRunfinished..
                }
                else
                {
                    MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_FINISHED_AS_POWER_FAILURE_NO_CONTAMINATED
                                                                                                              , m_RetortName));
                    Q_ASSERT(commandPtrFinish);
                    Global::tRefType Ref = mp_SchedulerThreadController->GetNewCommandRef();
                    mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtrFinish));
                    m_ProgramStatusInfor.SetProgramFinished();
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
                //TO do ---
                // Stop heating level sensor
                //mp_HeatingStrategy->StopTemperatureControl("LevelSensor");
                mp_SchedulerThreadController->LogDebug(QString("Program Step Beginning Pause"));
                m_SchedulerMachine->NotifyPause(SM_UNDEF);
                return;
            }

            //TO do ---PressureAL
//            if(mp_HeatingStrategy->Check260SensorsTemp())
//            {
//                LogDebug("Program Step Heating Rotary Valve heating rod OK");
//                if (m_bWaitToPause)
//                {

//                }
//                m_SchedulerMachine->NotifyRVRodHeatingReady();
//            }
        }
        else
        {
            if(CTRL_CMD_PAUSE == ctrlCmd)
            {
                m_SchedulerMachine->NotifyPause(SM_UNDEF);
                return;
            }

//            m_SchedulerMachine->NotifyRVRodHeatingReady();
        }

        qDebug()<<"************************ NotifyRVRodHeatingReady";
        m_SchedulerMachine->NotifyRVRodHeatingReady();
    }
    else if (PSSM_FILLING_LEVELSENSOR_HEATING == stepState)
    {
        m_CurrentStepState = PSSM_FILLING_LEVELSENSOR_HEATING;

        if(CTRL_CMD_PAUSE == ctrlCmd)
        {
            //TO do ---
            // Stop heating level sensor
            //mp_HeatingStrategy->StopTemperatureControl("LevelSensor");
            mp_SchedulerThreadController->LogDebug(QString("Program Step Beginning Pause"));
            m_SchedulerMachine->NotifyPause(SM_UNDEF);
            return;
        }

        qDebug()<<"************************ NotifyLevelSensorHeatingReady";
        m_SchedulerMachine->NotifyLevelSensorHeatingReady();
//TO do ---
//        if(mp_HNotifyRVRodHeatingReadyeatingStrategy->CheckLevelSensorHeatingStatus())
//        {
//            //m_IsProcessing = false;
//            LogDebug("Program Step Heating Level sensor stage OK");
//            m_SchedulerMachine->NotifyLevelSensorHeatingReady();
//        }
//        else
//        {
//            // Do nothing, just wait for status of level sensor
//        }
    }
    else if(PSSM_FILLING == stepState)
    {
        m_CurrentStepState = PSSM_FILLING;

        if(CTRL_CMD_PAUSE == ctrlCmd)
        {
            mp_SchedulerThreadController->SendProgramAcknowledge(SHOW_PAUSE_MSG_DLG);
            m_bWaitToPause = true;
        }

        if (CTRL_CMD_RS_PAUSE_CMD_YES == ctrlCmd)
        {
            mp_SchedulerThreadController->SendProgramAcknowledge(SHOW_PAUSE_MSG_DLG);
            m_bWaitToPauseCmdYes = true;
        }

        if( "Scheduler::ALFilling" == cmdName)
        {
            qDebug()<<"************************ fill result:"<<retCode;
            if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_FILLING_SUCCESSFULLY);
                if (m_bWaitToPauseCmdYes)
                {
                    mp_SchedulerThreadController->LogDebug(QString("Program Step Beginning Pause"));
                    m_SchedulerMachine->NotifyDrain4Pause(SM_UNDEF);
                    return;
                }
                else
                {
                    qDebug()<<"************************ NotifyFillFinished";
                    m_SchedulerMachine->NotifyFillFinished();
                }
            }
            else
            {
                mp_SchedulerThreadController->LogDebug(QString("Program Step Filling failed"));
                mp_SchedulerThreadController->SendOutErrMsg(retCode);
            }
        }
    }
    else if(PSSM_RV_MOVE_TO_SEAL == stepState)
    {
        m_CurrentStepState = PSSM_RV_MOVE_TO_SEAL;

        if(CTRL_CMD_PAUSE == ctrlCmd)
        {
            mp_SchedulerThreadController->SendProgramAcknowledge(SHOW_PAUSE_MSG_DLG);
            m_bWaitToPause = true;
        }

        if (CTRL_CMD_RS_PAUSE_CMD_YES == ctrlCmd)
        {
            mp_SchedulerThreadController->SendProgramAcknowledge(SHOW_PAUSE_MSG_DLG);
            m_bWaitToPauseCmdYes = true;
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
                mp_SchedulerThreadController->LogDebug(QString("The step:%1 gap time:%2").arg(m_CurProgramStepIndex + 1).arg( m_EndTimeAndStepTime.GapTime));
            }

            if (m_bWaitToPause)
            {
                //dismiss the prompt of waiting for pause
                mp_SchedulerThreadController->SendProgramAcknowledge(DISMISS_PAUSING_MSG_DLG);
                m_bWaitToPause = false;
                mp_SchedulerThreadController->LogDebug(QString("Program Step Beginning Pause"));
                m_ProgramStatusInfor.SetScenario(m_CurrentScenario+1);
                m_SchedulerMachine->NotifyPause(SM_UNDEF);
                return;
            }

            if (m_bWaitToPauseCmdYes)
            {
                mp_SchedulerThreadController->LogDebug(QString("Program Step Beginning Pause"));
                m_SchedulerMachine->NotifyDrain4Pause(SM_UNDEF);
                return;
            }

            qDebug()<<"************************ NotifyRVMoveToSealReady";
            m_SchedulerMachine->NotifyRVMoveToSealReady();
        }
        else
        {
            if(("Scheduler::RVReqMoveToRVPosition" == cmdName))
            {
                if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
                {
                    mp_SchedulerThreadController->SendOutErrMsg(retCode);
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
            mp_SchedulerThreadController->LogDebug(QString("Program Step Beginning Pause"));
            m_SchedulerMachine->NotifyPause(PSSM_PROCESSING);
            return;
        }

        if(CTRL_CMD_RS_PAUSE_CMD_YES == ctrlCmd)
        {
            if(m_CurProgramStepInfo.isPressure || m_CurProgramStepInfo.isVacuum)
            {
                AllStop();
                m_lastPVTime = 0;
            }
            mp_SchedulerThreadController->SendProgramAcknowledge(SHOW_PAUSE_MSG_DLG);
            m_bWaitToPauseCmdYes = true;
            mp_SchedulerThreadController->LogDebug(QString("Program Step Beginning Drain Pause"));
            m_SchedulerMachine->NotifyDrain4Pause(PSSM_PROCESSING);
            return;
        }
        //DCR7157( TG-51 TG-54 )
        if (m_bWaitToPause)
        {
            //dismiss the prompt of waiting for pause
            mp_SchedulerThreadController->SendProgramAcknowledge(DISMISS_PAUSING_MSG_DLG);
            m_bWaitToPause = false;
            mp_SchedulerThreadController->LogDebug(QString("Program Step Beginning Pause"));
            m_SchedulerMachine->NotifyPause(SM_UNDEF);
            return;
        }

        if (m_bWaitToPauseCmdYes)
        {
            mp_SchedulerThreadController->LogDebug(QString("Program Step Beginning Pause"));
            m_SchedulerMachine->NotifyDrain4Pause(SM_UNDEF);
            return;
        }
        //end of DCR7157( TG-51 TG-54 )

        if( "RG6" == m_CurProgramStepInfo.reagentGroup && IsLastStep(m_CurProgramStepIndex, m_CurProgramID) )
        {
            if(m_EndTimeAndStepTime.WarningFlagForStepTime)
            {
                mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_LA_PROCESS_INTERVAL_TIMEOUT_4MIN, false);
                m_EndTimeAndStepTime.WarningFlagForStepTime = false;
            }
            else if(m_EndTimeAndStepTime.WarningFlagForBufferTime)
            {
                mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_LA_ENDTIME_TIMEOUT, false);
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
                m_SchedulerCommandProcessor->pushCmd(new CmdALReleasePressure(500,  m_RetortName));
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
                            MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_WILL_COMPLETE, m_RetortName));
                            Q_ASSERT(commandPtrFinish);
                            Global::tRefType fRef = mp_SchedulerThreadController->GetNewCommandRef();
                            mp_SchedulerThreadController->SendCommand(fRef, Global::CommandShPtr_t(commandPtrFinish));
                            m_completionNotifierSent = true;

                            // We also need start up alarm(local and device) to info user
                            Global::AlarmHandler::Instance().setAlarm(EVENTID_ALARM_FOR_DRAIN,Global::ALARM_INFO,true);
                            HandleRmtLocAlarm(CTRL_CMD_ALARM_LOC_ON);
                            HandleRmtLocAlarm(CTRL_CMD_ALARM_DEVICE_ON);
                        }
                    }
                    else
                    {
                        mp_SchedulerThreadController->LogDebug(QString("Program Processing(Soak) Process finished"));
                        m_SchedulerMachine->NotifyProcessingFinished();
                        m_TimeStamps.CurStepSoakStartTime = 0;
                        m_IsProcessing = false;
                    }
                }
                else
                {
                    mp_SchedulerThreadController->SendOutErrMsg(retCode);
                }
            }
            if(CTRL_CMD_DRAIN == ctrlCmd && m_completionNotifierSent)
            {
                mp_SchedulerThreadController->LogDebug(QString("last Program Processing(Soak) Process finished"));
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
                            mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_LA_SEALING_FAILED_PRESSURE, false);
                        }
                        else if (1 == m_ProcessingPV)
                        {
                            m_ProcessingPV = 3; // Avoid the warning message to pop up too many times
                            mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_LA_SEALING_FAILED_VACUUM, false);
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
            mp_SchedulerThreadController->SendProgramAcknowledge(SHOW_PAUSE_MSG_DLG);
            m_bWaitToPause = true;
        }
        qDebug()<<"********************** rv move to tube workflow";
        m_SchedulerMachine->HandlePssmMoveTubeWorkflow(cmdName, retCode);
    }
    else if(PSSM_DRAINING == stepState)
    {
        m_CurrentStepState = PSSM_DRAINING;

        if(CTRL_CMD_PAUSE == ctrlCmd)
        {
            mp_SchedulerThreadController->SendProgramAcknowledge(SHOW_PAUSE_MSG_DLG);
            m_bWaitToPause = true;
        }

        //In case that Scheduler was recovered from Error
        if( "Scheduler::ALDraining"== cmdName)
        {
            mp_SchedulerThreadController->LogDebug("ALDraining has been got in Scheduler");
            if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_DRAINING_SUCCESSFULLY);
                if (m_bWaitToPause || m_bWaitToPauseCmdYes)
                {
                     //dismiss the prompt of waiting for pause
                    mp_SchedulerThreadController->SendProgramAcknowledge(DISMISS_PAUSING_MSG_DLG);
                    m_bWaitToPause = false;
                    if (m_bWaitToPauseCmdYes)
                    {
                        m_bWaitToPauseCmdYes = false;
                        m_SchedulerMachine->UpdateCurrentState(PSSM_FILLING_RVROD_HEATING);
                    }
                    mp_SchedulerThreadController->LogDebug(QString("Program Step Beginning Pause"));
                    m_SchedulerMachine->NotifyPause(SM_UNDEF);
                    return;
                }
                m_SchedulerMachine->NotifyDrainFinished();
            }
            else
            {
                mp_SchedulerThreadController->LogDebug(QString("Program Step Draining Build Pressure timeout"));
                mp_SchedulerThreadController->SendOutErrMsg(retCode);
            }
        }
    }
    else if(PSSM_RV_POS_CHANGE == stepState)
    {
        m_CurrentStepState = PSSM_RV_POS_CHANGE;
        if(CTRL_CMD_PAUSE == ctrlCmd)
        {
            mp_SchedulerThreadController->SendProgramAcknowledge(SHOW_PAUSE_MSG_DLG);
            m_bWaitToPause = true;
        }

        if(IsRVRightPosition(NEXT_TUBE_POS))
        {
            if (m_bWaitToPause)
            {
                //dismiss the prompt of waiting for pause
                mp_SchedulerThreadController->SendProgramAcknowledge(DISMISS_PAUSING_MSG_DLG);
                m_bWaitToPause = false;
                mp_SchedulerThreadController->LogDebug(QString("Program Step Beginning Pause"));
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
                    mp_SchedulerThreadController->SendOutErrMsg(retCode);
                }
            }
        }
    }
    else if(PSSM_STEP_PROGRAM_FINISH == stepState)
    {
        m_CurrentStepState = PSSM_STEP_PROGRAM_FINISH;
        if (0 == m_PssmStepFinSeq)
        {
            //TO do --- m_UnknownErrorLogVector.clear();
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_PROGRAM_STEP_FINISHED,QStringList()<<QString("[%1]").arg(m_CurProgramStepIndex));
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
        if(mp_DataManager && mp_DataManager->GetProgramList() && mp_DataManager->GetProgramList()->GetProgram(m_CurProgramID))
        {
            const DataManager::CProgram* program = mp_DataManager->GetProgramList()->GetProgram(m_CurProgramID);
            if(program->IsLeicaProgram())
            {
                Global::EventObject::Instance().RaiseEvent(EVENT_SCHEDULER_PROGRAM_FINISHED, Global::tTranslatableStringList()
                                                           <<Global::TranslatableString(program->GetNameID().toUInt()));
            }
            else
            {
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_PROGRAM_FINISHED,QStringList()<<program->GetName());
            }
        }


        //send command to main controller to tell the left time
        MsgClasses::CmdCurrentProgramStepInfor* commandPtr(new MsgClasses::CmdCurrentProgramStepInfor(5000, m_RetortName, "", m_CurProgramStepIndex, 0));
        Q_ASSERT(commandPtr);
        Global::tRefType Ref = mp_SchedulerThreadController->GetNewCommandRef();
        mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

        if(m_ProgramStatusInfor.IsRetortContaminted())
        {
            //send command to main controller to tell program finished
            MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_FINISHED, m_RetortName));
            Q_ASSERT(commandPtrFinish);
            Ref = mp_SchedulerThreadController->GetNewCommandRef();
            mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtrFinish));
        }
        else
        {
            MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_FINISHED_NO_CONTAMINATED, m_RetortName));
            Q_ASSERT(commandPtrFinish);
            Ref = mp_SchedulerThreadController->GetNewCommandRef();
            mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtrFinish));
            m_ProgramStatusInfor.SetProgramFinished();
        }

        if(m_IsCleaningProgram)
        {
            m_IsTakeSpecimen = true;
            m_IsCleaningProgram = false;
            m_ProgramStatusInfor.SetProgramFinished();
        }

        m_SchedulerMachine->SendRunComplete();
    }
    else if(PSSM_PAUSE == stepState)
    {
        m_CurrentStepState = PSSM_PAUSE;
        if(CTRL_CMD_START == ctrlCmd)
        {
            m_IsResumeFromPause = true;

            // tell the main controller the program is resuming
            MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::SHOW_RESUME_MSG_DLG, m_RetortName));
            Q_ASSERT(commandPtrFinish);
            Global::tRefType fRef = mp_SchedulerThreadController->GetNewCommandRef();
            mp_SchedulerThreadController->SendCommand(fRef, Global::CommandShPtr_t(commandPtrFinish));

            MsgClasses::CmdProgramAcknowledge* commandPtrRunBegin(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_BEGIN, m_RetortName));
            Q_ASSERT(commandPtrRunBegin);
            fRef = mp_SchedulerThreadController->GetNewCommandRef();
            mp_SchedulerThreadController->SendCommand(fRef, Global::CommandShPtr_t(commandPtrRunBegin));

            // resume the program
            emit NotifyResume();
            mp_SchedulerThreadController->LogDebug("The program is resume from pasue.");
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
                MsgClasses::CmdProgramAcknowledge* commandPtrPauseEnable(new MsgClasses::CmdProgramAcknowledge(5000, type, m_RetortName));
                Q_ASSERT(commandPtrPauseEnable);
                Global::tRefType fRef = mp_SchedulerThreadController->GetNewCommandRef();
                mp_SchedulerThreadController->SendCommand(fRef, Global::CommandShPtr_t(commandPtrPauseEnable));

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

void CSchedulerStateHandler::HandleErrorState(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd, SchedulerStateMachine_t currentState)
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
        else if(CTRL_CMD_RC_FORCEDRAINING == ctrlCmd)
        {
            m_SchedulerMachine->EnterRcForceDraining();
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
            m_SchedulerMachine->EnterRcReHeating(m_ProgramStatusInfor.GetScenario(), m_ProgramStatusInfor.GetStationID(), false);
        }
        else if (CTRL_CMD_RC_REHEATING_5MINTIMEOUT == ctrlCmd)
        {
            m_SchedulerMachine->EnterRcReHeating(m_ProgramStatusInfor.GetScenario(), m_ProgramStatusInfor.GetStationID(), true);
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
    else if(SM_ERR_RC_FORCEDRAINING == currentState)
    {
         m_SchedulerMachine->HandleForceDrainingWorkFlow(cmdName, retCode);
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
        //To do ---
        //mp_HeatingStrategy->SetHeatingStrategyScenario(0);
    }
    else if (SM_ERR_RS_ABORT == currentState)
    {
        m_SchedulerMachine->HandleRsAbortWorkFlow(cmdName, retCode, ctrlCmd);
    }
}

void CSchedulerStateHandler::DoCleaningDryStep(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd)
{
    Q_UNUSED(ctrlCmd)
    ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;
    CmdRVReqMoveToRVPosition* CmdMvRV = NULL;
    MsgClasses::CmdCurrentProgramStepInfor* commandPtr = NULL;
    Global::tRefType Ref;

    switch (m_CleaningDry.CurrentState)
    {
    case CDS_READY:
        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_DRY_PROCESSING);
        commandPtr = new MsgClasses::CmdCurrentProgramStepInfor(5000, m_RetortName, Global::UITranslator::TranslatorInstance().Translate(STR_SCHEDULER_DRY_PROCESSING),
                                                                m_CurProgramStepIndex, TIME_FOR_CLEANING_DRY_STEP + TIME_FOR_COOLING_DOWN);
        Q_ASSERT(commandPtr);
        Ref = mp_SchedulerThreadController->GetNewCommandRef();
        mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        m_CleaningDry.CurrentState = CDS_MOVE_TO_SEALING_13;
        break;
    case CDS_MOVE_TO_SEALING_13:
        CmdMvRV = new CmdRVReqMoveToRVPosition(500, m_RetortName);
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
            else if (DCL_ERR_DEV_RV_MOTOR_LOSTCURRENTPOSITION == retCode)
            {
                m_SchedulerCommandProcessor->pushCmd(new CmdRVReqMoveToInitialPosition(500, m_RetortName));
                m_CleaningDry.CurrentState = CDS_MOVE_TO_INIT_POS;
            }
            else
            {
                m_CleaningDry.CurrentState = CDS_READY;
                m_CleaningDry.StepStartTime = 0;
                mp_SchedulerThreadController->SendOutErrMsg(retCode);
            }
        }
        break;
    case CDS_MOVE_TO_INIT_POS:
        if (cmd != NULL && ("Scheduler::RVReqMoveToInitialPosition" == cmd->GetName()))
        {
            if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
            {
                mp_SchedulerThreadController->SendOutErrMsg(retCode);
            }
            else
            {
                m_CleaningDry.CurrentState = CDS_MOVE_TO_SEALING_13;
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
                mp_SchedulerThreadController->SendOutErrMsg(retCode);
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
        mp_SchedulerThreadController->ReleasePressure(m_RetortName);
        m_CleaningDry.CurrentState = CDS_STOP_HEATING;
        break;
    case CDS_STOP_HEATING:
        //mp_HeatingStrategy->StopTemperatureControl("RTSide");
        //mp_HeatingStrategy->StopTemperatureControl("RTBottom");
        mp_SchedulerThreadController->RaiseEvent(Event_SCHEDULER_COOLING_DOWN);
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
            MsgClasses::CmdProgramAcknowledge* CmdCoolingDownFaild = new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_COOLING_DWON, m_RetortName);
            Q_ASSERT(CmdCoolingDownFaild);
            Global::tRefType fRef = mp_SchedulerThreadController->GetNewCommandRef();
            mp_SchedulerThreadController->SendCommand(fRef, Global::CommandShPtr_t(CmdCoolingDownFaild));
            m_CleaningDry.CurrentState = CDS_MOVE_TO_TUBE_13;
        }
        break;
    case CDS_MOVE_TO_TUBE_13:
        CmdMvRV = new CmdRVReqMoveToRVPosition(500, m_RetortName);
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
                mp_SchedulerThreadController->SendOutErrMsg(retCode);
            }
        }
        break;
    case CDS_SUCCESS:
        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_FINISHED_DRY_PROCESSING);
        m_SchedulerMachine->NotifyProgramFinished();
        m_CleaningDry.CurrentState = CDS_READY;
        m_CleaningDry.StepStartTime = 0;
        break;
    case CDS_ERROR:
        break;
    }
}

void CSchedulerStateHandler::AllStop()
{
    mp_SchedulerThreadController->LogDebug("Send cmd to DCL to ALLStop");
    (void)m_SchedulerCommandProcessor->ALBreakAllOperation();
    m_SchedulerCommandProcessor->pushCmd(new CmdALAllStop(500, m_RetortName));
}

bool CSchedulerStateHandler::IsRVRightPosition(RVPosition_type type)
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
            mp_SchedulerThreadController->LogDebug(QString("RV hit seal position: %1").arg(targetPos));
        }
        else
        {
            mp_SchedulerThreadController->LogDebug(QString("RV hit tube position: %1").arg(targetPos));
        }
        m_ProgramStatusInfor.SetLastRVPosition(targetPos);
    }
    return ret;
}

RVPosition_t CSchedulerStateHandler::GetRVTubePositionByStationID(const QString& stationID)
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

RVPosition_t CSchedulerStateHandler::GetRVSealPositionByStationID(const QString& stationID)
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

void CSchedulerStateHandler::CalculateTheGapTimeAndBufferTime(bool IsStartTime, bool IsEndTime)
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
                mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_FOR_WARNING_FLAG, QStringList()<<QString("[%1]").arg(m_CurProgramStepIndex)<<QString("[%1]").arg(m_CurProgramStepIndex+1));
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
        mp_SchedulerThreadController->LogDebug(QString("The step:%1 gap time:%2 millisecond.").arg(m_CurProgramStepIndex + 1).arg( m_EndTimeAndStepTime.GapTime));

        if(IsLastStep(m_CurProgramStepIndex, m_CurProgramID))
        {
            m_EndTimeAndStepTime.BufferTime = m_EndTimeAndStepTime.UserSetEndTime - QDateTime::currentMSecsSinceEpoch() - m_EndTimeAndStepTime.LastParaffinProcessingTime * 1000;
            if(m_EndTimeAndStepTime.BufferTime < 0) // delay
            {
                if(m_EndTimeAndStepTime.BufferTime < -300000)// warning if the delay time is more than 5 minutes.
                {
                    m_EndTimeAndStepTime.WarningFlagForBufferTime = true;
                    mp_SchedulerThreadController->LogDebug(QString("The program will be completed %1 millisecond later").arg(m_EndTimeAndStepTime.BufferTime));
                }
                m_EndTimeAndStepTime.BufferTime = 0;
            }
            else if(m_EndTimeAndStepTime.BufferTime > 0)//finish in advance
            {
                // after the 10% compensation with paraffin,  the advanced time is still more than 5 minutes; then rise warning message.
                if(m_EndTimeAndStepTime.BufferTime > m_EndTimeAndStepTime.TotalParaffinProcessingTime * 1000 * 0.1  + 300000)
                {
                    m_EndTimeAndStepTime.WarningFlagForBufferTime = true;
                    mp_SchedulerThreadController->LogDebug(QString("The program will be completed %1 millisecond in advance.")
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

void CSchedulerStateHandler::HandleRmtLocAlarm(quint32 ctrlcmd)
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
    case CTRL_CMD_ALARM_DEVICE_ON:
        opcode = 1;
        break;
    case CTRL_CMD_ALARM_DEVICE_OFF:
        opcode = 0;
        break;
    case CTRL_CMD_ALARM_ALL_OFF:
        opcode = -1;
        break;
    case CTRL_CMD_POWER_FAILURE_MEG:
    case CTRL_CMD_DRAIN_SR:
        Global::AlarmHandler::Instance().reset();
        opcode = -1;
        break;
    case CTRL_CMD_DRAIN:
        Global::AlarmHandler::Instance().setAlarm(EVENTID_ALARM_FOR_DRAIN,Global::ALARM_INFO,false);
        opcode = -1;
        break;
    default:
        return ;
    }

    CmdRmtLocAlarm *cmd = new CmdRmtLocAlarm(500, m_RetortName);
    cmd->SetRmtLocOpcode(opcode);
    m_SchedulerCommandProcessor->pushCmd(cmd);
}

void CSchedulerStateHandler::Pressure()
{
    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_SET_PRESSURE,QStringList()<<QString("[%1]").arg(AL_TARGET_PRESSURE_POSITIVE));
    m_SchedulerCommandProcessor->pushCmd(new CmdALPressure(500, m_RetortName));

    mp_SchedulerThreadController->StartTimer();
}

void CSchedulerStateHandler::HighPressure()
{

    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_SET_PRESSURE, QStringList()<<QString("[%1]").arg(40));
    CmdALPressure* cmd = new CmdALPressure(500, m_RetortName);
    cmd->SetTargetPressure(40.0);
    m_SchedulerCommandProcessor->pushCmd(cmd);

    mp_SchedulerThreadController->StartTimer();
}

void CSchedulerStateHandler::Vaccum()
{

    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_SET_PRESSURE,QStringList()<<QString("[%1]").arg(AL_TARGET_PRESSURE_NEGATIVE));
    m_SchedulerCommandProcessor->pushCmd(new CmdALVaccum(500, "0"));

    mp_SchedulerThreadController->StartTimer();
}

int CSchedulerStateHandler::CurStepIndexForSafeReagent()
{
    if (0 == m_CurProgramStepIndex)
        return 0;

    int curProgramStepIndex = m_CurProgramStepIndex;
    if (211 == m_CurrentScenario || 221 == m_CurrentScenario || 231 == m_CurrentScenario ||
           241 == m_CurrentScenario || 251 == m_CurrentScenario || 260 == m_CurrentScenario ||
            271 == m_CurrentScenario)
    {
       curProgramStepIndex--;
    }
    return curProgramStepIndex;
}

bool CSchedulerStateHandler::IsFirstTimeUseCurReagent(const QString& reagentGroupID)
{
    if (m_CurProgramID.isEmpty() || m_CurProgramID.isNull())
    {
        return true;
    }

    if (-1 == m_CurProgramStepIndex)
    {
        return true;
    }

    if (!mp_DataManager)
    {
        Q_ASSERT(false);
        return true;
    }

    CDataProgramList* pDataProgramList = mp_DataManager->GetProgramList();
    if (!pDataProgramList)
    {
        Q_ASSERT(false);
        return true;
    }

    CProgram* pProgram = const_cast<CProgram*>(pDataProgramList->GetProgram(m_CurProgramID));
    int curProgramStepIndex = CurStepIndexForSafeReagent();
    for (int i = 0; i < curProgramStepIndex; i++)
    {
        const CProgramStep* pProgramStep = pProgram->GetProgramStep(i);//use order index
        QString reagentID = pProgramStep->GetReagentID();
        if (reagentGroupID == GetReagentGroupID(reagentID))
        {
            mp_SchedulerThreadController->LogDebug(QString("In IsFirstTimeUseCurReagent(), return true"));
            return false;
        }
    }
    return true;
}

bool CSchedulerStateHandler::GetNextProgramStepInformation(const QString& ProgramID,
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

void CSchedulerStateHandler::OnReportFillingTimeOut2Min()
{
    QString ReagentGroup = m_CurProgramStepInfo.reagentGroup;
    quint32 Scenario = GetScenarioBySchedulerState(m_SchedulerMachine->GetCurrentState(),ReagentGroup);
    if (0 != Scenario)
    {
        mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_LA_FILLING_TIMEOUT_2MIN, false);
    }
}

void CSchedulerStateHandler::OnReportDrainingTimeOut2Min()
{
    QString ReagentGroup = m_CurProgramStepInfo.reagentGroup;
    quint32 Scenario = GetScenarioBySchedulerState(m_SchedulerMachine->GetCurrentState(),ReagentGroup);
    if (0 != Scenario)
    {
        mp_SchedulerThreadController->SendOutErrMsg(DCL_ERR_DEV_LA_DRAINING_TIMEOUT_EMPTY_2MIN, false);
    }
}

bool CSchedulerStateHandler::PrepareProgramStationList(const QString& ProgramID, int beginStep)
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
        stationInfo.ReagentGroupID = GetReagentGroupID(reagentID);
        stationInfo.StationID = this->SelectStationFromReagentID(reagentID, unusedStationIDs, usedStations, isLastStep);
        m_StationList.push_back(stationInfo.StationID);
        m_StationAndReagentList.push_back(stationInfo);
    }
    return true;
}

bool CSchedulerStateHandler::GetSafeReagentStationList(const QString& curReagentGroupID, bool excludeCurStation, bool firstTimeUseReagent, QList<QString>& stationList)
{
    CDataReagentList* pReagentList =  mp_DataManager->GetReagentList();
    if (!pReagentList)
        return false;

    CDashboardDataStationList* pDashboardDataStationList = mp_DataManager->GetStationList();
    if (!pDashboardDataStationList)
        return false;

    QString specifiedReagentGroup = GetSafeReagentType(curReagentGroupID, firstTimeUseReagent);
    if (specifiedReagentGroup == Global::DehydratingDilutedFixation){ //DehydratingAbsolute, firstTime use
        //Firstly use DehydratingDiluted,
        //if no DehydratingDiluted, try Fixation
        GetSpecifiedStations(Global::DehydratingDiluted, stationList);
        if (stationList.empty()){
            GetSpecifiedStations(Global::FixationReagent, stationList);
        }
    }
    else if (specifiedReagentGroup == Global::FixationDehydratingDiluted){//DehydratingDiluted, firstTime use
        //if no Fixation, try itself
         GetSpecifiedStations(Global::FixationReagent, stationList);
         if (stationList.empty()){
             GetSpecifiedStations(Global::DehydratingDiluted, stationList);
             if (stationList.size() > 0){
                 QString firstStation = stationList.at(0);
                 if (!firstStation.isEmpty()){
                     stationList.clear();
                     stationList.push_back(firstStation);
                 }
             }
         }
    }
    else
    {
        int curProgramStepIndex = CurStepIndexForSafeReagent();
        GetSpecifiedStations(specifiedReagentGroup, stationList);
        if ((specifiedReagentGroup == Global::DehydratingDiluted) && (stationList.size() > 0)){//it is second time to use DehydratingDiluted
            stationList = stationList.mid(0, curProgramStepIndex - m_StationList.indexOf(stationList.at(0)));
        }
        else if (excludeCurStation)
        {
            QString curStationID;
            if (m_CurProgramStepIndex >= 0){
                curStationID = m_StationList[curProgramStepIndex];
            }

            if (stationList.size() > 1)//if only one, it will not be excluded
            {
                if (specifiedReagentGroup == Global::ParaffinReagent || specifiedReagentGroup == Global::ClearingReagent ||
                        specifiedReagentGroup == Global::FixationReagent || specifiedReagentGroup == Global::DehydratingAbsolute)
                {
                    stationList.removeOne(curStationID);
                }
            }
        }
    }

    return true;
}

void CSchedulerStateHandler::GetSpecifiedStations(const QString& specifiedReagentGroup, QList<QString>& stationList)
{
    CDataReagentList* pReagentList =  mp_DataManager->GetReagentList();
      if (!pReagentList)
          return;

    CDashboardDataStationList* pDashboardDataStationList = mp_DataManager->GetStationList();
      if (!pDashboardDataStationList)
          return;

    QList<QString> stations(m_StationList);
    //loop stations of the current program
    for (int i = 0; i < stations.count(); i++)
    {
        CDashboardStation* pDashboardStation = pDashboardDataStationList->GetDashboardStation(stations.at(i));
        if (!pDashboardStation)
            continue;

        const CReagent* pReagent = pReagentList->GetReagent(pDashboardStation->GetDashboardReagentID());
        if (!pReagent)
            continue;

        if (pDashboardStation->GetDashboardReagentStatus() == "Empty")
            continue;

        if (CReagentGroup* reagentGroup = mp_DataManager->GetReagentGroupList()->GetReagentGroup(pReagent->GetGroupID()))
        {
            if (reagentGroup->GetGroupID() == specifiedReagentGroup)
            {
                stationList.append(pDashboardStation->GetDashboardStationID());//use same reagent group
            }
        }
    }// end of for
}

QString CSchedulerStateHandler::SelectStationFromReagentID(const QString& ReagentID, ListOfIDs_t& unusedStationIDs,
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

bool CSchedulerStateHandler::IsCleaningReagent(const QString& ReagentID)
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

QString CSchedulerStateHandler::SelectStationByReagent(const CReagent* pReagent,
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

bool CSchedulerStateHandler::GetSafeReagentForSpecial(int index, QString& reagentGroupID, QList<QString>& stationList)
{
    if(index >= m_StationAndReagentList.size())
        return false;
    ProgramStationInfo_t stationInfo = m_StationAndReagentList.at(index);
    reagentGroupID = stationInfo.ReagentGroupID;
    stationList.push_back(stationInfo.StationID);
    return true;
}

quint32 CSchedulerStateHandler::GetLeftProgramStepsNeededTime(const QString& ProgramID, int BeginProgramStepID)
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

quint32 CSchedulerStateHandler::GetPreTestTime()
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

quint32 CSchedulerStateHandler::GetCurrentProgramStepNeededTime(const QString& ProgramID)
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

void CSchedulerStateHandler::UpdateStationReagentStatus(bool bOnlyNew)
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
        Global::tRefType Ref = mp_SchedulerThreadController->GetNewCommandRef();
        mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
    }
}

void CSchedulerStateHandler::OnEnterPssmProcessing()
{
    // Update current sceanrio
//    this->UpdateCurrentScenario();
//    CheckResuemFromPause(PSSM_PROCESSING);
    m_IsReleasePressureOfSoakFinish = false;
    // We only release pressure if neither P or V exists.
    if(!m_IsProcessing)
    {
        m_IsProcessing = true;
        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_PROCESSING);
        if (!m_CurProgramStepInfo.isPressure && !m_CurProgramStepInfo.isVacuum)
        {
            mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_RELEASE_PREASURE);
            m_SchedulerCommandProcessor->pushCmd(new CmdALReleasePressure(500,  m_RetortName));
        }
        if(0 == m_CurProgramStepIndex)
        {
            //the first reagent should need delay time
            if(m_IsFirstProcessingForDelay)
            {
                m_IsFirstProcessingForDelay = false;
                m_TimeStamps.ProposeSoakStartTime = QDateTime::currentDateTime().addSecs(m_delayTime).toMSecsSinceEpoch();
            }
            else
            {
                m_TimeStamps.ProposeSoakStartTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
            }
        }
        else
        {
            m_TimeStamps.ProposeSoakStartTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
        }
        m_TimeStamps.CurStepSoakStartTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
        mp_SchedulerThreadController->LogDebug(QString("The duration time:%1 seconds.").arg(m_CurProgramStepInfo.durationInSeconds));
        mp_SchedulerThreadController->LogDebug(QString("m_TimeStamps.ProposeSoakStartTime is: %1").arg(QDateTime::fromMSecsSinceEpoch(m_TimeStamps.ProposeSoakStartTime).toString("yyyy-MM-dd hh:mm")));
        mp_SchedulerThreadController->LogDebug(QString("m_TimeStamps.CurStepSoakStartTime: %1").arg(QDateTime::fromMSecsSinceEpoch(m_TimeStamps.CurStepSoakStartTime).toString("yyyy-MM-dd hh:mm")));
        m_lastPVTime = 0;
        m_completionNotifierSent = false;

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
    mp_SchedulerThreadController->StartTimer();
}

bool CSchedulerStateHandler::MoveRV(RVPosition_type type)
{
    /*lint -e593 */
    CmdRVReqMoveToRVPosition* cmd = new CmdRVReqMoveToRVPosition(500, m_RetortName);
    RVPosition_t targetPos = RV_UNDEF;

    if(TUBE_POS == type ) //tube position
    {
        //get target position here
        targetPos = GetRVTubePositionByStationID(m_CurProgramStepInfo.stationID);
        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_MOVE_RV_TUBE_POSITION,QStringList()<<m_CurProgramStepInfo.stationID);
    }
    else if(SEAL_POS == type) //seal positon
    {
        //get target position here
        targetPos = GetRVSealPositionByStationID(m_CurProgramStepInfo.stationID);
        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_MOVE_RV_SEALING_POSITION,QStringList()<<m_CurProgramStepInfo.stationID);
    }
    else if(NEXT_TUBE_POS == type)
    {
        //get target position here
        targetPos = GetRVTubePositionByStationID(m_CurProgramStepInfo.nextStationID);
        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_MOVE_RV_TUBE_POSITION,QStringList()<<m_CurProgramStepInfo.nextStationID);
    }

    if(RV_UNDEF != targetPos)
    {
        cmd->SetRVPosition(targetPos);
        m_SchedulerCommandProcessor->pushCmd(cmd);
        return true;
    }
    else
    {
       mp_SchedulerThreadController->LogDebug(QString("Error in MV RV to station: %1").arg(m_CurProgramStepInfo.stationID));
       return false;
    }
}

void CSchedulerStateHandler::Fill()
{
    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_START_FILLING);
    //Update current scenario
    //this->UpdateCurrentScenario();
    //CheckResuemFromPause(PSSM_FILLING);
    CmdALFilling* cmd  = new CmdALFilling(500, m_RetortName);

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
//    quint32 Scenario = GetScenarioBySchedulerState(m_SchedulerMachine->GetCurrentState(),ReagentGroup);
//    // For paraffin, Insufficient Check is NOT needed.
//    if (272 == Scenario)
//    {
//        cmd->SetEnableInsufficientCheck(false);
//    }
//    else
//    {
//        cmd->SetEnableInsufficientCheck(true);
//    }

    m_SchedulerCommandProcessor->pushCmd(cmd);

    // acknowledge to gui
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(15000,m_CurProgramStepInfo.stationID , true, true, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = mp_SchedulerThreadController->GetNewCommandRef();
    mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

    // Update station reagent status
    this->UpdateStationReagentStatus(false);

    mp_SchedulerThreadController->StartTimer();
}

void CSchedulerStateHandler::Drain()
{
    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_DRAINING);
    // Update current scenario
    //this->UpdateCurrentScenario();
    //CheckResuemFromPause(PSSM_DRAINING);
    CmdALDraining* cmd  = new CmdALDraining(500, m_RetortName);

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
    Global::tRefType Ref = mp_SchedulerThreadController->GetNewCommandRef();
    mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

    mp_SchedulerThreadController->StartTimer();
}

void CSchedulerStateHandler::OnStopFill()
{
    // acknowledge to gui
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(15000,m_CurProgramStepInfo.stationID , false, true, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = mp_SchedulerThreadController->GetNewCommandRef();
    mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
}

void CSchedulerStateHandler::RCDrain()
{
    mp_SchedulerThreadController->LogDebug("Send cmd to DCL to RcDrain");
    CmdALDraining* cmd  = new CmdALDraining(500, m_RetortName);
    cmd->SetDelayTime(5000);
    cmd->SetDrainPressure(40.0);
    cmd->SetIgnorePressure(true);
    m_SchedulerCommandProcessor->pushCmd(cmd);

    mp_SchedulerThreadController->LogDebug("Notice GUI Draining started");
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(15000,m_CurProgramStepInfo.stationID , true, false, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = mp_SchedulerThreadController->GetNewCommandRef();
    mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

    mp_SchedulerThreadController->StartTimer();
}

void CSchedulerStateHandler::RCForceDrain()
{
    mp_SchedulerThreadController->LogDebug("Send cmd to DCL to RcForceDrain");
    CmdIDForceDraining* cmd  = new CmdIDForceDraining(500, m_RetortName);
    //cmd->SetRVPosition(this->GetRVTubePositionByStationID(m_CurProgramStepInfo.stationID));
    cmd->SetDrainPressure(40.0);
    m_SchedulerCommandProcessor->pushCmd(cmd);

    mp_SchedulerThreadController->LogDebug("Notice GUI Draining started");
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(15000,m_CurProgramStepInfo.stationID , true, false, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = mp_SchedulerThreadController->GetNewCommandRef();
    mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

    mp_SchedulerThreadController->StartTimer();
}

void CSchedulerStateHandler::RcDrainAtOnce()
{
    mp_SchedulerThreadController->LogDebug("Send cmd to DCL to Drain in RC_Drain_AtOnce");
    CmdALDraining* cmd  = new CmdALDraining(500, m_RetortName);
    //todo: get delay time here
    cmd->SetDelayTime(5000);
    cmd->SetIgnorePressure(true);
    m_SchedulerCommandProcessor->pushCmd(cmd);

    // acknowledge to gui
    mp_SchedulerThreadController->LogDebug("Notice GUI Draining started");
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(15000,m_CurProgramStepInfo.stationID , true, false, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = mp_SchedulerThreadController->GetNewCommandRef();
    mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

    mp_SchedulerThreadController->StartTimer();
}

void CSchedulerStateHandler::OnBeginDrain()
{
    // acknowledge to gui
    mp_SchedulerThreadController->LogDebug("Notice GUI Draining started");
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(15000,m_CurProgramStepInfo.stationID , true, false, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = mp_SchedulerThreadController->GetNewCommandRef();
    mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
}

void CSchedulerStateHandler::OnStopDrain()
{
    // acknowledge to gui
    mp_SchedulerThreadController->LogDebug("Notice GUI Draining stopped");
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(15000,m_CurProgramStepInfo.stationID , false, false, false));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = mp_SchedulerThreadController->GetNewCommandRef();
    mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
}

void CSchedulerStateHandler::OnFillingHeatingRV()
{
    // Update current sceanrio
    mp_SchedulerThreadController->UpdateCurrentScenario();
    mp_SchedulerThreadController->CheckResuemFromPause(PSSM_FILLING_RVROD_HEATING);
    quint32 leftSeconds = GetCurrentProgramStepNeededTime(m_CurProgramID);
    MsgClasses::CmdCurrentProgramStepInfor* commandPtr(new MsgClasses::CmdCurrentProgramStepInfor(5000, m_RetortName, m_CurReagnetName, m_CurProgramStepIndex, leftSeconds));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = mp_SchedulerThreadController->GetNewCommandRef();
    mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

    QString PVMode = "/";
    if(m_CurProgramStepInfo.isPressure)
    {
        PVMode = "P/";
    }
    if(m_CurProgramStepInfo.isVacuum)
    {
        PVMode += "V";
    }
    QString ReagentID = mp_DataManager->GetProgramList()->GetProgram(m_CurProgramID)->GetProgramStep(m_CurProgramStepIndex)->GetReagentID();
    const DataManager::CReagent* reagent = mp_DataManager->GetReagentList()->GetReagent(ReagentID);
    if(reagent && reagent->IsLeicaReagent())
    {
        Global::EventObject::Instance().RaiseEvent(EVENT_SCHEDULER_PROGRAM_STEP_START,Global::tTranslatableStringList()<<QString("[%1]").arg(m_CurProgramStepIndex)
                   << m_CurProgramStepInfo.stationID
                   <<Global::TranslatableString(reagent->GetReagentNameID().toUInt())
                   << DataManager::Helper::ConvertSecondsToTimeString(m_CurProgramStepInfo.durationInSeconds)
                   <<(m_CurProgramStepInfo.temperature > 0 ? QString("[%1]").arg(m_CurProgramStepInfo.temperature) : QString("Amb"))
                   <<PVMode);
    }
    else
    {
        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_PROGRAM_STEP_START,QStringList()<<QString("[%1]").arg(m_CurProgramStepIndex)
                   << m_CurProgramStepInfo.stationID
                   <<m_CurReagnetName << DataManager::Helper::ConvertSecondsToTimeString(m_CurProgramStepInfo.durationInSeconds)
                   <<(m_CurProgramStepInfo.temperature > 0 ? QString("[%1]").arg(m_CurProgramStepInfo.temperature) : QString("Amb"))
                   <<PVMode);
    }

    if(m_CurProgramStepInfo.reagentGroup == "RG6")
    {
        //mp_HeatingStrategy->Init260ParamList();
        mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_WAITING_FOR_FILLING_PARAFFIN);
        mp_SchedulerThreadController->EnablePauseButton();
    }

    //m_TickTimer.start();
    mp_SchedulerThreadController->StartTimer();
}

void CSchedulerStateHandler::OnEnterHeatingLevelSensor()
{
    // Update current sceanrio
    //this->UpdateCurrentScenario();
    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_HEATING_LEVEL_SENSOR_FOR_FILLING);
    mp_SchedulerThreadController->CheckResuemFromPause(PSSM_FILLING_LEVELSENSOR_HEATING);
    if(m_CurProgramStepInfo.reagentGroup != "RG6")
    {
       mp_SchedulerThreadController->EnablePauseButton();
    }
    //m_TickTimer.start();
    mp_SchedulerThreadController->StartTimer();
}

void CSchedulerStateHandler::CompleteRsAbort()
{
    //program finished
    AllStop();
    //GetHeatingStrategy()->StopTemperatureControl("LevelSensor");

    m_SchedulerMachine->SendRunComplete();

    // tell the main controller the program has been aborted
    MsgClasses::CmdProgramAborted* commandPtrAbortFinish(new MsgClasses::CmdProgramAborted(5000, m_ProgramStatusInfor.IsRetortContaminted()));
    Q_ASSERT(commandPtrAbortFinish);
    Global::tRefType fRef = mp_SchedulerThreadController->GetNewCommandRef();
    mp_SchedulerThreadController->SendCommand(fRef, Global::CommandShPtr_t(commandPtrAbortFinish));

    m_CurProgramStepIndex = -1;
    mp_SchedulerThreadController->LogDebug("Program aborted!");
    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_OVEN_ABORTED);
}

QString CSchedulerStateHandler::GetReagentGroupID(const QString& ReagentID)
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

bool CSchedulerStateHandler::IsLastStep(int currentStepIndex, const QString& currentProgramID)
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

bool CSchedulerStateHandler::CurProgramHasClearingReagent()
{
    if (m_CurProgramID.isEmpty() || m_CurProgramID.isNull())
    {
        return false;
    }

    if (!mp_DataManager)
    {
        Q_ASSERT(false);
        return false;
    }

    CDataProgramList* pDataProgramList = mp_DataManager->GetProgramList();
    if (!pDataProgramList)
    {
        Q_ASSERT(false);
        return false;
    }

    CProgram* pProgram = const_cast<CProgram*>(pDataProgramList->GetProgram(m_CurProgramID));
    for (int i = 0; i < pProgram->GetNumberOfSteps(); i++)
    {
        const CProgramStep* pProgramStep = pProgram->GetProgramStep(i);//use order index
        QString reagentID = pProgramStep->GetReagentID();
        if (Global::ClearingReagent == GetReagentGroupID(reagentID))
        {
            mp_SchedulerThreadController->LogDebug(QString("In CurProgramUsedClearingReagent, return true"));
            return true;
        }
    }
    return false;
}

QString CSchedulerStateHandler::ReagentGroupOfLastStep()
{
    if (m_CurProgramID.isEmpty() || m_CurProgramID.isNull())
    {
        return "";
    }

    if (-1 == m_CurProgramStepIndex)
    {
        return "";
    }

    if (!mp_DataManager)
    {
        Q_ASSERT(false);
        return "";
    }

    CDataProgramList* pDataProgramList = mp_DataManager->GetProgramList();
    if (!pDataProgramList)
    {
        Q_ASSERT(false);
        return "";
    }

    if (m_CurProgramStepIndex >= 1){
        CProgram* pProgram = const_cast<CProgram*>(pDataProgramList->GetProgram(m_CurProgramID));
        int lastProgramStepIndex = m_CurProgramStepIndex -1;
        const CProgramStep* pLastProgramStep = pProgram->GetProgramStep(lastProgramStepIndex);
        return GetReagentGroupID(pLastProgramStep->GetReagentID());
    }

    return "";
}

QString CSchedulerStateHandler::ReagentIdOfCurProgramStep()
{
    if (m_CurProgramID.isEmpty() || m_CurProgramID.isNull())
    {
        return "";
    }

    if (-1 == m_CurProgramStepIndex)
    {
        return "";
    }

    if (!mp_DataManager)
    {
        Q_ASSERT(false);
        return "";
    }

    CDataProgramList* pDataProgramList = mp_DataManager->GetProgramList();
    if (!pDataProgramList)
    {
        Q_ASSERT(false);
        return "";
    }

    CProgram* pProgram = const_cast<CProgram*>(pDataProgramList->GetProgram(m_CurProgramID));
    const CProgramStep* pProgramStep = pProgram->GetProgramStep(m_CurProgramStepIndex);
    return pProgramStep->GetReagentID();
}


QString CSchedulerStateHandler::GetStationIDFromProgramStep(int ProgramStepIndex)
{
    ProgramStepIndex = ProgramStepIndex - m_FirstProgramStepIndex;

    if (ProgramStepIndex < 0 || (ProgramStepIndex > m_StationList.size()-1) || m_StationList.empty())
    {
        return "";
    }
    return m_StationList.at(ProgramStepIndex);
}

QString CSchedulerStateHandler::GetReagentName(const QString& ReagentID)
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

QString CSchedulerStateHandler::GetSafeReagentType(const QString& curReagentGroupID, bool firstTimeUseReagent)
{
    if (firstTimeUseReagent){
        if (Global::FixationReagent == curReagentGroupID || Global::ClearingReagent == curReagentGroupID ||
            Global::ParaffinReagent == curReagentGroupID){
            return curReagentGroupID;
        }

        if (Global::WaterReagent == curReagentGroupID){
            return Global::FixationReagent;
        }

        if (Global::DehydratingDiluted == curReagentGroupID){
            return Global::FixationDehydratingDiluted;
        }

        if (Global::DehydratingAbsolute == curReagentGroupID){
            return Global::DehydratingDilutedFixation;
        }
        return "";
    }

    //not first time to use reagent
    if (Global::WaterReagent == curReagentGroupID){
        return Global::FixationReagent;
    }
    return curReagentGroupID;
}

qint32 CSchedulerStateHandler::GetScenarioBySchedulerState(SchedulerStateMachine_t State, QString ReagentGroup)
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
        //During pressure test stage in the scenarios 2*5, we also think current scenario is 2*4
//        if (QString::number(scenario).left(1) == "2" && QString::number(scenario).right(1) =="5" && m_SchedulerMachine->GetPssmMVTubeSeq() <3)
//        {
//            return scenario;
//        }
        m_ProgramStatusInfor.SetScenario(scenario);
    }
    return scenario;
}

void CSchedulerStateHandler::OnEnterIdleState()
{
    // Disable Bottle Check button
    MsgClasses::CmdProgramAcknowledge* commandPtr(new MsgClasses::CmdProgramAcknowledge(5000, DataManager::DISABLE_BOTTLE_CHECK));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = mp_SchedulerThreadController->GetNewCommandRef();
    mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

    //for bottle check and Error restart in idle state , the cmd take out specimen is OK
    /*if((4 == m_CurrentScenario && SM_IDLE == m_CurrentStepState) || 7 == m_CurrentScenario)
    {
        m_IsTakeSpecimen = true;
    }*/
    m_IsWaitHeatingRV = true;
//    m_IsSendMsgForWaitHeatRV = false;
//    m_IdleState = IDLE_HEATING_RV;
//    m_RVPositioinChSeqForIdle = 0;
//    m_PressureStartTime = 0;

    if (m_ProgramStatusInfor.IsRetortContaminted() && !m_CleanAckSentGui)
    {
        m_CleanAckSentGui = true;
        MsgClasses::CmdEnterCleaningProgram* commandEnterCleaning(new MsgClasses::CmdEnterCleaningProgram(5000, m_ProgramStatusInfor.GetLastReagentGroup()));
        Q_ASSERT(commandEnterCleaning);
        Global::tRefType fRef = mp_SchedulerThreadController->GetNewCommandRef();
        mp_SchedulerThreadController->SendCommand(fRef, Global::CommandShPtr_t(commandEnterCleaning));
    }
}

void CSchedulerStateHandler::OnExitedInitState()
{
    mp_SchedulerThreadController->InitProgramStatus();
}

void CSchedulerStateHandler::OnExitIdleState()
{
    // Disable Bottle Check button
    MsgClasses::CmdProgramAcknowledge* commandPtr(new MsgClasses::CmdProgramAcknowledge(5000, DataManager::DISABLE_BOTTLE_CHECK));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = mp_SchedulerThreadController->GetNewCommandRef();
    mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

    //In case of Idle-error state transition, we need make DRAIN 10 MSG box disappear
    mp_SchedulerThreadController->RaiseEvent(EVENT_SCHEDULER_DRAIN_10S_NOT_OPEN_RETORT_LID, QStringList(), false);
}

void CSchedulerStateHandler::OnEnterDryStepState()
{
    // Update current scenario
    //this->UpdateCurrentScenario();
    m_CleaningDry.CurrentState = CDS_READY;
    m_CleaningDry.StepStartTime = 0;
    m_CleaningDry.warningReport = false;

    mp_SchedulerThreadController->StartTimer();
}

void CSchedulerStateHandler::HardwareMonitor(const QString& StepID)
{
    if(m_CheckTheHardwareStatus <= 2)  // Wait for about 500ms*3 = 1500ms to start hardware monitor
    {
        m_CheckTheHardwareStatus++;
        return;
    }
    QString ReagentGroup = m_CurProgramStepInfo.reagentGroup;
    quint32 Scenario = GetScenarioBySchedulerState(m_SchedulerMachine->GetCurrentState(),ReagentGroup);
    m_IsErrorStateForHM = false;

    HardwareMonitor_t strctHWMonitor = m_SchedulerCommandProcessor->HardwareMonitor(m_RetortName);
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
//    if (1 == strctHWMonitor.RemoteAlarmStatus && m_RemoteAlarmPreviousStatus < 1) //alarm was unplugged
//    {
//        RaiseError(0, DCL_ERR_DEV_MC_REMOTEALARM_UNCONNECTED, m_CurrentScenario, true);
//        m_RemoteAlarmPreviousStatus = 1;
//    }
//    else if(0 == strctHWMonitor.RemoteAlarmStatus && m_RemoteAlarmPreviousStatus == 1)// alarm was plugged
//    {
//        m_RemoteAlarmPreviousStatus = 0;
//        RaiseError(0, DCL_ERR_DEV_MC_REMOTEALARM_UNCONNECTED, 0,true, false);
//    }
//    if (1 == strctHWMonitor.LocalAlarmStatus && m_LocalAlarmPreviousStatus < 1) //alarm was unplugged
//    {
//        RaiseError(0, DCL_ERR_DEV_MC_LOCALALARM_UNCONNECTED, m_CurrentScenario, true);
//        m_LocalAlarmPreviousStatus = 1;
//    }
//    else if(0 == strctHWMonitor.LocalAlarmStatus && m_LocalAlarmPreviousStatus == 1) //alarm was unplugged
//    {
//        RaiseError(0, DCL_ERR_DEV_MC_LOCALALARM_UNCONNECTED, 0,true, false);
//        m_LocalAlarmPreviousStatus = 0;
//    }
#endif

    if(strctHWMonitor.RetortLockStatus != UNDEFINED_VALUE)
    {
        if(((m_RetortLockStatus == 0) ||(m_RetortLockStatus == UNDEFINED_VALUE))&&(strctHWMonitor.RetortLockStatus == 1))
        {
            MsgClasses::CmdLockStatus* commandPtr(new MsgClasses::CmdLockStatus(5000, DataManager::RETORT_LOCK, false));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = mp_SchedulerThreadController->GetNewCommandRef();
            mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        }
        if(((m_RetortLockStatus == 1) || (m_RetortLockStatus == UNDEFINED_VALUE))&&(strctHWMonitor.RetortLockStatus == 0))
        {
            MsgClasses::CmdLockStatus* commandPtr(new MsgClasses::CmdLockStatus(5000, DataManager::RETORT_LOCK, true));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = mp_SchedulerThreadController->GetNewCommandRef();
            mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        }
        m_RetortLockStatus = strctHWMonitor.RetortLockStatus;
    }

    if(strctHWMonitor.OvenLidStatus != UNDEFINED_VALUE)
    {
        if(((m_OvenLidStatus == 0) ||(m_OvenLidStatus == UNDEFINED_VALUE))&&(strctHWMonitor.OvenLidStatus == 1))
        {
            MsgClasses::CmdLockStatus* commandPtr(new MsgClasses::CmdLockStatus(5000, DataManager::PARAFFIN_BATH_LOCK, false));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = mp_SchedulerThreadController->GetNewCommandRef();
            mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        }
        if(((m_OvenLidStatus == 1) || (m_OvenLidStatus == UNDEFINED_VALUE))&&(strctHWMonitor.OvenLidStatus == 0))
        {
            MsgClasses::CmdLockStatus* commandPtr(new MsgClasses::CmdLockStatus(5000, DataManager::PARAFFIN_BATH_LOCK, true));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = mp_SchedulerThreadController->GetNewCommandRef();
            mp_SchedulerThreadController->SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        }
        m_OvenLidStatus = strctHWMonitor.OvenLidStatus;
    }
    if("ERROR" == StepID)
    {
        m_IsErrorStateForHM = true;
    }

    // Monitor the sensors' current
//    if ("ERROR" != StepID && 0 != m_CurrentScenario)
//    {
//        m_CurrentScenario = Scenario;
//        mp_SchedulerThreadController->CheckSlaveAllSensor(m_CurrentScenario, strctHWMonitor);
//    }

//    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.PressureAL))
//    {
//        m_PressureAL = strctHWMonitor.PressureAL;
//    }
//    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.TempALLevelSensor))
//    {
//        m_TempALLevelSensor = strctHWMonitor.TempALLevelSensor;
//    }
//    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.TempALTube1))
//    {
//        m_TempALTube1 = strctHWMonitor.TempALTube1;
//    }
//    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.TempALTube2))
//    {
//        m_TempALTube2 = strctHWMonitor.TempALTube2;
//    }
//    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.TempRV1))
//    {
//        m_TempRV1 = strctHWMonitor.TempRV1;
//    }
//    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.TempRV2))
//    {
//        m_TempRV2 = strctHWMonitor.TempRV2;
//    }
//    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.TempRTBottom1))
//    {
//        m_TempRTBottom = strctHWMonitor.TempRTBottom1;
//    }
//    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.TempRTSide))
//    {
//        m_TempRTSide = strctHWMonitor.TempRTSide;
//    }
//    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.TempOvenBottom1))
//    {
//        m_TempOvenBottom = strctHWMonitor.TempOvenBottom1;
//    }
//    if(mp_HeatingStrategy->isEffectiveTemp(strctHWMonitor.TempOvenTop))
//    {
//        m_TempOvenTop = strctHWMonitor.TempOvenTop;
//    }
    m_PositionRV = strctHWMonitor.PositionRV;
}

}
