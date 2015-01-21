/****************************************************************************/
/*! \file DlgWizardInputBottlePosition.h
 *
 *  \brief Declaration of input-bottle-position dialog.
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

#ifndef MAINMENU_DLGWIZARDINPUTBOTTLEPOSITION_H
#define MAINMENU_DLGWIZARDINPUTBOTTLEPOSITION_H

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGroupBox>

#include "ServiceWidget/Include/DlgWizardBase.h"

#include "KeyBoard/Include/KeyBoard.h"

namespace MainMenu {

/****************************************************************************/
/**
 * \brief This class for wizard input button position dialog
 */
/****************************************************************************/
class CDlgWizardInputBottlePosition : public CDlgWizardBase
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief  Constructor
     *  \iparam p_Parent = the parent widget
     */
    /****************************************************************************/
    explicit CDlgWizardInputBottlePosition(QWidget *p_Parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Deconstructor
     */
    /****************************************************************************/
    ~CDlgWizardInputBottlePosition(void);

public Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief Set label text
     *  \iparam Text = label text
     */
    /****************************************************************************/
    void SetText(const QString &Text);

    /****************************************************************************/
    /*!
     *  \brief Set button label text
     *  \iparam Label = label text
     */
    /****************************************************************************/
    void SetButtonLabel(const QString &Label);

Q_SIGNALS:
    /****************************************************************************/
    /*!
     *  \brief Signal emit for accept position
     */
    /****************************************************************************/
    void AcceptPosition(qint32);

private Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief Slot for open keyboard
     */
    /****************************************************************************/
    void OpenKeyBoard(void);

    /****************************************************************************/
    /*!
     *  \brief Slot for input position
     *  \iparam position = string position
     */
    /****************************************************************************/
    void InputPosition(QString position);

    /****************************************************************************/
    /*!
     *  \brief Slot for emit position
     */
    /****************************************************************************/
    void EmitPosition(void);

private:
    QLabel      *mp_Text;           //!< label text
    QGroupBox   *mp_GroupInput;     //!< input group box
    QHBoxLayout *mp_LayoutInput;    //!< input layout
    QLabel      *mp_ButtonLabel;    //!< button label
    QPushButton *mp_ButtonInput;    //!< input button object
    qint32       m_Position;        //!< store selected position

    KeyBoard::CKeyBoard *mp_KeyBoard;   //!< keyboard object
};

} // namespace MainMenu

#endif // MAINMENU_DLGWIZARDINPUTBOTTLEPOSITION_H
