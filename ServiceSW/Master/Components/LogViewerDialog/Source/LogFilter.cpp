/****************************************************************************/
/*! \file LogFilter.cpp
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

#include <QFile>

#include "LogViewerDialog/Include/LogFilter.h"
#include "Global/Include/GlobalDefines.h"
//lint -sem( QList::operator<<, custodial(1) )

namespace LogViewer {

quint8 CLogFilter::m_AllTypes = (1<<(int)Global::EVTTYPE_UNDEFINED) +
        (1<<(int)Global::EVTTYPE_DEBUG) +
        (1<<(int)Global::EVTTYPE_INFO) +
        (1<<(int)Global::EVTTYPE_WARNING) +
        (1<<(int)Global::EVTTYPE_ERROR) +
        (1<<(int)Global::EVTTYPE_FATAL_ERROR);

CLogFilter::CLogFilter(const QString& Filename, const QList<qint32>& Columns, bool NeedClassify):m_LogIndex(0)
{
    m_Filename = Filename;
    m_Columns = Columns;
    m_needClassify = NeedClassify;
}

CLogFilter::~CLogFilter()
{
    try {
        m_Columns.clear();
        ReleaseModelItem();
        m_Model.clear();
        m_SubModel.clear();
        m_LogItems.clear();
    }
    catch (...) {

    }
}

void CLogFilter::ReleaseModelItem()
{
    QList<QStandardItem *> list;
    for(int i=0; i<m_Model.rowCount(); i++) {
        list = m_Model.takeRow(i);
        for(int j=0; j<list.size(); j++) {
            if (list[j]) {
                delete list[j];
            }
        }
        list.clear();
    }
}

void CLogFilter::AddItem4Log(QString &data)
{
    QStringList List = data.split(";");
    QStringList DateTime = List.at(0).split(" ");

    if (!data.contains(";") || DateTime.size() != 2)
        return;

    QList<QStandardItem *> ItemList;
    ItemList << new QStandardItem(DateTime.at(0));
    ItemList << new QStandardItem(DateTime.at(1));

    for(int i=1; i<m_Columns.size(); i++) {
        if(m_Columns.at(i)<List.size()) {
            ItemList << new QStandardItem(List.at(m_Columns.at(i)));
        }
        else {
            break;
        }
    }
    m_Model.appendRow(ItemList);
}

void CLogFilter::AddItem4SwUpdateLog(QString &data)
{
    data.replace(QString("\n"), QString(" "))
           .replace(QString(", "), QString(";"))
           .replace(QString(","), QString(";"));
    if (data.size() < 10 || data[10] != ';')
        return;
    data[10] = ' ';
    AddItem4Log(data);
}

void CLogFilter::AddItem4LogNeedClassify(QString &data)
{
    QStringList List = data.split(";");
    QStringList DateTime = List.at(0).split(" ");
    QStringList ItemList;

    if (!data.contains(";") || DateTime.size() != 2)
        return;

    ItemList.append(DateTime.at(0));
    ItemList.append(DateTime.at(1));
    for(int i=1; i<m_Columns.size(); i++) {
        if(m_Columns.at(i)<List.size()) {
            ItemList.append(List.at(m_Columns.at(i)));
        }
        else {
            break;
        }
    }
    m_LogItems.append(ItemList);

    QString Type = List.at(2);
    if (Type.compare("Undefined Type")==0) {
        (void) m_SubItems[Global::EVTTYPE_UNDEFINED].insert(m_LogIndex, m_LogIndex);
    }
    else if (Type.compare("Debug") == 0) {
        (void) m_SubItems[Global::EVTTYPE_DEBUG].insert(m_LogIndex, m_LogIndex);
    }
    else if (Type.compare("Error") == 0) {
        (void) m_SubItems[Global::EVTTYPE_ERROR].insert(m_LogIndex, m_LogIndex);
    }
    else if (Type.compare("Info") == 0) {
        (void) m_SubItems[Global::EVTTYPE_INFO].insert(m_LogIndex, m_LogIndex);
    }
    else if (Type.compare("Warning") == 0) {
        (void) m_SubItems[Global::EVTTYPE_WARNING].insert(m_LogIndex, m_LogIndex);
    }
    else if (Type.compare("Fatal Error") == 0) {
        (void) m_SubItems[Global::EVTTYPE_FATAL_ERROR].insert(m_LogIndex, m_LogIndex);
    }
    m_LogIndex++;
}

void CLogFilter::AddItem4ServiceHelpText(QString &data)
{
    QStringList List = data.split(";");
    QList<QStandardItem *> ItemList;

    if (!data.contains(";"))
        return;

    for(int i=0; i<m_Columns.size(); i++) {
        ItemList << new QStandardItem(List.at(m_Columns.at(i)));
    }
    m_Model.appendRow(ItemList);
}

bool CLogFilter::CheckFileInfo(const QString &line)
{
    if (m_Filename.contains("ServiceHelpText")) {
        if (line.startsWith("Version") ||
                line.startsWith("FileName") ||
                line.startsWith("System")) {
            QStringList list = line.split(":");
            (void) m_FileInfo.insert(list.at(0), list.at(1));
            return true;
        }
    } else if (m_Filename.contains("SW_Update_Events")) {
        if (line.startsWith("Format Version")
                || line.startsWith("Component")
                || line.startsWith("BaseEventID")
                || line.startsWith("TimeStamp")) {
            QStringList list = line.split(":");
            (void) m_FileInfo.insert(list.at(0), list.at(1));
            return true;
        }
    } else {
        if (line.startsWith("Format Version") ||
                line.startsWith("FileName") ||
                line.startsWith("OperatingMode") ||
                line.startsWith("Serial Number")) {
            QStringList list = line.split(":");
            (void) m_FileInfo.insert(list.at(0), list.at(1));
            return true;
        }
    }

    return false;
}

bool CLogFilter::InitData()
{
    bool skipHeader = false;
    ReleaseModelItem();
    m_Model.clear();
    QFile File(m_Filename);

    if (!File.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    void (CLogFilter::*addItem)(QString &);

    if (m_Filename.contains("ServiceHelpText"))
        addItem = &CLogFilter::AddItem4ServiceHelpText;
    else if (m_Filename.contains("SW_Update_Events"))
        addItem = &CLogFilter::AddItem4SwUpdateLog;
    else
        addItem = &CLogFilter::AddItem4Log;

    // system log
    if (m_needClassify)
        addItem = &CLogFilter::AddItem4LogNeedClassify;

    QTextStream Text(&File);
    while (!Text.atEnd()) {
        QString data = Text.readLine().replace(QString("\\n"), QString(" "));
        // header
        if (skipHeader == false) {
            if (CheckFileInfo(data))
                continue;
            else
                skipHeader = true;
        }
        // entry
        (this->*addItem)(data);
    }
    File.close();

    return true;
}

QStandardItemModel* CLogFilter::GetItemModel(quint64 EventTypes)
{
    if (m_needClassify == false)
        return &m_Model;

    ReleaseModelItem();
    m_Model.clear();

    if (EventTypes == m_AllTypes) {
        for(int i=0; i<m_LogItems.size(); i++) {
            QList<QStandardItem *> ItemList;
            for(int j=0; j<m_LogItems[i].size(); j++) {
                QStandardItem* Item = new QStandardItem(m_LogItems[i].at(j));
                ItemList << Item;
            }
            m_Model.appendRow(ItemList);
        }
    }
    else
    {
        QMap<int, int> Numbers;

        // get index list for event types
        for (int i=(int)Global::EVTTYPE_UNDEFINED; i<=(int)Global::EVTTYPE_FATAL_ERROR; i++) {
            if ((int)EventTypes&(1<<i)) {
                if (Numbers.size()==0) {
                    Numbers = m_SubItems[i];
                }
                else {
                    QList<int> SubList = m_SubItems[i].values();
                    for(int a=0; a<SubList.size(); a++) {
                        (void) Numbers.insert(SubList.at(a), SubList.at(a));
                    }
                }
            }
        }

        QList<int> IndexList = Numbers.values();
        for(int j=0; j<IndexList.size(); j++) {
            QList<QStandardItem *> ItemList;
            QStringList LogItem = m_LogItems[IndexList.at(j)];

            for(int k=0; k<LogItem.size(); k++) {
                QStandardItem* Item = new QStandardItem(LogItem.at(k));
                ItemList << Item;
            }
            m_Model.appendRow(ItemList);
        }

        Numbers.clear();
        IndexList.clear();
    }

    return &m_Model;
}

const QMap<QString, QString>& CLogFilter::GetFileInfo()
{
    return m_FileInfo;
}

}

