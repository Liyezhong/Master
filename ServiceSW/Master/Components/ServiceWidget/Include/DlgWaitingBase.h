/****************************************************************************/
/*! \file DlgWaitingBase.h
 *
 *  \brief Declaration of base class of waiting dialog.
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

#ifndef MAINMENU_DLGWAITINGBASE_H
#define MAINMENU_DLGWAITINGBASE_H

#include "MainMenu/Include/DialogFrame.h"
#include <QTimer>

namespace MainMenu {

namespace Ui {
    class CDlgWaitingBase;
}

/****************************************************************************/
/**
 * \brief This dialog is started, when the user has to wait for the result of
 *        a background process.
 */
/****************************************************************************/
class CDlgWaitingBase : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief  Constructor
     *  \iparam p_Parent = the parent widget
     */
    /****************************************************************************/
    explicit CDlgWaitingBase(QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    virtual ~CDlgWaitingBase();

    /****************************************************************************/
    /*!
     *  \brief Add widget
     *  \iparam pContent = content widget
     */
    /****************************************************************************/
    void AddWidget(QWidget *pContent);

public Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief Sets the text displayed in the wait dialog
     *
     *  \iparam Text = Label text
     */
    /****************************************************************************/
    void SetText(const QString &Text);

    /****************************************************************************/
    /*!
     *  \brief Activates a timeout timer
     *
     *  \iparam Milliseconds = Timeout in milliseconds
     */
    /****************************************************************************/
    void SetTimeout(int MilliSeconds);

    /****************************************************************************/
    /*!
     *  \brief Sets the text of abort button
     *
     *  \iparam Label = Label text
     */
    /****************************************************************************/
    void SetAbortLabel(const QString &Label);

    /****************************************************************************/
    /*!
     *  \brief Show the abort button of the dialog
     */
    /****************************************************************************/
    void ShowAbort(void);

    /****************************************************************************/
    /*!
     *  \brief Hides the abort button of the dialog
     */
    /****************************************************************************/
    void HideAbort(void);

    /****************************************************************************/
    /*!
     *  \brief Enable the abort button of the dialog
     */
    /****************************************************************************/
    void EnableAbort(void);

    /****************************************************************************/
    /*!
     *  \brief Disable the abort button of the dialog
     */
    /****************************************************************************/
    void DisableAbort(void);

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
    Ui::CDlgWaitingBase *ui;     //!< User interface
    QTimer *m_Timer;             //!< Timeout timer

signals:
    /****************************************************************************/
    /*!
     *  \brief Signal emit for time out
     */
    /****************************************************************************/
    void Timeout();
};

} // end namespace MainMenu

#endif // MAINMENU_DLGWAITINGBASE_H
