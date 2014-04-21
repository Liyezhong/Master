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
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef LOGFILTER_H
#define LOGFILTER_H

#include <QVector>
#include <QList>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>
#include <QMap>

#include "Global/Include/GlobalDefines.h"

namespace LogViewer {

class CLogFilter : public QObject {
    Q_OBJECT
private:
    QVector<QStringList> m_LogItems;

    QMap<int, int> m_SubItems[6];
    QString m_Filename;
    QList<int> m_Columns;
    bool m_needClassify;
    QStandardItemModel m_Model;
    QStandardItemModel m_SubModel;
    QMap<QString, QString> m_FileInfo;
    quint32   m_LogIndex;

    void AddItem4RecoveryAction(QString& data);
    void AddItem4Log(QString& data);
    void AddItem4LogNeedClassify(QString& data);
    bool CheckFileInfo(const QString& line);

public:
    static quint8 m_AllTypes;

public:
    CLogFilter(const QString& Filename, const QList<qint32>& Columns, bool NeedClassify=false);
    ~CLogFilter();
    bool InitData();
    QStandardItemModel* GetItemModel( quint64 EventTypes = LogViewer::CLogFilter::m_AllTypes);
    const QMap<QString, QString>& GetFileInfo( );


};
}

#endif // LOGFILTER_H
