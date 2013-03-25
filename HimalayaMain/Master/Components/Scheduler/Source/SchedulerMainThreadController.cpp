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
#include "Scheduler/Include/SchedulerCommandProcessor.h"


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
    {
  
    }

    SchedulerMainThreadController::~SchedulerMainThreadController()
    {
        delete m_SchedulerCommandProcessorThread;
        m_SchedulerCommandProcessorThread = NULL;
        delete m_SchedulerCommandProcessor;
        m_SchedulerCommandProcessor = NULL;
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

    }

    void SchedulerMainThreadController::CreateAndInitializeObjects()
    {

        //create the SchedulerCommandProcessor thread
        m_SchedulerCommandProcessorThread = new QThread();
        DeviceControl::IDeviceProcessing* pIDeviceProcessing = NULL;//to do??

        m_SchedulerCommandProcessor = new SchedulerCommandProcessor(pIDeviceProcessing, this);
        //CONNECTSIGNALSLOT(&m_TickTimer, timeout(), m_SchedulerCommandProcessor, run());

        m_SchedulerCommandProcessor->moveToThread(m_SchedulerCommandProcessorThread);
        m_SchedulerCommandProcessorThread->start();

        //timer setting
        CONNECTSIGNALSLOT(&m_TickTimer, timeout(), this, OnTickTimer());
        m_TickTimer.setInterval(500);
        m_TickTimer.start();

        //command queue reset
        m_SchedulerCmdQueue.clear();
        m_DeviceControlCmdQueue.clear();

        // now register commands
        RegisterCommands();

        //InitializeDevice();
        //CreateDeviceThread();
    }

    void SchedulerMainThreadController::CleanupAndDestroyObjects()
    {
    }

    void SchedulerMainThreadController::OnGoReceived()
    {
       m_TickTimer.start();
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
            //check devcies
            //
            //process scheduler command
           //Global::CommandShPtr_t pt = mSchedulerCmdQueue.head();
           //send device control command

           //send scheduler command, like program status, request reply

    }

    void SchedulerMainThreadController::OnRaiseAlarmLocalRemote(Global::tRefType Ref,
                                                         const HimalayaErrorHandler::CmdRaiseAlarm &Cmd)
    {
        Q_UNUSED(Ref);
        m_Mutex.lock();
        m_SchedulerCmdQueue.enqueue(Global::CommandShPtr_t(const_cast<HimalayaErrorHandler::CmdRaiseAlarm*>(&Cmd)));
        m_Mutex.unlock();
    }

    void SchedulerMainThreadController::OnProgramAction(Global::tRefType Ref,
                                                        const MsgClasses::CmdProgramAction &Cmd)
    {
        Q_UNUSED(Ref);
        Q_UNUSED(Cmd);
    }

    void SchedulerMainThreadController::OnOnRetortLock(Global::tRefType Ref, const MsgClasses::CmdRetortLock &Cmd)
    {
        Q_UNUSED(Ref);
        Q_UNUSED(Cmd);
    }


} // EONS ::Scheduler

