/****************************************************************************/
/*! \file ClientUser.cpp
 *
 *  \brief ClientUser implementation.
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

#include "ClientUser/Include/ClientUser.h"
#include "ClientUser/Include/ClientUserProtocolRxCommand.h"
#include "ClientUser/Include/ClientUserProtocolTxCommand.h"

namespace ClientUser {

/****************************************************************************/
/*!
 *  \brief    Constructor
 *
 *  \iparam   ClientUserProxy = command proxy to front end
 *  \iparam   Mytype = type of network client
 *  \iparam   Ip = server's ip
 *  \iparam   Port = server's port
 *  \iparam   pParent = pointer to parent object
 */
/****************************************************************************/
CClientUser::CClientUser(CClientUserProxy *ClientUserProxy, NetworkBase::NetworkClientType_t Mytype,
        const QString & Ip, const QString & Port, QObject *pParent)
    : NetworkClientDevice::NetworkClientDevice(Mytype, Ip, Port, "", "", pParent), m_pClientUserProxy(ClientUserProxy)
{

}

/****************************************************************************/
/*!
 *  \brief    Returns the command proxy of an object.
 *
 *  \return   Pointer to the command proxy
 */
/****************************************************************************/
CClientUserProxy *CClientUser::GetCommandProxy()
{
    return m_pClientUserProxy;
}

/****************************************************************************/
/*!
 *  \brief    API function to call before the client can be used
 *
 *      This function does everything to initialize the client. It shall be
*       called before the client can be used.
 *
 *  \return   true if success, false if error
 *
 ****************************************************************************/
bool CClientUser::InitializeDevice()
{
    qDebug() << "CClientUser: initializing...";

    if (!NetworkClientDevice::InitializeDevice()) {
            qDebug() << "CClientUser: cannot initialize Network Client Device.";
            return false;
    }

    // configure static members of abstract factory commands
    ClientUserTxPCmd::Initialize();
    ClientUserRxPCmd::Initialize();

    return true;
}

/****************************************************************************/
/*!
 *  \brief  This method processes an incomig from peer device message.
 *
 *          This method is called by the ProtocolHandler.
 *
 *  \iparam cmdname - name of the command
 *  \iparam domD - message as XML document
 *
 *  \return true
 */
/****************************************************************************/
bool CClientUser::ProcessIncomingMessage(const QString &cmdname, const QDomDocument &domD)
{
    // check if it is a HeartBeat, Ack or any other base command:
    if (NetworkDevice::CheckClassRegistration<NetworkBase::ProtocolRxCommand>(cmdname)) {
        // yes, it is base command, run it:
        return NetworkDevice::ProcessIncomingMessage(cmdname, domD);
    }

    qDebug() << "CClientUser: incoming Protocol message : " << cmdname << "\n";

    // create command processing object
    ClientUser::CClientUserProtocolRxCommand *pC =
        CreateNewIncomingCommand<ClientUser::CClientUserProtocolRxCommand, ClientUser::ClientUserRxPCmd>(cmdname, domD);
    if (pC == NULL) {
        //TODO: log error
        return false;
    }
    // run created command:
    bool result = pC->Execute();
    delete pC;
    return result;
}

/****************************************************************************/
/*!
 *  \brief    This function sends a protocol message to Master over TCP Socket
 *
 *  \iparam Name = Message command
 *  \iparam paramList = Message parameters
 *  \oparam ref = Actual reference of the sent command
 *  \oparam result = If message sending was successful or not
 *
 *  \return
 *
 ****************************************************************************/
void CClientUser::SendMessageToMaster(const QString Name, const QStringList &paramList, QString *ref, QString *result)
{
    *result = "ok";
    Q_UNUSED(Name);
    Q_UNUSED(paramList);
    Q_UNUSED(ref);

    // create command processing object
    /*ClientUser::CClientUserProtocolTxCommand *pC =
        CreateNewOutgoingCommand<ClientUser::CClientUserProtocolTxCommand, ClientUser::ClientUserTxPCmd>(Name, paramList);
    if (pC == NULL) {
        *result = "failed";
        *ref = "";
        return;
    }
    // run created command:
    if (!pC->Execute()) {
        *result = "failed";
        *ref = "";
        return;
    }

    *ref = pC->GetReference();*/
}

} // end namespace ClientUser
