/****************************************************************************/
/*! \file CmdRVReqActRVPosition.h
 *
 *  \brief CmdRVReqActRVPosition command definition.
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


#ifndef CMDRVREQACTRVPOSITION_H
#define CMDRVREQACTRVPOSITION_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdRVReqActRVPosition : public CmdSchedulerCommandBase
{
public:
    CmdRVReqActRVPosition(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller);
    ~CmdRVReqActRVPosition();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::RVPosition_t GetResult()const{return m_result;}
	bool GetResult(DeviceControl::RVPosition_t& result) const{result = m_result; return true;}
	
	QString GetParameters()const{ return GetName() + QString("()");}
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

    void Execute();

private:
    CmdRVReqActRVPosition();                                                    ///< Not implemented.
    CmdRVReqActRVPosition(const CmdRVReqActRVPosition &);                     ///< Not implemented.
    const CmdRVReqActRVPosition & operator = (const CmdRVReqActRVPosition &); ///< Not implemented.


	mutable DeviceControl::RVPosition_t m_result;
	
};



}

#endif // CMDRVREQACTRVPOSITION_H
