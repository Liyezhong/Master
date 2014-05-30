/****************************************************************************/
/*! \file CmdRVGetRecentTemperature.h
 *
 *  \brief CmdRVGetRecentTemperature command definition.
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


#ifndef CMDRVGETRECENTTEMPERATURE_H
#define CMDRVGETRECENTTEMPERATURE_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdRVGetRecentTemperature
 */
/****************************************************************************/
class CmdRVGetRecentTemperature : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdRVGetRecentTemperature
     *
     *  \param Timeout = int type parameter
     *  \param controller =  SchedulerMainThreadController type parameter
     *
     *  \return from CmdRVGetRecentTemperature
     */
    /****************************************************************************/
    CmdRVGetRecentTemperature(int Timeout, SchedulerMainThreadController *controller);
    ~CmdRVGetRecentTemperature();

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
	 *  \brief  Definition/Declaration of function GetIndex
	 *
	 *  \return from GetIndex
	 */
	/****************************************************************************/
	quint32 GetIndex(){return m_Index;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function SetIndex
	 *
	 *  \param Index = quint32 type parameter
	 *
	 *  \return from SetIndex
	 */
	/****************************************************************************/
	void SetIndex(quint32 Index){m_Index = Index;}
	
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetParameters
	 *
	 *  \return from GetParameters
	 */
	/****************************************************************************/
	QString GetParameters()const{ return GetName() + QString("(%1)").arg(m_Index);}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetStrResult
	 *
	 *  \return from GetStrResult
	 */
	/****************************************************************************/
    QString GetStrResult()const{ return QString("%1").arg((qint32)m_result);}

private:
    CmdRVGetRecentTemperature();                                                    ///< Not implemented.
    CmdRVGetRecentTemperature(const CmdRVGetRecentTemperature &);                     ///< Not implemented.
    const CmdRVGetRecentTemperature & operator = (const CmdRVGetRecentTemperature &); ///< Not implemented.


	mutable qreal m_result;       ///<  Definition/Declaration of variable m_result
	mutable quint32 m_Index;       ///<  Definition/Declaration of variable m_Index
	
};



}

#endif // CMDRVGETRECENTTEMPERATURE_H
