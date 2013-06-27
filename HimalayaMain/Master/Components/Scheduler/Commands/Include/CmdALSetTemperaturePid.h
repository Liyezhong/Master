/****************************************************************************/
/*! \file CmdALSetTemperaturePid.h
 *
 *  \brief CmdALSetTemperaturePid command definition.
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


#ifndef CMDALSETTEMPERATUREPID_H
#define CMDALSETTEMPERATUREPID_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdALSetTemperaturePid : public CmdSchedulerCommandBase
{
public:
    CmdALSetTemperaturePid(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller);
    ~CmdALSetTemperaturePid();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::ReturnCode_t GetResult()const{return m_result;}
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	DeviceControl::ALTempCtrlType_t GetType(){return m_Type;}
	void SetType(DeviceControl::ALTempCtrlType_t Type){m_Type = Type;}
	quint16 GetMaxTemperature(){return m_MaxTemperature;}
	void SetMaxTemperature(quint16 MaxTemperature){m_MaxTemperature = MaxTemperature;}
	quint16 GetControllerGain(){return m_ControllerGain;}
	void SetControllerGain(quint16 ControllerGain){m_ControllerGain = ControllerGain;}
	quint16 GetResetTime(){return m_ResetTime;}
	void SetResetTime(quint16 ResetTime){m_ResetTime = ResetTime;}
	quint16 GetDerivativeTime(){return m_DerivativeTime;}
	void SetDerivativeTime(quint16 DerivativeTime){m_DerivativeTime = DerivativeTime;}
	
	QString GetParameters()const{ return GetName() + QString("(%1,%2,%3,%4,%5)").arg(m_Type).arg(m_MaxTemperature).arg(m_ControllerGain).arg(m_ResetTime).arg(m_DerivativeTime);}
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

    void Execute();

private:
    CmdALSetTemperaturePid();                                                    ///< Not implemented.
    CmdALSetTemperaturePid(const CmdALSetTemperaturePid &);                     ///< Not implemented.
    const CmdALSetTemperaturePid & operator = (const CmdALSetTemperaturePid &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;
	mutable DeviceControl::ALTempCtrlType_t m_Type;
	mutable quint16 m_MaxTemperature;
	mutable quint16 m_ControllerGain;
	mutable quint16 m_ResetTime;
	mutable quint16 m_DerivativeTime;
	
};



}

#endif // CMDALSETTEMPERATUREPID_H
