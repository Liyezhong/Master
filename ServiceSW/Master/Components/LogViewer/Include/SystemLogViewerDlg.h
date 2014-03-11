/****************************************************************************/
/*! \file SystemLogViewerDlg.h
 *
 *  \brief System log viewer pop up.
 *  This class implements system log viewer dialog to display system log
 *  information.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-07
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

#ifndef LOGVIEWER_SYSTEMLOGVIEWERDLG_H
#define LOGVIEWER_SYSTEMLOGVIEWERDLG_H

#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/BaseTable.h"
#include "MainMenu/Include/TextDialog.h"
#include "MainMenu/Include/MessageDlg.h"
#include <QStandardItemModel>
#include <QFile>

namespace LogViewer {

//!< Structure for Log information
typedef struct {
    QString Date;           //!< Log date
    QString TimeStamp;      //!< Timestamp for logged event
    QString EventID;        //!< Event Id
    QString Type;           //!< Log type
    QString Desc;           //!< Log description
} LogInfo_t;                //!< Structure variable for LogInfo

typedef QList<LogInfo_t*> LogInformation_t;     //!< List of Log events

namespace Ui {
class CSystemLogViewerDlg;
}

/****************************************************************************/
/**
 * \brief This is implements SystemLogViewer dialog
 */
/****************************************************************************/
class CSystemLogViewerDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    CSystemLogViewerDlg(QWidget *p_Parent = NULL);
    ~CSystemLogViewerDlg();

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
     *  \iparam Path = Log file path
     *  \return retruns 0, if file open gets failed.
     */
    /****************************************************************************/
    int InitDialog(QString Path);

    /****************************************************************************/
    /*!
     *  \brief  To set the Log information
     *  \iparam Date = Date of the log event
     *  \iparam TimeStamp = Time stamp for log event
     *  \iparam EventID = Event Id
     *  \iparam Type = Log event type
     *  \iparam Desc = Log event description
     */
    /****************************************************************************/
    void SetLogInformation(QString Date, QString TimeStamp, QString EventID, QString Type, QString Desc);

private:
    Ui::CSystemLogViewerDlg *mp_Ui;                 //!< User Interface
    MainMenu::CBaseTable *mp_TableWidget;           //!< Table for the system log information
    QStandardItemModel m_Model;                     //!< Model for the System log dialog table
    MainMenu::CTextDialog *mp_RecoveryActionDlg;    //!< Recovery action text dialog
    MainMenu::CMessageDlg *mp_MessageDlg;           //!< Information message dialog
    LogInformation_t m_LogInformation;              //!< QList of log information
    QString m_LogFilePath;                          //!< Stores log file path
    QModelIndexList m_SelectedRowValues;              //!< Model Index list stores selected row values

public slots:
    int RecoveryActionDialog();
    void CompleteLogInfo();
    void FilteredErrorLog();
    void FilteredInfoLog();
    void FilteredUndefinedLog();
    void FilteredWarningLog();
    void FilteredInfoErrorLog();
    void SelectionChanged(QModelIndex Index);
    void ShowRecoveryActionDetails();

};

}   // end namespace LogViewer

#endif // LOGVIEWER_SYSTEMLOGVIEWERDLG_H
