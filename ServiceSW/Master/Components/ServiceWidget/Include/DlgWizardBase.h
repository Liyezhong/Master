/****************************************************************************/
/*! \file DlgWizardBase.h
 *
 *  \brief Declaration of base class of wizard dialog.
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

#ifndef MAINMENU_DLGWIZARDBASE_H
#define MAINMENU_DLGWIZARDBASE_H

#include "MainMenu/Include/DialogFrame.h"

namespace MainMenu {

namespace Ui {
class CDlgWizardBase;
}

/****************************************************************************/
/**
 * \brief This class provides a base class for all dialogs which requires
 *        the user to make a choice from  "Next" or "Cancel".
 *        If "Next" is selected, QDialog signal "accepted()" will emit;
 *        If "Cancel" is selected, QDialog signal "rejected()" will emit.
 */
/****************************************************************************/
class CDlgWizardBase : public MainMenu::CDialogFrame
{
    Q_OBJECT
    
public:
    /****************************************************************************/
    /*!
     *  \brief  Constructor
     *  \iparam p_Parent = the parent widget
     *  \iparam p_MainWindow = main window widget
     */
    /****************************************************************************/
    explicit CDlgWizardBase(QWidget *p_Parent = 0, QWidget *p_MainWindow = 0);

    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    ~CDlgWizardBase();

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
     *  \brief Add a third button to the dialog
     *
     *  \iparam pButton = Third button to be added.
     */
    /****************************************************************************/
    void AddButton(QPushButton *pButton);

public Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief Show button "Next"
     *
     */
    /****************************************************************************/
    void ShowNext(void);

    /****************************************************************************/
    /*!
     *  \brief Show button "Cancel"
     *
     */
    /****************************************************************************/
    void ShowCancel(void);

    /****************************************************************************/
    /*!
     *  \brief Hide button "Next"
     *
     */
    /****************************************************************************/
    void HideNext(void);

    /****************************************************************************/
    /*!
     *  \brief Hide button "Cancel"
     *
     */
    /****************************************************************************/
    void HideCancel(void);

    /****************************************************************************/
    /*!
     *  \brief Enable button "Next"
     *
     */
    /****************************************************************************/
    void EnableNext(void);

    /****************************************************************************/
    /*!
     *  \brief Enable button "Cancel"
     *
     */
    /****************************************************************************/
    void EnableCancel(void);

    /****************************************************************************/
    /*!
     *  \brief Disable button "Next"
     *
     */
    /****************************************************************************/
    void DisableNext(void);

    /****************************************************************************/
    /*!
     *  \brief Disable button "Cancel"
     *
     */
    /****************************************************************************/
    void DisableCancel(void);

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
    Ui::CDlgWizardBase *ui; //!< User interface
};


} // namespace MainMenu

#endif // MAINMENU_DLGWIZARDBASE_H
