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
#include "Scheduler/Commands/Include/CmdSavedServiceInfor.h"
#include "DataManager/Containers/InstrumentHistory/Commands/Include/CmdModuleListUpdate.h"
#include "NetCommands/Include/CmdSystemAction.h"
#ifdef GOOGLE_MOCK
#include "Scheduler/Test/Mock/MockIDeviceProcessing.h"
#endif
#include "float.h"
#include "Global/Include/EventObject.h"
#include "Scheduler/Include/HeatingStrategy.h"
#include <unistd.h>
#include <Global/Include/SystemPaths.h>


using namespace DataManager;

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//                                                                                 Project Specific
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
        , m_PauseToBeProcessed(false)
        , m_ProcessCassetteCount(0)
        , m_OvenLidStatus(UNDEFINED_VALUE)
        , m_RetortLockStatus(UNDEFINED_VALUE)
        , mp_HeatingStrategy(NULL)
        , m_RefCleanup(Global::RefManager<Global::tRefType>::INVALID)
        , m_delayTime(0)
        , m_IsInSoakDelay(false)
{
    memset(&m_TimeStamps, 0, sizeof(m_TimeStamps));
    m_CurErrEventID = DCL_ERR_FCT_NOT_IMPLEMENTED;
    m_AllProgramCount = false;
    m_IsPrecheckMoveRV = false;

    m_lastPVTime = 0;
    m_completionNotifierSent = false;
    QString ProgramStatusFilePath = "../Settings/ProgramStatus.txt";
    m_IsCleaningMoveRV = false;
    m_IsCleaningRun = false;
    QFile ProgramStatusFile(ProgramStatusFilePath);

    if(!ProgramStatusFile.exists())
        CreateProgramStatusFile(&ProgramStatusFile);
    ReadProgramStatusFile(&ProgramStatusFile);
}

void SchedulerMainThreadController::CreateProgramStatusFile(QFile *p_StatusFile)
{
    if(p_StatusFile)
    {
        if(!p_StatusFile->open(QIODevice::ReadWrite | QIODevice::Text))
        {

    QString ProgramStatusFilePath = "../Settings/ProgramStatus.txt";
    QFile ProgramStatusFile(ProgramStatusFilePath);
    if(!ProgramStatusFile.exists())
    {
        CreateProgramStatusFile(&ProgramStatusFile);
        ReadProgramStatusFile(&ProgramStatusFile);
    }
    else
    {
        ReadProgramStatusFile(&ProgramStatusFile);
        UpdateProgramStatusFile("ProgramFinished", "No");
    }
            LogDebug("ProgramStatus file open failed");
        }
        QTextStream FileStream(p_StatusFile);
        FileStream.setFieldAlignment(QTextStream::AlignLeft);
        FileStream << "LastRVPosition:" << "1" << "\n" << left;
        FileStream << "ProgramFinished:" << "No" << "\n" << left;
        p_StatusFile->close();
    }
}

void SchedulerMainThreadController::ReadProgramStatusFile(QFile *p_StatusFile)
{
    if(p_StatusFile)
    {
        if(!p_StatusFile->open(QIODevice::ReadWrite | QIODevice::Text))
        {
            LogDebug("Read programStatus file open failed");
        }
        QString Line;
        QTextStream FileStream(p_StatusFile);
        do{
            Line = FileStream.readLine().simplified();
            QString tmpString("0");
            if(Line.contains("ProgramFinished", Qt::CaseInsensitive))
            {
                QStringList LineFields = Line.split(":", QString::SkipEmptyParts);
                if(LineFields.count() == 2)
                {
                    m_ProgramStatusFileMap.insert("ProgramFinished", LineFields[1]);
                }
            }
            else if(Line.contains("LastRVPosition", Qt::CaseInsensitive))
            {
                QStringList LineFields = Line.split(":", QString::SkipEmptyParts);
                if(LineFields.count() == 2)
                {
                    m_ProgramStatusFileMap.insert("LastRVPosition", LineFields[1]);
                }
            }

        }while(!Line.isNull());
    }
}

