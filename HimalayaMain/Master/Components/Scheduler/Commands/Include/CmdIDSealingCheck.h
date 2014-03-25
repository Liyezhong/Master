/****************************************************************************/
/*! \file CmdIDSealingCheck.h
 *
 *  \brief CmdIDSealingCheck command definition.
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


#ifndef CMDIDSEALINGCHECK_H
#define CMDIDSEALINGCHECK_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdIDSealingCheck : public CmdSchedulerCommandBase
{
public:
    CmdIDSealingCheck(int Timeout, SchedulerMainThreadController *controller);
    ~CmdIDSealingCheck();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::ReturnCode_t GetResult()const {return m_result;}
	void SetResult(DeviceControl::ReturnCode_t result) { m_result = result;}
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	qreal GetThresholdPressure(){return m_ThresholdPressure;}
	void SetThresholdPressure(qreal ThresholdPressure){m_ThresholdPressure = ThresholdPressure;}
	
	QString GetParameters()const{ return GetName() + QString("(%1)").arg(m_ThresholdPressure);}
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

private:
    CmdIDSealingCheck();                                                    ///< Not implemented.
    CmdIDSealingCheck(const CmdIDSealingCheck &);                     ///< Not implemented.
    const CmdIDSealingCheck & operator = (const CmdIDSealingCheck &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;
	mutable qreal m_ThresholdPressure;
	
};



}

#endif // CMDIDSEALINGCHECK_H
