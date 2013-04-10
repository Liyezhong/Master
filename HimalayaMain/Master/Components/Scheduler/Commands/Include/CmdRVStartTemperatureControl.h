/****************************************************************************/
/*! \file CmdRVStartTemperatureControl.h
 *
 *  \brief CmdRVStartTemperatureControl command definition.
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


#ifndef CMDRVSTARTTEMPERATURECONTROL_H
#define CMDRVSTARTTEMPERATURECONTROL_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdRVStartTemperatureControl : public CmdSchedulerCommandBase
{
public:
    CmdRVStartTemperatureControl(int Timeout, DeviceControl::IDeviceProcessing *IDP, SchedulerMainThreadController *controller);
    ~CmdRVStartTemperatureControl();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::ReturnCode_t GetResult()const{return m_result;}
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	qreal GetNominalTemperature(){return m_NominalTemperature;}
	void SetNominalTemperature(qreal NominalTemperature){m_NominalTemperature = NominalTemperature;}
	quint8 GetSlopeTempChange(){return m_SlopeTempChange;}
	void SetSlopeTempChange(quint8 SlopeTempChange){m_SlopeTempChange = SlopeTempChange;}
	

    void Execute();

private:
    CmdRVStartTemperatureControl();                                                    ///< Not implemented.
    CmdRVStartTemperatureControl(const CmdRVStartTemperatureControl &);                     ///< Not implemented.
    const CmdRVStartTemperatureControl & operator = (const CmdRVStartTemperatureControl &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;
	mutable qreal m_NominalTemperature;
	mutable quint8 m_SlopeTempChange;
	
};



}

#endif // CMDRVSTARTTEMPERATURECONTROL_H
