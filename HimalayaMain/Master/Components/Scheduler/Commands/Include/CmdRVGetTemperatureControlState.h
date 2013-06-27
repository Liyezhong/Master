/****************************************************************************/
/*! \file CmdRVGetTemperatureControlState.h
 *
 *  \brief CmdRVGetTemperatureControlState command definition.
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


#ifndef CMDRVGETTEMPERATURECONTROLSTATE_H
#define CMDRVGETTEMPERATURECONTROLSTATE_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdRVGetTemperatureControlState : public CmdSchedulerCommandBase
{
public:
    CmdRVGetTemperatureControlState(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller);
    ~CmdRVGetTemperatureControlState();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::TempCtrlState_t GetResult()const{return m_result;}
	bool GetResult(DeviceControl::TempCtrlState_t& result) const{result = m_result; return true;}
	
	QString GetParameters()const{ return GetName() + QString("()");}
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

    void Execute();

private:
    CmdRVGetTemperatureControlState();                                                    ///< Not implemented.
    CmdRVGetTemperatureControlState(const CmdRVGetTemperatureControlState &);                     ///< Not implemented.
    const CmdRVGetTemperatureControlState & operator = (const CmdRVGetTemperatureControlState &); ///< Not implemented.


	mutable DeviceControl::TempCtrlState_t m_result;
	
};



}

#endif // CMDRVGETTEMPERATURECONTROLSTATE_H
