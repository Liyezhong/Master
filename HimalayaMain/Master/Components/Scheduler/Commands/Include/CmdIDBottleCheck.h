/****************************************************************************/
/*! \file CmdIDBottleCheck.h
 *
 *  \brief CmdIDBottleCheck command definition.
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


#ifndef CMDIDBOTTLECHECK_H
#define CMDIDBOTTLECHECK_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdIDBottleCheck : public CmdSchedulerCommandBase
{
public:
    CmdIDBottleCheck(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller);
    ~CmdIDBottleCheck();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::ReturnCode_t GetResult()const{return m_result;}
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	QString GetReagentGrpID(){return m_ReagentGrpID;}
	void SetReagentGrpID(QString ReagentGrpID){m_ReagentGrpID = ReagentGrpID;}
	DeviceControl::RVPosition_t GetTubePos(){return m_TubePos;}
	void SetTubePos(DeviceControl::RVPosition_t TubePos){m_TubePos = TubePos;}
	

    void Execute();

private:
    CmdIDBottleCheck();                                                    ///< Not implemented.
    CmdIDBottleCheck(const CmdIDBottleCheck &);                     ///< Not implemented.
    const CmdIDBottleCheck & operator = (const CmdIDBottleCheck &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;
	mutable QString m_ReagentGrpID;
	mutable DeviceControl::RVPosition_t m_TubePos;
	
};



}

#endif // CMDIDBOTTLECHECK_H
