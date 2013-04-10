/****************************************************************************/
/*! \file CmdRTSetTemperaturePid.h
 *
 *  \brief CmdRTSetTemperaturePid command definition.
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


#ifndef CMDRTSETTEMPERATUREPID_H
#define CMDRTSETTEMPERATUREPID_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdRTSetTemperaturePid : public CmdSchedulerCommandBase
{
public:
    CmdRTSetTemperaturePid(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller);
    ~CmdRTSetTemperaturePid();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::ReturnCode_t GetResult()const{return m_result;}
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	DeviceControl::RTTempCtrlType_t GetType(){return m_Type;}
	void SetType(DeviceControl::RTTempCtrlType_t Type){m_Type = Type;}
	quint16 GetMaxTemperature(){return m_MaxTemperature;}
	void SetMaxTemperature(quint16 MaxTemperature){m_MaxTemperature = MaxTemperature;}
	quint16 GetControllerGain(){return m_ControllerGain;}
	void SetControllerGain(quint16 ControllerGain){m_ControllerGain = ControllerGain;}
	quint16 GetResetTime(){return m_ResetTime;}
	void SetResetTime(quint16 ResetTime){m_ResetTime = ResetTime;}
	quint16 GetDerivativeTime(){return m_DerivativeTime;}
	void SetDerivativeTime(quint16 DerivativeTime){m_DerivativeTime = DerivativeTime;}
	

    void Execute();

private:
    CmdRTSetTemperaturePid();                                                    ///< Not implemented.
    CmdRTSetTemperaturePid(const CmdRTSetTemperaturePid &);                     ///< Not implemented.
    const CmdRTSetTemperaturePid & operator = (const CmdRTSetTemperaturePid &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;
	mutable DeviceControl::RTTempCtrlType_t m_Type;
	mutable quint16 m_MaxTemperature;
	mutable quint16 m_ControllerGain;
	mutable quint16 m_ResetTime;
	mutable quint16 m_DerivativeTime;
	
};



}

#endif // CMDRTSETTEMPERATUREPID_H
