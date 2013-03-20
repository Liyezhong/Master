// ================================================================================================
/*! @file Himalaya/HimalayaMain/Master/Components/Scheduler/Include/SchedulerCommandProcessor.h
 *
 *  @brief
 *  Definition of class Scheduler::SchedulerCommandProcessor.
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



#ifndef SCHEDULERCOMMANDPROCESSOR_H
#define SCHEDULERCOMMANDPROCESSOR_H

#include <QObject>
#include <QQueue>
#include <QMutex>

#include "Global/Include/Commands/Command.h"

namespace DeviceControl
{
    class IDeviceProcessing;
}

namespace Scheduler{

class SchedulerMainThreadController;


/****************************************************************************/
/**
 * @brief command processing thread class for the main scheduler thread.
 *
 */
/****************************************************************************/
class SchedulerCommandProcessor : public QObject
{
    Q_OBJECT
public:
    /****************************************************************************/
    /**
     * @brief constructor.
     *
     */
    /****************************************************************************/
    explicit SchedulerCommandProcessor(DeviceControl::IDeviceProcessing *IDP,SchedulerMainThreadController *controller);

    
signals:
    /****************************************************************************/
    /**
     * @brief emit the signal when command be finished!
     *
     */
    /****************************************************************************/
    void onCmdFinished(Global::CommandShPtr_t *cmd, bool result);


public slots:
    /****************************************************************************/
    /**
     * @brief thread's callback function
     *
     */
    /****************************************************************************/
    void run();

    /****************************************************************************/
    /**
     * @brief controller call this function to add a new command to process.
     *
     */
    /****************************************************************************/
    void pushCmd(Global::CommandShPtr_t *cmd);

private:

    /****************************************************************************/
    /**
     * @brief check if there are new command comming.
     *
     */
    /****************************************************************************/
    bool newCmdComing();

    SchedulerCommandProcessor();                                             ///< Not implemented.
    SchedulerCommandProcessor(const SchedulerCommandProcessor&);                      ///< Not implemented.
    const SchedulerCommandProcessor& operator=(const SchedulerCommandProcessor&);     ///< Not implemented.

private:
    DeviceControl::IDeviceProcessing *m_IDP;
    SchedulerMainThreadController *mp_SchedulerThreadController;

    QQueue<Global::CommandShPtr_t *> m_Cmds;
    Global::CommandShPtr_t * m_currentCmd;
    QMutex m_CmdMutex;

    
};

} // end of namespace Scheduler

#endif // SCHEDULERCOMMANDPROCESSOR_H
