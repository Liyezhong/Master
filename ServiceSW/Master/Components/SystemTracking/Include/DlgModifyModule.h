/****************************************************************************/
/*! \file DlgModifyModule.h
 *
 *  \brief Declaration of Modify module dialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-06-06
 *   $Author:  $ R.Wu
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

#ifndef SYSTEMTRACKING_DLGMODIFYMODULE_H
#define SYSTEMTRACKING_DLGMODIFYMODULE_H

#include "MainMenu/Include/DialogFrame.h"

#include "MainMenu/Include/ScrollWheel.h"
#include "MainMenu/Include/MessageDlg.h"

#include "KeyBoard/Include/KeyBoard.h"

#include "DataManager/Containers/InstrumentHistory/Include/Module.h"

namespace SystemTracking {

namespace Ui {
class CDlgModifyModule;
}
/****************************************************************************/
/**
 * \brief This class provides facility to add/modify module configuration
 */
/****************************************************************************/
class CDlgModifyModule : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    explicit CDlgModifyModule(DataManager::CModule &Module,
                              bool HasBoard = false,
                              QWidget *p_Parent = 0);
    ~CDlgModifyModule();

Q_SIGNALS:
    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when a program is updated.
     *  \iparam Module = Modified Module.
     */
    /****************************************************************************/
    void UpdateModule(DataManager::CModule &Module);

    void AutoDetect(DataManager::CModule &Module);

public Q_SLOTS:
    void UpdateGUI(void);

    void UpdateGUI(const QString &SerialNumber, const QDate &DateOfProduction);

    void OnEditSerialNumber(void);

    void OnAutoDetect(void);

    void OnSave(void);

private Q_SLOTS:
    void OnOkClicked(QString EnteredString);

    void OnESCClicked(void);

private:
    Ui::CDlgModifyModule   *mp_Ui;                  //!< User Interface
    DataManager::CModule   *mp_Module;              //!< Current module object
    KeyBoard::CKeyBoard    *mp_KeyBoardWidget;      //!< Key board widget
    MainMenu::CMessageDlg  *mp_MessageDlg;          //!< Information dialog

    MainMenu::CScrollWheel *mp_DayWheel;            //!< Day scroll wheel
    MainMenu::CScrollWheel *mp_MonthWheel;          //!< Month scroll wheel
    MainMenu::CScrollWheel *mp_YearWheel;           //!< Year scroll wheel

    void ConnectKeyBoardSignalSlots(void);

    void DisconnectKeyBoardSignalSlots(void);


};

} // end namespace SystemTracking

#endif //SYSTEMTRACKING_DLGMODIFYMODULE_H

