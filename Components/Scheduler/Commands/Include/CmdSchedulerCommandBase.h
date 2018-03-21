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
#include <QSharedPointer>

namespace Scheduler {

class SchedulerMainThreadController;

/****************************************************************************/
/*!
 *  \brief  This class implements a CmdCancelProgram command.
 *
 */
/****************************************************************************/
class CmdSchedulerCommandBase : public Global::Command {

public:
    /****************************************************************************/
    /*!
     *  \brief Definition/Declaration of function CmdSchedulerCommandBase
     *  \param Timeout
     *  \param controller
     */
    /****************************************************************************/
    CmdSchedulerCommandBase(int Timeout, const QString& sender);
    virtual ~CmdSchedulerCommandBase();

    /****************************************************************************/
    /*!
     *  \brief   Get command name
     *
     *  \return  command name as string
     */
    /****************************************************************************/
    virtual QString GetName() const = 0;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetResult
     *
     *  \param result = DeviceControl::ReturnCode_t type parameter
     *
     *  \return from GetResult
     */
    /****************************************************************************/
    virtual bool GetResult(DeviceControl::ReturnCode_t& result) const;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetResult
     *
     *  \param result = DeviceControl::CBaseDevice type parameter
     *
     *  \return from GetResult
     */
    /****************************************************************************/
    virtual bool GetResult(DeviceControl::CBaseDevice* result) const;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetResult
     *
     *  \param result = DeviceControl::TempCtrlState_t type parameter
     *
     *  \return from GetResult
     */
    /****************************************************************************/
    virtual bool GetResult(DeviceControl::TempCtrlState_t& result) const;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetResult
     *
     *  \param result = qreal type parameter
     *
     *  \return from GetResult
     */
    /****************************************************************************/
    virtual bool GetResult(qreal& result) const;
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetResult
     *
     *  \param result = DeviceControl::ReturnCode_t type parameter
     *
     *  \return from SetResult
     */
    /****************************************************************************/
    virtual void SetResult(DeviceControl::ReturnCode_t result);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetResult
     *
     *  \param result = qreal type parameter
     *
     *  \return from SetResult
     */
    /****************************************************************************/
    virtual void SetResult(qreal result);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetResult
     *
     *  \param result = DeviceControl::TempCtrlState_t type parameter
     *
     *  \return from SetResult
     */
    /****************************************************************************/
    virtual void SetResult(DeviceControl::TempCtrlState_t result);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetParameters
     *
     *  \return from GetParameters
     */
    /****************************************************************************/
    virtual QString GetParameters() const{return "";}
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetStrResult
     *
     *  \return from GetStrResult
     */
    /****************************************************************************/
    virtual QString GetStrResult() const{return "";}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetResponse
     *
     *  \return from GetResponse
     */
    /****************************************************************************/
    bool GetResponse() const{return m_Response;}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetResponse
     *  \param  response - bool
     *  \return none
     */
    /****************************************************************************/
    void SetResponse(bool response) {m_Response = response;}

    // todo: return the caller's id
    QString GetSender() const {return m_Sender;}

private:
    CmdSchedulerCommandBase();                                                    ///< Not implemented.
    CmdSchedulerCommandBase(const CmdSchedulerCommandBase &);                     ///< Not implemented.
    const CmdSchedulerCommandBase & operator = (const CmdSchedulerCommandBase &); ///< Not implemented.
    bool    m_Response;                                                           ///< flag for response
protected:
    QString m_Sender;       ///<  Definition/Declaration of variable mp_SchedulerThreadController
}; // end class CmdSchedulerCommandBase

typedef QSharedPointer<Scheduler::CmdSchedulerCommandBase>  SchedulerCommandShPtr_t;   ///< Typedef for shared pointer of command.
} // end namespace Scheduler

#endif // SCHEDULER_CMDSCHEDULERCOMMANDBASE_H
