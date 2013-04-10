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
#include "DeviceControl/Include/Interface/IDeviceProcessing.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "DeviceControl/Include/Devices/RotaryValveDevice.h"
#include "DeviceControl/Include/Devices/AirLiquidDevice.h"
#include "DeviceControl/Include/Devices/RetortDevice.h"
#include "DeviceControl/Include/Devices/OvenDevice.h"
#include "DeviceControl/Include/Devices/PeripheryDevice.h"
#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

using namespace DeviceControl;
//{
//    class IDeviceProcessing;
//}

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
    explicit SchedulerCommandProcessor(SchedulerMainThreadController *controller);

    
signals:
    /****************************************************************************/
    /**
     * @brief emit the signal when command be finished!
     *
     */
    /****************************************************************************/
    void onCmdFinished(Global::CommandShPtr_t *cmd, bool result);
    void DCLConfigurationFinished(ReturnCode_t RetCode, IDeviceProcessing* pIDP);
    void NewCmdAdded();

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
    void pushCmd(CmdSchedulerCommandBase *cmd);

    void DevProcInitialisationAckn(DevInstanceID_t instanceID, ReturnCode_t configResult);
    void DevProcConfigurationAckn(DevInstanceID_t instanceID, ReturnCode_t hdlInfo);
    void DevProcStartNormalOpModeAckn(DevInstanceID_t instanceID, ReturnCode_t hdlInfo);
    void ThrowError(DevInstanceID_t instanceID, quint16 usErrorGroup, quint16 usErrorID, quint16 usErrorData,const QDateTime & TimeStamp);
    void DevProcDestroyAckn();
    void OnNewCmdAdded();

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
    DeviceControl::IDeviceProcessing *mp_IDeviceProcessing;
    SchedulerMainThreadController *mp_SchedulerThreadController;

    QQueue<Scheduler::SchedulerCommandShPtr_t> m_Cmds;
    Scheduler::SchedulerCommandShPtr_t m_currentCmd;
    QMutex m_CmdMutex;


};

} // end of namespace Scheduler

#endif // SCHEDULERCOMMANDPROCESSOR_H
