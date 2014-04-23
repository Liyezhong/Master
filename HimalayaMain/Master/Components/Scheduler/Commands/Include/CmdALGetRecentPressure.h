/****************************************************************************/
/*! \file CmdALGetRecentPressure.h
 *
 *  \brief CmdALGetRecentPressure command definition.
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


#ifndef CMDALGETRECENTPRESSURE_H
#define CMDALGETRECENTPRESSURE_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdALGetRecentPressure
 */
/****************************************************************************/
class CmdALGetRecentPressure : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdALGetRecentPressure
     *
     *  \param Timeout = int type parameter
     *  \param controller =  SchedulerMainThreadController type parameter
     *
     *  \return from CmdALGetRecentPressure
     */
    /****************************************************************************/
    CmdALGetRecentPressure(int Timeout, SchedulerMainThreadController *controller);
    ~CmdALGetRecentPressure();

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
	QString GetParameters()const{ return GetName() + QString("(%1)").arg(m_Index);}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetStrResult
	 *
	 *  \return from GetStrResult
	 */
	/****************************************************************************/
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

private:
    CmdALGetRecentPressure();                                                    ///< Not implemented.
    CmdALGetRecentPressure(const CmdALGetRecentPressure &);                     ///< Not implemented.
    const CmdALGetRecentPressure & operator = (const CmdALGetRecentPressure &); ///< Not implemented.


	mutable qreal m_result;       ///<  Definition/Declaration of variable m_result
	mutable quint8 m_Index;       ///<  Definition/Declaration of variable m_Index
	
};



}

#endif // CMDALGETRECENTPRESSURE_H
