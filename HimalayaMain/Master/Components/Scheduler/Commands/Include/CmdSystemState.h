/****************************************************************************/
/*! \file CmdSystemState.h
 *
 *  \brief Definition file for class CmdSystemState
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-1-5
 *  $Author:    $ Abe Yang
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2013 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef NETCOMMANDS_CMDSYSTEMSTATE_H
#define NETCOMMANDS_CMDSYSTEMSTATE_H

#include <Global/Include/Commands/Command.h>

namespace NetCommands {
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CmdSystemState
 */
/****************************************************************************/

class CmdSystemState : public Global::Command {
public:
    static QString NAME;    ///< Command name.
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CmdSystemState
     *  \param  stateId - quint32
     *  \return from CmdSystemState
     */
    /****************************************************************************/
    CmdSystemState(quint32 stateId);
    ~CmdSystemState();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetName
     *  \return from GetName
     */
    /****************************************************************************/
    virtual QString GetName() const;

private:
    CmdSystemState(const CmdSystemState &);                       ///< Not implemented.
    const CmdSystemState & operator = (const CmdSystemState &);   ///< Not implemented.

public:
     quint32 m_StateId;       ///<  Definition/Declaration of variable m_StateId


}; // end class CmdSystemState

}// end namespace NetCommands
#endif // NETCOMMANDS_CMDSYSTEMSTATE_H
