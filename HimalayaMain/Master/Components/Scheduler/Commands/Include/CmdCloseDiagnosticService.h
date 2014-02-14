/****************************************************************************/
/*! \file CmdCloseDiagnosticService.h
 *
 *  \brief CmdCloseDiagnosticService command definition.
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


#ifndef CMDCLOSEDIAGNOSTICSERVICE_H
#define CMDCLOSEDIAGNOSTICSERVICE_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdCloseDiagnosticService : public CmdSchedulerCommandBase
{
public:
    CmdCloseDiagnosticService(int Timeout, SchedulerMainThreadController *controller);
    ~CmdCloseDiagnosticService();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::ReturnCode_t GetResult()const {return m_result;}
	void SetResult(DeviceControl::ReturnCode_t result) { m_result = result;}
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	
	QString GetParameters()const{ return GetName() + QString("()");}
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

private:
    CmdCloseDiagnosticService();                                                    ///< Not implemented.
    CmdCloseDiagnosticService(const CmdCloseDiagnosticService &);                     ///< Not implemented.
    const CmdCloseDiagnosticService & operator = (const CmdCloseDiagnosticService &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;
	
};



}

#endif // CMDCLOSEDIAGNOSTICSERVICE_H
