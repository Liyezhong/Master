/****************************************************************************/
/*! \file CmdALGetRecentPressure.h
 *
 *  \brief CmdALGetRecentPressure command definition.
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


#ifndef CMDALGETRECENTPRESSURE_H
#define CMDALGETRECENTPRESSURE_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdALGetRecentPressure : public CmdSchedulerCommandBase
{
public:
    CmdALGetRecentPressure(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller);
    ~CmdALGetRecentPressure();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    qreal GetResult()const{return m_result;}
	bool GetResult(qreal& result) const{result = m_result; return true;}
	quint8 GetIndex(){return m_Index;}
	void SetIndex(quint8 Index){m_Index = Index;}
	

    void Execute();

private:
    CmdALGetRecentPressure();                                                    ///< Not implemented.
    CmdALGetRecentPressure(const CmdALGetRecentPressure &);                     ///< Not implemented.
    const CmdALGetRecentPressure & operator = (const CmdALGetRecentPressure &); ///< Not implemented.


	mutable qreal m_result;
	mutable quint8 m_Index;
	
};



}

#endif // CMDALGETRECENTPRESSURE_H
