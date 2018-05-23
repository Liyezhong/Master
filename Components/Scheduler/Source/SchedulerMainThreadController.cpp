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
#include "Scheduler/Commands/Include/CmdALFilling.h"
#include "Scheduler/Commands/Include/CmdALReleasePressure.h"
#include "Scheduler/Commands/Include/CmdPerTurnOnMainRelay.h"
#include "Scheduler/Commands/Include/CmdPerTurnOffMainRelay.h"
#include "Scheduler/Commands/Include/CmdALTurnOnFan.h"
#include "Scheduler/Commands/Include/CmdALTurnOffFan.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToRVPosition.h"
#include "Scheduler/Commands/Include/CmdIDBottleCheck.h"
#include "Scheduler/Commands/Include/CmdRmtLocAlarm.h"
#include "Scheduler/Commands/Include/CmdALStopCmdExec.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "HimalayaDataManager/Include/DataManager.h"
#include "HimalayaDataContainer/Containers/ProgramSettings/Include/ProgramSettings.h"
#include "Global/Include/GlobalDefines.h"
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
//#include "Scheduler/Test/Mock/MockIDeviceProcessing.h"
#include "DeviceControl/Test/Mock/MockDeviceControl.h"
#endif

#include "Global/Include/EventObject.h"
#include "Scheduler/Include/HeatingStrategy.h"

#include <QtAlgorithms>
#include "Global/Include/SystemPaths.h"
#include "Global/Include/Utils.h"
#include "Global/Include/Commands/CmdShutDown.h"
#include <DataManager/CommandInterface/Include/UserSettingsCommandInterface.h>
#include <DataManager/Helper/Include/DataManagerEventCodes.h>
#include <Scheduler/Include/SchedulerStateHandler.h>
#include <Scheduler/Include/ProgramSelfTest.h>
#ifdef GOOGLE_MOCK
#include "DeviceControl/Test/Mock/MockDeviceControl.h"
#endif
#include "DeviceControl/Include/Simulation/DeviceControlSim.h"

#include "Scheduler/Include/InstrumentManager.h"
#include "Scheduler/Include/EventDispatcher.h"

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
    RVPosition_t tube1 = CSchedulerStateHandler::GetRVTubePositionByStationID(va1.first);
    RVPosition_t tube2 = CSchedulerStateHandler::GetRVTubePositionByStationID(va2.first);
    return tube1 < tube2;
}

SchedulerMainThreadController::SchedulerMainThreadController(
        Global::gSourceType TheHeartBeatSource)
        : Threads::ThreadController(TheHeartBeatSource, "SchedulerMainThread")
        , m_TickTimer(this)
        , m_SchedulerCommandProcessorThread(new QThread())
//        #ifdef GOOGLE_MOCK
//        , m_SchedulerCommandProcessor(new SchedulerCommandProcessor<IDeviceControl>(this))
//        #else
//        , m_SchedulerCommandProcessor(new SchedulerCommandProcessor<IDeviceProcessing>(this))
//        #endif
        , mp_DataManager(NULL)
        , m_OvenLidStatus(UNDEFINED_VALUE)
        , m_RetortLockStatus(UNDEFINED_VALUE)
        , mp_HeatingStrategy(NULL)
        , m_RefCleanup(Global::RefManager<Global::tRefType>::INVALID)
        , m_IsInSoakDelay(false)
        ,m_InternalErrorRecv(false)
        ,m_bWaitToPause(false)
        ,m_ReagentExpiredFlag("")
        ,m_pEventDispatcher(new EventDispatcher())
{
    m_CurErrEventID = DCL_ERR_FCT_NOT_IMPLEMENTED;
    m_IsPrecheckMoveRV = false;

    m_lastPVTime = 0;
    m_ProcessingPV = 3; // 0 for Pressure and 1 for Vacuum, 3 for avoiding message to pop up too many times
    m_completionNotifierSent = false;
    m_IsReleasePressureOfSoakFinish = false;
    m_CleanAckSentGui = false;
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
    m_ReportExhaustFanWarning = true;
    m_PauseStartTime = 0;
    m_bWaitToPauseCmdYes = false;
    m_IsResumeFromPause = false;
    m_StateAtPause = SM_UNDEF;

    m_IsSelfTestDone = false;

    m_DisableAlarm = Global::Workaroundchecking("DISABLE_ALARM");


    //Initialize the Return Code counter list
    m_RetCodeCounterList.insert(DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_OVERRANGE, 0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_OVERRANGE,0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_OVERRANGE,0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_NOSIGNAL,0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_NOSIGNAL,0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_NOSIGNAL,0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_RETORT_TSENSOR1_TO_2_SELFCALIBRATION_FAILED,0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_RETORT_LEVELSENSOR_HEATING_OVERTIME,0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_LEVELSENSOR_TEMPERATURE_OVERRANGE,0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_LIDLOCK_CLOSE_STATUS_ERROR,0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_RETORT_HEATING_OVERTIME, 0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_WAXBATH_TSENSORUP_OUTOFRANGE,0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_WAXBATH_TSENSORDOWN1_OUTOFRANGE,0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_WAXBATH_TSENSORDOWN2_OUTOFRANGE,0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_WAXBATH_OVENCOVER_STATUS_OPEN,0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_RV_HEATING_TEMPSENSOR1_OUTOFRANGE,0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_RV_HEATING_TEMPSENSOR2_OUTOFRANGE,0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_RV_HEATING_TEMPSENSOR2_NOTREACHTARGET,0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_RV_HEATING_TSENSOR2_LESSTHAN_40DEGREEC_OVERTIME,0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_ABNORMAL,0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_LA_TUBEHEATING_TUBE2_ABNORMAL,0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR1_OUTOFRANGE,0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_LA_TUBEHEATING_TSENSOR2_OUTOFRANGE,0);
    m_RetCodeCounterList.insert(DCL_ERR_DEV_LA_TUBEHEATING_TUBE1_NOTREACHTARGETTEMP,0);

}

SchedulerMainThreadController::~SchedulerMainThreadController()
{
//    try {
//        if (m_SchedulerMachine != NULL)
//            delete m_SchedulerMachine;
//        m_SchedulerMachine = NULL;
//    }
//    CATCHALL_DTOR();
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
    m_pEventDispatcher->Start();
    QString mode = mp_DataManager->GetDeviceConfigurationInterface()->GetDeviceConfiguration()->GetValue("Mode");

#ifdef GOOGLE_MOCK
    m_SchedulerCommandProcessor = (new SchedulerCommandProcessor<IDeviceControl>(this));

#endif
    if(mode.toLower() == "simulation")
        m_SchedulerCommandProcessor = (new SchedulerCommandProcessor<DeviceControlSim>(this));
    else
        m_SchedulerCommandProcessor = (new SchedulerCommandProcessor<IDeviceProcessing>(this));

    m_SchedulerCommandProcessor->moveToThread(m_SchedulerCommandProcessorThread);
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessorThread, started(), m_SchedulerCommandProcessor, run());
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessor, destroyed(), m_SchedulerCommandProcessorThread, quit());
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessorThread, finished(), m_SchedulerCommandProcessorThread, deleteLater());
    m_SchedulerCommandProcessorThread->start();

    //timer setting
    CONNECTSIGNALSLOT(&m_TickTimer, timeout(), this, OnTickTimer());
    qRegisterMetaType<QList<QString>>("QList<QString>");
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessor,DCLConfigurationFinished(ReturnCode_t, QList<QString>),this,OnDCLConfigurationFinished(ReturnCode_t, QList<QString>));
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessor, DeviceProcessDestroyed(),this, DevProcDestroyed());
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessor, ReportGetServiceInfo(ReturnCode_t, const DataManager::CModule&, const QString&),
                     this, ReportGetServiceInfo(ReturnCode_t, const DataManager::CModule&, const QString&));
//    CONNECTSIGNALSLOT(m_SchedulerCommandProcessor, ReportFillingTimeOut2Min(),this, OnReportFillingTimeOut2Min());
//    CONNECTSIGNALSLOT(m_SchedulerCommandProcessor, ReportDrainingTimeOut2Min(),this, OnReportDrainingTimeOut2Min());
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessor, ReportError(quint32, quint16, quint16, quint16, QDateTime),this, OnReportError(quint32, quint16, quint16, quint16, QDateTime));
    CONNECTSIGNALSLOT(mp_DataManager->mp_SettingsCommandInterface, ResetActiveCarbonFilterLifeTime(quint32),
                     this, ResetActiveCarbonFilterLifetime(quint32));
    m_TickTimer.setInterval(500);

    //command queue reset
    m_SchedulerCmdQueue.clear();
    m_DeviceControlCmdQueue.clear();

    // now register commands
    RegisterCommands();

//    m_pInstrumentManager = new InstrumentManager("Instrument", this);

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
    Q_UNUSED(ReturnCode)
    //LogDebug(QString("LifeCycle:ReportGetServiceInfo:%1,%2").arg(ReturnCode).arg(DeviceType));
    //send command
    SendCommand(GetNewCommandRef(), Global::CommandShPtr_t(new MsgClasses::CmdModuleListUpdate(30000, ModuleInfo, DeviceType, false)));
}

void SchedulerMainThreadController::ResetActiveCarbonFilterLifetime(quint32 setVal)
{
    m_SchedulerCommandProcessor->ResetActiveCarbonFilterLifetime(setVal);
}

void SchedulerMainThreadController::OnReportError(quint32 instanceID, quint16 usErrorGroup, quint16 usErrorID, quint16 usErrorData, QDateTime timeStamp)
{
    Q_UNUSED(instanceID)
    Q_UNUSED(usErrorGroup)
    Q_UNUSED(usErrorID)
    Q_UNUSED(usErrorData)
    Q_UNUSED(timeStamp)
#if 0
    LogDebug(QString("In OnReportError, instanceID=%1, usErrorGroup=%2, usErrorID=%3, usErrorData=%4 and timeStamp=%5")
             .arg(instanceID, 0, 16).arg(usErrorGroup).arg(usErrorID).arg(usErrorData).arg(timeStamp.toString()));
#endif

    if(usErrorID == DCL_ERR_TIMEOUT || usErrorID == DCL_ERR_CANCOMMUTOR_COMM_FAILED)
    {
        if (false == m_InternalErrorRecv)
        {
            m_InternalErrorRecv = true;
            SendOutErrMsg(DCL_ERR_DEV_INTER_SW_ERROR, false);
        }
    }

}

