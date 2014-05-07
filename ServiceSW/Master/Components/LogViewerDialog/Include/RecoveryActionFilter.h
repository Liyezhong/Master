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
 *       Leica Biosystems Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Shanghai. All rights reserved.
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

/****************************************************************************/
/**
 * \brief This implements the class CRecoveryActionFilter
 */
/****************************************************************************/
class CRecoveryActionFilter : public QObject {
    Q_OBJECT
private:
    QString m_Path; //!< Recovery action configuration file path
    QHash<QString, qint64> m_ItemPositons; //!< Store event id and file position by Qhash
    qint64  m_Position; //!< position for Recovery action file

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *
     *  \iparam Path = Recovery Action configuration file path
     */
    /****************************************************************************/
    CRecoveryActionFilter(const QString& Path);


    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CRecoveryActionFilter();


    /****************************************************************************/
    /*!
     *  \brief To get recovery action text by event id
     *  \param EventId = event id
     *  \return The text of recovery action
     */
    /****************************************************************************/
    const QString GetRecoveryActionText(const QString& EventId );
private:

    /****************************************************************************/
    /*!
     *  \brief To check the line from Recovery Ation file by event id
     *  \param EventId = event id
     *  \return The text of recovery action
     */
    /****************************************************************************/
    const QString CheckFromFile(const QString& EventId);
};
}

#endif  // RECOVERYACTIONFILTER_H
