/****************************************************************************/
/*! \file CmdOvenSetTempCtrlON.h
 *
 *  \brief CmdOvenSetTempCtrlON command definition.
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


#ifndef CMDOVENSETTEMPCTRLON_H
#define CMDOVENSETTEMPCTRLON_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdOvenSetTempCtrlON
 */
/****************************************************************************/
class CmdOvenSetTempCtrlON : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdOvenSetTempCtrlON
     *
     *  \param Timeout = int type parameter
     *  \param controller =  SchedulerMainThreadController type parameter
     *
     *  \return from CmdOvenSetTempCtrlON
     */
    /****************************************************************************/
    CmdOvenSetTempCtrlON(int Timeout, SchedulerMainThreadController *controller);
    ~CmdOvenSetTempCtrlON();

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
	DeviceControl::OVENTempCtrlType_t GetType(){return m_Type;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function SetType
	 *
	 *  \param Type = DeviceControl::OVENTempCtrlType_t type parameter
	 *
	 *  \return from SetType
	 */
	/****************************************************************************/
	void SetType(DeviceControl::OVENTempCtrlType_t Type){m_Type = Type;}
	
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetParameters
	 *
	 *  \return from GetParameters
	 */
	/****************************************************************************/
    QString GetParameters()const{ return GetName() + QString("(%1)").arg((qint32)m_Type);}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetStrResult
	 *
	 *  \return from GetStrResult
	 */
	/****************************************************************************/
    QString GetStrResult()const{ return QString("%1").arg((qint32)m_result);}

private:
    CmdOvenSetTempCtrlON();                                                    ///< Not implemented.
    CmdOvenSetTempCtrlON(const CmdOvenSetTempCtrlON &);                     ///< Not implemented.
    const CmdOvenSetTempCtrlON & operator = (const CmdOvenSetTempCtrlON &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;       ///<  Definition/Declaration of variable m_result
	mutable DeviceControl::OVENTempCtrlType_t m_Type;       ///<  Definition/Declaration of variable m_Type
	
};



}

#endif // CMDOVENSETTEMPCTRLON_H
