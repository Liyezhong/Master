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
#include "Scheduler/Commands/Include/CmdRVReqMoveToInitialPosition.h"
#include "Scheduler/Commands/Include/CmdALStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdALFilling.h"
#include "Scheduler/Commands/Include/CmdALDraining.h"
#include "Scheduler/Commands/Include/CmdALPressure.h"
#include "Scheduler/Commands/Include/CmdALVaccum.h"
#include "Scheduler/Commands/Include/CmdRVStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdRTStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdOvenStartTemperatureControlWithPID.h"
#include "Scheduler/Commands/Include/CmdPerTurnOnMainRelay.h"
#include "Scheduler/Commands/Include/CmdPerTurnOffMainRelay.h"
#include "Scheduler/Commands/Include/CmdRVReqMoveToRVPosition.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"
#include "HimalayaDataManager/Include/DataManager.h"
#include "Global/Include/GlobalDefines.h"
#include <Scheduler/Commands/Include/CmdRTLock.h>
#include <Scheduler/Commands/Include/CmdRTUnlock.h>
#include <HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdCurrentProgramStepInfor.h>
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramStartReady.h"
//#include "Scheduler/Include/HimalayaHardwareSystemStateId.h"
//#include "Scheduler/Commands/Include/CmdSystemState.h"



using namespace DataManager;

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//                                                                                 Project Specific
namespace Scheduler {

SchedulerMainThreadController::SchedulerMainThreadController(
                Global::gSourceType TheHeartBeatSource
                )
        : Threads::ThreadController(TheHeartBeatSource, "SchedulerMainThread")
        , m_TickTimer(this)
        , m_SchedulerCommandProcessorThread(NULL)
        , m_SchedulerCommandProcessor(NULL)
        , m_SchedulerMachine(new SchedulerMachine())
        , mp_IDeviceProcessing(NULL)
        , mp_DataManager(NULL)
        , m_CurProgramStepID("")
        , m_CurProgramID("")
        , m_NewProgramID("")
        , mp_ProgramStepStateMachine(new ProgramStepStateMachine())
        , m_CurStepSoakStartTime(0)
        , m_CurReagnetName("")
{

}

SchedulerMainThreadController::~SchedulerMainThreadController()
{
    delete m_SchedulerCommandProcessorThread;
    m_SchedulerCommandProcessorThread = NULL;
    delete m_SchedulerCommandProcessor;
    m_SchedulerCommandProcessor = NULL;
    delete m_SchedulerMachine;
    m_SchedulerMachine = NULL;
    delete mp_ProgramStepStateMachine;
    mp_ProgramStepStateMachine = NULL;
}

void SchedulerMainThreadController::RegisterCommands()
{
    // register commands
    RegisterCommandForProcessing<HimalayaErrorHandler::CmdRaiseAlarm,
                    SchedulerMainThreadController>(&SchedulerMainThreadController::OnRaiseAlarmLocalRemote, this);

    RegisterCommandForProcessing<MsgClasses::CmdProgramAction,
                    SchedulerMainThreadController>(&SchedulerMainThreadController::OnProgramAction, this);

    RegisterCommandForProcessing<MsgClasses::CmdRetortLock,
                    SchedulerMainThreadController>(&SchedulerMainThreadController::OnOnRetortLock, this);

    RegisterCommandForProcessing<NetCommands::CmdSystemAction,
                    SchedulerMainThreadController>(&SchedulerMainThreadController::OnActionCommandReceived, this);

}

void SchedulerMainThreadController::CreateAndInitializeObjects()
{

    //create the SchedulerCommandProcessor thread
    m_SchedulerCommandProcessorThread = new QThread();
    m_SchedulerCommandProcessor = new SchedulerCommandProcessor(this);
    //CONNECTSIGNALSLOT(&m_SchedulerCommandProcessor, timeout(), m_SchedulerCommandProcessor, run());

    m_SchedulerCommandProcessor->moveToThread(m_SchedulerCommandProcessorThread);
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessorThread, started(), m_SchedulerCommandProcessor, run());
    m_SchedulerCommandProcessorThread->start();

    //timer setting
    CONNECTSIGNALSLOT(&m_TickTimer, timeout(), this, OnTickTimer());
    CONNECTSIGNALSLOT(m_SchedulerCommandProcessor, DCLConfigurationFinished(ReturnCode_t, IDeviceProcessing*),
                      this,OnDCLConfigurationFinished(ReturnCode_t, IDeviceProcessing*))

