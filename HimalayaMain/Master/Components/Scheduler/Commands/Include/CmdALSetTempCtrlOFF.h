/****************************************************************************/
/*! \file CmdALSetTempCtrlOFF.h
 *
 *  \brief CmdALSetTempCtrlOFF command definition.
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


#ifndef CMDALSETTEMPCTRLOFF_H
#define CMDALSETTEMPCTRLOFF_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdALSetTempCtrlOFF : public CmdSchedulerCommandBase
{
public:
    CmdALSetTempCtrlOFF(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller);
    ~CmdALSetTempCtrlOFF();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::ReturnCode_t GetResult()const{return m_result;}
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	DeviceControl::ALTempCtrlType_t Gettype(){return m_type;}
	void Settype(DeviceControl::ALTempCtrlType_t type){m_type = type;}
	
	QString GetParameters()const{ return GetName() + QString("(%1)").arg(m_type);}
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

    void Execute();

private:
    CmdALSetTempCtrlOFF();                                                    ///< Not implemented.
    CmdALSetTempCtrlOFF(const CmdALSetTempCtrlOFF &);                     ///< Not implemented.
    const CmdALSetTempCtrlOFF & operator = (const CmdALSetTempCtrlOFF &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;
	mutable DeviceControl::ALTempCtrlType_t m_type;
	
};



}

#endif // CMDALSETTEMPCTRLOFF_H
