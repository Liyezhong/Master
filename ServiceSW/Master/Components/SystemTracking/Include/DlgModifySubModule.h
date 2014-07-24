/****************************************************************************/
/*! \file DlgModifySubModule.h
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

#ifndef SYSTEMTRACKING_DLGMODIFYSUBMODULE_H
#define SYSTEMTRACKING_DLGMODIFYSUBMODULE_H

#include "MainMenu/Include/DialogFrame.h"

#include "MainMenu/Include/ScrollWheel.h"
#include "MainMenu/Include/MessageDlg.h"

#include "KeyBoard/Include/KeyBoard.h"

#include "ServiceDataManager/Include/SubModule.h"

namespace SystemTracking {

namespace Ui {
class CDlgModifyModule;
}
/****************************************************************************/
/**
 * \brief This class provides facility to add/modify submodule configuration
 */
/****************************************************************************/
class CDlgModifySubModule : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    /****************************************************************************/
    /**
     * \brief  Default constructor.
     * \iparam SubModule = Reference of SubModule object
     * \iparam HasBoard = flag of Board
     * \iparam p_Parent = parent of widget
     */
    /****************************************************************************/
    explicit CDlgModifySubModule(ServiceDataManager::CSubModule &SubModule,
                                 bool HasBoard = false,
                                 QWidget *p_Parent = 0);

    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    ~CDlgModifySubModule();

Q_SIGNALS:
    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when a program is updated.
     *  \iparam SubModule = Modified SubModule.
     */
    /****************************************************************************/
    void UpdateSubModule(ServiceDataManager::CSubModule &SubModule);

    /****************************************************************************/
    /*!
     *  \brief This signal is emitted when clicked Auto detect button .
     *  \iparam SubModule = Modified SubModule.
     */
    /****************************************************************************/
    void AutoDetect(ServiceDataManager::CSubModule &SubModule);

public Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief Slot for updating the GUI
     */
    /****************************************************************************/
    void UpdateGUI(void);

    /****************************************************************************/
    /*!
     *  \brief Slot for updating the GUI
     *  \iparam SerialNumber = Module serial number
     *  \iparam DateOfProduction = the Date of production
     */
    /****************************************************************************/
    void UpdateGUI(const QString &SerialNumber, const QDate &DateOfProduction);

    /****************************************************************************/
    /*!
     *  \brief Slot for Edit serial number
     */
    /****************************************************************************/
    void OnEditSerialNumber(void);

    /****************************************************************************/
    /*!
     *  \brief Slot for auto detect
     */
    /****************************************************************************/
    void OnAutoDetect(void);

    /****************************************************************************/
    /*!
     *  \brief Slot for save button
     */
    /****************************************************************************/
    void OnSave(void);

private Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief Slot for Ok button
     *  \iparam EnteredString = entered string of serial number
     */
    /****************************************************************************/
    void OnOkClicked(QString EnteredString);

    /****************************************************************************/
    /*!
     *  \brief Slot for esc button
     */
    /****************************************************************************/
    void OnESCClicked(void);

private:
    Ui::CDlgModifyModule    *mp_Ui;                  //!< User Interface
    ServiceDataManager::CSubModule *mp_SubModule;           //!< Current sub-module object
    KeyBoard::CKeyBoard     *mp_KeyBoardWidget;      //!< Key board widget
    MainMenu::CMessageDlg   *mp_MessageDlg;          //!< Information dialog

    MainMenu::CScrollWheel  *mp_DayWheel;            //!< Day scroll wheel
    MainMenu::CScrollWheel  *mp_MonthWheel;          //!< Month scroll wheel
    MainMenu::CScrollWheel  *mp_YearWheel;           //!< Year scroll wheel

    /****************************************************************************/
    /*!
     *  \brief Connects signals and slots of keyboard.
     */
    /****************************************************************************/
    void ConnectKeyBoardSignalSlots(void);

    /****************************************************************************/
    /*!
     *  \brief Disconnects signals and slots of keyboard.
     */
    /****************************************************************************/
    void DisconnectKeyBoardSignalSlots(void);


};

} // end namespace SystemTracking

#endif //SYSTEMTRACKING_DLGMODIFYSUBMODULE_H

