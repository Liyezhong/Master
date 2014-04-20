/****************************************************************************/
/*! \file CmdRVReqActRVPosition.h
 *
 *  \brief CmdRVReqActRVPosition command definition.
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


#ifndef CMDRVREQACTRVPOSITION_H
#define CMDRVREQACTRVPOSITION_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdRVReqActRVPosition
 */
/****************************************************************************/
class CmdRVReqActRVPosition : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdRVReqActRVPosition
     *
     *  \param Timeout = int type parameter
     *  \param controller =  SchedulerMainThreadController type parameter
     *
     *  \return from CmdRVReqActRVPosition
     */
    /****************************************************************************/
    CmdRVReqActRVPosition(int Timeout, SchedulerMainThreadController *controller);
    ~CmdRVReqActRVPosition();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetResult
     *
     *  \return from GetResult
     */
    /****************************************************************************/
    DeviceControl::RVPosition_t GetResult()const {return m_result;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function SetResult
	 *
	 *  \param result = DeviceControl::RVPosition_t type parameter
	 *
	 *  \return from SetResult
	 */
	/****************************************************************************/
	void SetResult(DeviceControl::RVPosition_t result) { m_result = result;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetResult
	 *
	 *  \param result = DeviceControl::RVPosition_t type parameter
	 *
	 *  \return from GetResult
	 */
	/****************************************************************************/
	bool GetResult(DeviceControl::RVPosition_t& result) const{result = m_result; return true;}
	
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetParameters
	 *
	 *  \return from GetParameters
	 */
	/****************************************************************************/
	QString GetParameters()const{ return GetName() + QString("()");}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetStrResult
	 *
	 *  \return from GetStrResult
	 */
	/****************************************************************************/
	QString GetStrResult()const{ return QString("%1").arg(m_result);}

private:
    CmdRVReqActRVPosition();                                                    ///< Not implemented.
    CmdRVReqActRVPosition(const CmdRVReqActRVPosition &);                     ///< Not implemented.
    const CmdRVReqActRVPosition & operator = (const CmdRVReqActRVPosition &); ///< Not implemented.


	mutable DeviceControl::RVPosition_t m_result;       ///<  Definition/Declaration of variable m_result
	
};



}

#endif // CMDRVREQACTRVPOSITION_H
