/****************************************************************************/
/*! \file ServiceLogViewer.h
 *
 *  \brief Service log information
 *  This class implements the functions to display Service log information.
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

#ifndef LOGVIEWER_SERVICELOGVIEWER_H
#define LOGVIEWER_SERVICELOGVIEWER_H

#include "MainMenu/Include/BaseTable.h"
#include <QWidget>
#include <QStandardItemModel>
#include "LogViewer/Include/ServiceLogViewerDlg.h"
#include "MainMenu/Include/MessageDlg.h"
#include <QFile>
#include <QFileInfoList>

namespace LogViewer {

namespace Ui {
    class CSystemLogViewer;
}

/****************************************************************************/
/**
 * \brief This implements the class CServiceLogViewer
 */
/****************************************************************************/
class CServiceLogViewer : public QWidget
{
    Q_OBJECT

public:
    explicit CServiceLogViewer(QWidget *p_Parent = 0);
    ~CServiceLogViewer();

    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam ServiceLogFileName = Service log file name
     */
    /****************************************************************************/
    void AddItem(QString ServiceLogFileName);

private:
    Ui::CSystemLogViewer *mp_Ui;                                //!< User Interface
    MainMenu::CBaseTable *mp_TableWidget;                       //!< Table for the service log files
    QStandardItemModel m_Model;                                 //!< Model for the table
    LogViewer::CServiceLogViewerDlg *mp_ServiceLogViewerDlg;    //!< Model for the Service log viewer dialog
    QVariant m_ServiceLogFilePath;                              //!< Stores Service log file path
    MainMenu::CMessageDlg *mp_MessageDlg;                       //!< Information message dialog

public slots:
    void SelectionChanged(QModelIndex Index);
    void ExecDialog();

};

} // end namespace LogViewer

#endif // LOGVIEWER_SERVICELOGVIEWER_H
