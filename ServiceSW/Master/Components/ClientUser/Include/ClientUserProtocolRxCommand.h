/****************************************************************************/
/*! \file ClientUserProtocolRxCommand.h
 *
 *  \brief ClientUserProtocolRxCommand definition.
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

#ifndef CLIENTUSER_CLIENTUSERPROTOCOLRXCOMMAND_H
#define CLIENTUSER_CLIENTUSERPROTOCOLRXCOMMAND_H

#include <QDomDocument>
#include <Global/Include/AbstractFactory.h>
#include <NetworkComponents/Include/ProtocolRxCommand.h>

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
class CClientUserProtocolRxCommand : public NetworkBase::ProtocolRxCommand {
    Q_OBJECT

public:

    CClientUserProtocolRxCommand();
    CClientUserProtocolRxCommand(const QStringList &attrs);
    CClientUserProtocolRxCommand(const QStringList attrs, const QStringList dattrs);

    /****************************************************************************/
    /*!
     *  \brief   Destructor
     *
     ****************************************************************************/
    virtual ~CClientUserProtocolRxCommand()
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

typedef Global::AbstractFactory<CClientUserProtocolRxCommand> ClientUserRxPCmd;    ///< Typedef for easier usage.

} // end namespace ClientUser

#endif // CLIENTUSER_CLIENTUSERPROTOCOLRXCOMMAND_H
