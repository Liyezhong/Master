/****************************************************************************/
/*! \file RecoveryActionDlg.h
 *
 *  \brief recovery action dialog
 *  This calss displays trouble shooting information for the Errors.
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

#ifndef LOGVIEWER_RECOVERYACTIONDLG_H
#define LOGVIEWER_RECOVERYACTIONDLG_H

#include "MainMenu/Include/DialogFrame.h"
#include "MainMenu/Include/BaseTable.h"
#include <QStandardItemModel>
#include <QFile>
#include <QTextStream>

namespace LogViewer {

namespace Ui {
    class CRecoveryActionDlg;
}

/****************************************************************************/
/**
 * \brief This class implements RecoveryAction Dialog
 */
/****************************************************************************/
class CRecoveryActionDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT   

public:
    CRecoveryActionDlg(QWidget *p_Parent = NULL);
    ~CRecoveryActionDlg();

    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam EventID = Log Event ID
     *  \iparam Description = Event description
     *  \iparam RecoveryAction = Recovery action to be taken
     */
    /****************************************************************************/
    void AddItem(QString EventID, QString Description, QString RecoveryAction);


    /****************************************************************************/
    /*!
     *  \brief  To initialize the pop up dialog
     *  \iparam FilePath = Log file path
     *  \return retruns 0, if file open gets failed.
     */
    /****************************************************************************/
    int InitDialog(QString FilePath);

private:
    Ui::CRecoveryActionDlg *mp_Ui;               //!< User Interface
    MainMenu::CBaseTable *mp_TableWidget;        //!< Table for the recovery action dialog files
    QStandardItemModel m_Model;                  //!< Model for the table

};

}   // end namespace LogViewer

#endif // LOGVIEWER_RECOVERYACTIONDLG_H
