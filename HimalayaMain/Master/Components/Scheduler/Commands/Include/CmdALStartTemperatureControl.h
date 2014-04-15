/****************************************************************************/
/*! \file CmdALStartTemperatureControl.h
 *
 *  \brief CmdALStartTemperatureControl command definition.
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


#ifndef CMDALSTARTTEMPERATURECONTROL_H
#define CMDALSTARTTEMPERATURECONTROL_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdALStartTemperatureControl
 */
/****************************************************************************/
class CmdALStartTemperatureControl : public CmdSchedulerCommandBase
{
public:
    CmdALStartTemperatureControl(int Timeout, SchedulerMainThreadController *controller);
    ~CmdALStartTemperatureControl();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    DeviceControl::ReturnCode_t GetResult()const {return m_result;}
	void SetResult(DeviceControl::ReturnCode_t result) { m_result = result;}
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	DeviceControl::ALTempCtrlType_t GetType(){return m_Type;}
	void SetType(DeviceControl::ALTempCtrlType_t Type){m_Type = Type;}
	qreal GetNominalTemperature(){return m_NominalTemperature;}
	void SetNominalTemperature(qreal NominalTemperature){m_NominalTemperature = NominalTemperature;}
	quint8 GetSlopeTempChange(){return m_SlopeTempChange;}
	void SetSlopeTempChange(quint8 SlopeTempChange){m_SlopeTempChange = SlopeTempChange;}
	
	QString GetParameters()const{ return GetName() + QString("(%1,%2,%3)").arg(m_Type).arg(m_NominalTemperature).arg(m_SlopeTempChange);}
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

private:
    CmdALStartTemperatureControl();                                                    ///< Not implemented.
    CmdALStartTemperatureControl(const CmdALStartTemperatureControl &);                     ///< Not implemented.
    const CmdALStartTemperatureControl & operator = (const CmdALStartTemperatureControl &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;
	mutable DeviceControl::ALTempCtrlType_t m_Type;
	mutable qreal m_NominalTemperature;
	mutable quint8 m_SlopeTempChange;
	
};



}

#endif // CMDALSTARTTEMPERATURECONTROL_H
