/****************************************************************************/
/*! \file CmdRmtLocAlarm.h
 *
 *  \brief CmdRmtLocAlarm command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 21.08.2014
 *   $Author:  $ Jeff Chen
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


#ifndef CMD_RMTLOC_ALARM_H
#define CMD_RMTLOC_ALARM_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler {

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdRmtLocAlarm
 */
/****************************************************************************/
class CmdRmtLocAlarm : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdRmtLocAlarm
     *
     *  \param Timeout = int type parameter
     *  \param controller =  SchedulerMainThreadController type parameter
     *
     *  \return from CmdRmtLocAlarm
     */
    /****************************************************************************/
    CmdRmtLocAlarm(int Timeout, SchedulerMainThreadController *controller);
    ~CmdRmtLocAlarm();

    static QString NAME;    ///< Command name.

    QString GetName() const{ return NAME; }

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetResult
     *
     *  \return from GetResult
     */
    /****************************************************************************/
    DeviceControl::ReturnCode_t GetResult()const { return m_result; }
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetResult
     *
     *  \param result = DeviceControl::ReturnCode_t type parameter
     *
     *  \return from SetResult
     */
    /****************************************************************************/
    void SetResult(DeviceControl::ReturnCode_t result) { m_result = result; }
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetResult
     *
     *  \param result = DeviceControl::ReturnCode_t type parameter
     *
     *  \return from GetResult
     */
    /**************************************************************l**************/
    bool GetResult(DeviceControl::ReturnCode_t& result) const{ result = m_result; return true; }

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetStrResult
     *
     *  \return from GetStrResult
     */
    /****************************************************************************/
    QString GetStrResult()const{ return QString("%1").arg((qint32)m_result);}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetRmtLocOpcode
     *  \param  code - int
     *  \return from SetRmtLocOpcode
     */
    /****************************************************************************/
    void SetRmtLocOpcode(int code) { opcode = code; }
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetRmtLocOpcode
     *
     *  \return opcode
     */
    /****************************************************************************/
    int GetRmtLocOpcode(void) const { return opcode; }

private:
    CmdRmtLocAlarm();                                              ///< Not implemented.
    CmdRmtLocAlarm(const CmdRmtLocAlarm &);                     ///< Not implemented.
    const CmdRmtLocAlarm & operator = (const CmdRmtLocAlarm &); ///< Not implemented.

    mutable DeviceControl::ReturnCode_t m_result;       ///<  Definition/Declaration of variable m_result
    int opcode;   ///<  Definition/Declaration of variable opcode
};

}

#endif //CMD_RMTLOC_ALARM_H

