/****************************************************************************/
/*! \file CmdALControlValve.h
 *
 *  \brief CmdALControlValve command definition.
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


#ifndef CMDCMDALCONTROLVALVE_H
#define CMDCMDALCONTROLVALVE_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdALControlValve
 */
/****************************************************************************/
class CmdALControlValve : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdALControlValve
     *
     *  \param Timeout = int type parameter
     *  \param controller =  SchedulerMainThreadController type parameter
     *
     *  \return from CmdALControlValve
     */
    /****************************************************************************/
    CmdALControlValve(int Timeout, SchedulerMainThreadController *controller);
    ~CmdALControlValve();

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
     *  \brief  Definition/Declaration of function GetResult
     *
     *  \param  result - DeviceControl::ReturnCode_t result
     *
     *  \return from GetResult
     */
    /****************************************************************************/
	void SetResult(DeviceControl::ReturnCode_t result) { m_result = result;}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetResult
     *
     *  \param result - DeviceControl::ReturnCode_t
     *
     *  \return from GetResult
     */
    /****************************************************************************/
	bool GetResult(DeviceControl::ReturnCode_t& result) const{result = m_result; return true;}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetResult
     *
     *  \return from GetResult
     */
    /****************************************************************************/
	quint8 GetValveIndex(){return m_ValveIndex;}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetResult
     *
     *  \param ValveIndex - quint8
     *
     *  \return from GetResult
     */
    /****************************************************************************/
	void SetValveIndex(quint8 ValveIndex){m_ValveIndex = ValveIndex;}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetResult
     *
     *  \return from GetResult
     */
    /****************************************************************************/
	quint8 GetValveState(){return m_ValveState;}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetResult
     *
     *  \param  ValveState - quint8
     *
     *  \return from GetResult
     */
    /****************************************************************************/
	void SetValveState(quint8 ValveState){m_ValveState = ValveState;}
	
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetResult
     *
     *  \return from GetResult
     */
    /****************************************************************************/
	QString GetParameters()const{ return GetName() + QString("(%1,%2)").arg(m_ValveIndex).arg(m_ValveState);}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetResult
     *
     *  \return from GetResult
     */
    /****************************************************************************/
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

private:
    CmdALControlValve();                                              ///< Not implemented.
    CmdALControlValve(const CmdALControlValve &);                     ///< Not implemented.
    const CmdALControlValve & operator = (const CmdALControlValve &); ///< Not implemented.


    mutable DeviceControl::ReturnCode_t m_result;                   ///<  Definition/Declaration of variable m_result
    mutable quint8 m_ValveIndex;                                    ///<  Definition/Declaration of variable m_ValveIndex
    mutable quint8 m_ValveState;                                    ///<  Definition/Declaration of variable m_ValveState
	
};



}

#endif // CMDCMDALCONTROLVALVE_H