void SchedulerMainThreadController::UpdateProgramStatusFile(const QString& key, const QString& value)
{
    QMap<QString, QString>::iterator iter = m_ProgramStatusFileMap.find(key);
    if(iter != m_ProgramStatusFileMap.end())
    {
        iter.value() = value;
    }

    const QString ProgramStatusFilePath = Global::SystemPaths::Instance().GetSettingsPath() + "/ProgramStatus.txt";
    QFile ProgramStatusFile(ProgramStatusFilePath);
    if(!ProgramStatusFile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
    {
        LogDebug("open the ProgramStatusFilePath failed");
    }
    QTextStream FileStream(&ProgramStatusFile);
    FileStream.setFieldAlignment(QTextStream::AlignLeft);

    QMapIterator<QString, QString> StatusfileItr(m_ProgramStatusFileMap);
    while (StatusfileItr.hasNext()) {
        StatusfileItr.next();
        QString Key1 = StatusfileItr.key();
        QString Value1 = m_ProgramStatusFileMap.value(Key1);
        FileStream << Key1 << ":" << Value1 << "\n" << left;
    }
    ProgramStatusFile.flush();
    fsync(ProgramStatusFile.handle());
    ProgramStatusFile.close();
}

QString SchedulerMainThreadController::getTheProgramStatus(const QString& key)
{
    QMap<QString, QString>::iterator iter = m_ProgramStatusFileMap.find(key);
    if(iter != m_ProgramStatusFileMap.end())
    {
        return iter.value();
    }
    else
    {
        return "";
    }
}

SchedulerMainThreadController::~SchedulerMainThreadController()
{
    delete m_SchedulerMachine;
    m_SchedulerMachine = NULL;
}

void SchedulerMainThreadController::CreateProgramStatusFile(QFile *p_StatusFile)
{
    if(p_StatusFile)
    {
        if(!p_StatusFile->open(QIODevice::ReadWrite | QIODevice::Text))
        {
            LogDebug("ProgramStatus file open failed");
        }
        QTextStream FileStream(p_StatusFile);
        FileStream.setFieldAlignment(QTextStream::AlignLeft);
        FileStream << "LastRVPosition:" << "1" << "\n" << left;
        FileStream << "ProgramFinished:" << "No" << "\n" << left;
        p_StatusFile->close();
    }
}

void SchedulerMainThreadController::ReadProgramStatusFile(QFile *p_StatusFile)
{
    if(p_StatusFile)
    {
        if(!p_StatusFile->open(QIODevice::ReadWrite | QIODevice::Text))
        {
            LogDebug("Read programStatus file open failed");
        }
        QString Line;
        QTextStream FileStream(p_StatusFile);
        do{
            Line = FileStream.readLine().simplified();
            QString tmpString("0");
            if(Line.contains("ProgramFinished", Qt::CaseInsensitive))
            {
                QStringList LineFields = Line.split(":", QString::SkipEmptyParts);
                if(LineFields.count() == 2)
                {
                    m_ProgramStatusFileMap.insert("ProgramFinished", LineFields[1]);
                }
            }
            else if(Line.contains("LastRVPosition", Qt::CaseInsensitive))
            {
                QStringList LineFields = Line.split(":", QString::SkipEmptyParts);
                if(LineFields.count() == 2)
                {
                    m_ProgramStatusFileMap.insert("LastRVPosition", LineFields[1]);
                }
            }

        }while(!Line.isNull());
    }
}

void SchedulerMainThreadController::UpdateProgramStatusFile(const QString& key, const QString& value)
{
    QMap<QString, QString>::iterator iter = m_ProgramStatusFileMap.find(key);
    if(iter != m_ProgramStatusFileMap.end())
    {
        iter.value() = value;
    }

    const QString ProgramStatusFilePath = Global::SystemPaths::Instance().GetSettingsPath() + "/ProgramStatus.txt";
    QFile ProgramStatusFile(ProgramStatusFilePath);
    if(!ProgramStatusFile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
    {
        LogDebug("open the ProgramStatusFilePath failed");
    }
    QTextStream FileStream(&ProgramStatusFile);
    FileStream.setFieldAlignment(QTextStream::AlignLeft);

    QMapIterator<QString, QString> StatusfileItr(m_ProgramStatusFileMap);
    while (StatusfileItr.hasNext()) {
        StatusfileItr.next();
        QString Key1 = StatusfileItr.key();
        QString Value1 = m_ProgramStatusFileMap.value(Key1);
        FileStream << Key1 << ":" << Value1 << "\n" << left;
    }
    ProgramStatusFile.flush();
    fsync(ProgramStatusFile.handle());
    ProgramStatusFile.close();
}

QString SchedulerMainThreadController::getTheProgramStatus(const QString& key)
{
    QMap<QString, QString>::iterator iter = m_ProgramStatusFileMap.find(key);
    if(iter != m_ProgramStatusFileMap.end())
    {
        return iter.value();
    }
    else
    {
        return "";
    }
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
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessor, DCLConfigurationFinished(ReturnCode_t),
                      this,OnDCLConfigurationFinished(ReturnCode_t))

    CONNECTSIGNALSLOT(m_SchedulerCommandProcessor, DeviceProcessDestroyed(),
                      this, DevProcDestroyed())
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessor, ReportGetServiceInfo(ReturnCode_t, const DataManager::CModule&, const QString&),
                     this, ReportGetServiceInfo(ReturnCode_t, const DataManager::CModule&, const QString&));
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
    //send command
    SendCommand(GetNewCommandRef(), Global::CommandShPtr_t(new MsgClasses::CmdModuleListUpdate(3000, ModuleInfo, DeviceType, false)));
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

    if (CTRL_CMD_SHUTDOWN == newControllerCmd)
    {
        m_TickTimer.stop();
        m_SchedulerCommandProcessor->ShutDownDevice();
        //DequeueNonDeviceCommand();
        return;
    }

    SchedulerCommandShPtr_t cmd;
    PopDeviceControlCmdQueue(cmd);

    SchedulerStateMachine_t currentState = m_SchedulerMachine->GetCurrentState();
    m_SchedulerMachine->UpdateCurrentState(currentState);

    switch(currentState & 0xFF)
    {
    case SM_INIT:
        //refuse any main controller request if there is any
        //qDebug()<<"DBG"<<"Scheduler main controller state: INIT";
        break;
    case SM_IDLE:
        //qDebug()<<"DBG"<<"Scheduler main controller state: IDLE";
        HardwareMonitor( "IDLE" );
        HandleIdleState(newControllerCmd,cmd);
        break;
    case SM_BUSY:
        //qDebug()<<"DBG"<<"Scheduler main controller state: RUN";
        HardwareMonitor( m_CurProgramID );
        HandleRunState(newControllerCmd, cmd);
        break;
    case SM_ERROR:
        HardwareMonitor( "ERROR" );
        HandleErrorState(newControllerCmd, cmd, currentState);
        break;
    default:
        LogDebug(QString("Scheduler main controller gets unexpected state: %1").arg(currentState));
    }
}

