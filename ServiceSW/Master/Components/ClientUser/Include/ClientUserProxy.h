/****************************************************************************/
/*! \file ClientUserProxy.h
 *
 *  \brief ClientUserProxy definition.
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

#ifndef CLIENTUSER_CLIENTUSERPROXY_H
#define CLIENTUSER_CLIENTUSERPROXY_H

#include <QDateTime>

namespace ClientUser {

/****************************************************************************/
/**
 * \brief This class holds signals transferring data from received messages
 *        to the main data structures of the GUI.
 */
/****************************************************************************/
class CClientUserProxy : public QObject
{
    Q_OBJECT

public:
    CClientUserProxy();
    void EmitDateAndTime(QDateTime DateTime);

signals:
/****************************************************************************/
/**
 * \brief Forwards the received date and time to the application
 *
 * \iparam DateTime = Current date and time
 */
/****************************************************************************/
    void DateAndTime(QDateTime DateTime);
};

} // end namespace ClientUser

#endif // CLIENTUSER_CLIENTUSERPROXY_H