void SchedulerMainThreadController::CleanupAndDestroyObjects()
{
}

void SchedulerMainThreadController::OnGoReceived()
{
    qDebug()<<"************************************************ master thread on go received...";
//    OnDCLConfigurationFinished(DCL_ERR_FCT_CALL_SUCCESS);
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
    NonDeviceCommand_t newControllerCmd = PeekNonDeviceCommand();

#if 1
    if (CTRL_CMD_RS_SHUTDOWN == newControllerCmd.Cmd)
    {
        //send shutdown command to MasterThreadController
        SendCommand(GetNewCommandRef(), Global::CommandShPtr_t(new Global::CmdShutDown(true)));
        m_TickTimer.stop();
        m_ProgramStatusInfor.SetErrorFlag(0);
        m_SchedulerCommandProcessor->ShutDownDevice();
        return;
    }
#endif
    if (CTRL_CMD_SHUTDOWN == newControllerCmd.Cmd)
    {
        m_TickTimer.stop();
        m_ProgramStatusInfor.SetErrorFlag(0);
        m_ProgramStatusInfor.SetProgramID("");
        m_SchedulerCommandProcessor->ShutDownDevice();
        //DequeueNonDeviceCommand();
        return;
    }

    if (CTRL_CMD_TAKEOUT_SPECIMEN_FINISH == newControllerCmd.Cmd)
    {
        m_IsTakeSpecimen = true;
    }

    // For remote alarm and local alarm checking
    if (CTRL_CMD_RS_CHECKREMOTEALARMSTATUS == newControllerCmd.Cmd)
    {
        m_CheckRemoteAlarmStatus = true;
    }

    if (CTRL_CMD_RS_CHECKLOCALALARMSTATUS == newControllerCmd.Cmd)
    {
        m_CheckLocalAlarmStatus = true;
    }

    if (CTRL_CMD_CHECK_OVEN_COVER == newControllerCmd.Cmd)
    {
        m_CheckOvenCover = true;
    }
    if (CTRL_CMD_OPEN_OVEN_CHANGE_HEATING_PARAFFIN == newControllerCmd.Cmd)
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

    //HandleRmtLocAlarm(ctrlCmd);

    Scheduler::SchedulerCommandShPtr_t cmd;
    (void)PopDeviceControlCmdQueue(cmd);

    if (!m_IsSelfTestDone)
    {
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

//        HandleSelfTestWorkFlow(cmdName, retCode);
    }
    else
    {
        if (cmd != NULL)
        {
            qDebug()<<"************** handler state command. sender: "<<cmd->GetSender()<<cmd->GetName();
        }
        qDebug()<<"************** handler state command. RetortName: "<<newControllerCmd.Retort_name<<newControllerCmd.Cmd;

        QMap<QString, QSharedPointer<CSchedulerStateHandler>>::const_iterator itr;
        for (itr = m_SchedulerStateHandlerList.constBegin(); itr != m_SchedulerStateHandlerList.constEnd(); ++itr)
        {
            CSchedulerStateHandler* stateHandler = itr.value().data();
            QString RetortName = stateHandler->GetRetortName();
            if (newControllerCmd.Retort_name == RetortName)
            {
                stateHandler->HandleStateCommand(newControllerCmd.Cmd, cmd);
            }
            else
            {
                stateHandler->HandleStateCommand(CTRL_CMD_NONE, cmd);
            }

        }
    }
}

void SchedulerMainThreadController::HandleSelfTestWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode)
{
    qDebug()<<"**************** handler self test, cmd:"<<cmdName<<retCode;
    mp_ProgramSelfTest->HandleWorkFlow(cmdName, retCode);
}

void SchedulerMainThreadController::OnSelfTestDone(bool flag)
{
    qDebug()<<"***************** self test is done. flag:"<<flag;
    if(flag)
    {
        // Turn on fan, and we will open it all the time
        m_SchedulerCommandProcessor->pushCmd(new CmdALTurnOnFan(500, m_Sender), false);

        bool bErrorHandlingFailed = m_ProgramStatusInfor.GetErrorFlag();
        //if(!m_ProgramStatusInfor.IsProgramFinished())//power failure
        if (false) // for demo test, we ignore the power failure
        {
//            MsgClasses::CmdRecoveryFromPowerFailure* commandPtr(
//                        new MsgClasses::CmdRecoveryFromPowerFailure(5000,m_ProgramStatusInfor.GetProgramId(),
//                                                                    m_ProgramStatusInfor.GetStepID(),
//                                                                    m_ProgramStatusInfor.GetScenario(),
//                                                                    GetLeftProgramStepsNeededTime(m_ProgramStatusInfor.GetProgramId(), m_ProgramStatusInfor.GetStepID()),
//                                                                    m_ProgramStatusInfor.GetLastReagentGroup(),
//                                                                    m_ProgramStatusInfor.GetStationList(),
//                                                                    bErrorHandlingFailed, m_ProgramStatusInfor.GetStationID()));
//            Q_ASSERT(commandPtr);
//            Global::tRefType Ref = GetNewCommandRef();
//            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
            if(1 == bErrorHandlingFailed)
            {
                RaiseError(0, DCL_ERR_DEV_INTER_POWERFAILURE_AFTER_ERRHANDLING_FAILED, m_ProgramStatusInfor.GetScenario(), true);
                //m_SchedulerMachine->SendErrorSignal();
            }
            else
            {
                QString ProgramName = "";
                quint32 ProgramNameID = 0;
                if(mp_DataManager&& mp_DataManager->GetProgramList()&&mp_DataManager->GetProgramList()->GetProgram(m_ProgramStatusInfor.GetProgramId()))
                {
                    ProgramName = mp_DataManager->GetProgramList()->GetProgram(m_ProgramStatusInfor.GetProgramId())->GetName();
                    if(mp_DataManager->GetProgramList()->GetProgram(m_ProgramStatusInfor.GetProgramId())->IsLeicaProgram())
                    {
                        ProgramNameID = mp_DataManager->GetProgramList()->GetProgram(m_ProgramStatusInfor.GetProgramId())->GetNameID().toUInt();
                    }
                }
                if(ProgramNameID == 0)
                {
                    RaiseEvent(EVENT_SCHEDULER_POWER_FAILURE,QStringList()<<ProgramName<<QString(" [ %1 ]").arg(m_ProgramStatusInfor.GetStepID() + 1));
                }
                else
                {
                    Global::EventObject::Instance().RaiseEvent(EVENT_SCHEDULER_POWER_FAILURE,Global::tTranslatableStringList()
                                                               <<Global::TranslatableString(ProgramNameID)
                                                               <<Global::TranslatableString(QString(" [ %1 ]").arg(m_ProgramStatusInfor.GetStepID() + 1))
                                                               );
                }
                //Let GUI know master SW will enter power failure
                MsgClasses::CmdProgramAcknowledge* commandPtr(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RESUME_AFTER_POWER_FAILURE));
                Q_ASSERT(commandPtr);
                Global::tRefType fRef = GetNewCommandRef();
                SendCommand(fRef, Global::CommandShPtr_t(commandPtr));

                //m_SchedulerMachine->EnterPowerFailure();
            }
        }
        else
        {
            m_IsSelfTestDone = true;

            InitProgramStatus();

            if(1 == bErrorHandlingFailed)
            {
                m_ProgramStatusInfor.SetErrorFlag(0);
            }
            //OnEnterIdleState();
            OnStartStateHandler();
            m_IsTakeSpecimen = true;
            //m_SchedulerMachine->SendSchedulerInitComplete();
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
        //m_SchedulerMachine->SendErrorSignal();
    }
}

void SchedulerMainThreadController::OnStartStateHandler()
{
    qDebug()<<"********* Start state handler.";
    QMap<QString, QSharedPointer<CSchedulerStateHandler>>::const_iterator itr;
    for (itr = m_SchedulerStateHandlerList.constBegin(); itr != m_SchedulerStateHandlerList.constEnd(); ++itr)
    {
        itr.value().data()->Start();
    }
}

#if 0
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
           // For there is the scenario 003, master need let GUI know that SW is running rather than idle
           this->SendSystemBusy2GUI(true);

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

               //In this case, master SW will be in idle state
               this->SendSystemBusy2GUI(false);
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
            // For there is the scenario 003, master need let GUI know that SW is running rather than idle
            this->SendSystemBusy2GUI(true);

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
                    m_WasPressureIdle = false;
                    m_SentInfoForLockLidIdle = false;
                }
                break;
            case IDLE_DRAIN_10S:
                if(m_RetortLockStatus == 1)
                {
                    if(CTRL_CMD_USER_PRESS_OK_BUTTON_LOCK_RTLID_IDLE == ctrlCmd || !m_SentInfoForLockLidIdle)  //raise again
                    {
                        RaiseEvent(EVENT_SCHEDULER_LOCK_RETORT_LID);
                        m_SentInfoForLockLidIdle = true;
                    }
                    if(m_WasPressureIdle)
                    {
                        RaiseEvent(EVENT_SCHEDULER_DRAIN_10S_NOT_OPEN_RETORT_LID,QStringList(), false); // remove the msg
                        ReleasePressure();
                    }
                    m_RVPositioinChSeqForIdle = 0;
                    return;
                }
                if(0 == m_RVPositioinChSeqForIdle)
                {
                    if(m_SentInfoForLockLidIdle)
                    {
                        RaiseEvent(EVENT_SCHEDULER_LOCK_RETORT_LID, QStringList(), false); // remove the message box
                    }
                    m_SentInfoForLockLidIdle = false;
                    RaiseEvent(EVENT_SCHEDULER_DRAIN_10S_NOT_OPEN_RETORT_LID);

                    CmdALPressure* CmdPressure = new CmdALPressure(500, m_Sender);
                    CmdPressure->SetTargetPressure(30.0);
                    m_SchedulerCommandProcessor->pushCmd(CmdPressure);
                    m_PressureStartTime = QDateTime::currentMSecsSinceEpoch();
                    m_RVPositioinChSeqForIdle++;
                    m_WasPressureIdle = true;
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
                    m_WasPressureIdle = false;
                    RaiseEvent(EVENT_SCHEDULER_DRAIN_10S_NOT_OPEN_RETORT_LID,QStringList(), false); // remove the msg
                }
                break;
            case IDLE_MOVE_RV_INITIALIZE:
                if(0 == m_RVPositioinChSeqForIdle)
                {
                    m_SchedulerCommandProcessor->pushCmd(new CmdRVReqMoveToInitialPosition(500, m_Sender));
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
                   CmdRVReqMoveToRVPosition *CmdMvRV = new CmdRVReqMoveToRVPosition(500, m_Sender);
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

                    //In this case, master SW will be in idle state
                    this->SendSystemBusy2GUI(false);
                }
                break;
            default:
                break;
        }
    }
}
#endif


