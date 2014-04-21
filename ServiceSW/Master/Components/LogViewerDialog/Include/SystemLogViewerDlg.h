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
#include "LogViewerDialog/Include/LogFilter.h"
#include "LogViewerDialog/Include/RecoveryActionFilter.h"

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
     *  \brief  To initialize the pop up dialog
     *  \iparam Path = Log file path
     *  \return retruns 0, if file open gets failed.
     */
    /****************************************************************************/
    int InitDialog(QString Path);

private:
    Ui::CSystemLogViewerDlg *mp_Ui;                 //!< User Interface
    MainMenu::CBaseTable *mp_TableWidget;           //!< Table for the system log information
    QStandardItemModel *mp_Model;                     //!< Model for the System log dialog table
    MainMenu::CTextDialog *mp_RecoveryActionDlg;    //!< Recovery action text dialog
    MainMenu::CMessageDlg *mp_MessageDlg;           //!< Information message dialog
    LogInformation_t m_LogInformation;              //!< QList of log information
    QString m_LogFilePath;                          //!< Stores log file path
    QModelIndexList m_SelectedRowValues;              //!< Model Index list stores selected row values
    CLogFilter* mp_LogFilter;                       //!< Log filter to get model list.
    quint8 m_EventTypes;                           //!< Event Types of previously diaplayed.
    CRecoveryActionFilter *mp_RecoveryActionFilter; //!< Recovery Action Filter to get Recovery Action Text
    QHash<QString, qint64> m_PositionItems;

private:
    /****************************************************************************/
    /*!
     *  \brief  Reset Enable Status for Buttons.
     *  \iparam EnableFlag = Enable Status
     */
    /****************************************************************************/
    void ResetButtons(bool EnableFlag);

    /****************************************************************************/
    /*!
     *  \brief  Set Header Labels for TableWidget
     */
    /****************************************************************************/
    void SetTableModel();

public slots:
    int RecoveryActionDialog();
    void CompleteLogInfo();
    void FilteredErrorLog();
    void FilteredInfoLog();
    void FilteredUndefinedLog();
    void FilteredWarningLog();
    void SelectionChanged(QModelIndex Index);
    void ShowRecoveryActionDetails();

};

}   // end namespace LogViewer

#endif // LOGVIEWER_SYSTEMLOGVIEWERDLG_H
