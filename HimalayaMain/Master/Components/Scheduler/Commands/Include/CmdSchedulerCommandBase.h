/****************************************************************************/
/*! \file CmdSchedulerCommandBase.h
 *
 *  \brief CmdSchedulerCommandBase command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 29.03.2013
 *   $Author:  $ L. Yang
 *
 *  \b Company:
 *
 *       Leica Biosystems SH CN.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef SCHEDULER_CMDSCHEDULERCOMMANDBASE_H
#define SCHEDULER_CMDSCHEDULERCOMMANDBASE_H

#include "Global/Include/Commands/Command.h"
#include "DeviceControl/Include/Interface/IDeviceProcessing.h"



namespace Scheduler {

class SchedulerMainThreadController;

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdCancelProgram command.
 *
 * \todo implement
 */
/****************************************************************************/
class CmdSchedulerCommandBase : public Global::Command {

public:
    /****************************************************************************/
    CmdSchedulerCommandBase(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController* controller);
    ~CmdSchedulerCommandBase();

    /****************************************************************************/
    /*!
     *  \brief   Get command name
     *
     *  \return  command name as string
     */
    /****************************************************************************/
    virtual QString GetName() const = 0;
    virtual bool GetResult(DeviceControl::ReturnCode_t& result) const;
    virtual bool GetResult(DeviceControl::CBaseDevice* result) const;
    virtual bool GetResult(DeviceControl::TempCtrlState_t& result) const;
    virtual bool GetResult(qreal& result) const;
    virtual QString GetParameters() const{return "";}
    virtual QString GetStrResult() const{return "";}


    /****************************************************************************/
    /*!
     *  \brief   Execute command by calling IDeviceProcessing API
     *
     *  \return  void
     */
    /****************************************************************************/
    virtual void Execute() = 0;

private:
    CmdSchedulerCommandBase();                                                    ///< Not implemented.
    CmdSchedulerCommandBase(const CmdSchedulerCommandBase &);                     ///< Not implemented.
    const CmdSchedulerCommandBase & operator = (const CmdSchedulerCommandBase &); ///< Not implemented.
protected:
    DeviceControl::IDeviceProcessing *m_IDeviceProcessing;
    SchedulerMainThreadController *mp_SchedulerThreadController;
}; // end class CmdSchedulerCommandBase

typedef Global::SharedPointer<Scheduler::CmdSchedulerCommandBase>  SchedulerCommandShPtr_t;   ///< Typedef for shared pointer of command.
} // end namespace Scheduler

#endif // SCHEDULER_CMDSCHEDULERCOMMANDBASE_H





















#ifndef CMDSCHEDULERCOMMANDBASE_H
#define CMDSCHEDULERCOMMANDBASE_H

#endif // CMDSCHEDULERCOMMANDBASE_H
