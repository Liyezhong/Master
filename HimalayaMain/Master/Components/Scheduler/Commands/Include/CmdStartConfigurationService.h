/****************************************************************************/
/*! \file CmdStartConfigurationService.h
 *
 *  \brief CmdStartConfigurationService command definition.
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


#ifndef CMDSTARTCONFIGURATIONSERVICE_H
#define CMDSTARTCONFIGURATIONSERVICE_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdStartConfigurationService : public CmdSchedulerCommandBase
{
public:
    CmdStartConfigurationService(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller);
    ~CmdStartConfigurationService();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::ReturnCode_t GetResult()const{return m_result;}
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	

    void Execute();

private:
    CmdStartConfigurationService();                                                    ///< Not implemented.
    CmdStartConfigurationService(const CmdStartConfigurationService &);                     ///< Not implemented.
    const CmdStartConfigurationService & operator = (const CmdStartConfigurationService &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;
	
};



}

#endif // CMDSTARTCONFIGURATIONSERVICE_H
