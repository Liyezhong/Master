/****************************************************************************/
/*! \file CmdIDForceDraining.h
 *
 *  \brief CmdIDForceDraining command definition.
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


#ifndef CMDIDFORCEDRAINING_H
#define CMDIDFORCEDRAINING_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdALForceDraining
 */
/****************************************************************************/
class CmdIDForceDraining : public CmdSchedulerCommandBase
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
    CmdIDForceDraining(int Timeout, const QString& sender);
    ~CmdIDForceDraining();

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
     *  \brief  Definition/Declaration of function GetRVPosition
	 *
     *  \return from GetRVPosition
	 */
	/****************************************************************************/
    quint32 GetRVPosition(){return m_RVPosition;}
	/****************************************************************************/
	/*!
     *  \brief  Definition/Declaration of function SetRVPosition
	 *
     *  \param RVPos - quint32
	 *
     *  \return from SetRVPosition
	 */
	/****************************************************************************/
    void SetRVPosition(quint32 RVPos){m_RVPosition = RVPos;}
	
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetParameters
	 *
	 *  \return from GetParameters
	 */
	/****************************************************************************/
    QString GetParameters()const{ return GetName() + QString("(%1)").arg(m_RVPosition);}
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
     *  \param tartgetPressure - float
     *
     *  \return from SetDrainPressure
     */
    /****************************************************************************/
    void SetDrainPressure(float tartgetPressure){m_TargetPressure = tartgetPressure;}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetDrainIsMoveRV
     *
     *  \return QString from GetDrainPressure
     */
    /****************************************************************************/
    QString GetReagentGrpID(){return m_ReagentGrpID;}
    /***************************l*************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetDrainIsMoveRV
     *
     *  \param ReagentGrpID - reagent group ID
     *
     *  \return from SetDrainPressure
     */
    /****************************************************************************/
    void SetReagentGrpID(const QString& ReagentGrpID){m_ReagentGrpID = ReagentGrpID;}

private:
    CmdIDForceDraining();                                                    ///< Not implemented.
    CmdIDForceDraining(const CmdIDForceDraining &);                     ///< Not implemented.
    const CmdIDForceDraining & operator = (const CmdIDForceDraining &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;       ///<  Definition/Declaration of variable m_result
    mutable quint32 m_RVPosition;       ///<  Definition/Declaration of variable m_DelayTime
    mutable float  m_TargetPressure;           ///< Defiinition/Declaration of variable m_TargetPressure
    mutable QString   m_ReagentGrpID;              ///< Defiinition/Declaration of variable m_ReagentGrpID
	
};



}

#endif // CMDIDFORCEDRAINING_H