    m_TickTimer.setInterval(500);
    CONNECTSIGNALSLOT(this, signalProgramStart(const QString&), this, ProgramStart(const QString&));
    CONNECTSIGNALSLOT(this, signalProgramPause(), this, ProgramPause());
    CONNECTSIGNALSLOT(this, signalProgramAbort(), this, ProgramAbort());
    CONNECTSIGNALSLOT(mp_ProgramStepStateMachine, OnInit(), this, StepStart());
    CONNECTSIGNALSLOT(mp_ProgramStepStateMachine, OnHeatLevelSensorTempS1(), this, HeatLevelSensor());
    CONNECTSIGNALSLOT(mp_ProgramStepStateMachine, OnHeatLevelSensorTempS2(), this, HeatLevelSensor());
    CONNECTSIGNALSLOT(mp_ProgramStepStateMachine, OnMoveToTubeBefore(), this, MoveRV());
    CONNECTSIGNALSLOT(mp_ProgramStepStateMachine, OnMoveToTubeAfter(), this, MoveRV());
    CONNECTSIGNALSLOT(mp_ProgramStepStateMachine, OnMoveToSeal(), this, MoveRV());
    CONNECTSIGNALSLOT(mp_ProgramStepStateMachine, OnFill(), this, Fill());
    CONNECTSIGNALSLOT(mp_ProgramStepStateMachine, OnSoak(), this, Soak());
    CONNECTSIGNALSLOT(mp_ProgramStepStateMachine, OnDrain(), this, Drain());


    //command queue reset
    m_SchedulerCmdQueue.clear();
    m_DeviceControlCmdQueue.clear();

    // now register commands
    RegisterCommands();

    //InitializeDevice();

    m_SchedulerMachine->Start();
    //for debug
    qDebug() << "Current state of Scheduler is: " << m_SchedulerMachine->GetCurrentState();
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

void SchedulerMainThreadController::OnPowerFail()
{
    DEBUGWHEREAMI;

    //! @todo: Implement Power Fail Handling
}

void SchedulerMainThreadController::OnTickTimer()
{
    //ProcessNonDeviceCommand();

    ControlCommandType_t newControllerCmd = ReceiveNonDeviceCommand();
    SchedulerCommandShPtr_t cmd;
    ReturnCode_t retCode = DCL_ERR_UNDEFINED;
    if(PopDeviceControlCmdQueue(cmd))
    {
        if(!(cmd->GetResult(retCode)))
        {
            retCode = DCL_ERR_UNDEFINED;
        }
    }
    SchedulerState_t currentState = m_SchedulerMachine->GetCurrentState();
    switch(currentState)
    {
    case INIT_STATE:
        //refuse any main controller request if there is any
        break;
    case IDLE_STATE:
        HardwareMonitor(mp_IDeviceProcessing, "IDLE");
        HandleIdleState(newControllerCmd);
        break;
    case RUN_STATE:
        HardwareMonitor(mp_IDeviceProcessing, m_CurProgramID);
        HandleRunState(newControllerCmd, retCode);
        break;
    case ERROR_STATE:
        //refuse any main controller request if there is any
        qDebug()<<"Scheduler main controller is in error state";
        break;
    default:
        qDebug()<<"Scheduler main controller gets unexpected state: " << currentState;
    }
}

void SchedulerMainThreadController::HandleIdleState(ControlCommandType_t ctrlCmd)
{
    switch (ctrlCmd)
    {
    case CTRL_CMD_START:
        m_CurProgramID = m_NewProgramID;
        m_NewProgramID = "";
        m_CurProgramStepID = "";
        this->GetNextProgramStepInformation(m_CurProgramID, m_CurProgramStepInfo);
        if(m_CurProgramStepID != "")
        {
            qDebug() << "Start step: " << m_CurProgramID;
            mp_ProgramStepStateMachine->Start();
            m_SchedulerMachine->SendRunSignal();

            //send command to main controller to tell the left time
            quint32 leftSeconds = GetLeftProgramNeededTime(m_CurProgramID);
            QTime leftTime(0,0,0);
            leftTime = leftTime.addSecs(leftSeconds);
            MsgClasses::CmdCurrentProgramStepInfor* commandPtr(new MsgClasses::CmdCurrentProgramStepInfor(5000, m_CurReagnetName, leftTime));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));

        }
        break;
    case CTRL_CMD_LOCK_RETORT:
        break;
    case CTRL_CMD_UNLOCK_RETORT:
        break;
    default:
        ;
    }
}

