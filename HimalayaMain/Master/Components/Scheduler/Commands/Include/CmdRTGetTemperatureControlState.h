/****************************************************************************/
/*! \file CmdRTGetTemperatureControlState.h
 *
 *  \brief CmdRTGetTemperatureControlState command definition.
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


#ifndef CMDRTGETTEMPERATURECONTROLSTATE_H
#define CMDRTGETTEMPERATURECONTROLSTATE_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdRTGetTemperatureControlState
 */
/****************************************************************************/
class CmdRTGetTemperatureControlState : public CmdSchedulerCommandBase
{
public:
    CmdRTGetTemperatureControlState(int Timeout, SchedulerMainThreadController *controller);
    ~CmdRTGetTemperatureControlState();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::TempCtrlState_t GetResult()const {return m_result;}
	void SetResult(DeviceControl::TempCtrlState_t result) { m_result = result;}
	bool GetResult(DeviceControl::TempCtrlState_t& result) const{result = m_result; return true;}
	DeviceControl::RTTempCtrlType_t GetType(){return m_Type;}
	void SetType(DeviceControl::RTTempCtrlType_t Type){m_Type = Type;}
	
	QString GetParameters()const{ return GetName() + QString("(%1)").arg(m_Type);}
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

private:
    CmdRTGetTemperatureControlState();                                                    ///< Not implemented.
    CmdRTGetTemperatureControlState(const CmdRTGetTemperatureControlState &);                     ///< Not implemented.
    const CmdRTGetTemperatureControlState & operator = (const CmdRTGetTemperatureControlState &); ///< Not implemented.


	mutable DeviceControl::TempCtrlState_t m_result;
	mutable DeviceControl::RTTempCtrlType_t m_Type;
	
};



}

#endif // CMDRTGETTEMPERATURECONTROLSTATE_H
