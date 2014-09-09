/****************************************************************************/
/*! \file ServiceHelpTextFilter.cpp
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
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include "LogViewerDialog/Include/ServiceHelpTextFilter.h"
#include "Global/Include/GlobalDefines.h"

namespace LogViewer {

CServiceHelpTextFilter::CServiceHelpTextFilter(const QString& Path)
{
    m_Path = Path;
    m_ItemPositons.clear();
    m_Position = 0;
}

CServiceHelpTextFilter::~CServiceHelpTextFilter()
{
    try {
        m_ItemPositons.clear();
    }
    catch (...) {

    }
}

const QString CServiceHelpTextFilter::CheckFromFile(const QString& EventId)
{
    QFile File(m_Path);

    if(!File.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "";
    }

    (void)File.seek(m_Position);
        qDebug() << "\n\n" << __FUNCTION__ << __LINE__ << "\n\n";
    while(!File.atEnd()) {
        qint64 Position = File.pos();
        QByteArray LineBa = File.readLine();
        m_Position = File.pos();
        if (LineBa.contains(";")) {
            QList<QByteArray> List = LineBa.split(';');
            QString EventIdInFile = QString(List.at(0));
            m_ItemPositons.insert(EventIdInFile, Position);
            if (EventIdInFile == EventId) {
                File.close();
                return QString(LineBa);
            }
        }
    }

    File.close();

    return "";
}

const QString CServiceHelpTextFilter::GetServiceHelpText(const QString &EventId)
{
    quint64 Position = m_ItemPositons.value(EventId);

    if (Position > 0) {
        QFile File(m_Path);
        if(!File.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return "";
        }
        (void)File.seek(Position);
        QByteArray Line = File.readLine();
        return QString(Line);
    }
    else {
        return CheckFromFile(EventId);
    }
}

}
