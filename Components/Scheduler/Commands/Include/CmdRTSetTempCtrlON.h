/****************************************************************************/
/*! \file CmdRTSetTempCtrlON.h
 *
 *  \brief CmdRTSetTempCtrlON command definition.
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


#ifndef CMDRTSETTEMPCTRLON_H
#define CMDRTSETTEMPCTRLON_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{
/*lint -e1411 */
/*lint -e1527 */

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdRTSetTempCtrlON
 */
/****************************************************************************/
class CmdRTSetTempCtrlON : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdRTSetTempCtrlON
     *
     *  \param Timeout = int type parameter
     *  \param controller =  SchedulerMainThreadController type parameter
     *
     *  \return from CmdRTSetTempCtrlON
     */
    /****************************************************************************/
    CmdRTSetTempCtrlON(int Timeout, SchedulerMainThreadController *controller);
    ~CmdRTSetTempCtrlON();

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
    CmdRTSetTempCtrlON();                                                    ///< Not implemented.
    CmdRTSetTempCtrlON(const CmdRTSetTempCtrlON &);                     ///< Not implemented.
    const CmdRTSetTempCtrlON & operator = (const CmdRTSetTempCtrlON &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;       ///<  Definition/Declaration of variable m_result
	mutable DeviceControl::RTTempCtrlType_t m_Type;       ///<  Definition/Declaration of variable m_Type
	
};



}

#endif // CMDRTSETTEMPCTRLON_H
