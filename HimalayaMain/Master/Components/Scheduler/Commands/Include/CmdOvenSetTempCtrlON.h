/****************************************************************************/
/*! \file CmdOvenSetTempCtrlON.h
 *
 *  \brief CmdOvenSetTempCtrlON command definition.
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


#ifndef CMDOVENSETTEMPCTRLON_H
#define CMDOVENSETTEMPCTRLON_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdOvenSetTempCtrlON : public CmdSchedulerCommandBase
{
public:
    CmdOvenSetTempCtrlON(int Timeout, SchedulerMainThreadController *controller);
    ~CmdOvenSetTempCtrlON();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::ReturnCode_t GetResult()const {return m_result;}
	void SetResult(DeviceControl::ReturnCode_t result) { m_result = result;}
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	DeviceControl::OVENTempCtrlType_t GetType(){return m_Type;}
	void SetType(DeviceControl::OVENTempCtrlType_t Type){m_Type = Type;}
	
	QString GetParameters()const{ return GetName() + QString("(%1)").arg(m_Type);}
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

private:
    CmdOvenSetTempCtrlON();                                                    ///< Not implemented.
    CmdOvenSetTempCtrlON(const CmdOvenSetTempCtrlON &);                     ///< Not implemented.
    const CmdOvenSetTempCtrlON & operator = (const CmdOvenSetTempCtrlON &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;
	mutable DeviceControl::OVENTempCtrlType_t m_Type;
	
};



}

#endif // CMDOVENSETTEMPCTRLON_H
