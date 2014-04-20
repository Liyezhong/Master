/****************************************************************************/
/*! \file CmdIDSealingCheck.h
 *
 *  \brief CmdIDSealingCheck command definition.
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


#ifndef CMDIDSEALINGCHECK_H
#define CMDIDSEALINGCHECK_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdIDSealingCheck
 */
/****************************************************************************/
class CmdIDSealingCheck : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdIDSealingCheck
     *
     *  \param Timeout = int type parameter
     *  \param controller =  SchedulerMainThreadController type parameter
     *
     *  \return from CmdIDSealingCheck
     */
    /****************************************************************************/
    CmdIDSealingCheck(int Timeout, SchedulerMainThreadController *controller);
    ~CmdIDSealingCheck();

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
	 *  \brief  Definition/Declaration of function GetThresholdPressure
	 *
	 *  \return from GetThresholdPressure
	 */
	/****************************************************************************/
	qreal GetThresholdPressure(){return m_ThresholdPressure;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function SetThresholdPressure
	 *
	 *  \param ThresholdPressure = qreal type parameter
	 *
	 *  \return from SetThresholdPressure
	 */
	/****************************************************************************/
	void SetThresholdPressure(qreal ThresholdPressure){m_ThresholdPressure = ThresholdPressure;}
	
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetParameters
	 *
	 *  \return from GetParameters
	 */
	/****************************************************************************/
	QString GetParameters()const{ return GetName() + QString("(%1)").arg(m_ThresholdPressure);}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetStrResult
	 *
	 *  \return from GetStrResult
	 */
	/****************************************************************************/
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

private:
    CmdIDSealingCheck();                                                    ///< Not implemented.
    CmdIDSealingCheck(const CmdIDSealingCheck &);                     ///< Not implemented.
    const CmdIDSealingCheck & operator = (const CmdIDSealingCheck &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;       ///<  Definition/Declaration of variable m_result
	mutable qreal m_ThresholdPressure;       ///<  Definition/Declaration of variable m_ThresholdPressure
	
};



}

#endif // CMDIDSEALINGCHECK_H
