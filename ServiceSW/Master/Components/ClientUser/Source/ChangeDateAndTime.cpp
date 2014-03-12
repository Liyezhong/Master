/****************************************************************************/
/*! \file ChangeDateAndTime.cpp
 *
 *  \brief ChangeDateAndTime command implementation.
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

#include <ClientUser/Include/ChangeDateAndTime.h>
#include <ClientUser/Include/ClientUser.h>

namespace ClientUser {

/****************************************************************************/
/*!
 *  \brief   Constructor for OutgoingCommand class
 *
 ****************************************************************************/
ChangeDateAndTime::ChangeDateAndTime() : ClientUser::CClientUserProtocolTxCommand()
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor for OutgoingCommand class
 *
 ****************************************************************************/
ChangeDateAndTime::~ChangeDateAndTime()
{
}

/****************************************************************************/
/*!
 *  \brief   Call this function to execute the command
 *
 *  \return  TRUE if execution succeeded, FALSE otherwise
 *
 ****************************************************************************/
bool ChangeDateAndTime::Execute()
{
    qDebug() << "ChangeDateAndTime: ExecuteTransmitter called!";

    if (m_myDevice == NULL) {
        qDebug() << "ChangeDateAndTime: device pointer is NULL!\n";
        return false;
    }

    QString cmd = "<message><cmd name=\"ChangeDateAndTime\" ref=\"" +
        QString::number(m_myRef, 10) + "\" /><dataitems dt=\"" + m_myPayload.at(0) + "\" /></message>";

    qDebug() << "\nChangeDateAndTime: sent date and time - " << m_myPayload.at(0) << "\n";

    return NetworkBase::ProtocolTxCommand::ExecuteWithParameter(cmd);
}

/****************************************************************************/
/*!
 *  \brief   This function handles acknowledge of this command
 *
 *  \oparam  status = if remote command was executed successfully or not
 *
 ****************************************************************************/
void ChangeDateAndTime::HandleAck(const QString &status)
{
    if (m_myDevice == NULL) {
        qDebug() << "ChangeDateAndTime: device pointer is NULL !\n";
        return;
    }
    qDebug() << "ChangeDateAndTime: _ACK_ called.";
    if (status != "ok") {
        //TODO: handle failed command
        qDebug() << "ChangeDateAndTime: negative _ACK_ received with status: " << status;
    }
    // do basic stuff: stop timer, deregister command, etc.
    ProtocolTxCommand::HandleAck(status);
}

/****************************************************************************/
/*!
 *  \brief   This function handles reply timeout
 *
 ****************************************************************************/
void ChangeDateAndTime::HandleAckTimeout()
{
    if (m_myDevice == NULL) {
        qDebug() << "ChangeDateAndTime: device pointer is NULL !\n";
        return;
    }
    qDebug() << "ChangeDateAndTime: _ACK-TIMEOUT_ called.";
    // do basic stuff:
    ProtocolTxCommand::HandleAckTimeout();
}

} // end namespace ClientUser
