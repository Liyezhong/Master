/****************************************************************************/
/*! \file CmdALStopCmdExec.h
 *
 *  \brief CmdALStopCmdExec command definition.
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


#ifndef CMDALSTOPCMDEXEC_H
#define CMDALSTOPCMDEXEC_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdALStopCmdExec
 */
/****************************************************************************/
class CmdALStopCmdExec : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdALStopCmdExec
     *
     *  \param Timeout = int type parameter
     *  \param controller =  SchedulerMainThreadController type parameter
     *
     *  \return from CmdALFilling
     */
    /****************************************************************************/
    CmdALStopCmdExec(int Timeout, const QString& sender);
    ~CmdALStopCmdExec();

    static QString NAME;    ///< Command name.

    QString GetName() const{return NAME;}

    /****************************************************************************/
    /*! 
     *  \brief  Definition/Declaration of function GetCmdType
     *
     *  \return from GetCmdType
     */
    /****************************************************************************/
    quint8 GetCmdType(){return m_CmdType;}

    /****************************************************************************/
    /*! 
     *  \brief  Definition/Declaration of function SetCmdType
     *
     *  \param CmdType = quint32 type parameter
     *
     *  \return from SetCmdType
     */
    /****************************************************************************/
    void SetCmdType(quint8 CmdType){m_CmdType= CmdType;}


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
private:
    CmdALStopCmdExec(const CmdALStopCmdExec &);                     ///< Not implemented.
    const CmdALStopCmdExec & operator = (const CmdALStopCmdExec &); ///< Not implemented.
    mutable DeviceControl::ReturnCode_t m_result;   				///<  Definition/Declaration of variable m_result
	quint8 m_CmdType;												///< Command Type
	
};
}

#endif // CMDALSTOPCMDEXEC_H
