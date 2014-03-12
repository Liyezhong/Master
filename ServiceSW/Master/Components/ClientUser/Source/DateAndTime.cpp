/****************************************************************************/
/*! \file DateAndTime.cpp
 *
 *  \brief DateAndTime command implementation.
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

#include <ClientUser/Include/DateAndTime.h>
#include <ClientUser/Include/ClientUser.h>
#include <ClientUser/Include/ClientUserProxy.h>
#include <NetworkComponents/Include/ProtocolRxCommands/Ack.h>

namespace ClientUser {

//! XML attributes processed by this command:
const QStringList DateAndTimeAttr = QStringList() << "dt";
//! actual Date and Time format:
const QString DT_DATEANDTIME_FORMAT = "dd.MM.yyyy hh:mm:ss";

/****************************************************************************/
/*!
 *  \brief   Constructor
 *
 ****************************************************************************/
DateAndTime::DateAndTime() : ClientUser::CClientUserProtocolRxCommand(DateAndTimeAttr)
{
}

/****************************************************************************/
/*!
 *  \brief   Destructor
 *
 ****************************************************************************/
DateAndTime::~DateAndTime()
{
}

/****************************************************************************/
/*!
 *  \brief   Call this function to execute the command
 *
 *  \return  TRUE if execution succeeded, FALSE otherwise
 *
 ****************************************************************************/
bool DateAndTime::Execute()
{
    CClientUserProxy *pClientUserProxy;
    qDebug() << (QString)"DateAndTime: Execute called !";

    if (m_myDevice == NULL) {
        qDebug() << "DateAndTime: device pointer is NULL!\n";
        return false;
    }

    Q_ASSERT(!m_myMessageDataTree.isEmpty());

    // fetch date and time
    QDateTime qdt = QDateTime::fromString(m_myMessageDataTree[0].DataitemsValues[0], DT_DATEANDTIME_FORMAT);
    qdt.setTimeSpec(Qt::UTC);

    // message header was successfully extracted, forward the message:
    pClientUserProxy = ((CClientUser *)m_myDevice)->GetCommandProxy();
    pClientUserProxy->EmitDateAndTime(qdt);
    return NetworkBase::Ack::SendAcknowledge(m_myDevice, m_myRef, "DateAndTime", "ok");
}

} // end namespace ClientUser
