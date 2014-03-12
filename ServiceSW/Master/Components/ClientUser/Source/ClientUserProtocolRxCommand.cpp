/****************************************************************************/
/*! \file ClientUserProtocolRxCommand.cpp
 *
 *  \brief ClientUserProtocolRxCommand implementation.
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

#include "ClientUser/Include/ClientUserProtocolRxCommand.h"
#include "ClientUser/Include/DateAndTime.h"

namespace ClientUser {

/*! Defintion of the DateAndTime RX command registration */
DEFINE_REG_CLASS(ClientUser::CClientUserProtocolRxCommand, DateAndTime);

/****************************************************************************/
/*!
 *  \brief   Default Constructor
 *
 ****************************************************************************/
CClientUserProtocolRxCommand::CClientUserProtocolRxCommand() :
        ProtocolRxCommand::ProtocolRxCommand()
{
}

/****************************************************************************/
/*!
 *  \brief   Constructor
 *
 *  \iparam  attrs = Message parameters
 *
 ****************************************************************************/
CClientUserProtocolRxCommand::CClientUserProtocolRxCommand(const QStringList &attrs) :
        ProtocolRxCommand::ProtocolRxCommand(attrs)
{
}

/****************************************************************************/
/*!
 *  \brief   Constructor
 *
 *  \iparam  attrs = Message parameters
 *  \iparam  dattrs = Message data parameters
 *
 ****************************************************************************/
CClientUserProtocolRxCommand::CClientUserProtocolRxCommand(const QStringList attrs, const QStringList dattrs) :
        ProtocolRxCommand::ProtocolRxCommand(attrs, dattrs)
{
}

/****************************************************************************/
/*!
 * \brief This method must be called prior to first use of the class object.
 *
 *        Calling this method ensures that the linker will create all needed
 *        static members. Otherwise it will not. Seem to be a linker bug.
 *
 */
/****************************************************************************/
void CClientUserProtocolRxCommand::LinkerTamer()
{
    qDebug() << "CClientUserProtocolRxCommand: LinkerTamer called !";
}


} // end namespace ClientUser
