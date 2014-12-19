/****************************************************************************/
/*! \file SVCValveStateInfoDlg.h
 *
 *  \brief Declaration of Valve state info dialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-12-03
 *   $Author:  $ Dixiong Li
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#ifndef SVCVALVESTATEINFODLG_H
#define SVCVALVESTATEINFODLG_H

#include "MainMenu/Include/DialogFrame.h"
#include <MainMenu/Include/BaseTable.h>
#include <QStandardItemModel>

namespace SVCDiagnostics {

namespace Ui {
    class CSVCValveStateInfoDlg;
}

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of class CSVCValveStateInfoDlg
 */
/****************************************************************************/
class CSVCValveStateInfoDlg : public MainMenu::CDialogFrame
{
    Q_OBJECT
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CSVCValveStateInfoDlg
     *  \iparam p_Parent = parent widget
     */
    /****************************************************************************/
    explicit CSVCValveStateInfoDlg(QWidget* p_Parent);

    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    ~CSVCValveStateInfoDlg();

private:
    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam Name  = part name
     *  \iparam State1 = state of one
     *  \iparam State2 = state of two
     *  \iparam State3 = state of three
     */
    /****************************************************************************/
    void AddItem(const QString& Name, const QString& State1, const QString& State2, const QString& State3);

private:
    Ui::CSVCValveStateInfoDlg* mp_Ui;    //!< User interface
    QStandardItemModel m_Model;          //!< Model for the table
    MainMenu::CBaseTable *mp_TableWidget;//!< Base Table widget
};
}//end of namespace SVCDiagnostics

#endif // SVCVALVESTATEINFODLG_H
