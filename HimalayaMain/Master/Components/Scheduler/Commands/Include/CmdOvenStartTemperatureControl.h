/****************************************************************************/
/*! \file CmdOvenStartTemperatureControl.h
 *
 *  \brief CmdOvenStartTemperatureControl command definition.
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


#ifndef CMDOVENSTARTTEMPERATURECONTROL_H
#define CMDOVENSTARTTEMPERATURECONTROL_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

class CmdOvenStartTemperatureControl : public CmdSchedulerCommandBase
{
public:
    CmdOvenStartTemperatureControl(int Timeout, SchedulerMainThreadController *controller);
    ~CmdOvenStartTemperatureControl();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::ReturnCode_t GetResult()const {return m_result;}
	void SetResult(DeviceControl::ReturnCode_t result) { m_result = result;}
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	DeviceControl::OVENTempCtrlType_t GetType(){return m_Type;}
	void SetType(DeviceControl::OVENTempCtrlType_t Type){m_Type = Type;}
	qreal GetNominalTemperature(){return m_NominalTemperature;}
	void SetNominalTemperature(qreal NominalTemperature){m_NominalTemperature = NominalTemperature;}
	quint8 GetSlopeTempChange(){return m_SlopeTempChange;}
	void SetSlopeTempChange(quint8 SlopeTempChange){m_SlopeTempChange = SlopeTempChange;}
	
	QString GetParameters()const{ return GetName() + QString("(%1,%2,%3)").arg(m_Type).arg(m_NominalTemperature).arg(m_SlopeTempChange);}
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

private:
    CmdOvenStartTemperatureControl();                                                    ///< Not implemented.
    CmdOvenStartTemperatureControl(const CmdOvenStartTemperatureControl &);                     ///< Not implemented.
    const CmdOvenStartTemperatureControl & operator = (const CmdOvenStartTemperatureControl &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;
	mutable DeviceControl::OVENTempCtrlType_t m_Type;
	mutable qreal m_NominalTemperature;
	mutable quint8 m_SlopeTempChange;
	
};



}

#endif // CMDOVENSTARTTEMPERATURECONTROL_H
