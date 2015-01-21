/****************************************************************************/
/*! \file DlgWizardText.h
 *
 *  \brief Declaration of text wizard dialog.
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

#ifndef MAINMENU_DLGWIZARDTEXT_H
#define MAINMENU_DLGWIZARDTEXT_H

#include <QLabel>

#include "ServiceWidget/Include/DlgWizardBase.h"

namespace MainMenu {

/****************************************************************************/
/**
 * \brief This class for wizard selected test option dialog
 */
/****************************************************************************/
class CDlgWizardText : public CDlgWizardBase
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief  Constructor
     *  \iparam p_Parent = the parent widget
     */
    /****************************************************************************/
    explicit CDlgWizardText(QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    ~CDlgWizardText(void);

public Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief Set label text
     *  \iparam Text = label text
     */
    /****************************************************************************/
    void SetText(const QString &Text);

private:
    QLabel *mp_Text;    //!< text label object
};

} // namespace MainMenu

#endif // MAINMENU_DLGWIZARDTEXT_H
