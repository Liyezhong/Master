/****************************************************************************/
/*! \file DateAndTime.h
 *
 *  \brief DateAndTime command definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-05-25
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
#ifndef CLIENTUSER_DATEANDTIME_H
#define CLIENTUSER_DATEANDTIME_H

#include <Global/Include/AbstractFactory.h>
#include <ClientUser/Include/ClientUserProtocolRxCommand.h>

namespace ClientUser {

/****************************************************************************/
/*!
 *  \brief  This class implements a IncomingCommand command.
 *
 *      This class shall be used as a derived class for AbstractFactory of
 *      protocol command class objects. See description of the base class.
 */
/****************************************************************************/

class DateAndTime : public ClientUser::CClientUserProtocolRxCommand {
    Q_OBJECT

private:
    DECLARE_REG_CLASS(ClientUser::CClientUserProtocolRxCommand, DateAndTime);   ///< Declaration of static registration instance.

public:
    DateAndTime();
    ~DateAndTime();

    bool Execute();

}; // command

} // end namespace ClientUser

#endif // CLIENTUSER_DATEANDTIME_H