void SchedulerMainThreadController::HandleRunState(ControlCommandType_t ctrlCmd, ReturnCode_t retCode)
{
    ProgramStepStateMachine_t stepState = mp_ProgramStepStateMachine->GetCurrentState();
    if(PSSM_INIT == stepState)
    {
        if(CTRL_CMD_PAUSE == ctrlCmd)
        {
           mp_ProgramStepStateMachine->NotifyPause(PSSM_INIT);
        }
        else if(CheckStepTemperature())
        {
            mp_ProgramStepStateMachine->NotifyTempsReady();
        }
    }
    else if(PSSM_READY_TO_HEAT_LEVEL_SENSOR_S1 == stepState)
    {
        //todo: get data here
        if(CheckLevelSensorTemperature(85))
        {
            mp_ProgramStepStateMachine->NotifyLevelSensorTempS1Ready();
        }
    }
    else if(PSSM_READY_TO_HEAT_LEVEL_SENSOR_S2 == stepState)
    {
        //todo: get data here
        if(CheckLevelSensorTemperature(85))
        {
            mp_ProgramStepStateMachine->NotifyLevelSensorTempS2Ready();
        }
    }
    else if(PSSM_READY_TO_TUBE_BEFORE == stepState)
    {
         // get current step tube position here
         RVPosition_t targetPos = GetRVTubePositionByStationID(m_CurProgramStepInfo.stationID);
         if(m_PositionRV == targetPos)
         {
             mp_ProgramStepStateMachine->NotifyHitTubeBefore();
         }
    }
    else if(PSSM_READY_TO_FILL == stepState)
    {
         if(retCode == DCL_ERR_DEV_AL_FILL_SUCCESS)
         {
             mp_ProgramStepStateMachine->NotifyFillFinished();
         }
         else if(retCode != DCL_ERR_UNDEFINED)
         {
            //todo: error handling here
         }
    }
    else if(PSSM_READY_TO_SEAL == stepState)
    {
         //todo: get current step seal position here
         RVPosition_t targetPos = GetRVSealPositionByStationID(m_CurProgramStepInfo.stationID);
         if(m_PositionRV == targetPos)
         {
             mp_ProgramStepStateMachine->NotifyHitSeal();
         }
    }
    else if(PSSM_SOAK == stepState)
    {
        qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();
        static qint64 lastPVTime = 0;
        //todo: 1/10 the time
        qint32 period = m_CurProgramStepInfo.durationInSeconds * 1000;
        //qint32 period = m_CurProgramStepInfo.durationInSeconds * 20;
        if((now - m_CurStepSoakStartTime) > (period))
        {
            mp_ProgramStepStateMachine->NotifySoakFinished();
        }
        else
        {
            if(m_CurProgramStepInfo.isPressure && m_CurProgramStepInfo.isVacuum)
            {
                // P/V take turns in 1 minute
                if((now - lastPVTime)>60000)
                {
                    if(((now - m_CurStepSoakStartTime)/60000)%2 == 0)
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
    else if(PSSM_READY_TO_TUBE_AFTER == stepState)
    {
         //get current step tube position here
         RVPosition_t targetPos = GetRVTubePositionByStationID(m_CurProgramStepInfo.stationID);
         if(m_PositionRV == targetPos)
         {
             mp_ProgramStepStateMachine->NotifyHitTubeAfter();
         }
    }
    else if(PSSM_READY_TO_DRAIN == stepState)
    {
         if(retCode == DCL_ERR_DEV_AL_DRAIN_SUCCESS)
         {
             mp_ProgramStepStateMachine->NotifyDrainFinished();
         }
         else if(retCode != DCL_ERR_UNDEFINED)
         {
            //todo: error handling here
         }
    }
    else if(PSSM_FINISH == stepState)
    {
        //todo: start next program step or finish all program
        qDebug()<< "step finished!";
        this->GetNextProgramStepInformation(m_CurProgramID, m_CurProgramStepInfo);
        if(m_CurProgramStepID != "")
        {
            //start next step
            qDebug() << "Start step: " << m_CurProgramID;
            mp_ProgramStepStateMachine->Start();
            //send command to main controller to tell the left time
            quint32 leftSeconds = GetLeftProgramNeededTime(m_CurProgramID);
            QTime leftTime(0,0,0);
            leftTime = leftTime.addSecs(leftSeconds);
            MsgClasses::CmdCurrentProgramStepInfor* commandPtr(new MsgClasses::CmdCurrentProgramStepInfor(5000, m_CurReagnetName, leftTime));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        }
        else
        {
            //program finished
            qDebug() << "Program finished!";
            m_SchedulerMachine->SendRunComplete();
            //todo: tell main controller that program is complete
            //send command to main controller to tell the left time
            QTime leftTime(0,0,0);
            MsgClasses::CmdCurrentProgramStepInfor* commandPtr(new MsgClasses::CmdCurrentProgramStepInfor(5000, "", leftTime));
            Q_ASSERT(commandPtr);
            Global::tRefType Ref = GetNewCommandRef();
            SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
        }
    }
    else if(PSSM_PAUSE == stepState)
    {
        if(CTRL_CMD_START == ctrlCmd)
        {
            // resume the program
            mp_ProgramStepStateMachine->NotifyResume();
        }
    }
    else if(PSSM_PAUSE_DRAIN == stepState)
    {
    }


}

ControlCommandType_t SchedulerMainThreadController::ReceiveNonDeviceCommand()
{

    if(m_SchedulerCmdQueue.isEmpty())
        return CTRL_CMD_NONE;

    Global::CommandShPtr_t pt = m_SchedulerCmdQueue.dequeue();//should use m_SchedulerCmdQueue.head()?
    MsgClasses::CmdProgramAction* pCmdProgramAction = dynamic_cast<MsgClasses::CmdProgramAction*>(pt.GetPointerToUserData());
    if(pCmdProgramAction)
    {
        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_START)
        {
            //emit signalProgramStart(pCmdProgramAction->GetProgramID());
            m_NewProgramID = pCmdProgramAction->GetProgramID();
#if 0 //for debug
            quint32 timeneeded = GetLeftProgramNeededTime(m_NewProgramID);
#endif
            return CTRL_CMD_START;
        }
        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_PAUSE)
        {
            //emit signalProgramPause();
            return CTRL_CMD_PAUSE;
        }
        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_ABORT)
        {
            //emit signalProgramAbort();
            return CTRL_CMD_ABORT;
        }
    }

    MsgClasses::CmdRetortLock* pCmdRetortLock = dynamic_cast<MsgClasses::CmdRetortLock*>(pt.GetPointerToUserData());
    if (pCmdRetortLock)
    {
        if(pCmdRetortLock->IsLock())
        {
            return CTRL_CMD_LOCK_RETORT;
        }
        else
        {
            return CTRL_CMD_UNLOCK_RETORT;
        }
    }

    HimalayaErrorHandler::CmdRaiseAlarm* pCmdRaiseAlarm = dynamic_cast<HimalayaErrorHandler::CmdRaiseAlarm*>(pt.GetPointerToUserData());
    if (pCmdRaiseAlarm)
    {
        if(pCmdRaiseAlarm->m_localAlarm)
        {

        }
        else
        {

        }
        //todo temp. return set remote alarm, update later
        return CTRL_CMD_SET_REMOTE_ALARM;
    }
    return CTRL_CMD_UNKNOWN;

}//When end of this function, the command in m_SchedulerCmdQueue will be destrory by share pointer (CommandShPtr_t) mechanism

void SchedulerMainThreadController::ProcessNonDeviceCommand()
{
    if(m_SchedulerCmdQueue.isEmpty())
        return;

    SchedulerState_t currentState = m_SchedulerMachine->GetCurrentState();

    Global::CommandShPtr_t& pt = m_SchedulerCmdQueue.head();
    MsgClasses::CmdProgramAction* pCmdProgramAction = dynamic_cast<MsgClasses::CmdProgramAction*>(pt.GetPointerToUserData());
    if(pCmdProgramAction)
    {
        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_START)
        {
            if (currentState == IDLE_STATE)
            {
                emit signalProgramStart(pCmdProgramAction->GetProgramID());
                m_SchedulerCmdQueue.dequeue();
            }
            return;
        }

        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_PAUSE)
        {
            if (currentState == RUN_STATE)
            {
                emit signalProgramPause();
                m_SchedulerCmdQueue.dequeue();
            }
            return;
        }

        if (pCmdProgramAction->ProgramActionType() == DataManager::PROGRAM_ABORT)
        {
            if (currentState == RUN_STATE)
            {
                emit signalProgramAbort();
                m_SchedulerCmdQueue.dequeue();
            }
            return;
        }

        Q_ASSERT(0);
        return;
    }

    MsgClasses::CmdRetortLock* pCmdRetortLock = dynamic_cast<MsgClasses::CmdRetortLock*>(pt.GetPointerToUserData());
    if (pCmdRetortLock)
    {
        if(pCmdRetortLock->IsLock())//in Error status?
        {
            if (currentState == IDLE_STATE || currentState == PAUSE_STATE)
            {
                m_SchedulerCommandProcessor->pushCmd(new CmdRTLock(500, mp_IDeviceProcessing, this));
                m_SchedulerCmdQueue.dequeue();
            }
            return;
        }
        else if (currentState != RUN_STATE)
        {
            m_SchedulerCommandProcessor->pushCmd(new CmdRTUnlock(500, mp_IDeviceProcessing, this));
            m_SchedulerCmdQueue.dequeue();
            return;
        }
        return;
    }

    HimalayaErrorHandler::CmdRaiseAlarm* pCmdRaiseAlarm = dynamic_cast<HimalayaErrorHandler::CmdRaiseAlarm*>(pt.GetPointerToUserData());
    if (pCmdRaiseAlarm)
    {
        if(pCmdRaiseAlarm->m_localAlarm)
        {

        }
        else
        {

        }
        m_SchedulerCmdQueue.dequeue();
        return;
    }

    //CmdSystemAction--1
    //CmdSystemAction--2
    //CmdSystemAction--3


}//When end of this function, the command in m_SchedulerCmdQueue will be destrory by share pointer (CommandShPtr_t) mechanism

