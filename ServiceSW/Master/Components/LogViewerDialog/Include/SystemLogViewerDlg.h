/****************************************************************************/
/*! \file SystemLogViewerDlg.h
 *
 *  \brief System log viewer pop up.
 *  This class implements system log viewer dialog to display system log
 *  information.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-04-19
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

#ifndef LOGVIEWER_SYSTEMLOGVIEWERDLG_H
#define LOGVIEWER_SYSTEMLOGVIEWERDLG_H

#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/BaseTable.h"
#include "MainMenu/Include/TextDialog.h"
#include "MainMenu/Include/MessageDlg.h"
#include <QStandardItemModel>
#include <QFile>
#include "LogViewerDialog/Include/LogFilter.h"
#include "LogViewerDialog/Include/ServiceHelpTextFilter.h"

namespace LogViewer {

namespace Ui {
class CSystemLogViewerDlg;
}

/****************************************************************************/
/**
 * \brief This is implements SystemLogViewer dialog
 */
/****************************************************************************/
class  CSystemLogViewerDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *
     *  \iparam p_Parent = Parent widget
     */
    /****************************************************************************/
    CSystemLogViewerDlg(QWidget *p_Parent = NULL);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CSystemLogViewerDlg();

    /****************************************************************************/
    /*!
     *  \brief  To initialize the pop up dialog
     *  \iparam Path = Log file path
     *  \return retruns false, if file open gets failed.
     */
    /****************************************************************************/
    bool InitDialog(QString Path);

protected:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function changeEvent
     *
     *  \param p_Event = QEvent type parameter
     *
     *  \return from changeEvent
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);

private:
    Ui::CSystemLogViewerDlg *mp_Ui;                 //!< User Interface
    MainMenu::CBaseTable *mp_TableWidget;           //!< Table for the system log information
    QStandardItemModel *mp_Model;                     //!< Model for the System log dialog table
    MainMenu::CTextDialog *mp_ServiceHelpTextDlg;    //!< Service help text text dialog
    MainMenu::CMessageDlg *mp_MessageDlg;           //!< Information message dialog
    QString m_LogFilePath;                          //!< Stores log file path
    QModelIndexList m_SelectedRowValues;              //!< Model Index list stores selected row values
    CLogFilter* mp_LogFilter;                       //!< Log filter to get model list.
    quint8 m_EventTypes;                           //!< Event Types of previously diaplayed.
    CServiceHelpTextFilter *mp_ServiceHelpTextFilter; //!< Service Help Text Filter to get Service Help Text
    QHash<QString, qint64> m_PositionItems;
    QString m_strOK;
    QString m_strWarning;
    QString m_strMsgSelectRowText;
    QString m_strMsgServiceHelpTextTitle;
    QString m_strMsgServiceHelpTextText;
    QString m_strErrorCode;
    QString m_strDescription;
    QString m_strServiceHelpText;
    QString m_strServiceHelpTextTitle;
    QString m_strMsgServerHelpTitle;
    QString m_strMsgServerHelpText;
    QString m_strDate;
    QString m_strTimeStamp;
    QString m_strEventID;
    QString m_strType;
    QString m_strServiceHelpTextInfoTitle;

private:
    /****************************************************************************/
    /*!
     *  \brief Reset Buttons
     *  \iparam EnableFlag = true:enable, false:disable
     */
    /****************************************************************************/
    void ResetButtons(bool EnableFlag);

    /****************************************************************************/
    /*!
     *  \brief  Set Header Labels for TableWidget
     */
    /****************************************************************************/
    void SetTableModel();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function RetranslateUI
     *
     *  \return from RetranslateUI
     */
    /****************************************************************************/
    void RetranslateUI();

public slots:
    /****************************************************************************/
    /*!
     *  \brief  To initialize the pop up dialog
     */
    /****************************************************************************/
    void ServiceHelpTextDialog();

    /****************************************************************************/
    /*!
     *  \brief To display complete log information
     */
    /****************************************************************************/
    void CompleteLogInfo();

    /****************************************************************************/
    /*!
     *  \brief To display filtered log information for event type Error
     */
    /****************************************************************************/
    void FilteredErrorLog();

    /****************************************************************************/
    /*!
     *  \brief To display filtered log information for event type Info
     */
    /****************************************************************************/
    void FilteredInfoLog();

    /****************************************************************************/
    /*!
     *  \brief To display filtered log information for event type Undefined
     */
    /****************************************************************************/
    void FilteredUndefinedLog();

    /****************************************************************************/
    /*!
     *  \brief To display filtered log information for event type Warning
     */
    /****************************************************************************/
    void FilteredWarningLog();

    /****************************************************************************/
    /*!
     *  \brief To get the index of selected row from the table
     *  \iparam Index = Index of the row
     */
    /****************************************************************************/
    void SelectionChanged(QModelIndex Index);

    /****************************************************************************/
    /*!
     *  \brief  To display selected EventID information
     */
    /****************************************************************************/
    void ShowServiceHelpTextDetails();

};

}   // end namespace LogViewer

#endif // LOGVIEWER_SYSTEMLOGVIEWERDLG_H
