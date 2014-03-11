/****************************************************************************/
/*! \file RecoveryAction.h
 *
 *  \brief Recovery action - provides help pages.
 *  This class displays recovery action help page files.
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

#ifndef LOGVIEWER_RECOVERYACTION_H
#define LOGVIEWER_RECOVERYACTION_H

#include "Global/Include/SystemPaths.h"
#include "MainMenu/Include/BaseTable.h"
#include <QStandardItemModel>
#include <QWidget>
#include "LogViewer/Include/RecoveryActionDlg.h"

namespace LogViewer {

namespace Ui {
    class CSystemLogViewer;
}

/****************************************************************************/
/**
 * \brief This class implements RecoveryAction - Provides help pages.
 */
/****************************************************************************/
class CRecoveryAction : public QWidget
{
    Q_OBJECT

public:
    explicit CRecoveryAction(QWidget *p_Parent = 0);
    ~CRecoveryAction();

    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam LogFileName = Recovery Action file name
     */
    /****************************************************************************/
    void AddItem(QString LogFileName);

private:
    Ui::CSystemLogViewer *mp_Ui;                            //!< User Interface
    MainMenu::CBaseTable *mp_TableWidget;                   //!< Table for recovery action files
    QStandardItemModel m_Model;                             //!< Model for the recovery action table
    LogViewer::CRecoveryActionDlg *mp_RecoveryActionDlg;    //!< Recovery Action dialog
    QString m_TroubleShootInfoFile;                         //!< Stores recovery action file name

public slots:
    void ExecDialog();
};

} // end namespace LogViewer

#endif // LOGVIEWER_RECOVERYACTION_H