void SchedulerMainThreadController::ProgramStart(const QString& ProgramID)
{
    m_CurProgramID = ProgramID;
    processNextProgramStep();
}

void SchedulerMainThreadController::processNextProgramStep()
{
    qDebug()<<"processNextProgramStep";
    ProgramStepInfor programStepInfor;
    this->GetNextProgramStepInformation(m_CurProgramID, programStepInfor);
    if ("" != programStepInfor.stationID)
    {
        //do something

        //calculate the cost time for this step
        int costTime = 5000;
        QTimer::singleShot(costTime, this, SLOT(processNextProgramStep()));
    }
}

bool SchedulerMainThreadController::GetNextProgramStepInformation(const QString& ProgramID,
                                                                  ProgramStepInfor& programStepInfor)
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
    ListOfProgramSteps_t* programSteps = pProgram->GetStepList();

    int stepSize =programSteps->size();
    bool isLastStep = false;

    QString nextProgramStep("");
    if (m_CurProgramStepID.isEmpty())
    {
        nextProgramStep = "0";
    }
    else
    {
        int nextOne = m_CurProgramStepID.toInt() ;
        ++nextOne;
        nextProgramStep = QString::number(nextOne);
        if (stepSize == nextOne + 1)
            isLastStep = true;
    }


    ListOfProgramSteps_t::iterator iter = programSteps->find(nextProgramStep);
    if (iter != programSteps->end())
    {
        CProgramStep* pProgramStep = iter.value();
        QString reagentID = pProgramStep->GetReagentID();
        programStepInfor.stationID  = this->GetStationIDFromReagentID(reagentID, isLastStep);
        programStepInfor.durationInSeconds = pProgramStep->GetDurationInSeconds();
        programStepInfor.temperature = pProgramStep->GetTemperature().toInt();
        programStepInfor.isPressure = (pProgramStep->GetPressure() == "On");
        programStepInfor.isVacuum = (pProgramStep->GetVacuum() == "On");
        m_CurProgramStepID = nextProgramStep;
        m_CurReagnetName = GetReagentName(reagentID);
    }
    else
    {
        m_CurProgramStepID = "";
    }
    return true;
}

