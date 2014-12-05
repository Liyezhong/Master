/****************************************************************************/
/*! \file SVCValveStateInfoDlg.cpp
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
    explicit CSVCValveStateInfoDlg(bool PumpState, bool Valve1State, bool Valve2State, QWidget* p_Parent);

    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    ~CSVCValveStateInfoDlg();

    //void SetState(bool PumpState, bool Valve1State, bool Valve2State);

//protected:
//    /****************************************************************************/
//    /*!
//     *  \brief  Definition/Declaration of function changeEvent
//     *  \param p_Object = The event object
//     *  \param p_Event = QEvent type parameter
//     *  \return ture or false
//     */
//    /****************************************************************************/
//    bool eventFilter(QObject *p_Object, QEvent *p_Event);

private:
    /****************************************************************************/
    /*!
     *  \brief  To add data item to the table
     *  \iparam State = Valve or pump state
     *  \iparam Name  = part name
     */
    /****************************************************************************/
    void AddItem(bool State, const QString& Name);

private:
    Ui::CSVCValveStateInfoDlg* mp_Ui;    //!< User interface
    QStandardItemModel m_Model;          //!< Model for the table
    MainMenu::CBaseTable *mp_TableWidget;//!< Base Table widget
};
}//end of namespace SVCDiagnostics

#endif // SVCVALVESTATEINFODLG_H
