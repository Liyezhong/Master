/****************************************************************************/
/*! \file DlgWizard3Buttons.h
 *
 *  \brief Declaration of wizard dialog with 3 buttons.
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

#ifndef MAINMENU_DLGWIZARD3BUTTONS_H
#define MAINMENU_DLGWIZARD3BUTTONS_H

#include <QPushButton>

#include "ServiceWidget/Include/DlgWizardText.h"

namespace MainMenu {

/****************************************************************************/
/**
 * \brief This class for wizard buttons dialog
 */
/****************************************************************************/
class CDlgWizard3Buttons : public CDlgWizardText
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief  Constructor
     *  \iparam p_Parent = the parent widget
     */
    /****************************************************************************/
    explicit CDlgWizard3Buttons(QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    ~CDlgWizard3Buttons(void);

public Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief Set button text
     *  \iparam Text = button text
     */
    /****************************************************************************/
    void SetButtonText(const QString &Text);

Q_SIGNALS:
    /****************************************************************************/
    /*!
     *  \brief Signal emit for button selected
     */
    /****************************************************************************/
    void ThirdSelected(void);

private Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief Slot for button selected
     */
    /****************************************************************************/
    void ThirdSelect(void);

private:
    QPushButton *mp_PushThird;  //!< push button object
};

} // namespace MainMenu

#endif // MAINMENU_DLGWIZARD3BUTTONS_H