void SchedulerMainThreadController::HandleIdleState(ControlCommandType_t ctrlCmd, SchedulerCommandShPtr_t cmd)
{
    bool isCleaningProgram = false;
    switch (ctrlCmd)
    {
    case CTRL_CMD_START:
        m_UsedStationIDs.clear();

        //Check if it is a Cleaning Program or not?
        if (m_NewProgramID.at(0) == 'C')
        {
            isCleaningProgram = true;
            int sep = m_NewProgramID.indexOf('_');
            m_CurProgramID = m_NewProgramID.left(sep);
            m_ReagentIdOfLastStep = m_NewProgramID.right(m_NewProgramID.count()- sep -1);
        }
        else
        {
            m_CurProgramID = m_NewProgramID;
        }

        //m_NewProgramID = "";
        //m_CurProgramStepIndex = -1;
        this->GetNextProgramStepInformation(m_CurProgramID, m_CurProgramStepInfo);
        if(m_CurProgramStepIndex != -1)
        {
            //send command to main controller to tell program is starting
            MsgClasses::CmdProgramAcknowledge* commandPtrFinish(new MsgClasses::CmdProgramAcknowledge(5000,DataManager::PROGRAM_RUN_BEGIN));
            Q_ASSERT(commandPtrFinish);
            Global::tRefType fRef = GetNewCommandRef();
            SendCommand(fRef, Global::CommandShPtr_t(commandPtrFinish));

            //LOG_STR_ARG(STR_START_PROGRAM, Global::FmtArgs()<<ProgramName);
            QString ProgramName = mp_DataManager->GetProgramList()->GetProgram(m_CurProgramID)->GetName();
            LogDebug(QString("Start Program: %1").arg(ProgramName));
            LogDebug(QString("Start Step: %1").arg(m_CurProgramStepIndex));
            //m_SchedulerMachine->SendRunSignal();
            //send command to main controller to tell the left time
            quint32 leftSeconds = GetCurrentProgramStepNeededTime(m_CurProgramID);
            //LOG_STR_ARG(STR_CURRENT_PROGRAM_NAME_STEP_REAGENT_LEFTTIME,Global::FmtArgs()<< ProgramName << m_CurProgramStepIndex +1 << m_CurReagnetName << leftSeconds);
            QTime leftTime(0,0,0);
            leftTime = leftTime.addSecs(leftSeconds);
            MsgClasses::CmdCurrentProgramStepInfor* commandPtr(new MsgClasses::CmdCurrentProgramStepInfor(5000, m_CurReagnetName, m_CurProgramStepIndex, leftSeconds));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
            //DequeueNonDeviceCommand();

            //wether cleaning program
            if ( 'C' == ProgramName.at(0) )
            {
                QString LastPosition = getTheProgramStatus("LastRVPosition");
                CmdRVReqMoveToInitialPosition* cmdSet = new CmdRVReqMoveToInitialPosition(500, this);
                cmdSet->SetRVPosition( (RVPosition_t)LastPosition.toUInt() );
                m_SchedulerCommandProcessor->pushCmd(cmdSet);
                LogDebug(QString("cleaning program set the rv position to:%1").arg(LastPosition));

                m_AllProgramCount = true;
            }
            else
            {
            }
            else
            {
                m_SchedulerMachine->SendRunSignal();
            }
        }
        break;
    default:
        break;
          //DequeueNonDeviceCommand();
    }

    // For Cleaning program, we will wait until "RVReqMoveToInitialPositon" response is got.
    if ("" != m_CurProgramID && 'C' == m_CurProgramID.at(0) && NULL != cmd)
    {
        if ("Scheduler::RVReqMoveToInitialPosition" == cmd->GetName())
        {
            ReturnCode_t retCode = DCL_ERR_FCT_CALL_SUCCESS;
            cmd->GetResult(retCode);
            if (DCL_ERR_FCT_CALL_SUCCESS == retCode)
            {
                m_SchedulerMachine->SendRunSignal();
            }
            else
            {
                this->SendOutErrMsg(retCode);
            }
        }
        else
        {
            // just ignore this.
        }

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
    QString ReagentGroup = m_CurProgramStepInfo.reagentGroup;

    m_CurrentScenario = GetScenarioBySchedulerState(m_SchedulerMachine->GetCurrentState(),ReagentGroup);
    if(cmd != NULL)
    {
        if(!(cmd->GetResult(retCode)))
        {
            retCode = DCL_ERR_UNDEFINED;
        }
        cmdName = cmd->GetName();
    }
#if 0
    //just for testing
    if (200 ==Scenario)
    {
        RaiseError(0, 500010301, 200, false);
        m_SchedulerMachine->SendErrorSignal();
    }
#endif

    if(CTRL_CMD_ABORT == ctrlCmd)
    {
        LogDebug(QString("Scheduler received command: ABORT"));
        m_SchedulerMachine->NotifyAbort();
        //DequeueNonDeviceCommand();
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
            if(!m_IsPrecheckMoveRV)
            {
                if(RV_UNDEF != targetPos)
                {
                    m_IsPrecheckMoveRV = true;
                    CmdRVReqMoveToRVPosition* cmd = new CmdRVReqMoveToRVPosition(500, this);
                    cmd->SetRVPosition(targetPos);
                    m_SchedulerCommandProcessor->pushCmd(cmd);
                    LogDebug(QString("Move to RV tube position(first): %1").arg(targetPos));
                }
                else
                {
                   //todo: error handling
                   LogDebug(QString("Get invalid RV position: %1").arg(m_CurProgramStepInfo.stationID));
                }
            }
            else if(m_PositionRV == targetPos)
            {
                else
                {
                    m_IsPrecheckMoveRV = false;
                    LogDebug(QString("Program Step Hit tube(precheck) %1").arg(targetPos));
                    UpdateProgramStatusFile("LastRVPosition", QString("%1").arg(targetPos));
                    LogDebug("Precheck DONE, if cleaning program it just move RV");
                    m_SchedulerMachine->NotifyStDone(); //todo: update later
                }
            }
            else
            {
                if("Scheduler::RVReqMoveToRVPosition" == cmdName)
                {
                    if(DCL_ERR_DEV_RV_MOTOR_INTERNALSTEPS_RETRY == retCode)
                    {
                        //fail to move to tube, raise event here
                        LogDebug(QString("Program Step Move to tube(first)%1 internal steps retry").arg(targetPos));
                        RaiseError(0, DCL_ERR_DEV_RV_MOTOR_INTERNALSTEPS_RETRY, Scenario, true);
                        m_SchedulerMachine->SendErrorSignal();
                    }
                    else if(DCL_ERR_DEV_RV_MOTOR_INTERNALSTEPS_EXCEEDUPPERLIMIT == retCode)
                    {
                        LogDebug(QString("Program Step Move to tube(first)%1 exceed upper limit").arg(targetPos));
                        RaiseError(0, DCL_ERR_DEV_RV_MOTOR_INTERNALSTEPS_EXCEEDUPPERLIMIT, Scenario, true);
                        m_SchedulerMachine->SendErrorSignal();
                    }
                }
                else if(CTRL_CMD_PAUSE == ctrlCmd)
                {
                    m_SchedulerMachine->NotifyPause(PSSM_INIT);
                    DequeueNonDeviceCommand();
                }
            }
        {
            if(CTRL_CMD_PAUSE == ctrlCmd)
            {
                AllStop();
                m_SchedulerMachine->NotifyPause(PSSM_INIT);
                //DequeueNonDeviceCommand();
            }
            else
            {
                QString ProgramName = mp_DataManager->GetProgramList()->GetProgram(m_CurProgramID)->GetName();
                if ('C' != ProgramName.at(0))
                {
                    m_SchedulerMachine->SendRunPreTest();
                }
                else
                {
                    if(!m_IsCleaningMoveRV)
                    {
                        RVPosition_t targetPos = GetRVTubePositionByStationID(m_CurProgramStepInfo.stationID);
                        if(RV_UNDEF != targetPos)
                        {
                            m_IsCleaningMoveRV = true;
                            this->MoveRV(0);
                        }
                    }
                    else if(IsRVRightPosition(0))
                    {
                        m_IsCleaningMoveRV = false;
                        m_IsCleaningRun = true;
                        m_SchedulerMachine->SendRunCleaning();
                    }
                    else
                    {
                        m_IsCleaningMoveRV = false;
                        if(("Scheduler::RVReqMoveToRVPosition" == cmdName))
                        {
                            if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
                            {
                                RaiseError(0, retCode, m_CurrentScenario, true);
                                m_SchedulerMachine->SendErrorSignal();
                            }
                        }
                    }
                }//end cleaning program
            }
        }
        else if (PSSM_PRETEST == stepState)
        {
           m_SchedulerMachine->HandlePssmPreTestWorkFlow(cmdName, retCode);
        }
        else if (PSSM_FILLING_RVROD_HEATING == stepState)
        {
            if(m_CurProgramStepInfo.reagentGroup == "RG6")
            {
                if(mp_HeatingStrategy->CheckRVHeatingStatus())
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
            if(mp_HeatingStrategy->CheckLevelSensorHeatingStatus())
            else if(m_CurProgramStepInfo.reagentGroup != "RG6")
            {
                if(mp_HeatingStrategy->CheckLevelSensorHeatingStatus())
            }
            else if(m_CurProgramStepInfo.reagentGroup == "RG6")
                if(mp_HeatingStrategy->CheckRVHeatingStatus())
                {
                    LogDebug("Program Step Heating Rotary Valve heating rod OK");
                    m_SchedulerMachine->NotifyLevelSensorTempS1Ready();
                }

            }//end RG6
            else if(mp_HeatingStrategy->CheckLevelSensorHeatingStatus())
            RVPosition_t targetPos = GetRVTubePositionByStationID(m_CurProgramStepInfo.nextStationID);
            {
                LogDebug("Program Step Heating Level sensor stage OK");
                m_SchedulerMachine->NotifyLevelSensorHeatingReady();
                    UpdateProgramStatusFile("LastRVPosition", QString("%1").arg(targetPos));
            }
            else
            {
                // Do nothing, just wait for status of level sensor
            }
        }
        else if(PSSM_FILLING == stepState)
        {
            m_SchedulerMachine->HandleProtocolFillingWorkFlow(cmdName, retCode);
        }
        else if(PSSM_RV_MOVE_TO_SEAL == stepState)
        {
            RVPosition_t targetPos = GetRVSealPositionByStationID(m_CurProgramStepInfo.stationID);
            if(IsRVRightPosition(1))
            {
                if((CTRL_CMD_PAUSE == ctrlCmd)||(m_PauseToBeProcessed))
                {
                    m_SchedulerMachine->NotifyPause(PSSM_RV_MOVE_TO_SEAL);
                    m_PauseToBeProcessed = false;
                    //DequeueNonDeviceCommand();
                }
                else
                {
                    m_SchedulerMachine->NotifyRVMoveToSealReady();
                }
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
                if (CTRL_CMD_PAUSE == ctrlCmd)
                {
                    m_PauseToBeProcessed = true;
                }
            }
        }
        else if(PSSM_PROCESSING == stepState)
        {
            if(CTRL_CMD_PAUSE == ctrlCmd)
            {
                if(m_CurProgramStepInfo.isPressure || m_CurProgramStepInfo.isVacuum)
                {
                    AllStop();
                    m_lastPVTime = 0;
                }
                m_SchedulerMachine->NotifyPause(PSSM_PROCESSING);
                //DequeueNonDeviceCommand();
            }
            else
            {
                qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();
                //todo: 1/10 the time
                qint32 period = m_CurProgramStepInfo.durationInSeconds * 1000;
                if((now - m_TimeStamps.CurStepSoakStartTime ) > (period))
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
                         if(CTRL_CMD_DRAIN == ctrlCmd)
                         {
                             LogDebug(QString("Program Processing(Soak) Process finished"));
                             m_SchedulerMachine->NotifyProcessingFinished();
                             m_TimeStamps.CurStepSoakStartTime = 0;
                             m_completionNotifierSent = false;
                         }
                    }
                    else
                    {
                        LogDebug(QString("Program Processing(Soak) Process finished"));
                        m_SchedulerMachine->NotifyProcessingFinished();
                        m_TimeStamps.CurStepSoakStartTime = 0;
                    }
                }
                else
                {
                    if (now > m_TimeStamps.ProposeSoakStartTime)
                    {
                        if(m_IsInSoakDelay)
                        {
                            if(m_CurProgramStepInfo.isPressure ^ m_CurProgramStepInfo.isVacuum)
                            {
                                if(m_CurProgramStepInfo.isPressure)
                                {
                                    Pressure();
                                }else if(m_CurProgramStepInfo.isVacuum)
                                {
                                    Vaccum();
                                }
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
                                    Pressure();
                                }
                                else
                                {
                                    Vaccum();
                                }
                            m_lastPVTime = now;
                            }
                        }
                    }
                }
            }
        else if(PSSM_RV_MOVE_TO_TUBE == stepState)
        {
            if(IsRVRightPosition(0))
            {
                if((CTRL_CMD_PAUSE == ctrlCmd)||(m_PauseToBeProcessed))
                {
                    m_SchedulerMachine->NotifyPause(PSSM_RV_MOVE_TO_TUBE);
                    m_PauseToBeProcessed = false;
                }
                else
                {
                    m_SchedulerMachine->NotifyRVMoveToTubeReady();
                }
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
                if (CTRL_CMD_PAUSE == ctrlCmd)
                {
                    m_PauseToBeProcessed = true;
                }
            }
        }
        else if(PSSM_DRAINING == stepState)
        {
            if(m_PauseToBeProcessed)
            {
                RVPosition_t sealPos = GetRVSealPositionByStationID(m_CurProgramStepInfo.stationID);
                if(m_PositionRV == sealPos)
                {
                    //release pressure
                    AllStop();
                    m_PauseToBeProcessed = false;
                    m_SchedulerMachine->NotifyPause(PSSM_DRAINING);
                    //DequeueNonDeviceCommand();
                 }
             }

             if (CTRL_CMD_PAUSE == ctrlCmd)
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

            if( "Scheduler::ALDraining"== cmdName)
            {
                if(DCL_ERR_FCT_CALL_SUCCESS == retCode)
                {
                    LogDebug(QString("Program Step Draining succeed!"));
                    m_SchedulerMachine->NotifyDrainFinished();
                }
                else if(DCL_ERR_DEV_LA_DRAINING_TIMEOUT_BULIDPRESSURE == retCode)
                {
                    LogDebug(QString("Program Step Draining Build Pressure timeout"));
                    RaiseError(0, retCode, m_CurrentScenario, true);
                    m_SchedulerMachine->SendErrorSignal();
                }
            }
        }
        else if(PSSM_RV_POS_CHANGE == stepState)
        {
            if(IsRVRightPosition(2))
            {
                if((CTRL_CMD_PAUSE == ctrlCmd)||(m_PauseToBeProcessed))
                {
                    m_SchedulerMachine->NotifyPause(PSSM_RV_POS_CHANGE);
                    m_PauseToBeProcessed = false;
                }
                else
                {
                    m_SchedulerMachine->NotifyStepFinished();
                }
            }
            else
            {
                if("Scheduler::RVReqMoveToRVPosition" == cmdName)
                {
                    if(DCL_ERR_DEV_RV_MOTOR_INTERNALSTEPS_RETRY == retCode)
                    {
                        //fail to move to seal, raise event here
                        RaiseError(0, DCL_ERR_DEV_RV_MOTOR_INTERNALSTEPS_RETRY, m_CurrentScenario, true);
                        m_SchedulerMachine->SendErrorSignal();
                    }
                    else if(DCL_ERR_DEV_RV_MOTOR_INTERNALSTEPS_EXCEEDUPPERLIMIT == retCode)
                    {
                        RaiseError(0, DCL_ERR_DEV_RV_MOTOR_INTERNALSTEPS_EXCEEDUPPERLIMIT, m_CurrentScenario, true);
                        m_SchedulerMachine->SendErrorSignal();
                    }
                }

                if(CTRL_CMD_PAUSE == ctrlCmd)
                {
                    m_PauseToBeProcessed = true;
                }
            }
        }
        else if(PSSM_STEP_PROGRAM_FINISH == stepState)
        {
            m_UsedStationIDs.append(m_CurProgramStepInfo.stationID);
            LogDebug(QString("Program Step Finished"));
            this->GetNextProgramStepInformation(m_CurProgramID, m_CurProgramStepInfo);
            QString ProgramName = mp_DataManager->GetProgramList()->GetProgram(m_CurProgramID)->GetName();
            if(m_CurProgramStepIndex != -1)
            {
                //start next step
                LogDebug(QString("Start Step %1").arg(m_CurProgramStepIndex));
                m_SchedulerMachine->NotifyStepProgramFinished();
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
            LogDebug(QString("Program %1 finished!").arg(ProgramName));
            m_SchedulerMachine->SendRunComplete();
            //m_SchedulerMachine->Stop();
            //todo: tell main controller that program is complete
            UpdateStationReagentStatus();
            if(m_IsCleaningRun)
            {
                m_IsCleaningRun = false;
                UpdateProgramStatusFile("ProgramFinished", "Yes");
            }

            if(m_AllProgramCount)
            {
                m_AllProgramCount = false;
                UpdateProgramStatusFile("ProgramFinished", "Yes");
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
            if(CTRL_CMD_START == ctrlCmd)
            {
                // resume the program
                emit NotifyResume();
                //DequeueNonDeviceCommand();
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
                //DequeueNonDeviceCommand();
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
                this->OnStopDrain();
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
            //DequeueNonDeviceCommand();
        }
    }
#endif
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
            LogDebug("Go to RC_Restart");
            m_SchedulerMachine->EnterRcRestart();
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
        else if(CTRL_CMD_RC_LEVELSENSOR_HEATING_OVERTIME == ctrlCmd)
        {
            LogDebug("Go to RC_Levelsensor_Heating_Overtime");
            m_SchedulerMachine->EnterRcLevelsensorHeatingOvertime();
        }
        else
        {
            LogDebug(QString("Unknown Command: %1").arg(ctrlCmd, 0, 16));
        }
    }
    else if (SM_ERR_RS_STANDBY == currentState)
    {
        LogDebug(QString("In RS_STandBy state"));
        m_SchedulerMachine->HandleRsStandByWorkFlow(cmdName, retCode);
    }
    else if (SM_ERR_RS_HEATINGERR30SRETRY == currentState)
    {
        LogDebug(QString("In RS_HeatingErr_30SRetry state"));
        m_SchedulerMachine->HandleRsHeatingErr30SRetry();
    }
    else if (SM_ERR_RS_STANDBY_WITH_TISSUE == currentState)
    {
        LogDebug(QString("In RS_STandBy_WithTissue state"));
        m_SchedulerMachine->HandleRsStandByWithTissueWorkFlow(cmdName, retCode);
    }
    else if (SM_ERR_RC_LEVELSENSOR_HEATING_OVERTIME == currentState)
    {
        LogDebug(QString("In RC_Levelsensor_Heating_Overtime State"));
        m_SchedulerMachine->HandleRcLevelSensorHeatingOvertimeWorkFlow();

    }
    else if(SM_ERR_RS_RV_GETORIGINALPOSITIONAGAIN == currentState)
    {
        LogDebug(QString("In RS_RV_GET_ORIGINAL_POSITION_AGAIN state"));
        m_SchedulerMachine->HandleRsRVGetOriginalPositionAgainWorkFlow(cmdName, retCode);
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
    }

    NetCommands::CmdSystemAction* pCmdSystemAction = dynamic_cast<NetCommands::CmdSystemAction*>(pt.GetPointerToUserData());
    if(pCmdSystemAction)
    {
        QString cmd = pCmdSystemAction->GetActionString();
        m_EventKey = pCmdSystemAction->GetEventKey();

        LogDebug(QString("Get action: %1").arg(cmd));
        cmd = cmd.toLower();

        if(cmd == "rs_rv_getoriginalpositionagain")
        {
            return CTRL_CMD_RS_RV_GET_ORIGINAL_POSITION_AGAIN;
        }
        if(cmd == "rc_restart")
        {
            return CTRL_CMD_RC_RESTART;
        }
        if(cmd == "rs_standby")
        {
            return CTRL_CMD_RS_STANDBY;
        }
        if(cmd == "rs_standby_withtissue")
        {
            return CTRL_CMD_RS_STANDBY_WITHTISSUE;
        }
        if (cmd == "rs_heatingerr30sretry")
        {
            return CTRL_CMD_RS_HEATINGERR30SRETRY;
        }
        if (cmd == "rc_levelsensor_heating_overtime")
        {
            return CTRL_CMD_RC_LEVELSENSOR_HEATING_OVERTIME;
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
        leftTime += 5 +  2 * 2; //suppose RV need 5 seconds to move to the target station, and movment between tube and its seal need 2 seconds.
        leftTime += 60; //suppose need 60 seconds to fill
        leftTime += 40; //suppose need 40 seconds to drain
        leftTime += 20; //suppose need 20 seconds to heat level sensor
        QString reagent;
        reagent = pProgramStep->GetReagentID();
        if (reagent == "L8")
        {
            leftTime += 10 * 60;
        }
        if (0 == i)
        {
            leftTime += m_delayTime;
        }
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
        warningthreshold = 10 * 30 * 24;
    }
    else
    {
        warningthreshold = 5 * 30 * 24;
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
    m_RefCleanup = Ref;
}

void SchedulerMainThreadController::OnSavedServiceInfor(Global::tRefType Ref, const MsgClasses::CmdSavedServiceInfor & Cmd)
{
    this->SendAcknowledgeOK(Ref);
    m_SchedulerCommandProcessor->NotifySavedServiceInfor(Cmd.DeviceType());
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
    case PSSM_PRETEST:
        scenario = 200;
        break;
    case PSSM_PAUSE:
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
    case PSSM_PAUSE_DRAIN:
        scenario = 206;
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
    QString ReagentGroup = m_CurProgramStepInfo.reagentGroup;
    quint32 Scenario = GetScenarioBySchedulerState(m_SchedulerMachine->GetCurrentState(),ReagentGroup);
       // if(StepID == "IDLE")
	HardwareMonitor_t strctHWMonitor = m_SchedulerCommandProcessor->HardwareMonitor();
    LogDebug(strctHWMonitor.toLogString());

    // Run Heating Strategy
    if ("ERROR" != StepID)
    {
        DeviceControl::ReturnCode_t retCode = mp_HeatingStrategy->RunHeatingStrategy(strctHWMonitor, Scenario);
        if (DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {      
            LogDebug(QString("Heating Strategy got an error at event %1 and scenario %2").arg(retCode).arg(Scenario));
            RaiseError(0, retCode, Scenario, true);
            m_SchedulerMachine->SendErrorSignal();
        }
    }
    if(strctHWMonitor.PressureAL == UNDEFINED_VALUE)
	{
        m_PressureAL = strctHWMonitor.PressureAL;
	}
#if 0
    if(m_PressureAL < -45 || m_PressureAL > 45){
        quint32 EVENT_LA_PressureSensor_OutofRange = 34; //only for test
        RaiseError(0,EVENT_LA_PressureSensor_OutofRange,Scenario,true);
    }
#endif
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
    if(strctHWMonitor.TempRTBottom1 != UNDEFINED_VALUE)
	{
        m_TempRTBottom = strctHWMonitor.TempRTBottom1;
	}
	if(strctHWMonitor.TempRTSide!= UNDEFINED_VALUE)
	{
        m_TempRTSide = strctHWMonitor.TempRTSide;
	}
    if(strctHWMonitor.TempOvenBottom1 != UNDEFINED_VALUE)
	{
        m_TempOvenBottom = strctHWMonitor.TempOvenBottom1;
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
            SchedulerStateMachine_t currentState = m_SchedulerMachine->GetCurrentState();
            if(((currentState & 0xF) == SM_BUSY)&&(currentState != PSSM_PAUSE)&&(currentState != PSSM_PAUSE_DRAIN))
            {
                RaiseError(0, DCL_ERR_DEV_LIDLOCK_CLOSE_STATUS_ERROR, Scenario, true);
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

void SchedulerMainThreadController::PopDeviceControlCmdQueue(Scheduler::SchedulerCommandShPtr_t& PtrCmd, const QString& CmdName)
{
    QQueue<Scheduler::SchedulerCommandShPtr_t>::iterator iter = m_DeviceControlCmdQueue.begin();
    m_MutexDeviceControlCmdQueue.lock();
    while (true)
    {
        if (m_DeviceControlCmdQueue.isEmpty())
        {
            m_WaitCondition.wait(&m_MutexDeviceControlCmdQueue);
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
            m_WaitCondition.wait(&m_MutexDeviceControlCmdQueue);
        }
        else
        {
            break;
        }
    }

    // Get the command
    PtrCmd = *iter;
    m_DeviceControlCmdQueue.removeOne(*iter);
    m_MutexDeviceControlCmdQueue.unlock();
}


void SchedulerMainThreadController::PushDeviceControlCmdQueue(Scheduler::SchedulerCommandShPtr_t CmdPtr)
{
    m_MutexDeviceControlCmdQueue.lock();
    m_DeviceControlCmdQueue.push_back(CmdPtr);
    m_WaitCondition.wakeAll();
    m_MutexDeviceControlCmdQueue.unlock();
}

void SchedulerMainThreadController::StepStart()
{

}

bool SchedulerMainThreadController::BottleCheck()
{
    if (m_ProgramStationList.empty())
    {
        return false;
    }

    ProgramStationInfo_t stationInfo = m_ProgramStationList.dequeue();
    RVPosition_t tubePos = GetRVTubePositionByStationID(stationInfo.StationID);
    QString reagentGrpId = stationInfo.ReagentGroupID;

    LogDebug(QString("Bottle check for tube %1").arg(tubePos));
    CmdIDBottleCheck* cmd  = new CmdIDBottleCheck(500, this);
    cmd->SetReagentGrpID(reagentGrpId);
    cmd->SetTubePos(tubePos);
    m_SchedulerCommandProcessor->pushCmd(cmd);
    return true;
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

void SchedulerMainThreadController::OnEnterPssmProcessing()
{
    if(m_TimeStamps.CurStepSoakStartTime == 0)
    {
        m_TimeStamps.CurStepSoakStartTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
        LogDebug(QString("Start to soak, start time stamp is: %1").arg(m_TimeStamps.CurStepSoakStartTime));
    }

    m_lastPVTime = 0;
    m_completionNotifierSent = false;

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

bool SchedulerMainThreadController::IsRVRightPosition(qint16 type)
{
    RVPosition_t targetPos = RV_UNDEF;
    bool ret = false;

    if(0 == type )
    {
        targetPos = GetRVTubePositionByStationID(m_CurProgramStepInfo.stationID);
    }
    else if(1 == type)
    {
        targetPos = GetRVSealPositionByStationID(m_CurProgramStepInfo.stationID);
    }
    else if(2 == type)
    {
        targetPos = GetRVTubePositionByStationID(m_CurProgramStepInfo.nextStationID);
    }

    if (m_PositionRV == targetPos)
    {
        ret = true;
        if(1 == type)
        {
            LogDebug(QString("RV hit seal position: %1").arg(targetPos));
        }
        else
        {
            LogDebug(QString("RV hit tube position: %1").arg(targetPos));
        }
        UpdateProgramStatusFile("LastRVPosition", QString("%1").arg(targetPos));
    }
    return ret;
}

void SchedulerMainThreadController::MoveRV(qint16 type)
{
    CmdRVReqMoveToRVPosition* cmd = new CmdRVReqMoveToRVPosition(500, this);
    RVPosition_t targetPos = RV_UNDEF;

    if(0 == type ) //tube position
    {
        //get target position here
        targetPos = GetRVTubePositionByStationID(m_CurProgramStepInfo.stationID);
        LogDebug(QString("Move to RV tube position: %1").arg(targetPos));
    }
    else if(1 == type) //seal positon
    {
        //get target position here
        targetPos = GetRVSealPositionByStationID(m_CurProgramStepInfo.stationID);
        LogDebug(QString("Move to RV Seal position: %1").arg(targetPos));
    }
    else if(2 == type)
    {
        //get target position here
        targetPos = GetRVTubePositionByStationID(m_CurProgramStepInfo.nextStationID);
        LogDebug(QString("Move to RV next tube position: %1").arg(targetPos));
    }

    if(RV_UNDEF != targetPos)
    {
        cmd->SetRVPosition(targetPos);
        m_SchedulerCommandProcessor->pushCmd(cmd);
    }
    else
    {
       LogDebug(QString("Get invalid RV position: %1").arg(m_CurProgramStepInfo.stationID));
       RaiseError(0, DCL_ERR_INVALID_PARAM, m_CurrentScenario, true);
       m_SchedulerMachine->SendErrorSignal();
    }
}

void SchedulerMainThreadController::Fill()
{
    LogDebug("Send cmd to DCL to Fill");
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
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(5000,m_CurProgramStepInfo.stationID , true, true));
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
    default:
        break;
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
    default:
        break;
    }

    return false;
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
        if (reportError1.instanceID != 0 && (now-reportError1.errorTime.toMSecsSinceEpoch()) >= interval*1000)
        {
            return false;
        }
        break;
    case LATUBE1:
         reportError1 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "LA", AL_TUBE1);
         if (reportError1.instanceID != 0 && (now-reportError1.errorTime.toMSecsSinceEpoch()) >= interval*1000)
         {
             return false;
         }
         break;
    case LATUBE2:
        reportError1 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "LA", AL_TUBE2);
        if (reportError1.instanceID != 0 && (now-reportError1.errorTime.toMSecsSinceEpoch()) >= interval*1000)
        {
            return false;
        }
        break;
    case RV:
        reportError1 =  m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "RV");
        if (reportError1.instanceID != 0 && (now-reportError1.errorTime.toMSecsSinceEpoch()) >= interval*1000)
        {
            return false;
        }
        break;
        // Retort and Oven are in the same card, so any heater goes wrong, we will stop all the others.
    case RETORT:
    case OVEN:
        reportError1 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Retort", RT_BOTTOM);
        reportError2 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Retort", RT_SIDE);
        reportError3 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Oven", OVEN_TOP);
        reportError4 = m_SchedulerCommandProcessor->GetSlaveModuleReportError(TEMP_CURRENT_OUT_OF_RANGE, "Oven", OVEN_BOTTOM);
        if (reportError1.instanceID != 0 && (now-reportError1.errorTime.toMSecsSinceEpoch()) >= interval*1000)
        {
            return false;
        }
        if (reportError2.instanceID != 0 && (now-reportError2.errorTime.toMSecsSinceEpoch()) >= interval*1000)
        {
            return false;
        }
        if (reportError3.instanceID != 0 && (now-reportError3.errorTime.toMSecsSinceEpoch()) >= interval*1000)
        {
            return false;
        }
        if (reportError4.instanceID != 0 && (now-reportError4.errorTime.toMSecsSinceEpoch()) >= interval*1000)
        {
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
    MsgClasses::CmdStationSuckDrain* commandPtr(new MsgClasses::CmdStationSuckDrain(5000,m_CurProgramStepInfo.stationID , false, true));
    Q_ASSERT(commandPtr);
    Global::tRefType Ref = GetNewCommandRef();
    SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
}

        m_TimeStamps.ProposeSoakStartTime = QDateTime::currentDateTime().addSecs(m_delayTime).toMSecsSinceEpoch();
		m_SchedulerCommandProcessor->pushCmd(new CmdALReleasePressure(500,  this));

    if ((0 == m_CurProgramStepIndex) && (m_delayTime > 0))
    {
        m_IsInSoakDelay = true;
        return;
    }

        m_IsInSoakDelay = false;
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

void SchedulerMainThreadController::OnStopDrain()
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
            if(m_SchedulerMachine->GetPreviousState() != PSSM_DRAINING)
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

qint64 SchedulerMainThreadController::GetOvenHeatingTime()
{
    qint64 OvenStartTime = mp_HeatingStrategy->GetOvenHeatingBeginTime();
    qint64 RetTime = 0;
    if (0 != OvenStartTime)
    {
        RetTime = (QDateTime::currentDateTime().toMSecsSinceEpoch() - OvenStartTime)/1000;
    }
    return RetTime;
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
        return LATUBE1;
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

    return UNKNOWN;
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

float SchedulerMainThreadController::ReadPressureDrift()
{
    QString FileName = Global::SystemPaths::Instance().GetSettingsPath() + "/PressureNullDrift.txt";
    float pressureDrift = 999999;
    FILE* pFile;

    if ((pFile = fopen(FileName.toStdString().c_str(), "r")) == NULL)
    {
        return pressureDrift;
    }

    char Buf[200];
    memset(Buf, 0, sizeof(Buf));
    if(fread(Buf, 1, 200, pFile) > 0 )
    {
        QString Content = QString::fromAscii(Buf, -1);
        QStringList StrList = Content.split(";");
        if(StrList.size() >= 1)
        {
            pressureDrift = StrList.at(0).toDouble();
        }
    }
    fclose(pFile);
    return pressureDrift;
}

}
