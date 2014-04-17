/****************************************************************************/
/*! \file CmdTemplate.h
 *
 *  \brief CmdTemplate command definition.
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


#ifndef CMDTEMPLATE_H
#define CMDTEMPLATE_H

#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"

namespace Scheduler{

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdTemplate
 */
/****************************************************************************/
class CmdTemplate : public CmdSchedulerCommandBase
{
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdTemplate
     *
     *  \return from CmdTemplate
     */
    /****************************************************************************/
    CmdTemplate(int Timeout, SchedulerMainThreadController *controller);
    ~CmdTemplate();

    static QString NAME;    ///< Command name.


    QString GetName() const{return NAME;}

    fct_ret
	fct_par
	fct_str

private:
    CmdTemplate();                                                    ///< Not implemented.
    CmdTemplate(const CmdTemplate &);                     ///< Not implemented.
    const CmdTemplate & operator = (const CmdTemplate &); ///< Not implemented.


	var_ret
	var_par
};



}

#endif // CMDTEMPLATE_H
