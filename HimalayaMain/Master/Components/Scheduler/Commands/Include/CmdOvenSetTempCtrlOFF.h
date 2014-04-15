/****************************************************************************/
/*! \file CmdOvenSetTempCtrlOFF.h
 *
 *  \brief CmdOvenSetTempCtrlOFF command definition.
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


#ifndef CMDOVENSETTEMPCTRLOFF_H
#define CMDOVENSETTEMPCTRLOFF_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdOvenSetTempCtrlOFF
 */
/****************************************************************************/
class CmdOvenSetTempCtrlOFF : public CmdSchedulerCommandBase
{
public:
    CmdOvenSetTempCtrlOFF(int Timeout, SchedulerMainThreadController *controller);
    ~CmdOvenSetTempCtrlOFF();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::ReturnCode_t GetResult()const {return m_result;}
	void SetResult(DeviceControl::ReturnCode_t result) { m_result = result;}
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	DeviceControl::OVENTempCtrlType_t Gettype(){return m_type;}
	void Settype(DeviceControl::OVENTempCtrlType_t type){m_type = type;}
	
	QString GetParameters()const{ return GetName() + QString("(%1)").arg(m_type);}
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

private:
    CmdOvenSetTempCtrlOFF();                                                    ///< Not implemented.
    CmdOvenSetTempCtrlOFF(const CmdOvenSetTempCtrlOFF &);                     ///< Not implemented.
    const CmdOvenSetTempCtrlOFF & operator = (const CmdOvenSetTempCtrlOFF &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;
	mutable DeviceControl::OVENTempCtrlType_t m_type;
	
};



}

#endif // CMDOVENSETTEMPCTRLOFF_H
