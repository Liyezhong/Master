/****************************************************************************/
/*! \file RecoveryActionFilter.h
 *
 *  \brief Filter to get log items for Log content dialog implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-04-19
 *   $Author:  $ Sunny Qu
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

#ifndef RECOVERYACTIONFILTER_H
#define RECOVERYACTIONFILTER_H

#include <QHash>
#include <QString>
#include <QObject>

namespace LogViewer {

class CRecoveryActionFilter : public QObject {
    Q_OBJECT
private:
    QString m_Path;
    QHash<QString, qint64> m_ItemPositons;
    qint64  m_Position;

public:
    CRecoveryActionFilter(const QString& Path);
    ~CRecoveryActionFilter();
    const QString GetRecoveryAtionText(const QString& EventId );
private:
    const QString CheckFromFile(const QString& EventId);
};
}

#endif  // RECOVERYACTIONFILTER_H
