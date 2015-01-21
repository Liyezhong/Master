/****************************************************************************/
/*! \file DlgWaitingStatus.h
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

#ifndef MAINMENU_DLGWAITINGSTATUS_H
#define MAINMENU_DLGWAITINGSTATUS_H

#include <QLabel>
#include <QHBoxLayout>
#include <QGroupBox>

#include "ServiceWidget/Include/DlgWaitingBase.h"

namespace MainMenu {

/****************************************************************************/
/**
 * \brief This class for waiting status dialog
 */
/****************************************************************************/
class CDlgWaitingStatus : public CDlgWaitingBase
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief  Constructor
     *  \iparam p_Parent = the parent widget
     */
    /****************************************************************************/
    explicit CDlgWaitingStatus(QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    ~CDlgWaitingStatus(void);

public Q_SLOTS:

    /****************************************************************************/
    /*!
     *  \brief Slot for set status
     *  \iparam Status = status string.
     */
    /****************************************************************************/
    void SetStatus(const QString &Status);

private:
    QGroupBox   *mp_GroupStatus;    //!< status group box
    QHBoxLayout *mp_LayoutStatus;   //!< status layout
    QLabel      *mp_Status;         //!< Status label
};

} // namespace MainMenu

#endif // MAINMENU_DLGWAITINGSTATUS_H
