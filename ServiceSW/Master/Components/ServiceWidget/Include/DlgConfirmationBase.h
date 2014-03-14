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
    explicit CDlgConfirmationBase(QWidget *parent = 0);
    ~CDlgConfirmationBase();

    void AddWidget(QWidget *pContent);
    void RemoveWidget(QWidget *pContent);

public Q_SLOTS:
    void ShowYes(void);

    void ShowNo(void);

    void HideYes(void);

    void HideNo(void);

    void EnableYes(void);

    void EnableNo(void);

    void DisableYes(void);

    void DisableNo(void);

protected:
    void changeEvent(QEvent *p_Event);

private:
    Ui::CDlgConfirmationBase *ui;
};


} // namespace MainMenu
#endif // MAINMENU_DLGCONFIRMATIONBASE_H
