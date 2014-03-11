/****************************************************************************/
/*! \file ServiceLogViewerDlg.h
 *
 *  \brief Service log viewer dialog
 *  This class implements Service log viewer dialog to display log information.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-14
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

#ifndef LOGVIEWER_SERVICELOGVIEWERDLG_H
#define LOGVIEWER_SERVICELOGVIEWERDLG_H

#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/BaseTable.h"
#include <QStandardItemModel>
#include <QFile>
#include <QTextStream>

namespace LogViewer {

namespace Ui {
    class CServiceLogViewerDlg;
}

/****************************************************************************/
/**
 * \brief This implements the class CServiceLogViewerDlg
 */
/****************************************************************************/
class CServiceLogViewerDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    CServiceLogViewerDlg(QWidget *p_Parent = NULL);
    ~CServiceLogViewerDlg();

    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam Date = Date of the log event
     *  \iparam TimeStamp = Time stamp for log event
     *  \iparam EventID = Event Id
     *  \iparam Type = Log event type
     *  \iparam Desc = Log event description
     */
    /****************************************************************************/
    void AddItem(QString Date, QString TimeStamp, QString EventID, QString Type, QString Desc);

    /****************************************************************************/
    /*!
     *  \brief  To initialize the pop up dialog
     *  \iparam FilePath = Log file path
     *  \return retruns 0, if file open gets failed.
     */
    /****************************************************************************/
    int InitDialog(QString FilePath);


private:
    Ui::CServiceLogViewerDlg *mp_Ui;         //!< User Interface
    MainMenu::CBaseTable *mp_TableWidget;    //!< Table for the service log information
    QStandardItemModel m_Model;              //!< Model for the service log dialog table

};

}   // end namespace LogViewer

#endif // LOGVIEWER_SERVICELOGVIEWERDLG_H
