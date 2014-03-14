/****************************************************************************/
/*! \file ClientUserProtocolTxCommand.h
 *
 *  \brief ClientUserProtocolTxCommand definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-07-07
 *   $Author:  $ M.Scherer
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef CLIENTUSER_CLIENTUSERPROTOCOLTXCOMMAND_H
#define CLIENTUSER_CLIENTUSERPROTOCOLTXCOMMAND_H

#include <QDomDocument>
#include <Global/Include/AbstractFactory.h>
#include <NetworkComponents/Include/ProtocolTxCommand.h>

namespace ClientUser {

/****************************************************************************/
/*!
 *  \brief  This is a Base class for protocol commands.
 *
 *      This class shall be used as a base class for AbstractFactory of
 *      protocol command's derived class objects.
 *
 *      Use DEFINE_REG_CLASS(BaseProtocolCommand, DerivedProtocolCommand) macro
 *      in this class's implementation file to register a particular
 *      derived class. Thus it will be possible to instantiate a particular
 *      command by using its name as a string. The purpose of this
 *      "complication" is to make code maintenance much easier. Because we
 *      avoid giant switch-case statements OR need for loads of individual
 *      functions (and corresponding table of function pointers) in order
 *      to instantiate particular commands.
 *
 */
/****************************************************************************/

class CClientUserProtocolTxCommand : public NetworkBase::ProtocolTxCommand {
    Q_OBJECT

public:

    CClientUserProtocolTxCommand();

    /****************************************************************************/
    /*!
     *  \brief   Destructor
     *
     ****************************************************************************/
    virtual ~CClientUserProtocolTxCommand()
    {
    }

public:

    /****************************************************************************/
    /*!
     *  \brief   Call this function to execute the command
     *
     *  \warning Implement this function in the derived class!
     *
     *  \return  TRUE if execution succeeded, FALSE otherwise
     *
     ****************************************************************************/
    virtual bool Execute() = 0;

    /****************************************************************************/
    static void LinkerTamer();

}; // ProtocolCommand

typedef Global::AbstractFactory<CClientUserProtocolTxCommand> ClientUserTxPCmd;    ///< Typedef for easier usage.

} // end namespace ClientUser

#endif // CLIENTUSER_CLIENTUSERPROTOCOLTXCOMMAND_H
