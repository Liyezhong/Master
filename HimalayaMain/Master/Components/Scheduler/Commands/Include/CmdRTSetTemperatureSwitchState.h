/****************************************************************************/
/*! \file CmdRTSetTemperatureSwitchState.h
 *
 *  \brief CmdRTSetTemperatureSwitchState command definition.
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


#ifndef CMDRTSETTEMPERATURESWITCHSTATE_H
#define CMDRTSETTEMPERATURESWITCHSTATE_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdRTSetTemperatureSwitchState
 */
/****************************************************************************/
class CmdRTSetTemperatureSwitchState : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdRTSetTemperatureSwitchState
     *
     *  \param Timeout = int type parameter
     *  \param controller =  SchedulerMainThreadController type parameter
     *
     *  \return from CmdRTSetTemperatureSwitchState
     */
    /****************************************************************************/
    CmdRTSetTemperatureSwitchState(int Timeout, SchedulerMainThreadController *controller);
    ~CmdRTSetTemperatureSwitchState();

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
     *  \param  result - DeviceControl::ReturnCode_t
     *  \return from SetResult
     */
    /****************************************************************************/
	void SetResult(DeviceControl::ReturnCode_t result) { m_result = result;}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetResult
     *  \param result - DeviceControl::ReturnCode_t
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
     *  \param Type = DeviceControl::ALTempCtrlType_t type parameter
     *
     *  \return from SetType
     */
    /****************************************************************************/
    void SetType(DeviceControl::RTTempCtrlType_t Type){m_Type = Type;}

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
     *  \param  HeaterVoltage - qint8
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
     *  \param AutoType - qint8
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
    CmdRTSetTemperatureSwitchState();                                                    ///< Not implemented.
    CmdRTSetTemperatureSwitchState(const CmdRTSetTemperatureSwitchState &);                     ///< Not implemented.
    const CmdRTSetTemperatureSwitchState & operator = (const CmdRTSetTemperatureSwitchState &); ///< Not implemented.


    mutable DeviceControl::ReturnCode_t m_result;           ///< the return code
    mutable DeviceControl::RTTempCtrlType_t m_Type;         ///<  Definition/Declaration of variable m_Type
    mutable qint8 m_HeaterVoltage;                          ///< the heater voltage
    mutable qint8 m_AutoType;                               ///< the switch auto type
	
};



}

#endif // CMDRTSETTEMPERATURESWITCHSTATE_H
