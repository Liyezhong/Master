/****************************************************************************/
/*! \file CmdOvenGetTemperatureControlState.h
 *
 *  \brief CmdOvenGetTemperatureControlState command definition.
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


#ifndef CMDOVENGETTEMPERATURECONTROLSTATE_H
#define CMDOVENGETTEMPERATURECONTROLSTATE_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdOvenGetTemperatureControlState : public CmdSchedulerCommandBase
{
public:
    CmdOvenGetTemperatureControlState(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller);
    ~CmdOvenGetTemperatureControlState();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::TempCtrlState_t GetResult()const{return m_result;}
	bool GetResult(DeviceControl::TempCtrlState_t& result) const{result = m_result; return true;}
	DeviceControl::OVENTempCtrlType_t GetType(){return m_Type;}
	void SetType(DeviceControl::OVENTempCtrlType_t Type){m_Type = Type;}
	

    void Execute();

private:
    CmdOvenGetTemperatureControlState();                                                    ///< Not implemented.
    CmdOvenGetTemperatureControlState(const CmdOvenGetTemperatureControlState &);                     ///< Not implemented.
    const CmdOvenGetTemperatureControlState & operator = (const CmdOvenGetTemperatureControlState &); ///< Not implemented.


	mutable DeviceControl::TempCtrlState_t m_result;
	mutable DeviceControl::OVENTempCtrlType_t m_Type;
	
};



}

#endif // CMDOVENGETTEMPERATURECONTROLSTATE_H
