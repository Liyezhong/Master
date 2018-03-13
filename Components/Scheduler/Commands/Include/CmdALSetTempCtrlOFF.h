/****************************************************************************/
/*! \file CmdALSetTempCtrlOFF.h
 *
 *  \brief CmdALSetTempCtrlOFF command definition.
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


#ifndef CMDALSETTEMPCTRLOFF_H
#define CMDALSETTEMPCTRLOFF_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{
/*lint -e1411 */
/*lint -e1527 */

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdALSetTempCtrlOFF
 */
/****************************************************************************/
class CmdALSetTempCtrlOFF : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdALSetTempCtrlOFF
     *
     *  \param Timeout = int type parameter
     *  \param controller =  SchedulerMainThreadController type parameter
     *
     *  \return from CmdALSetTempCtrlOFF
     */
    /****************************************************************************/
    CmdALSetTempCtrlOFF(int Timeout, SchedulerMainThreadController *controller);
    ~CmdALSetTempCtrlOFF();

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
	 *  \brief  Definition/Declaration of function Gettype
	 *
	 *  \return from Gettype
	 */
	/****************************************************************************/
	DeviceControl::ALTempCtrlType_t Gettype(){return m_type;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function Settype
	 *
	 *  \param type = DeviceControl::ALTempCtrlType_t type parameter
	 *
	 *  \return from Settype
	 */
	/****************************************************************************/
	void Settype(DeviceControl::ALTempCtrlType_t type){m_type = type;}
	
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetParameters
	 *
	 *  \return from GetParameters
	 */
	/****************************************************************************/
    QString GetParameters()const{ return GetName() + QString("(%1)").arg((qint32)m_type);}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetStrResult
	 *
	 *  \return from GetStrResult
	 */
	/****************************************************************************/
    QString GetStrResult()const{ return QString("%1").arg((qint32)m_result);}

private:
    CmdALSetTempCtrlOFF();                                                    ///< Not implemented.
    CmdALSetTempCtrlOFF(const CmdALSetTempCtrlOFF &);                     ///< Not implemented.
    const CmdALSetTempCtrlOFF & operator = (const CmdALSetTempCtrlOFF &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;       ///<  Definition/Declaration of variable m_result
	mutable DeviceControl::ALTempCtrlType_t m_type;       ///<  Definition/Declaration of variable m_type
	
};



}

#endif // CMDALSETTEMPCTRLOFF_H
