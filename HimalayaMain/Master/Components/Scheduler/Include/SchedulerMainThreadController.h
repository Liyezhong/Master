// ================================================================================================
/*! @file Himalaya/HimalayaMain/Master/Components/Scheduler/Include/SchedulerMainThreadController.h
 *
 *  @brief
 *  Definition of class Scheduler::Schedule.
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
#ifndef SCHEDULER_MAINTHREADCONTROLLER_H
#define SCHEDULER_MAINTHREADCONTROLLER_H

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//                                                                                  From Qt-Library
#include <QTimer>
#include <QQueue>
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//                                                                                 Project Specific
#include "Threads/Include/ThreadController.h"
#include <HimalayaErrorHandler/Include/Commands/CmdRaiseAlarm.h>
#include <Global/Include/Commands/Command.h>
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAction.h"
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdRetortLock.h"

namespace Scheduler {

class SchedulerCommandProcessor;

    /****************************************************************************/
    /**
     * @brief Controller class for the main scheduler thread.
     *
     */
    /****************************************************************************/
    class SchedulerMainThreadController : public Threads::ThreadController {
        Q_OBJECT

    private:
        QTimer m_TickTimer;
        QMutex m_Mutex;
        QQueue<Global::CommandShPtr_t> m_SchedulerCmdQueue;
        QQueue<Global::Command*> m_DeviceControlCmdQueue;

        QThread* m_SchedulerCommandProcessorThread;
        SchedulerCommandProcessor* m_SchedulerCommandProcessor;

        SchedulerMainThreadController();                                             ///< Not implemented.
        SchedulerMainThreadController(const SchedulerMainThreadController&);                      ///< Not implemented.
        const SchedulerMainThreadController& operator=(const SchedulerMainThreadController&);     ///< Not implemented.
        /****************************************************************************/
        /**
         * @brief Register commands.
         */
        /****************************************************************************/
        virtual void RegisterCommands();
    protected:

        /****************************************************************************/
        /**
         * Called when the base class received the \ref Go signal.
         *
         * This means that everything is fine and normal operation started.
         * We are running in our own thread now.
         */
        /****************************************************************************/
        virtual void OnGoReceived();

        /****************************************************************************/
        /**
         * Called when the base class received the \ref Stop signal.
         *
         * This means that normal operation will stop after processing this signal.
         * We are still running in our own thread.
         */
        /****************************************************************************/
        virtual void OnStopReceived();
        
        /****************************************************************************/
        /**
         * Called when power failure is to expect.
         */
        /****************************************************************************/
        virtual void OnPowerFail();

        /****************************************************************************/
        /**
         * Called when a local/remote alarm raises.
         */
        /****************************************************************************/
        void OnRaiseAlarmLocalRemote(Global::tRefType Ref, const HimalayaErrorHandler::CmdRaiseAlarm &Cmd);

        void OnProgramAction(Global::tRefType Ref, const MsgClasses::CmdProgramAction& Cmd);
        void OnOnRetortLock(Global::tRefType Ref, const MsgClasses::CmdRetortLock& Cmd);

    public:
        /****************************************************************************/
        /**
         * @brief Constructor.
         *
         * @iparam   TheHeartBeatSource    Logging source to be used.
         */
        /****************************************************************************/
        SchedulerMainThreadController(
            Global::gSourceType TheHeartBeatSource
        );

        /****************************************************************************/
        /**
         * @brief Destructor.
         */
        /****************************************************************************/
        virtual ~SchedulerMainThreadController();

        /****************************************************************************/
        /**
         * Create and configure your objects.
         */
        /****************************************************************************/
        virtual void CreateAndInitializeObjects();

        /****************************************************************************/
        /**
         * Cleanup and destroy your objects.
         */
        /****************************************************************************/
        virtual void CleanupAndDestroyObjects();

    public slots:

        /****************************************************************************/
        /**
         * @brief For advancing the time
         */
        /****************************************************************************/
        void OnTickTimer();

    };

} // EONS ::Scheduler

#endif