void SchedulerMainThreadController::CheckResuemFromPause(SchedulerStateMachine_t currentState)
{
#if 0
    if (!m_IsResumeFromPause || currentState != m_StateAtPause)
    {
        return;
    }
    // First of all, set the boolean variable to false;
    m_IsResumeFromPause = false;

    // Dismiss the resuming dialogue
    MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::DISMISS_RESUME_MSG_DLG));
    Q_ASSERT(commandPtrFinish);
    Global::tRefType fRef = GetNewCommandRef();
    SendCommand(fRef, Global::CommandShPtr_t(commandPtrFinish));

    //For the sceanrios PSSM_RV_MOVE_TO_TUBE, PSSM_DRAINING and PSSM_RV_POS_CHANGE in last step, SW will just return
    if (IsLastStep(m_CurProgramStepIndex, m_CurProgramID))
    {
        if (PSSM_RV_MOVE_TO_TUBE == currentState || PSSM_DRAINING == currentState || PSSM_RV_POS_CHANGE == currentState)
        {
            return;
        }
    }

    // Enable pause button
    MsgClasses::CmdProgramAcknowledge* commandPtrPauseEnable(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_PAUSE_ENABLE));
    Q_ASSERT(commandPtrPauseEnable);
    fRef = GetNewCommandRef();
    SendCommand(fRef, Global::CommandShPtr_t(commandPtrPauseEnable));

    qint64 now = QDateTime::currentMSecsSinceEpoch();
    qint64 offset = 0;
    if (PSSM_FILLING_LEVELSENSOR_HEATING == m_StateAtPause || PSSM_FILLING_RVROD_HEATING == m_StateAtPause
            || PSSM_FILLING == m_StateAtPause || PSSM_RV_MOVE_TO_SEAL == m_StateAtPause)
    {        
        if (m_CurProgramStepIndex == 0)
        {
            if ((now-m_PauseStartTime) > m_delayTime*1000)
            {
                offset = now - m_PauseStartTime - m_delayTime*1000;
                m_CurProgramStepInfo.durationInSeconds -= m_delayTime;
                m_delayTime = 0;
            }
            else
            {
                m_delayTime -= (now-m_PauseStartTime)/1000;
                m_CurProgramStepInfo.durationInSeconds -= (now-m_PauseStartTime)/1000;
                return;
            }
        }
        else
        {
            offset = now - m_PauseStartTime;
        }
    }
    else if (PSSM_PROCESSING == m_StateAtPause)
    {
        // For Filling and Move to Seal
        if (!m_IsProcessing)
        {
            if (m_CurProgramStepIndex == 0)
            {
                if ((now-m_PauseStartTime) > m_delayTime*1000)
                {
                    offset = now - m_PauseStartTime - m_delayTime*1000;
                    m_CurProgramStepInfo.durationInSeconds -= m_delayTime;
                    m_delayTime = 0;
                }
                else
                {
                    m_delayTime -= (now-m_PauseStartTime)/1000;
                    m_CurProgramStepInfo.durationInSeconds -= (now-m_PauseStartTime)/1000;
                    return;
                }
            }
            else
            {
                offset = now-m_PauseStartTime;
            }
        }
        else
        {
            if (now <= m_TimeStamps.ProposeSoakStartTime)
            {
                return;
            }
            if (now > m_TimeStamps.ProposeSoakStartTime)
            {
                if (m_CurProgramStepIndex == 0)
                {
                    m_CurProgramStepInfo.durationInSeconds -= m_delayTime;
                    m_IsProcessing = false;
                    m_delayTime = 0;
                    if (m_PauseStartTime <= m_TimeStamps.ProposeSoakStartTime)
                    {
                        offset = now - m_TimeStamps.ProposeSoakStartTime;
                    }
                    else
                    {
                        m_CurProgramStepInfo.durationInSeconds -= (m_PauseStartTime - m_TimeStamps.ProposeSoakStartTime)/1000;
                        offset = now - m_PauseStartTime;
                    }
                }
                else
                {
                    m_IsProcessing = false;
                    m_CurProgramStepInfo.durationInSeconds -= (m_PauseStartTime - m_TimeStamps.ProposeSoakStartTime)/1000;
                    offset = now - m_PauseStartTime;

                }

            }
        }
    }
    else
    {
        offset = now - m_PauseStartTime;
    }

    if (offset < 1000) //If offset is less than one second, we just ignore this.
    {
        return;
    }

    LogDebug(QString("Pause resuming has compensated %1 seconds").arg(offset/1000));
    // Send time update to GUI
    MsgClasses::CmdUpdateProgramEndTime* commandUpdateProgramEndTime(new MsgClasses::CmdUpdateProgramEndTime(5000, offset/1000));
    Q_ASSERT(commandUpdateProgramEndTime);
    Global::tRefType tfRef = GetNewCommandRef();
    SendCommand(tfRef, Global::CommandShPtr_t(commandUpdateProgramEndTime));

    // Update end time and prompt the MSG box to info end user
    m_EndTimeAndStepTime.UserSetEndTime += offset;
    QDateTime endTime = QDateTime::fromMSecsSinceEpoch(m_EndTimeAndStepTime.UserSetEndTime);
    int diff = endTime.secsTo(QDateTime::currentDateTime());
    endTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(-diff);
    if (offset > 1000*60) //If offset is less than one minute, we will NOT notify end user
    {
        Global::TimeFormat timeFormat = mp_DataManager->GetUserSettings()->GetTimeFormat();
        switch(mp_DataManager->GetUserSettings()->GetDateFormat())
        {
        case Global::DATE_INTERNATIONAL:
            if (timeFormat == Global::TIME_12)
            {
                QString str = endTime.date().toString("dd.MM.yyyy") + " " + endTime.time().toString("hh:mm ap");
                RaiseEvent(EVENT_SCHEDULER_PAUSE_ENDTIME_UPDATE, QStringList()<<str);
            }
            else
            {
                RaiseEvent(EVENT_SCHEDULER_PAUSE_ENDTIME_UPDATE, QStringList()<<endTime.toString("dd.MM.yyyy hh:mm"));
            }
            break;
        case Global::DATE_ISO:
            if (timeFormat == Global::TIME_12)
            {
                QString str = endTime.date().toString("yyyy-MM-dd") + " " + endTime.time().toString("hh:mm ap");
                RaiseEvent(EVENT_SCHEDULER_PAUSE_ENDTIME_UPDATE, QStringList()<<str);
            }
            else
            {
                RaiseEvent(EVENT_SCHEDULER_PAUSE_ENDTIME_UPDATE, QStringList()<<endTime.toString("yyyy-MM-dd hh:mm"));
            }
            break;
        case Global::DATE_US:
            if (timeFormat == Global::TIME_12)
            {
                QString str = endTime.date().toString("MM/dd/yyyy") + " " + endTime.time().toString("hh:mm ap");
                RaiseEvent(EVENT_SCHEDULER_PAUSE_ENDTIME_UPDATE, QStringList()<<str);
            }
            else
            {
                RaiseEvent(EVENT_SCHEDULER_PAUSE_ENDTIME_UPDATE, QStringList()<<endTime.toString("MM/dd/yyyy hh:mm"));
            }
            break;
        default:
            if (timeFormat == Global::TIME_12)
            {
                QString str = endTime.date().toString("yyyy-MM-dd") + " " + endTime.time().toString("hh:mm ap");
                RaiseEvent(EVENT_SCHEDULER_PAUSE_ENDTIME_UPDATE, QStringList()<<str);
            }
            else
            {
                RaiseEvent(EVENT_SCHEDULER_PAUSE_ENDTIME_UPDATE, QStringList()<<endTime.toString("yyyy-MM-dd hh:mm"));
            }
            break;
        }
    }
#endif
}

void SchedulerMainThreadController::SendSystemBusy2GUI(bool isBusy)
{
    MsgClasses::CmdProgramAcknowledge* commandPtr = NULL;
    if (isBusy)
    {
        commandPtr = new MsgClasses::CmdProgramAcknowledge(5000,DataManager::SYSTEM_BUSY);
    }
    else
    {
      commandPtr = new MsgClasses::CmdProgramAcknowledge(5000,DataManager::SYSTEM_IDLE);
    }
    Q_ASSERT(commandPtr);
    Global::tRefType fRef = GetNewCommandRef();
    SendCommand(fRef, Global::CommandShPtr_t(commandPtr));
}

void SchedulerMainThreadController::SendProgramAcknowledge(DataManager::ProgramAcknownedgeType_t ackType)
{
    MsgClasses::CmdProgramAcknowledge* commandPtrWaitForDrain(new MsgClasses::CmdProgramAcknowledge(5000, ackType));
    Q_ASSERT(commandPtrWaitForDrain);
    Global::tRefType fRef = GetNewCommandRef();
    SendCommand(fRef, Global::CommandShPtr_t(commandPtrWaitForDrain));
}

