/****************************************************************************/
/*! \file ServiceHelpTextFilter.h
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

#ifndef SERVICEHELPTEXTFILTER_H
#define SERVICEHELPTEXTFILTER_H

#include <QHash>
#include <QString>
#include <QObject>

namespace LogViewer {

/****************************************************************************/
/**
 * \brief This implements the class CServiceHelpTextFilter
 */
/****************************************************************************/
class CServiceHelpTextFilter : public QObject {
    Q_OBJECT
private:
    QString m_Path; //!< Service help text configuration file path
    QHash<QString, qint64> m_ItemPositons; //!< Store event id and file position by Qhash
    qint64  m_Position; //!< position for Service help text file

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *
     *  \iparam Path = Service Help Text configuration file path
     */
    /****************************************************************************/
    CServiceHelpTextFilter(const QString& Path);


    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CServiceHelpTextFilter();


    /****************************************************************************/
    /*!
     *  \brief To get Service Help Text by event id
     *  \param EventId = event id
     *  \return The text of Service Help Text
     */
    /****************************************************************************/
    const QString GetServiceHelpText(const QString& EventId );
private:

    /****************************************************************************/
    /*!
     *  \brief To check the line from Service Help Text file by event id
     *  \param EventId = event id
     *  \return The text of Service help text
     */
    /****************************************************************************/
    const QString CheckFromFile(const QString& EventId);
};
}

#endif  // SERVICEHELPTEXTFILTER_H
