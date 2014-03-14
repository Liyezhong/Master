/****************************************************************************/
/*! \file ClientUserProxy.cpp
 *
 *  \brief ClientUserProxy implementation.
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

#include "ClientUser/Include/ClientUserProxy.h"

namespace ClientUser {

/****************************************************************************/
/*!
 *  \brief    Constructor
 */
/****************************************************************************/
CClientUserProxy::CClientUserProxy() : QObject()
{
}

/****************************************************************************/
/*!
 *  \brief    Constructor
 *
 *  \iparam   DateTime = Date and time received from the network
 */
/****************************************************************************/
void CClientUserProxy::EmitDateAndTime(QDateTime DateTime)
{
    qDebug("CClientUserProxy: EmitDateAndTime called");
    emit DateAndTime(DateTime);
}

} // end namespace ClientUser