NonDeviceCommand_t SchedulerMainThreadController::PeekNonDeviceCommand()
{
    NonDeviceCommand_t cmd_type;

    cmd_type.Cmd = CTRL_CMD_UNKNOWN;

    if(m_SchedulerCmdQueue.isEmpty())
    {
        cmd_type.Cmd = CTRL_CMD_NONE;
        return cmd_type;
        //return CTRL_CMD_NONE;
    }
    m_Mutex.lock();
    Global::CommandShPtr_t pt = m_SchedulerCmdQueue.dequeue();
    m_Mutex.unlock();

    MsgClasses::CmdQuitAppShutdown* pCmdShutdown = dynamic_cast<MsgClasses::CmdQuitAppShutdown*>(pt.GetPointerToUserData());
    if(pCmdShutdown)
    {
        cmd_type.Cmd = CTRL_CMD_SHUTDOWN;
        return cmd_type;
        //return CTRL_CMD_SHUTDOWN;
    }

    MsgClasses::CmdBottleCheck* pCmdBottleCheck = dynamic_cast<MsgClasses::CmdBottleCheck*>(pt.GetPointerToUserData());
    if(pCmdBottleCheck)
    {
        m_CheckOvenCover = true;
        cmd_type.Cmd = CTRL_CMD_BOTTLE_CHECK;
        //return CTRL_CMD_BOTTLE_CHECK;
    }

    MsgClasses::CmdTakeOutSpecimenFinished* pCmdTakeOutSpecimenFinished = dynamic_cast<MsgClasses::CmdTakeOutSpecimenFinished*>(pt.GetPointerToUserData());
    if(pCmdTakeOutSpecimenFinished)
    {
        cmd_type.Cmd = CTRL_CMD_TAKEOUT_SPECIMEN_FINISH;
        return cmd_type;
        //return CTRL_CMD_TAKEOUT_SPECIMEN_FINISH;
    }

    MsgClasses::CmdProgramAction* pCmdProgramAction = dynamic_cast<MsgClasses::CmdProgramAction*>(pt.GetPointerToUserData());
    if(pCmdProgramAction)
    {
        cmd_type.Retort_name = pCmdProgramAction->GetRetortName();

        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_START)
        {
            //if (PSSM_PAUSE != m_CurrentStepState)
            {
                //m_NewProgramID = pCmdProgramAction->GetProgramID();
                m_SchedulerStateHandlerList[cmd_type.Retort_name]->SetNewProgramID(pCmdProgramAction->GetProgramID());
                //m_delayTime = pCmdProgramAction->DelayTime();
                m_ReagentExpiredFlag = pCmdProgramAction->GetReagentExpiredFlag();
                LogDebug(QString("Get the delay time: %1 seconds.").arg(pCmdProgramAction->DelayTime()));
            }
            cmd_type.Cmd = CTRL_CMD_START;
            return cmd_type;
            //return CTRL_CMD_START;
        }
        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_PAUSE)
        {
            SchedulerStateMachine_t currentState;// = m_SchedulerMachine->GetCurrentState();
            if (PSSM_FILLING == currentState || PSSM_RV_MOVE_TO_SEAL == currentState || PSSM_PROCESSING == currentState)
            {
                m_PauseStartTime = 0;
                this->SendOutErrMsg(DCL_ERR_DEV_INTER_PAUSE, false);
                cmd_type.Cmd = CTRL_CMD_NONE;
                return cmd_type;
                //return CTRL_CMD_NONE;
            }
            else
            {
                switch (currentState)
                {
                case PSSM_FILLING_LEVELSENSOR_HEATING:
                case PSSM_FILLING_RVROD_HEATING:
                    m_StateAtPause = currentState;
                    break;
                case PSSM_RV_MOVE_TO_TUBE:
                case PSSM_DRAINING:
                    m_StateAtPause = PSSM_RV_POS_CHANGE;
                    break;
                case PSSM_RV_POS_CHANGE:
                    m_StateAtPause = PSSM_STEP_PROGRAM_FINISH;
                    break;
                default:
                    break;
                }
                m_PauseStartTime = QDateTime::currentMSecsSinceEpoch();
                cmd_type.Cmd = CTRL_CMD_PAUSE;
                return cmd_type;
            }

        }
        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_ABORT)
        {
            cmd_type.Cmd = CTRL_CMD_ABORT;
            return cmd_type;
            //return CTRL_CMD_ABORT;
        }
        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_DRAIN)
        {
            qDebug()<<"********************** program drain...";
            cmd_type.Cmd = CTRL_CMD_DRAIN;
            return cmd_type;
            //return CTRL_CMD_DRAIN;
        }
        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_DRAIN_SR)
        {
            cmd_type.Cmd = CTRL_CMD_DRAIN_SR;
            return cmd_type;
            //return CTRL_CMD_DRAIN_SR;
        }
        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_OVEN_COVER_OPEN)
        {
            cmd_type.Cmd = CTRL_CMD_OVEN_COVER_OPEN;
            return cmd_type;
            //return CTRL_CMD_OVEN_COVER_OPEN;
        }
        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_RETORT_COVER_OPEN)
        {
            cmd_type.Cmd = CTRL_CMD_RETORT_COVER_OPEN;
            return cmd_type;
            //return CTRL_CMD_RETORT_COVER_OPEN;
        }
        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_POWER_FAILURE_MSG)
        {
            cmd_type.Cmd = CTRL_CMD_POWER_FAILURE_MEG;
            return cmd_type;
            //return CTRL_CMD_POWER_FAILURE_MEG;
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
            cmd_type.Cmd = CTRL_CMD_RS_RV_GET_ORIGINAL_POSITION_AGAIN;
        }
        if(cmd == "rc_restart")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RC_RESTART;
        }
        if (cmd == "rc_report")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RC_REPORT;
        }
        if(cmd == "rs_standby")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RS_STANDBY;
        }
        if(cmd == "rs_standby_withtissue")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RS_STANDBY_WITHTISSUE;
        }
        if (cmd == "rs_heatingerr30sretry")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RS_HEATINGERR30SRETRY;
        }
        if (cmd == "rs_pressureoverrange_3sretry")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RS_PRESSUREOVERRANGE_3SRETRY;
        }
        if (cmd == "rs_tsensorerr3minretry")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RS_TSENSORERR3MINRETRY;
        }
        if (cmd == "rc_levelsensor_heating_overtime")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RC_LEVELSENSOR_HEATING_OVERTIME;
        }
        if (cmd == "rc_pressure")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RC_PRESSURE;
        }
        if (cmd == "rc_vacuum")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RC_VACUUM;
        }
        if (cmd == "rc_filling")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RC_FILLING;
        }
        if (cmd == "rc_draining")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RC_DRAINING;
        }
        if (cmd == "rc_forcedraining")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RC_FORCEDRAINING;
        }

        if (cmd == "rs_drainatonce")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RS_DRAINATONCE;
        }
        if (cmd == "rc_bottlecheck_i")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RC_BOTTLECHECK_I;
        }
        if (cmd == "rs_fillingafterflush")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RS_FILLINGAFTERFLUSH;
        }
        if (cmd == "rs_check_blockage")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RS_CHECK_BLOCKAGE;
        }
        if (cmd == "rs_pause")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RS_PAUSE;
        }
        if (cmd == "rs_rv_waitingtempup")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RS_RV_WAITINGTEMPUP;
        }
        if (cmd == "rs_tissue_protect")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RS_TISSUE_PROTECT;
        }
        if (cmd == "user_response_pause_alarm")
        {
            //warning action not need eventkey
            //m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_USER_RESPONSE_PAUSE_ALARM;
        }
        if (cmd == "user_confirm_lock_rtlid_idle")
        {
            //warning action not need eventkey
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_USER_PRESS_OK_BUTTON_LOCK_RTLID_IDLE;
        }
        if (cmd == "rc_check_rtlock")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RC_CHECK_RTLOCK;
        }
        if (cmd == "rc_reheating")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RC_REHEATING;
        }
        if (cmd == "rc_reheating_5mintimeout")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RC_REHEATING_5MINTIMEOUT;
        }
        if (cmd == "rs_reagentcheck")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RS_REAGENTCHECK;
        }
        if (cmd == "rs_rv_movetoposition3.5")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RS_RV_MOVETOSEALPOSITION;
        }
        if (cmd == "rs_checkremotealarmstatus")
        {
            //warning action not need eventkey
            //m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RS_CHECKREMOTEALARMSTATUS;
        }
        if (cmd == "rs_checklocalalarmstatus")
        {
            //warning action not need eventkey
            //m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RS_CHECKLOCALALARMSTATUS;
        }
        if (cmd == "rs_abort")
        {
            m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_RS_ABORT;
        }
        if (cmd == "rc_turnon_remotealarm")
        {
            // Turn on remote alarm
            Global::AlarmHandler::Instance().setAlarm(pCmdSystemAction->GetEventKey(),Global::ALARM_ERROR,true);
            HandleRmtLocAlarm(CTRL_CMD_ALARM_RMT_ON);
            this->RaiseError(pCmdSystemAction->GetEventKey(), DCL_ERR_FCT_CALL_SUCCESS, 0, true);
//            return CTRL_CMD_NONE;
        }
        if (cmd == "checkovencover")
        {
            //warning action not need eventkey
            //m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_CHECK_OVEN_COVER;
        }
        if (cmd == "open_oven_change_heating_paraffin")
        {
            //warning action not need eventkey
            //m_EventKey = pCmdSystemAction->GetEventKey();
            cmd_type.Cmd = CTRL_CMD_OPEN_OVEN_CHANGE_HEATING_PARAFFIN;
        }
        if (cmd == "rs_pause_cmd_yes")
        {
            m_PauseStartTime = QDateTime::currentMSecsSinceEpoch();
            SchedulerStateMachine_t currentState;// = m_SchedulerMachine->GetCurrentState();
            if (PSSM_FILLING == currentState || PSSM_RV_MOVE_TO_SEAL == currentState || PSSM_PROCESSING == currentState)
            {
                LogDebug("Input CTRL_CMD_RS_PAUSE_CMD_YES");
                m_StateAtPause = currentState;
                cmd_type.Cmd = CTRL_CMD_RS_PAUSE_CMD_YES;
            }
            else
            {
                switch (currentState)
                {
                case PSSM_FILLING_LEVELSENSOR_HEATING:
                case PSSM_FILLING_RVROD_HEATING:
                    m_StateAtPause = currentState;
                    break;
                case PSSM_RV_MOVE_TO_TUBE:
                case PSSM_DRAINING:
                    m_StateAtPause = PSSM_RV_POS_CHANGE;
                    break;
                case PSSM_RV_POS_CHANGE:
                    m_StateAtPause = PSSM_STEP_PROGRAM_FINISH;
                    break;
                default:
                    break;
                }
                LogDebug("Input CTRL_CMD_PAUSE");
                cmd_type.Cmd = CTRL_CMD_PAUSE;
            }
        }
        if (cmd == "rs_pause_cmd_no")
        {
            m_PauseStartTime = QDateTime::currentMSecsSinceEpoch();
            SchedulerStateMachine_t currentState;// = m_SchedulerMachine->GetCurrentState();
            switch (currentState)
            {
            case PSSM_FILLING_LEVELSENSOR_HEATING:
            case PSSM_FILLING_RVROD_HEATING:
                m_StateAtPause = currentState;
                break;
            case PSSM_FILLING:
            case PSSM_RV_MOVE_TO_SEAL:
            case PSSM_PROCESSING:
                m_StateAtPause = PSSM_PROCESSING;
                break;
            case PSSM_RV_MOVE_TO_TUBE:
            case PSSM_DRAINING:
                m_StateAtPause = PSSM_RV_POS_CHANGE;
                break;
            case PSSM_RV_POS_CHANGE:
                m_StateAtPause = PSSM_STEP_PROGRAM_FINISH;
                break;
            default:
                break;
            }
            LogDebug("Select CTRL_CMD_PAUSE_NO");
            cmd_type.Cmd = CTRL_CMD_PAUSE;
        }
        if (cmd == "rs_pause_cmd_cancel")
        {
            MsgClasses::CmdProgramAcknowledge* CmdPauseCmdCancel = new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_PAUSE_ENABLE);
            Q_ASSERT(CmdPauseCmdCancel);
            Global::tRefType fRef = GetNewCommandRef();
            SendCommand(fRef, Global::CommandShPtr_t(CmdPauseCmdCancel));
            cmd_type.Cmd = CTRL_CMD_NONE;
        }
        if (cmd.startsWith("ALARM_", Qt::CaseInsensitive))
        {
            QString str = cmd;
            int opcode = str.remove("ALARM_", Qt::CaseInsensitive).toInt();
            switch (opcode) {
            case 5:
                cmd_type.Cmd =  CTRL_CMD_ALARM_RMT_ON;
            case 4:
                cmd_type.Cmd =  CTRL_CMD_ALARM_RMT_OFF;
            case 3:
                cmd_type.Cmd =  CTRL_CMD_ALARM_LOC_ON;
            case 2:
                cmd_type.Cmd =  CTRL_CMD_ALARM_LOC_OFF;
            case 1:
                cmd_type.Cmd =  CTRL_CMD_ALARM_DEVICE_ON;
            case 0:
                cmd_type.Cmd =  CTRL_CMD_ALARM_DEVICE_OFF;
            case -1:
                cmd_type.Cmd =  CTRL_CMD_ALARM_ALL_OFF;
            }
        }
        if(cmd == "rs_shutdown")
        {
            cmd_type.Cmd = CTRL_CMD_RS_SHUTDOWN;
        }
    }
    return cmd_type;
    //return CTRL_CMD_UNKNOWN;

}//When end of this function, the command in m_SchedulerCmdQueue will be destrory by share pointer (CommandShPtr_t) mechanism

