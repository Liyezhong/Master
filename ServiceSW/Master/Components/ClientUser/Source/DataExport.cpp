/****************************************************************************/
/*! \file DataExport.cpp
 *
 *  \brief DataExport command implementation.
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

#include <ClientUser/Include/DataExport.h>
#include <ClientUser/Include/ClientUser.h>

namespace ClientUser {

/****************************************************************************/
/*!
 *  \brief   Constructor for OutgoingCommand class
 *
 ****************************************************************************/
DataExport::DataExport() : ClientUser::CClientUserProtocolTxCommand()
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor for OutgoingCommand class
 *
 ****************************************************************************/
DataExport::~DataExport()
{
}

/****************************************************************************/
/*!
 *  \brief   Call this function to execute the command
 *
 *  \return  TRUE if execution succeeded, FALSE otherwise
 *
 ****************************************************************************/
bool DataExport::Execute()
{
    qDebug() << "DataExport: ExecuteTransmitter called!";

    if (m_myDevice == NULL) {
        qDebug() << "DataExport: device pointer is NULL!\n";
        return false;
    }

    QString cmd = "<message><cmd name=\"DataExport\" ref=\"" +
        QString::number(m_myRef, 10) + "\" /><dataitems exporttype=\"" + m_myPayload.at(0) + "\" /></message>";

    qDebug() << "DataExport: sent export data - " << m_myPayload.at(0) << "\n";

    return NetworkBase::ProtocolTxCommand::ExecuteWithParameter(cmd);
}

/****************************************************************************/
/*!
 *  \brief   This function handles acknowledge of this command
 *
 *  \oparam  status = if remote command was executed successfully or not
 *
 ****************************************************************************/
void DataExport::HandleAck(const QString &status)
{
    if (m_myDevice == NULL) {
        qDebug() << "DataExport: device pointer is NULL !\n";
        return;
    }
    qDebug() << "DataExport: _ACK_ called.";
    if (status != "ok") {
        //TODO: handle failed command
        qDebug() << "DataExport: negative _ACK_ received with status: " << status;
    }
    // do basic stuff: stop timer, deregister command, etc.
    ProtocolTxCommand::HandleAck(status);
}

/****************************************************************************/
/*!
 *  \brief   This function handles reply timeout
 *
 ****************************************************************************/
void DataExport::HandleAckTimeout()
{
    if (m_myDevice == NULL) {
        qDebug() << "DataExport: device pointer is NULL !\n";
        return;
    }
    qDebug() << "DataExport: _ACK-TIMEOUT_ called.";
    // do basic stuff:
    ProtocolTxCommand::HandleAckTimeout();
}

} // end namespace ClientUser
