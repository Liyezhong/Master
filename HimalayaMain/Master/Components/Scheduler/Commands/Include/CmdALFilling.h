/****************************************************************************/
/*! \file CmdALFilling.h
 *
 *  \brief CmdALFilling command definition.
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


#ifndef CMDALFILLING_H
#define CMDALFILLING_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdALFilling : public CmdSchedulerCommandBase
{
public:
    CmdALFilling(int Timeout, SchedulerMainThreadController *controller);
    ~CmdALFilling();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::ReturnCode_t GetResult()const {return m_result;}
	void SetResult(DeviceControl::ReturnCode_t result) { m_result = result;}
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	quint32 GetDelayTime(){return m_DelayTime;}
	void SetDelayTime(quint32 DelayTime){m_DelayTime = DelayTime;}
	
	QString GetParameters()const{ return GetName() + QString("(%1)").arg(m_DelayTime);}
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

private:
    CmdALFilling();                                                    ///< Not implemented.
    CmdALFilling(const CmdALFilling &);                     ///< Not implemented.
    const CmdALFilling & operator = (const CmdALFilling &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;
	mutable quint32 m_DelayTime;
	
};



}

#endif // CMDALFILLING_H
