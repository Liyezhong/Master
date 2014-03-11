/****************************************************************************/
/*! \file ClientUserInterface.cpp
 *
 *  \brief ClientUserInterface implementation.
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

#include "ClientUser/Include/ClientUserInterface.h"

namespace ClientUser {

/****************************************************************************/
/*!
 *  \brief    Constructor
 *
 *  \iparam   ip = server's ip
 *  \iparam   port = server's port
 *
 ****************************************************************************/
CClientUserInterface::CClientUserInterface(const QString &ip, const QString &port) :
        m_myNetClient(NULL),
        m_IP(ip),
        m_Port(port)
{
}

/****************************************************************************/
/*!
 *  \brief    Destructor
 *
 ****************************************************************************/
CClientUserInterface::~CClientUserInterface()
{
    try {
        m_NetworkThread.exit();
        delete m_myNetClient;
    }
    catch (...) {}
}

/****************************************************************************/
/*!
 *  \brief   Initializes network layer for communication with Master
 *
 *  \iparam  ClientUserProxy = Message proxy of the application
 *
 *  \return  true if init successful, false otherwise
 *
 ****************************************************************************/
bool CClientUserInterface::Initialize(CClientUserProxy *ClientUserProxy)
{
    try {
        // parent shall be NULL if ClientUser runs in a diferent thread!
        m_myNetClient = new CClientUser(ClientUserProxy, MY_CLIENT_TYPE, m_IP, m_Port, NULL);
    }
    catch (const std::bad_alloc &) {
        qDebug() << "CClientUserInterface: Cannot create CClientUserInterface!\n";
        return false;
    }

    if (!m_myNetClient->InitializeDevice()) {
        qDebug() << "CClientUserInterface: Cannot initialize CClientUserInterface!\n";
        return false;
    }

    // the Network Layer will start as soon as thread emits "started" signal
    if(!QObject::connect(&m_NetworkThread, SIGNAL(started()), m_myNetClient, SLOT(StartDevice()))) {
        qDebug() << "CClientUserInterface: cannot connect CClientUserInterface's StartDevice slot!\n";
        return false;
    }

    // initialization done, move client to network thread
    m_myNetClient->moveToThread(&m_NetworkThread);

    return true;
}

/****************************************************************************/
/*!
 *  \brief   Starts network layer after successful initialization.
 *
 ****************************************************************************/
void CClientUserInterface::Go()
{
    if (m_myNetClient == NULL) {
        return;
    }
    // start the network thread
    m_NetworkThread.start();
}

/****************************************************************************/
/*!
 *  \brief    Send test message to Master
 *
 *  \iparam   Name = Command name
 *  \iparam   paramList = Message parameters
 *
 ****************************************************************************/
void CClientUserInterface::SendMessageToMaster(const QString Name, const QStringList &paramList)
{
    qDebug() << "CClientUserInterface: sending message: " << Name << " " << paramList;

    if (m_myNetClient == NULL) {
        qDebug() << "CClientUserInterface: m_myNetClient is not initialized";
        return;
    }

    QString err = "";
    QString cmdReference = "";
    ///< \warning: the cmdReference's value must be stored to be available outside
    ///<           the scope of this function. otherwise it will not be possible
    ///<           to evaluate acknowledges from Master.
    m_myNetClient->SendMessageToMaster(Name, paramList, &cmdReference, &err);
    qDebug() << (QString)("CClientUserInterface: sent command with reference " + cmdReference);

    // check local command execution status:
    if (err != "ok") {
        qDebug() << (QString)("CClientUserInterface: sending message failed! Local execution status: " + err);
        ///< \todo:  do whatever needs to be done if SendMessage fails !
    }
}

} // end namespace ClientUser
