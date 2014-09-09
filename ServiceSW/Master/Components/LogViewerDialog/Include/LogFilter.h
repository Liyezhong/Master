/****************************************************************************/
/*! \file LogFilter.h
 *
 *  \brief Filter to get log items for Log content dialog implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-04-18
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

#ifndef LOGFILTER_H
#define LOGFILTER_H

#include <QVector>
#include <QList>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>
#include <QMap>

#include "Global/Include/GlobalDefines.h"

namespace LogViewer {

/****************************************************************************/
/**
 * \brief This implements the class CLogFilter
 */
/****************************************************************************/
class CLogFilter : public QObject {
    Q_OBJECT
private:
    QVector<QStringList> m_LogItems; //!< The list of log items

    QMap<int, int> m_SubItems[6]; //!< The map of sub items ,the key = event type , value = log index
    QString m_Filename; //!< Systems Log file name
    QList<int> m_Columns; //!< Columns for the table widget
    bool m_needClassify; //!< Flag for need classify
    QStandardItemModel m_Model; //!< Model for the log dialog table
    QStandardItemModel m_SubModel; //!< no use ?
    QMap<QString, QString> m_FileInfo;//!< The map of file info
    quint32   m_LogIndex;

    /****************************************************************************/
    /*!
     *  \brief  Add Service help text item to Model of the dialog table
     *  \param  data = The line of log data;
     */
    /****************************************************************************/
    void AddItem4ServiceHelpText(QString& data);

    /****************************************************************************/
    /*!
     *  \brief  Add item to Model of the dialog table
     *  \param  line = The line of log data;
     */
    /****************************************************************************/
    void AddItem4Log(QString& data);

    /****************************************************************************/
    /*!
     *  \brief  Add item to Model of the dialog table if need classify
     *  \param  line = The line of log data;
     */
    /****************************************************************************/
    void AddItem4LogNeedClassify(QString& data);

    /****************************************************************************/
    /*!
     *  \brief  Check file info to m_FileInfo
     *  \param  line = The line of log data;
     *  \return retruns false, if check failed.
     */
    /****************************************************************************/
    bool CheckFileInfo(const QString& line);

    /****************************************************************************/
    /*!
     *  \brief  Release resource.
     */
    /****************************************************************************/
    void ReleaseModelItem();

public:
    static quint8 m_AllTypes;

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam Flilename = Log file name
     *  \iparam Columns = Columns for the table widget
     *  \iparam p_Parent = Parent widget
     */
    /****************************************************************************/
    CLogFilter(const QString& Filename, const QList<qint32>& Columns, bool NeedClassify=false);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CLogFilter();

    /****************************************************************************/
    /*!
     *  \brief  To initialize data by the file
     *  \return retruns false, if file open gets failed.
     */
    /****************************************************************************/
    bool InitData();

    /****************************************************************************/
    /*!
     *  \brief To get item model
     *  \iparam EventTypes = Event Types
     *  \return Model pointer
     */
    /****************************************************************************/
    QStandardItemModel* GetItemModel( quint64 EventTypes = LogViewer::CLogFilter::m_AllTypes);

    /****************************************************************************/
    /*!
     *  \brief To get log file info
     *  \return The file info map
     */
    /****************************************************************************/
    const QMap<QString, QString>& GetFileInfo( );


};
}

#endif // LOGFILTER_H
