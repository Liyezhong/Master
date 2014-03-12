/****************************************************************************/
/*! \file ClientUser.h
 *
 *  \brief ClientUser definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-05-31
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

#ifndef CLIENTUSER_CLIENTUSER_H
#define CLIENTUSER_CLIENTUSER_H

#include <ClientUser/Include/ClientUserProxy.h>
#include <NetworkComponents/Include/NetworkClientDevice.h>
#include <QObject>

namespace ClientUser {

/****************************************************************************/
/**
 * \brief This class adds custom send and receive functions to the basic
 *        network communication module.
 */
/****************************************************************************/
class CClientUser : public NetworkBase::NetworkClientDevice
{
    Q_OBJECT

public:
    CClientUser(CClientUserProxy *ClientUserProxy, NetworkBase::NetworkClientType_t mytype,
        const QString & ip, const QString & port, QObject * pParent = 0);

    CClientUserProxy *GetCommandProxy();
    bool InitializeDevice();
    void SendMessageToMaster(const QString Name, const QStringList &paramList, QString *ref, QString *result);

private:
    bool ProcessIncomingMessage(const QString &cmdname, const QDomDocument &domD);

    CClientUserProxy *m_pClientUserProxy;   //!< Helps to forward incoming messages to the application
};

} // end namespace ClientUser

#endif // CLIENTUSER_CLIENTUSER_H
