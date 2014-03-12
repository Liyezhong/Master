/****************************************************************************/
/*! \file FileListModel.h
 *
 *  \brief Implementation of CFileListModel.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-29
 *   $Author:  $ R.Wu
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Diagnostics/Include/System/FileListModel.h"

#include <QDebug>


namespace Diagnostics {

namespace System {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CFileListModel::CFileListModel(QObject *p_Parent)
    : QAbstractTableModel(p_Parent)
    , m_FileList()
    , m_VisibleRows(1)
{
}

/****************************************************************************/
/*!
 *  \brief Returns the number of rows in the table
 *
 *  \return Row count
 */
/****************************************************************************/
int CFileListModel::rowCount(const QModelIndex &) const
{
    return (m_FileList.count() < m_VisibleRows) ?
                m_VisibleRows : m_FileList.count();
}

/****************************************************************************/
/*!
 *  \brief Returns the number of columns in the table
 *
 *  \return Columns count
 */
/****************************************************************************/
int CFileListModel::columnCount(const QModelIndex &) const
{
    return 1;
}

/****************************************************************************/
/*!
 *  \brief Returns the data items displayed in the table
 *
 *  \iparam Index = Index of a table cell
 *  \iparam Role = Display role to be returned
 *
 *  \return Data item
 */
/****************************************************************************/
QVariant CFileListModel::data(const QModelIndex &Index, int Role) const
{
    if (!Index.isValid())
    {
        return QVariant();
    }

    if (Qt::DisplayRole == Role)
    {
        int Row = Index.row();
        if ((0 <= Row) && (Row < m_FileList.count()))
        {
            return m_FileList.at(Row);
        }
    }

    return QVariant();
}

/****************************************************************************/
/*!
 *  \brief Returns the caption of the column headers
 *
 *  \iparam Section = Row or column number
 *  \iparam Orientation = Horizontal or vertical header
 *  \iparam Role = Display role to be returned
 *
 *  \return Header data
 */
/****************************************************************************/
QVariant CFileListModel::headerData(int /* Section */,
                                    Qt::Orientation Orientation,
                                    int Role) const
{
    if ((Qt::DisplayRole == Role) && (Qt::Horizontal == Orientation))
    {
        return tr("File Name");
    }

    return QVariant();
}

void CFileListModel::SetVisibleRows(int VisibleRows)
{
    this->beginResetModel();
    m_VisibleRows = VisibleRows;
    this->endResetModel();;
}

void CFileListModel::SetFileList(const QStringList &FileList)
{
    this->beginResetModel();
    m_FileList = FileList;
    this->endResetModel();
}

} // namespace System

} // namespace Diagnostics
