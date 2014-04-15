/****************************************************************************/
/*! \file CmdOvenStartTemperatureControlWithPID.h
 *
 *  \brief CmdOvenStartTemperatureControlWithPID command definition.
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


#ifndef CMDOVENSTARTTEMPERATURECONTROLWITHPID_H
#define CMDOVENSTARTTEMPERATURECONTROLWITHPID_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdOvenStartTemperatureControlWithPID
 */
/****************************************************************************/
class CmdOvenStartTemperatureControlWithPID : public CmdSchedulerCommandBase
{
public:
    CmdOvenStartTemperatureControlWithPID(int Timeout, SchedulerMainThreadController *controller);
    ~CmdOvenStartTemperatureControlWithPID();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::ReturnCode_t GetResult()const {return m_result;}
	void SetResult(DeviceControl::ReturnCode_t result) { m_result = result;}
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	DeviceControl::OVENTempCtrlType_t GetType(){return m_Type;}
	void SetType(DeviceControl::OVENTempCtrlType_t Type){m_Type = Type;}
	qreal GetNominalTemperature(){return m_NominalTemperature;}
	void SetNominalTemperature(qreal NominalTemperature){m_NominalTemperature = NominalTemperature;}
	quint8 GetSlopeTempChange(){return m_SlopeTempChange;}
	void SetSlopeTempChange(quint8 SlopeTempChange){m_SlopeTempChange = SlopeTempChange;}
	quint16 GetMaxTemperature(){return m_MaxTemperature;}
	void SetMaxTemperature(quint16 MaxTemperature){m_MaxTemperature = MaxTemperature;}
	quint16 GetControllerGain(){return m_ControllerGain;}
	void SetControllerGain(quint16 ControllerGain){m_ControllerGain = ControllerGain;}
	quint16 GetResetTime(){return m_ResetTime;}
	void SetResetTime(quint16 ResetTime){m_ResetTime = ResetTime;}
	quint16 GetDerivativeTime(){return m_DerivativeTime;}
	void SetDerivativeTime(quint16 DerivativeTime){m_DerivativeTime = DerivativeTime;}
	
	QString GetParameters()const{ return GetName() + QString("(%1,%2,%3,%4,%5,%6,%7)").arg(m_Type).arg(m_NominalTemperature).arg(m_SlopeTempChange).arg(m_MaxTemperature).arg(m_ControllerGain).arg(m_ResetTime).arg(m_DerivativeTime);}
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

private:
    CmdOvenStartTemperatureControlWithPID();                                                    ///< Not implemented.
    CmdOvenStartTemperatureControlWithPID(const CmdOvenStartTemperatureControlWithPID &);                     ///< Not implemented.
    const CmdOvenStartTemperatureControlWithPID & operator = (const CmdOvenStartTemperatureControlWithPID &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;
	mutable DeviceControl::OVENTempCtrlType_t m_Type;
	mutable qreal m_NominalTemperature;
	mutable quint8 m_SlopeTempChange;
	mutable quint16 m_MaxTemperature;
	mutable quint16 m_ControllerGain;
	mutable quint16 m_ResetTime;
	mutable quint16 m_DerivativeTime;
	
};



}

#endif // CMDOVENSTARTTEMPERATURECONTROLWITHPID_H