quint32 SchedulerMainThreadController::GetLeftProgramNeededTime(const QString& ProgramID)
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
    ListOfProgramSteps_t* programSteps = pProgram->GetStepList();

    QString programStepID("");
    if (m_CurProgramStepID.isEmpty())
    {
        programStepID = "0";
    }
    else
    {
        programStepID = m_CurProgramStepID ;
    }


    ListOfProgramSteps_t::iterator iter = programSteps->find(programStepID);
    while (iter != programSteps->end())
    {
        CProgramStep* pProgramStep = iter.value();
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
        iter++;
    }
    return leftTime;
}

quint32 SchedulerMainThreadController::GetCurrentProgramNeededTime(const QString& ProgramID)
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
    ListOfProgramSteps_t* programSteps = pProgram->GetStepList();

    QString programStepID("");
    if (m_CurProgramStepID.isEmpty())
    {
        programStepID = "0";
    }
    else
    {
        programStepID = m_CurProgramStepID ;
    }


    ListOfProgramSteps_t::iterator iter = programSteps->find(programStepID);
    if (iter != programSteps->end())
    {
        CProgramStep* pProgramStep = iter.value();
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

void SchedulerMainThreadController::ProgramPause()
{

}

void SchedulerMainThreadController::ProgramAbort()
{

}

void SchedulerMainThreadController::OnRaiseAlarmLocalRemote(Global::tRefType Ref,
                                                            const HimalayaErrorHandler::CmdRaiseAlarm &Cmd)
{
    Q_UNUSED(Ref);
    m_Mutex.lock();
    m_SchedulerCmdQueue.enqueue(Global::CommandShPtr_t(new HimalayaErrorHandler::CmdRaiseAlarm(Cmd.m_localAlarm)));
    m_Mutex.unlock();
    this->SendAcknowledgeOK(Ref);
}

//client-->master
void SchedulerMainThreadController::OnProgramAction(Global::tRefType Ref,
                                                    const MsgClasses::CmdProgramAction &Cmd)
{
    m_Mutex.lock();
    m_SchedulerCmdQueue.enqueue(Global::CommandShPtr_t(new MsgClasses::CmdProgramAction(Cmd.GetTimeout(), Cmd.GetProgramID(), Cmd.ProgramActionType())));
    m_Mutex.unlock();
    this->SendAcknowledgeOK(Ref);
}

//client-->master
void SchedulerMainThreadController::OnOnRetortLock(Global::tRefType Ref, const MsgClasses::CmdRetortLock &Cmd)
{
    m_Mutex.lock();
    m_SchedulerCmdQueue.enqueue(Global::CommandShPtr_t(new MsgClasses::CmdRetortLock(Cmd.GetTimeout(), Cmd.IsLock())));
    m_Mutex.unlock();
    this->SendAcknowledgeOK(Ref);
}

//response or recovery
void SchedulerMainThreadController::OnActionCommandReceived(Global::tRefType Ref, const NetCommands::CmdSystemAction & Cmd)    // todo: should be of type CmdAction
{
    m_Mutex.lock();
    NetCommands::CmdSystemAction *p_CmdSystemAction = new NetCommands::CmdSystemAction();
    p_CmdSystemAction->SetAction(Cmd.GetAction());
    m_SchedulerCmdQueue.enqueue(Global::CommandShPtr_t(p_CmdSystemAction));
    m_Mutex.unlock();
}

QString SchedulerMainThreadController::GetStationIDFromReagentID(const QString& ReagentID, bool IsLastStep)
{
    if (!mp_DataManager)
        return "";

    CDashboardDataStationList* pDashboardDataStationList = mp_DataManager->GetStationList();
    if (!pDashboardDataStationList)
        return "";
    Global::RMSOptions_t rmsMode = Global::RMS_CASSETTES;//should get the actual information from DataManager later
    if (CDataReagentList* pReagentList =  mp_DataManager->GetReagentList())
    {
        const CReagent* pReagent = pReagentList->GetReagent(ReagentID);
        return pDashboardDataStationList->FindStationByReagent(pReagent, IsLastStep, rmsMode);
    }

    return "";
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

void SchedulerMainThreadController::OnDCLConfigurationFinished(ReturnCode_t RetCode, IDeviceProcessing* pIDP)
{
    if(RetCode == DCL_ERR_FCT_CALL_SUCCESS)
    {
        mp_IDeviceProcessing = pIDP;
        m_SchedulerCommandProcessor->pushCmd(new CmdRVReqMoveToInitialPosition(500, mp_IDeviceProcessing, this));
        SchedulerCommandShPtr_t resRVInitPos;
        while(!PopDeviceControlCmdQueue(resRVInitPos));
        ReturnCode_t retCode;
        resRVInitPos->GetResult(retCode);
        if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            //todo: error handling
            qDebug()<<"Failed move to initial position, return code: " << retCode;
            goto ERROR;
        }
#if 0
    //hardware not ready yet
        m_SchedulerCommandProcessor->pushCmd(new CmdPerTurnOnMainRelay(500, mp_IDeviceProcessing, this));
        SchedulerCommandShPtr_t resPerTurnOnRelay;
        while(!PopDeviceControlCmdQueue(resPerTurnOnRelay));
        resPerTurnOnRelay->GetResult(retCode);
        if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            //todo: error handling
            qDebug()<<"Failed turn on main relay, return code: " << retCode;
            goto ERROR;
        }
        CmdRTStartTemperatureControlWithPID* cmdHeatRTSide = new CmdRTStartTemperatureControlWithPID(500, mp_IDeviceProcessing, this);
        cmdHeatRTSide->SetType(RT_SIDE);
        //todo: get temperature here
        cmdHeatRTSide->SetNominalTemperature(90);
        cmdHeatRTSide->SetSlopeTempChange(10);
        cmdHeatRTSide->SetMaxTemperature(120);
        cmdHeatRTSide->SetControllerGain(1212);
        cmdHeatRTSide->SetResetTime(1000);
        cmdHeatRTSide->SetDerivativeTime(80);
        m_SchedulerCommandProcessor->pushCmd(cmdHeatRTSide);
        SchedulerCommandShPtr_t resHeatRtSide;
        while(!PopDeviceControlCmdQueue(resHeatRtSide));
        resHeatRtSide->GetResult(retCode);
        if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            //todo: error handling
            qDebug()<<"Failed to heat Retort side, return code: " << retCode;
            goto ERROR;
        }

        CmdRTStartTemperatureControlWithPID* cmdHeatRTBot = new CmdRTStartTemperatureControlWithPID(500, mp_IDeviceProcessing, this);
        cmdHeatRTBot->SetType(RT_BOTTOM);
        //todo: get temperature here
        cmdHeatRTBot->SetNominalTemperature(90);
        cmdHeatRTBot->SetSlopeTempChange(10);
        cmdHeatRTBot->SetMaxTemperature(120);
        cmdHeatRTBot->SetControllerGain(1212);
        cmdHeatRTBot->SetResetTime(1000);
        cmdHeatRTBot->SetDerivativeTime(80);
        m_SchedulerCommandProcessor->pushCmd(cmdHeatRTBot);
        SchedulerCommandShPtr_t resHeatRtBot;
        while(!PopDeviceControlCmdQueue(resHeatRtBot));
        resHeatRtBot->GetResult(retCode);
        if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            //todo: error handling
            qDebug()<<"Failed to heat Retort bottom, return code: " << retCode;
            goto ERROR;
        }


        CmdRVStartTemperatureControlWithPID* cmdHeatRV = new CmdRVStartTemperatureControlWithPID(500, mp_IDeviceProcessing, this);
        //todo: get temperature here
        cmdHeatRV->SetNominalTemperature(90);
        cmdHeatRV->SetSlopeTempChange(10);
        cmdHeatRV->SetMaxTemperature(120);
        cmdHeatRV->SetControllerGain(1212);
        cmdHeatRV->SetResetTime(1000);
        cmdHeatRV->SetDerivativeTime(80);
        m_SchedulerCommandProcessor->pushCmd(cmdHeatRV);
        SchedulerCommandShPtr_t resHeatRV;
        while(!PopDeviceControlCmdQueue(resHeatRV));
        resHeatRV->GetResult(retCode);
        if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            //todo: error handling
            qDebug()<<"Failed to heat Rotary valve, return code: " << retCode;
            goto ERROR;
        }

        CmdOvenStartTemperatureControlWithPID* cmdHeatOvenBot = new CmdOvenStartTemperatureControlWithPID(500, mp_IDeviceProcessing, this);
        cmdHeatOvenBot->SetType(OVEN_BOTTOM);
        //todo: get temperature here
        cmdHeatOvenBot->SetNominalTemperature(90);
        cmdHeatOvenBot->SetSlopeTempChange(10);
        cmdHeatOvenBot->SetMaxTemperature(120);
        cmdHeatOvenBot->SetControllerGain(1212);
        cmdHeatOvenBot->SetResetTime(1000);
        cmdHeatOvenBot->SetDerivativeTime(80);
        m_SchedulerCommandProcessor->pushCmd(cmdHeatOvenBot);
        SchedulerCommandShPtr_t resHeatOvenBot;
        while(!PopDeviceControlCmdQueue(resHeatOvenBot));
        resHeatOvenBot->GetResult(retCode);
        if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            //todo: error handling
            qDebug()<<"Failed to heat oven bottom, return code: " << retCode;
            goto ERROR;
        }

        CmdOvenStartTemperatureControlWithPID* cmdHeatOvenTop = new CmdOvenStartTemperatureControlWithPID(500, mp_IDeviceProcessing, this);
        cmdHeatOvenTop->SetType(OVEN_TOP);
        //todo: get temperature here
        cmdHeatOvenTop->SetNominalTemperature(90);
        cmdHeatOvenTop->SetSlopeTempChange(10);
        cmdHeatOvenTop->SetMaxTemperature(120);
        cmdHeatOvenTop->SetControllerGain(1212);
        cmdHeatOvenTop->SetResetTime(1000);
        cmdHeatOvenTop->SetDerivativeTime(80);
        m_SchedulerCommandProcessor->pushCmd(cmdHeatOvenTop);
        SchedulerCommandShPtr_t resHeatOvenTop;
        while(!PopDeviceControlCmdQueue(resHeatOvenTop));
        resHeatOvenTop->GetResult(retCode);
        if(DCL_ERR_FCT_CALL_SUCCESS != retCode)
        {
            //todo: error handling
            qDebug()<<"Failed to heat oven top, return code: " << retCode;
            goto ERROR;
        }
