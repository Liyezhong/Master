/****************************************************************************/
/*! \file CmdALPressure.h
 *
 *  \brief CmdALPressure command definition.
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


#ifndef CMDALPRESSURE_H
#define CMDALPRESSURE_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdALPressure
 */
/****************************************************************************/
class CmdALPressure : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdALPressure
     *
     *  \param Timeout = int type parameter
     *  \param controller =  SchedulerMainThreadController type parameter
     *
     *  \return from CmdALPressure
     */
    /****************************************************************************/
    CmdALPressure(int Timeout, SchedulerMainThreadController *controller);
    ~CmdALPressure();

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
    QString GetStrResult()const{ return QString("%1").arg((qint32)m_result);}

private:
    CmdALPressure();                                                    ///< Not implemented.
    CmdALPressure(const CmdALPressure &);                     ///< Not implemented.
    const CmdALPressure & operator = (const CmdALPressure &); ///< Not implemented.


	mutable DeviceControl::ReturnCode_t m_result;       ///<  Definition/Declaration of variable m_result
	
};



}

#endif // CMDALPRESSURE_H
