/****************************************************************************/
/*! \file LogContentDlg.h
 *
 *  \brief Log Content dialog
 *  This class implements common log content dialog to display detail information for a log file.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-04-14
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

#ifndef LOGVIEWER_LOGCONTENTDLG_H
#define LOGVIEWER_LOGCONTENTDLG_H

#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/BaseTable.h"
#include <QStandardItemModel>
#include <QFile>
#include <QTextStream>
#include <QItemDelegate>
#include "LogViewerDialog/Include/LogFilter.h"

namespace LogViewer {

namespace Ui {
    class CLogContentDlg;
}

class MyItemDelegate:public QItemDelegate
{
public:
    MyItemDelegate(QObject *parent=0);
    void paint(QPainter * painter,
           const QStyleOptionViewItem & option,
           const QModelIndex & index) const;
};

/****************************************************************************/
/**
 * \brief This implements the class CLogContentDlg
 */
/****************************************************************************/
class CLogContentDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam HeaderLabels = The header labels of the table widget
     *  \iparam Columns = Columns for the table widget
     *  \iparam p_Parent = Parent widget
     */
    /****************************************************************************/
    CLogContentDlg(const QStringList& HeaderLabels, QList<int>& Columns, QWidget *p_Parent = NULL);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CLogContentDlg();


    /****************************************************************************/
    /*!
     *  \brief  To initialize the pop up dialog
     *  \iparam FilePath = Log file path
     *  \return retruns 0, if file open gets failed.
     */
    /****************************************************************************/
    int InitDialog(QString FilePath);

private:
    Ui::CLogContentDlg *mp_Ui;         //!< User Interface
    MainMenu::CBaseTable *mp_TableWidget;    //!< Table for the log content information
    QStandardItemModel *mp_Model;              //!< Model for the log content dialog table
    CLogFilter *mp_LogFilter;          //!< Log Filter to get the models for table widget.
    QStringList m_HeaderLabels;         //!< Header tables for the table widget
    QList<int> m_Columns;              //!< Columns for the table widget

    MyItemDelegate *mp_MyDelegate;
};


}   // end namespace LogViewer

#endif // LOGVIEWER_LOGCONTENTDLG_H
