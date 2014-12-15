/****************************************************************************/
/*! \file DlgConfirmationBase.h
 *
 *  \brief Declaration of base class of confirmation dialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-28
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

#ifndef MAINMENU_DLGCONFIRMATIONBASE_H
#define MAINMENU_DLGCONFIRMATIONBASE_H

#include "MainMenu/Include/DialogFrame.h"

namespace MainMenu {

namespace Ui {
class CDlgConfirmationBase;
}

/****************************************************************************/
/**
 * \brief This class provides a base class for all dialogs which requires
 *        the user to make a choice from  "Yes" or "No".
 *        If "Yes" is selected, QDialog signal "accepted()" will emit;
 *        If "No" is selected, QDialog signal "rejected()" will emit.
 */
/****************************************************************************/
class CDlgConfirmationBase : public MainMenu::CDialogFrame
{
    Q_OBJECT
    
public:
    /****************************************************************************/
    /*!
     *  \brief  Constructor
     *  \iparam p_Parent = the parent widget
     */
    /****************************************************************************/
    explicit CDlgConfirmationBase(QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    ~CDlgConfirmationBase();

    /****************************************************************************/
    /*!
     *  \brief Add a child widget to the dialog
     *
     *  \iparam pContent = Child widget to be added.
     */
    /****************************************************************************/
    void AddWidget(QWidget *pContent);

    /****************************************************************************/
    /*!
     *  \brief Remove a child widget from the dialog
     *
     *  \iparam pContent = Child widget to be removed.
     */
    /****************************************************************************/
    void RemoveWidget(QWidget *pContent);

public Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief Show button "Yes"
     *
     */
    /****************************************************************************/
    void ShowYes(void);

    /****************************************************************************/
    /*!
     *  \brief Show button "No"
     *
     */
    /****************************************************************************/
    void ShowNo(void);

    /****************************************************************************/
    /*!
     *  \brief Hide button "Yes"
     *
     */
    /****************************************************************************/
    void HideYes(void);

    /****************************************************************************/
    /*!
     *  \brief Hide button "No"
     *
     */
    /****************************************************************************/
    void HideNo(void);

    /****************************************************************************/
    /*!
     *  \brief Enable button "Yes"
     *
     */
    /****************************************************************************/
    void EnableYes(void);

    /****************************************************************************/
    /*!
     *  \brief Enable button "No"
     *
     */
    /****************************************************************************/
    void EnableNo(void);

    /****************************************************************************/
    /*!
     *  \brief Disable button "Yes"
     *
     */
    /****************************************************************************/
    void DisableYes(void);

    /****************************************************************************/
    /*!
     *  \brief Disable button "No"
     *
     */
    /****************************************************************************/
    void DisableNo(void);

protected:
    /****************************************************************************/
    /*!
     *  \brief Event handler for change events
     *
     *  \iparam p_Event = Change event
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);

private:
    Ui::CDlgConfirmationBase *ui;   //!< user interface
};


} // namespace MainMenu
#endif // MAINMENU_DLGCONFIRMATIONBASE_H
