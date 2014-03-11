/****************************************************************************/
/*! \file ClientUserInterface.h
 *
 *  \brief ClientUserInterface definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-05-26
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

#ifndef CLIENTUSER_ClientUser_H
#define CLIENTUSER_ClientUser_H

#include <ClientUser/Include/ClientUser.h>
#include <ClientUser/Include/ClientUserProxy.h>

namespace ClientUser {

///< Type of network client to create
const NetworkBase::NetworkClientType_t MY_CLIENT_TYPE = NetworkBase::NCE_TYPE_COLORADO_GUI;

/****************************************************************************/
/**
 * \brief This class is an interface to ClientUser.
 */
/****************************************************************************/
class CClientUserInterface : public QObject
{
   Q_OBJECT

public:
    CClientUserInterface(const QString & ip, const QString & port);
    ~CClientUserInterface();

public:
    bool Initialize(CClientUserProxy *ClientUserProxy);
    void Go();

public slots:
    void SendMessageToMaster(const QString Name, const QStringList &paramList);

private:
    CClientUser *m_myNetClient; ///< network layer
    QThread m_NetworkThread;    ///< thread to move network communication to
    QString m_IP;               ///< IP to connect to
    QString m_Port;             ///< port to connect to
};

} // end namespace ClientUser

#endif // CLIENTUSER_ClientUser_H
