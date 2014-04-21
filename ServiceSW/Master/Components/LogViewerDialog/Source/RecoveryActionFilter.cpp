/****************************************************************************/
/*! \file RecoveryActionFilter.cpp
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
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include "LogViewerDialog/Include/RecoveryActionFilter.h"
#include "Global/Include/GlobalDefines.h"

namespace LogViewer {

CRecoveryActionFilter::CRecoveryActionFilter(const QString& Path)
{
    m_Path = Path;
    m_ItemPositons.clear();
    m_Position = 0;
}

CRecoveryActionFilter::~CRecoveryActionFilter()
{
    m_ItemPositons.clear();
}

const QString CRecoveryActionFilter::CheckFromFile(const QString& EventId)
{
    QFile File(m_Path);

    if(!File.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "";
    }

    File.seek(m_Position);

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

const QString CRecoveryActionFilter::GetRecoveryAtionText(const QString &EventId)
{
    quint64 Position = m_ItemPositons.value(EventId);

    if (Position > 0) {
        QFile File(m_Path);
        if(!File.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return "";
        }
        File.seek(Position);
        QByteArray Line = File.readLine();
        return QString(Line);
    }
    else {
        return CheckFromFile(EventId);
    }
}

}
