/****************************************************************************/
/*! \file CmdALPurge.h
 *
 *  \brief CmdALPurge command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 07.06.2018
 *   $Author:  $ Dixiong Li
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
#ifndef CMDALPURGE_H
#define CMDALPURGE_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdALFilling
 */
/****************************************************************************/
class CmdALPurge : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdALFilling
     *
     *  \param Timeout = int type parameter
     *  \param controller =  SchedulerMainThreadController type parameter
     *
     *  \return from CmdALPurge
     */
    /****************************************************************************/
    CmdALPurge(int Timeout, const QString& sender);
    ~CmdALPurge();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetResult
     *
     *  \return from GetResult
     */
    /****************************************************************************/
    DeviceControl::ReturnCode_t GetResult()const {return m_result;}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetResult
     *
     *  \param result = DeviceControl::ReturnCode_t type parameter
     *
     *  \return from SetResult
     */
    /****************************************************************************/
    void SetResult(DeviceControl::ReturnCode_t result) { m_result = result;}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetResult
     *
     *  \param result = DeviceControl::ReturnCode_t type parameter
     *
     *  \return from GetResult
     */
    /****************************************************************************/
    bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetDelayTime
     *
     *  \return from GetDelayTime
     */
    /****************************************************************************/
    quint32 GetDelayTime(){return m_DelayTime;}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetDelayTime
     *
     *  \param DelayTime = quint32 type parameter
     *
     *  \return from SetDelayTime
     */
    /****************************************************************************/
    void SetDelayTime(quint32 DelayTime){m_DelayTime = DelayTime;}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetStrResult
     *
     *  \return from GetStrResult
     */
    /****************************************************************************/
    QString GetStrResult()const{ return QString("%1").arg((qint32)m_result);}

private:
    CmdALPurge();                                         ///< Not implemented.
    CmdALPurge(const CmdALPurge &);                     ///< Not implemented.
    const CmdALPurge & operator = (const CmdALPurge &); ///< Not implemented.


    mutable DeviceControl::ReturnCode_t m_result;   ///<  Definition/Declaration of variable m_result
    mutable quint32 m_DelayTime;                    ///<  Definition/Declaration of variable m_DelayTime

};
}

#endif // CMDALPURGE_H