void SchedulerMainThreadController::SendTissueProtectMsg(bool flag)
{
    MsgClasses::CmdProgramAcknowledge* CmdTissueProtectDone = NULL;
    if (flag)
    {
        CmdTissueProtectDone = new MsgClasses::CmdProgramAcknowledge(5000,DataManager::TISSUE_PROTECT_PASSED);
    }
    else
    {
        CmdTissueProtectDone = new MsgClasses::CmdProgramAcknowledge(5000,DataManager::TISSUE_PROTECT_PASSED_WARNING);
    }
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
    CheckResuemFromPause(PSSM_STEP_PROGRAM_FINISH);
    m_PssmStepFinSeq = 0;
    m_TickTimer.start();
}

void SchedulerMainThreadController::OnEnterPssMProgFin()
{
    m_TickTimer.start();
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

void SchedulerMainThreadController::CheckCarbonFilterExpired()
{
    if (Global::GetMantainenceFirstRecordFlag())
        return;

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
    m_SchedulerCmdQueue.enqueue(Global::CommandShPtr_t(new MsgClasses::CmdProgramAction(Cmd.GetRetortName(), Cmd.GetTimeout(), Cmd.GetProgramID(), Cmd.ProgramActionType(),
                                                                                        Cmd.DelayTime(), Cmd.ProgramRunDuration(), Cmd.GetReagentExpiredFlag())));
    m_Mutex.unlock();
    this->SendAcknowledgeOK(Ref);

    QString ProgramName;
    quint32 ProgramNameID = 0;
    if(mp_DataManager && mp_DataManager->GetProgramList() && mp_DataManager->GetProgramList()->GetProgram(Cmd.GetProgramID()))
    {
        ProgramName = mp_DataManager->GetProgramList()->GetProgram(Cmd.GetProgramID())->GetName();
        if(mp_DataManager->GetProgramList()->GetProgram(Cmd.GetProgramID())->IsLeicaProgram())
        {
            ProgramNameID = mp_DataManager->GetProgramList()->GetProgram(Cmd.GetProgramID())->GetNameID().toUInt();
        }

    }

    SchedulerStateMachine_t stepState;// = m_SchedulerMachine->GetCurrentState();
    if ((PSSM_PAUSE != stepState) && (Cmd.ProgramActionType() == DataManager::PROGRAM_START))
    {
        if(Cmd.ProgramRunDuration() > 0) // start new program
        {
            QDateTime EndDateTime = Global::AdjustedTime::Instance().GetCurrentDateTime().addSecs(Cmd.ProgramRunDuration());
            //m_EndTimeAndStepTime.UserSetEndTime = QDateTime::currentDateTime().addSecs(Cmd.ProgramRunDuration()).toMSecsSinceEpoch();
            if(ProgramNameID == 0)
            {
                RaiseEvent(EVENT_SCHEDULER_REC_START_PROGRAM, QStringList()<<ProgramName
                           <<EndDateTime.toString()); //log
            }
            else
            {
                Global::EventObject::Instance().RaiseEvent(EVENT_SCHEDULER_REC_START_PROGRAM,Global::tTranslatableStringList()
                                                           <<Global::TranslatableString(ProgramNameID) << EndDateTime.toString());
            }
        }
        else
        {
            if(ProgramNameID == 0)
            {
                RaiseEvent(EVENT_SCHEDULER_REC_CONTINUE_PROGRAM, QStringList()<<ProgramName); //log
            }
            else
            {
                Global::EventObject::Instance().RaiseEvent(EVENT_SCHEDULER_REC_CONTINUE_PROGRAM,Global::tTranslatableStringList()
                                                           <<Global::TranslatableString(ProgramNameID));
            }
        }
        //CheckCarbonFilterExpired();
        return;
    }
    else if(Cmd.ProgramActionType() == DataManager::PROGRAM_PAUSE)
    {
        if(ProgramNameID == 0)
        {
            RaiseEvent(EVENT_SCHEDULER_REC_PAUSE_PROGRAM,QStringList()<<ProgramName); //log
        }
        else
        {
            Global::EventObject::Instance().RaiseEvent(EVENT_SCHEDULER_REC_PAUSE_PROGRAM,Global::tTranslatableStringList()
                                                   <<Global::TranslatableString(ProgramNameID));
        }
    }
    else if(Cmd.ProgramActionType() == DataManager::PROGRAM_ABORT)
    {
        if(ProgramNameID == 0)
        {
            RaiseEvent(EVENT_SCHEDULER_REC_ABORT_PROGRAM,QStringList()<<ProgramName); //log
        }
        else
        {
            Global::EventObject::Instance().RaiseEvent(EVENT_SCHEDULER_REC_ABORT_PROGRAM,Global::tTranslatableStringList()
                                                   <<Global::TranslatableString(ProgramNameID));
        }
    }
    else if(Cmd.ProgramActionType() == DataManager::PROGRAM_DRAIN)
    {
        if(ProgramNameID == 0)
        {
            RaiseEvent(EVENT_SCHEDULER_REC_DRAIN_PROGRAM,QStringList()<<ProgramName); //log
        }
        else
        {
            Global::EventObject::Instance().RaiseEvent(EVENT_SCHEDULER_REC_DRAIN_PROGRAM,Global::tTranslatableStringList()
                                                   <<Global::TranslatableString(ProgramNameID));
        }
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
//    m_ProcessCassetteCount += Cmd.CassetteCount();
//    m_ProcessCassetteNewCount = Cmd.CassetteCount();
    this->SendAcknowledgeOK(Ref);
    SchedulerStateMachine_t previousState;// = m_SchedulerMachine->GetPreviousState();
    if (previousState == PSSM_RV_MOVE_TO_SEAL ||
        previousState == PSSM_PROCESSING ||
        previousState == PSSM_RV_MOVE_TO_TUBE)
    {
        //this->UpdateStationReagentStatus(true);
    }
}

void SchedulerMainThreadController::OnProgramSelected(Global::tRefType Ref, const MsgClasses::CmdProgramSelected & Cmd)
{
    this->SendAcknowledgeOK(Ref);
    //m_delayTime = 0;
    QString curProgramID = Cmd.GetProgramID();
    qDebug()<<"************* on program selected id:"<<curProgramID;
    qDebug()<<"************* on program selected retort:"<<Cmd.GetRetortId();

    CSchedulerStateHandler* stateHandler = m_SchedulerStateHandlerList[Cmd.GetRetortId()].data();
    if (stateHandler)
    {
        m_CurrentBottlePosition.ReagentGrpId = "";
        m_CurrentBottlePosition.RvPos = RV_UNDEF;
        stateHandler->ProgramSelectedReply(Ref, curProgramID, Cmd.ParaffinStepIndex());
    }
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

void SchedulerMainThreadController::OnDCLConfigurationFinished(ReturnCode_t RetCode, QList<QString> retorts)
{
    // Create Instrument
//    m_pInstrumentManager->Initialize(retorts);

    // Turn off local/remote alarm by default
    CmdRmtLocAlarm *cmd = new CmdRmtLocAlarm(500, m_Sender);
    cmd->SetRmtLocOpcode(-1);
    m_SchedulerCommandProcessor->pushCmd(cmd);

    bool working = false;
    if(RetCode == DCL_ERR_FCT_CALL_SUCCESS)
    {
        RaiseEvent(EVENT_SCHEDULER_SLAVE_BOARD_INITIALIZED_SUCCESSFULLY);//log
        ReturnCode_t retCode;
        //hardware not ready yet
        m_SchedulerCommandProcessor->pushCmd(new CmdPerTurnOnMainRelay(500, m_Sender));
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
#if 0
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
#endif
    mp_ProgramSelfTest = QSharedPointer<CProgramSelfTest>(new CProgramSelfTest(this));

	// Create HeatingStrategy
    mp_HeatingStrategy = QSharedPointer<HeatingStrategy>(new HeatingStrategy(this, m_SchedulerCommandProcessor, mp_DataManager));
//    m_SchedulerMachine = new CSchedulerStateMachine(this);
//    //for debug
//    LogDebug(QString("Current state of Scheduler is: %1").arg(m_SchedulerMachine->GetCurrentState()));
//    m_SchedulerMachine->Start();
//    //m_TickTimer.start();
   foreach(auto retort, retorts)
   {

       if (!retort.isEmpty())
       {
           qDebug()<<"Get retort name:"<<retort;
           if (m_SchedulerStateHandlerList.find(retort) == m_SchedulerStateHandlerList.end())
               m_SchedulerStateHandlerList.insert(retort,
                                                  QSharedPointer<CSchedulerStateHandler>(new CSchedulerStateHandler(retort, this, mp_DataManager)));
       }
   }

    qDebug()<<"************************ DCL configuration finished....: state handler size:"<<m_SchedulerStateHandlerList.size();
    m_TickTimer.start();
//    m_pInstrumentManager = new InstrumentManager("Common", m_pEventDispatcher, this);

//    m_pInstrumentManager->Initialize(retorts);
//        m_pInstrumentManager->CreateStateMachine();
//    m_pInstrumentManager->Start();

}

void SchedulerMainThreadController::HardwareMonitor(const QString& StepID)
{
 #if 0
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
        RaiseError(0, DCL_ERR_DEV_MC_REMOTEALARM_UNCONNECTED, Scenario, true);
        m_RemoteAlarmPreviousStatus = 1;
    }
    else if(0 == strctHWMonitor.RemoteAlarmStatus && m_RemoteAlarmPreviousStatus == 1)// alarm was plugged
    {
        m_RemoteAlarmPreviousStatus = 0;
        RaiseError(0, DCL_ERR_DEV_MC_REMOTEALARM_UNCONNECTED, 0,true, false);
    }
    if (1 == strctHWMonitor.LocalAlarmStatus && m_LocalAlarmPreviousStatus < 1) //alarm was unplugged
    {
        RaiseError(0, DCL_ERR_DEV_MC_LOCALALARM_UNCONNECTED, Scenario, true);
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

    if(strctHWMonitor.OvenLidStatus != UNDEFINED_VALUE)
    {
        if(((m_OvenLidStatus == 0) ||(m_OvenLidStatus == UNDEFINED_VALUE))&&(strctHWMonitor.OvenLidStatus == 1))
        {
            MsgClasses::CmdLockStatus* commandPtr(new MsgClasses::CmdLockStatus(5000, DataManager::PARAFFIN_BATH_LOCK, false));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        }
        if(((m_OvenLidStatus == 1) || (m_OvenLidStatus == UNDEFINED_VALUE))&&(strctHWMonitor.OvenLidStatus == 0))
        {
            MsgClasses::CmdLockStatus* commandPtr(new MsgClasses::CmdLockStatus(5000, DataManager::PARAFFIN_BATH_LOCK, true));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        }
        m_OvenLidStatus = strctHWMonitor.OvenLidStatus;
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

#endif
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
    m_pEventDispatcher->IncomingEvent(TPEventArgs<Scheduler::SchedulerCommandShPtr_t>::CreateEvent(CmdPtr));
    m_MutexDeviceControlCmdQueue.unlock();
}

void SchedulerMainThreadController::RcBottleCheckI()
{
    if(m_CurrentBottlePosition.RvPos != RV_UNDEF && !m_CurrentBottlePosition.ReagentGrpId.isEmpty())
    {
        LogDebug(QString("BottleCheckI check for tube %1").arg(m_CurrentBottlePosition.RvPos));
        CmdIDBottleCheck* cmd  = new CmdIDBottleCheck(500, m_Sender);
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

void SchedulerMainThreadController::ReleasePressure(const QString& Sender)
{
    RaiseEvent(EVENT_SCHEDULER_RELEASE_PREASURE);
    m_SchedulerCommandProcessor->pushCmd(new CmdALReleasePressure(500, Sender));
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
        m_SchedulerCommandProcessor->pushCmd(new CmdALTurnOffFan(500, m_Sender));
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
    m_SchedulerCommandProcessor->pushCmd(new CmdPerTurnOnMainRelay(500, m_Sender), false);

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
        m_SchedulerCommandProcessor->pushCmd(new CmdALTurnOnFan(500, m_Sender));
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
        temp1 = m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempALLevelSensor;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("LevelSensor", temp1) )
            return false;
        break;
    case LATUBE1:
        temp1 = m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempALTube1;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("LA_Tube1", temp1) )
            return false;
        break;
    case LATUBE2:
        temp1 = m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempALTube2;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("LA_Tube2", temp1) )
            return false;
        break;
    case RV:
        temp1 = m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempRV1;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("RVRod", temp1) )
            return false;
        temp2 = m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempRV2;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("RVOutle", temp2) )
            return false;
        break;
    case RETORT:
        temp1 = m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempRTSide;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("RTSide", temp1) )
            return false;
        temp2 = m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempRTBottom1;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("RTBottom", temp2) )
            return false;
        if (false == mp_HeatingStrategy->CheckRTBottomsDifference(m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempRTBottom1,
                                                         m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempRTBottom2))
        {
            return false;
        }
        temp3 = m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempRTBottom2;
        if(false == mp_HeatingStrategy->CheckSensorTempOverRange("RTBottom", temp3))
        {
            return false;
        }
        break;
    case RETORT_NOSIGNAL:
        temp1 = m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempRTSide;
        temp2 = m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempRTBottom1;
        temp3 = m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempRTBottom2;
        if (qFuzzyCompare(temp1,299) || qFuzzyCompare(temp2,299) || qFuzzyCompare(temp3,299))
        {
            return false;
        }
        break;
    case OVEN:
        temp1 = m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempOvenTop;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("OvenTop", temp1) )
            return false;
        temp2 = m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempOvenBottom1;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("OvenBottom", temp2) )
            return false;
        temp3 = m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempOvenBottom2;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("OvenBottom", temp3))
            return false;
        break;
    case ASB5:
        temp1 = m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempRTSide;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("RTSide", temp1) )
            return false;
        temp2 = m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempRTBottom1;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("RTBottom", temp2) )
            return false;
        if (false == mp_HeatingStrategy->CheckRTBottomsDifference(m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempRTBottom1,
                                                         m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempRTBottom2))
        {
            return false;
        }
        temp3 = m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempRTBottom2;
        if(false == mp_HeatingStrategy->CheckSensorTempOverRange("RTBottom", temp3))
        {
            return false;
        }
        temp1 = m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempOvenTop;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("OvenTop", temp1) )
            return false;
        temp2 = m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempOvenBottom1;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("OvenBottom", temp2) )
            return false;
        temp3 = m_SchedulerCommandProcessor->HardwareMonitor(m_Sender).TempOvenBottom2;
        if( false == mp_HeatingStrategy->CheckSensorTempOverRange("OvenBottom", temp3))
            return false;
        break;
    default:
        break;
    }
    return true;
}

