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
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdALStartTemperatureControl
     *
     *  \return from CmdALStartTemperatureControl
     */
    /****************************************************************************/
    CmdALStartTemperatureControl(int Timeout, SchedulerMainThreadController *controller);
    ~CmdALStartTemperatureControl();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetResult
     *
     *  \return from GetResult
     */
    /****************************************************************************/
    DeviceControl::ReturnCode_t GetResult()const {return m_result;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function SetResult
	 *
	 *  \return from SetResult
	 */
	/****************************************************************************/
	void SetResult(DeviceControl::ReturnCode_t result) { m_result = result;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetResult
	 *
	 *  \return from GetResult
	 */
	/****************************************************************************/
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetType
	 *
	 *  \return from GetType
	 */
	/****************************************************************************/
	DeviceControl::ALTempCtrlType_t GetType(){return m_Type;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function SetType
	 *
	 *  \return from SetType
	 */
	/****************************************************************************/
	void SetType(DeviceControl::ALTempCtrlType_t Type){m_Type = Type;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetNominalTemperature
	 *
	 *  \return from GetNominalTemperature
	 */
	/****************************************************************************/
	qreal GetNominalTemperature(){return m_NominalTemperature;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function SetNominalTemperature
	 *
	 *  \return from SetNominalTemperature
	 */
	/****************************************************************************/
	void SetNominalTemperature(qreal NominalTemperature){m_NominalTemperature = NominalTemperature;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetSlopeTempChange
	 *
	 *  \return from GetSlopeTempChange
	 */
	/****************************************************************************/
	quint8 GetSlopeTempChange(){return m_SlopeTempChange;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function SetSlopeTempChange
	 *
	 *  \return from SetSlopeTempChange
	 */
	/****************************************************************************/
	void SetSlopeTempChange(quint8 SlopeTempChange){m_SlopeTempChange = SlopeTempChange;}
	
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetParameters
	 *
	 *  \return from GetParameters
	 */
	/****************************************************************************/
	QString GetParameters()const{ return GetName() + QString("(%1,%2,%3)").arg(m_Type).arg(m_NominalTemperature).arg(m_SlopeTempChange);}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetStrResult
	 *
	 *  \return from GetStrResult
	 */
	/****************************************************************************/
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

private:
    CmdALStartTemperatureControl();                                                    ///< Not implemented.
    CmdALStartTemperatureControl(const CmdALStartTemperatureControl &);                     ///< Not implemented.
    const CmdALStartTemperatureControl & operator = (const CmdALStartTemperatureControl &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;       ///<  Definition/Declaration of variable m_result
	mutable DeviceControl::ALTempCtrlType_t m_Type;       ///<  Definition/Declaration of variable m_Type
	mutable qreal m_NominalTemperature;       ///<  Definition/Declaration of variable m_NominalTemperature
	mutable quint8 m_SlopeTempChange;       ///<  Definition/Declaration of variable m_SlopeTempChange
	
};



}

#endif // CMDALSTARTTEMPERATURECONTROL_H
