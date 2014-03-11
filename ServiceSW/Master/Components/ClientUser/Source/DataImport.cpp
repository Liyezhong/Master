/****************************************************************************/
/*! \file DataImport.cpp
 *
 *  \brief DataImport command implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-06-22
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

#include <ClientUser/Include/DataImport.h>
#include <ClientUser/Include/ClientUser.h>

namespace ClientUser {

/****************************************************************************/
/*!
 *  \brief   Constructor for OutgoingCommand class
 *
 ****************************************************************************/
DataImport::DataImport() : ClientUser::CClientUserProtocolTxCommand()
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor for OutgoingCommand class
 *
 ****************************************************************************/
DataImport::~DataImport()
{
}

/****************************************************************************/
/*!
 *  \brief   Call this function to execute the command
 *
 *  \return  TRUE if execution succeeded, FALSE otherwise
 *
 ****************************************************************************/
bool DataImport::Execute()
{
    qDebug() << "DataImport: ExecuteTransmitter called!";

    if (m_myDevice == NULL) {
        qDebug() << "DataImport: device pointer is NULL!\n";
        return false;
    }

    QString cmd = "<message><cmd name=\"DataImport\" ref=\"" +
        QString::number(m_myRef, 10) + "\" /><dataitems importtype=\"" + m_myPayload.at(0) + "\" /></message>";

    qDebug() << "DataImport: sent import data - " << m_myPayload.at(0) << "\n";

    return NetworkBase::ProtocolTxCommand::ExecuteWithParameter(cmd);
}

/****************************************************************************/
/*!
 *  \brief   This function handles acknowledge of this command
 *
 *  \oparam  status = if remote command was executed successfully or not
 *
 ****************************************************************************/
void DataImport::HandleAck(const QString &status)
{
    if (m_myDevice == NULL) {
        qDebug() << "DataImport: device pointer is NULL !\n";
        return;
    }
    qDebug() << "DataImport: _ACK_ called.";
    if (status != "ok") {
        //TODO: handle failed command
        qDebug() << "DataImport: negative _ACK_ received with status: " << status;
    }
    // do basic stuff: stop timer, deregister command, etc.
    ProtocolTxCommand::HandleAck(status);
}

/****************************************************************************/
/*!
 *  \brief   This function handles reply timeout
 *
 ****************************************************************************/
void DataImport::HandleAckTimeout()
{
    if (m_myDevice == NULL) {
        qDebug() << "DataImport: device pointer is NULL !\n";
        return;
    }
    qDebug() << "DataImport: _ACK-TIMEOUT_ called.";
    // do basic stuff:
    ProtocolTxCommand::HandleAckTimeout();
}

} // end namespace ClientUser
