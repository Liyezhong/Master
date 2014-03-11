/****************************************************************************/
/*! \file SoftwareUpdateLog.h
 *
 *  \brief Software Update log information
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

#ifndef LOGVIEWER_SOFTWAREUPDATELOG_H
#define LOGVIEWER_SOFTWAREUPDATELOG_H

#include "MainMenu/Include/BaseTable.h"
#include "LogViewer/Include/SoftwareUpdateLogDlg.h"
#include <QWidget>
#include <QStandardItemModel>

namespace LogViewer {

namespace Ui {
    class CSystemLogViewer;
}

/****************************************************************************/
/**
 * \brief This class implements functions for Software update log file
 */
/****************************************************************************/
class CSoftwareUpdateLog : public QWidget
{
    Q_OBJECT

public:
    explicit CSoftwareUpdateLog(QWidget *p_Parent = 0);
    ~CSoftwareUpdateLog();


    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam LogFileName = Software update log file name
     */
    /****************************************************************************/
    void AddItem(QString LogFileName);

private:
    Ui::CSystemLogViewer *mp_Ui;                            //!< User Interface
    MainMenu::CBaseTable *mp_TableWidget;                   //!< Table for the Software update log files
    LogViewer::CSoftwareUpdateLogDlg *mp_SoftwareUpdateDlg; //!< Software Update Log dialog
    QStandardItemModel m_Model;                             //!< Model for the Software update table
    QString m_SoftwareUpateFilePath;                        //!< Software Update file path

public slots:
    void ExecDialog();

};

} // end namespace LogViewer

#endif // LOGVIEWER_SOFTWAREUPDATELOG_H
