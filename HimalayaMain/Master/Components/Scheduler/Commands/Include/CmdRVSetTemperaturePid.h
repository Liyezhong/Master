/****************************************************************************/
/*! \file CmdRVSetTemperaturePid.h
 *
 *  \brief CmdRVSetTemperaturePid command definition.
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


#ifndef CMDRVSETTEMPERATUREPID_H
#define CMDRVSETTEMPERATUREPID_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdRVSetTemperaturePid
 */
/****************************************************************************/
class CmdRVSetTemperaturePid : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdRVSetTemperaturePid
     *
     *  \return from CmdRVSetTemperaturePid
     */
    /****************************************************************************/
    CmdRVSetTemperaturePid(int Timeout, SchedulerMainThreadController *controller);
    ~CmdRVSetTemperaturePid();

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
	 *  \return from SetResult
	 */
	/****************************************************************************/
	void SetResult(DeviceControl::ReturnCode_t result) { m_result = result;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetResult
	 *
	 *  \return from GetResult
	 */
	/****************************************************************************/
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetMaxTemperature
	 *
	 *  \return from GetMaxTemperature
	 */
	/****************************************************************************/
	quint16 GetMaxTemperature(){return m_MaxTemperature;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function SetMaxTemperature
	 *
	 *  \return from SetMaxTemperature
	 */
	/****************************************************************************/
	void SetMaxTemperature(quint16 MaxTemperature){m_MaxTemperature = MaxTemperature;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetControllerGain
	 *
	 *  \return from GetControllerGain
	 */
	/****************************************************************************/
	quint16 GetControllerGain(){return m_ControllerGain;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function SetControllerGain
	 *
	 *  \return from SetControllerGain
	 */
	/****************************************************************************/
	void SetControllerGain(quint16 ControllerGain){m_ControllerGain = ControllerGain;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetResetTime
	 *
	 *  \return from GetResetTime
	 */
	/****************************************************************************/
	quint16 GetResetTime(){return m_ResetTime;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function SetResetTime
	 *
	 *  \return from SetResetTime
	 */
	/****************************************************************************/
	void SetResetTime(quint16 ResetTime){m_ResetTime = ResetTime;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetDerivativeTime
	 *
	 *  \return from GetDerivativeTime
	 */
	/****************************************************************************/
	quint16 GetDerivativeTime(){return m_DerivativeTime;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function SetDerivativeTime
	 *
	 *  \return from SetDerivativeTime
	 */
	/****************************************************************************/
	void SetDerivativeTime(quint16 DerivativeTime){m_DerivativeTime = DerivativeTime;}
	
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetParameters
	 *
	 *  \return from GetParameters
	 */
	/****************************************************************************/
	QString GetParameters()const{ return GetName() + QString("(%1,%2,%3,%4)").arg(m_MaxTemperature).arg(m_ControllerGain).arg(m_ResetTime).arg(m_DerivativeTime);}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetStrResult
	 *
	 *  \return from GetStrResult
	 */
	/****************************************************************************/
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

private:
    CmdRVSetTemperaturePid();                                                    ///< Not implemented.
    CmdRVSetTemperaturePid(const CmdRVSetTemperaturePid &);                     ///< Not implemented.
    const CmdRVSetTemperaturePid & operator = (const CmdRVSetTemperaturePid &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;       ///<  Definition/Declaration of variable m_result
	mutable quint16 m_MaxTemperature;       ///<  Definition/Declaration of variable m_MaxTemperature
	mutable quint16 m_ControllerGain;       ///<  Definition/Declaration of variable m_ControllerGain
	mutable quint16 m_ResetTime;       ///<  Definition/Declaration of variable m_ResetTime
	mutable quint16 m_DerivativeTime;       ///<  Definition/Declaration of variable m_DerivativeTime
	
};



}

#endif // CMDRVSETTEMPERATUREPID_H
