/****************************************************************************/
/*! \file CmdRVGetRecentTemperature.h
 *
 *  \brief CmdRVGetRecentTemperature command definition.
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


#ifndef CMDRVGETRECENTTEMPERATURE_H
#define CMDRVGETRECENTTEMPERATURE_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdRVGetRecentTemperature : public CmdSchedulerCommandBase
{
public:
    CmdRVGetRecentTemperature(int Timeout, SchedulerMainThreadController *controller);
    ~CmdRVGetRecentTemperature();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    qreal GetResult()const {return m_result;}
	void SetResult(qreal result) { m_result = result;}
	bool GetResult(qreal& result) const{result = m_result; return true;}
	quint32 GetIndex(){return m_Index;}
	void SetIndex(quint32 Index){m_Index = Index;}
	
	QString GetParameters()const{ return GetName() + QString("(%1)").arg(m_Index);}
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

private:
    CmdRVGetRecentTemperature();                                                    ///< Not implemented.
    CmdRVGetRecentTemperature(const CmdRVGetRecentTemperature &);                     ///< Not implemented.
    const CmdRVGetRecentTemperature & operator = (const CmdRVGetRecentTemperature &); ///< Not implemented.


	mutable qreal m_result;
	mutable quint32 m_Index;
	
};



}

#endif // CMDRVGETRECENTTEMPERATURE_H