#endif
        // set state machine "init" to "idle" (David)
        m_SchedulerMachine->SendSchedulerInitComplete();
        //for debug
        qDebug() << "Current state of Scheduler is: " << m_SchedulerMachine->GetCurrentState();

        //send command to main controller to tell init complete
        MsgClasses::CmdProgramStartReady* commandPtr(new MsgClasses::CmdProgramStartReady(5000, true));
        Q_ASSERT(commandPtr);
        Global::tRefType Ref = GetNewCommandRef();
        SendCommand(Ref, Global::CommandShPtr_t(commandPtr));
    }
    else
    {
ERROR:
        if(RetCode == DCL_ERR_TIMEOUT)
        {
            qDebug()<<"Some devices are not found during DCL's initialization period.";
        }
        //error happend
        // set state machine "init" to "error" (David)
        m_SchedulerMachine->SendErrorSignal();
        //for debug
        qDebug() << "Error while init, Current state of Scheduler is: " << m_SchedulerMachine->GetCurrentState();
    }
    m_TickTimer.start();
}

void SchedulerMainThreadController::HardwareMonitor(IDeviceProcessing* pIDP, const QString& StepID)
{
    if(pIDP)
    {
       // if(StepID == "IDLE")
        {
            qreal PressureAL= pIDP->ALGetRecentPressure(0);
            qreal TempALLevelSensor= pIDP->ALGetRecentTemperature(AL_LEVELSENSOR, 0);
            qreal TempALTube1= pIDP->ALGetRecentTemperature(AL_TUBE1,0);
            qreal TempALTube2= pIDP->ALGetRecentTemperature(AL_TUBE2,0);
            qreal TempRV = pIDP->RVGetRecentTemperature(0);
            RVPosition_t PositionRV = pIDP->RVReqActRVPosition();
            qreal TempRTBottom= pIDP->RTGetRecentTemperature(RT_BOTTOM,0);
            qreal TempRTSide= pIDP->RTGetRecentTemperature(RT_SIDE,0);
            qreal TempOvenBottom= pIDP->OvenGetRecentTemperature(OVEN_BOTTOM,0);
            qreal TempOvenTop= pIDP->OvenGetRecentTemperature(OVEN_TOP,0);
            if(PressureAL != UNDEFINED_VALUE)
            {
                m_PressureAL = PressureAL;
            }
            if(TempALLevelSensor != UNDEFINED_VALUE)
            {
                m_TempALLevelSensor = TempALLevelSensor;
            }
            if(TempALTube1 != UNDEFINED_VALUE)
            {
                m_TempALTube1 = TempALTube1;
            }
            if(TempALTube2 != UNDEFINED_VALUE)
            {
                m_TempALTube2 = TempALTube2;
            }
            if(TempRV != UNDEFINED_VALUE)
            {
                m_TempRV = TempRV;
            }
            if(TempRTBottom != UNDEFINED_VALUE)
            {
                m_TempRTBottom = TempRTBottom;
            }
            if(TempRTSide!= UNDEFINED_VALUE)
            {
                m_TempRTSide = TempRTSide;
            }
            if(TempOvenBottom != UNDEFINED_VALUE)
            {
                m_TempOvenBottom = TempOvenBottom;
            }
            if(TempOvenTop != UNDEFINED_VALUE)
            {
                m_TempOvenTop = TempOvenTop;
            }
            m_PositionRV = PositionRV;
            qDebug()<<"Rotary valve's position is" << PositionRV;
            qDebug()<<"Air liquid system pressure is" << PressureAL;
            qDebug()<<"Air liquid system level sensor's temp is" << TempALLevelSensor;
#if 0
            qDebug()<<"Air liquid system tube1's temp is" << TempALTube1;
            qDebug()<<"Air liquid system tube2's temp is" << TempALTube2;
            qDebug()<<"Rotary valve's temp is" << TempRV;
            qDebug()<<"Retort bottom temp is" << TempRTBottom;
            qDebug()<<"Retort side temp is" << TempRTSide;
            qDebug()<<"Oven bottom temp is" << TempOvenBottom;
            qDebug()<<"Oven top temp is" << TempOvenTop;
#endif
        }
    }
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
    ProgramStepStateMachine_t stepState = mp_ProgramStepStateMachine->GetCurrentState();
    if(PSSM_READY_TO_HEAT_LEVEL_SENSOR_S1 == stepState)
    {
        CmdALStartTemperatureControlWithPID* cmd  = new CmdALStartTemperatureControlWithPID(500, mp_IDeviceProcessing, this);
        cmd->SetType(AL_LEVELSENSOR);
        //todo: get temperature here
        cmd->SetNominalTemperature(90);
        cmd->SetSlopeTempChange(10);
        cmd->SetMaxTemperature(120);
        cmd->SetControllerGain(1212);
        cmd->SetResetTime(1000);
        cmd->SetDerivativeTime(80);
        m_SchedulerCommandProcessor->pushCmd(cmd);
    }
    else if(PSSM_READY_TO_HEAT_LEVEL_SENSOR_S2 == stepState)
    {
        CmdALStartTemperatureControlWithPID* cmd  = new CmdALStartTemperatureControlWithPID(500, mp_IDeviceProcessing, this);
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

void SchedulerMainThreadController::MoveRV()
{
    ProgramStepStateMachine_t stepState = mp_ProgramStepStateMachine->GetCurrentState();
    CmdRVReqMoveToRVPosition* cmd = new CmdRVReqMoveToRVPosition(500, mp_IDeviceProcessing, this);

    if(PSSM_READY_TO_TUBE_BEFORE == stepState)
    {
        //get target position here
        RVPosition_t targetPos = GetRVTubePositionByStationID(m_CurProgramStepInfo.stationID);
        if(RV_UNDEF != targetPos)
        {
            cmd->SetRVPosition(targetPos);
            m_SchedulerCommandProcessor->pushCmd(cmd);
        }
        else
        {
           //todo: error handling
           qDebug()<<"Get invalid RV position: " << m_CurProgramStepInfo.stationID;
        }
    }
    else if(PSSM_READY_TO_TUBE_AFTER == stepState)
    {
        //get target position here
        RVPosition_t targetPos = GetRVTubePositionByStationID(m_CurProgramStepInfo.stationID);
        if(RV_UNDEF != targetPos)
        {
            cmd->SetRVPosition(targetPos);
            m_SchedulerCommandProcessor->pushCmd(cmd);
        }
        else
        {
           //todo: error handling
           qDebug()<<"Get invalid RV position: " << m_CurProgramStepInfo.stationID;
        }
    }
    else if(PSSM_READY_TO_SEAL == stepState)
    {
        //get target position here
        RVPosition_t targetPos = GetRVSealPositionByStationID(m_CurProgramStepInfo.stationID);
        if(RV_UNDEF != targetPos)
        {
            cmd->SetRVPosition(targetPos);
            m_SchedulerCommandProcessor->pushCmd(cmd);
        }
        else
        {
           //todo: error handling
           qDebug()<<"Get invalid RV position: " << m_CurProgramStepInfo.stationID;
        }
    }
}

void SchedulerMainThreadController::Fill()
{
    CmdALFilling* cmd  = new CmdALFilling(500, mp_IDeviceProcessing, this);
    //todo: get delay time here
    cmd->SetDelayTime(2000);
    m_SchedulerCommandProcessor->pushCmd(cmd);
}

void SchedulerMainThreadController::Soak()
{
    m_CurStepSoakStartTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    qDebug() << "Start to soak, start time stamp is:" << m_CurStepSoakStartTime;
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
    CmdALDraining* cmd  = new CmdALDraining(500, mp_IDeviceProcessing, this);
    //todo: get delay time here
    cmd->SetDelayTime(2000);
    m_SchedulerCommandProcessor->pushCmd(cmd);
}

void SchedulerMainThreadController::Pressure()
{
    m_SchedulerCommandProcessor->pushCmd(new CmdALPressure(500, mp_IDeviceProcessing, this));
}

void SchedulerMainThreadController::Vaccum()
{
    m_SchedulerCommandProcessor->pushCmd(new CmdALVaccum(500, mp_IDeviceProcessing, this));
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

RVPosition_t SchedulerMainThreadController::GetRVTubePositionByStationID(const QString stationID)
{
    RVPosition_t ret = RV_UNDEF;
    if(!stationID.isEmpty())
    {
        bool ok;
        int position = stationID.right(stationID.length() - 1).toInt(&ok, 10);
        if(ok)
        {
            ret = (RVPosition_t)(position * 2 - 1);
        }
    }
    return ret;
}

RVPosition_t SchedulerMainThreadController::GetRVSealPositionByStationID(const QString stationID)
{
    RVPosition_t ret = RV_UNDEF;
    if(!stationID.isEmpty())
    {
        bool ok;
        int position = stationID.right(stationID.length() - 1).toInt(&ok, 10);
        if(ok)
        {
            ret = (RVPosition_t)(position * 2);
        }
    }
    return ret;
}

} // EONS ::Scheduler

