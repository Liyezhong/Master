/****************************************************************************/
/*! \file CmdRTStartTemperatureControl.h
 *
 *  \brief CmdRTStartTemperatureControl command definition.
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


#ifndef CMDRTSTARTTEMPERATURECONTROL_H
#define CMDRTSTARTTEMPERATURECONTROL_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdRTStartTemperatureControl
 */
/****************************************************************************/
class CmdRTStartTemperatureControl : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdRTStartTemperatureControl
     *
     *  \param Timeout = int type parameter
     *  \param controller =  SchedulerMainThreadController type parameter
     *
     *  \return from CmdRTStartTemperatureControl
     */
    /****************************************************************************/
    CmdRTStartTemperatureControl(int Timeout, SchedulerMainThreadController *controller);
    ~CmdRTStartTemperatureControl();

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
	 *  \param result = DeviceControl::ReturnCode_t type parameter
	 *
	 *  \return from SetResult
	 */
	/****************************************************************************/
	void SetResult(DeviceControl::ReturnCode_t result) { m_result = result;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetResult
	 *
	 *  \param result = DeviceControl::ReturnCode_t type parameter
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
	DeviceControl::RTTempCtrlType_t GetType(){return m_Type;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function SetType
	 *
	 *  \param Type = DeviceControl::RTTempCtrlType_t type parameter
	 *
	 *  \return from SetType
	 */
	/****************************************************************************/
	void SetType(DeviceControl::RTTempCtrlType_t Type){m_Type = Type;}
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
	 *  \param NominalTemperature = qreal type parameter
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
	 *  \param SlopeTempChange = quint8 type parameter
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
    QString GetParameters()const{ return GetName() + QString("(%1,%2,%3)").arg((qint32)m_Type).arg(m_NominalTemperature).arg(m_SlopeTempChange);}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetStrResult
	 *
	 *  \return from GetStrResult
	 */
	/****************************************************************************/
    QString GetStrResult()const{ return QString("%1").arg((qint32)m_result);}

private:
    CmdRTStartTemperatureControl();                                                    ///< Not implemented.
    CmdRTStartTemperatureControl(const CmdRTStartTemperatureControl &);                     ///< Not implemented.
    const CmdRTStartTemperatureControl & operator = (const CmdRTStartTemperatureControl &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;       ///<  Definition/Declaration of variable m_result
	mutable DeviceControl::RTTempCtrlType_t m_Type;       ///<  Definition/Declaration of variable m_Type
	mutable qreal m_NominalTemperature;       ///<  Definition/Declaration of variable m_NominalTemperature
	mutable quint8 m_SlopeTempChange;       ///<  Definition/Declaration of variable m_SlopeTempChange
	
};



}

#endif // CMDRTSTARTTEMPERATURECONTROL_H
