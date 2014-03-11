/****************************************************************************/
/*! \file FileListModel.h
 *
 *  \brief Declaration of CFileListModel.
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

#ifndef DIAGNOSTICS_SYSTEM_FILELISTMODEL_H
#define DIAGNOSTICS_SYSTEM_FILELISTMODEL_H


#include <QAbstractTableModel>

#include <QStringList>


namespace Diagnostics {

namespace System {

/****************************************************************************/
/**
 * \brief This is the data model for the file list
 */
/****************************************************************************/
class CFileListModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    CFileListModel(QObject *p_Parent = 0);

    int rowCount(const QModelIndex &) const;

    int columnCount(const QModelIndex &) const;

    QVariant data(const QModelIndex &Index, int Role) const;

    QVariant headerData(int Section, Qt::Orientation Orientation, int Role) const;

    void SetVisibleRows(int VisibleRows);

public Q_SLOTS:
    void SetFileList(const QStringList &FileList);

private:
    QStringList m_FileList;
    int         m_VisibleRows;
};

} // namespace System

} // namespace Diagnostics

#endif // DIAGNOSTICS_SYSTEM_FILELISTMODEL_H
