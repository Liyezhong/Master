/****************************************************************************/
/*! \file CmdStartDiagnosticService.h
 *
 *  \brief CmdStartDiagnosticService command definition.
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


#ifndef CMDSTARTDIAGNOSTICSERVICE_H
#define CMDSTARTDIAGNOSTICSERVICE_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdStartDiagnosticService : public CmdSchedulerCommandBase
{
public:
    CmdStartDiagnosticService(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller);
    ~CmdStartDiagnosticService();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::ReturnCode_t GetResult()const{return m_result;}
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	
	QString GetParameters()const{ return GetName() + QString("()");}
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

    void Execute();

private:
    CmdStartDiagnosticService();                                                    ///< Not implemented.
    CmdStartDiagnosticService(const CmdStartDiagnosticService &);                     ///< Not implemented.
    const CmdStartDiagnosticService & operator = (const CmdStartDiagnosticService &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;
	
};



}

#endif // CMDSTARTDIAGNOSTICSERVICE_H
