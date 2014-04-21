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
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
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
#include "LogViewerDialog/Include/LogFilter.h"

namespace LogViewer {

namespace Ui {
    class CLogContentDlg;
}

/****************************************************************************/
/**
 * \brief This implements the class CLogContentDlg
 */
/****************************************************************************/
class CLogContentDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    CLogContentDlg(const QStringList& HeaderLabels, QList<int>& Columns, QWidget *p_Parent = NULL);
    ~CLogContentDlg();


    /****************************************************************************/
    /*!
     *  \brief  To initialize the pop up dialog
     *  \iparam FilePath = Log file path
     *  \return retruns 0, if file open gets failed.
     */
    /****************************************************************************/
    int InitDialog(QString FilePath);


    void SetTableWidgetSize(qint32 Width, qint32 Height);

private:
    Ui::CLogContentDlg *mp_Ui;         //!< User Interface
    MainMenu::CBaseTable *mp_TableWidget;    //!< Table for the log content information
    QStandardItemModel *mp_Model;              //!< Model for the log content dialog table
    QStringList m_HeaderLabels;         //!< Header tables for the table widget
    QList<int> m_Columns;              //!< Columns for the table widget
    CLogFilter *mp_LogFilter;          //!< Log Filter to get the models for table widget.
};

}   // end namespace LogViewer

#endif // LOGVIEWER_LOGCONTENTDLG_H
