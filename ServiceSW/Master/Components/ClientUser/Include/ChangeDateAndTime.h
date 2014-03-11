/****************************************************************************/
/*! \file ChangeDateAndTime.h
 *
 *  \brief ChangeDateAndTime command definition.
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

#ifndef CLIENTUSER_CHANGEDATEANDTIME_H
#define CLIENTUSER_CHANGEDATEANDTIME_H

#include <Global/Include/AbstractFactory.h>
#include <ClientUser/Include/ClientUserProtocolTxCommand.h>

namespace ClientUser {

/****************************************************************************/
/*!
 *  \brief  This class implements a generic Outgoing command.
 *
 *      This class shall be used as a derived class for AbstractFactory of
 *      protocol command class objects. See description of the base class.
 */
/****************************************************************************/

class ChangeDateAndTime : public ClientUser::CClientUserProtocolTxCommand {
    Q_OBJECT

private:
    DECLARE_REG_CLASS(ClientUser::CClientUserProtocolTxCommand, ChangeDateAndTime);   ///< Declaration of static registration instance.

public:
    ChangeDateAndTime();
    ~ChangeDateAndTime();

public:
    bool Execute();
    void HandleAck(const QString & status);
    void HandleAckTimeout();

}; // command

} // end namespace ClientUser

#endif // CLIENTUSER_CHANGEDATEANDTIME_H
