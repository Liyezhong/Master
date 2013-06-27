/****************************************************************************/
/*! \file CmdALSetTempCtrlON.h
 *
 *  \brief CmdALSetTempCtrlON command definition.
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


#ifndef CMDALSETTEMPCTRLON_H
#define CMDALSETTEMPCTRLON_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdALSetTempCtrlON : public CmdSchedulerCommandBase
{
public:
    CmdALSetTempCtrlON(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller);
    ~CmdALSetTempCtrlON();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::ReturnCode_t GetResult()const{return m_result;}
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	DeviceControl::ALTempCtrlType_t GetType(){return m_Type;}
	void SetType(DeviceControl::ALTempCtrlType_t Type){m_Type = Type;}
	
	QString GetParameters()const{ return GetName() + QString("(%1)").arg(m_Type);}
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

    void Execute();

private:
    CmdALSetTempCtrlON();                                                    ///< Not implemented.
    CmdALSetTempCtrlON(const CmdALSetTempCtrlON &);                     ///< Not implemented.
    const CmdALSetTempCtrlON & operator = (const CmdALSetTempCtrlON &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;
	mutable DeviceControl::ALTempCtrlType_t m_Type;
	
};



}

#endif // CMDALSETTEMPCTRLON_H
