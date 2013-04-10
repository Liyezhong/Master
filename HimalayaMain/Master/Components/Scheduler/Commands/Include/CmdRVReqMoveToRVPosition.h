/****************************************************************************/
/*! \file CmdRVReqMoveToRVPosition.h
 *
 *  \brief CmdRVReqMoveToRVPosition command definition.
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


#ifndef CMDRVREQMOVETORVPOSITION_H
#define CMDRVREQMOVETORVPOSITION_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdRVReqMoveToRVPosition : public CmdSchedulerCommandBase
{
public:
    CmdRVReqMoveToRVPosition(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller);
    ~CmdRVReqMoveToRVPosition();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::ReturnCode_t GetResult()const{return m_result;}
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	DeviceControl::RVPosition_t GetRVPosition(){return m_RVPosition;}
	void SetRVPosition(DeviceControl::RVPosition_t RVPosition){m_RVPosition = RVPosition;}
	

    void Execute();

private:
    CmdRVReqMoveToRVPosition();                                                    ///< Not implemented.
    CmdRVReqMoveToRVPosition(const CmdRVReqMoveToRVPosition &);                     ///< Not implemented.
    const CmdRVReqMoveToRVPosition & operator = (const CmdRVReqMoveToRVPosition &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;
	mutable DeviceControl::RVPosition_t m_RVPosition;
	
};



}

#endif // CMDRVREQMOVETORVPOSITION_H
