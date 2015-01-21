/****************************************************************************/
/*! \file DlgConfirmationStatus.h
 *
 *  \brief Declaration of confirmation dialog with status.
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

#ifndef MAINMENU_DLGCONFIRMATIONSTATUS_H
#define MAINMENU_DLGCONFIRMATIONSTATUS_H

#include <QLabel>
#include <QHBoxLayout>
#include <QGroupBox>

#include "ServiceWidget/Include/DlgConfirmationBase.h"

namespace MainMenu {

/****************************************************************************/
/**
 * \brief This class for confirmation status dialog
 */
/****************************************************************************/
class CDlgConfirmationStatus : public CDlgConfirmationBase
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief  Constructor
     *  \iparam p_Parent = the parent widget
     */
    /****************************************************************************/
    explicit CDlgConfirmationStatus(QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    ~CDlgConfirmationStatus(void);

public Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief Set text for dialog
     *  \iparam Text = text string
     */
    /****************************************************************************/
    void SetText(const QString &Text);

    /****************************************************************************/
    /*!
     *  \brief Set status for dialog
     *  \iparam Status = status string
     */
    /****************************************************************************/
    void SetStatus(const QString &Status);

private:
    QLabel      *mp_Text;           //!< store text label
    QGroupBox   *mp_GroupStatus;    //!< status group box
    QHBoxLayout *mp_LayoutStatus;   //!< status layout
    QLabel      *mp_Status;         //!< status label
};

} // namespace MainMenu

#endif // MAINMENU_DLGCONFIRMATIONSTATUS_H
