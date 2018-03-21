/****************************************************************************/
/*! \file CmdRVReqMoveToCurrentTubePosition.h
 *
 *  \brief CmdRVReqMoveToCurrentTubePosition command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 10.12.2015
 *   $Author:  $ Songtao Yu
 *
 *  \b Company:
 *
 *       Leica Biosystems SH CN.
 *
 *  (C) Copyright 2015 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/


#ifndef CMDRVREQMOVETOCURRENTTUBEPOSITION_H
#define CMDRVREQMOVETOCURRENTTUBEPOSITION_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{
/*lint -e1527 */
/*lint -e1411 */

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdRVReqMoveToRVPosition
 */
/****************************************************************************/
class CmdRVReqMoveToCurrentTubePosition : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdRVReqMoveToCurrentTubePosition
     *
     *  \param Timeout = int type parameter
     *  \param controller =  SchedulerMainThreadController type parameter
     *
     *  \return from CmdRVReqMoveToCurrentTubePosition 
     */
    /****************************************************************************/
    CmdRVReqMoveToCurrentTubePosition(int Timeout, const QString& sender);
    ~CmdRVReqMoveToCurrentTubePosition();

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
	DeviceControl::RVPosition_t GetRVPosition(){return m_RVPosition;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function SetRVPosition
	 *
	 *  \param RVPosition = DeviceControl::RVPosition_t type parameter
	 *
	 *  \return from SetRVPosition
	 */
	/****************************************************************************/
	void SetRVPosition(DeviceControl::RVPosition_t RVPosition){m_RVPosition = RVPosition;}
	
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetParameters
	 *
	 *  \return from GetParameters
	 */
	/****************************************************************************/
    QString GetParameters()const{ return GetName() + QString("(%1)").arg((qint32)m_RVPosition);}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetStrResult
	 *
	 *  \return from GetStrResult
	 */
	/****************************************************************************/
    QString GetStrResult()const{ return QString("%1").arg((qint32)m_result);}

private:
    CmdRVReqMoveToCurrentTubePosition();                                                    ///< Not implemented.
    CmdRVReqMoveToCurrentTubePosition(const CmdRVReqMoveToCurrentTubePosition &);                     ///< Not implemented.
    CmdRVReqMoveToCurrentTubePosition & operator = (const CmdRVReqMoveToCurrentTubePosition &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;       ///<  Definition/Declaration of variable m_result
	mutable DeviceControl::RVPosition_t m_RVPosition;       ///<  Definition/Declaration of variable m_RVPosition
	
};



}

#endif // CMDRVREQMOVETOCURRENTTUBEPOSITION_H
