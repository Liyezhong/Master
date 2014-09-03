/****************************************************************************/
/*! \file CmdRVSetTemperatureSwitchState.h
 *
 *  \brief CmdRVSetTemperatureSwitchState command definition.
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


#ifndef CMDRVSETTEMPERATURESWITCHSTATE_H
#define CMDRVSETTEMPERATURESWITCHSTATE_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdRVSetTemperatureSwitchState
 */
/****************************************************************************/
class CmdRVSetTemperatureSwitchState : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdRVSetTemperatureSwitchState
     *
     *  \param Timeout = int type parameter
     *  \param controller =  SchedulerMainThreadController type parameter
     *
     *  \return from CmdRVSetTemperatureSwitchState
     */
    /****************************************************************************/
    CmdRVSetTemperatureSwitchState(int Timeout, SchedulerMainThreadController *controller);
    ~CmdRVSetTemperatureSwitchState();

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
     *  \brief  Definition/Declaration of function GetHeaterVoltage
     *
     *  \return from GetHeaterVoltage
     */
    /****************************************************************************/
    qint8 GetHeaterVoltage(){return m_HeaterVoltage;}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetHeaterVoltage
     *
     *  \return from SetHeaterVoltage
     */
    /****************************************************************************/
    void SetHeaterVoltage(qint8 HeaterVoltage){m_HeaterVoltage = HeaterVoltage;}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetAutoType
     *
     *  \return from GetAutoType
     */
    /****************************************************************************/
    qint8 GetAutoType(){return m_AutoType;}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetAutoType
     *
     *  \return from SetAutoType
     */
    /****************************************************************************/
    void SetAutoType(qint8 AutoType){m_AutoType = AutoType;}
	
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetParameters
     *
     *  \return from GetParameters
     */
    /****************************************************************************/
	QString GetParameters()const{ return GetName() + QString("(%1,%2)").arg(m_HeaterVoltage).arg(m_AutoType);}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetStrResult
     *
     *  \return from GetStrResult
     */
    /****************************************************************************/
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

private:
    CmdRVSetTemperatureSwitchState();                                                    ///< Not implemented.
    CmdRVSetTemperatureSwitchState(const CmdRVSetTemperatureSwitchState &);                     ///< Not implemented.
    const CmdRVSetTemperatureSwitchState & operator = (const CmdRVSetTemperatureSwitchState &); ///< Not implemented.


    mutable DeviceControl::ReturnCode_t m_result;                                       ///< the retrun code
    mutable qint8 m_HeaterVoltage;                                                      ///< the heater voltage
    mutable qint8 m_AutoType;                                                           ///< the switch type
	
};



}

#endif // CMDRVSETTEMPERATURESWITCHSTATE_H
