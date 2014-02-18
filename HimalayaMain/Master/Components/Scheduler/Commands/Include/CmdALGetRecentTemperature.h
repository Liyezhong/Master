/****************************************************************************/
/*! \file CmdALGetRecentTemperature.h
 *
 *  \brief CmdALGetRecentTemperature command definition.
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


#ifndef CMDALGETRECENTTEMPERATURE_H
#define CMDALGETRECENTTEMPERATURE_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdALGetRecentTemperature : public CmdSchedulerCommandBase
{
public:
    CmdALGetRecentTemperature(int Timeout, SchedulerMainThreadController *controller);
    ~CmdALGetRecentTemperature();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    qreal GetResult()const {return m_result;}
	void SetResult(qreal result) { m_result = result;}
	bool GetResult(qreal& result) const{result = m_result; return true;}
	DeviceControl::ALTempCtrlType_t GetType(){return m_Type;}
	void SetType(DeviceControl::ALTempCtrlType_t Type){m_Type = Type;}
	quint8 GetIndex(){return m_Index;}
	void SetIndex(quint8 Index){m_Index = Index;}
	
	QString GetParameters()const{ return GetName() + QString("(%1,%2)").arg(m_Type).arg(m_Index);}
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

private:
    CmdALGetRecentTemperature();                                                    ///< Not implemented.
    CmdALGetRecentTemperature(const CmdALGetRecentTemperature &);                     ///< Not implemented.
    const CmdALGetRecentTemperature & operator = (const CmdALGetRecentTemperature &); ///< Not implemented.


	mutable qreal m_result;
	mutable DeviceControl::ALTempCtrlType_t m_Type;
	mutable quint8 m_Index;
	
};



}

#endif // CMDALGETRECENTTEMPERATURE_H
