/****************************************************************************/
/*! \file CmdRVReqMoveToInitialPosition.h
 *
 *  \brief CmdRVReqMoveToInitialPosition command definition.
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


#ifndef CMDRVREQMOVETOINITIALPOSITION_H
#define CMDRVREQMOVETOINITIALPOSITION_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdRVReqMoveToInitialPosition
 */
/****************************************************************************/
class CmdRVReqMoveToInitialPosition : public CmdSchedulerCommandBase
{
public:
    CmdRVReqMoveToInitialPosition(int Timeout, SchedulerMainThreadController *controller);
    ~CmdRVReqMoveToInitialPosition();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::ReturnCode_t GetResult()const {return m_result;}
	void SetResult(DeviceControl::ReturnCode_t result) { m_result = result;}
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	
	QString GetParameters()const{ return GetName() + QString("()");}
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

private:
    CmdRVReqMoveToInitialPosition();                                                    ///< Not implemented.
    CmdRVReqMoveToInitialPosition(const CmdRVReqMoveToInitialPosition &);                     ///< Not implemented.
    const CmdRVReqMoveToInitialPosition & operator = (const CmdRVReqMoveToInitialPosition &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;
	
};



}

#endif // CMDRVREQMOVETOINITIALPOSITION_H
