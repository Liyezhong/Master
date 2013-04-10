/****************************************************************************/
/*! \file CmdALDraining.h
 *
 *  \brief CmdALDraining command definition.
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


#ifndef CMDALDRAINING_H
#define CMDALDRAINING_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdALDraining : public CmdSchedulerCommandBase
{
public:
    CmdALDraining(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller);
    ~CmdALDraining();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::ReturnCode_t GetResult()const{return m_result;}
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	quint32 GetDelayTime(){return m_DelayTime;}
	void SetDelayTime(quint32 DelayTime){m_DelayTime = DelayTime;}
	

    void Execute();

private:
    CmdALDraining();                                                    ///< Not implemented.
    CmdALDraining(const CmdALDraining &);                     ///< Not implemented.
    const CmdALDraining & operator = (const CmdALDraining &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;
	mutable quint32 m_DelayTime;
	
};



}

#endif // CMDALDRAINING_H
