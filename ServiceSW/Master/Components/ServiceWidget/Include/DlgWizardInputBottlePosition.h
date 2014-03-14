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

class CDlgWizardInputBottlePosition : public CDlgWizardBase
{
    Q_OBJECT

public:
    explicit CDlgWizardInputBottlePosition(QWidget *parent = 0);
    ~CDlgWizardInputBottlePosition(void);

public Q_SLOTS:
    void SetText(const QString &Text);

    void SetButtonLabel(const QString &Label);

Q_SIGNALS:
    void AcceptPosition(qint32);

private Q_SLOTS:
    void OpenKeyBoard(void);
    void InputPosition(QString position);
    void EmitPosition(void);

private:
    QLabel      *mp_Text;
    QGroupBox   *mp_GroupInput;
    QHBoxLayout *mp_LayoutInput;
    QLabel      *mp_ButtonLabel;
	QPushButton *mp_ButtonInput;
    qint32       m_Position;

    KeyBoard::CKeyBoard *mp_KeyBoard;
};

} // namespace MainMenu

#endif // MAINMENU_DLGWIZARDINPUTBOTTLEPOSITION_H
