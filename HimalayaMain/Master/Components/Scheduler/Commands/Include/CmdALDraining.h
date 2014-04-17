/****************************************************************************/
/*! \file CmdALDraining.h
 *
 *  \brief CmdALDraining command definition.
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


#ifndef CMDALDRAINING_H
#define CMDALDRAINING_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdALDraining
 */
/****************************************************************************/
class CmdALDraining : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdALDraining
     *
     *  \return from CmdALDraining
     */
    /****************************************************************************/
    CmdALDraining(int Timeout, SchedulerMainThreadController *controller);
    ~CmdALDraining();

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
	 *  \brief  Definition/Declaration of function GetDelayTime
	 *
	 *  \return from GetDelayTime
	 */
	/****************************************************************************/
	quint32 GetDelayTime(){return m_DelayTime;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function SetDelayTime
	 *
	 *  \return from SetDelayTime
	 */
	/****************************************************************************/
	void SetDelayTime(quint32 DelayTime){m_DelayTime = DelayTime;}
	
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetParameters
	 *
	 *  \return from GetParameters
	 */
	/****************************************************************************/
	QString GetParameters()const{ return GetName() + QString("(%1)").arg(m_DelayTime);}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetStrResult
	 *
	 *  \return from GetStrResult
	 */
	/****************************************************************************/
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

private:
    CmdALDraining();                                                    ///< Not implemented.
    CmdALDraining(const CmdALDraining &);                     ///< Not implemented.
    const CmdALDraining & operator = (const CmdALDraining &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;       ///<  Definition/Declaration of variable m_result
	mutable quint32 m_DelayTime;       ///<  Definition/Declaration of variable m_DelayTime
	
};



}

#endif // CMDALDRAINING_H
