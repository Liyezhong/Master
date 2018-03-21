/****************************************************************************/
/*! \file CmdIDBottleCheck.h
 *
 *  \brief CmdIDBottleCheck command definition.
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


#ifndef CMDIDBOTTLECHECK_H
#define CMDIDBOTTLECHECK_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdIDBottleCheck
 */
/****************************************************************************/
class CmdIDBottleCheck : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdIDBottleCheck
     *
     *  \param Timeout = int type parameter
     *  \param controller =  SchedulerMainThreadController type parameter
     *
     *  \return from CmdIDBottleCheck
     */
    /****************************************************************************/
    CmdIDBottleCheck(int Timeout, const QString& sender);
    ~CmdIDBottleCheck();

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
	 *  \brief  Definition/Declaration of function GetReagentGrpID
	 *
	 *  \return from GetReagentGrpID
	 */
	/****************************************************************************/
	QString GetReagentGrpID(){return m_ReagentGrpID;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function SetReagentGrpID
	 *
	 *  \param ReagentGrpID = QString type parameter
	 *
	 *  \return from SetReagentGrpID
	 */
	/****************************************************************************/
	void SetReagentGrpID(QString ReagentGrpID){m_ReagentGrpID = ReagentGrpID;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetTubePos
	 *
	 *  \return from GetTubePos
	 */
	/****************************************************************************/
	DeviceControl::RVPosition_t GetTubePos(){return m_TubePos;}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function SetTubePos
	 *
	 *  \param TubePos = DeviceControl::RVPosition_t type parameter
	 *
	 *  \return from SetTubePos
	 */
	/****************************************************************************/
	void SetTubePos(DeviceControl::RVPosition_t TubePos){m_TubePos = TubePos;}
	
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetParameters
	 *
	 *  \return from GetParameters
	 */
	/****************************************************************************/
    QString GetParameters()const{ return GetName() + QString("(%1,%2)").arg(m_ReagentGrpID).arg((qint32)m_TubePos);}
	/****************************************************************************/
	/*!
	 *  \brief  Definition/Declaration of function GetStrResult
	 *
	 *  \return from GetStrResult
	 */
	/****************************************************************************/
    QString GetStrResult()const{ return QString("%1").arg((qint32)m_result);}

private:
    CmdIDBottleCheck();                                                    ///< Not implemented.
    CmdIDBottleCheck(const CmdIDBottleCheck &);                     ///< Not implemented.
    const CmdIDBottleCheck & operator = (const CmdIDBottleCheck &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;       ///<  Definition/Declaration of variable m_result
	mutable QString m_ReagentGrpID;       ///<  Definition/Declaration of variable m_ReagentGrpID
	mutable DeviceControl::RVPosition_t m_TubePos;       ///<  Definition/Declaration of variable m_TubePos
	
};



}

#endif // CMDIDBOTTLECHECK_H
