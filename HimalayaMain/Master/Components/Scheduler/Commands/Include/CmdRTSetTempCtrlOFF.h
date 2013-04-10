/****************************************************************************/
/*! \file CmdRTSetTempCtrlOFF.h
 *
 *  \brief CmdRTSetTempCtrlOFF command definition.
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


#ifndef CMDRTSETTEMPCTRLOFF_H
#define CMDRTSETTEMPCTRLOFF_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdRTSetTempCtrlOFF : public CmdSchedulerCommandBase
{
public:
    CmdRTSetTempCtrlOFF(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller);
    ~CmdRTSetTempCtrlOFF();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::ReturnCode_t GetResult()const{return m_result;}
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	DeviceControl::RTTempCtrlType_t GetType(){return m_Type;}
	void SetType(DeviceControl::RTTempCtrlType_t Type){m_Type = Type;}
	

    void Execute();

private:
    CmdRTSetTempCtrlOFF();                                                    ///< Not implemented.
    CmdRTSetTempCtrlOFF(const CmdRTSetTempCtrlOFF &);                     ///< Not implemented.
    const CmdRTSetTempCtrlOFF & operator = (const CmdRTSetTempCtrlOFF &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;
	mutable DeviceControl::RTTempCtrlType_t m_Type;
	
};



}

#endif // CMDRTSETTEMPCTRLOFF_H
