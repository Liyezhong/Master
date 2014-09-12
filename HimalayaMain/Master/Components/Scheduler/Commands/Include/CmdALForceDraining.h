/****************************************************************************/
/*! \file CmdALForceDraining.h
 *
 *  \brief CmdALForceDraining command definition.
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


#ifndef CMDALFORCEDRAINING_H
#define CMDALFORCEDRAINING_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdALForceDraining
 */
/****************************************************************************/
class CmdALForceDraining : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdALForceDraining
     *
     *  \param Timeout = int type parameter
     *  \param controller =  SchedulerMainThreadController type parameter
     *
     *  \return from CmdALForceDraining
     */
    /****************************************************************************/
    CmdALForceDraining(int Timeout, SchedulerMainThreadController *controller);
    ~CmdALForceDraining();

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
	 *  \param DelayTime = quint32 type parameter
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
    QString GetStrResult()const{ return QString("%1").arg((qint32)m_result);}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetDelayTime
     *
     *  \return from GetDrainPressure
     */
    /****************************************************************************/
    float GetDrainPressure(){return m_TargetPressure;}
    /***************************l*************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetDelayTime
     *
     *  \param DelayTime = quint32 type parameter
     *
     *  \return from SetDrainPressure
     */
    /****************************************************************************/
    void SetDrainPressure(float tartgetPressure){m_TargetPressure = tartgetPressure;}

private:
    CmdALForceDraining();                                                    ///< Not implemented.
    CmdALForceDraining(const CmdALForceDraining &);                     ///< Not implemented.
    const CmdALForceDraining & operator = (const CmdALForceDraining &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;       ///<  Definition/Declaration of variable m_result
	mutable quint32 m_DelayTime;       ///<  Definition/Declaration of variable m_DelayTime
    mutable float  m_TargetPressure;           ///< Defiinition/Declaration of variable m_TargetPressure
	
};



}

#endif // CMDALFORCEDRAINING_H