void SchedulerMainThreadController::FillRsTissueProtect(const QString& StationID, bool EnableInsufficientCheck, bool SafeReagent4Paraffin)
{
    LogDebug("Send cmd to DCL to Fill in Rs_Tissue_Protect");
    CmdALFilling* cmd  = new CmdALFilling(500, m_Sender);
    cmd->SetDelayTime(0);

    cmd->SetEnableInsufficientCheck(EnableInsufficientCheck);
    cmd->SetSafeReagent4Paraffin(SafeReagent4Paraffin);
    m_SchedulerCommandProcessor->pushCmd(cmd);

    //if(!m_IsCleaningProgram)
    {
        //m_ProgramStatusInfor.SetLastReagentGroup(m_CurProgramStepInfo.reagentGroup);
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

void SchedulerMainThreadController::Pause()
{
    RaiseEvent(EVENT_SCHEDULER_OVEN_PAUSE);

    // Update currrent scenario
    this->UpdateCurrentScenario();

    // Disable pause button
    this->DisablePauseButton();

    GetHeatingStrategy()->StopTemperatureControl("LevelSensor");
    //First of all, release pressure
    m_SchedulerCommandProcessor->pushCmd(new CmdALReleasePressure(500,m_Sender), false);

#if 0
    //update the remaining time for the current step
    if (PSSM_PROCESSING == m_CurrentStepState)
    {
        m_IsProcessing = false;
        m_CurProgramStepInfo.durationInSeconds = m_CurProgramStepInfo.durationInSeconds - ((QDateTime::currentDateTime().toMSecsSinceEpoch() - m_TimeStamps.CurStepSoakStartTime) / 1000);
    }
#endif

    LogDebug("SchedulerMainThreadController Paused");
    //m_TimeStamps.PauseStartTime = QDateTime::currentMSecsSinceEpoch();
//    m_Is5MinPause = false;
//    m_Is10MinPause = false;
//    m_Is15MinPause = false;
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

void SchedulerMainThreadController::UpdateCurrentScenario()
{
//    SchedulerStateMachine_t currentState = m_SchedulerMachine->GetCurrentState();
//    quint32 state = currentState & 0xFF;
//    if (state == SM_INIT || state == SM_IDLE || state == SM_BUSY)
//    {
//        m_CurrentScenario = GetScenarioBySchedulerState(m_SchedulerMachine->GetCurrentState(), m_CurProgramStepInfo.reagentGroup);
//    }
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
            // For events 500020081, 500020181,500020082, 500020182, 500020083 and 500020183, Scheduler will NOT enter error state
            if (retCode == DCL_ERR_DEV_WAXBATH_SENSORUP_HEATING_OUTOFTARGETRANGE_HIGH ||
                retCode == DCL_ERR_DEV_WAXBATH_SENSORUP_HEATING_OUTOFTARGETRANGE_LOW ||
                retCode == DCL_ERR_DEV_WAXBATH_SENSORDOWN1_HEATING_OUTOFTARGETRANGE_HIGH ||
                retCode == DCL_ERR_DEV_WAXBATH_SENSORDOWN1_HEATING_OUTOFTARGETRANGE_LOW ||
                retCode == DCL_ERR_DEV_WAXBATH_SENSORDOWN2_HEATING_OUTOFTARGETRANGE_HIGH ||
                retCode == DCL_ERR_DEV_WAXBATH_SENSORDOWN2_HEATING_OUTOFTARGETRANGE_LOW)
            {
                SendOutErrMsg(retCode, false);
            }
            else
            {
                SendOutErrMsg(retCode, true);
            }
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
    if (reportError8.instanceID != 0 && m_ReportExhaustFanWarning)
    {
        LogDebug(QString("Exhaust Fan Current out of range, Current is: %1").arg(reportError8.errorData));

        m_ReportExhaustFanWarning = false;
        SendOutErrMsg(DCL_ERR_DEV_LA_STATUS_EXHAUSTFAN, false);
        if(m_IsErrorStateForHM)
        {
            return;
        }
    }

    if (reportError9.instanceID != 0)
    {
        LogDebug(QString("Current out of range, the current is :%1").arg(reportError9.errorData));

        SendOutErrMsg(DCL_ERR_DEV_RV_HEATING_CURRENT_OUTOFRANGE);
        if(m_IsErrorStateForHM)
        {
            return;
        }
    }

    // For voltage related
    quint16 slave3UpperLimit = (m_SlaveAttrList[0].Voltagerated24VDC + m_SlaveAttrList[0].VoltageTolerance24VDC)*1000;
    quint16 slave3LowerLimit = (m_SlaveAttrList[0].Voltagerated24VDC - m_SlaveAttrList[0].VoltageTolerance24VDC)*1000;
    if (strctHWMonitor.Slave3Voltage < slave3LowerLimit || strctHWMonitor.Slave3Voltage > slave3UpperLimit)
    {
        LogDebug(QString("slave 3 voltage is: %1").arg(strctHWMonitor.Slave3Voltage));
        SendOutErrMsg(DCL_ERR_DEV_MC_VOLTAGE_24V_ASB3_OUTOFRANGE);
        if(m_IsErrorStateForHM)
        {
            return ;
        }
    }

    quint16 slave5UpperLimit =  (m_SlaveAttrList[1].Voltagerated24VDC + m_SlaveAttrList[1].VoltageTolerance24VDC)*1000;
    quint16 slave5LowerLimit =  (m_SlaveAttrList[1].Voltagerated24VDC - m_SlaveAttrList[1].VoltageTolerance24VDC)*1000;
    if (strctHWMonitor.Slave5Voltage < slave5LowerLimit || strctHWMonitor.Slave5Voltage > slave5UpperLimit)
    {
        LogDebug(QString("slave 5 voltage is: %1").arg(strctHWMonitor.Slave5Voltage));
        SendOutErrMsg(DCL_ERR_DEV_MC_VOLTAGE_24V_ASB5_OUTOFRANGE);
        if(m_IsErrorStateForHM)
        {
            return;
        }
    }

    quint16 slave15UpperLimit = (m_SlaveAttrList[2].Voltagerated24VDC + m_SlaveAttrList[2].VoltageTolerance24VDC)*1000;
    quint16 slave15LowerLimit = (m_SlaveAttrList[2].Voltagerated24VDC - m_SlaveAttrList[2].VoltageTolerance24VDC)*1000;
    if (strctHWMonitor.Slave15Voltage < slave15LowerLimit || strctHWMonitor.Slave15Voltage > slave15UpperLimit)
    {
        LogDebug(QString("slave 15 voltage is: %1").arg(strctHWMonitor.Slave15Voltage));
        SendOutErrMsg(DCL_ERR_DEV_MC_VOLTAGE_24V_ASB15_OUTOFRANGE);
        if(m_IsErrorStateForHM)
        {
            return;
        }

    }

    // For current related
    if (strctHWMonitor.Slave3Current > m_SlaveAttrList[0].CurrentMax5VDC)
    {
        LogDebug(QString("slave 3 5V current is: %1").arg(strctHWMonitor.Slave3Current));
        SendOutErrMsg(DCL_ERR_DEV_MC_DC_5V_ASB3_OUTOFRANGE);
        if(m_IsErrorStateForHM)
        {
            return;
        }

    }

    if (strctHWMonitor.Slave5Current > m_SlaveAttrList[1].CurrentMax5VDC)
    {
        LogDebug(QString("slave 5 5V current is: %1").arg(strctHWMonitor.Slave5Current));

        SendOutErrMsg(DCL_ERR_DEV_MC_DC_5V_ASB5_OUTOFRANGE);
        if(m_IsErrorStateForHM)
        {
            return;
        }
    }

    //Check No-Signal error for Retort sensors
    if (false == this->CheckRetortTempSensorNoSignal(Scenario, strctHWMonitor.TempRTBottom1))
    {
        quint8 val = m_RetCodeCounterList[DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_NOSIGNAL];
        val++;
        if (val>=3)
        {
            m_RetCodeCounterList[DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_NOSIGNAL] = 0;
            SendOutErrMsg(DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_NOSIGNAL);
            if(m_IsErrorStateForHM)
                return;
        }
        else
        {
            m_RetCodeCounterList[DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_NOSIGNAL] = val;
        }
    }
    else
    {
        m_RetCodeCounterList[DCL_ERR_DEV_RETORT_TSENSOR1_TEMPERATURE_NOSIGNAL] = 0;
    }
    if (false == this->CheckRetortTempSensorNoSignal(Scenario, strctHWMonitor.TempRTBottom2))
    {
        quint8 val = m_RetCodeCounterList[DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_NOSIGNAL];
        val++;
        if (val>=3)
        {
            m_RetCodeCounterList[DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_NOSIGNAL] = 0;
            SendOutErrMsg(DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_NOSIGNAL);
            if(m_IsErrorStateForHM)
                return;
        }
        else
        {
            m_RetCodeCounterList[DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_NOSIGNAL] = val;
        }
    }
    else
    {
        m_RetCodeCounterList[DCL_ERR_DEV_RETORT_TSENSOR2_TEMPERATURE_NOSIGNAL] = 0;
    }
    if (false == this->CheckRetortTempSensorNoSignal(Scenario, strctHWMonitor.TempRTSide))
    {
        quint8 val = m_RetCodeCounterList[DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_NOSIGNAL];
        val++;
        if (val>=3)
        {
            m_RetCodeCounterList[DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_NOSIGNAL] = 0;
            SendOutErrMsg(DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_NOSIGNAL);
            if(m_IsErrorStateForHM)
                return;
        }
        else
        {
            m_RetCodeCounterList[DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_NOSIGNAL] = val;
        }
    }
    else
    {
        m_RetCodeCounterList[DCL_ERR_DEV_RETORT_TSENSOR3_TEMPERATURE_NOSIGNAL] = 0;
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
        quint8 val = m_RetCodeCounterList[DCL_ERR_DEV_WAXBATH_OVENCOVER_STATUS_OPEN];
        val++;
        //oven is open
        if(m_CheckOvenCover)
        {
            if ( (Scenario >= 2 && Scenario <= 260) || (Scenario >= 281 && Scenario <= 297) )
            {
                if (val>=3)
                {
                    m_RetCodeCounterList[DCL_ERR_DEV_WAXBATH_OVENCOVER_STATUS_OPEN] = 0;
                    SendOutErrMsg(DCL_ERR_DEV_WAXBATH_OVENCOVER_STATUS_OPEN, false);
                    m_CheckOvenCover = false;
                }
                else
                {
                    m_RetCodeCounterList[DCL_ERR_DEV_WAXBATH_OVENCOVER_STATUS_OPEN] = val;
                }
            }
        }
        if(Scenario >= 271 && Scenario <= 277)
        {
            if (val>=3)
            {
                m_RetCodeCounterList[DCL_ERR_DEV_WAXBATH_OVENCOVER_STATUS_OPEN] = 0;
                ReleasePressure();
                SendOutErrMsg(DCL_ERR_DEV_WAXBATH_OVENCOVER_STATUS_OPEN);
                if(m_IsErrorStateForHM)
                    return;
            }
            else
            {
                m_RetCodeCounterList[DCL_ERR_DEV_WAXBATH_OVENCOVER_STATUS_OPEN] = val;
            }
        }
    }
    else
    {
       m_RetCodeCounterList[DCL_ERR_DEV_WAXBATH_OVENCOVER_STATUS_OPEN] = 0;
    }
    if(strctHWMonitor.RetortLockStatus != UNDEFINED_VALUE)
    {
        if(1 == strctHWMonitor.RetortLockStatus /*&& m_CurrentStepState != PSSM_PAUSE*/)
        {
            quint8 val = m_RetCodeCounterList[DCL_ERR_DEV_LIDLOCK_CLOSE_STATUS_ERROR];
            val++;
            if (Scenario >=200 && Scenario <= 297)
            {
                if (val>=3)
                {
                    // Notify retort lid is opened
                    MsgClasses::CmdProgramAcknowledge* CmdRetortCoverOpen = new MsgClasses::CmdProgramAcknowledge(5000,DataManager::RETORT_COVER_OPERN);
                    Q_ASSERT(CmdRetortCoverOpen);
                    Global::tRefType fRef = GetNewCommandRef();
                    SendCommand(fRef, Global::CommandShPtr_t(CmdRetortCoverOpen));
                    m_RetCodeCounterList[DCL_ERR_DEV_LIDLOCK_CLOSE_STATUS_ERROR] = 0;
                    SendOutErrMsg(DCL_ERR_DEV_LIDLOCK_CLOSE_STATUS_ERROR);
                }
                else
                {
                    m_RetCodeCounterList[DCL_ERR_DEV_LIDLOCK_CLOSE_STATUS_ERROR] = val;
                }
            }
            else if (007 == Scenario)
            {
//                if (!m_SchedulerMachine->NonRVErrorOccuredInBottleCheck())
//                {
//                    if (val>=3)
//                    {
//                        m_RetCodeCounterList[DCL_ERR_DEV_LIDLOCK_CLOSE_STATUS_ERROR] = 0;
//                        SendOutErrMsg(DCL_ERR_DEV_LIDLOCK_CLOSE_STATUS_ERROR, false);
//                        // Notify retort lid is opened
//                        MsgClasses::CmdProgramAcknowledge* CmdRetortCoverOpen = new MsgClasses::CmdProgramAcknowledge(5000,DataManager::RETORT_COVER_OPERN);
//                        Q_ASSERT(CmdRetortCoverOpen);
//                        Global::tRefType fRef = GetNewCommandRef();
//                        SendCommand(fRef, Global::CommandShPtr_t(CmdRetortCoverOpen));
//                    }
//                    else
//                    {
//                        m_RetCodeCounterList[DCL_ERR_DEV_LIDLOCK_CLOSE_STATUS_ERROR] = val;
//                    }
//                }
            }
            if(m_IsErrorStateForHM)
                return ;
        }
        else
        {
            m_RetCodeCounterList[DCL_ERR_DEV_LIDLOCK_CLOSE_STATUS_ERROR] = 0;
        }
    }
}

void SchedulerMainThreadController::EnablePauseButton()
{
    //if (!m_IsCleaningProgram)
        EnablePauseButton(true);
}

void SchedulerMainThreadController::DisablePauseButton()
{
    //if (!m_IsCleaningProgram)
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

    CmdRmtLocAlarm *cmd = new CmdRmtLocAlarm(500, m_Sender);
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
//    if(m_CurrentScenario > 2)
//    {
//        m_ProgramStatusInfor.SetErrorFlag(1);
//    }
//    if(PSSM_PROCESSING == m_CurrentStepState)
//    {
//        if(m_completionNotifierSent)
//        {
//            MsgClasses::CmdProgramAcknowledge* command(new MsgClasses::CmdProgramAcknowledge(5000,
//                                                                    DataManager::CANCEL_PROGRAM_WILL_COMPLETE_PROMPT));
//            Global::tRefType ref = GetNewCommandRef();
//            SendCommand(ref, Global::CommandShPtr_t(command));
//            m_completionNotifierSent = false;
//        }
//    }

    ProgramAcknownedgeType_t type =  DataManager::PROGRAM_SYSTEM_EEEOR;
    MsgClasses::CmdProgramAcknowledge* commandPtrSystemError(new MsgClasses::CmdProgramAcknowledge(5000, type));
    Q_ASSERT(commandPtrSystemError);
    Global::tRefType fRef = GetNewCommandRef();
    SendCommand(fRef, Global::CommandShPtr_t(commandPtrSystemError));

    //m_TimeStamps.SystemErrorStartTime = QDateTime::currentMSecsSinceEpoch();
}

void SchedulerMainThreadController::OnBackToBusy()
{
    ProgramAcknownedgeType_t type =  DataManager::PROGRAM_SYSTEM_RC_RESTART;
    MsgClasses::CmdProgramAcknowledge* commandPtrRcRestart(new MsgClasses::CmdProgramAcknowledge(5000, type));
    Q_ASSERT(commandPtrRcRestart);
    Global::tRefType fRef = GetNewCommandRef();
    SendCommand(fRef, Global::CommandShPtr_t(commandPtrRcRestart));


    //If SW is in progress of pausing, we will update time at here. We will do that in CheckResuemFromPause
    if ((!m_IsResumeFromPause && !m_bWaitToPause && !m_bWaitToPauseCmdYes) /*&& (-1 != m_CurProgramStepIndex)*/)
    {
//        qint64 delta = (QDateTime::currentMSecsSinceEpoch() - m_TimeStamps.SystemErrorStartTime) / 1000;
//        m_EndTimeAndStepTime.UserSetEndTime += delta;
//        MsgClasses::CmdUpdateProgramEndTime* commandUpdateProgramEndTime(new MsgClasses::CmdUpdateProgramEndTime(5000, delta));
//        Q_ASSERT(commandUpdateProgramEndTime);
//        Global::tRefType tfRef = GetNewCommandRef();
//        SendCommand(tfRef, Global::CommandShPtr_t(commandUpdateProgramEndTime));
    }

    //Show pausing related message box again
    if (m_bWaitToPause || m_bWaitToPauseCmdYes)
    {
        SendProgramAcknowledge(SHOW_PAUSE_MSG_DLG);
    }
    if (m_IsResumeFromPause)
    {
        SendProgramAcknowledge(SHOW_RESUME_MSG_DLG);
    }

}

void SchedulerMainThreadController::OnPreTestDone(const QString& RetortID)
{
    ProgramAcknownedgeType_t type =  DataManager::PROGRAM_PRETEST_DONE;
    MsgClasses::CmdProgramAcknowledge* commandPreTest(new MsgClasses::CmdProgramAcknowledge(5000, type, RetortID));
    Q_ASSERT(commandPreTest);
    Global::tRefType fRef = GetNewCommandRef();
    SendCommand(fRef, Global::CommandShPtr_t(commandPreTest));

    if (mp_DataManager->CheckMaintenanceTimeCountStart()){

        Global::tRefType fRef = GetNewCommandRef();
        SendCommand(fRef, Global::CommandShPtr_t(new MsgClasses::CmdProgramAcknowledge(30000, DataManager::MAINTAINANCE_TIMECOUNT_START)));
    }
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

void SchedulerMainThreadController::GetStringIDList(quint32 ErrorID,
                                                    Global::tTranslatableStringList &EventStringParList,
                                                    Global::tTranslatableStringList &EventRDStringParList)
{
#if 0
    quint32 InternalSteps = 0;
    quint32 RVPosition = 0;
    switch(ErrorID)
    {
        case 513040012:
        case 513040161:
        case 513040163:
            EventStringParList<<QString("%1").arg(ErrorID);
            EventRDStringParList<<QString("%1").arg(m_CurProgramStepInfo.stationID);
            break;
        case 513040203:
        case 513040201:
            EventStringParList<<QString("%1").arg(ErrorID);
            EventRDStringParList<<QString("%1").arg(m_CurProgramStepInfo.stationID)<<QString("%1").arg(m_SchedulerCommandProcessor->HardwareMonitor().PressureAL);
            break;
        case 513040014:
        case 513040241:
        case 511040101:
        case 511040221:
            EventStringParList<<QString("%1").arg(ErrorID);
            EventRDStringParList<<QString("%1").arg(m_CurProgramStepInfo.stationID);
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
        case 513020081:
        case 513020082:
        case 513020083:
        {
            qreal userInputMeltingPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
            EventStringParList<<QString("%1").arg(userInputMeltingPoint+12);
            break;
        }
        case 513020181:
        case 513020182:
        case 513020183:
        {
            qreal userInputMeltingPoint = mp_DataManager->GetUserSettings()->GetTemperatureParaffinBath();
            EventStringParList<<QString("%1").arg(userInputMeltingPoint-4);
            break;
        }
        default:
            EventStringParList << QString("%1").arg(ErrorID);
            break;
    }
#endif
}

void SchedulerMainThreadController::SendSafeReagentFinishedCmd()
{
//    QString ProgramName = mp_DataManager->GetProgramList()->GetProgram(m_CurProgramID)->GetName();
//    RaiseEvent(EVENT_SCHEDULER_PROGRAM_FINISHED,QStringList()<<ProgramName);

//    //send command to main controller to tell program finished
//    Global::tRefType Ref = GetNewCommandRef();
//    if(m_IsCleaningProgram)
//    {
//        MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::CLEANING_PROGRAM_COMPLETE_AS_SAFE_REAGENT));
//        Q_ASSERT(commandPtrFinish);
//        Ref = GetNewCommandRef();
//        SendCommand(Ref, Global::CommandShPtr_t(commandPtrFinish));
//    }
//    else
//    {
//        if(m_ProgramStatusInfor.IsRetortContaminted())
//        {
//            MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_FINISHED_AS_SAFE_REAGENT));
//            Q_ASSERT(commandPtrFinish);
//            Ref = GetNewCommandRef();
//            SendCommand(Ref, Global::CommandShPtr_t(commandPtrFinish));
//        }
//        else
//        {
//            MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_FINISHED_AS_SAFE_REAGENT_NO_CONTAMINATED));
//            Q_ASSERT(commandPtrFinish);
//            Ref = GetNewCommandRef();
//            SendCommand(Ref, Global::CommandShPtr_t(commandPtrFinish));
//        }
//    }
//    m_ProgramStatusInfor.SetErrorFlag(0);
}

void SchedulerMainThreadController::SendOutErrMsg(ReturnCode_t EventId, bool IsErrorMsg)
{
#if 0
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
            // When SW Pause and Resumming, we dismiss the GUI message
            if (m_bWaitToPause || m_bWaitToPauseCmdYes)
            {
                SendProgramAcknowledge(DISMISS_PAUSING_MSG_DLG);
            }
            if (m_IsResumeFromPause)
            {
                SendProgramAcknowledge(DISMISS_RESUME_MSG_DLG);
            }
            m_SchedulerMachine->SendErrorSignal();
            m_IsErrorStateForHM = true;
        }
    }
#endif
}

/*lint -e641 */
bool SchedulerMainThreadController::RaiseError(const quint32 EventKey, ReturnCode_t EventID, const quint32 Scenario,
                          const bool ActionResult, const bool Active)
{
#if 0
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

        if ((520070300 == ErrorID) || (513030081 == ErrorID) || (512040401 == ErrorID)){
            this->GetHeatingStrategy()->StopTemperatureControl("LevelSensor");
            this->GetSchedCommandProcessor()->pushCmd(new CmdALReleasePressure(500, m_Sender), false);
        }
    }
    else
    {
        Global::EventObject::Instance().RaiseEvent(EventKey, 0, ActionResult,Active);
    }
#endif
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
