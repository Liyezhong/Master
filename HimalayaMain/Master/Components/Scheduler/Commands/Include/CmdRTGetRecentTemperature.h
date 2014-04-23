/****************************************************************************/
/*! \file CmdRTGetRecentTemperature.h
 *
 *  \brief CmdRTGetRecentTemperature command definition.
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


#ifndef CMDRTGETRECENTTEMPERATURE_H
#define CMDRTGETRECENTTEMPERATURE_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdRTGetRecentTemperature
 */
/****************************************************************************/
class CmdRTGetRecentTemperature : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdRTGetRecentTemperature
     *
     *  \param Timeout = int type parameter
     *  \param controller =  SchedulerMainThreadController type parameter
     *
     *  \return from CmdRTGetRecentTemperature
     */
    /****************************************************************************/
    CmdRTGetRecentTemperature(int Timeout, SchedulerMainThreadController *controller);
    ~CmdRTGetRecentTemperature();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetResult
     *
     *  \return from GetResult
     */
    /****************************************************************************/
    qreal GetResult()const {return m_result;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function SetResult
	 *
	 *  \param result = qreal type parameter
	 *
	 *  \return from SetResult
	 */
	/****************************************************************************/
	void SetResult(qreal result) { m_result = result;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetResult
	 *
	 *  \param result = qreal type parameter
	 *
	 *  \return from GetResult
	 */
	/****************************************************************************/
	bool GetResult(qreal& result) const{result = m_result; return true;}
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
	 *  \brief  Definition/Declaration of function GetIndex
	 *
	 *  \return from GetIndex
	 */
	/****************************************************************************/
	quint8 GetIndex(){return m_Index;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function SetIndex
	 *
	 *  \param Index = quint8 type parameter
	 *
	 *  \return from SetIndex
	 */
	/****************************************************************************/
	void SetIndex(quint8 Index){m_Index = Index;}
	
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetParameters
	 *
	 *  \return from GetParameters
	 */
	/****************************************************************************/
	QString GetParameters()const{ return GetName() + QString("(%1,%2)").arg(m_Type).arg(m_Index);}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetStrResult
	 *
	 *  \return from GetStrResult
	 */
	/****************************************************************************/
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

private:
    CmdRTGetRecentTemperature();                                                    ///< Not implemented.
    CmdRTGetRecentTemperature(const CmdRTGetRecentTemperature &);                     ///< Not implemented.
    const CmdRTGetRecentTemperature & operator = (const CmdRTGetRecentTemperature &); ///< Not implemented.


	mutable qreal m_result;       ///<  Definition/Declaration of variable m_result
	mutable DeviceControl::RTTempCtrlType_t m_Type;       ///<  Definition/Declaration of variable m_Type
	mutable quint8 m_Index;       ///<  Definition/Declaration of variable m_Index
	
};



}

#endif // CMDRTGETRECENTTEMPERATURE_H
