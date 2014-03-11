/****************************************************************************/
/*! \file SystemLogViewer.h
 *
 *  \brief Definition file for class CSystemLogViewer.
 *  This class is implemented to read Himalaya Log files.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-01-16
 *   $Author:  $ Soumya. D
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

#ifndef LOGVIEWER_SYSTEMLOGVIEWER_H
#define LOGVIEWER_SYSTEMLOGVIEWER_H

#include "MainMenu/Include/BaseTable.h"
#include <QWidget>
#include <QStandardItemModel>
#include "LogViewer/Include/SystemLogViewerDlg.h"
#include "MainMenu/Include/MessageDlg.h"
#include <QDir>
#include <QFileInfoList>

namespace LogViewer {

namespace Ui {
    class CSystemLogViewer;
}

/****************************************************************************/
/**
 * \brief This is implements SystemLogViewer
 */
/****************************************************************************/
class CSystemLogViewer : public QWidget
{
    Q_OBJECT

public:
    explicit CSystemLogViewer(QWidget *p_Parent = 0);
    ~CSystemLogViewer();

    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam LogFileName = System log file name
     */
    /****************************************************************************/
    void AddItem(QString LogFileName);

private:
    Ui::CSystemLogViewer *mp_Ui;                                //!< User Interface
    MainMenu::CBaseTable *mp_TableWidget;                       //!< Table for the system log files
    QStandardItemModel m_Model;                                 //!< Model for the System log table
    int m_CurrentIndex;                                         //!< Current Selected Index
    LogViewer::CSystemLogViewerDlg *mp_SystemLogViewerDlg;      //!< System log viewer dialog
    QVariant m_LogFileName;                                     //!< Stores system log file name
    MainMenu::CMessageDlg *mp_MessageDlg;                       //!< Information Message dialog

public slots:
    void SelectionChanged(QModelIndex Index);
    void ExecDialog();    
};

} // end namespace LogViewer

#endif // LOGVIEWER_SYSTEMLOGVIEWER_H
